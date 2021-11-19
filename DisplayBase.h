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
protected:
    const int Digits = 0;
    TimePosition CurrentTimePosition = TimePosition::Middle;

public:
    virtual bool Initialize() = 0;
    virtual void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;    // Time display as HR:MIN:SEC
    virtual void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;        // Time display as HR:MIN
    virtual void ShiftRaw(char data[]) = 0;
    virtual void ShiftBlank() = 0;
};
 

#endif //DISPLAYBASE_H