#ifndef DISPLAYBASE_H
#define DISPLAYBASE_H

class DisplayBase
{
public:
    virtual bool Initialize() = 0;
    virtual void ShiftCurrentTime(int hour, int minute, int second) = 0;
    virtual void ShiftRaw(char data[]);
};
 

#endif //DISPLAYBASE_H