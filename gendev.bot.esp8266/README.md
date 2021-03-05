# gendev.bot.esp8266 Arduino IDE code
Code to flash on the NodeMCU compatible ESP8266.

Requires to install ESP8266 boards in Arduino IDE.

Libraries
* NewPing for ultra-sonic sensor (USS)
* ArduinoJson for parsing JSON responses

## Functionality
* reads distance using an USS
* sends a HTTP GET request http://host:8080/robot?distance=123
* reads JSON response with fields
  * rightM in {FWD, STP, BWD}
  * leftM in {FWD, STP, BWD}
* powers two or four motors via H-bridge

