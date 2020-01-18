/**
 * @file main.c
 * @author Oliver Loftus
 * @note
 * There exist 2 modes:
 *   1) Wall following
 *   2) Navigation
 *   3) Line following
 */

#include <math.h>

#include "motor.h"
#include "ir.h"
#include "uart.h"
#include "debug.h"
#include "wall_follow.h"
#include "line_search.h"
#include "config.h"
#include "gpio.h"
#include "pid.h"
#include "advanced_movement.h"
#include "coordinate.h"
#include "lcd.h"

#define OPMODE 1


void init();
void start_wf();
void start_nav();
void start_lf();


void main()
{
    init();
    
    switch (OPMODE)
    {
      case 1: start_wf();
      case 2: start_nav();
      case 3: start_lf();
    }
}

void init()
{
    debug_frmwrk_init();
    uart_init();
    ir_init();
    gpio_init();
    gpio_int_enable();
    mouse_setup();
}

void start_wf()
{
  motors_move(CRUISE_SPEED);
  nav_follow_wall(WALL_TO_FOLLOW);
}

void start_nav()
{
  motors_stop();
  gpio_button_wait();
  
  adv_travel(200);
  adv_move_polar(100, 90);
  adv_move_polar(0, 0);

  Coordinate coord = coordinate_read();
  
  lcd_printf("Current location: X:%d; Y:%d; T:%d", coord.x, coord.y, coord.t);
}

void start_lf()
{
  nav_search_line();
  pid_run();
}
