/**
 * @file pid.c
 * @brief Line following code
 * @author Sam Gamble
 */

#include "line_sensors.h"
#include "motor.h"
#include "pid.h"
#include "debug.h"
#include "gpio.h"
#include "line_follow.h"
#include "advanced_movement.h"
//#include "config.h"

#define Kp_l            45
#define Ki_l            10000
#define Kd_l            500
#define LINE_MISS_MAX   4   //after n consecutive detect line 'fails'  the PID algorithm will end
#define MIN_SPEED       3
#define MAX_SPEED       30
#define DEBUG

//#define MBED_PID_1
#define MBED_PID_2
//#define POLULU_PID


short int PID_FLAG      = 0;
short int PID_RUNNING   = 0;
short int miss_counter  = 0;
short int hit_counter   = 0;
char sensor_pattern     = 0;
uint16_t sensors[5];



/**
 * Cases
 * 'B' = Black line, all sensors dark
 * 'F' = Follow, Line is under middle sensors
 * 'L' = Left Turn detected, Line under middle sensors && leftmost sensor
 * 'R' = Right turn detected   ..
 */
char pid_match_pattern(uint16_t * sensor)
{
    if ( sensor[0] > 900  
      && sensor[1] > 900 
      && sensor[2] > 900 
      && sensor[3] > 900 
      && sensor[4] > 900) {
        return 'B';   //solid black line, STOP!
    }
                     return 'R';
    if ( sensor[1] < 200  
      && sensor[2] < 200 
      && sensor[3] < 200) {
        return 'E';   // exit pattern
    }
    return 0;
}

int pid_turn_detect()
{
    line_calibrate_read(sensors);
    
    
// CASE: LEFT TURN    
    if( sensors[0] > 700 
     && (sensors[1] > 300 
      || sensors[2] > 300 
      || sensors[3] > 300)
       && sensors[4] < 200) {
         return 1;
    }
    
// CASE: RIGHT TURN 
    if( sensors[0] < 200 
     && (sensors[1] > 300 
      || sensors[2] > 300 
      || sensors[3] > 300)
       && sensors[4] > 700) {
         return 2;
    }
    
    return 0;
}

void pid_read_sensors()
{
    line_calibrate_read(sensors);
    sensor_pattern = pid_match_pattern(sensors);

    #ifdef DEBUG
    dbg_printf( "Sensor Values %d %d %d %d %d\n\r" 
    ,sensors[0], sensors[1],sensors[2],sensors[3],sensors[4]);
    
    dbg_printf("Sensor ptn: %c\n\r",sensor_pattern);
    #endif
}


//should only run on entry to the PID LOOP
int pid_line_detect()
{
    #ifdef DEBUG
    dbg_printf("-pid line detect-\n\r");
    #endif

    pid_read_sensors(sensor_pattern);
    if(sensor_pattern == 'B') {
        return 0;
    }
    
    if( sensors[0] == 1000 
     || sensors[1] == 1000 
     || sensors[2] == 1000 
     || sensors[3] == 1000 
     || sensors[4] == 1000) {
         return 1;
    }
    return 0;
}



int pid_over_line()
{
    #ifdef DEBUG
    dbg_printf("-pid over line detect-\n\r");
    #endif

    pid_read_sensors();
    if(sensor_pattern == 'B') 
        return 0;
    if(sensor_pattern == 'E') 
        return 0;
    
    return 1;
}




/**
 * Set and Clear PID Flags used to track the current state of the PID loop
 *
 */
void pid_set()
{
    #ifdef DEBUG
    dbg_printf("PID_FLAG = 1\n\r");
    #endif

    #ifdef POLULU_PID
    nav_follow_line(35);
    #endif
    PID_FLAG = 1;
}

void pid_clear()
{
    #ifdef DEBUG
    dbg_printf("PID_FLAG = 0\n\r");
    #endif

    PID_FLAG = 0;
    PID_RUNNING = 0;

    #ifdef POLULU_PID
    nav_stop_follow_line();
    #endif
}


/*
 * PID Check functions, used to know when to enter or leave the main PID loop
 *
 * Only 'pid_check()'  is intended for external used

void pid_check()
{
    //if (PID_RUNNING) return;
    #ifdef DEBUG
    dbg_printf("-pid check-\n\r");
    #endif

    pid_clear();
    if (pid_line_detect()) 
        pid_set();
}
 */

