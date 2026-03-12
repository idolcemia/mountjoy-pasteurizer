#include "PasteurizerRelays.h"

PasteurizerRelays::PasteurizerRelays(
    int disableSOLO,
    int coolCycleSOLO,
    int operation,
    int wallHeater,
    int pump,
    int chiller)
    : _disableSOLO(disableSOLO),
      _coolCycleSOLO(coolCycleSOLO),
      _operation(operation),
      _wallHeater(wallHeater),
      _pump(pump),
      _chiller(chiller)
{
}

void PasteurizerRelays::begin()
{
    pinMode(_disableSOLO, OUTPUT);
    pinMode(_coolCycleSOLO, OUTPUT);
    pinMode(_operation, OUTPUT);
    pinMode(_wallHeater, OUTPUT);
    pinMode(_pump, OUTPUT);
    pinMode(_chiller, OUTPUT);
    // Set all relays to OFF at startup
    deactivateDisableSOLO();
    deactivateCoolCycleSOLO();
    deactivateOperationRelay();
    deactivateWallHeaterRelay();
    deactivateChillerRelay();
    deactivatePumpRelay();
}

void PasteurizerRelays::setRelay(int pin, bool state)
{
    // HIGH = ON for most relay shields; adjust if your board is active LOW
    digitalWrite(pin, state ? HIGH : LOW);
}

// ---------------- Disable SOLO ----------------
void PasteurizerRelays::activateDisableSOLO() { setRelay(_disableSOLO, true); }
void PasteurizerRelays::deactivateDisableSOLO() { setRelay(_disableSOLO, false); }

// ---------------- Cool Cycle SOLO ----------------
void PasteurizerRelays::activateCoolCycleSOLO() { setRelay(_coolCycleSOLO, true); }
void PasteurizerRelays::deactivateCoolCycleSOLO() { setRelay(_coolCycleSOLO, false); }

// ---------------- Operation ----------------
void PasteurizerRelays::activateOperationRelay() { setRelay(_operation, true); }
void PasteurizerRelays::deactivateOperationRelay() { setRelay(_operation, false); }

// ---------------- Wall Heater ----------------
void PasteurizerRelays::activateWallHeaterRelay() { setRelay(_wallHeater, true); }
void PasteurizerRelays::deactivateWallHeaterRelay() { setRelay(_wallHeater, false); }

// ---------------- Pump ----------------
void PasteurizerRelays::activatePumpRelay() { setRelay(_pump, true); }
void PasteurizerRelays::deactivatePumpRelay() { setRelay(_pump, false); }

// ---------------- Chiller ----------------
void PasteurizerRelays::activateChillerRelay() { 
    setRelay(_chiller, true); 
    setRelay(_disableSOLO, true); }
void PasteurizerRelays::deactivateChillerRelay() { 
    setRelay(_disableSOLO, false);
    setRelay(_chiller, false); } 