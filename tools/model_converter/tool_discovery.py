"""Auto-discover STM32 development tools on the host.

Handles Windows, Linux, macOS, and WSL2-accessible paths.
For Docker mode the X-CUBE-AI pack includes a Linux ELF binary even on
a Windows host (at .../Utilities/linux/stedgeai), which Docker Desktop
can mount directly into a Linux container via WSL2.
"""
from __future__ import annotations

import glob
import os
import shutil
import subprocess
import sys
from pathlib import Path


# ── Internal helpers ──────────────────────────────────────────────────────────

def _which(name: str) -> str | None:
    return shutil.which(name)


def _first_glob(patterns: list[str]) -> str | None:
    """Return the lexicographically last match across all patterns (prefers higher version numbers)."""
    hits: list[str] = []
    for pat in patterns:
        hits += glob.glob(pat, recursive=True)
    if hits:
        hits.sort(reverse=True)
        return hits[0]
    return None


def _win_home() -> Path:
    return Path(os.environ.get("USERPROFILE", r"C:\Users"))


def _xcubeai_base() -> str:
    """Root of STM32Cube packs directory (works on all platforms)."""
    if sys.platform == "win32":
        return str(_win_home() / "STM32Cube" / "Repository" / "Packs" / "STMicroelectronics")
    return str(Path.home() / "STM32Cube" / "Repository" / "Packs" / "STMicroelectronics")


# ── stedgeai — native (runs directly on host OS) ─────────────────────────────

def find_stedgeai_native() -> str | None:
    """Find a stedgeai executable that runs natively on the current OS."""
    exe = "stedgeai.exe" if sys.platform == "win32" else "stedgeai"
    found = _which(exe)
    if found:
        return found

    base = _xcubeai_base()

    if sys.platform == "win32":
        patterns = [
            # X-CUBE-AI pack Windows utilities
            f"{base}\\X-CUBE-AI\\**\\Utilities\\windows\\stedgeai.exe",
            # STM32CubeIDE bundled plugin
            r"C:\ST\STM32CubeIDE_*\STM32CubeIDE\plugins"
            r"\com.st.stm32cube.ide.mcu.externaltools.stedgeai*\tools\bin\stedgeai.exe",
        ]
    else:
        suffix = "mac" if sys.platform == "darwin" else "linux"
        patterns = [
            f"{base}/X-CUBE-AI/**/Utilities/{suffix}/stedgeai",
        ]

    return _first_glob(patterns)


# ── stedgeai — Linux ELF (for mounting into a Docker Linux container) ─────────

def find_stedgeai_linux() -> str | None:
    """Find the Linux ELF stedgeai binary.

    On Windows the X-CUBE-AI pack ships Linux utilities too, so they live at
    %USERPROFILE%\\STM32Cube\\...\\Utilities\\linux\\stedgeai even on Windows.
    Docker Desktop (WSL2) can mount these Windows-hosted Linux ELF files directly.
    """
    base = _xcubeai_base()

    if sys.platform == "win32":
        patterns = [
            f"{base}\\X-CUBE-AI\\**\\Utilities\\linux\\stedgeai",
        ]
        found = _first_glob(patterns)
        if found:
            return found

        # Fallback: check WSL distros via `wsl wslpath` trick
        try:
            r = subprocess.run(
                [
                    "wsl", "--", "bash", "-c",
                    r"find ~/STM32Cube/Repository/Packs/STMicroelectronics/X-CUBE-AI"
                    r" -name 'stedgeai' -type f 2>/dev/null | sort -rV | head -1",
                ],
                capture_output=True, text=True, timeout=15,
            )
            if r.returncode == 0 and r.stdout.strip():
                wsl_path = r.stdout.strip()
                r2 = subprocess.run(
                    ["wsl", "wslpath", "-w", wsl_path],
                    capture_output=True, text=True, timeout=5,
                )
                if r2.returncode == 0:
                    win_path = r2.stdout.strip()
                    if Path(win_path).exists():
                        return win_path
        except Exception:
            pass
        return None

    # Linux/macOS: same as native
    return find_stedgeai_native()


# ── arm-none-eabi-objcopy ─────────────────────────────────────────────────────

def find_objcopy() -> str | None:
    """Find arm-none-eabi-objcopy on the host (needed for native mode only)."""
    for name in ("arm-none-eabi-objcopy", "arm-none-eabi-objcopy.exe"):
        found = _which(name)
        if found:
            return found

    if sys.platform == "win32":
        patterns = [
            # STM32CubeIDE bundled GNU toolchain
            r"C:\ST\STM32CubeIDE_*\STM32CubeIDE\plugins"
            r"\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.*"
            r"\tools\bin\arm-none-eabi-objcopy.exe",
            # Standalone GNU Arm Embedded Toolchain installers
            r"C:\Program Files\GNU Arm Embedded Toolchain\*\bin\arm-none-eabi-objcopy.exe",
            r"C:\Program Files (x86)\GNU Arm Embedded Toolchain\*\bin\arm-none-eabi-objcopy.exe",
        ]
        return _first_glob(patterns)

    return None


# ── STM32_Programmer_CLI ──────────────────────────────────────────────────────

def find_programmer() -> str | None:
    for name in ("STM32_Programmer_CLI", "STM32_Programmer_CLI.exe"):
        found = _which(name)
        if found:
            return found

    if sys.platform == "win32":
        patterns = [
            r"C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe",
            r"C:\Program Files (x86)\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe",
        ]
    else:
        home = Path.home()
        patterns = [
            str(home / "STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI"),
            "/opt/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI",
        ]

    return _first_glob(patterns)


# ── External loader (.stldr) ──────────────────────────────────────────────────

def find_external_loader(name: str = "MX66UW1G45G_STM32N6570-DK.stldr") -> str | None:
    if sys.platform == "win32":
        candidates = [
            rf"C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader\{name}",
            rf"C:\Program Files (x86)\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader\{name}",
        ]
    else:
        home = Path.home()
        candidates = [
            str(home / f"STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/ExternalLoader/{name}"),
            f"/opt/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/ExternalLoader/{name}",
        ]

    for c in candidates:
        if Path(c).exists():
            return c
    return None


# ── Docker ────────────────────────────────────────────────────────────────────

DOCKER_IMAGE = "stm32n6-converter:latest"


def is_docker_available() -> bool:
    try:
        r = subprocess.run(["docker", "info"], capture_output=True, timeout=8)
        return r.returncode == 0
    except Exception:
        return False


def is_docker_image_built(tag: str = DOCKER_IMAGE) -> bool:
    try:
        r = subprocess.run(
            ["docker", "image", "inspect", tag],
            capture_output=True, timeout=5,
        )
        return r.returncode == 0
    except Exception:
        return False


# ── All-in-one discovery ──────────────────────────────────────────────────────

def discover_all() -> dict:
    """Scan the host and return a dict with found paths and Docker status.

    Runs quickly; heavy operations (WSL subprocess) are only triggered when
    the primary scan finds nothing.
    """
    docker_ok = is_docker_available()
    return {
        "stedgeai_native": find_stedgeai_native(),
        "stedgeai_linux":  find_stedgeai_linux(),
        "objcopy":         find_objcopy(),
        "programmer":      find_programmer(),
        "external_loader": find_external_loader(),
        "docker_available":    docker_ok,
        "docker_image_built":  is_docker_image_built() if docker_ok else False,
    }
