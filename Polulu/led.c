/**
 * @file led.c
 * @brief LED controller
 * @author Fufu Fang (wrote the whole thing)
 */

#include <lpc17xx_gpio.h>
#include "led.h"
void led_init()
{
    GPIO_SetDir(1, LED_ALL, 1);
}

void led_set(int led)
{
    GPIO_ClearValue(1, LED_ALL);
    GPIO_SetValue(1, led);
}

void led_on(int led)
{
    GPIO_SetValue(1, led);
}

void led_off(int led)
{
    GPIO_ClearValue(1, led);
}
