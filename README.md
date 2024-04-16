# OLEDWidget
This is a small project I made out of boredom. It displays the weather and really any kind of information you like, as long as you have access to an API that serves this information. Every time "packy" hits the top of the screen, the page changes.
## Hardware
- ESP32-C6
- SSD1306 0.96" OLED display
- GY-521 MPU6050 (3-axis gyroscope + 3-axis accelerometer)
Feel free to add a battery and battery management board if you'd like.
## Wiring
The only connections required are the SDA, SCL, VCC, and ground pins of the OLED display and MPU to the corresponding pins on the ESP32. To find the exact pins on your board, search for the name of your board + "pinout" and you'll find that information.
## How To Use
Just update small_oled.cpp with your Wi-Fi SSID and password, get an API key as shown here https://www.youtube.com/watch?v=Cemk32wKN_k, and enter your latitude and longitude. Then, compile and flash small_oled.ino using the Arduino IDE, and you're good to go!


I used https://javl.github.io/image2cpp/ to convert images to icons, which can be seen in the logos.h file. If you'd like to add animated logos, you can do so, but note that in some cases, the number of frames is hardcoded in the implementation.
