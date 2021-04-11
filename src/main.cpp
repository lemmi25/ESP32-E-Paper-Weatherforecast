

/*
    LilyGo Ink Screen Series u8g2Fonts Test
        - Created by Lewis he
*/

// According to the board, cancel the corresponding macro definition
#define LILYGO_T5_V213
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  240        /* Time ESP32 will go to sleep (in seconds) */

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <boards.h>
#include <GxEPD.h>
#include <Icon_list.h>
#include <HTTPClient.h>
#include <Adafruit_NeoPixel.h>             //Depend  https://github.com/adafruit/Adafruit_NeoPixel

#include <GxDEPG0213BN/GxDEPG0213BN.h>    // 2.13" b/w  form DKE GROUP

RTC_DATA_ATTR int bootCount = 0;
StaticJsonDocument<5000> doc;

String houre_wifi;
String minute_wifi;
String month_wifi;
String day_wifi;
int week_number;

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

char *substring(const char *str, size_t begin, size_t len);

GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);


// Define NTP Client to get time
HTTPClient http;

void set_WiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("FRITZ!Box 7490", "58625300401074548598");
    // WiFi.scanNetworks will return the number of networks found

  delay(500);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed!");
    break;
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void get_time(){

http.begin("http://worldtimeapi.org/api/timezone/Europe/Berlin.json"); //Specify the URL
  int httpCodeTime = http.GET();

  if (httpCodeTime > 0)
  { //Check for the returning code

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, http.getString());

    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    else
    {
      const char *date = doc["datetime"]; //Get current time
      Serial.println(date);
      month_wifi = (String)substring(date, 5, 2);  
      day_wifi = (String)substring(date, 8, 2); 
      houre_wifi = (String)substring(date, 11, 2);
      minute_wifi = (String)substring(date, 14, 2);

      week_number = doc["week_number"];
      delay(500);

    }
  }
    else
  {
    Serial.println("Error on HTTP request Time");


}
}

void setup()
{
    
    Serial.begin(115200);
    Serial.println();
    Serial.println("setup");

    set_WiFi();

    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    ++bootCount;

    delay(500);
   

}

void loop()
{
    
    display.init();
    
    display.fillScreen(GxEPD_WHITE);

    display.drawBitmap(thunderstorm, 0, 120, 150, 150, GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setRotation(1);

    get_time();

    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(0, 115);
    display.print("CW ");

    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(26, 115);
    display.print(week_number);

    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(75, 115);
    display.print("Date ");
    display.setCursor(125, 115);
    display.print(day_wifi + "-");
    display.setCursor(158, 115);
    display.print(month_wifi);

    display.setCursor(195, 115);
    display.print(houre_wifi);
    display.setCursor(205, 115);
    display.print(" : ");
    display.setCursor(225, 115);
    display.print(minute_wifi);


    display.setFont(&FreeMonoBold24pt7b);
    display.setCursor(65, 60);
    display.print(bootCount);
    display.update();

    delay(2000);
    esp_deep_sleep_start();

}

char *substring(const char *str, size_t begin, size_t len)
{
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin + len))
    return 0;

  return strndup(str + begin, len);
}