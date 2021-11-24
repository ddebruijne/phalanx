# Phalanx
Firmware designed for projects with an ESP-M2 driving VFD or Nixie vacuum tubes

# Resources
- https://arduino-esp8266.readthedocs.io/en/latest/index.html
- https://github.com/mcer12/Flora-ESP8266
- https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
- https://github.com/jwrw/ESP_EEPROM/blob/master/examples/ESP_EEPROM_Simple/ESP_EEPROM_Simple.ino
- https://github.com/witnessmenow/spotify-api-arduino

# To Do List
- date display (at x intervals)
- spotify device mode & mode switching
- Take another look at DisplayIV6::OnTick - can we avoid unneccessary shifting on full brightness for example?

# Backlog features
- defining & writing string to display
- Animations
- Active based on device on network. / https://github.com/dancol90/ESP8266Ping
- Brightness for individual tubes, sometimes 
- OTA Updates

# Libraries
- SPI: with transfer(data), transfer(*buf, count), transferFast(data), setDataBits(bits) functions having the IRAM_ATTR macro added for [interrupt safety (IRAM_ATTR replaces ICACHE_RAM_ATTR)](https://stackoverflow.com/questions/58113937/esp8266-arduino-why-is-it-necessary-to-add-the-icache-ram-attr-macro-to-isrs-an)
- Timezone
- ESP8266mDNS
- ESP8266WebServer
- ESP8266TimerInterrupt
- ESP8266WiFi
- ESP_EEPROM

# MIT License
Copyright 2021 Danny de Bruijne

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.