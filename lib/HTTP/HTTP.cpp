#include "HTTP.h"

WebServer HTTP(80);

///////////////////////////////
// Initialisation WebServer  //
///////////////////////////////
void HTTPinit()
{
  HTTP.begin();
  // ElegantOTA.begin(&HTTP); // Start ElegantOTA
  HTTP.on("/update.json", UpdateData);
  HTTP.on("/wcupd.json", UpdateStateWC);
  HTTP.on("/SysUPD", SystemUpdate);
  HTTP.on("/TimeUPD", TimeUpdate);
  HTTP.on("/TextUPD", TextUpdate);
  HTTP.on("/ColUPD", ColorUpdate);
  HTTP.on("/SNUPD", SerialNumberUPD);
  HTTP.on("/WCLUPD", WCLogiqUPD);
  HTTP.on("/WiFiUPD", SaveSecurity);
  HTTP.on("/BRBT", Restart);              // Restart MCU
  HTTP.on("/BTTS", TimeToSpeech);         // Tell me Date an Time
  HTTP.on("/BDS1", WC1DoorStateToSpeech); // Tell me Door state
  HTTP.on("/BDS2", WC2DoorStateToSpeech); // Tell me Door state
  HTTP.on("/FW", ShowSystemInfo);
  HTTP.on("/BFRST", FactoryReset);               // Set default parametrs.
  HTTP.onNotFound([]() {                         // Event "Not Found"
    if (!handleFileRead(HTTP.uri()))             // If function  handleFileRead (discription bellow) returned false in request for file searching in file syste
      HTTP.send(404, "text/plain", "Not Found"); // return message "File isn't found" error state 404 (not found )
  });
}

