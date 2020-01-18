#ifndef UTILITIES_H
#define UTILITIES_H

/**
 * @file utilities.h
 * @brief Polulu control utilities (header)
 * @details This module handles miscellaneous commands to AVR processor on the Polulu
 * @author Sam Gamble (added in the function defs)
 * @author Fufu Fang (tidying up)
 */

/**
 * @brief Request the version of the slave code that is running.
 * @note This function also shuts down the motors and disables PID,
 * so it is useful as an initial command.
 * @return version number of the slave code
 */
int request_sig(char *read_sig);

/**
 * @brief Request the trimpot value
 * @details the trimpot is used for some kind of calibration.
 * @return current trimpot settings, between 0 to 1023.
 */
int read_trimpot();

/**
 * @brief Request battery voltage
 * @return battery voltage in milivolt
 */
int read_battery();

/**
 * @brief Play music on Pololu
 * @details For how to compose music score, please look at:
 * http://www.pololu.com/docs/0J18/3
 * @param score pointer to your music score
 */
void play_music(char *score);
#endif
