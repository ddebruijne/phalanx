#include "DisplayBase_SevenSegmentVFD.h"
#include "binary.h"

// Time position for when we don't display seconds.
// Rotating between these options gives the components some downtime and should extend lifetime.
enum TimePosition {
    Middle,
    Left,
    Right,
    Split,
    MAX
};

/*
    DisplayIV6 - Phalanx
    - IV-6 is a seven-segment display with dot, so fits perfectly in a byte. Last byte is the dot.
    - 6 tubes.
*/
class DisplayIV6 : public DisplayBase_SevenSegmentVFD
{
protected:
    const int Digits = 6;
    TimePosition currentTimePosition = TimePosition::Middle;    // for when there's less numbers on screen than there are tubes.

    int lastHour = -1;
    int shiftOutIndex = 0;
    volatile byte displayData[6];

public:
    bool Initialize() override;
    void IRAM_ATTR OnTick();
    void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftRaw(byte data[]);
    void ShiftText(String text);
    void ShiftBlank();

private:
    void InternalShiftTimeComponent(int number, bool displayZeroFirstDigit, bool dotOnSecondDigit);
    void InternalShiftOut(byte data);   // emulates shiftOut() with shiftOutIndex on displayData.
};