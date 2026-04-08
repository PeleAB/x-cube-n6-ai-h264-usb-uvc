#!/usr/bin/env python3
"""STM32N6 Model Converter — ONNX/TFLite → C code + HEX for STM32N6.

Native mode pipeline (per model):
  1. Generate memory-pool JSON  (model's flash address → xSPI2)
  2. Generate neural-art profile config JSON
  3. Run  stedgeai generate ... (host binary)
  4. Find .raw output → rename to _data.bin
  5. Run  arm-none-eabi-objcopy -I binary ... --change-addresses ... -O ihex

Docker mode pipeline (per model):
  Same steps 1-2 (files written into the output dir, accessible as /output inside container).
  3. docker run -v stedgeai:/tools/stedgeai:ro  -v output:/output  image
         /tools/stedgeai generate ...
  4. (raw rename handled on host after docker run)
  5. docker run -v output:/output  image
         arm-none-eabi-objcopy ... (objcopy bundled in image, no host toolchain needed)

Flash tab: run STM32_Programmer_CLI natively for each HEX file in order.
"""
from __future__ import annotations  # dict | None union syntax on Python < 3.10

import copy
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from PySide6.QtCore import Qt, QThread, Signal
from PySide6.QtGui import QColor, QFont, QTextCursor, QAction
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QTableWidget, QTableWidgetItem, QPushButton, QLabel, QLineEdit,
    QFileDialog, QDialog, QDialogButtonBox, QFormLayout, QTextEdit,
    QSplitter, QGroupBox, QHeaderView, QMessageBox, QStatusBar,
    QToolBar, QAbstractItemView, QListWidget, QTabWidget, QComboBox,
    QCheckBox,
)

import tool_discovery
from tool_discovery import DOCKER_IMAGE

# ── Paths ─────────────────────────────────────────────────────────────────────
APP_DIR = Path(__file__).parent
CONFIG_PATH = APP_DIR / "model_converter_config.json"

# ── Defaults ──────────────────────────────────────────────────────────────────
# --mvei removed: deprecated in STEdgeAI v4.0 (E102 CliArgumentError).
# MVE usage is now controlled by the target cdesc file, not a CLI flag.
DEFAULT_OPTIONS = (
    "-O0 --all-buffers-info --cache-maintenance "
    "--Oalt-sched --enable-virtual-mem-pools --Omax-ca-pipe 4 "
    "--Ocache-opt --Os --enable-epoch-controller"
)

# Flags that stedgeai no longer accepts in neural-art options.
# They are silently stripped before writing the profile config so that
# old saved model entries continue to work without manual editing.
_DEPRECATED_NA_FLAGS = {
    "--mvei",   # removed in v4.0 — set by cdesc file instead
}

_MPOOL_BASE_POOLS = [
    {"fname": "AXIFLEXMEM", "name": "flexMEM", "fformat": "FORMAT_RAW",
     "prop": {"rights": "ACC_WRITE", "throughput": "MID", "latency": "MID",
              "byteWidth": 8, "freqRatio": 2.50,
              "read_power": 9.381, "write_power": 8.569},
     "offset": {"value": "0x34000000", "magnitude": "BYTES"},
     "size": {"value": "0", "magnitude": "KBYTES"}},
    {"fname": "AXISRAM1", "name": "cpuRAM1", "fformat": "FORMAT_RAW",
     "prop": {"rights": "ACC_WRITE", "throughput": "MID", "latency": "MID",
              "byteWidth": 8, "freqRatio": 2.50,
              "read_power": 16.616, "write_power": 14.522},
     "offset": {"value": "0x34080000", "magnitude": "BYTES"},
     "size": {"value": "0", "magnitude": "KBYTES"}},
    {"fname": "AXISRAM2", "name": "cpuRAM2", "fformat": "FORMAT_RAW",
     "prop": {"rights": "ACC_WRITE", "throughput": "MID", "latency": "MID",
              "byteWidth": 8, "freqRatio": 2.50,
              "read_power": 17.324, "write_power": 15.321},
     "offset": {"value": "0x34100000", "magnitude": "BYTES"},
     "size": {"value": "1024", "magnitude": "KBYTES"}},
    {"fname": "AXISRAM3", "name": "npuRAM3", "fformat": "FORMAT_RAW",
     "prop": {"rights": "ACC_WRITE", "throughput": "HIGH", "latency": "LOW",
              "byteWidth": 8, "freqRatio": 1.25,
              "read_power": 18.531, "write_power": 16.201},
     "offset": {"value": "0x34200000", "magnitude": "BYTES"},
     "size": {"value": "448", "magnitude": "KBYTES"}},
    {"fname": "AXISRAM4", "name": "npuRAM4", "fformat": "FORMAT_RAW",
     "prop": {"rights": "ACC_WRITE", "throughput": "HIGH", "latency": "LOW",
              "byteWidth": 8, "freqRatio": 1.25,
              "read_power": 18.531, "write_power": 16.201},
     "offset": {"value": "0x34270000", "magnitude": "BYTES"},
     "size": {"value": "448", "magnitude": "KBYTES"}},
    {"fname": "AXISRAM5", "name": "npuRAM5", "fformat": "FORMAT_RAW",
     "prop": {"rights": "ACC_WRITE", "throughput": "HIGH", "latency": "LOW",
              "byteWidth": 8, "freqRatio": 1.25,
              "read_power": 18.531, "write_power": 16.201},
     "offset": {"value": "0x342e0000", "magnitude": "BYTES"},
     "size": {"value": "448", "magnitude": "KBYTES"}},
    {"fname": "AXISRAM6", "name": "npuRAM6", "fformat": "FORMAT_RAW",
     "prop": {"rights": "ACC_WRITE", "throughput": "HIGH", "latency": "LOW",
              "byteWidth": 8, "freqRatio": 1.25,
              "read_power": 19.006, "write_power": 15.790},
     "offset": {"value": "0x34350000", "magnitude": "BYTES"},
     "size": {"value": "448", "magnitude": "KBYTES"}},
    {"fname": "xSPI1", "name": "hyperRAM", "fformat": "FORMAT_RAW",
     "prop": {"rights": "ACC_WRITE", "throughput": "MID", "latency": "HIGH",
              "byteWidth": 2, "freqRatio": 5.00, "cacheable": "CACHEABLE_ON",
              "read_power": 380, "write_power": 340.0,
              "constants_preferred": "true"},
     "offset": {"value": "0x90000000", "magnitude": "BYTES"},
     "size": {"value": "16", "magnitude": "MBYTES"}},
]


