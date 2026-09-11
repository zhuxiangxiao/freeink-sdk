# Firmware Layout Analysis

- **Firmware Path**: `/tmp/file_attachments/firmware/c5c7380f-ebbb-4efb-8419-baddbdb863be.bin`
- **Total Image Size**: `16,405,079` bytes (~16MB Flash)
- **Target Microcontroller**: ESP32-S3 (Xtensa LX7 dual-core) `[CONFIRMED]`
- **App Entry Point**: `0x40379a80` `[CONFIRMED]`

## Partition Table (`0x8000 - 0x9000`) `[CONFIRMED]`

| Offset Start | Offset End | Type | Subtype | Label | Confidence | Evidence |
|---|---|---|---|---|---|---|
| `0x009000` | `0x00D000` | `0x01` (Data) | `0x02` (NVS) | `nvs` | `[CONFIRMED]` | ESP-IDF Partition Entry Header |
| `0x00D000` | `0x00F000` | `0x01` (Data) | `0x00` (OTA Data) | `otadata` | `[CONFIRMED]` | ESP-IDF Partition Entry Header |
| `0x00F000` | `0x010000` | `0x01` (Data) | `0x01` (PHY) | `phy_init` | `[CONFIRMED]` | ESP-IDF Partition Entry Header |
| `0x010000` | `0x074000` | `0x01` (Data) | `0x82` (Custom) | `model` | `[CONFIRMED]` | WakeNet model binary data (`wn9l_hai1tai4ling2`) |
| `0x080000` | `0x580000` | `0x00` (App) | `0x10` (OTA_0) | `ota_0` | `[CONFIRMED]` | ESP-IDF App Image (0xe9 magic, entry 0x40379a80) |
| `0x580000` | `0xA80000` | `0x00` (App) | `0x11` (OTA_1) | `ota_1` | `[CONFIRMED]` | ESP-IDF App Image (Secondary OTA app) |
| `0xA80000` | `0xAE4000` | `0x01` (Data) | `0x82` (Custom) | `resources` | `[CONFIRMED]` | Resource storage partition |
| `0xAE4000` | `0xFE4000` | `0x01` (Data) | `0x40` (FAT/NVS) | `font_data` | `[CONFIRMED]` | Font data partition |
| `0xFE4000` | `0xFF4000` | `0x01` (Data) | `0x03` (Coredump)| `coredump` | `[CONFIRMED]` | Core dump storage area |
