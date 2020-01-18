#ifndef RIT
#define RIT
/**
 * @brief Repetitive Interrupt Timer Controller (Header)
 * @note the actual interrupt service routine is in the main.c
 * @author Sam Gamble
 */


/**
 * @brief Initialise RIT timer
 * @param[in] time time in milliseconds
 */
void RIT_Start(int time);

/**
 * @brief Re-enable RIT timer
 * @pre RIT_Start must have been called previously
 */
void RIT_Enable(void);

/**
 * @brief Disable RIT timer
 */
void RIT_Disable(void);

#endif
