/*
 * Mountjoy Kettle fill / Temp gauge using the following components:
 * Arduino Giga R1 M7
 * Giga Display Shield (800 x 480)
 * LvGL 9.x
 * 20251125 tested good.
 * Enhanced with WiFi debug, retries, RSSI, MAC, and encryption info
 */

#include <Arduino.h>
#include <ArduinoJson.h>

#include <lv_conf.h>
#include <lvgl.h>

#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include "ui/ui.h"

// --- Internal libs ---
// #include <NetworkManager.h>
// #include <WiFi/WiFiNetworkManager.h>
#include <RumpshiftLogger.h>
#include <WiFi/WiFiClientWrapper.h>
#include <HttpResponse.h>
#include <Users.h>
#include <Diagnostic.h>

#include "config.h"
#include "Globals.h"
#include "ui/screens/labels/ui_GlobalLabels.h"

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;

WiFiClientWrapper testClient;

unsigned long lastTempUpdate = 0;
const unsigned long TEMP_UPDATE_INTERVAL = 1000;

void _log();

void setup()
{
    logger.begin();
    logger.info("Starting log....");

    network->setRemote(server, port);
    // network->begin();
    // network->printStatus();

    // wifiClient.begin(&logger);
    // users = new Users(wifiClient, &logger);

    initGlobals();
    // testClient.begin(&logger);
    Display.begin();
    TouchDetector.begin();
    ui_init();
}

void loop()
{
    lv_timer_handler();
    // ui_GlobalLabels::updateNetworkStatus();
    // ui_GlobalLabels::networkChecked();

    // Only update once LVGL is stable
    // Maybe move this to particular screens
    // if (millis() > 3000) // wait 3s after boot
    // {
    //     Diagnostic::updateDefault();
    // }

    // Update temperature display periodically
    unsigned long currentMillis = millis();
    if (currentMillis - lastTempUpdate >= TEMP_UPDATE_INTERVAL)
    {
        lastTempUpdate = currentMillis;

        // Read temperatures from sensors
        float probeTemp = probeTemperatureSensor.getTempC();
        float floodTemp = floodTemperatureSensor.getTempC();

        // At 72 degrees probe temp, turn off heater and pump, and 
        // turn on the fresh water flood to begin cooling cycle.
        if (probeTemp == 72)
        {
            lv_obj_clear_state(ui_switchPump, LV_STATE_CHECKED);
            lv_obj_clear_state(ui_WallHeater, LV_STATE_CHECKED);
            lv_obj_add_state(ui_CoolSOLO, LV_STATE_CHECKED);
        }

        // If flood temperature exceeds 78 degrees, turn the wall heater off.
        if (floodTemp > 78)
        {
            lv_obj_clear_state(ui_WallHeater, LV_STATE_CHECKED);
        }

        logger.info("[MAIN] Flood temp: " + String(floodTemp));
        logger.info("[MAIN] Probe temp: " + String(probeTemp));
        // Update the UI
        lv_label_set_text(ui_floodTemp, String(floodTemp).c_str());
        lv_label_set_text(ui_probeTemp, String(probeTemp).c_str());
    }

    delay(5); // Small delay for stability
}

void _log()
{
    StaticJsonDocument<512> doc;

    // Database ID
    // doc["database_id"] = DB_ID;

    // Title property
    doc["Entry"] = "Log Entry 1";

    // Number property
    doc["Temperature (C)"] = 23.5;

    // Timestamp property (Notion date)
    // char timestampStr[25];
    // unsigned long now = millis() / 1000;
    // snprintf(timestampStr, sizeof(timestampStr), "2025-10-30T21:%02lu:00Z", (now / 60) % 60);

    // doc["Timestamp"] = timestampStr;

    // Serialize JSON
    String output;
    serializeJson(doc, output);

    // POST
    const char *host = "3.136.200.179";
    const int port = 8000;
    const char *url = API_PATH;
    logger.info("[HTTP_LOG] " + output);

    // HttpResponse resp = wifiClient.http().post(host, port, url, output);
    // logger.info("[POST RESPONSE - FULL] " + resp.full());
}