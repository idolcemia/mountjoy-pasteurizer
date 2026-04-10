#include "HeatControl.h"
#include "PasteurizerRelays.h"
#include "ui/ui.h"
#include "lvgl.h"
// #include "TemperatureSensorDallas.h"
#include <math.h>

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

    _state = HeatControlState::HC_DONE;
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
}
void HeatControl::pause()
{
    _startingTemp = _temp; // Reset starting temp to current temp to allow for resumption without large jumps in UI
    _state = HeatControlState::HC_PAUSE;
    _relays.deactivateWallHeaterRelay();
    lv_obj_clear_state(ui_WallHeater, LV_STATE_CHECKED);
    _pauseStartTime = millis();
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


    _state = HeatControlState::HC_DONE;
}
void HeatControl::reset()
{
    stop();
    updateUI(); // Update UI to reflect reset state
}
float HeatControl::getTempC()
{
    _temp = probeTemperatureSensor.getTempC();

    // If set point reached or exceeded, transition to HOLD state
    if (_temp >= _tempSetPoint)
    {
        pause();
    }
    if (_state == HeatControlState::HC_PAUSE)
    {
        // If we're in the PAUSE state and the hold time has elapsed, transition to DONE
        if (millis() - _pauseStartTime >= _holdTimeMinutes * 60 * 1000)
        {
            _state = HeatControlState::HC_DONE;
            stop();
        }
    }
    else if (_state == HeatControlState::HC_RUN)
    {
        // If we're in the RUN state and the temperature has dropped significantly below the set point, pause to prevent overshooting
        if (_temp < _tempSetPoint - _tempAllowedDeviation)
        {
            start(); // Resume heating if temp drops below set point minus allowed deviation
        }
    }
    return _temp;
}

float HeatControl::updateUI()
{

    float tempPercent = min(100.0, max(0.0, (_temp - _startingTemp) / (_tempSetPoint - _startingTemp) * 100.0));
    //  lv_slider_set_value(ui_sliderHeat, int(tempPercent), LV_ANIM_OFF);
    // lv_arc_set_value(ui_arcHeat, int(_temp));
    // lv_label_set_text(ui_labelTempValue, String(_temp, 1).c_str());

    return _temp;
}