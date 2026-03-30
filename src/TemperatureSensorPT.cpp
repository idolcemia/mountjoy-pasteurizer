#include "TemperatureSensorPT.h"


TemperatureSensorPT::TemperatureSensorPT(int cs_pin, int sdi_pin, int sdo_pin, int sck_pin)
    : thermo(cs_pin, sdi_pin, sdo_pin, sck_pin)
{
}

void TemperatureSensorPT::begin()
{
    thermo.begin(MAX31865_3WIRE); // Assuming 3-wire configuration; change if needed
}

float TemperatureSensorPT::getTempC()
{
    float tempC = thermo.temperature(RNOMINAL, RREF);

    return tempC;
}
