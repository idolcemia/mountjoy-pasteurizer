#ifndef TEMPERATURE_SENSOR_PT_H
#define TEMPERATURE_SENSOR_PT_H

#include <Arduino.h>
#include <Adafruit_MAX31865.h>

class TemperatureSensorPT
{
private:
    Adafruit_MAX31865 thermo;

    static constexpr float RNOMINAL = 100.0f; // PT100 nominal resistance
    static constexpr float RREF = 430.0f;     // MAX31865 reference resistor

public:
    TemperatureSensorPT(int cs_pin, int sdi_pin, int sdo_pin, int sck_pin);

    /**
     * Initialize the sensor
     * Sets up the analog pin and reference voltage
     * Call this in setup()
     */
    void begin();

    /**
     * Get temperature in Celsius
     * @return Temperature in °C, or NAN if reading is invalid
     */
    float getTempC();
};

#endif // TEMPERATURE_SENSOR_PT_H