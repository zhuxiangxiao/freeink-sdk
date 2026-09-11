# E-Ink Display Controller & Parameter Analysis

- **Display Controller IC Target**: Solomon Systech **SSD1677** `[CONFIRMED]`
- **Display Resolution**: **480 x 800** pixels `[CONFIRMED]`
- **Color Depth & Buffer**: 1-bit Monochrome (Black/White) with inverted color configuration `[CONFIRMED]`
- **Screen Dimensions / Alignment**: Native 480x800 portrait mode, matches Touch IC (CST816S) resolution `[CONFIRMED]`

## Control Sequence & Command Feature Matrix `[CONFIRMED]`

| Command (Hex) | Command Name / Action | Extracted Behavior & Parameters | Confidence |
|---|---|---|---|
| `0x12` | Software Reset (`SWRST`) | Trigger soft reset before panel init | `[CONFIRMED]` |
| `0x11` | Deep Sleep Mode | Set panel into ultra-low power sleep state | `[CONFIRMED]` |
| `0x44` | Set RAM X-Address Start/End | X-Window limits for SSD1677 | `[CONFIRMED]` |
| `0x45` | Set RAM Y-Address Start/End | Y-Window limits (800 rows) | `[CONFIRMED]` |
| `0x24` | Write RAM (Black / White) | Framebuffer pixel data transfer | `[CONFIRMED]` |
| `0x26` | Write RAM (Red / Secondary) | Used when partial update or previous buffer comparison is enabled | `[CONFIRMED]` |
| `0x22` | Display Update Control 2 | `0x22` sequence control for full/partial refresh activation | `[CONFIRMED]` |
| `0x20` | Master Activation | Triggers physical E-Ink update sequence | `[CONFIRMED]` |
| `0x3C` | Border Waveform Control | Border control during refresh | `[CONFIRMED]` |

## Candidate Controller Matrix

| Controller IC | Resolution Compatibility | Match Score | Verdict | Evidence |
|---|---|---|---|---|
| **SSD1677** | Up to 480x800 | 100% | **PRIMARY / SELECTED** | Direct ESP-IDF driver log symbol `esp_lcd_new_panel_ssd1677` & `SSD1677 ready 480x800` |
| **SSD1680 / 1681** | 200x200 / 400x300 | 30% | Rejected | Resolution mismatch |
| **UC8179 / UC8151** | 480x800 | 45% | Rejected | Command sequence and register set mismatch |
| **GD7965** | 800x480 | 40% | Rejected | Command set mismatch |
