# Phalanx
Firmware designed for projects with an ESP-M2 driving VFD or Nixie vacuum tubes

![VFD Tube](https://i.imgur.com/6GI5cyK.jpg)

## Implemented features
- Configuration mode - will broadcast a WiFi network to set the device up if none is defined.
- Support for multiple types of hardware
  * Currently implemented is a 6xIV-6 VFD direct drive setup.
  * New class can be inherited from DisplayBase for other boards as desired
- Configuration stored on-device.
- Timezones
- Dimming
- Time Display
  * 12/24h mode
  * Toggle seconds display
  * Toggle first zero display if first digit ("07:13:09" vs " 7:13: 9")
  * Active Hours (tubes on between hours x and y)
- Spotify Now Playing - Can display the progress in the song you're currently listening to
- Live updating config (except WiFi, needs a power cycle)
- Shifting text (IV-6)

<br/>

## Using the firmware
1. Flash the firmware. After its done the tubes should show "CONF..."
2. Connect to the wireless network that is set up
3. Visit http://phalanx.local/ or http://192.168.0.1/ from your browser
4. Enter wireless credentials and hit save
5. Do a power cycle
6. Phalanx will attempt to connect to your network and the internet. "CONN..." is displayed. 
   1. If "CONF..." is displayed after a while, network or internet connection failed. Go back to step 2 or try again.
7. Done! The current time should now be displayed.

Spotify setup is only available if you are already connected to a network. <br/>Visit http://phalanx.local/ again from your network or use a networking tool like Fing to find the IP address.

<br />

## Backlog, idea list
- Spotify display time improvements, so it doesn't lag during web request.
- DisplayBase add ShiftMusicTime to shift HHH:SS for music.
- defining & writing text string to display
- date display (at x intervals)
- Animations
- Active based on device on network. / https://github.com/dancol90/ESP8266Ping
- Handle losing internet connection in DeviceModes
- Smooth fading between states, so numbers don't immediately transform.
- Brightness for individual tubes, sometimes a tube is brighter than the others and this could offset it.
- Take another look at DisplayIV6::OnTick - can we avoid unneccessary shifting on full brightness, or outside of active hours?
- Prettier config site, or create a mobile app
- OTA Updates

<br />

## Bugs
- Seems to be an occasional reset, something with hw interrupt?
- Had a couple of instances where the time drifted from real time

<br />

## Libraries
- [SPI](https://www.arduino.cc/en/Reference/SPI): with transfer(data), transfer(*buf, count), transferFast(data), setDataBits(bits) functions having the IRAM_ATTR macro added for [interrupt safety (IRAM_ATTR replaces ICACHE_RAM_ATTR)](https://stackoverflow.com/questions/58113937/esp8266-arduino-why-is-it-necessary-to-add-the-icache-ram-attr-macro-to-isrs-an)
- [Timezone](https://github.com/JChristensen/Timezone)
- [ESP8266mDNS](https://www.arduino.cc/reference/en/libraries/esp8266_mdns/)
- [ESP8266WebServer](https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html)
- [ESP8266TimerInterrupt](https://github.com/khoih-prog/ESP8266TimerInterrupt)
- [ESP8266WiFi](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html)
- [ESP8266Ping](https://github.com/dancol90/ESP8266Ping)
- [ESP_EEPROM](https://www.arduino.cc/reference/en/libraries/esp_eeprom/) / [Sample](https://github.com/jwrw/ESP_EEPROM/blob/master/examples/ESP_EEPROM_Simple/ESP_EEPROM_Simple.ino)
- [SpotifyArduino](https://github.com/witnessmenow/spotify-api-arduino), modified getCurrentlyPlaying function to pass in CurrentlyPlaying class ref instead of callback.

<br />

## Other resources used
- https://arduino-esp8266.readthedocs.io/en/latest/index.html
- https://github.com/mcer12/Flora-ESP8266
- Various other undocumented online sources on any type of VFD project I could find.

<br />

## secret.h
Is a file not included in source control to contain the following variables:
```
#ifndef SECRET_H
#define SECRET_H

const char SpotifyClientId[] = "";
const char SpotifyClientSecret[] = "";
const char TwelveDataSecret[] = "";

#endif // SECRET_H
```

<br />

# MIT License
Copyright 2021 Danny de Bruijne

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
