# Kiddo Blaster
ESP12 + IR + YX5300 MP3 player



An infrared remote controlled MP3 player.
Hardware:

| Component          | Model                | Link  |
| ------------------ |:--------------------:| ------|
| Microcontroller    | NodeMCU v3 (ESP-12E) | https://www.amazon.com/gp/product/B071RNQPHV/ref=oh_aui_search_detailpage?ie=UTF8&psc=1 |
| Infrared receiver  | CHQ1838              |   https://www.amazon.com/gp/product/B00EFOQEUM/ref=oh_aui_search_detailpage?ie=UTF8&psc=1 |
| MP3 player         | YX5300               |    https://www.amazon.com/gp/product/B01JCI23JG/ref=oh_aui_search_detailpage?ie=UTF8&psc=1 |

ESP-12E (https://www.amazon.com/gp/product/B071RNQPHV/ref=oh_aui_search_detailpage?ie=UTF8&psc=1)
Infrared receiver: CHQ1838 https://www.amazon.com/gp/product/B00EFOQEUM/ref=oh_aui_search_detailpage?ie=UTF8&psc=1
MP3 player: YX5300 https://www.amazon.com/gp/product/B01JCI23JG/ref=oh_aui_search_detailpage?ie=UTF8&psc=1

Based on:
IRrecvDemo (IRremoteESP8266) https://github.com/markszabo/IRremoteESP8266
ArduinoSerialMP3Player https://github.com/salvadorrueda/ArduinoSerialMP3Player


Assumes two folders on the SD card inserted into the YX5300 named "01" and "02".
MP3s in the folders should have a prefix in the name of "001", "002", and so on.
