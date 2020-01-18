/**
 * @file fufu_test.h
 * @brief Fufu's test functions (header0
 * @author Fufu Fang
 */

#ifndef TEST_H
#define TEST_H

/**
 * @brief variable size tests
 * @details Output the maximum values that
 * can be stored in different types of variables to the
 * debug console.
 * @author Fufu Fang
 */
void size_test();

/**
 * @brief IR sensor test
 * @details Output the readings of the IR sensor to the
 * debug console.
 * @author Fufu Fang
 */
void ir_test();

/**
 * @brief LCD display test
 * @details Output a single character typed into the
 * debug console to LCD display
 * @author Fufu Fang
 */
void lcd_test();

/**
 * @brief Test the motor logger
 * @warning currently disabled
 */
void motor_logger_test();

/**
 * @brief Motor bumping test
 */
void bumping_test();

/**
 * @brief Motor rotation test
 */
void rotate_test();

/**
 * @brief check IRQ priorities
 */
void IRQ_priority_test();

/**
 * @brief Test the vector logger
 * @warning currently disabled
 */
void vector_logger_test();

/**
 * @brief motors movement test
 * @details move both motors at maximum speed for 5 seconds,
 * in order to test if the vehicle move in a straight line.
 */
void motors_move_test();

/**
 * @brief GPIO test functions
 * @details display GPIO readings
 */
void gpio_test();

#ifdef COORD_DBG
/**
 * @brief coordinate record reading test
 * @details do something then read off the coordinate,repeat 10 times,
 * print out the average
 * @param[in] *func the function to be performed
 * @param[in] size the number of tests to be performed
 */
void coordinate_test(void (*func)(), int size);
#endif

#ifdef MOUSE_DEBUG
/**
 * @brief mouse record reading test
 * @details do something then read off the mouse,repeat 10 times,
 * print out the average
 * @param[in] *func the function to be performed
 * @param[in] size the number of tests to be performed
 */
void mouse_test(void (*func)(), int size);
#endif

#endif
