#ifndef HEAT_CONTROL_H
#define HEAT_CONTROL_H

#include <Arduino.h>
#include "TemperatureSensorPT.h"

class PasteurizerRelays;

extern TemperatureSensorPT probeTemperatureSensor, floodTemperatureSensor;

enum HeatControlState
{
    HC_RESET = 0,
    HC_RUN = 1,
    HC_PAUSE = 2,
    HC_DONE = 3
};

/**
 * Controls burner.
 *
 * CIRCUIT CONFIGURATION:
 * HOW IT WORKS:
 */

class HeatControl
{
private:
    PasteurizerRelays &_relays;

    float _tempSetPoint;
    float _temp;
    float _startingTemp;
    unsigned long _holdTimeMinutes;

    const float _tempAllowedDeviation = 5; // If the flood temperature exceeds the probe
                                           // temperature by more than this amount, pause heating to prevent boiling.

public:
    /**
     * Constructor
     *
     */

    // OneWire oneWire;
    // DallasTemperature dallasTemperature;

    HeatControlState _state;
    void begin();

    HeatControl(
        PasteurizerRelays &relays,
        float tempSetPoint = 72,
        float holdTimeMinutes = 1);

    /**
     * Start heating
     */
    void start();

    /**
     * Stop heating
     */
    void stop();

    /**
     * Reset the heat control state and stop heating
     */
    void reset();

    /**
     * Take temperature reading and update state
     */
    float getTempC();

    void processControl();

    /**
     * Update display elements based on current temperature and state. Returns current temperature for convenience.
     */
    float updateUI();

    void pause();

    void resume();
};

#endif
