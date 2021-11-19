#ifndef DISPLAYBASE_H
#define DISPLAYBASE_H

class DisplayBase
{
    const int digits = 0;

public:
    virtual bool Initialize() = 0;
    virtual void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;    // Time display as HR:MIN:SEC
    virtual void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;        // Time display as HR:MIN
    virtual void ShiftRaw(char data[]) = 0;
    virtual void ShiftBlank() = 0;
};
 

#endif //DISPLAYBASE_H