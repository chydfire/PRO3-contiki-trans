

#ifndef _LED_H
#define _LED_H
#include <SN8F5708.h>
#include "clock.h"

#define open_led1   P26 = 0
#define close_led1  P26 = 1
#define toggle_led1 P26 = ~P26
#define blink_led1  {P26 = ~P26;clock_delay_ms(200);P26 = ~P26;}
#define open_led2   P27 = 0
#define close_led2  P27 = 1
#define toggle_led2 P27 = ~P27
#define blink_led2  {P27 = ~P27;clock_delay_ms(200);P27 = ~P27;}

#define open_led_red         open_led1
#define close_led_red        close_led1
#define toggle_led_red       toggle_led1
#define blink_led_red        blink_led1
#define open_led_blue        open_led2
#define close_led_blue       close_led2
#define toggle_led_blue      toggle_led2
#define blink_led_blue        blink_led2

void led_init(void);


#endif