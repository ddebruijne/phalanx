#ifndef DISPLAYBASE_H
#define DISPLAYBASE_H

// Time position for when we don't display seconds.
// Rotating between these options gives the components some downtime and should extend lifetime.
enum TimePosition {
    Middle,
    Left,
    Right,
    Split,
    MAX
};

class DisplayBase
{
public:
    const bool RequiresTimer = false;
    const int TimerIntervalUs = 200;
    const int MaxDimmingSteps = 10;

protected:
    const int Digits = 0;
    TimePosition currentTimePosition = TimePosition::Middle;    // for when there's less numbers on screen than there are tubes.

public:
    virtual bool Initialize() = 0;
    virtual void OnTick(int deltaTime) {};
    virtual void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;    // Time display as HR:MIN:SEC
    virtual void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;        // Time display as HR:MIN
    virtual void ShiftRaw(char data[]) = 0;
    virtual void ShiftBlank() = 0;
};
 

#endif //DISPLAYBASE_H