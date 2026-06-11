#include "PasteurizerRelays.h"
#include "HeatControl.h"
#include "ChillControl.h"

PasteurizerRelays::PasteurizerRelays(
    int chillerB,
    int freshWater,
    int floodPump,
    int wallHeater,
    int recirculationPump,
    int chiller)
    : _chillerB(chillerB),
      _freshWater(freshWater),
      _floodPump(floodPump),
      _wallHeater(wallHeater),
      _recirculationPump(recirculationPump),
      _chiller(chiller),
      _cycle(0),
      _heatControl(nullptr),
      _chillControl(nullptr)
{
}

void PasteurizerRelays::attachControllers(HeatControl *heatControl, ChillControl *chillControl)
{
    _heatControl = heatControl;
    _chillControl = chillControl;
}

void PasteurizerRelays::begin()
{
    pinMode(_chillerB, OUTPUT);
    pinMode(_freshWater, OUTPUT);
    pinMode(_floodPump, OUTPUT);
    pinMode(_wallHeater, OUTPUT);
    pinMode(_recirculationPump, OUTPUT);
    pinMode(_chiller, OUTPUT);

    // Set all relays to OFF at startup
    deactivateChillerB();
    deactivateFreshWaterRelay();
    deactivateFloodPumpRelay();
    deactivateWallHeaterRelay();
    deactivateChillerRelay();
    deactivateRecirculationPumpRelay();
}

void PasteurizerRelays::setRelay(int pin, bool state)
{
    // HIGH = OFF  LOW = ON for the chosen relay, so we invert the state when writing to the pin
    digitalWrite(pin, state ? LOW : HIGH);
}

// ---------------- Disable SOLO ----------------
void PasteurizerRelays::activateChillerB() { setRelay(_chillerB, true); }
void PasteurizerRelays::deactivateChillerB() { setRelay(_chillerB, false); }

// ---------------- Fresh Water  ----------------
void PasteurizerRelays::activateFreshWaterRelay() { setRelay(_freshWater, true); }
void PasteurizerRelays::deactivateFreshWaterRelay() { setRelay(_freshWater, false); }

// ---------------- Flood Pump ----------------
void PasteurizerRelays::activateFloodPumpRelay() { setRelay(_floodPump, true); }
void PasteurizerRelays::deactivateFloodPumpRelay() { setRelay(_floodPump, false); }

// ---------------- Wall Heater ----------------
void PasteurizerRelays::activateWallHeaterRelay() { setRelay(_wallHeater, true); }
void PasteurizerRelays::deactivateWallHeaterRelay() { setRelay(_wallHeater, false); }

// ---------------- Recirculation Pump ----------------
void PasteurizerRelays::activateRecirculationPumpRelay() { setRelay(_recirculationPump, true); }
void PasteurizerRelays::deactivateRecirculationPumpRelay() { setRelay(_recirculationPump, false); }

// ---------------- Chiller ----------------
void PasteurizerRelays::activateChillerRelay()
{
    setRelay(_chiller, true);
    setRelay(_chillerB, true);
}
void PasteurizerRelays::deactivateChillerRelay()
{
    setRelay(_chillerB, false);
    setRelay(_chiller, false);
}

// ---------------- Cycle ----------------
void PasteurizerRelays::CycleButtonEnabled()
{
    _cycle = 1; // Set cycle state to enabled
    if (_heatControl != nullptr)
    {
        _heatControl->start();
    }
}
void PasteurizerRelays::CycleButtonDisabled()
{
    _cycle = 0; // Set cycle state to disabled
    if (_heatControl != nullptr)
    {
        _heatControl->stop();
    }
    if (_chillControl != nullptr)
    {
        _chillControl->stop();
    }
}