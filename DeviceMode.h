#ifndef DEVICEMODE_H
#define DEVICEMODE_H

class DeviceMode
{
public:
    virtual bool Start() = 0;
    virtual bool Stop() = 0;
    virtual void OnTick() = 0;
};

#endif // DEVICEMODE_H