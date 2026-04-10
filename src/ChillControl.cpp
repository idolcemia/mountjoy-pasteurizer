#include "ChillControl.h"
#include "PasteurizerRelays.h"
#include "ui/ui.h"
#include "lvgl.h"
#include <math.h>

// ---------------- Constructor ----------------
ChillControl::ChillControl(
    PasteurizerRelays &relays,
    float tempSetPoint,
    float glycolChillSetPoint)
    : _relays(relays), _tempSetPoint(tempSetPoint), _glycolChillSetPoint(glycolChillSetPoint)

{
}

void ChillControl::begin()
{

    _state = ChillControlState::CC_DONE;
    _temp = NAN;
    _startingTemp = NAN;
}

void ChillControl::start()
{
    _relays.deactivateChillerRelay(); // Ensure chiller is off before starting flood and recirculation to prevent cold shock
    _relays.activateFreshWaterRelay();
    _relays.activateFloodPumpRelay();
    _startingTemp = getTempC(); // Capture starting temperature for deviation checks
    _state = ChillControlState::CC_RUN;
    // Update UI to reflect new state
    lv_obj_add_state(ui_switchOperation, LV_STATE_CHECKED);
    lv_obj_add_state(ui_CoolSOLO, LV_STATE_CHECKED);
}
void ChillControl::stop()
{
    //  lv_obj_clear_state(ui_switchChill, LV_STATE_CHECKED);
    //  lv_slider_set_value(ui_sliderChill, 0, LV_ANIM_OFF);
    _relays.deactivateChillerRelay();
    _relays.deactivateRecirculationPumpRelay();
    _relays.deactivateFloodPumpRelay();
    _relays.deactivateChillerRelay();
    lv_obj_clear_state(ui_switchOperation, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_CoolSOLO, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_ChillerButton, LV_STATE_CHECKED);
    lv_obj_clear_state(ui_switchPump, LV_STATE_CHECKED);
    _state = ChillControlState::CC_DONE;
}
void ChillControl::reset()
{
    stop();
    updateUI(); // Update UI to reflect reset state
}
float ChillControl::getTempC()
{
    _temp = probeTemperatureSensor.getTempC();

    if (_temp <= _glycolChillSetPoint) // If glycol chill set point reached or exceeded, activate chiller
    {
        _relays.activateChillerRelay();
        _relays.activateRecirculationPumpRelay();
        _relays.deactivateFreshWaterRelay();
        lv_obj_add_state(ui_ChillerButton, LV_STATE_CHECKED);
        lv_obj_add_state(ui_switchPump, LV_STATE_CHECKED);
        lv_obj_clear_state(ui_CoolSOLO, LV_STATE_CHECKED);
    }
    // If set point reached or exceeded, transition to DONE state
    if (_temp <= _tempSetPoint)
    {
        reset();
    }

    return _temp;
}

float ChillControl::updateUI()
{

    float tempPercent = min(100.0, max(0.0, (_temp - _startingTemp) / (_tempSetPoint - _startingTemp) * 100.0));
    // lv_slider_set_value(ui_sliderChill, int(tempPercent), LV_ANIM_OFF);
    // lv_arc_set_value(ui_arcChill, int(_temp));
    //  lv_label_set_text(ui_labelTempValue, String(_temp, 1).c_str());

    return _temp;
}