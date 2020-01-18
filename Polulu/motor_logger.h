#ifndef MOTORLOGGER_H
#define MOTORLOGGER_H

/**
 * @file motor_logger.h
 * @brief Motor motion logger header
 * @warning Obsolete code!
 * @author Fufu Fang (wrote the whole thing)
 */

/**
 * @brief Motor Speed record structure
 */
typedef struct {
    short left_speed;
    short right_speed;
    short time;
} Motor_Record;

/**
 * @brief Interrupt Service Routine for Timer 3
 */
void TIMER3_IRQHandler();

/**
 * @brief Initialise logging module
 */
void motor_logger_init();

/**
 * @brief turn off the logging module
 */
void motor_logger_off();

/**
 * @brief Push a record onto the ring buffer
 */
void motor_logger_push();

/**
 * @brief Pop a motor record from the ring buffer
 * @return a motor record
 */
Motor_Record motor_logger_pop();
/**
 * @brief play back motor record
 */
void motor_logger_playback();

/**
 * @brief clear all log
 */
void motor_logger_clear();

#endif
