#pragma once
#include <Arduino.h>

// Forward declarations to break circular dependency
class HeatControl;
class ChillControl;

class PasteurizerRelays
{
public:
    PasteurizerRelays(
        int disableSOLO,
        int freshWater,
        int floodPump,
        int wallHeater,
        int recirculationPump,
        int chiller);

    void begin();

    // Disable SOLO relay
    void activateDisableSOLO();
    void deactivateDisableSOLO();

    // Fresh Water relay
    void activateFreshWaterRelay();
    void deactivateFreshWaterRelay();

    // Flood relay
    void activateFloodPumpRelay();
    void deactivateFloodPumpRelay();

    // Wall Heater relay
    void activateWallHeaterRelay();
    void deactivateWallHeaterRelay();

    // Recirculation Pump relay
    void activateRecirculationPumpRelay();
    void deactivateRecirculationPumpRelay();

    // Chiller relay
    void activateChillerRelay();
    void deactivateChillerRelay();

    // Cycle switch events
    void CycleButtonEnabled();
    void CycleButtonDisabled();

    // Inject controller dependencies after construction.
    void attachControllers(HeatControl *heatControl, ChillControl *chillControl);

private:
    int _disableSOLO;
    int _freshWater;
    int _floodPump;
    int _wallHeater;
    int _recirculationPump;
    int _chiller;
    int _cycle;
    HeatControl *_heatControl;
    ChillControl *_chillControl;

    void setRelay(int pin, bool state);
};
