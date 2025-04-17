#include "small_oled.h"
#include "logos.h"
#include "secrets.h"


WiFiClient wifiClient;

// WiFi
const char* ssid     = WIFI_NAME;             //WiFi Name
const char* password = WIFI_PASSWORD;         // WiFi Password
//YouTube
const char* youtubeApiKey = YOUTUBE_API_KEY;
const char* channelId = CHANNEL_ID;
//Weather
const char* lat = LATITUDE;
const char* lon = LONGITUDE;

// global variables
unsigned long timer = 0;
double x, y, past_x, past_y;
double ax, ay;
float temp;
float past_z;
int curr_pages_awake = 0;
int dir_x = 1;
int time_on_screen = 0;
int curr_page = 0;
int curr_day;
int subscriberCount;
int to_sub_flag, to_temp_flag;
int monthDay;
int currentMonth;
int currentYear;
int currentHour;
int dead_packy_counter;
int next_page_flag = 0;
DynamicJsonDocument doc(4096);
String formattedTime;
time_t epochTime;
struct tm *ptm; 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MPU6050 mpu(Wire);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

void getSubscriberCount() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String("https://www.googleapis.com/youtube/v3/channels?part=statistics&id=") + channelId + "&key=" + youtubeApiKey;
    http.begin(url);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();

      DynamicJsonDocument doc2(1024);
      deserializeJson(doc2, payload);

      subscriberCount = doc2["items"][0]["statistics"]["subscriberCount"].as<int>();
      Serial.print("Subscriber Count: ");
      Serial.println(subscriberCount);
    } else {
      Serial.print("HTTP Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void get_temp(){
    if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String("https://api.open-meteo.com/v1/forecast?latitude=") + lat + "&longitude=" + lon + "&current=wind_speed_10m&hourly=temperature_2m,relative_humidity_2m,rain,visibility&daily=sunrise,sunset,uv_index_max&timezone=auto&forecast_days=1";
    http.begin(url);
    int httpResponseCode = http.GET();    
    if (httpResponseCode > 0) {
      doc.clear();
      String payload = http.getString();
      // Parse the temperature value from the JSON response

      deserializeJson(doc, payload);
      Serial.println("Weather:     ");
      Serial.println(payload);
      float temperature = doc["hourly"]["temperature_2m"][0];
      Serial.println(temperature);
      // Update the display with the fetched temperature
    } else {
      Serial.print("HTTP Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void reset_screen(){
  display.clearDisplay();
  display.setCursor(0, 0);
}

void show_logo_animation(int place, uint8_t* animated_logo){
  display.drawBitmap(0, place *32, animated_logo, 32, 32, 1);
}

void show_page(int page){
  if(page == 0){
    //timeClient.update();
    display.print(" ");
    display.println(formattedTime);
    //display.print(" "); display.print(monthDay);display.print("/");display.print(currentMonth);display.print("/");display.print(currentYear);
    display.printf("%02d/%02d/%04d", monthDay, currentMonth, currentYear); 
    display.drawBitmap(0, 32, temp_logo, 16, 16, 2);
    if(doc["hourly"]["temperature_2m"].size() >= currentHour + 1){
    display.printf("  %.1fC", doc["hourly"]["temperature_2m"][currentHour].as<float>()); display.println("");
    } else {
      to_temp_flag = 1;
    }
    display.drawBitmap(0, 48, youtube_logo, 16, 16, 2);
    display.printf("   %d", subscriberCount);
  } else if(page <= ANIMATIONS){
    show_animation(page - 1);
  } else {
    show_stock(page - ANIMATIONS -1);
  }
}

void show_animation(int num){
  static int frame = 0;
  char *junk, *time;
  char *dateStr;
  frame++;
  if(frame > 50)
    frame=0;
  display.setTextSize(3);
  switch(num){
    case 0:
      // temprature visability 
      show_logo_animation(0, weather[frame%28]);
      if(doc["hourly"]["temperature_2m"].size() >= currentHour + 1){
        display.printf("  %.1fC", doc["hourly"]["temperature_2m"][currentHour].as<float>()); display.println(""); 
      } else {
        to_temp_flag = 1;
      }
      display.setTextSize(2);
      display.println("");
      display.setTextSize(3);
      show_logo_animation(1, visability_logo[frame%28]);
      if(doc["hourly"]["visibility"].size() >= currentHour + 1){
        display.printf("  %d", doc["hourly"]["visibility"][currentHour].as<int>()); display.println("");
      } else {
        to_temp_flag = 1;
      }
      break;
    case 1:
      // humidity rain
      show_logo_animation(0, humidity[frame%28]);
      if(doc["hourly"]["relative_humidity_2m"].size() >= currentHour + 1){
        display.printf("   %d%%", doc["hourly"]["relative_humidity_2m"][currentHour].as<int>()); display.println("");
      } else {
        to_temp_flag = 1;
      }
      display.setTextSize(2);
      display.println("");
      display.setTextSize(3);
      show_logo_animation(1, rain[frame%28]);
      if(doc["hourly"]["rain"].size() >= currentHour + 1){
        display.printf("   %d%%", doc["hourly"]["rain"][currentHour].as<int>()); display.println("");
      } else {
        to_temp_flag = 1;
      }
      break;
    case 2:
      show_logo_animation(0, sunrise[frame%28]);
      if(doc["daily"]["sunrise"].size() > 0){
        String sunriseTime = doc["daily"]["sunrise"][0].as<String>();
        char sunriseTimeStr[sunriseTime.length() + 1];
        sunriseTime.toCharArray(sunriseTimeStr, sunriseTime.length() + 1);
        char* tPosition = strchr(sunriseTimeStr, 'T') + 1;
        Serial.println(tPosition);
        display.print("  "); display.print(tPosition); display.println("");
      } else {
        to_temp_flag = 1;
      }
      display.setTextSize(2);
      display.println("");
      display.setTextSize(3);
      show_logo_animation(1, sunrise[27 - frame%28]);
      if(doc["daily"]["sunset"].size() > 0){
        String sunriseTime = doc["daily"]["sunset"][0].as<String>();
        char sunriseTimeStr[sunriseTime.length() + 1];
        sunriseTime.toCharArray(sunriseTimeStr, sunriseTime.length() + 1);
        char* tPosition = strchr(sunriseTimeStr, 'T') + 1;
        Serial.println(tPosition);
        display.print("  "); display.print(tPosition); display.println("");
      } else {
        to_temp_flag = 1;
      }
      break;
    case 3:
      // UV wind speed
      show_logo_animation(0, wind[frame%28]);
      if(doc["current"]["wind_speed_10m"].as<float>()  > -1){
        display.printf("  %.1f", doc["current"]["wind_speed_10m"].as<float>()); display.println("");
      } else {
        to_temp_flag = 1;
      }
      display.setTextSize(2);
      display.println("");
      display.setTextSize(3);
      show_logo_animation(1, sunrise[27]);
      if(doc["daily"]["uv_index_max"].size() > 0){
        display.printf("  %.2f", doc["daily"]["uv_index_max"][0].as<float>()); display.println("");
      } else {
        to_temp_flag = 1;
      }
      break;
    default:
      show_logo_animation(0, visability_logo[0]);
      break;
  }
  display.setTextSize(2);
}

void show_stock(int num){

}

void update_time(){
  unsigned long currentMillis = millis();
  bool updated = false;

  while (millis() - currentMillis < 1000) {
    if (timeClient.update()) {
      updated = true;
      break;
    }
  }

  if (updated) {
    Serial.println("Time updated");
    Serial.println(timeClient.getFormattedTime());
  } else {
    reset_wifi();
  }
  //timeClient.update();
}

void reset_wifi(){
  Serial.println("Resetting WiFi");
  WiFi.disconnect();
  delay(1000);
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    reset_screen();
    display.fillRect(5, 40, (i++)*3, 10, 1);
    i = i > 40 ? 0 : i;
    display.setCursor(0, 0);
    display.print("Connecting to ");
    display.println(ssid);
    display.display();
    delay(300);
  }
  Serial.println("WiFi reconnected");
  update_time();
}

void my_setup(){
  //Wire.begin(SDA, SCL);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // display stuff
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 

  // making sure we are connected to wifi
  int i = 0;
  while (WiFi.status() != WL_CONNECTED){
  Serial.print(".");
  reset_screen();
  display.fillRect(5, 40, (i++)*3, 10, 1);
  i = i > 40 ? 0 : i;
  display.setCursor(0, 0);
  display.print("Connecting to ");
  display.println(ssid);
  display.display();
  delay(300);
  }

  // after succesfull connection we print this
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  reset_screen();
  display.println("WiFi connected.");
  display.println("IP address: ");
  display.println(WiFi.localIP());
  display.println("Beginnig calibration");
  display.display();

  // starting up the NTP, Wire and MPU connection
  timeClient.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  /*
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(500);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
  */

  mpu.setAccOffsets(AccXOffset, AccYOffset, AccZOffset);
  mpu.setGyroOffsets(GyroXOffset, GyroYOffset, GyroZOffset);

  // getting all the relavent data
  //get_temp();
  getSubscriberCount();
  update_time();

  // setting up the scren
  reset_screen();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  curr_page = 0;

  past_x = 0;
  past_y = 0;
  dead_packy_counter = 0;
}

void my_loop(){
  // reset screen
  reset_screen();

  // updating MPU
  mpu.update();   // updating the mpu(Gyro)

  // setting up the NTP variables
  formattedTime = timeClient.getFormattedTime();
  epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  monthDay = ptm->tm_mday;
  currentMonth = ptm->tm_mon+1;
  currentYear = ptm->tm_year+1900;
  currentHour = ptm->tm_hour;

  // after a day we get the new forcast
  if(curr_day != monthDay){
    curr_day = monthDay;
    get_temp();
    to_temp_flag = 0;
    update_time();
  }

  // calculating packy's movement
  float accAngleX = mpu.getAccAngleX();  // calculated from accelerometer
  float accAngleY = mpu.getAccAngleY();

  float gyroX = mpu.getGyroX();  // degrees/sec
  float gyroY = mpu.getGyroY();

  float alpha = 0.95;  // Complementary filter weight

  // Integrate gyro data (gyroX and gyroY are in deg/sec, DT in ms)
  ax = alpha * (ax + gyroX * DT * 0.001) + (1 - alpha) * accAngleX;
  ay = alpha * (ay + gyroY * DT * 0.001) + (1 - alpha) * accAngleY;


  x = x + 0.5 * ax * DT * DT * 0.001;
  y = y + 0.5 * ay * DT * DT * 0.05;
  x = x > RAD ? x : RAD;
  x = x < 128 - RAD ? x : 128 - RAD - 1;
  y = y > RAD ? y : RAD;
  y = y < 64 - RAD ? y : 64 - RAD - 1;


  if(y == RAD || y == 64 - RAD - 1)
    ay = -ay * 0.5;
  if(x == RAD || x == 128 - RAD - 1)
    ax = -ax * 0.7;



  if(y == RAD){
    next_page_flag = 1;
  }
  // after ALLOWED * DT ms the screen changes back to screen 0
  if (time_on_screen >= ALLOWED){
    if(to_temp_flag){
      get_temp();
      to_temp_flag = 0;
    }
    time_on_screen = 0;
    curr_page = 0;
    curr_pages_awake++;
    if(curr_pages_awake >= ALOOWED_AWAKE){
      delay(1000);
      curr_pages_awake = 0;
      Serial.println("Waking up");
      getSubscriberCount();
    }
  }

  if((time_on_screen % 5) == 0){
    if(next_page_flag){
      curr_page = (curr_page + 1) % (ANIMATIONS+1);
      next_page_flag = 0;
    }
  }
  // showing current screen
  show_page(curr_page);
  
  #ifdef KEEP_PACKY_ALIVE
  // direction of packy
  if(ax > 0){
    dir_x = 1;
  } else {
    dir_x = -1;
  }

  if(past_x == x && past_y == y){
    dead_packy_counter++;
  }
  else{
    dead_packy_counter = 0;
  }

  if(dead_packy_counter < 5000){
    // drawing packy
    for(int i = 1; i <= RAD; i++)
      display.drawCircle((int)x, (int)y , i, 1);
    if((time_on_screen / 10) % 2 == 0){
      for(int i = 0; i <= RAD; i ++){
        for(int j = 0; j <= RAD - i; j++){
          display.drawPixel(x + dir_x * (j+i), y + j, 0);
          display.drawPixel(x + dir_x * (j+i), y - j, 0);
        }
      }
    }
  }
  past_x = x;
  past_y = y;
  #endif


  display.display();
  time_on_screen++;
  delay(DT);
}
