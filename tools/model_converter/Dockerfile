# STM32N6 Model Converter — conversion environment
#
# Bundles arm-none-eabi-objcopy so the host needs no ARM toolchain.
# stedgeai is NOT included (not redistributable); the host binary is mounted
# at runtime by model_converter.py via:
#   docker run -v <host_stedgeai>:/tools/stedgeai:ro ...
#
# Build:  docker build -t stm32n6-converter .
# (or use the "Build Docker Image" button in the UI)

FROM ubuntu:22.04

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
        binutils-arm-none-eabi \
 && rm -rf /var/lib/apt/lists/*

# stedgeai will be mounted at /tools/stedgeai — make sure it is on PATH
ENV PATH="/tools:${PATH}"

WORKDIR /workspace
