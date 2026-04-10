#include "TemperatureSensorPT.h"

TemperatureSensorPT::TemperatureSensorPT(int cs_pin, int sdi_pin, int sdo_pin, int sck_pin)
    : thermo(cs_pin, sdi_pin, sdo_pin, sck_pin)
{
}

void TemperatureSensorPT::begin()
{
    thermo.begin(MAX31865_3WIRE); // Assuming 3-wire configuration; change if needed

    for (int i = 0; i < READING_AVG_WINDOW; ++i)
    {
        _recentReading[i] = 0.0f;
    }
}

float TemperatureSensorPT::getTempC()
{
    float tempC = thermo.temperature(RNOMINAL, RREF);

    // Update the circular buffer for smoothing
    _recentReadingSum -= _recentReading[_recentReadingIndex];
    _recentReading[_recentReadingIndex] = tempC;
    _recentReadingSum += tempC;
    _recentReadingIndex = (_recentReadingIndex + 1) % READING_AVG_WINDOW;
    if (_recentReadingCount < READING_AVG_WINDOW)
    {
        _recentReadingCount++;
    }

    return _recentReadingSum / _recentReadingCount;
}
