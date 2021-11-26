#include "DeviceModeSpotify.h"

bool DeviceModeSpotify::Start()
{
    hasValidAccessToken = spotify->checkAndRefreshAccessToken();
    if(!hasValidAccessToken)
    {
        Serial.println("DeviceModeSpotify: CheckAndRefreshAccessToken failed.");
        return hasValidAccessToken;
    }
}

bool DeviceModeSpotify::Stop()
{
    return true;
}

void DeviceModeSpotify::OnTick()
{
    timeSinceUpdate = millis() - lastUpdatedMillis;
    if (currentlyPlaying.isPlaying)
        currentSongTime = currentlyPlaying.progressMs + timeSinceUpdate;

    unsigned long totalSeconds = currentSongTime / 1000;
    int hrs = totalSeconds / 3600;
    int secsRemaining = totalSeconds % 3600;
    int mins = secsRemaining / 60;
    int secs = secsRemaining % 60;

    display->ShiftCurrentTimeFull(hrs, mins, secs, true);

    if (timeSinceUpdate < timeBetweenUpdates || !hasValidAccessToken)
        return;

    int status = spotify->getCurrentlyPlaying(currentlyPlaying);
    if (status == 200)
        currentSongTime = currentlyPlaying.progressMs;

    lastUpdatedMillis = millis();
    timeSinceUpdate = 0;
}
