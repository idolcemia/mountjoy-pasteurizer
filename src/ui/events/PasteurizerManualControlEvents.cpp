#include <Arduino.h>
#include "PasteurizerManualControlEvents.h"
#include "events.h"
#include "ui/ui.h"
#include "Globals.h"
#include "ui/screens/labels/ui_GlobalLabels.h"
#include "ui/screens/ui_GlobalButtons.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // Helper function to log button state change
    static void logButtonState(const char *name, bool state)
    {
        logger.info("[PasteurizerManualControlEvents] " + String(name) +
                    " button " + (state ? "enabled" : "disabled"));
    }

    /* ============================================================
       Disable SOLO Button Events
       ============================================================ */
    void ui_event_DisableSOLOEnabled(lv_event_t *e)
    {
        logButtonState("Disable SOLO", true);
        pasteurizerRelays.activateDisableSOLO();
    }

    void ui_event_DisableSOLODisabled(lv_event_t *e)
    {
        logButtonState("Disable SOLO", false);
        pasteurizerRelays.deactivateDisableSOLO();
    }

    /* ============================================================
       Cool Cycle SOLO Button Events
       ============================================================ */
    void ui_event_CoolCycleSOLOEnabled(lv_event_t *e)
    {
        logButtonState("Cool Cycle SOLO", true);
        pasteurizerRelays.activateCoolCycleSOLO();
    }

    void ui_event_CoolCycleSOLODisabled(lv_event_t *e)
    {
        logButtonState("Cool Cycle SOLO", false);
        pasteurizerRelays.deactivateCoolCycleSOLO();
    }

    /* ============================================================
       Operation Button Events
       ============================================================ */
    void ui_event_OperationButtonEnabled(lv_event_t *e)
    {
        logButtonState("Operation", true);
        pasteurizerRelays.activateOperationRelay();
    }

    void ui_event_OperationButtonDisabled(lv_event_t *e)
    {
        logButtonState("Operation", false);
        pasteurizerRelays.deactivateOperationRelay();
    }

    /* ============================================================
       Wall Heater Button Events
       ============================================================ */
    void ui_event_WallHeaterEnabled(lv_event_t *e)
    {
        logButtonState("Wall Heater", true);
        pasteurizerRelays.activateWallHeaterRelay();
    }

    void ui_event_WallHeaterDisabled(lv_event_t *e)
    {
        logButtonState("Wall Heater", false);
        pasteurizerRelays.deactivateWallHeaterRelay();
    }

    /* ============================================================
       Pump Button Events
       ============================================================ */
    void ui_event_PumpButtonEnabled(lv_event_t *e)
    {
        logButtonState("Pump", true);
        pasteurizerRelays.activatePumpRelay();
    }

    void ui_event_PumpButtonDisabled(lv_event_t *e)
    {
        logButtonState("Pump", false);
        pasteurizerRelays.deactivatePumpRelay();
    }

    /* ============================================================
       Chiller Button Events
       ============================================================ */
    void ui_event_ChillerButtonEnabled(lv_event_t *e)
    {
        logButtonState("Chiller", true);
        pasteurizerRelays.activateChillerRelay();
    }

    void ui_event_ChillerButtonDisabled(lv_event_t *e)
    {
        logButtonState("Chiller", false);
        pasteurizerRelays.deactivateChillerRelay();
    }

    /* ============================================================
       Generic button handler
       ============================================================ */
    void handleManualControlButton(
        lv_event_t *e,
        lv_obj_t *button, bool &state,
        void (*onEnabled)(lv_event_t *),
        void (*onDisabled)(lv_event_t *))
    {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED)
        {
            state = !state;

            // Update button color
            lv_color_t color = state
                                   ? lv_color_hex(0x28A745)  // green
                                   : lv_color_hex(0xDC3545); // red

            lv_obj_set_style_bg_color(button, color, LV_PART_MAIN);

            // Call appropriate event
            if (state && onEnabled)
                onEnabled(e);

            if (!state && onDisabled)
                onDisabled(e);
        }
    }

#ifdef __cplusplus
} /* extern "C" */
#endif
