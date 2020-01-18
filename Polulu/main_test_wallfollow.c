

#include <lpc17xx_rit.h>

#include "motor.h"
#include "ir.h"
#include "uart.h"
#include "debug.h"
#include "wall_follow.h"
#include "config.h"
#include "gpio.h"
#include "pid.h"
#include "advanced_movement.h"
#include "coordinate.h"
#include "rit.h"
#include "line_sensors.h"
#include "utilities.h"
#include "delay.h"

short int MODE_RUNNING_FLAG = 0;

void init()
{
    debug_frmwrk_init();
    uart_init();
    ir_init();
    gpio_init();
    gpio_int_enable();
    //mouse_setup();
    //line_autocalibrate();
    RIT_Start(8000);
}


void RIT_IRQHandler(void)
{
 
    RIT_GetIntStatus(LPC_RIT);

    follow_wall_stop();
    motors_stop();
}

void main_follow_wall(nav_Wall wall_side)
{
    if(MODE_RUNNING_FLAG) {
        return;
    }

    MODE_RUNNING_FLAG = 1;
    motor_set(MOTOR_LEFT, 30);
    motor_set(MOTOR_RIGHT, 30);
    nav_follow_wall(wall_side);
    


}

void main_hold()
{
    while(gpio_button_read());
}


void main()
{
    init();
    main_hold();
    main_follow_wall(WALL_LEFT);
    
    
    
}
