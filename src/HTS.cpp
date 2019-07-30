/*
  This file is part of the Arduino_HTS221 library.
  Copyright (c) 2019 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Wire.h>

#include "HTS.h"

#define HTS221_ADDRESS   0x5F

#define HTS221_WHO_AM_I_REG         0x0f
#define HTS221_CTRL1_REG            0x20
#define HTS221_CTRL2_REG            0x21
#define HTS221_STATUS_REG           0x27
#define HTS221_HUMIDITY_OUT_L_REG   0x28
#define HTS221_TEMP_OUT_L_REG       0x2a
#define HTS221_H0_rH_x2_REG         0x30
#define HTS221_H1_rH_x2_REG         0x31
#define HTS221_T0_degC_x8_REG       0x32
#define HTS221_T1_degC_x8_REG       0x33
#define HTS221_T1_T0_MSB_REG        0x35
#define HTS221_H0_T0_OUT_REG        0x36
#define HTS221_H1_T0_OUT_REG        0x3a
#define HTS221_T0_OUT_REG           0x3c
#define HTS221_T1_OUT_REG           0x3e

HTS221Class::HTS221Class(TwoWire& wire) :
  _wire(&wire)
{
}

int HTS221Class::begin()
{
  _wire->begin();

  if (i2cRead(HTS221_WHO_AM_I_REG) != 0xbc) {
    end();

    return 0;
  }

  readHTS221Calibration();

  // enable HTS221
  i2cWrite(HTS221_CTRL1_REG, 0x80);

  return 1;
}

void HTS221Class::end()
{
  // disable HTS221
  i2cWrite(HTS221_CTRL1_REG, 0x00);

  _wire->end();
}

float HTS221Class::readTemperature(int units)
{
  // trigger one shot
  i2cWrite(HTS221_CTRL2_REG, 0x01);

  // wait for completion
  while ((i2cRead(HTS221_STATUS_REG) & 0x01) == 0) {
    yield();
  }

  // read value and convert
  int16_t tout = i2cRead16(HTS221_TEMP_OUT_L_REG);
  float reading = (tout * _hts221TemperatureSlope + _hts221TemperatureZero);
  if (units == FAHRENHEIT) { // Fahrenheit = (Celsius * 9 / 5) + 32
    return (reading * 9.0 / 5.0) + 32.0;
  } else {
    return reading;
  }
}

float HTS221Class::readHumidity()
{
  // trigger one shot
  i2cWrite(HTS221_CTRL2_REG, 0x01);

  // wait for completion
  while ((i2cRead(HTS221_STATUS_REG) & 0x02) == 0) {
    yield();
  }

  // read value and convert
  int16_t hout = i2cRead16(HTS221_HUMIDITY_OUT_L_REG);

  return (hout * _hts221HumiditySlope + _hts221HumidityZero);
}

int HTS221Class::i2cRead(uint8_t reg)
{
  _wire->beginTransmission(HTS221_ADDRESS);
  _wire->write(reg);
  if (_wire->endTransmission(false) != 0) {
    return -1;
  }

  if (_wire->requestFrom(HTS221_ADDRESS, 1) != 1) {
    return -1;
  }
  
  return _wire->read();
}

int HTS221Class::i2cWrite(uint8_t reg, uint8_t val)
{
  _wire->beginTransmission(HTS221_ADDRESS);
  _wire->write(reg);
  _wire->write(val);
  if (_wire->endTransmission() != 0) {
    return 0;
  }

  return 1;
}

void HTS221Class::readHTS221Calibration()
{
  uint8_t h0rH = i2cRead(HTS221_H0_rH_x2_REG);
  uint8_t h1rH = i2cRead(HTS221_H1_rH_x2_REG);

  uint16_t t0degC = i2cRead(HTS221_T0_degC_x8_REG) | ((i2cRead(HTS221_T1_T0_MSB_REG) & 0x03) << 8);
  uint16_t t1degC = i2cRead(HTS221_T1_degC_x8_REG) | ((i2cRead(HTS221_T1_T0_MSB_REG) & 0x0c) << 6);

  int16_t h0t0Out = i2cRead16(HTS221_H0_T0_OUT_REG);
  int16_t h1t0Out = i2cRead16(HTS221_H1_T0_OUT_REG);

  int16_t t0Out = i2cRead16(HTS221_T0_OUT_REG);
  int16_t t1Out = i2cRead16(HTS221_T1_OUT_REG);

  // calculate slopes and 0 offset from calibration values,
  // for future calculations: value = a * X + b

  _hts221HumiditySlope = (h1rH - h0rH) / (2.0 * (h1t0Out - h0t0Out));
  _hts221HumidityZero = (h0rH / 2.0) - _hts221HumiditySlope * h0t0Out;

  _hts221TemperatureSlope = (t1degC - t0degC) / (8.0 * (t1Out - t0Out));
  _hts221TemperatureZero = (t0degC / 8.0) - _hts221TemperatureSlope * t0Out;
}

#ifdef ARDUINO_ARDUINO_NANO33BLE
HTS221Class HTS(Wire1);
#else
HTS221Class HTS(Wire);
#endif
