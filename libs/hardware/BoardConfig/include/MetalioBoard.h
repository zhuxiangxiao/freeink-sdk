#pragma once

#include <Arduino.h>
#include <Wire.h>

// TCA9555 / TCA95XX 16-bit I2C IO Expander for Metalio E-Ink 4 board.
// I2C Address: 0x20
//
// Pin assignments on Metalio E-Ink 4:
//   P0.7 (Pin 7): VOL- Button (Input)
//   P1.0 (Pin 8): VOL+ Button (Input)
//   P1.1 (Pin 9): TP_RST / Touch Reset (Output)
//   P1.2 (Pin 10): SCREEN_SOCKET_PWR (Output)
//   P1.3 (Pin 11): PWR_KEY_PULSE (Output)
//   P1.4 (Pin 12): ACCEL_INT (Input)
//   P1.5 (Pin 13): PA_SWITCH (Output)
//   P1.6 (Pin 14): PA / Audio Amp Power (Output)
//   P1.7 (Pin 15): MAIN_PWR / System Power (Output)

namespace metalio {

class Tca9555 {
 public:
  static constexpr uint8_t I2C_ADDR = 0x20;

  // Register Map
  static constexpr uint8_t REG_INPUT_PORT0 = 0x00;
  static constexpr uint8_t REG_INPUT_PORT1 = 0x01;
  static constexpr uint8_t REG_OUTPUT_PORT0 = 0x02;
  static constexpr uint8_t REG_OUTPUT_PORT1 = 0x03;
  static constexpr uint8_t REG_POLARITY_PORT0 = 0x04;
  static constexpr uint8_t REG_POLARITY_PORT1 = 0x05;
  static constexpr uint8_t REG_CONFIG_PORT0 = 0x06;
  static constexpr uint8_t REG_CONFIG_PORT1 = 0x07;

  // IO Expander Pins
  enum Pin : uint8_t {
    VOL_DOWN = 7,          // P0.7
    VOL_UP = 8,            // P1.0
    TP_RST = 9,            // P1.1
    SCREEN_SOCKET_PWR = 10,// P1.2
    PWR_KEY_PULSE = 11,    // P1.3
    ACCEL_INT = 12,        // P1.4
    PA_SWITCH = 13,        // P1.5
    PA = 14,               // P1.6
    MAIN_PWR = 15,         // P1.7
    USB_MUX_SEL = 15       // Alias for main/USB power selection
  };

  static bool begin(int sda = 39, int scl = 38, uint32_t frequency = 400000) {
    Wire.begin(sda, scl, frequency);

    // Set Pin Directions (0 = Output, 1 = Input)
    // Port 0: P0.7 input (0x80), P0.0-P0.6 outputs (0x00) -> 0x80
    // Port 1: P1.0 input, P1.4 input -> 0x11
    uint8_t cfg0 = 0x80;
    uint8_t cfg1 = 0x11;

    if (!writeReg(REG_CONFIG_PORT0, cfg0) || !writeReg(REG_CONFIG_PORT1, cfg1)) {
      return false;
    }

    // Default Output Levels
    // MAIN_PWR (P1.7) = HIGH, SCREEN_SOCKET_PWR (P1.2) = HIGH, PWR_KEY_PULSE (P1.3) = HIGH
    // PA (P1.6) = LOW, PA_SWITCH (P1.5) = LOW, TP_RST (P1.1) = HIGH
    uint8_t out1 = (1 << (MAIN_PWR - 8)) | (1 << (SCREEN_SOCKET_PWR - 8)) |
                   (1 << (PWR_KEY_PULSE - 8)) | (1 << (TP_RST - 8));
    writeReg(REG_OUTPUT_PORT1, out1);

    return true;
  }

  static bool setPin(Pin pin, bool level) {
    uint8_t reg = (pin < 8) ? REG_OUTPUT_PORT0 : REG_OUTPUT_PORT1;
    uint8_t bit = pin % 8;
    uint8_t val = 0;
    if (!readReg(reg, val)) return false;
    if (level) val |= (1 << bit);
    else val &= ~(1 << bit);
    return writeReg(reg, val);
  }

  static uint16_t readInputs() {
    uint8_t p0 = 0, p1 = 0;
    readReg(REG_INPUT_PORT0, p0);
    readReg(REG_INPUT_PORT1, p1);
    return (static_cast<uint16_t>(p1) << 8) | p0;
  }

  static bool isVolDownPressed() {
    uint8_t p0 = 0;
    if (readReg(REG_INPUT_PORT0, p0)) {
      return (p0 & 0x80) == 0; // Active-low
    }
    return false;
  }

  static bool isVolUpPressed() {
    uint8_t p1 = 0;
    if (readReg(REG_INPUT_PORT1, p1)) {
      return (p1 & 0x01) == 0; // Active-low
    }
    return false;
  }

 private:
  static bool writeReg(uint8_t reg, uint8_t val) {
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(reg);
    Wire.write(val);
    return Wire.endTransmission() == 0;
  }

  static bool readReg(uint8_t reg, uint8_t& val) {
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) return false;
    if (Wire.requestFrom(I2C_ADDR, (uint8_t)1) == 1) {
      val = Wire.read();
      return true;
    }
    return false;
  }
};

inline uint8_t metalioButtonHook() {
  uint8_t mask = 0;
  if (Tca9555::isVolDownPressed()) {
    mask |= (1 << 5); // BTN_DOWN
  }
  if (Tca9555::isVolUpPressed()) {
    mask |= (1 << 4); // BTN_UP
  }
  return mask;
}

}  // namespace metalio
