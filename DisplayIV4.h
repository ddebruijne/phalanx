#include "DisplayBase.h"

/*
    DisplayIV4 - Ameise
    - IV-12 is a 18-segment display (including dots), including 2 dead pins on the HV5182, we need 20 bits per tube.
    - 8 tubes.
    - TODO: figure out how to define and shift this data correctly.
*/
class DisplayIV12 : public DisplayBase
{
protected:
    const int Digits = 8;
    volatile byte displayData[20];

public:
    bool Initialize();
    void IRAM_ATTR OnTick();
    void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftRaw(byte data[]);
    void ShiftText(String text);
    void ShiftBlank();
};