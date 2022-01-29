#include "DisplayBase_SevenSegmentVFD.h"
#include "binary.h"

/*
    DisplayIV12 - Noctiluca
    - IV-12 is a seven-segment display like IV-6, but lacks the dot.
    - 4 tubes.
    - Grid pin is wired to dot. if blanking, we turn off the grid too.
*/
class DisplayIV12 : public DisplayBase_SevenSegmentVFD
{
protected:
    const int Digits = 4;

    volatile byte displayData[4];

public:
    bool Initialize();
    void IRAM_ATTR OnTick();
    void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftRaw(byte data[]);
    void ShiftText(String text);
    void ShiftBlank();
};