#ifndef CHILL_CONTROL_H
#define CHILL_CONTROL_H

#include <Arduino.h>
#include "TemperatureSensorPT.h"

class PasteurizerRelays;

extern TemperatureSensorPT probeTemperatureSensor;
enum ChillControlState
{
    CC_RESET = 0,
    CC_RUN = 1,
    CC_PAUSE = 2,
    CC_DONE = 3
};

/**
 * Controls cooler.
 *
 * CIRCUIT CONFIGURATION:
 * HOW IT WORKS:
 */

class ChillControl
{
private:
    PasteurizerRelays &_relays;
    float _tempSetPoint;
    float _glycolChillSetPoint;
    float _temp;
    float _startingTemp;
    const float _tempAllowedDeviation = 1.0; // Maximum deviation from baseline allowed.
    // TemperatureSensorDallas tempSensor; // Dallas temperature sensor instance

public:
    /**
     * Constructor
     *
     */

    // OneWire oneWire;
    // DallasTemperature dallasTemperature;

    ChillControlState _state;
    void begin();

    ChillControl(
        PasteurizerRelays &relays,
        float tempSetPoint = 9,
        float glycolChillSetPoint = 18);

    /**
     * Start cooling
     */
    void start();

    /**
     * Stop cooling
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

    /**
     * Update display elements based on current temperature and state. Returns current temperature for convenience.
     */
    float updateUI();
};

#endif
