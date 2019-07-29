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

#ifndef _HTS_H_
#define _HTS_H_

#include <Arduino.h>
#include <Wire.h>

enum {
  FAHRENHEIT,
  CELSIUS
};

class HTS221Class {
public:
  HTS221Class(TwoWire& wire);

  int begin();
  void end();

  float readTemperature(int units = CELSIUS);
  float readHumidity();

private:
  int i2cRead(uint8_t reg);
  int i2cRead16(uint8_t reg) { 
    return (i2cRead(reg) | (i2cRead(reg + 1) << 8));
  }
  int i2cWrite(uint8_t reg, uint8_t val);

  void readHTS221Calibration();

private:
  TwoWire* _wire;

  float _hts221HumiditySlope;
  float _hts221HumidityZero;
  float _hts221TemperatureSlope;
  float _hts221TemperatureZero;
};

extern HTS221Class HTS;

#endif
