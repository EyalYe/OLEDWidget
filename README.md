Here’s a polished and slightly more engaging version of your README:

---

# OLEDWidget

**OLEDWidget** is a fun little project I created out of curiosity (and boredom). It displays real-time weather data—or pretty much any information you like, as long as there's an API to serve it. A character named **"Packy"** bounces around the screen, and each time he hits the top edge, a new information page appears.

If you're running the project on battery power, remember to comment out `Serial.begin(...)` in `small_oled.ino` to save energy.

---

## 🛠️ Hardware

- **ESP32-C3** microcontroller  
- **SSD1306 0.96" OLED** display  
- **GY-521 MPU6050** (3-axis gyroscope + accelerometer)  

Want it to be portable? Add a Li-ion battery and a battery management board for a fully wireless setup.

---

## 🔌 Wiring

All you need to connect are:

- `SDA`, `SCL`, `VCC`, and `GND` from both the OLED and MPU modules to the ESP32.

If you’d like to specify custom `SCL` and `SDA` pins:
- Set them in `small_oled.h`
- Uncomment the corresponding line in the `setup()` function inside `small_oled.cpp`

---

## 🚀 Getting Started

1. Open `secrets.h` and update it with your:
   - Wi-Fi SSID and password  
   - API key (guide here: [YouTube link](https://www.youtube.com/watch?v=Cemk32wKN_k))  
   - Latitude and longitude  
   - (Optional) Update the NTP server address to one near you.

2. Compile and upload `small_oled.ino` via the Arduino IDE.  
3. Sit back and watch Packy do his thing!

---

## 🖼️ Icons & Animation

I used [javl/image2cpp](https://javl.github.io/image2cpp/) to convert image files into bitmaps for display. These are stored in `logos.h`.

Want animated icons? You can add your own frame sequences—but keep in mind that some frame counts are hardcoded in the current implementation.

---

## 🎬 Demo Video

Check out a quick demo of the widget in action:  
[![Watch on YouTube](https://img.youtube.com/vi/hRmmU7YOUD4/0.jpg)](https://youtu.be/hRmmU7YOUD4)

---
