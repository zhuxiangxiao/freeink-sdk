# SPI and GPIO Bus Mapping Analysis

- **Target Board Model**: `metalio-e-ink-4` `[CONFIRMED]`
- **Display Controller IC**: Solomon Systech SSD1677 `[CONFIRMED]`
- **Display Bus**: SPI Interface (using ESP32-S3 `SPI2_HOST` / `EPD_SPI_HOST`) `[CONFIRMED]`

## Peripheral & Bus Pin Assignment Table `[HIGH CONFIDENCE]`

| Signal Name | Connection Type | Pin / Location | Confidence | Evidence & Log References |
|---|---|---|---|---|
| **EPD_SCK** | ESP32-S3 Native GPIO | SPI Clock (`EPD_SPI_HOST`) | `[HIGH CONFIDENCE]` | `InitializeSsd1677`, `spi_bus_initialize(EPD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO)` |
| **EPD_MOSI** | ESP32-S3 Native GPIO | SPI Master Output | `[HIGH CONFIDENCE]` | Standard E-Ink SPI configuration in `InitializeSsd1677` |
| **EPD_CS** | ESP32-S3 Native GPIO | EPD Chip Select | `[HIGH CONFIDENCE]` | `esp_lcd_new_panel_io_spi` initialization context |
| **EPD_DC** | ESP32-S3 Native GPIO | Data / Command Control | `[HIGH CONFIDENCE]` | `esp_lcd_new_panel_io_spi` (`&io_config`) |
| **EPD_RST** | ESP32-S3 Native GPIO / Expander | Panel Hardware Reset | `[HIGH CONFIDENCE]` | Log strings: `RST gpio err`, `RST idle high err`, `gpio_hold_en(RST)` |
| **EPD_BUSY** | ESP32-S3 Native GPIO | Panel Busy Signal (ISR) | `[HIGH CONFIDENCE]` | Log strings: `SSD1677: gpio_install_isr_service...`, `BUSY gpio err`, `BUSY isr err` |
| **I2C Bus** | ESP32-S3 Native GPIOs | SDA / SCL (Shared Bus) | `[CONFIRMED]` | `MetalioEInk4Board::InitializeI2c()` |
| **IO Expander** | I2C Peripheral | TCA9555 (Address `0x20`) | `[CONFIRMED]` | Log strings: `TCA9555 ready addr=0x02lx`, `TCA9555 P1.4` |
| **Touch IC** | I2C Peripheral | CST816S (Address `0x15`) | `[CONFIRMED]` | Log strings: `CST816S ready (SDA=%d SCL=%d INT=%d) native 480x800 IRQ` |
| **Touch Reset** | IO Expander Pin | Pin P1.1 (P11) | `[CONFIRMED]` | Log string: `TP_RST pulse done (P1.1/P11 H->L->H)` |
| **Main Power** | IO Expander Pin | `MAIN_PWR` | `[CONFIRMED]` | Code call: `io.setLevel(IOExpander::Pin::MAIN_PWR, true)` |
| **Screen Socket Power**| IO Expander Pin | `SCREEN_SOCKET_PWR` | `[CONFIRMED]` | Code call: `io.setLevel(IOExpander::Pin::SCREEN_SOCKET_PWR, true)` |
| **Audio / Speaker PA** | IO Expander Pin | `PA` / `PA_SWITCH` | `[CONFIRMED]` | Code call: `io.setLevel(IOExpander::Pin::PA_SWITCH, false)` |
| **Buttons (VOL-/VOL+)**| IO Expander Pins | VOL-=P0.7, VOL+=P1.0 | `[CONFIRMED]` | Log string: `Buttons ready: BOOT=GPIO%d POWER=GPIO%d VOL-=P0.7 VOL+=P1.0` |
| **RTC IC** | I2C Peripheral | PCF8563 (Address `0x51`) | `[CONFIRMED]` | Log strings: `network time synced to PCF8563` |
| **Battery Charger IC** | I2C Peripheral | CX25601N (Address `0x6B`) | `[CONFIRMED]` | Log strings: `CX25601N init OK at 0x02X, ichg=%d mA` |
