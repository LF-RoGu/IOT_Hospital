
/*
   NOTAS:
   -Revisar siguiente link (mas fiable) para publicar por medio de mqtt: https://help.ubidots.com/en/articles/748067-connect-an-esp32-devkitc-to-ubidots-over-mqtt
*/
//=======================================================================
//                               Lib
//=======================================================================
#include <SPI.h>
#include <Wire.h>
/*
   Ubidots
*/
#include <ESP8266WiFi.h>
#include "UbidotsESPMQTT.h"
/*
  MAX30105 LIBRARY
*/
#include "MAX30105.h"
#include "heartRate.h"
/** DS18B20*/
#include <OneWire.h>
#include <DallasTemperature.h>
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
#define EVENT_TIME_Max30105 100
#define EVENT_TIME_LM75 120
#define EVENT_TIME 1000

#define TRUE 1
#define FALSE 0

#define TOKEN "BBFF-YXileBcbucSl944RMPB6MJwddlGdrL" // Your Ubidots TOKEN
#define WIFINAME "LEAL" //Your SSID
#define WIFIPASS "luis2020" // Your Wifi Pass
//=======================================================================
//                               Objects
//=======================================================================
/** Ubidots*/
Ubidots client(TOKEN);
/** MAX30105*/
MAX30105 particleSensor;
/** DS18B20*/
/** Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)*/
OneWire oneWire(GPIO_D4);
/** Pass our oneWire reference to Dallas Temperature. */
DallasTemperature sensors(&oneWire);
/** arrays to hold device address*/
DeviceAddress insideThermometer;
//=======================================================================
//                               Variables
//=======================================================================
uint32_t previous_time_Max30105 = 0;
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

/** MAX30105 variables*/
const uint8_t RATE_SIZE = 4;
uint8_t rates[RATE_SIZE]; //Array of heart rates
uint8_t rateSpot = 0;
int64_t lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int32_t beatAvg;
int64_t samplesTaken = 0; //Counter for calculating the Hz or read rate
int64_t unblockedValue; //Average IR at power up
int64_t startTime; //Used to calculate measurement rate
int32_t perCent; 
int32_t degOffset = 0.5; //calibrated Farenheit degrees
int32_t irOffset = 1800;
int32_t count;
int32_t noFinger;
//auto calibrate
int32_t avgIr;
int32_t avgTemp;

//=======================================================================
//                               Prototype
//=======================================================================
/*

*/
void task_Max30105_id (void);
/*

*/
void funct_HeartBeat (void);
/*

*/
void task_LM75_id (void);

//=======================================================================
//                               Setup
//=======================================================================

void setup() {
  /** Enable the baudrate*/
  Serial.begin(115200);
  /*
     Max30105 SetUp
  */
  /** Local variables*/
  /**Setup to sense up to 18 inches, max LED brightness*/
  uint8_t ledBrightness = 25; //Options: 0=Off to 255=50mA=0xFF hexadecimal. 100=0x64; 50=0x32 25=0x19
  uint8_t sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  uint8_t ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int32_t sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int32_t pulseWidth = 411; //Options: 69, 118, 215, 411
  int32_t adcRange = 2048; //Options: 2048, 4096, 8192, 16384
  /** Init sensor MAX30105*/
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  particleSensor.setup(0); //Configure sensor. Turn off LEDs
  particleSensor.enableDIETEMPRDY(); //Enable the temp ready interrupt. This is required.
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  particleSensor.enableDIETEMPRDY(); //Enable the temp ready interrupt. This is required.
  /*
     LM75 SetUp
  */
}

void loop() {
  task_Max30105_id();
}
