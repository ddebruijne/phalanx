#ifndef EEPROMDATA_H
#define EEPROMDATA_H

#define DATAVER 2

class EEPROMData {
public:
    bool initialized = false;
    int version = DATAVER;

    // Global settings
    char wifi_ssid[32] = "";
    char wifi_pass[32] = "";
    uint8_t wifi_bssid[6] = {0, 0, 0, 0, 0, 0};
    int dimmingStep = 0;

    // Normal/Time mode settings
    int timeZone = 0;
    int activeHours[2] = {0, 0};
    bool time_12hmode = false;
    bool time_displayZeroFirstDigit = true;
    bool time_displaySeconds = true;

    // Other
    char spotifyRefreshToken[150] = "";

    EEPROMData()
    {
        initialized = false;
        version = DATAVER;
        strcpy(wifi_ssid, "");
        strcpy(wifi_pass, "");
        wifi_bssid[0] = 0;
        wifi_bssid[1] = 0;
        wifi_bssid[2] = 0;
        wifi_bssid[3] = 0;
        wifi_bssid[4] = 0;
        wifi_bssid[5] = 0;
        dimmingStep = 0;
        timeZone = 0;
        activeHours[0] = 0;
        activeHours[1] = 0;
        time_12hmode = false;
        time_displayZeroFirstDigit = true;
        time_displaySeconds = true;
        strcpy(spotifyRefreshToken, "");
    }
};

#endif // EEPROMDATA_H