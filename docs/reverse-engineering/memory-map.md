# Memory Map Analysis (ESP32-S3)

- **Target Architecture**: ESP32-S3 (Xtensa LX7) `[CONFIRMED]`

## Memory Map Segments `[CONFIRMED]`

| Bus / Memory Region | Address Range | Size / Type | Description / Evidence |
|---|---|---|---|
| **IROM (Flash Executable Code)** | `0x42000000 - 0x44000000` | Flash Mapped | Application instructions mapped via MMU. Verified by ESP32-S3 TRM & binary vector table. |
| **DROM (Flash Read-Only Data)** | `0x3C000000 - 0x3E000000` | Flash Mapped | String literals, constant tables, waveforms. |
| **IRAM (Internal SRAM Instructions)** | `0x40370000 - 0x403E0000` | Internal SRAM | High-speed IRAM routines and entry point (`0x40379a80`). |
| **DRAM (Internal SRAM Data)** | `0x3FC80000 - 0x3FCE0000` | Internal SRAM | Static data, BSS, and internal heap memory. |
| **External PSRAM (SPI RAM)** | `0x37000000 - 0x38000000` | 8 MB PSRAM | Framebuffers, audio buffers, font caches. Verified by log: `Found 8MB PSRAM device`. |
| **GPIO / IO MUX Base (MMIO)** | `0x60004000` | MMIO Peripheral | Direct GPIO registers (`GPIO_OUT_REG`, `GPIO_OUT_W1TS_REG`, `GPIO_IN_REG`). |
| **SPI2 (FSPI) Base (MMIO)** | `0x60002000` | MMIO Peripheral | E-Ink display / Flash SPI controller. |
| **SPI3 (HSPI) Base (MMIO)** | `0x60003000` | MMIO Peripheral | Secondary SPI peripheral controller. |
| **I2C0 Base (MMIO)** | `0x60013000` | MMIO Peripheral | IO Expander / Power management / Touch / Audio Codec controller bus. |