def _make_mpool(flash_address: str) -> dict:
    pools = copy.deepcopy(_MPOOL_BASE_POOLS)
    pools.append({
        "fname": "xSPI2", "name": "octoFlash", "fformat": "FORMAT_RAW",
        "prop": {"rights": "ACC_READ", "throughput": "MID", "latency": "HIGH",
                 "byteWidth": 1, "freqRatio": 6.00, "cacheable": "CACHEABLE_ON",
                 "read_power": 110, "write_power": 400.0,
                 "constants_preferred": "true"},
        "offset": {"value": flash_address, "magnitude": "BYTES"},
        "size": {"value": "61", "magnitude": "MBYTES"},
    })
    return {
        "params": {"param": [{"paramname": "max_onchip_sram_size",
                               "value": "1024", "magnitude": "KBYTES"}]},
        "memory": {
            "cacheinfo": [{"nlines": 512, "linesize": 64, "associativity": 8,
                           "bypass_enable": 1,
                           "prop": {"rights": "ACC_WRITE", "throughput": "MID",
                                    "latency": "MID", "byteWidth": 8,
                                    "freqRatio": 2.50, "read_power": 13.584,
                                    "write_power": 12.645}}],
            "mem_file_prefix": "atonbuf",
            "mempools": pools,
        },
    }


def _vol(host_path: Path) -> str:
    """Convert a host path to a Docker volume mount source (forward-slash safe on Windows)."""
    return str(host_path.resolve()).replace("\\", "/")


# ── Config ────────────────────────────────────────────────────────────────────
def _default_config() -> dict:
    return {
        "tools": {
            "stedgeai":         "",   # native host binary
            "stedgeai_docker":  "",   # Linux ELF binary to mount in Docker
            "objcopy":          "arm-none-eabi-objcopy",
            "programmer":       "",
            "external_loader":  "",
        },
        "use_docker":    False,
        "output_dir":    str(APP_DIR / "output"),
        "firmware_dir":  "",          # root of firmware repo (contains Model/)
        "models":        [],
        "flash_files":   [],
    }


def load_config() -> dict:
    if CONFIG_PATH.exists():
        try:
            data = json.loads(CONFIG_PATH.read_text(encoding="utf-8"))
            # Ensure any keys added in newer versions are present
            default = _default_config()
            default.update(data)
            default["tools"].update(data.get("tools", {}))
            return default
        except Exception:
            pass
    return _default_config()


def save_config(cfg: dict):
    CONFIG_PATH.write_text(json.dumps(cfg, indent=2), encoding="utf-8")


# ── Worker Threads ────────────────────────────────────────────────────────────

