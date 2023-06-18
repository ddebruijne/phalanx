#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Timezone.h>
#include "Secret.h"

// Australia Eastern Time Zone (Sydney, Melbourne)
static TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
static TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours
static Timezone tzAusET(aEDT, aEST);

// Moscow Standard Time (MSK, does not observe DST)
static TimeChangeRule msk = {"MSK", Last, Sun, Mar, 1, 180};
static Timezone tzMSK(msk);

// Central European Time (Frankfurt, Paris)
static TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
static TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
static Timezone tzCE(CEST, CET);

// United Kingdom (London, Belfast)
static TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
static TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
static Timezone tzUK(BST, GMT);

// UTC
static TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC
static Timezone tzUTC(utcRule);

// US Eastern Time Zone (New York, Detroit)
static TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
static TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
static Timezone tzUsET(usEDT, usEST);

struct TimezoneSetting {
public:
    Timezone* timezone;
    String Name;
};

const int AmountTimezones = 6;
static TimezoneSetting Timezones[AmountTimezones] = 
{
    { &tzUTC, String("UTC") },
    { &tzUK, String("Britain, Portugal") },
    { &tzCE, String("Central Europe") },
    { &tzMSK, String("Moscow") },
    { &tzAusET, String("Australia Eastern") },
    { &tzUsET, String("US Eastern") },
};

static String WifiStatusCode[8]
{
    String("Idle"),
    String("SSID Can't be reached"),
    String(""),
    String("Connected Successfully"),
    String("Connection Failed."),
    String(""),
    String("Wrong Password"),
    String("Not connected.")
};

class SpotifyApiConstants 
{
public:  
    static constexpr char SpotifyScope[] = "user-read-playback-state";
    static constexpr char CallbackUri[] = "http%3A%2F%2Fphalanx.local%2FSpotifyCallback";

    static void GetAuthUri(String& str)
    {
        str = "https://accounts.spotify.com/authorize?client_id=";
        str += SpotifyClientId;
        str += "&response_type=code&redirect_uri=";
        str += CallbackUri;
        str += "&scope=";
        str += SpotifyScope;
    }
};

class FinnhubConstants {
public:
    const char apiBaseUrl[31] = "https://finnhub.io/docs/api/v1";
    const char stockUrl[15] = "/quote?symbol=";
    const char tokenSuffix[8] = "&token=";
};

static void SetPinLow(uint8_t pin) { GPOC = (1 << pin); }
static void SetPinHigh(uint8_t pin) { GPOS = (1 << pin); }

#endif // CONSTANTS_H