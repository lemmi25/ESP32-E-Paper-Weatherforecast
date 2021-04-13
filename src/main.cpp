

/*
    LilyGo Ink Screen Series u8g2Fonts Test
        - Created by Lewis he
*/

// According to the board, cancel the corresponding macro definition
#define LILYGO_T5_V213
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  300        /* Time ESP32 will go to sleep (in seconds) */

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

StaticJsonDocument<5000> docWeatherNow;
StaticJsonDocument<5000> docWeather;

String month_wifi;
String day_wifi;

float feels_like;

float main_temp_now;
String weather_description_now;
String houre_wifi_now;

float main_temp_03;
int weather_description_03;
String houre_wifi_03;

float main_temp_06;
int weather_description_06;
String houre_wifi_06;


String city;

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);


// Define NTP Client to get time
HTTPClient httpWeatherNow;
HTTPClient httpWeather;

void set_WiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("FRITZ!Box 7490", "58625300401074548598");
  // WiFi.scanNetworks will return the number of networks found

  int count = 0;
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
/*     Serial.println("Connection Failed!");
 */
    delay(1000);

    count++;

    if(count == 10){
      esp_deep_sleep_start();
    }
   
  }

/*   Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); */
}

void get_weather(){


  httpWeather.begin("http://api.openweathermap.org/data/2.5/forecast?q=Freiburg,de&cnt=3&units=metric&appid=03e2fbe874af4836c6bf932b697a809b");
  httpWeatherNow.begin("http://api.openweathermap.org/data/2.5/weather?q=Freiburg,de&cnt=3&units=metric&appid=03e2fbe874af4836c6bf932b697a809b");
  int httpCodeWeather = httpWeather.GET();
  int httpCodeWeatherNow = httpWeatherNow.GET();

  if (httpCodeWeather > 0 || httpCodeWeatherNow > 0)
  { //Check for the returning code

    DeserializationError errorWeather = deserializeJson(docWeather, httpWeather.getString());
    DeserializationError errorWeatherNow = deserializeJson(docWeatherNow, httpWeatherNow.getString());

    if (errorWeather || errorWeatherNow)
    {
/*       Serial.print(F("deserializeJson() from weather failed: "));
      Serial.println(errorWeather.c_str()); */
      esp_deep_sleep_start();
            
    }
    else
    {

      display.init();

      display.fillScreen(GxEPD_WHITE);

      main_temp_now = docWeatherNow["main"]["temp"];         //Get current time forecast 0h
      feels_like = docWeatherNow["main"]["feels_like"];
      
      main_temp_03 = docWeather["list"][1]["main"]["temp"];         //Get current time forecast 0h
      weather_description_03 = docWeather["list"][1]["weather"][0]["id"];
      String weather_icon_03 = docWeather["list"][1]["weather"][0]["icon"];

      main_temp_06 = docWeather["list"][2]["main"]["temp"];         //Get current time forecast 0h
      weather_description_06 = docWeather["list"][2]["weather"][0]["id"];
      String weather_icon_06 = docWeather["list"][2]["weather"][0]["icon"];

      String dt_txt_now = docWeather["list"][0]["dt_txt"];
      month_wifi =  dt_txt_now.substring(5,7);
      day_wifi = dt_txt_now.substring(8,10); 
      houre_wifi_now = dt_txt_now.substring(11,13);

      String dt_txt_03 = docWeather["list"][1]["dt_txt"];
      houre_wifi_03 = dt_txt_03.substring(11,13);

      String dt_txt_06 = docWeather["list"][2]["dt_txt"];
      houre_wifi_06 = dt_txt_06.substring(11,13);

      String c = docWeather["city"]["name"];
      city = c;

    if ((weather_description_03 >=200) && (weather_description_03 <= 232)){
      display.drawBitmap(thunderstorm_small, 15, 79, 100, 100, GxEPD_WHITE);

    }else if((weather_description_03 >=300) && (weather_description_03 <= 321)){
      display.drawBitmap(shower_rain_small, 15, 79, 100, 100, GxEPD_WHITE);

    }else if((weather_description_03 >=500) && (weather_description_03 <= 504)){
      display.drawBitmap(rain_small, 15, 79, 100, 100, GxEPD_WHITE);

    }else if((weather_description_03 >=520) && (weather_description_03 <= 531)){
      display.drawBitmap(shower_rain_small, 15, 79, 100, 100, GxEPD_WHITE);

    }else if(weather_description_03 == 511){
      display.drawBitmap(snow_small, 15, 79, 100, 100, GxEPD_WHITE);

    }else if((weather_description_03 >=600) && (weather_description_03 <= 622)){
      display.drawBitmap(snow_small, 15, 79, 100, 100, GxEPD_WHITE);

    }else if((weather_description_03 >=701) && (weather_description_03 <= 781)){
      display.drawBitmap(mist_small, 15, 79, 100, 100, GxEPD_WHITE);

    }else if(weather_description_03 == 800 ){
      if(weather_icon_03.substring(2,3) == "d"){
        display.drawBitmap(clear_sky_small, 35, 89, 65, 65, GxEPD_WHITE);

      }else{
        display.drawBitmap(clear_sky_night_small, 15, 79, 100, 100, GxEPD_WHITE);

      }
    }else if((weather_description_03 >=801) && (weather_description_03 <= 804)){
      if(weather_description_03 == 801){
        display.drawBitmap(few_clouds_small, 15, 79, 100, 100, GxEPD_WHITE);

      }else if (weather_description_03 == 802)
      {
        display.drawBitmap(scattered_clouds_small, 15, 79, 100, 100, GxEPD_WHITE);

      }else
      {
        display.drawBitmap(broken_clouds_small, 15, 81, 100, 100, GxEPD_WHITE);
      }
    }

    if ((weather_description_06 >=200) && (weather_description_06 <= 232)){
      display.drawBitmap(thunderstorm_small, 15, 167, 100, 100, GxEPD_WHITE);
    }else if((weather_description_06 >=300) && (weather_description_06 <= 321)){
      display.drawBitmap(shower_rain_small, 15, 167, 100, 100, GxEPD_WHITE);
    }else if((weather_description_06 >=500) && (weather_description_06 <= 504)){
      display.drawBitmap(rain_small, 15, 167, 100, 100, GxEPD_WHITE);
    }else if((weather_description_06 >=520) && (weather_description_06 <= 531)){
      display.drawBitmap(shower_rain_small, 15, 167, 100, 100, GxEPD_WHITE);
    }else if(weather_description_06 == 511){
      display.drawBitmap(snow_small, 15, 167, 100, 100, GxEPD_WHITE);
    }else if((weather_description_06 >=600) && (weather_description_06 <= 622)){
      display.drawBitmap(snow_small, 15, 167, 100, 100, GxEPD_WHITE);
    }else if((weather_description_06 >=701) && (weather_description_06 <= 781)){
      display.drawBitmap(mist_small, 15, 167, 100, 100, GxEPD_WHITE);
    }else if(weather_description_06 == 800 ){
      if(weather_icon_06.substring(2,3) == "d"){
        display.drawBitmap(clear_sky_small, 35, 177, 65, 65, GxEPD_WHITE);
      }else{
        display.drawBitmap(clear_sky_night_small, 15, 167, 100, 100, GxEPD_WHITE);
      }
    }else if((weather_description_06 >=801) && (weather_description_06 <= 804)){
      if(weather_description_06 == 801){
        display.drawBitmap(few_clouds_small, 15, 167, 100, 100, GxEPD_WHITE);
      }else if (weather_description_06 == 802)
      {
        display.drawBitmap(scattered_clouds_small, 15, 167, 100, 100, GxEPD_WHITE);
      }else
      {
        display.drawBitmap(broken_clouds_small, 15, 167, 100, 100, GxEPD_WHITE);
      }
    }
      
      display.setTextColor(GxEPD_BLACK);
    }
  }
  //vTaskDelay(30000); //30sec
}

