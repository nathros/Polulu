/**
 * @file line_search.c
 * @author Oliver Loftus
 * @brief line-searching algorithm (source)
 */


#include "line_search.h"
#include "motor.h"
#include "delay.h"
#include "pid.h"
#include "rit.h"

#include <lpc17xx_systick.h>
#include <lpc17xx_rit.h>

#define EDGE_DELAY     4000 // Proceed at EDGE_SPEED for EDGE_DELAY ms
#define EDGE_SPEED     20

static int lineSearching = 0;


void RIT_IRQHandler()
{
  RIT_GetIntStatus(LPC_RIT);
  pid_check();
}

void nav_search_line()
{
  RIT_Start(100);
//  int initSpeed = motor_read(MOTOR_LEFT);

  motor_rot_rel(-90);

  motors_stop();
  int facingRight = 0;

  lineSearching = 1;
  while (lineSearching && !PID_FLAG)
  {
    motors_move(EDGE_SPEED);
    delay_start(EDGE_DELAY);
    motor_rot_rel(facingRight ? -160 : 160);
    facingRight = facingRight ? 0 : 1;
  }
}

void nav_stop_search_line()
{
  lineSearching = 0;
}
