#ifndef GPIO_H
#define GPIO_H

/**
 * @file gpio.h
 * @brief Collision sensor controller header
 * @note pin.h is in the source file, because its content does
 * not need to be exposed to the public.
 * @author Fufu Fang (wrote the initialisation and polling code, general clean up)
 * @author Nathan Bartram (wrote the interrupt code)
 */

/* Conditional compilation options */
//#define GPIO_DBG

/**
 * @brief Initialise collision sensor
 */
void gpio_init();

/**
 * @brief Read collision sensor data
 * @return 0, if no object is detected; 1, if collision is imminent.
 */
int gpio_collision_read();

/**
 * @brief Read button
 * @return 0, if button is not pressed; 1, if button is pressed
 */
int gpio_button_read();

/**
 * @brief Interrupt Service Routine for GPIO ports
 */
void EINT3_IRQHandler();

/**
 * @brief Enable GPIO interrupt
 */
void gpio_int_enable();

/**
 * @brief Disable GPIO interrupt
 */
void gpio_int_disable();

/**
 * @brief Wait for GPIO button click
 * @details Do nothing until the GPIO button is clicked.
 */
void gpio_button_wait();

/**
 * @brief Wait for collision
 * @details Do nothing until a collision is detected.
 */
void gpio_collision_wait();
#endif
