#ifndef SMALL_OLED_H
#define SMALL_OLED_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <SPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include <ESP8266HTTPClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define NTP_OFFSET  10800 // In seconds
#define NTP_INTERVAL 60 * 1000   // In miliseconds
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define ALLOWED 10000  // number of frames until page changes
#define ALOOWED_AWAKE 20 // number of page switches before sleep
#define RAD 5
#define DT 20
#define PAGES 10
#define ANIMATIONS 4
#define SDA 21
#define SCL 22

void getSubscriberCount();
void get_temp();
void reset_screen();
void my_setup();
void my_loop();
void show_page(int page);
void show_animation(int num);
void show_stock(int num);
void show_logo_animation(int place, uint8_t* animated_logo);
void update_time();
void reset_wifi();
#endif
