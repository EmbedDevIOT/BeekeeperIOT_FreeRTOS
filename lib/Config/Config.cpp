#include "Config.h"
//=======================================================================

/************************ System Initialisation **********************/
void SystemInit(void)
{
  GetChipID();
}
/*******************************************************************************************************/
//=======================   I2C Scanner     =============================
void I2C_Scanning(void)
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 8; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}
//=======================================================================
/***************************** Function Show information or Device *************************************/
void ShowInfoDevice(void)
{
  Serial.println(F("Starting..."));
  Serial.println(F("Beekeeper"));
  Serial.print(F("SN:"));
  Serial.println(Config.sn);
  Serial.print(F("fw_date:"));
  Serial.println(Config.fwdate);
  Serial.println(Config.firmware);
  Serial.println(Config.chipID);
  Serial.println(F("by EmbedDev"));
  Serial.println();
}
/*******************************************************************************************************/

/*******************************************************************************************************/
void GetChipID()
{
  uint32_t chipId = 0;

  for (int i = 0; i < 17; i = i + 8)
  {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  Config.chipID = chipId;
}
/*******************************************************************************************************/

/*******************************************************************************************************/
// String GetMacAdr()
// {
//   // Config.MacAdr = WiFi.macAddress(); //
//   // Serial.print(F("MAC:"));           // временно
//   // Serial.println(Config.MacAdr);     // временно
//   // return WiFi.macAddress();
// }
/*******************************************************************************************************/

/*******************************************************************************************************/
void CheckSystemState()
{
}
/*******************************************************************************************************/

/*******************************************************************************************************/
// Debug information
void DebugControl()
{
    char message[37];

    Serial.println(F("!!!!!!!!!!!!!!  DEBUG INFO  !!!!!!!!!!!!!!!!!!"));
    sprintf(message, "RTC Time: %02d:%02d:%02d", Clock.hour, Clock.minute, Clock.second);
    Serial.println(message);
    sprintf(message, "RTC Date: %4d.%02d.%02d", Clock.year, Clock.month, Clock.date);
    Serial.println(message);

}

/*******************************************************************************************************/

/*******************************************************************************************************/
void SystemFactoryReset()
{

}
/*******************************************************************************************************/
