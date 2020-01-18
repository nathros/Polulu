

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

short int LINE_FOUND_FLAG = 0;
short int MODE_RUNNING_FLAG = 0;
short int LINE_TURN_FLAG = 0;
uint16_t sensors[5];
int temp;

void init()
{
    debug_frmwrk_init();
    uart_init();
    ir_init();
    gpio_init();
    gpio_int_enable();
    //mouse_setup();
    line_autocalibrate();
    //RIT_Start(1000);
}

void main_line_turn()
{
    if (pid_turn_flg() == 1) {        
        play_music("!O6 T70 L6 V10 MS a8");
        while(!pid_turn_detect()) {
            motor_set(MOTOR_LEFT,-30);
            motor_set(MOTOR_RIGHT,30);
            //delay_start(400);
        }
        pid_set();
    }
        
     else if (pid_turn_flg() == 2) {
         play_music("!O6 T70 L6 V10 MS a8 a8");
        while(!pid_turn_detect()) {
            motor_set(MOTOR_LEFT,  30);
            motor_set(MOTOR_RIGHT,-30);
            //delay_start(400);
        }
        pid_set();
    }
    motors_stop();
    
}






void main_line_follow()
{
    if(MODE_RUNNING_FLAG) {
        return;
    }
    MODE_RUNNING_FLAG   = 1;

    do {
        pid_run();
        main_line_turn();
        } while( pid_turn_flg());

}

void main_hold()
{
    while(gpio_button_read());
}



void main()
{
    init();
    
    main_hold();
    pid_set();
    main_line_follow();
}
