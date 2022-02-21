#ifndef DISPLAYBASE_H
#define DISPLAYBASE_H

#include <Arduino.h>

class DisplayBase
{
public:
    const bool RequiresTimer = true;
    const int TimerIntervalUs = 200;
    const int MaxDimmingSteps = 8;

protected:
    const int GPIO_Blank = 12;
    const int GPIO_Data = 13;
    const int GPIO_Clock = 14;
    const int GPIO_Latch = 15;
    const int Digits = 0;

    volatile int currentDimmingStep = 2;
    volatile int ticksSinceLastWrite = 0;

public:
    virtual bool Initialize() = 0;
    virtual void IRAM_ATTR OnTimer() = 0;                // hw interrupt - all vars need volatile
    virtual void OnTick(uint8_t displayModeDelay) = 0;   // normal cpu cycle
    virtual void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;    // Time display as HR:MIN:SEC
    virtual void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;        // Time display as HR:MIN
    virtual void ShiftText(String text) = 0;
    virtual void ShiftBlank() = 0;
    virtual void SetDimmingStep(int value) { currentDimmingStep = value; }
};
 

#endif //DISPLAYBASE_H