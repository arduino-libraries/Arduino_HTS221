/*
  HTS221 - Temperature Alert
  This example reads data from the on-board HTS221 sensor of the
  Nano 33 BLE Sense.If the temperature increases above a certain limit, It turns the buzzer on.
  The circuit:
  - Arduino Nano 33 BLE Sense
  -Buzzer module connected at pin 9

  written by K.Abhijeet
  This example code is in the public domain
*/

#include <Arduino_HTS221.h>

float tempLimit = 37;                         // set your temperature limit in °C

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(9, OUTPUT);

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
}

void loop() {

  float temperature = HTS.readTemperature();         // read the sensor value

  if (temperature > tempLimit)
  {
    digitalWrite(9, HIGH);
    delay(500);
    digitalWrite(9, LOW);
    delay(500);
  }
  else
  {
    digitalWrite(9, LOW);
    delay(2000);
  }
  Serial.print("Temperature = ");                   // print the sensor value
  Serial.print(temperature);
  Serial.println(" °C");

}
