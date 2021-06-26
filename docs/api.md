# ArduinoHTS221 library

## Methods

### `begin()`

Initialize the humidity and temperature sensor.

#### Syntax 

```
HTS.begin()
```

#### Parameters

None.

#### Returns

1 on success, 0 on failure.

#### Example

```
if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
}
```

#### See also

* [end()](#end)
* [readTemperature()](#readTemperature)
* [readHumidity()](#readHumidity)

### `end()`

De-initialize the humidity and temperature sensor.

#### Syntax 

```
HTS.end()
```

#### Parameters

None.

#### Returns

None.

#### Example

```
if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
}

// ...

HTS.end();
```

#### See also

* [begin()](#begin)
* [readTemperature()](#readTemperature)
* [readHumidity()](#readHumidity)

### `readTemperature()`

Read the sensor’s measured temperature value.

#### Syntax 

```
HTS.readTemperature()
HTS.readTemperature(units)
```

#### Parameters

* _units_: FAHRENHEIT to read the temperature in Fahrenheit and CELSIUS to read the temperature in Celsius. If unit parameter is not provided, default is CELSIUS.

#### Returns

The temperature in degrees Fahrenheit or Celsius, depending on the units requested.

#### Example

```
float temperature = HTS.readTemperature();

Serial.print("Temperature = ");
Serial.print(temperature);
Serial.println(" °C");
```

#### See also

* [begin()](#begin)
* [end()](#end)
* [readHumidity()](#readHumidity)

### `readHumidity()`

Read the sensor’s measured relative humidity value.

#### Syntax 

```
HTS.readHumidity()
```

#### Parameters

None.

#### Returns

The humidity value from the sensor as a percentage.

#### Example

```
float humidity = HTS.readHumidity();

Serial.print("Humidity = ");
Serial.print(humidity);
Serial.println(" %");
```

#### See also

* [begin()](#begin)
* [end()](#end)
* [readTemperature()](#readTemperature)





