void pid_exit_check()
{
    #ifdef DEBUG
    dbg_printf("-Exit check-\n\r",miss_counter);
    #endif

    if (pid_over_line()){
        hit_counter++;
        miss_counter = 0;
    } 
    else {
        hit_counter = 0;
        miss_counter++;
        if (miss_counter >= LINE_MISS_MAX) {
            pid_clear();
        }
    }
  
    #ifdef DEBUG
    dbg_printf("miss cnt = %d   hit cnt = %d\n\r",miss_counter,hit_counter);
    #endif
}

//-POLULU PID--------------------------------------------------------*/
#ifdef POLULU_PID
void pid_control_loop()
{
    PID_RUNNING = 1;
    while(PID_FLAG){
        pid_exit_check();
    }
}
#endif

//-Orignal MBED PID--------------------------------------------------*/
#ifdef MBED_PID_1
void pid_set_motors(int power_diff)
{
    short int left_speed  = 0;
    short int right_speed = 0;


    if( power_diff > MAX_SPEED) 
        power_diff = MAX_SPEED;
    if( power_diff < -MAX_SPEED) 
        power_diff = -MAX_SPEED;

        if(power_diff < 0) {
            left_speed  = (short int)(MIN_SPEED + MAX_SPEED +power_diff);
            right_speed = (short int)(MIN_SPEED + MAX_SPEED);
        }

        else {
            left_speed  = (short int)(MIN_SPEED + MAX_SPEED);
            right_speed = (short int)(MIN_SPEED + MAX_SPEED -power_diff);
        }


        motor_set(MOTOR_LEFT,left_speed);
        motor_set(MOTOR_RIGHT,right_speed);

        #ifdef DEBUG
        dbg_printf("Left Motor:%d   ",left_speed);
        dbg_printf("Right Motor:%d\n\r",right_speed);
        #endif
}

void pid_control_loop()
{
    #ifdef DEBUG
    dbg_printf("---pid control loop v1--\n\r");
    #endif

    PID_RUNNING = 1;
    int last_proportional=0;
    long integral=0;
    sensor_pattern = 0;
    
    while(1)
    {
        if(!PID_FLAG) return;

        unsigned int position = line_read_position();
        while (position > 4000) position = line_read_position();
        int proportional = ((int)position) - 2000;
        int derivative = proportional - last_proportional;

        #ifdef DEBUG
        dbg_printf("last_proportional  %d\n\r",last_proportional);
        #endif

        last_proportional = proportional;
        integral += proportional;
        int power_difference = proportional/Kp_l + integral/Ki_l + derivative/Kd_l;

        #ifdef DEBUG
        dbg_printf("position           %d\n\r",position);
        dbg_printf("proportional       %d\n\r", proportional);
        dbg_printf("integral           %d\n\r",integral);
        dbg_printf("derivative         %d\n\r",derivative);
        dbg_printf("power_difference   %d\n\r",power_difference);
        dbg_printf("last_snr_pattern   %c\n\r",sensor_pattern);
        #endif

        pid_set_motors(power_difference);
        pid_exit_check();
    }
}
#endif


//-PID from Advanced movment-----------------------------------------*/
#ifdef MBED_PID_2
int normalized_position()
{
    unsigned int position = line_read_position();
    while (position > 4000) 
        position = line_read_position();
    return ((int)position) - 2000;
}


void pid_control_loop()
{
    #ifdef DEBUG
    dbg_printf("---pid control loop v2--\n\r");
    #endif

    PID_RUNNING     = 1;
    sensor_pattern  = 0;

    int last_p = 0;
    int p      = 0;
    int i      = 0;
    int d      = 0;

    while(1){
        if(!PID_FLAG) 
            return;
            
    p = normalized_position();
    adv_pid_motor_set(adv_pid_calc(p, &last_p, &i, &d));
    pid_exit_check();
    }    
}
#endif

void pid_run()
{
    #ifdef DEBUG
    dbg_printf("---pid run---\n\r");
    #endif

    if (PID_RUNNING || !PID_FLAG)
        return;
    else 
        pid_control_loop();
}

