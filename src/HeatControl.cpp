#include "HeatControl.h"
#include "PasteurizerRelays.h"
#include "AudioSlave.h"
#include "ui/ui.h"
#include "lvgl.h"
#include <math.h>
#include "Globals.h"

extern AudioSlave audioSlave;

// Initialize static member variable
unsigned long HeatControl::_pauseStartTime = 0;

// ---------------- Constructor ----------------
HeatControl::HeatControl(
    PasteurizerRelays &relays,
    float tempSetPoint,
    float holdTimeMinutes)
    : _relays(relays), _tempSetPoint(tempSetPoint), _holdTimeMinutes(holdTimeMinutes)
{
}

void HeatControl::begin()
{

    _state = HeatControlState::HC_RESET;
    _temp = NAN;
    _startingTemp = NAN;
}

void HeatControl::start()
{

    _relays.activateFloodPumpRelay();
    _relays.activateRecirculationPumpRelay();
    _relays.activateWallHeaterRelay();
    _startingTemp = getTempC(); // Capture starting temperature for deviation checks
    _state = HeatControlState::HC_RUN;
    lv_obj_add_state(ui_switchOperation, LV_STATE_CHECKED);
    lv_obj_add_state(ui_switchPump, LV_STATE_CHECKED);
    lv_obj_add_state(ui_WallHeater, LV_STATE_CHECKED);
    lv_bar_set_value(ui_setPointBar, int(_tempSetPoint), LV_ANIM_OFF);
    lv_label_set_text(ui_setPointTemp, String(_tempSetPoint).c_str());
}

void HeatControl::pause()
{
    _state = HeatControlState::HC_PAUSE; // Turn off heater but keep recirculation pump running to cool the heater core.
    _relays.deactivateWallHeaterRelay();
    lv_obj_clear_state(ui_WallHeater, LV_STATE_CHECKED);
}

void HeatControl::resume()
{
    _state = HeatControlState::HC_RUN; // Resume heating
    _relays.activateWallHeaterRelay();
    lv_obj_add_state(ui_WallHeater, LV_STATE_CHECKED);
}
void HeatControl::stop()
{
    //   lv_obj_clear_state(ui_switchHeat, LV_STATE_CHECKED);
    //   lv_slider_set_value(ui_sliderHeat, 0, LV_ANIM_OFF);
    _relays.deactivateWallHeaterRelay();
    _relays.deactivateRecirculationPumpRelay();
    _relays.deactivateFloodPumpRelay();
    lv_obj_clear_state(ui_switchOperation, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_switchPump, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_WallHeater, LV_STATE_CHECKED);
    lv_bar_set_value(ui_setPointBar, 0, LV_ANIM_OFF);
}
void HeatControl::reset()
{
    stop();
    _state = HeatControlState::HC_RESET;
    updateUI(); // Update UI to reflect reset state
}
float HeatControl::getTempC()
{
    _temp = probeTemperatureSensor.getTempC();

    return _temp;
}

void HeatControl::processControl()
{

    float floodTemp = floodTemperatureSensor.getTempC();

    _temp = getTempC();

    if (floodTemp >= 60.0)
    {

        // In the latter stage of heating, if the flood temperature exceeds the probe temperature by more than the allowed deviation,
        // pause heating to prevent overshooting and potential safety hazard.
        // Once we're below that threshold again, transition back to RUN state if we're not already there.

        if (floodTemp - _temp > _tempAllowedDeviation)
        {
            if (_state == HeatControlState::HC_RUN)
            {
                pause();
            }
        }
        else if (_state == HeatControlState::HC_PAUSE && floodTemp - _temp <= _tempAllowedDeviation * 0.75) // Add some hysteresis to prevent rapid toggling
        {
            resume();
        }
    }

    // If setpoint is reached, transition to DONE state and stop heating.
    // We allow the user to set a hold time, so we don't immediately transition to DONE when the setpoint is reached.
    // Instead, we start a timer and only transition to DONE if we've been above the setpoint for the duration of the hold time. This allows for some overshoot while still ensuring we hold at the target temperature for the desired amount of time.
    if (_temp >= _tempSetPoint)
    {
        pause();
        _pauseStartTime = _pauseStartTime == 0 ? millis() : _pauseStartTime;

        logger.info("MS Elapsed: " + String(millis() - _pauseStartTime) + " hold for " + String(_holdTimeMinutes * 60000));

        if ((millis() - _pauseStartTime) >= (_holdTimeMinutes * 60000))
        {
            _pauseStartTime = 0; // Reset the pause start time
            _state = HeatControlState::HC_DONE;
            stop();
        }
    }
}

float HeatControl::updateUI()
{

    float tempPercent = min(100.0, max(0.0, (_temp - _startingTemp) / (_tempSetPoint - _startingTemp) * 100.0));

    //  lv_slider_set_value(ui_sliderHeat, int(tempPercent), LV_ANIM_OFF);
    // lv_arc_set_value(ui_arcHeat, int(_temp));
    // lv_label_set_text(ui_labelTempValue, String(_temp, 1).c_str());

    return _temp;
}