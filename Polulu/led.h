#ifndef LED_H
#define LED_H

/**
 * @file led.h
 * @brief LED controller header
 * @author Fufu Fang (wrote the whole thing)
 */

#define LED_0 (1 << 18)
#define LED_1 (1 << 20)
#define LED_2 (1 << 21)
#define LED_3 (1 << 23)
#define LED_ALL (LED_0|LED_1|LED_2|LED_3)

/** LED initialisation */
void led_init();

/** Set the status of specified LED */
void led_set(int led);

/** Turn on certain LED */
void led_on(int led);

/** Turn off certain LED*/
void led_off(int led);

#endif
