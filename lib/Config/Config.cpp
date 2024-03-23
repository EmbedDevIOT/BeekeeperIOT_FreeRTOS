#include "Config.h"
//=======================================================================

/************************ System Initialisation **********************/
void SystemInit(void)
{
  GetChipID();
}
/*******************************************************************************************************/

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
