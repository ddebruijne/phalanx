#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Timezone.h>

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

static Timezone* Timezones[6] = 
{
    &tzUTC,
    &tzUK,
    &tzCE,
    &tzMSK,
    &tzAusET,
    &tzUsET,
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

//TODO: move to DisplayIV6
class TubeCharacter
{
public:
    static const uint8_t h = B00101110;
    static const uint8_t E = B10011110;
    static const uint8_t L = B00011100;
    static const uint8_t l = B00001100;
    static const uint8_t o = B00111010;
    static const uint8_t c = B00011010;
    static const uint8_t n = B00101010;
    static const uint8_t F = B10001110;
    static const uint8_t dot = B00000001;
    static const uint8_t blank = B00000000;
};

#endif // CONSTANTS_H