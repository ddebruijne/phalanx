#ifndef EEPROMDATA_H
#define EEPROMDATA_H

#define DATAVER 1

class EEPROMData {
public:
    bool initialized = false;
    int version = 1;
    char wifi_ssid[32] = "";
    char wifi_pass[32] = "";
    bool time_12hmode = false;
    bool time_displayZeroFirstDigit = true;
    bool time_displaySeconds = true;
};

#endif // EEPROMDATA_H