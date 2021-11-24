#include "DisplayBase.h"
#include "binary.h"

class DisplayIV6 : public DisplayBase
{
public:
    const bool RequiresTimer = true;
    const int TimerIntervalUs = 200;
    const int MaxDimmingSteps = 8;

protected:
    const int Digits = 6;

    const int GPIO_Data = 13;
    const int GPIO_Clock = 14;
    const int GPIO_Latch = 15;

    const char TubeDigit[10] = {
        B11111100, // 0
        B01100000, // 1
        B11011010, // 2
        B11110010, // 3
        B01100110, // 4
        B10110110, // 5
        B10111110, // 6
        B11100000, // 7
        B11111110, // 8
        B11110110  // 9
    };

    int lastHour = -1;
    volatile char displayData[6];
    volatile int ticksSinceLastWrite = 0;
    int currentDimmingStep = 2;     //TODO move to EEPROMData

public:
    bool Initialize();
    void IRAM_ATTR OnTick();
    void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit);
    void ShiftRaw(char data[]);
    void ShiftBlank();

private:
    void InternalShiftTimeComponent(int number, bool displayZeroFirstDigit, bool dotOnSecondDigit);
};