void setup()
{
    
    /*     Serial.begin(115200);
    Serial.println();
    Serial.println("setup"); */

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    set_WiFi();

    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);

    ++bootCount;
  
}

void loop()
{
    
    get_weather();

    display.setRotation(1);

    

    display.setFont(&FreeMonoBold12pt7b);
    display.setCursor(0, 20);
    display.print(city);

    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(0, 40);
    display.print(weather_description_now);

    display.setFont(&FreeSansBold24pt7b);
    if((int)round(main_temp_now) < -9){
      display.setCursor(0, 87);
      display.print((int)round(main_temp_now));
      display.setCursor(70, 60);

      display.setFont(&FreeSansBold12pt7b);
      display.print("o");
    }else if((int)round(main_temp_now) >= 10 || (int)round(main_temp_now) < 0){
      display.setCursor(0, 87);
      display.print((int)round(main_temp_now));
      display.setCursor(53, 60);
      display.setFont(&FreeSansBold12pt7b);
      display.print("o");

    }else{
      display.setCursor(20, 87);
      display.print((int)round(main_temp_now));
      display.setCursor(50, 60);
      display.setFont(&FreeSansBold12pt7b);
      display.print("o");

    }

    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(65, 90);
    display.print((int)round(feels_like));
    


    display.setFont(&FreeSansBold9pt7b);
    display.setCursor(0, 117);
    display.print(day_wifi + " / ");
    display.setCursor(33, 117);
    display.print(month_wifi);
    display.setFont(&FreeSansBold18pt7b);

    
    if((int)round(main_temp_06) < -9){
      display.setCursor(186, 115);
      display.print((int)round(main_temp_06));
      display.setCursor(237, 95);
    }else if((int)round(main_temp_06) >= 10 || (int)round(main_temp_06) < 0){
      display.setCursor(187, 115);
      display.print((int)round(main_temp_06));
      display.setCursor(224, 95);
    }else{
      display.setCursor(200, 115);
      display.print((int)round(main_temp_06));
      display.setCursor(219, 95);
    }

    display.setFont(&FreeSansBold9pt7b);
    display.print("o");

    display.setCursor(230, 25);
    display.print(houre_wifi_06);

    display.setFont(&FreeSansBold18pt7b);

    if((int)round(main_temp_03) < -9){
      display.setCursor(186-89, 115);
      display.print((int)round(main_temp_03));
      display.setCursor(237-89, 95);
    }else if((int)round(main_temp_03) >= 10 || (int)round(main_temp_03) < 0){
      display.setCursor(187-89, 115);
      display.print((int)round(main_temp_03));
      display.setCursor(224-89, 95);
    }else{
      display.setCursor(200-89, 115);
      display.print((int)round(main_temp_03));
      display.setCursor(219-89, 95);
    }

    display.setFont(&FreeSansBold9pt7b);
    display.print("o");

    display.setCursor(154, 25);//63
    display.print(houre_wifi_03);

    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(60, 121);
    display.print(bootCount);
    
    display.update();

    delay(100);
    esp_deep_sleep_start();

}