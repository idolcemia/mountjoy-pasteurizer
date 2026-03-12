#pragma once
#include <Arduino.h>

class PasteurizerRelays
{
public:
    PasteurizerRelays(
        int disableSOLO,
        int coolCycleSOLO,
        int operation,
        int wallHeater,
        int pump,
        int chiller);


    void begin();

    // Disable SOLO relay
    void activateDisableSOLO();
    void deactivateDisableSOLO();

    // Cool Cycle SOLO relay
    void activateCoolCycleSOLO();
    void deactivateCoolCycleSOLO();

    // Operation relay
    void activateOperationRelay();
    void deactivateOperationRelay();

    // Wall Heater relay
    void activateWallHeaterRelay();
    void deactivateWallHeaterRelay();

    // Pump relay
    void activatePumpRelay();
    void deactivatePumpRelay();

    // Chiller relay
    void activateChillerRelay();
    void deactivateChillerRelay();

private:
    int _disableSOLO;
    int _coolCycleSOLO;
    int _operation;
    int _wallHeater;
    int _pump;
    int _chiller;

    // Internal helper
    void setRelay(int pin, bool state);
};
