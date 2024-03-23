#ifndef _Config_H
#define _Config_H

#include <Arduino.h>

#include <GyverOS.h>
#include "HardwareSerial.h"
#include <EEPROM.h>
#include <microDS3231.h>
// #include "SoftwareSerial.h"
#include <Wire.h>
#include <GyverBME280.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include "HX711.h"
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
  Battery
};

// MAPing EEPROM
enum eep_map
{
  ADR_CALIB_OK = 0,
  ADR_CALIB = 1,
  ADR_F_START_OK = ADR_CALIB + 4,
  ADR_CONTAIN = 6
};

//=======================================================================

//=========================== GLOBAL CONFIG =============================
struct GlobalConfig
{
  uint16_t sn = 0;

  String phone = ""; // номер телефона в международном формате
  uint16_t iso_code = 7;
  uint8_t phoneNumber[10] = {0}; // номер телефона в международном формате

  String firmware = ""; // accepts from setup()
  // System_Information
  String fwdate = "24.02.2024";
  String chipID = "";
  String MacAdr = "";

  String APSSID = "Beekeeper";
  String APPAS = "12345678";

  int8_t UserSendTime1 = 9;
  int8_t UserSendTime2 = 20;

  byte WiFiMode = 0; // Режим работы WiFi
  // long WiFiPeriod = 0;
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
  float dsT = 0.0;     // Temperature DS18B20
  float bmeT = 0.0;    // Temperature BME280
  int bmeH = 0.0;      // Humidity   BME280
  float bmeHcal = 4.2; // Calibration factors
  float bmeA = 0.0;    // Altitude   BME280 m
  float bmeP_hPa = 0;  // Pressure   BME280 hPa
  int bmeP_mmHg = 0;   // Pressure   BME280 mmHg
  float calib = 23.38; // Save and Reading from EEPROM
  float units = 0.0;
  float kg = 0.0;
  float g_eep = 2.31;
  float grms = 10.5;
  float g_contain = 0.0; // Save and Reading from EEPROM
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
  int8_t num[10] = {9, 5, 0, 6, 0, 4, 5, 5, 6, 5};
};
extern EEP_Data eep_data;
//=======================================================================
struct Flag
{
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
//============================================================================
#endif // _Config_H