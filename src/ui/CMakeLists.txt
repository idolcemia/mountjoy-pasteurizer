SET(SOURCES ui_PasteurizerChiller.c
    ui_theme_manager.c
    ui_themes.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    events/events.cpp
    events/PasteurizerManualControlEvents.cpp
    screens/ui_DebugLogScreen.cpp
    screens/ui_GlobalButtons.cpp
    screens/ui_TempControl.cpp
    screens/ui_UserSelectionScreen.cpp
    screens/ui_WiFiConnect.cpp)

add_library(ui ${SOURCES})
