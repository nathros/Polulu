#ifndef LINE_H
#define LINE_H

/**
 * @file line_sensors.h
 * @brief Header for the line sensor module
 * @author Sam Gamble (created the file,)
 * @author Fufu Fang (clean up, added function descriptions)
 */

#include <stdint.h>

/**
 * @brief Reads the calibrated values from line sensors.
 * @param result An uint16_t array to store the result.
 while 1000 represents black
 *
 */
void line_read(uint16_t* result);

/**
 * @brief Read the raw values of line sensors into an array.
 */
void line_read_raw(uint16_t * result);

/**
 * @brief Calibrate and Read
 * @param result An uint16_t array to store the result.
 */
void line_calibrate_read(uint16_t * result);

/**
 * @brief Get current estimated line position
 * @return an estimated position of the line;
 */
uint16_t line_read_position();

/**
 * @brief Resets line sensor calibration.
 */
void line_calibration_reset();

/**
 * @brief Built-in line sensor auto-calibration
 */
int line_autocalibrate();

/**
 * @brief Detect the presence of the line
 */
int line_detect();
#endif