class ConversionThread(QThread):
    log = Signal(str)
    model_status = Signal(str, str)   # name, "running" | "ok" | "error"
    finished_all = Signal(bool)

    def __init__(self, models: list, cfg: dict):
        super().__init__()
        self.models  = models
        self.cfg     = cfg
        self._abort  = False

    def abort(self):
        self._abort = True

    # ── helpers ────────────────────────────────────────────────────────────
    def _run(self, cmd: list, cwd=None) -> bool:
        self.log.emit("$ " + " ".join(str(c) for c in cmd) + "\n")
        try:
            proc = subprocess.Popen(
                [str(c) for c in cmd],
                stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                text=True, encoding="utf-8", errors="replace", cwd=cwd,
            )
            for line in proc.stdout:
                self.log.emit(line)
            proc.wait()
            return proc.returncode == 0
        except FileNotFoundError as e:
            self.log.emit(f"ERROR: tool not found — {e}\n")
            return False
        except Exception as e:
            self.log.emit(f"ERROR: {e}\n")
            return False

    @staticmethod
    def _sanitize_options(raw: str) -> str:
        """Strip deprecated neural-art flags from an options string.

        Handles both bare flags (--mvei) and flags with a following value
        (--some-flag VALUE).  Tokens are rebuilt preserving the original
        spacing so the result is a clean, single-space-separated string.
        """
        tokens = raw.split()
        clean, skip_next = [], False
        for i, tok in enumerate(tokens):
            if skip_next:
                skip_next = False
                continue
            if tok in _DEPRECATED_NA_FLAGS:
                # peek: if the next token is a value (not a flag), drop it too
                if i + 1 < len(tokens) and not tokens[i + 1].startswith("--"):
                    skip_next = True
                continue
            clean.append(tok)
        return " ".join(clean)

    def _write_configs(self, model: dict, output_dir: Path) -> tuple[str, str]:
        """Write mpool + neural-art configs.  Returns (mpool_path, na_cfg_path) as
        *container-internal* paths when in Docker mode, or host paths otherwise."""
        name    = model["name"]
        address = model["address"]
        raw_options = model.get("options", DEFAULT_OPTIONS)
        options = self._sanitize_options(raw_options)
        if options != raw_options:
            stripped = set(raw_options.split()) - set(options.split())
            self.log.emit(
                f"[INFO] Stripped deprecated stedgeai flag(s): {', '.join(stripped)}\n"
                f"       (removed in v4.0 — update your model options to silence this)\n"
            )
        use_docker = self.cfg.get("use_docker", False)

        cfg_dir = output_dir / "_cfg"
        cfg_dir.mkdir(parents=True, exist_ok=True)

        if use_docker:
            # Paths must be valid inside the container (/output/_cfg/...)
            mpool_host = cfg_dir / "mpool.json"
            nacfg_host = cfg_dir / "na_cfg.json"
            mpool_container = f"/output/_cfg/mpool.json"
            nacfg_container = f"/output/_cfg/na_cfg.json"
            mpool_host.write_text(json.dumps(_make_mpool(address), indent="\t"), encoding="utf-8")
            na_cfg = {"Globals": {}, "Profiles": {
                name: {"memory_pool": mpool_container, "options": options}
            }}
            nacfg_host.write_text(json.dumps(na_cfg, indent=2), encoding="utf-8")
            return mpool_container, nacfg_container
        else:
            # Temp files on host; paths are native
            mf = tempfile.NamedTemporaryFile(mode="w", suffix=".mpool",
                                             delete=False, encoding="utf-8")
            json.dump(_make_mpool(address), mf, indent="\t")
            mf.close()
            nf = tempfile.NamedTemporaryFile(mode="w", suffix=".json",
                                             delete=False, encoding="utf-8")
            na_cfg = {"Globals": {}, "Profiles": {
                name: {"memory_pool": mf.name, "options": options}
            }}
            json.dump(na_cfg, nf, indent=2)
            nf.close()
            return mf.name, nf.name

    # ── main loop ──────────────────────────────────────────────────────────
    def run(self):
        overall = True
        for model in self.models:
            if self._abort:
                self.log.emit("\n[ABORTED]\n")
                break
            ok = self._convert_one(model)
            self.model_status.emit(model["name"], "ok" if ok else "error")
            if not ok:
                overall = False
        self.finished_all.emit(overall)

    def _convert_one(self, model: dict) -> bool:
        name       = model["name"]
        model_file = Path(model["file"])
        address    = model["address"]
        input_type = model.get("input_type", "float32")
        use_docker = self.cfg.get("use_docker", False)
        output_dir = Path(self.cfg["output_dir"]) / name
        output_dir.mkdir(parents=True, exist_ok=True)

        self.log.emit(f"\n{'═'*60}\n  Model : {name}\n  Mode  : {'Docker' if use_docker else 'Native'}\n{'═'*60}\n")
        self.model_status.emit(name, "running")

        mpool_path, na_cfg_path = self._write_configs(model, output_dir)
        native_tmp = [] if use_docker else [mpool_path, na_cfg_path]

        try:
            # ── Step 1: stedgeai generate ──────────────────────────────────
            if use_docker:
                ok = self._stedgeai_docker(
                    name, model_file, input_type, na_cfg_path, output_dir
                )
            else:
                stedgeai = self.cfg["tools"]["stedgeai"]
                ok = self._run([
                    stedgeai, "generate",
                    "--name", name,
                    "--model", str(model_file),
                    "--target", "stm32n6",
                    "--st-neural-art", f"{name}@{na_cfg_path}",
                    "--input-data-type", input_type,
                    "--output", str(output_dir),
                ])

            # Verify outputs (non-fatal: stedgeai may exit non-zero on warnings)
            c_files = list(output_dir.glob(f"{name}*.c"))
            h_files = list(output_dir.glob(f"{name}*.h"))
            if not c_files or not h_files:
                self.log.emit(
                    f"\n[ERROR] No C/H files generated for '{name}'.\n"
                    "  Check that stedgeai path is correct and the model is valid.\n"
                )
                return False
            self.log.emit(f"\n[OK] C/H: {[f.name for f in sorted(c_files + h_files)]}\n")

            # ── Step 2: .raw → .bin → .hex ────────────────────────────────
            raw_files = list(output_dir.glob("*.raw"))
            if not raw_files:
                self.log.emit("[WARN] No .raw binary — skipping HEX generation.\n")
                return True

            raw_file = raw_files[0]
            bin_file = output_dir / f"{name}_data.bin"
            hex_file = output_dir / f"{name}_data.hex"
            raw_file.rename(bin_file)
            self.log.emit(f"[OK] Binary : {bin_file.name}\n")

            if use_docker:
                ok_hex = self._objcopy_docker(name, address, output_dir)
            else:
                objcopy = self.cfg["tools"]["objcopy"]
                ok_hex = self._run([
                    objcopy, "-I", "binary", str(bin_file),
                    "--change-addresses", address,
                    "-O", "ihex", str(hex_file),
                ])

            if ok_hex:
                self.log.emit(f"[OK] HEX    : {hex_file.name}  (@ {address})\n")
            else:
                self.log.emit("[WARN] HEX generation failed.\n")

            return True

        finally:
            for p in native_tmp:
                try:
                    os.unlink(p)
                except Exception:
                    pass

    # ── Docker helpers ─────────────────────────────────────────────────────
    def _stedgeai_docker(
        self, name: str, model_file: Path,
        input_type: str, na_cfg_container: str,
        output_dir: Path,
    ) -> bool:
        stedgeai_host = self.cfg["tools"]["stedgeai_docker"]
        return self._run([
            "docker", "run", "--rm",
            "-v", f"{_vol(Path(stedgeai_host))}:/tools/stedgeai:ro",
            "-v", f"{_vol(model_file.parent)}:/input:ro",
            "-v", f"{_vol(output_dir)}:/output",
            DOCKER_IMAGE,
            "/tools/stedgeai", "generate",
            "--name", name,
            "--model", f"/input/{model_file.name}",
            "--target", "stm32n6",
            "--st-neural-art", f"{name}@{na_cfg_container}",
            "--input-data-type", input_type,
            "--output", "/output",
        ])

    def _objcopy_docker(self, name: str, address: str, output_dir: Path) -> bool:
        return self._run([
            "docker", "run", "--rm",
            "-v", f"{_vol(output_dir)}:/output",
            DOCKER_IMAGE,
            "arm-none-eabi-objcopy",
            "-I", "binary", f"/output/{name}_data.bin",
            "--change-addresses", address,
            "-O", "ihex", f"/output/{name}_data.hex",
        ])


class DockerBuildThread(QThread):
    log   = Signal(str)
    done  = Signal(bool)

    def run(self):
        self.log.emit(f"Building Docker image {DOCKER_IMAGE} …\n")
        cmd = ["docker", "build", "-t", DOCKER_IMAGE, str(APP_DIR)]
        self.log.emit("$ " + " ".join(cmd) + "\n")
        try:
            proc = subprocess.Popen(
                cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                text=True, encoding="utf-8", errors="replace",
            )
            for line in proc.stdout:
                self.log.emit(line)
            proc.wait()
            ok = proc.returncode == 0
        except Exception as e:
            self.log.emit(f"ERROR: {e}\n")
            ok = False
        self.done.emit(ok)


class FlashThread(QThread):
    log          = Signal(str)
    finished_all = Signal(bool)

    def __init__(self, hex_files: list, cfg: dict):
        super().__init__()
        self.hex_files = hex_files
        self.cfg       = cfg

    def run(self):
        programmer = self.cfg["tools"]["programmer"]
        loader     = self.cfg["tools"]["external_loader"]
        overall    = True
        for hex_path in self.hex_files:
            self.log.emit(f"\n=== Flashing: {Path(hex_path).name} ===\n")
            cmd = [programmer, "-c", "port=SWD", "freq=10000", "ap=1",
                   "-el", loader, "-hardRst", "-w", hex_path]
            self.log.emit("$ " + " ".join(str(c) for c in cmd) + "\n")
            try:
                proc = subprocess.Popen(
                    [str(c) for c in cmd],
                    stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                    text=True, encoding="utf-8", errors="replace",
                )
                for line in proc.stdout:
                    self.log.emit(line)
                proc.wait()
                if proc.returncode == 0:
                    self.log.emit("[OK] Flashed successfully.\n")
                else:
                    self.log.emit(f"[ERROR] Flash failed (exit {proc.returncode}).\n")
                    overall = False
            except Exception as e:
                self.log.emit(f"ERROR: {e}\n")
                overall = False
        self.finished_all.emit(overall)


