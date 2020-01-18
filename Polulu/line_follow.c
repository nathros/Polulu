/**
 * @file line_follow.c
 * @author Sam Gamble
 * @brief Contains line following algorithms
 */

#include "line_follow.h"
#include "uart.h"

#define START_PID         0xBB
#define STOP_PID          0xBC

/***** Line following *****/
void nav_follow_line(int speed)
{
  uint8_t command[] = { START_PID, speed, 1, 20, 3, 2 };
  uart_send_blk(command, 6);
}

void nav_stop_follow_line()
{
  uart_send_byte(STOP_PID);
}
