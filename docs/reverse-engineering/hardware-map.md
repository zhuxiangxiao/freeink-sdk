# Hardware Map Definition (Metalio E-Ink 4)

## 1. System Hardware Definition Matrix `[CONFIRMED]`

- **Board Name**: `MetalioEInk4` (`metalio-e-ink-4`)
- **MCU**: ESP32-S3 (Xtensa LX7 Dual-Core 32-bit RISC up to 240MHz)
- **Flash Memory**: 16 MB SPI Flash
- **PSRAM Memory**: 8 MB SPI PSRAM
- **Display Controller**: Solomon Systech SSD1677
- **Display Resolution**: 480 x 800 (1-bit monochrome, inverted)
- **Touch Controller**: CST816S (I2C address `0x15`, native 480x800 IRQ)
- **IO Expander**: TCA9555 (I2C address `0x20`)
- **RTC Chip**: PCF8563 (I2C address `0x51`)
- **Battery Charger IC**: CX25601N (I2C address `0x6B`)

## 2. Bus & IO Mapping

- **SPI Host**: `SPI2_HOST` (`EPD_SPI_HOST`)
- **I2C Host**: `I2C_NUM_0`
- **TCA9555 IO Expander Mapping**:
  - `P0.7`: VOL- Button
  - `P1.0`: VOL+ Button
  - `P1.1` (P11): Touch Reset (`TP_RST`)
  - `P1.4`: Accelerator Interrupt (`ACCEL_INT`)
  - `MAIN_PWR`: Main Board Power Enable
  - `SCREEN_SOCKET_PWR`: Screen Power Enable
  - `PA` / `PA_SWITCH`: Audio Power Amplifier Switch
  - `PWR_KEY_PULSE`: Power Button Key Line
  - `USB_MUX_SEL`: USB Routing MUX Selection
