# LYT-SCRIPTED
An alternative firmware for LYT8266 RGBW LED lamps

Features:
- Switches on almost immediatly after powering the bulb, no 500 ms delay
- Does not use the regular LYT8266 library which contained a lot of code
- Features an iOS Webapp Icon to give the native look and feel of an App on iOS
- Nice colorpicker that automatically switches to white LEDs
- can poll a webserver for colors (example JSON: http://lyt.naaa.de)
- startup behaviour can be configured (Static color or polling a webserver)
- uses a jQuery Mobile Interface
- Several clients can control the lamp in parallel
- changes to firmware are only possible after accessing a special URL
- allows HTTP Update of firmware
- provides a FiFo log of latest 100 Log-messages accessible at http://lyt8266/log

[![IMAGE ALT TEXT](http://img.youtube.com/vi/ZmReog0DOrA/0.jpg)](http://www.youtube.com/watch?v=ZmReog0DOrA "LYT-Scripted")
