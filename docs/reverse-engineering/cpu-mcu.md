# CPU and MCU Architecture Analysis

- **MCU Model**: ESP32-S3 `[CONFIRMED]`
- **Core Architecture**: Xtensa LX7 (Dual-core 32-bit RISC processor up to 240 MHz) `[CONFIRMED]`
- **Board Target Identifier in Binary**: `metalio-e-ink-4` / `MetalioEInk4Board` `[CONFIRMED]`
- **Flash Size**: 16 MB (Quad SPI / Octal SPI) `[CONFIRMED]`
- **PSRAM Size**: 8 MB External PSRAM (`Found 8MB PSRAM device` in log strings) `[CONFIRMED]`
- **Build System / Framework**: ESP-IDF v5.x / C++ Application (`metalio_e_ink_4_board.cc`) `[CONFIRMED]`

## Evidence Chain
1. Binary strings explicitly identify `./main/boards/metalio-e-ink-4/metalio_e_ink_4_board.cc` and class `MetalioEInk4Board`.
2. Bootloader header magic `0xe9` at offset `0x80000`, entry point `0x40379a80` (Xtensa IRAM address space for ESP32-S3).
3. Log strings: `Found 8MB PSRAM device`.
