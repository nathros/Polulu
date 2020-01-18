

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
    //RIT_Start(8000);
}


void main_check_mouse_data()
{
    dbg_print_coordinate(coordinate_read());
}


void RIT_IRQHandler(void)
{
 
    RIT_GetIntStatus(LPC_RIT);
    //adv_exit();
    //main_check_mouse_data();
}




void main_fwd_move(float x, float y)
{
    if(MODE_RUNNING_FLAG) {
        return;
    }

    MODE_RUNNING_FLAG = 1;
    //adv_move_pid_polar(distance, angle_dir);
    
}


void main_hold()
{
    while(!gpio_button_read());
}


void main()
{
    init();
    
    main_hold();
    
    //while(1) adv_print_rmp();
    adv_motors_rpm(20);
    //motor_rot_rel(90);
    
    
}
