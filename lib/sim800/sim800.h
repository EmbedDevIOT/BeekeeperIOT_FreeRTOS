#ifndef SIM800_H_
#define SIM800_H_

#include <Arduino.h>

void sim800_init(u_long speed, uint8_t rx_pin, uint8_t tx_pin);
void sim800_conf();
String waitResponse();
String sendATCommand(String cmd, bool waiting);
void IncommingRing();
void SendUserSMS();
void GetLevel();

#endif 