////////////////////////////////////////
// File System work handler           //
////////////////////////////////////////
bool handleFileRead(String path)
{
  if (path.endsWith("/"))
    path += "index.html";                    // Если устройство вызывается по корневому адресу, то должен вызываться файл index.html (добавляем его в конец адреса)
  String contentType = getContentType(path); // С помощью функции getContentType (описана ниже) определяем по типу файла (в адресе обращения) какой заголовок необходимо возвращать по его вызову
  if (SPIFFS.exists(path))
  {                                                   // Если в файловой системе существует файл по адресу обращения
    File file = SPIFFS.open(path, "r");               //  Открываем файл для чтения
    size_t sent = HTTP.streamFile(file, contentType); //  Выводим содержимое файла по HTTP, указывая заголовок типа содержимого contentType
    file.close();                                     //  Закрываем файл
    return true;                                      //  Завершаем выполнение функции, возвращая результатом ее исполнения true (истина)
  }
  return false; // Завершаем выполнение функции, возвращая результатом ее исполнения false (если не обработалось предыдущее условие)
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Функция, возвращающая необходимый заголовок типа содержимого в зависимости от расширения файла //
////////////////////////////////////////////////////////////////////////////////////////////////////
String getContentType(String filename)
{
  if (filename.endsWith(".html"))
    return "text/html"; // Если файл заканчивается на ".html", то возвращаем заголовок "text/html" и завершаем выполнение функции
  else if (filename.endsWith(".css"))
    return "text/css"; // Если файл заканчивается на ".css", то возвращаем заголовок "text/css" и завершаем выполнение функции
  else if (filename.endsWith(".js"))
    return "application/javascript"; // Если файл заканчивается на ".js", то возвращаем заголовок "application/javascript" и завершаем выполнение функции
  else if (filename.endsWith(".png"))
    return "image/png"; // Если файл заканчивается на ".png", то возвращаем заголовок "image/png" и завершаем выполнение функции
  else if (filename.endsWith(".ttf"))
    return "font/ttf"; // Если файл заканчивается на ".png", то возвращаем заголовок "image/png" и завершаем выполнение функции
  else if (filename.endsWith(".bmp"))
    return "image/bmp";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg"; // Если файл заканчивается на ".jpg", то возвращаем заголовок "image/jpg" и завершаем выполнение функции
  else if (filename.endsWith(".gif"))
    return "image/gif"; // Если файл заканчивается на ".gif", то возвращаем заголовок "image/gif" и завершаем выполнение функции
  else if (filename.endsWith(".svg"))
    return "image/svg+xml";
  else if (filename.endsWith(".ico"))
    return "image/x-icon"; // Если файл заканчивается на ".ico", то возвращаем заголовок "image/x-icon" и завершаем выполнение функции
  return "text/plain";     // Если ни один из типов файла не совпал, то считаем что содержимое файла текстовое, отдаем соответствующий заголовок и завершаем выполнение функции
}
/*******************************************************************************************************/

/*******************************************************************************************************/
// Time data dynamic update
void UpdateData()
{
  String buf = "{";

  buf += "\"t\":\"";
  buf += ((Clock.hour < 10) ? "0" : "") + String(Clock.hour) + ":" + ((Clock.minute < 10) ? "0" : "") + String(Clock.minute) + "\",";
  buf += "\"d\":\"";
  buf += String(Clock.year) + "-" + ((Clock.month < 10) ? "0" : "") + String(Clock.month) + "-" + ((Clock.date < 10) ? "0" : "") + String(Clock.date) + "\"";
  buf += "}";

  HTTP.send(200, "text/plain", buf);
}
/*******************************************************************************************************/

/*******************************************************************************************************/
// Time and Date update
void TimeUpdate()
{
  char TempBuf[15];
  char msg[32] = {0};

  struct _sys
  {
    uint8_t H = 0;
    uint8_t M = 0;
    int8_t D = 0;
    int8_t MO = 0;
    int16_t Y = 0;
  } S;

  HTTP.arg("T").toCharArray(TempBuf, 10);
  S.H = atoi(strtok(TempBuf, ":"));
  S.M = atoi(strtok(NULL, ":"));

  HTTP.arg("D").toCharArray(TempBuf, 15);
  S.Y = atoi(strtok(TempBuf, "-"));
  S.MO = atoi(strtok(NULL, "-"));
  S.D = atoi(strtok(NULL, "-"));

  CFG.gmt = HTTP.arg("GMT").toInt();

  Clock.hour = S.H;
  Clock.minute = S.M;
  Clock.year = S.Y;
  Clock.month = S.MO;
  Clock.date = S.D;

  sprintf(msg, "GMT: %d", CFG.gmt);
  Serial.println(msg);
  sprintf(msg, "Time: %d : %d", S.H, S.M);
  Serial.println(msg);
  sprintf(msg, "DataIN: %0004d.%02d.%02d", S.Y, S.MO, S.D);
  Serial.println(msg);
  sprintf(msg, "DataRTC: %0004d.%02d.%02d", Clock.year, Clock.month, Clock.date);
  Serial.println(msg);

  RTC.setTime(Clock);
  SaveConfig();
  
  Serial.println("Time Update");
  HTTP.send(200, "text/plain", "OK");
}
/*******************************************************************************************************/

/*******************************************************************************************************/
void UpdateStateWC()
{
  String buf = "{";
  buf += "\"st_wc1\":";
  buf += STATE.StateWC1;
  buf += ",";
  buf += "\"st_wc2\":";
  buf += STATE.StateWC2;
  buf += "}";
  HTTP.send(200, "text/plain", buf);
}
/*******************************************************************************************************/

/*******************************************************************************************************/
void SystemUpdate()
{
  HCONF.T1_offset = HTTP.arg("T1O").toInt();
  HCONF.T2_offset = HTTP.arg("T2O").toInt();
  HCONF.bright = HTTP.arg("BR").toInt();
  HCONF.volume = HTTP.arg("VOL").toInt();

#ifndef DEBUG
  Serial.printf("T1_OFFset: %d", HCONF.T1_offset);
  Serial.println();
  Serial.printf("T2_OFFset: %d", HCONF.T2_offset);
  Serial.println();
  Serial.printf("Brigh: %d", HCONF.bright);
  Serial.println();
  Serial.printf("Volume: %d", HCONF.volume);
  Serial.println();
#endif

  SaveConfig();
  STATE.StaticUPD = true;
  STATE.cnt_Supd = 0;
  STATE.VolumeUPD = true;

  // Show Led state (add function)
  Serial.println("System Update");
  HTTP.send(200, "text/plain", "OK");
}
/*******************************************************************************************************/

/*******************************************************************************************************/
void TextUpdate()
{
  char TempBuf[10];
  char msg[512] = {0};

  struct _txt
  {
    char TN[17];     // car_name
    uint8_t TNU = 0; // car num
    bool SWH = false;
  } T;

  HTTP.arg("TN").toCharArray(T.TN, 17);
  T.TNU = HTTP.arg("TNU").toInt();

  T.SWH = HTTP.arg("SWH").toInt();

  memset(UserText.carname, 0, strlen(UserText.carname));
  strcat(UserText.carname, T.TN);

  UserText.carnum = T.TNU;

  // Если новое состояние != старому
  if (T.SWH != UserText.hide_t)
  {
    UserText.hide_t = T.SWH;

    // SendXMLDataD();
  }

  // #ifndef DEBUG
  Serial.printf("Name: ");
  Serial.printf(T.TN);
  Serial.println(msg);

  Serial.printf("CarNum: ");
  Serial.println(T.TNU);

  Serial.printf("Hide Carnum: %d", T.SWH);
  Serial.println();
  // #endif

  SaveConfig();
  // ShowLoadJSONConfig();

  // Show Led state (add function)

  Serial.println("Text Update");
  HTTP.send(200, "text/plain", "OK");
}
/*******************************************************************************************************/

/*******************************************************************************************************/
void ColorUpdate()
{
  struct _col
  {
    // uint8_t CDY = HTTP.arg("CDY").toInt();   // color day weeks
    uint8_t CC = HTTP.arg("CC").toInt();   // color car num
    uint8_t CT = HTTP.arg("CT").toInt();   // color time
    uint8_t CD = HTTP.arg("CD").toInt();   // color date
    uint8_t CTI = HTTP.arg("CTI").toInt(); // color temp IN
    uint8_t CTO = HTTP.arg("CTO").toInt(); // color temp OUT
    uint8_t CSP = HTTP.arg("CSP").toInt(); // color speed
  } C;

  // #ifndef DEBUG
  //   Serial.printf("Color CarNum: %d", C.CC);
  //   Serial.println();
  //   Serial.printf("Color InfoText: %d", C.CI);
  //   Serial.println();
  //   Serial.printf("Color Time: %d", C.CT);
  //   Serial.println();
  //   Serial.printf("Color Date: %d", C.CD);
  //   Serial.println();
  //   Serial.printf("Color TempIN: %d", C.CTI);
  //   Serial.println();
  //   Serial.printf("Color TempOUT: %d", C.CTO);
  //   Serial.println();
  // #endif

  ColorSet(&col_carnum, C.CC);
  ColorSet(&col_time, C.CT);
  ColorSet(&col_date, C.CD);
  // ColorSet(&col_day, C.CDY);
  ColorSet(&col_tempin, C.CTI);
  ColorSet(&col_tempout, C.CTO);
  ColorSet(&col_speed, C.CSP);

  SaveConfig();
  STATE.StaticUPD = true;
  STATE.cnt_Supd = 0;
  // ShowLoadJSONConfig();

  Serial.println("Сolor Update");
  HTTP.send(200, "text/plain", "OK");
}

/*******************************************************************************************************/
/*******************************************************************************************************/
void WCLogiqUPD(void)
{
  HCONF.WCL = HTTP.arg("WCL").toInt();   // WC_STATE_LOGIQ
  HCONF.WCSS = HTTP.arg("WCSS").toInt(); // WC_SENSOR_SIGNAL
  HCONF.WCGS = HTTP.arg("WCGS").toInt(); // WC_SENSOR_GET_SIGNAL

  SaveConfig();
  Serial.printf("WCL: %d WCSS: %d WCGS: %d \r\n", HCONF.WCL, HCONF.WCSS, HCONF.WCGS);
  HTTP.send(200, "text/plain", "Serial Number set");
}
/*******************************************************************************************************/
/*******************************************************************************************************/
void SerialNumberUPD()
{
  CFG.sn = HTTP.arg("sn").toInt();
  Serial.printf("SN:");
  Serial.println(CFG.sn);
  SaveConfig();

  // EEP_Write();
  HTTP.send(200, "text/plain", "Serial Number set");
}
/*******************************************************************************************************/
/*******************************************************************************************************/
void HandleClient()
{
  HTTP.handleClient();
}
/*******************************************************************************************************/
/*******************************************************************************************************/
void SaveSecurity()
{
  char TempBuf[10];

  CFG.APSSID = HTTP.arg("ssid");
  CFG.APPAS = HTTP.arg("pass");

#ifndef DEBUG
  Serial.println("Network name:");
  Serial.print(CFG.APSSID);
  Serial.println();
  Serial.print("Network password:");
  Serial.println(CFG.APPAS);
#endif

  SaveConfig();
  // ShowLoadJSONConfig();

  HTTP.send(200, "text/plain", "OK");
}
/*******************************************************************************************************/
/*******************************************************************************************************/
void TimeToSpeech()
{
  STATE.TTS = true;
  HTTP.send(200, "text/plain", "OK");
}
/*******************************************************************************************************/
/*******************************************************************************************************/
void WC1DoorStateToSpeech()
{
  STATE.DSTS1 = true;
  HTTP.send(200, "text/plain", "OK");
}
void WC2DoorStateToSpeech()
{
  STATE.DSTS2 = true;
  HTTP.send(200, "text/plain", "OK");
}
/*******************************************************************************************************/
/*******************************************************************************************************/
// ESP Restart
void Restart()
{
  HTTP.send(200, "text/plain", "OK"); // Oтправляем ответ Reset OK
  Serial.println("Restart Core");
  ESP.restart(); // перезагружаем модуль
}
/*******************************************************************************************************/
/*******************************************************************************************************/
void FactoryReset()
{
  HTTP.send(200, "text/plain", "OK"); // Oтправляем ответ Reset OK
  Serial.println("#### FACTORY RESET ####");
  SystemFactoryReset();
  SaveConfig();
  // EEP_Write();
  ShowFlashSave();
  Serial.println("#### SAVE DONE ####");
  ESP.restart(); // перезагружаем модуль
}
/*******************************************************************************************************/
void ShowSystemInfo()
{
  char msg[30];

  Serial.printf("System Information");
  sprintf(msg, "%s.%d", CFG.fw, CFG.sn);
  // SendXMLUserData(msg);

  HTTP.send(200, "text/plain", "OK"); // Oтправляем ответ Reset OK
}