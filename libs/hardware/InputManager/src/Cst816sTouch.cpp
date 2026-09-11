#include "InputManager.h"

// Note: CST816S touch driver implementation for InputManager.
// CST816S is an I2C capacitive touch controller operating at 7-bit address 0x15.
// Registers:
// 0x02: Finger Num / Touch Count
// 0x03: Touch 0 X High [3:0], Event Flag [7:6]
// 0x04: Touch 0 X Low [7:0]
// 0x05: Touch 0 Y High [3:0], Touch ID [7:4]
// 0x06: Touch 0 Y Low [7:0]
// Native coordinates reported by CST816S on Metalio E-Ink 4: 480 x 800 (portrait).

#if FREEINK_CAP_TOUCH
#include <Wire.h>

void InputManager::beginCst816s() {
  const auto& t = BoardConfig::ACTIVE.touch;
  if (t.powerEnable >= 0) {
    gpio_hold_dis(static_cast<gpio_num_t>(t.powerEnable));
    pinMode(t.powerEnable, OUTPUT);
    digitalWrite(t.powerEnable, t.powerEnableActiveHigh ? HIGH : LOW);
    delay(50);
  }

  if (t.reset >= 0) {
    pinMode(t.reset, OUTPUT);
    digitalWrite(t.reset, LOW);
    delay(20);
    digitalWrite(t.reset, HIGH);
    delay(50);
  }

  if (t.sda >= 0 && t.scl >= 0) {
    Wire.begin(t.sda, t.scl, 400000);
    Wire.setTimeOut(10);
  }

  if (t.irq >= 0) {
    pinMode(t.irq, INPUT_PULLUP);
  }

  // Probe CST816S I2C address (default 0x15)
  const uint8_t addr = t.i2cAddress ? t.i2cAddress : 0x15;
  Wire.beginTransmission(addr);
  if (Wire.endTransmission() == 0) {
    touchDataEnabled = true;
  } else {
    touchDataEnabled = false;
  }
}

void InputManager::pollCst816s(const unsigned long now) {
  const auto& t = BoardConfig::ACTIVE.touch;
  if (now < touchReadAt) return;
  touchReadAt = now + TOUCH_SAMPLE_DELAY_MS;

  const uint8_t addr = t.i2cAddress ? t.i2cAddress : 0x15;

  // Read 5 bytes starting at register 0x02
  Wire.beginTransmission(addr);
  Wire.write(0x02);
  if (Wire.endTransmission(false) != 0) {
    if (touchPressed && now - touchPoint.timestamp > 100) {
      touchPressed = false;
      touchPoint.valid = false;
      touchReleasedEvent = true;
      lastTouchHeldDurationMs = now - touchDownPoint.timestamp;
    }
    return;
  }

  uint8_t buf[5] = {0};
  if (Wire.requestFrom(addr, (uint8_t)5, (uint8_t)true) != 5) {
    while (Wire.available()) Wire.read();
    if (touchPressed && now - touchPoint.timestamp > 100) {
      touchPressed = false;
      touchPoint.valid = false;
      touchReleasedEvent = true;
      lastTouchHeldDurationMs = now - touchDownPoint.timestamp;
    }
    return;
  }

  for (uint8_t i = 0; i < 5; i++) buf[i] = Wire.read();

  uint8_t touchCount = buf[0] & 0x0F;
  if (touchCount == 0) {
    if (touchPressed) {
      touchPressed = false;
      touchPoint.valid = false;
      touchReleasedEvent = true;
      lastTouchHeldDurationMs = now - touchDownPoint.timestamp;
    }
    return;
  }

  uint16_t rawX = ((static_cast<uint16_t>(buf[1]) & 0x0F) << 8) | buf[2];
  uint16_t rawY = ((static_cast<uint16_t>(buf[3]) & 0x0F) << 8) | buf[4];

  // Apply axis swapping, scaling, and flipping
  const uint16_t sx = t.swapXY ? rawY : rawX;
  const uint16_t sy = t.swapXY ? rawX : rawY;

  touchPoint.valid = true;
  touchPoint.x = mapTouchAxis(sx, t.rawMinX, t.rawMaxX, t.rawMaxX - t.rawMinX);
  touchPoint.y = mapTouchAxis(sy, t.rawMinY, t.rawMaxY, t.rawMaxY - t.rawMinY);
  if (t.flipX) touchPoint.x = static_cast<uint16_t>((t.rawMaxX - t.rawMinX) - touchPoint.x);
  if (t.flipY) touchPoint.y = static_cast<uint16_t>((t.rawMaxY - t.rawMinY) - touchPoint.y);
  touchPoint.timestamp = now;

  if (!touchPressed) {
    touchPressed = true;
    touchPressedEvent = true;
    touchDownPoint = touchPoint;
    touchUpPoint = touchPoint;
    touchMovedBeyondTapSlop = false;
    touchMovedBeyondTapReleaseSlop = false;
  } else {
    touchUpPoint = touchPoint;
    const int dx = static_cast<int>(touchUpPoint.x) - static_cast<int>(touchDownPoint.x);
    const int dy = static_cast<int>(touchUpPoint.y) - static_cast<int>(touchDownPoint.y);
    if (absInt(dx) > TOUCH_TAP_SLOP_PX || absInt(dy) > TOUCH_TAP_SLOP_PX) {
      touchMovedBeyondTapSlop = true;
    }
    if (absInt(dx) > TOUCH_TAP_RELEASE_SLOP_PX || absInt(dy) > TOUCH_TAP_RELEASE_SLOP_PX) {
      touchMovedBeyondTapReleaseSlop = true;
    }
  }
}
#endif
