#include "sim800.h"
#include "Config.h"

HardwareSerial SIM800(1);
String _response = "";

/*******************************************************************************************************/
void sim800_init(u_long speed, uint8_t rx_pin, uint8_t tx_pin)
{
    SIM800.begin(speed, SERIAL_8N1, rx_pin, tx_pin);
    Serial.println("SIM800_init...");
}

/*******************************************************************************************************/
void sim800_conf()
{
    sendATCommand("AT", true);
    sendATCommand("AT+CMGDA=\"DEL ALL\"", true);

    sendATCommand("AT+CMGF=1;&W", true);
    sendATCommand("AT+IFC=1, 1", true);
    sendATCommand("AT+CPBS=\"SM\"", true);
    sendATCommand("AT+CLIP=1", true);
    sendATCommand("AT+CNMI=1,2,2,1,0", true);
}
/*******************************************************************************************************/
String waitResponse()
{
    String _resp = "";
    long _timeout = millis() + 10000;
    while (!SIM800.available() && millis() < _timeout)
    {
    };
    if (SIM800.available())
    {
        _resp = SIM800.readString();
    }
    else
    {
        Serial.println("Timeout...");
    }
    return _resp;
}
/*******************************************************************************************************/

/*******************************************************************************************************/
String sendATCommand(String cmd, bool waiting)
{
    String _resp = "";
    // Serial.println(cmd);
    SIM800.println(cmd);
    if (waiting)
    {
        _resp = waitResponse();
        if (_resp.startsWith(cmd))
        {
            _resp = _resp.substring(_resp.indexOf("\r", cmd.length()) + 2);
        }
        // Serial.println(_resp);
    }
    return _resp;
}
/*******************************************************************************************************/

/*******************************************************************************************************/
void sendSMS(String phone, String message)
{
    sendATCommand("AT+CMGS=\"" + phone + "\"", true);
    sendATCommand(message + "\r\n" + (String)((char)26), true); // 26
}
/*******************************************************************************************************/

/*******************************************************************************************************/
void GetLevel()
{
    String msg;
    int level = 0;
    msg = sendATCommand("AT+CSQ", true);
    msg.trim();
    msg = msg.substring(6);
    sensors.signal = msg.toInt();
}
/*******************************************************************************************************/

/*******************************************************************************************************/
void IncommingRing()
{
    if (SIM800.available())
    {
        _response = waitResponse();
        _response.trim();
        // Serial.println(_response);

        if (_response.startsWith("RING"))
        {
            char bufRing[128] = "";
            ST.Call_Block = true;
            int phoneindex = _response.indexOf("+CLIP: \"");
            String innerPhone = "";

            if (phoneindex >= 0)
            {
                phoneindex += 8;
                innerPhone = _response.substring(phoneindex, _response.indexOf("\"", phoneindex));
                // Serial.println("Number: " + innerPhone);
                // delay(500);
                sendATCommand("ATH", true);
                // delay(500);
                char buf[128] = "";

                strcat(buf, "Bec: ");
                dtostrf(sensors.kg, 3, 1, buf + strlen(buf));
                strcat(buf, " kg\n");
                strcat(buf, "T1: ");
                dtostrf(sensors.dsT, 3, 1, buf + strlen(buf));
                strcat(buf, " *C\n");
                strcat(buf, "T2: ");
                dtostrf(sensors.bmeT, 3, 1, buf + strlen(buf));
                strcat(buf, " *C\n");
                strcat(buf, "H: ");
                itoa(sensors.bmeH, buf + strlen(buf), DEC);
                strcat(buf, " %\n");
                strcat(buf, "P: ");
                itoa(sensors.bmeP_mmHg, buf + strlen(buf), DEC);
                strcat(buf, "\n");
                strcat(buf, "B: ");
                itoa(sensors.voltage, buf + strlen(buf), DEC);
                strcat(buf, " %\n");
                strcat(buf, "Signal: ");
                itoa(sensors.signal, buf + strlen(buf), DEC);
                strcat(buf, " %\n");

                sendSMS(innerPhone, buf);

                phoneindex = -1;
                _response.clear();
                ST.Call_Block = false;
                sendATCommand("AT+CMGDA=\"DEL ALL\"", true);
            }
        }
    }
}
/*******************************************************************************************************/
void SendUserSMS()
{
    char buf[128] = "";

    strcat(buf, "Bec: ");
    dtostrf(sensors.kg, 3, 1, buf + strlen(buf));
    strcat(buf, " kg\n");
    strcat(buf, "T1: ");
    dtostrf(sensors.dsT, 3, 1, buf + strlen(buf));
    strcat(buf, " *C\n");
    strcat(buf, "T2: ");
    dtostrf(sensors.bmeT, 3, 1, buf + strlen(buf));
    strcat(buf, " *C\n");
    strcat(buf, "H: ");
    itoa(sensors.bmeH, buf + strlen(buf), DEC);
    strcat(buf, " %\n");
    strcat(buf, "P: ");
    itoa(sensors.bmeP_mmHg, buf + strlen(buf), DEC);
    strcat(buf, "\n");
    strcat(buf, "B: ");
    itoa(sensors.voltage, buf + strlen(buf), DEC);
    strcat(buf, " %\n");
    strcat(buf, "Signal: ");
    itoa(sensors.signal, buf + strlen(buf), DEC);
    strcat(buf, " %\n");

#ifdef DEBUG
    Serial.printf("MSG:");
    Serial.println(buf);
    Serial.printf("lenght:%d", strlen(buf));
    Serial.println();
#endif
    // #ifndef DEBUG
    sendSMS(Config.phone, buf);
    // #endif
}