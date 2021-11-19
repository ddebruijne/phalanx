#ifndef DISPLAYBASE_H
#define DISPLAYBASE_H

class DisplayBase
{
public:
    virtual bool Initialize() = 0;
    virtual void ShiftCurrentTimeFull(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;    // Time display as HR:MIN:SEC
    virtual void ShiftCurrentTime(int hour, int minute, int second, bool displayZeroFirstDigit) = 0;        // Time display as HR:MIN
    virtual void ShiftRaw(char data[]);
};
 

#endif //DISPLAYBASE_H