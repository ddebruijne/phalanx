#ifndef CONSTANTS_H
#define CONSTANTS_H

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