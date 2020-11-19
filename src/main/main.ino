
/*
 * NOTAS:
 * -Revisar siguiente link (mas fiable) para publicar por medio de mqtt: https://help.ubidots.com/en/articles/748067-connect-an-esp32-devkitc-to-ubidots-over-mqtt
 */
//=======================================================================
//                               Lib
//=======================================================================
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_INA219.h>
#include <Ticker.h>  //Ticker Library
/*
   Ubidots
*/
#include <ESP8266WiFi.h>
#include "UbidotsESPMQTT.h"

//=======================================================================
//                               GPIO
//=======================================================================
const uint8_t GPIO_D0   = 16;
const uint8_t GPIO_D1   = 5;
const uint8_t GPIO_D2   = 4;
const uint8_t GPIO_D3   = 0;
const uint8_t GPIO_D4   = 2;
const uint8_t GPIO_D5   = 14;
const uint8_t GPIO_D6   = 12;
const uint8_t GPIO_D7   = 13;
const uint8_t GPIO_D8   = 15;
const uint8_t GPIO_S2   = 9;
const uint8_t GPIO_S3   = 10;
//=======================================================================
//                               Definitions
//=======================================================================
#define EVENT_TIME_Max30102 100
#define EVENT_TIME_LM75 120
#define EVENT_TIME 1000

#define TRUE 1
#define FALSE 0

#define TOKEN "BBFF-YXileBcbucSl944RMPB6MJwddlGdrL" // Your Ubidots TOKEN
#define WIFINAME "LEAL" //Your SSID
#define WIFIPASS "luis2020" // Your Wifi Pass

Ubidots client(TOKEN);

//=======================================================================
//                               Variables
//=======================================================================
uint32_t previous_time_Max30102 = 0;
uint32_t previous_time_LM75 = 0;
uint32_t previous_time_TIME = 0;

uint16_t time_s = 0;
uint16_t time_m = 0;
uint16_t time_h = 0;

uint8_t publish_flag = FALSE;
uint8_t publish_time = 10;

/** Sensors Variables*/
uint16_t oxymeter_var = 0;
uint16_t temperature_var = 0;

//=======================================================================
//                               Callback
//=======================================================================
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
//=======================================================================
//                               Prototype
//=======================================================================
/*

*/
void task_Max30102_id (void);
/*

*/
void task_LM75_id (void);
/*

*/
void task_RTC_id (void);

//=======================================================================
//                               Setup
//=======================================================================

void setup() {
  // put your setup code here, to run once:
  // initialize with the I2C addr 0x3C
  Serial.begin(115200);

  /*
     Ubidots SetUp
  */
  /** Set the broker ID*/
  client.ubidotsSetBroker("things.ubidots.com");
  /** Enable debug options*/
  client.setDebug(true);
  /** Enable the baudrate*/
  Serial.begin(115200);
  /** Set a connection to the server*/
  client.wifiConnection(WIFINAME, WIFIPASS);
  /** Callback for debug options*/
  client.begin(callback);
  /*
     Max30102 SetUp
  */

  /*
     LM75 SetUp
  */
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t currentTime = millis(); //ms

  /*
     Ubidots run
  */
  /** If connection is lost, then reconnect*/
  if(!client.connected())
  {
    client.reconnect();
  }

  /*
     Run task in a certain time
  */
  if (currentTime - previous_time_Max30102 >= EVENT_TIME_Max30102)
  {
    task_Max30102_id();

    previous_time_Max30102 = currentTime;
  }
  /*
     Run task in a certain time
  */
  if (currentTime - previous_time_LM75 >= EVENT_TIME_LM75)
  {
    task_LM75_id();

    previous_time_LM75 = currentTime;
  }
  /*
     Run task in a certain time
  */
  if (currentTime - previous_time_TIME >= EVENT_TIME)
  {
    task_RTC_id();

    previous_time_TIME = currentTime;
  }

  /*
     Run task to publish info report
  */
  if(TRUE == publish_flag)
  {
    client.add("oxymeter", oxymeter_var);
    client.add("temperature", temperature_var);
    /** API LABEL*/
    client.ubidotsPublish("iot_hospital");
    client.loop();
  }
}