# ── Settings dialog ───────────────────────────────────────────────────────────

class SettingsDialog(QDialog):
    """Tool-path configuration with Docker section and auto-discovery."""

    def __init__(self, cfg: dict, discovery: dict | None = None, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Settings")
        self.setMinimumWidth(680)
        self._build_thread: DockerBuildThread | None = None
        layout = QVBoxLayout(self)

        # ── Native tools ───────────────────────────────────────────────────
        native_grp = QGroupBox("Native Tools")
        nf = QFormLayout(native_grp)

        self._stedgeai    = QLineEdit(cfg["tools"]["stedgeai"])
        self._objcopy     = QLineEdit(cfg["tools"]["objcopy"])
        self._prog        = QLineEdit(cfg["tools"]["programmer"])
        self._loader      = QLineEdit(cfg["tools"]["external_loader"])
        self._outdir      = QLineEdit(cfg["output_dir"])
        self._firmware    = QLineEdit(cfg.get("firmware_dir", ""))

        nf.addRow("stedgeai (native):",         self._file_row(self._stedgeai))
        nf.addRow("arm-none-eabi-objcopy:",      self._file_row(self._objcopy))
        nf.addRow("STM32_Programmer_CLI:",       self._file_row(self._prog))
        nf.addRow("External Loader (.stldr):",
                  self._file_row(self._loader, filt="STLDR (*.stldr);;All (*)"))
        nf.addRow("Output Directory:",           self._dir_row(self._outdir))
        nf.addRow("Firmware Root (for Deploy):", self._dir_row(self._firmware))

        # ── Docker section ─────────────────────────────────────────────────
        docker_grp = QGroupBox("Docker Mode  (bundles objcopy · mounts stedgeai Linux binary)")
        df = QFormLayout(docker_grp)

        self._use_docker = QCheckBox("Use Docker for conversion pipeline")
        self._use_docker.setChecked(cfg.get("use_docker", False))

        self._stedgeai_docker = QLineEdit(cfg["tools"].get("stedgeai_docker", ""))

        # Status labels
        dok = discovery or {}
        da  = dok.get("docker_available", False)
        di  = dok.get("docker_image_built", False)
        sl  = dok.get("stedgeai_linux")

        self._lbl_docker = QLabel()
        self._lbl_image  = QLabel()
        self._update_status_labels(da, di)

        self._build_btn = QPushButton("Build Docker Image")
        self._build_btn.setEnabled(da)
        self._build_btn.clicked.connect(self._build_image)

        self._build_log = QTextEdit()
        self._build_log.setReadOnly(True)
        self._build_log.setFont(QFont("Courier New", 8))
        self._build_log.setFixedHeight(90)
        self._build_log.setVisible(False)

        # Auto-fill stedgeai_docker if empty
        if not self._stedgeai_docker.text() and sl:
            self._stedgeai_docker.setText(sl)

        df.addRow("", self._use_docker)
        df.addRow("stedgeai Linux binary:", self._file_row(self._stedgeai_docker))
        df.addRow("Docker daemon:",  self._lbl_docker)
        df.addRow("Docker image:",   self._lbl_image)
        df.addRow("",                self._build_btn)
        df.addRow("Build log:",      self._build_log)

        self._use_docker.stateChanged.connect(
            lambda: self._build_log.setVisible(False)
        )

        # ── Auto-discover button ───────────────────────────────────────────
        discover_btn = QPushButton("⟳  Auto-discover Tools")
        discover_btn.setToolTip("Scan the system for ST tools and fill empty fields")
        discover_btn.clicked.connect(self._auto_discover)

        layout.addWidget(native_grp)
        layout.addWidget(docker_grp)
        layout.addWidget(discover_btn)

        btns = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        btns.accepted.connect(self.accept)
        btns.rejected.connect(self.reject)
        layout.addWidget(btns)

    def _update_status_labels(self, docker_available: bool, image_built: bool):
        if docker_available:
            self._lbl_docker.setText("● running")
            self._lbl_docker.setStyleSheet("color: green;")
        else:
            self._lbl_docker.setText("○ not available")
            self._lbl_docker.setStyleSheet("color: red;")

        if image_built:
            self._lbl_image.setText(f"● built  ({DOCKER_IMAGE})")
            self._lbl_image.setStyleSheet("color: green;")
        else:
            self._lbl_image.setText("○ not built  — click 'Build Docker Image'")
            self._lbl_image.setStyleSheet("color: #c07000;")

    def _auto_discover(self):
        from PySide6.QtWidgets import QApplication
        QApplication.setOverrideCursor(Qt.WaitCursor)
        try:
            d = tool_discovery.discover_all()
        finally:
            QApplication.restoreOverrideCursor()

        def _fill(edit: QLineEdit, value: str | None):
            if value and not edit.text():
                edit.setText(value)

        _fill(self._stedgeai,        d["stedgeai_native"])
        _fill(self._objcopy,         d["objcopy"])
        _fill(self._prog,            d["programmer"])
        _fill(self._loader,          d["external_loader"])
        _fill(self._stedgeai_docker, d["stedgeai_linux"])
        self._update_status_labels(d["docker_available"], d["docker_image_built"])
        self._build_btn.setEnabled(d["docker_available"])

    def _build_image(self):
        self._build_log.clear()
        self._build_log.setVisible(True)
        self._build_btn.setEnabled(False)
        self._build_thread = DockerBuildThread()
        self._build_thread.log.connect(
            lambda t: self._build_log.append(t.rstrip("\n"))
        )
        self._build_thread.done.connect(self._build_done)
        self._build_thread.start()

    def _build_done(self, ok: bool):
        self._build_btn.setEnabled(True)
        self._update_status_labels(True, ok)
        if ok:
            self._build_log.append("\n[OK] Image built successfully.")
        else:
            self._build_log.append("\n[ERROR] Build failed.")

    # ── Browse helpers ─────────────────────────────────────────────────────
    def _file_row(self, edit: QLineEdit, filt: str = "All (*)") -> QWidget:
        w = QWidget(); h = QHBoxLayout(w); h.setContentsMargins(0, 0, 0, 0)
        h.addWidget(edit)
        b = QPushButton("Browse…"); b.setFixedWidth(80)
        b.clicked.connect(lambda: self._browse_file(edit, filt))
        h.addWidget(b)
        return w

    def _dir_row(self, edit: QLineEdit) -> QWidget:
        w = QWidget(); h = QHBoxLayout(w); h.setContentsMargins(0, 0, 0, 0)
        h.addWidget(edit)
        b = QPushButton("Browse…"); b.setFixedWidth(80)
        b.clicked.connect(lambda: self._browse_dir(edit))
        h.addWidget(b)
        return w

    def _browse_file(self, edit: QLineEdit, filt: str):
        p, _ = QFileDialog.getOpenFileName(self, "Select File", edit.text() or "", filt)
        if p:
            edit.setText(p)

    def _browse_dir(self, edit: QLineEdit):
        p = QFileDialog.getExistingDirectory(self, "Select Directory", edit.text() or "")
        if p:
            edit.setText(p)

    def apply_to(self, cfg: dict):
        cfg["tools"]["stedgeai"]        = self._stedgeai.text().strip()
        cfg["tools"]["stedgeai_docker"] = self._stedgeai_docker.text().strip()
        cfg["tools"]["objcopy"]         = self._objcopy.text().strip()
        cfg["tools"]["programmer"]      = self._prog.text().strip()
        cfg["tools"]["external_loader"] = self._loader.text().strip()
        cfg["output_dir"]               = self._outdir.text().strip()
        cfg["firmware_dir"]             = self._firmware.text().strip()
        cfg["use_docker"]               = self._use_docker.isChecked()


# ── Add/Edit model dialog ─────────────────────────────────────────────────────

class ModelDialog(QDialog):
    def __init__(self, model: dict | None = None, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Add Model" if model is None else "Edit Model")
        self.setMinimumWidth(560)
        layout = QVBoxLayout(self)
        form = QFormLayout()

        self._name    = QLineEdit(model["name"]    if model else "")
        self._file    = QLineEdit(model["file"]    if model else "")
        self._address = QLineEdit(model["address"] if model else "0x71000000")
        self._type    = QComboBox()
        self._type.addItems(["float32", "int8", "uint8"])
        if model:
            idx = self._type.findText(model.get("input_type", "float32"))
            self._type.setCurrentIndex(max(0, idx))
        self._options = QTextEdit()
        self._options.setPlainText(
            model.get("options", DEFAULT_OPTIONS) if model else DEFAULT_OPTIONS
        )
        self._options.setFixedHeight(72)

        file_row = QWidget()
        fh = QHBoxLayout(file_row); fh.setContentsMargins(0, 0, 0, 0)
        fh.addWidget(self._file)
        bb = QPushButton("Browse…"); bb.setFixedWidth(80)
        bb.clicked.connect(self._browse)
        fh.addWidget(bb)

        form.addRow("Name:",           self._name)
        form.addRow("Model File:",     file_row)
        form.addRow("Flash Address:",  self._address)
        form.addRow("Input Type:",     self._type)
        form.addRow("stedgeai Options:", self._options)

        layout.addLayout(form)
        btns = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        btns.accepted.connect(self._validate_accept)
        btns.rejected.connect(self.reject)
        layout.addWidget(btns)

    def _browse(self):
        p, _ = QFileDialog.getOpenFileName(
            self, "Select Model", "",
            "AI Models (*.onnx *.tflite);;All (*)"
        )
        if p:
            self._file.setText(p)
            if not self._name.text():
                self._name.setText(Path(p).stem)

    def _validate_accept(self):
        if not self._name.text().strip():
            QMessageBox.warning(self, "Validation", "Model name is required.")
            return
        if not self._file.text().strip():
            QMessageBox.warning(self, "Validation", "Model file is required.")
            return
        self.accept()

    def get_model(self) -> dict:
        return {
            "name":       self._name.text().strip(),
            "file":       self._file.text().strip(),
            "address":    self._address.text().strip(),
            "input_type": self._type.currentText(),
            "options":    self._options.toPlainText().strip(),
        }


# ── Status colours ────────────────────────────────────────────────────────────
_STATUS = {
    "—":          ("#888", ""),
    "▶ running…": ("#c06000", "#fff8e8"),
    "✓ OK":       ("#1a6b1a", "#e8f5e8"),
    "✕ ERROR":    ("#900",    "#fde8e8"),
}


def _status_item(text: str) -> QTableWidgetItem:
    it = QTableWidgetItem(text)
    it.setTextAlignment(Qt.AlignCenter)
    fg, bg = _STATUS.get(text, ("#000", ""))
    it.setForeground(QColor(fg))
    if bg:
        it.setBackground(QColor(bg))
    return it


# ── Main Window ───────────────────────────────────────────────────────────────
class MainWindow(QMainWindow):
    COL_NAME, COL_FILE, COL_ADDR, COL_TYPE, COL_STATUS = range(5)

    def __init__(self):
        super().__init__()
        self.setWindowTitle("STM32N6 Model Converter")
        self.setMinimumSize(1080, 720)
        self.cfg        = load_config()
        self._discovery: dict = {}
        self._conv_thread:  ConversionThread  | None = None
        self._flash_thread: FlashThread       | None = None
        self._build_ui()
        self._refresh_model_table()
        self._refresh_flash_list()
        # Auto-discover in background on startup
        self._run_auto_discover(silent=True)

    # ── UI construction ────────────────────────────────────────────────────
    def _build_ui(self):
        tb = self.addToolBar("Main")
        tb.setMovable(False)

        act_settings = QAction("⚙  Settings", self)
        act_settings.triggered.connect(self._open_settings)
        tb.addAction(act_settings)
        tb.addSeparator()

        self._act_conv_all = QAction("▶  Convert All", self)
        self._act_conv_all.triggered.connect(self._convert_all)
        tb.addAction(self._act_conv_all)

        self._act_conv_sel = QAction("▶  Convert Selected", self)
        self._act_conv_sel.triggered.connect(self._convert_selected)
        tb.addAction(self._act_conv_sel)

        self._act_abort = QAction("■  Abort", self)
        self._act_abort.setEnabled(False)
        self._act_abort.triggered.connect(self._abort_conversion)
        tb.addAction(self._act_abort)

        tb.addSeparator()

        self._act_deploy_sel = QAction("📦  Deploy Selected", self)
        self._act_deploy_sel.setToolTip(
            "Copy selected model's generated files to firmware Model/<name>/")
        self._act_deploy_sel.triggered.connect(self._deploy_selected)
        tb.addAction(self._act_deploy_sel)

        self._act_deploy_all = QAction("📦  Deploy All", self)
        self._act_deploy_all.setToolTip(
            "Copy all converted models' files to firmware Model/<name>/")
        self._act_deploy_all.triggered.connect(self._deploy_all)
        tb.addAction(self._act_deploy_all)

        tb.addSeparator()
        self._lbl_mode = QLabel("  Mode: —")
        self._lbl_mode.setStyleSheet("color: #555; font-size: 11px;")
        tb.addWidget(self._lbl_mode)

        self.setStatusBar(QStatusBar())

        tabs = QTabWidget()
        self.setCentralWidget(tabs)
        tabs.addTab(self._build_convert_tab(), "Models && Convert")
        tabs.addTab(self._build_flash_tab(),   "Flash")

    def _build_convert_tab(self) -> QWidget:
        page   = QWidget()
        layout = QVBoxLayout(page)

        models_grp = QGroupBox("Models")
        mg = QVBoxLayout(models_grp)

        self._model_table = QTableWidget(0, 5)
        self._model_table.setHorizontalHeaderLabels(
            ["Name", "File", "Address", "Type", "Status"]
        )
        hh = self._model_table.horizontalHeader()
        hh.setSectionResizeMode(self.COL_FILE,   QHeaderView.Stretch)
        hh.setSectionResizeMode(self.COL_NAME,   QHeaderView.ResizeToContents)
        hh.setSectionResizeMode(self.COL_ADDR,   QHeaderView.ResizeToContents)
        hh.setSectionResizeMode(self.COL_TYPE,   QHeaderView.ResizeToContents)
        hh.setSectionResizeMode(self.COL_STATUS, QHeaderView.ResizeToContents)
        self._model_table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self._model_table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self._model_table.setAlternatingRowColors(True)
        self._model_table.doubleClicked.connect(self._edit_model)
        mg.addWidget(self._model_table)

        btn_row = QHBoxLayout()
        for lbl, slot in [("＋  Add", self._add_model),
                           ("✎  Edit", self._edit_model),
                           ("✕  Remove", self._remove_model)]:
            b = QPushButton(lbl); b.clicked.connect(slot); btn_row.addWidget(b)
        btn_row.addStretch()
        oo = QPushButton("📂  Open Output Dir")
        oo.clicked.connect(self._open_output_dir)
        btn_row.addWidget(oo)
        mg.addLayout(btn_row)

        log_grp = QGroupBox("Conversion Log")
        lg = QVBoxLayout(log_grp)
        self._log = QTextEdit()
        self._log.setReadOnly(True)
        self._log.setFont(QFont("Courier New", 9))
        lg.addWidget(self._log)
        clr = QPushButton("Clear Log"); clr.setFixedWidth(90)
        clr.clicked.connect(self._log.clear)
        cr = QHBoxLayout(); cr.addStretch(); cr.addWidget(clr)
        lg.addLayout(cr)

        sp = QSplitter(Qt.Vertical)
        sp.addWidget(models_grp)
        sp.addWidget(log_grp)
        sp.setSizes([280, 340])
        layout.addWidget(sp)
        return page

    def _build_flash_tab(self) -> QWidget:
        page   = QWidget()
        layout = QVBoxLayout(page)

        hex_grp = QGroupBox("HEX Files to Flash  (drag to reorder)")
        hg = QVBoxLayout(hex_grp)
        self._flash_list = QListWidget()
        self._flash_list.setDragDropMode(QAbstractItemView.InternalMove)
        hg.addWidget(self._flash_list)

        br = QHBoxLayout()
        for lbl, slot in [("＋  Add HEX", self._add_flash_file),
                           ("✕  Remove",  self._remove_flash_file),
                           ("↑", self._move_up), ("↓", self._move_down)]:
            b = QPushButton(lbl); b.clicked.connect(slot); br.addWidget(b)
        br.addStretch()
        ab = QPushButton("⟳  Auto-detect from output")
        ab.setToolTip("Scan output directory for *_data.hex files")
        ab.clicked.connect(self._auto_detect_hex)
        br.addWidget(ab)
        hg.addLayout(br)

        note = QLabel("Tip: BOOT1 → right (dev mode) before flash.  BOOT1 → left after.")
        note.setStyleSheet("color: #777; font-size: 11px;")
        hg.addWidget(note)

        self._flash_btn = QPushButton("⚡  Flash All")
        self._flash_btn.setStyleSheet(
            "QPushButton { font-size:14px; padding:8px; }"
            "QPushButton:disabled { color:#aaa; }"
        )
        self._flash_btn.clicked.connect(self._flash_all)

        flash_log_grp = QGroupBox("Flash Log")
        flg = QVBoxLayout(flash_log_grp)
        self._flash_log = QTextEdit()
        self._flash_log.setReadOnly(True)
        self._flash_log.setFont(QFont("Courier New", 9))
        flg.addWidget(self._flash_log)

        top = QWidget()
        tl  = QVBoxLayout(top); tl.setContentsMargins(0, 0, 0, 0)
        tl.addWidget(hex_grp)
        tl.addWidget(self._flash_btn)

        sp = QSplitter(Qt.Vertical)
        sp.addWidget(top)
        sp.addWidget(flash_log_grp)
        sp.setSizes([320, 260])
        layout.addWidget(sp)
        return page

    # ── Auto-discovery ─────────────────────────────────────────────────────
    def _run_auto_discover(self, silent: bool = False):
        """Run discovery in a background thread (non-blocking)."""
        class _DiscoverThread(QThread):
            result = Signal(dict)
            def run(self_):
                self_.result.emit(tool_discovery.discover_all())

        self._disc_thread = _DiscoverThread()
        self._disc_thread.result.connect(lambda d: self._apply_discovery(d, silent))
        self._disc_thread.start()

    def _apply_discovery(self, d: dict, silent: bool):
        self._discovery = d
        t = self.cfg["tools"]

        def _fill(key: str, value: str | None):
            if value and not t.get(key):
                t[key] = value

        _fill("stedgeai",        d["stedgeai_native"])
        _fill("stedgeai_docker", d["stedgeai_linux"])
        _fill("objcopy",         d["objcopy"])
        _fill("programmer",      d["programmer"])
        _fill("external_loader", d["external_loader"])
        save_config(self.cfg)
        self._update_mode_label()

        if not silent:
            found = [k for k, v in d.items() if v and k != "docker_available" and k != "docker_image_built"]
            self.statusBar().showMessage(
                f"Auto-discovery: found {', '.join(found)}", 5000
            )

    def _update_mode_label(self):
        if self.cfg.get("use_docker"):
            img_ok = tool_discovery.is_docker_image_built()
            state  = "ready" if img_ok else "image not built"
            self._lbl_mode.setText(f"  Mode: 🐳 Docker ({state})")
            self._lbl_mode.setStyleSheet("color: #1060c0; font-size:11px;")
        else:
            self._lbl_mode.setText("  Mode: 🖥  Native")
            self._lbl_mode.setStyleSheet("color: #555; font-size:11px;")

    # ── Model table ────────────────────────────────────────────────────────
    def _refresh_model_table(self):
        t = self._model_table
        t.setRowCount(0)
        for m in self.cfg["models"]:
            row = t.rowCount(); t.insertRow(row)
            t.setItem(row, self.COL_NAME,   QTableWidgetItem(m["name"]))
            t.setItem(row, self.COL_FILE,   QTableWidgetItem(m["file"]))
            t.setItem(row, self.COL_ADDR,   QTableWidgetItem(m["address"]))
            t.setItem(row, self.COL_TYPE,   QTableWidgetItem(m.get("input_type", "float32")))
            t.setItem(row, self.COL_STATUS, _status_item("—"))

    def _selected_row(self) -> int:
        items = self._model_table.selectedItems()
        return self._model_table.row(items[0]) if items else -1

    def _add_model(self):
        dlg = ModelDialog(parent=self)
        if dlg.exec() == QDialog.Accepted:
            self.cfg["models"].append(dlg.get_model())
            save_config(self.cfg); self._refresh_model_table()

    def _edit_model(self):
        row = self._selected_row()
        if row < 0: return
        dlg = ModelDialog(model=self.cfg["models"][row], parent=self)
        if dlg.exec() == QDialog.Accepted:
            self.cfg["models"][row] = dlg.get_model()
            save_config(self.cfg); self._refresh_model_table()

    def _remove_model(self):
        row = self._selected_row()
        if row < 0: return
        name = self.cfg["models"][row]["name"]
        if QMessageBox.question(self, "Confirm", f"Remove '{name}'?",
                                QMessageBox.Yes | QMessageBox.No) == QMessageBox.Yes:
            del self.cfg["models"][row]
            save_config(self.cfg); self._refresh_model_table()

    # ── Conversion ─────────────────────────────────────────────────────────
    def _validate_tools(self, need_flash: bool = False) -> bool:
        t   = self.cfg["tools"]
        use = self.cfg.get("use_docker", False)
        missing = []

        if use:
            if not t.get("stedgeai_docker"):
                missing.append("stedgeai Linux binary (Docker mode)")
            if not tool_discovery.is_docker_image_built():
                missing.append(f"Docker image '{DOCKER_IMAGE}' (build it in Settings)")
        else:
            if not t.get("stedgeai"):
                missing.append("stedgeai (native)")
            if not t.get("objcopy"):
                missing.append("arm-none-eabi-objcopy")

        if need_flash:
            if not t.get("programmer"):
                missing.append("STM32_Programmer_CLI")
            if not t.get("external_loader"):
                missing.append("External Loader (.stldr)")

        if missing:
            QMessageBox.warning(self, "Missing Tools",
                                "Configure in Settings ⚙:\n• " + "\n• ".join(missing))
            return False
        return True

    def _start_conversion(self, models: list):
        if not models:
            QMessageBox.information(self, "Nothing to do", "No models to convert.")
            return
        if not self._validate_tools():
            return
        self._set_busy(True)
        mode = "Docker" if self.cfg.get("use_docker") else "Native"
        self._log.append(f"\n[START] {len(models)} model(s) — {mode} mode\n")
        self._conv_thread = ConversionThread(models, self.cfg)
        self._conv_thread.log.connect(self._append_log)
        self._conv_thread.model_status.connect(self._update_row_status)
        self._conv_thread.finished_all.connect(self._conversion_done)
        self._conv_thread.start()

    def _convert_all(self):
        self._start_conversion(list(self.cfg["models"]))

    def _convert_selected(self):
        row = self._selected_row()
        if row < 0:
            QMessageBox.information(self, "Select a model", "Click a row first.")
            return
        self._start_conversion([self.cfg["models"][row]])

    def _abort_conversion(self):
        if self._conv_thread:
            self._conv_thread.abort()

    def _set_busy(self, active: bool):
        self._act_conv_all.setEnabled(not active)
        self._act_conv_sel.setEnabled(not active)
        self._act_abort.setEnabled(active)

    def _update_row_status(self, name: str, status: str):
        labels = {"running": "▶ running…", "ok": "✓ OK", "error": "✕ ERROR"}
        for row in range(self._model_table.rowCount()):
            if self._model_table.item(row, self.COL_NAME).text() == name:
                self._model_table.setItem(
                    row, self.COL_STATUS, _status_item(labels.get(status, status))
                )
                break

    def _conversion_done(self, ok: bool):
        self._set_busy(False)
        msg = "[DONE] All conversions completed." if ok else "[DONE] Some conversions FAILED."
        self._append_log(f"\n{msg}\n")
        self.statusBar().showMessage(msg, 6000)

    # ── Flash ───────────────────────────────────────────────────────────────
    def _refresh_flash_list(self):
        self._flash_list.clear()
        for p in self.cfg.get("flash_files", []):
            self._flash_list.addItem(p)

    def _add_flash_file(self):
        paths, _ = QFileDialog.getOpenFileNames(
            self, "Select HEX Files", "", "Intel HEX (*.hex);;All (*)"
        )
        for p in paths:
            if p not in self.cfg["flash_files"]:
                self.cfg["flash_files"].append(p)
                self._flash_list.addItem(p)
        save_config(self.cfg)

    def _remove_flash_file(self):
        row = self._flash_list.currentRow()
        if row < 0: return
        self.cfg["flash_files"].pop(row)
        self._flash_list.takeItem(row)
        save_config(self.cfg)

    def _move_up(self):
        row = self._flash_list.currentRow()
        if row <= 0: return
        self.cfg["flash_files"].insert(row - 1, self.cfg["flash_files"].pop(row))
        save_config(self.cfg); self._refresh_flash_list()
        self._flash_list.setCurrentRow(row - 1)

    def _move_down(self):
        row = self._flash_list.currentRow()
        lst = self.cfg["flash_files"]
        if row < 0 or row >= len(lst) - 1: return
        lst.insert(row + 1, lst.pop(row))
        save_config(self.cfg); self._refresh_flash_list()
        self._flash_list.setCurrentRow(row + 1)

    def _auto_detect_hex(self):
        out   = Path(self.cfg["output_dir"])
        found = sorted(out.rglob("*_data.hex"))
        added = 0
        for f in found:
            p = str(f)
            if p not in self.cfg["flash_files"]:
                self.cfg["flash_files"].append(p)
                self._flash_list.addItem(p)
                added += 1
        save_config(self.cfg)
        self.statusBar().showMessage(f"Auto-detect: added {added} HEX file(s).", 4000)

    def _flash_all(self):
        # Sync order from widget (drag-drop may reorder)
        self.cfg["flash_files"] = [
            self._flash_list.item(i).text()
            for i in range(self._flash_list.count())
        ]
        save_config(self.cfg)
        if not self.cfg["flash_files"]:
            QMessageBox.information(self, "Nothing to flash", "Add HEX files first.")
            return
        if not self._validate_tools(need_flash=True):
            return
        self._flash_btn.setEnabled(False)
        self._flash_log.append(
            f"\n[START] Flashing {len(self.cfg['flash_files'])} file(s)…\n"
        )
        self._flash_thread = FlashThread(self.cfg["flash_files"], self.cfg)
        self._flash_thread.log.connect(
            lambda t: self._flash_log.append(t.rstrip("\n"))
        )
        self._flash_thread.finished_all.connect(self._flash_done)
        self._flash_thread.start()

    def _flash_done(self, ok: bool):
        self._flash_btn.setEnabled(True)
        msg = "[DONE] Flash completed." if ok else "[DONE] Flash FAILED."
        self._flash_log.append(f"\n{msg}\n")
        self.statusBar().showMessage(msg, 6000)

    # ── Helpers ─────────────────────────────────────────────────────────────
    def _append_log(self, text: str):
        c = self._log.textCursor()
        c.movePosition(QTextCursor.End)
        c.insertText(text)
        self._log.setTextCursor(c)
        self._log.ensureCursorVisible()

    def _open_settings(self):
        dlg = SettingsDialog(self.cfg, discovery=self._discovery, parent=self)
        if dlg.exec() == QDialog.Accepted:
            dlg.apply_to(self.cfg)
            save_config(self.cfg)
            self._update_mode_label()
            self.statusBar().showMessage("Settings saved.", 3000)

    def _open_output_dir(self):
        path = self.cfg["output_dir"]
        if not path or not Path(path).exists():
            QMessageBox.warning(self, "Not found", f"Output dir not found:\n{path}")
            return
        import subprocess as sp
        if sys.platform == "win32":
            sp.Popen(["explorer", path])
        elif sys.platform == "darwin":
            sp.Popen(["open", path])
        else:
            sp.Popen(["xdg-open", path])

    # ── Deploy helpers ───────────────────────────────────────────────────────

    def _deploy_to_firmware(self, models: list[dict]):
        """Copy .c/.h/.hex files for *models* into firmware_dir/Model/<name>/."""
        fw_dir = self.cfg.get("firmware_dir", "").strip()
        if not fw_dir:
            QMessageBox.warning(
                self, "No firmware directory",
                "Please set the Firmware Directory in Settings before deploying.",
            )
            return
        fw_path = Path(fw_dir)
        if not fw_path.is_dir():
            QMessageBox.warning(
                self, "Firmware directory not found",
                f"The firmware directory does not exist:\n{fw_dir}",
            )
            return

        out_root = Path(self.cfg["output_dir"])
        success, errors = 0, []

        for model in models:
            name = model["name"]
            src_dir = out_root / name
            if not src_dir.is_dir():
                errors.append(f"  {name}: output dir not found ({src_dir})")
                continue

            dst_dir = fw_path / "Model" / name
            try:
                dst_dir.mkdir(parents=True, exist_ok=True)
            except OSError as exc:
                errors.append(f"  {name}: cannot create destination — {exc}")
                continue

            copied = 0
            for ext in ("*.c", "*.h", "*.hex"):
                for src_file in src_dir.glob(ext):
                    shutil.copy2(src_file, dst_dir / src_file.name)
                    self._append_log(f"  → {src_file.name}  ({dst_dir})\n")
                    copied += 1

            if copied:
                success += 1
                self._append_log(
                    f"[deploy] {name}: {copied} file(s) copied to {dst_dir}\n"
                )
            else:
                errors.append(f"  {name}: no .c/.h/.hex files found in {src_dir}")

        if errors:
            self.statusBar().showMessage(
                f"Deploy: {success} ok, {len(errors)} error(s).", 5000
            )
            QMessageBox.warning(
                self, "Deploy partial",
                "Deploy completed with errors:\n" + "\n".join(errors),
            )
        else:
            self.statusBar().showMessage(
                f"Deploy OK: {success} model(s) copied to firmware.", 5000
            )

    def _deploy_selected(self):
        row = self._selected_row()
        if row < 0:
            QMessageBox.warning(self, "No selection", "Select a model row first.")
            return
        self._deploy_to_firmware([self.cfg["models"][row]])

    def _deploy_all(self):
        if not self.cfg["models"]:
            QMessageBox.warning(self, "No models", "Add at least one model first.")
            return
        self._deploy_to_firmware(list(self.cfg["models"]))

    def closeEvent(self, event):
        save_config(self.cfg)
        super().closeEvent(event)


# ── Entry point ───────────────────────────────────────────────────────────────
def main():
    app = QApplication(sys.argv)
    app.setApplicationName("STM32N6 Model Converter")
    app.setStyle("Fusion")
    win = MainWindow()
    win.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
