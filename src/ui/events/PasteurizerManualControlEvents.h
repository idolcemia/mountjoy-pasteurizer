#pragma once

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // --- Disable SOLO Button Events ---
    void ui_event_DisableSOLOEnabled(lv_event_t *e);
    void ui_event_DisableSOLODisabled(lv_event_t *e);
    // void ui_event_DisableSOLO(lv_event_t *e);

    // --- Cool Cycle SOLO Button Events ---
    void ui_event_CoolCycleSOLOEnabled(lv_event_t *e);
    void ui_event_CoolCycleSOLODisabled(lv_event_t *e);
    // void ui_event_CoolCycleSOLO(lv_event_t *e);

    // --- Operation Button Events ---
    void ui_event_OperationButtonEnabled(lv_event_t *e);
    void ui_event_OperationButtonDisabled(lv_event_t *e);
    // void ui_event_OperationButton(lv_event_t *e);

    // --- Wall Heater Button Events ---
    void ui_event_WallHeaterEnabled(lv_event_t *e);
    void ui_event_WallHeaterDisabled(lv_event_t *e);
    // void ui_event_WallHeater(lv_event_t *e);

    // --- Pump Button Events ---
    void ui_event_PumpButtonEnabled(lv_event_t *e);
    void ui_event_PumpButtonDisabled(lv_event_t *e);
    // void ui_event_PumpButton(lv_event_t *e);

    // --- Chiller Button Events ---
    void ui_event_ChillerButtonEnabled(lv_event_t *e);
    void ui_event_ChillerButtonDisabled(lv_event_t *e);

        // --- Cycle Button Events ---
    void ui_event_CycleButtonEnabled(lv_event_t *e);
    void ui_event_CycleButtonDisabled(lv_event_t *e);

#ifdef __cplusplus
} /* extern "C" */
#endif
