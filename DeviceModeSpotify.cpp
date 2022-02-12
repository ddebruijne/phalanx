#include "DeviceModeSpotify.h"
#include "defines.h"

bool DeviceModeSpotify::Start()
{
    hasValidAccessToken = spotify->checkAndRefreshAccessToken();
    if(!hasValidAccessToken)
    {
        Serial.println("DeviceModeSpotify: CheckAndRefreshAccessToken failed.");
        #ifdef DISPLAYTYPE_IV4
            display->ShiftText("Login to Spotify...");
        #endif
        return hasValidAccessToken;
    }

    // Ensure we poll for an update immediately
    timeSinceUpdate = timeBetweenUpdates;
}

bool DeviceModeSpotify::Stop()
{
    return true;
}

void DeviceModeSpotify::OnTick()
{
    timeSinceUpdate = millis() - lastUpdatedMillis;

    // Update song time
    if (currentlyPlaying.isPlaying && currentSongTime <= currentlyPlaying.durationMs)
        currentSongTime = currentlyPlaying.progressMs + timeSinceUpdate;

    if (currentSongTime > currentlyPlaying.durationMs)
        currentSongTime = currentlyPlaying.durationMs;

    // manually calculate the time in hrs, min and sec the song is playing for.
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
