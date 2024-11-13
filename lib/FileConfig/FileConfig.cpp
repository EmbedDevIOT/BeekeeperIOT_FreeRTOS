#include "FileConfig.h"

// AT24Cxx eep(0x57, 32);

//////////////////////////////////////////////
// Loading settings from config.json file   //
//////////////////////////////////////////////
void LoadConfig()
{
  // Serial.println("Loading Configuration from config.json");
  String jsonConfig = "";

  File configFile = SPIFFS.open("/config.json", "r"); // Открываем файл для чтения
  jsonConfig = configFile.readString();               // загружаем файл конфигурации из EEPROM в глобальную переменную JsonObject
  configFile.close();

  StaticJsonDocument<1024> doc; //  Резервируем памяь для json обекта
  // Дессиарилизация - преобразование строки в родной объкт JavaScript (или преобразование последовательной формы в параллельную)
  deserializeJson(doc, jsonConfig); //  вызовите парсер JSON через экземпляр jsonBuffer

  char TempBuf[15];


  struct _sys
  {
    uint8_t H = 0;
    uint8_t M = 0;
    int D = 0;
    int MO = 0;
    int Y = 0;
  } S;

  ST.WiFiEnable = doc["wifi_st"];

  CFG.fw = doc["firmware"].as<String>();

  CFG.IP1 = doc["ip1"];
  CFG.IP2 = doc["ip2"];
  CFG.IP3 = doc["ip3"];
  CFG.IP4 = doc["ip4"];

  CFG.APPAS = doc["pass"].as<String>();

  CFG.sn = doc["sn"];

  CFG.APSSID = doc["ssid"].as<String>();

  sensors.T1_offset = doc["t1_offset"];

}

void ShowLoadJSONConfig()
{
  char msg[32] = {0}; // buff for send message

  Serial.println(F("##############  System Configuration  ###############"));
  Serial.println("-------------------- USER DATA -----------------------");
  Serial.printf("####  T1_OFFSET: %d \r\n", sensors.T1_offset);
  Serial.println("------------------ USER DATA END----------------------");
  Serial.println();
  Serial.println("---------------------- SYSTEM ------------------------");
  sprintf(msg, "####  DATA: %0002d-%02d-%02d", Clock.year, Clock.month, Clock.date);
  Serial.println(F(msg));
  sprintf(msg, "####  TIME: %02d:%02d:%02d", Clock.hour, Clock.minute, Clock.second);
  Serial.println(F(msg));
  Serial.printf("####  WiFI_PWR: %d \r\n", ST.WiFiEnable);
  Serial.printf("####  WiFI NAME: %s \r\n",CFG.APSSID);
  Serial.printf("####  WiFI PASS: %s \r\n", CFG.APPAS);
  sprintf(msg, "####  IP: %00d.%00d.%00d.%00d", CFG.IP1, CFG.IP2, CFG.IP3, CFG.IP4);
  Serial.println(F(msg));
  Serial.printf("####  SN: %d", CFG.sn);
  Serial.printf(" FW:");
  Serial.print(CFG.fw);
  Serial.println();
  Serial.println("-------------------- SYSTEM END-----------------------");
  Serial.println(F("######################################################"));
}

//////////////////////////////////////////////
// Save configuration in config.json file   //
//////////////////////////////////////////////
void SaveConfig()
{
  String jsonConfig = "";
  StaticJsonDocument<2048> doc;

  // doc["carname"] = String(UserText.carname);



  doc["firmware"] = CFG.fw;
  doc["ip1"] = CFG.IP1;
  doc["ip2"] = CFG.IP2;
  doc["ip3"] = CFG.IP3;
  doc["ip4"] = CFG.IP4;
  doc["pass"] = CFG.APPAS;
  doc["sn"] = CFG.sn;
  doc["ssid"] = CFG.APSSID;
  doc["t1_offset"] = sensors.T1_offset;

  doc["wifi_st"] = ST.WiFiEnable;


  File configFile = SPIFFS.open("/config.json", "w");
  serializeJson(doc, configFile); // Writing json string to file
  configFile.close();

  Serial.println("Configuration SAVE");
}

void TestDeserializJSON()
{
  String jsonConfig = "";

  File configFile = SPIFFS.open("/config.json", "r"); // Открываем файл для чтения
  jsonConfig = configFile.readString();               // загружаем файл конфигурации из EEPROM в глобальную переменную JsonObject
  configFile.close();

  StaticJsonDocument<2048> doc;
  deserializeJson(doc, jsonConfig); //  вызовите парсер JSON через экземпляр jsonBuffer

  serializeJsonPretty(doc, Serial);
  Serial.println();

  Serial.println("JSON testing comleted");
}
