#ifndef SECRETS_H
#define SECRETS_H
//Wifi
#define WIFI_NAME "your_wifi_name";        // WiFi Name
#define WIFI_PASSWORD "your_wifi_password";  // WiFi Password
//YouTube
#define YOUTUBE_API_KEY "your_youtube_api_key"; // YouTube API Key
#define CHANNEL_ID "your_channel_id"; // YouTube Channel ID
//Weather
#define LATITUDE "your_latitude"; // Latitude
#define LONGITUDE "your_longitude"; // Longitude

// mpu6050 sensor use the mpu clibration script to find the offsets
// not really sensitive but I know you wouldn't notice it otherwise
#define AccXOffset 0.1816
#define AccYOffset -0.0175
#define AccZOffset -0.0062

#define GyroXOffset -3.1579
#define GyroYOffset 0.6320
#define GyroZOffset -0.0010

// NTP Server change this to your local NTP server
#define NTP_ADDRESS  "1.asia.pool.ntp.org"
#endif
