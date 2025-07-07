#include "DeviceMode.h"
#include <SpotifyArduino.h>

class DeviceModeSpotify : public DeviceMode
{
    const int timeBetweenUpdates = 30000;
    const char* market = "NL";

    unsigned long lastUpdatedMillis = 0;
    unsigned long timeSinceUpdate = 0;
    bool hasValidAccessToken = false;
    unsigned long currentSongTime = 0;   // updated with timeSinceUpdate

public:
    SpotifyArduino* spotify;
    static CurrentlyPlaying currentlyPlaying;

    bool Start();
    bool Stop();
    void OnTick();
    void OnSerialDataReceived(String s) {};
    EDeviceMode GetDeviceMode() { return EDeviceMode::Spotify; };
};