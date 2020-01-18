/**
 * @file gpio.c
 * @brief GPIO module
 * @author Fufu Fang (wrote the GPIO initialisation and polling code,
 * general clean up)
 * @author Nathan Bartram (wrote the interrupt code)
 */

#include <lpc17xx_gpio.h>

#include "config.h"

#include "led.h"
#include "odometry.h"
#include "debug.h"

#include "gpio.h"

/* Button definition */
#define BUTTON_PORT 2
#define BUTTON_DETECT_PIN_NUM 5
#define BUTTON_PIN (1 << BUTTON_DETECT_PIN_NUM)

/* Wheel encoder definitions */
#define ENCODER_PORT 0
#define LEFT_PIN_NUM 5
#define LEFT_PIN (1 << LEFT_PIN_NUM)
#define RIGHT_PIN_NUM 4
#define RIGHT_PIN (1 << RIGHT_PIN_NUM)
#define ENCODER_PINS (RIGHT_PIN | LEFT_PIN)

/**
 * @note Please configure the pin and port number
 * accordingly.
 */
void gpio_init()
{
    GPIO_SetDir(ENCODER_PORT, LEFT_PIN, 0);
    GPIO_SetDir(ENCODER_PORT, RIGHT_PIN, 0);
    GPIO_SetDir(COLLISION_SENSOR_PORT, COLLISION_SENSOR_PIN, 0);
    GPIO_SetDir(BUTTON_PORT, BUTTON_PIN, 0);
}

int gpio_collision_read()
{
    if (COLLISION_SENSOR_PIN & GPIO_ReadValue(COLLISION_SENSOR_PORT)) {
        return 0;
    } else {
        return 1;
    }
}

int gpio_button_read()
{
    if (BUTTON_PIN & GPIO_ReadValue(BUTTON_PORT)) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * @details Only one interrupt service routine is available for all GPIO pins.
 */
void EINT3_IRQHandler()
{
    if (GPIO_GetIntStatus(ENCODER_PORT, RIGHT_PIN_NUM, 0)) {
        #ifdef GPIO_DBG
        dbg_printf("Right Wheel Encoder!\n\r");
        #endif

        #ifdef WHEEL_ENCODER
        encoder_update(RIGHT_ENCODER);
        #endif

        GPIO_ClearInt(ENCODER_PORT, RIGHT_PIN);
    }
    if (GPIO_GetIntStatus(ENCODER_PORT, LEFT_PIN_NUM, 0)) {
        #ifdef GPIO_DBG
        dbg_printf("Left Wheel Encoder!\n\r");
        #endif

        #ifdef WHEEL_ENCODER
        encoder_update(LEFT_ENCODER);
        #endif

        GPIO_ClearInt(ENCODER_PORT, LEFT_PIN);
    }
    if (GPIO_GetIntStatus(ENCODER_PORT, RIGHT_PIN_NUM, 1)) {
        #ifdef GPIO_DBG
        dbg_printf("Right Wheel Encoder!\n\r");
        #endif

        #ifdef WHEEL_ENCODER
        encoder_update(RIGHT_ENCODER);
        #endif

        GPIO_ClearInt(ENCODER_PORT, RIGHT_PIN);
    }
    if (GPIO_GetIntStatus(ENCODER_PORT, LEFT_PIN_NUM, 1)) {
        #ifdef GPIO_DBG
        dbg_printf("Left Wheel Encoder!\n\r");
        #endif

        #ifdef WHEEL_ENCODER
        encoder_update(LEFT_ENCODER);
        #endif

        GPIO_ClearInt(ENCODER_PORT, LEFT_PIN);
    }
    if (GPIO_GetIntStatus(BUTTON_PORT, BUTTON_DETECT_PIN_NUM, 0)) {
        #ifdef GPIO_DBG
        dbg_printf("Button pressed!\n\r");
        #endif

        GPIO_ClearInt(BUTTON_PORT, BUTTON_PIN);
    }
    if (GPIO_GetIntStatus(COLLISION_SENSOR_PORT, COLLISION_SENSOR_PIN_NUM, 1)) {
        #ifdef GPIO_DBG
        dbg_printf("Collision warning!\n\r");
        #endif

        GPIO_ClearInt(COLLISION_SENSOR_PORT, COLLISION_SENSOR_PIN);
    }
    #ifdef GPIO_DBG
        dbg_printf("GPIO Port values: %d\r\n", GPIO_ReadValue(0));
    #endif
}

/**
 * @note There are four interrupt can be set. They are:
 * - Port 0 Rising edge
 * - Port 0 Lowering edge
 * - Port 2 Rising edge
 * - Port 2 Lowering edge
 */
void gpio_int_enable()
{
    /* Wheel encoders are triggered at rising edge */
    GPIO_IntCmd(ENCODER_PORT, ENCODER_PINS, 0);

    /* Button interrupt is triggered at rising edge */
    GPIO_IntCmd(BUTTON_PORT, BUTTON_PIN, 0);

    /* Collision sensor interrupt is triggered at falling edge */
    GPIO_IntCmd(COLLISION_SENSOR_PORT, COLLISION_SENSOR_PIN|ENCODER_PINS, 1);

    /* Enable interrupt */
    NVIC_EnableIRQ(EINT3_IRQn);
}

void gpio_int_disable()
{
    NVIC_DisableIRQ(EINT3_IRQn);
}

void gpio_button_wait()
{
    while (gpio_button_read()==0) {
        led_on(LED_ALL);
    }
    led_off(LED_ALL);
}

void gpio_collision_wait()
{
    while (gpio_collision_read()==0) {
        led_on(LED_ALL);
    }
    led_off(LED_ALL);
}
