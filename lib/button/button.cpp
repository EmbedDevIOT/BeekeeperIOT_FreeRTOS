#include "button.h"

int previousButtonState = HIGH;
bool flag = false;

void Button_init(uint8_t _pin, uint8_t state)
{
    pinMode(_pin, state);
}

// debounce read pin
int Button_read(int _pin)
{
    int ButtonState = digitalRead(_pin);

    if (ButtonState && !flag)
    {
        flag = true;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    if (!ButtonState && flag)
    {
        flag = false;
    }
    return ButtonState;
}

    //   if (!Button_read(SET_PIN) && !ST.BTSET)
    //   {
    //     Serial.println("SET press");
    //     ST.BTSET = true;
    //   }
    //   else if (Button_read(SET_PIN) && ST.BTSET)
    //   {
    //     ST.BTSET = false;
    //   }

    // if (!Button_read(PL_PIN) && !ST.BTUP)
    // {
    //   Serial.println("UP press");
    //   ST.BTUP = true;

    // }
    // else if (Button_read(PL_PIN) && ST.BTUP)
    // {
    //   ST.BTUP = false;
    // }

    // if (!Button_read(MN_PIN) && !ST.BTDWN)
    // {
    //   Serial.println("DWN press");
    //   ST.BTDWN = true;
    // }
    // else if (Button_read(MN_PIN) && ST.BTDWN)
    // {
    //   ST.BTDWN = false;
    // }
