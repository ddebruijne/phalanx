#ifndef EEPROMDATA_H
#define EEPROMDATA_H

#define DATAVER 1

class EEPROMData {
public:
    bool initialized = false;
    int version = 1;

    // Global settings
    char wifi_ssid[32] = "";
    char wifi_pass[32] = "";
    int dimmingStep = 0;

    // Normal/Time mode settings
    int timeZone = 0;
    int activeHours[2] = {0, 0};
    bool time_12hmode = false;
    bool time_displayZeroFirstDigit = true;
    bool time_displaySeconds = true;
};

#endif // EEPROMDATA_H