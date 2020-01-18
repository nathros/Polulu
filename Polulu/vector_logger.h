#ifndef VECTOR_H
#define VECTOR_H

/**
 * @file motor_logger.h
 * @brief Vector logger header
 * @warning obsolete code!
 * @author Fufu Fang (wrote the whole thing)
 */

/**
 * @brief Vector record structure
 */
typedef struct {
    short speed;
    short angle;
    short time;
} Vector_Record;

/**
 * @brief Interrupt Service Routine for Timer 3
 */
void TIMER3_IRQHandler();

/**
 * @brief Initialise logging module
 */
void vector_logger_init();

/**
 * @brief turn off the logging module
 */
void vector_logger_off();

/**
 * @brief Push a record onto the ring buffer
 */
void vector_logger_push();

/**
 * @brief Pop a vector record from the ring buffer
 * @return a vector record
 */
Vector_Record vector_logger_pop();
/**
 * @brief play back vector record
 */
void vector_logger_playback();

/**
 * @brief clear all log
 */
void vector_logger_clear();

#endif
