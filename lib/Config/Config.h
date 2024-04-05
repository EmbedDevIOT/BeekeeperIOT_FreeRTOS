#ifndef _Config_H
#define _Config_H

#include <Arduino.h>

#include <GyverOS.h>
#include "HardwareSerial.h"
#include <EEPROM.h>
#include <microDS3231.h>
#include <Wire.h>
#include <GyverBME280.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include "HX711.h"

#define EB_DEB_TIME 20     
#define EB_CLICK_TIME 50   
#define EB_HOLD_TIME 600    
#define EB_STEP_TIME 200    

#include <EncButton.h>
#include <GyverOLED.h>

#define UARTSpeed 115200
#define MODEMSpeed 9600

#define WiFi_

#define CALL_FAIL 255
#define EEP_DONE 200

#define WiFiTimeON 15

#define DEBUG
// #define I2C_SCAN

#define DISABLE 0
#define ENABLE 1

#define BAT_MIN 30

// I2C Adress
#define BME_ADR 0x76
#define OLED_ADR 0x3C
#define RTC_ADR 0x68

//=======================================================================
extern MicroDS3231 RTC;
extern DateTime Clock;
//=======================================================================

//========================== ENUMERATION ================================
//=======================================================================
enum menu
{
  Menu = 1,
  Action,
  Time,
  Calib,
  Notifycation,
  SMS_NUM,
  ZeroSet
};

//EEPROM
struct EEP_D
{
  uint8_t st_cal = 0; 
  float cal_f = 0.0;  
  int32_t avr = 0;
  int8_t t1_sms = 0;
  int8_t t2_sms = 0;
  int8_t num[10] = {0};
};
extern EEP_D _eep;

//=======================================================================

//=========================== GLOBAL CONFIG =============================
struct GlobalConfig
{
  uint16_t sn = 0;
  String phone = ""; // номер телефона в международном формате
  uint16_t iso_code = 7;
  String firmware = ""; // accepts from setup()
  String fwdate = "05.04.2024";
  String chipID = "";
  String MacAdr = "";
  String APSSID = "Beekeeper";
  String APPAS = "12345678";
  int8_t UserSendTime1 = 0;
  int8_t UserSendTime2 = 0;
};
extern GlobalConfig Config;
//=======================================================================

//=======================================================================
struct SYTM
{
  bool DispState = true;
  uint8_t DispMenu = Action;
};
extern SYTM System;
//=======================================================================

struct SNS
{
  int signal = 0;
  float dsT = 0.0;     // Temperature DS18B20
  float bmeT = 0.0;    // Temperature BME280
  int bmeH = 0.0;      // Humidity   BME280
  float bmeHcal = 4.2; // Calibration factors
  float bmeA = 0.0;    // Altitude   BME280 m
  float bmeP_hPa = 0;  // Pressure   BME280 hPa
  int bmeP_mmHg = 0;   // Pressure   BME280 mmHg
  float calib = 0.0;   // Save and Reading from EEPROM
  float units = 0.0;
  float kg = 0.0;
  float grms = 10.5;
  uint32_t averange = 0;
  uint32_t voltage = 0;
};
extern SNS sensors;
//=======================================================================
struct EEP_Data
{
  uint8_t st_cal = 0;
  uint8_t st_fstart = 0;
  float calibr_factor = 0.0;
  float g_contain = 0.0;
  int8_t t1_sms = 0;
  int8_t t2_sms = 0;
  int8_t num[10] = {9, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};
extern EEP_Data eep_data;
//=======================================================================
struct Flag
{
  bool BTSET = false;
  bool BTUP = false;
  bool BTDWN = false;
  bool debug = true;
  bool SMS1 = true;
  bool SMS2 = true;
  bool HX711_Block = false;
  bool Call_Block = false;
  uint8_t FirstStart = 0;
  uint8_t Calibration = 0;
};
extern Flag ST;
//============================================================================

//============================================================================
void SystemInit(void);     //  System Initialisation (variables and structure)
void ShowInfoDevice(void); //  Show information or this Device
void GetChipID(void);
void CheckSystemState(void);
void DebugControl(void);
void SystemFactoryReset(void);
void I2C_Scanning(void);
//============================================================================
#endif // _Config_H