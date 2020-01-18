#ifndef PID
#define PID
/**
 * @file pid.h
 * @brief Header for Line following code
 * @author Sam Gamble
 */

/**
 * @brief Sets Flags which trigger line following
 * @details Explicitly sets flags to start / stop pid line following algorithm,
 * should not need to call in most cases, pid_check() will set flags when a
 * line is detected
 */
void pid_set();

/**
 * @brief clear PID flag
 * @details It can cause PID algorithm to exit.
 */
void pid_clear();

/**
 * @brief Line detection for the PID algorithm
 * @note should only run on entry to the PID LOOP
 */
int pid_line_detect();

/**
 * @brief Check if Line following is in PID LOOP
 * @note running -> return 1,  exited -> return 0
 */
int pid_check_running();

/**
 * @brief Detecting of LEFT / RIGHT Turn at end of line
 * @note should only run on entry to the PID LOOP
 */
int pid_turn_detect();

/**
 * @brief Calls Auto calibration on Polulu
 */
void pid_calibrate();

/**
 * @brief Checks for lines
 * @details Sets flags to enable/disable PID line following based on the
 * underside line sensors
 */
void pid_check();

/**
 * @brief Start Line following
 * @details Note, will do NOTHING unless flags are set using either
 * pid_check() or pid_set()
 */
void pid_run();

/**
 * @brief Factored out PID calculation function
 * @author Sam Gamble
 * @note based on the PID formula running on polulu, if it gives problems
 * i'll spend some quality time with MATLAB and work out why (if time allows)
 * @param[in] 'prop_current' expected range (-2000 .. 2000) if not in this
 * range returns 0
 * @param[in] 'prop_last', 'integral' and 'derivative' pointers to integers
 * defined in the calling function. Initial values of the variables should be
 * 0, cal
 */
int pid_calc(int prop_current, int *prop_last, int *integral, int *derivative);

/**
 * @brief Set Motors move fwd based on PID result
 * @author Sam Gamble
 * @warning Requires #define 'SPEED_OFFSET',currently = 0
 * (will be set on testing)
 * @param[in] power difference value (-30 .. +30) if above these values the
 * motor speeds will be capped to (SPEED_OFFSET + SPEED_LIMIT)
 */
void pid_motor_set(int power_diff);


int pid_turn_flg();

#endif
