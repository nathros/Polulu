#ifndef DELAY_H
#define DELAY_H

/**
 * @file delay.h
 * @brief Timer functions for causing delay (header)
 * @author Sam Gamble
 * @author Fufu Fang (tidying up)
 */

/**
 * @brief Delay function
 * @details cause the C function to delay certain amount of milliseconds,
 * to be used instead of waiting loops.
 * @param delay_length The length of delay in milliseconds
 */
void delay_start(int delay_length);

/**
 * @brief Music delay function
 * @details This function is used for sending music strings one after the
 * other, a delay is required to stop the 2nd string overwriting the first.
 */
void delay_music(int tempo, int beats_per_bar, int num_bars);

#endif
