#ifndef button_h_
#define button_h_

#include <Arduino.h>

#define BUTTON_PROTECTION 20

struct ButtonState
{
    bool S1 = false; 
    bool S2 = false; 
    bool S3 = false; 
};
extern ButtonState BT;

//физическое состояние кнопки
enum ButtonResult {
  buttonNotPress,   //если кнопка не нажата
  button_SB1_Press, //если кнопка SB1 нажата
  button_SB2_Press, //если кнопка SB2 нажата
  button_SB3_Press, //если кнопка SB3 нажата

};

void Button_init(uint8_t _pin, uint8_t state);
int Button_read(int _pin);


#endif