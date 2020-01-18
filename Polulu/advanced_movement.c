/**
 * @file advanced_movement.c
 * @author Fufu Fang  (Non-PID movement, Review)
 * @author Sam Gamble (PID straight line version)
 * @brief Pololu advanced movement module
 * @details This file contains code to for moving Pololu to arbitrary
 * coordinates.
 */

#include <math.h>

#include "common.h"
#include "debug.h"
#include "motor.h"
#include "pid.h"

#include "advanced_movement.h"

#define ADV_MOV_DBG
#define ADV_PID_DBG

#define Kp_rpm      0.8
#define Kd_rpm      0
#define Ki_rpm      0
#define SCALE_RPM   0.1
#define RPM_MAX     35
#define RPM_MIN     25

static int g_pid_angle = 0;
static short int g_exit_flag = 0;
static RPM g_rpm = {0, 0};
int TARGET_RPM = 90;

void adv_rpm_update(RPM rpm)
{
    g_rpm.left = rpm.left;
    g_rpm.right = rpm.right;
}

void adv_angle_update(int n)
{
    g_pid_angle = n;
}

void adv_exit()
{
    g_exit_flag = 1;
}

void adv_travel(float distance)
{
    if (distance == 0) {
        return;
    }
    float travelled = 0;
    distance_clear();
    motors_move(SPEED_LIMIT);
    while(travelled <= distance && !g_exit_flag) {
        travelled = distance_read();
    }
    motors_stop();
}

void adv_move_polar(float distance, int bearing)
{
    motor_rot_abs(bearing);
    adv_travel(distance);
}

/**
 * @note For calculating the direction of travel, the angle reference
 * comes from coordinate.c.However the rotation angle record actually comes
 * from motor.c!
 */
void adv_move(Coordinate target)
{
    Coordinate current = coordinate_read();
    int dx = target.x - current.x;
    int dy = target.y - current.y;

    /* defensive programming, error avoidance) */
    if ( (dy == 0) && (dx == 0) ){
        return;
    }

    float distance = sqrt(dx*dx + dy*dy);
    float direction = atan2(dy, dx);
    #ifdef ADV_MOV_DBG
    dbg_printf("ADV_MOVE: Starting, Rotating to %f\r\n", toDegree(direction));
    #endif
    motor_rot_abs(toDegree(direction));
    adv_travel(distance);
    #ifdef ADV_MOV_DBG
    dbg_printf("ADV_MOVE: Finishing, Rotating to %f\r\n", toDegree(target.t));
    #endif
    motor_rot_abs(toDegree(target.t));
}

void adv_move_pid_polar(float distance, int bearing)
{
    if( (distance< 1) || (bearing < 0) || (bearing > 360) ){
        return;
    }

    int last_p      = 0;
    int i           = 0;
    int d           = 0;
    int power_diff  = 0;
    float travelled = 0;
    g_exit_flag   = 0;

    coordinate_angle_set(bearing);
    motor_rot_abs(bearing);

    distance_clear();
    while(travelled <= distance && !g_exit_flag){
        travelled = distance_read();

        power_diff = pid_calc(g_pid_angle, &last_p, &i, &d);
        pid_motor_set(power_diff);
    }
    motors_stop();
}

void adv_move_pid(Coordinate target)
{
    Coordinate current = coordinate_read();
    int dx = target.x - current.x;
    int dy = target.y - current.y;

    /* defensive programming, error avoidance) */
    if ( (dy == 0) && (dx == 0) ){
        return;
    }

    float distance= sqrt(dx*dx + dy*dy);
    int bearing = toDegree(atan2(dy, dx));

    adv_move_pid_polar(distance, bearing);

    motor_rot_abs(toDegree(target.t));
}


/**
 * @note   Motor speed 30: Walked 50cm in 3000 ms  17cm/s
 * wheel diameter(3.2)*pi = circumference of 10.05cm = 1.7 revs per sec
 * = 102 RPM
 *
 * 30/100 (approx) = 0.3   so --> moter speed diff = 0.3 * (target_rpm - measured_rpm)
 *
 * Note: intergal part might not be needed (pending testing)
 *
 * tested with dt = 0.1 in gcc and got crazy high values (exlude for now)
 *
 = *=tuning the system==
 First set I and D to 0.
 Find a good P term that gives a quick response, but will have oscillations.
 Then find a D to reduce oscillations.
 stay away from the I term for your system. Keep I = 0, for now, and tell me how your systems looks.
 */




void adv_print_rmp()
{
    dbg_printf("l_rpm_measured: %f   l_rpm_measured: %f\r\n" ,g_rpm.left ,g_rpm.right);
}

void adv_motors_rpm(float distance)
{
        if (distance) TARGET_RPM = 90;
        else TARGET_RPM = -90;
        float travelled         = 0;
        float prv_error_left    = 0;
        float prv_error_right   = 0;
        float error_left        = 0;
        float error_right       = 0;
        float deriv_left        = 0;
        float deriv_right       = 0;
        float ingrl_left        = 0;
        float ingrl_right       = 0;
        float calc_l            = 0;
        float calc_r            = 0;
        int motor_spd_l         = 30;
        int motor_spd_r         = 30;
        
       // float  current_right;
        //float  current_left;
        g_exit_flag = 0;

        motors_move(30);
        
       // dbg_printf("set motors\r\n");


        //-Correcting for BOTH motors RPM-------------------------------------

        
        distance_clear();
        while(travelled <= distance){
            if(g_exit_flag) 
                return;
                
            dbg_printf("-pid loop-\r\n");
            travelled = distance_read();


            //Proportianl
            error_left      = TARGET_RPM - g_rpm.left;
            error_right     = TARGET_RPM - g_rpm.right;
            
             //Ignore crazy values
            if(g_rpm.left <= 0) error_left      = 0;
            if(g_rpm.right <= 0) error_right    = 0;

            //Derivative --not sure about dt div (wikipedia told me so, THERFORE IT MUST BE TRUE!)
            deriv_left      = (prv_error_left - error_left);     
            deriv_right     = (prv_error_right - error_right);  

            //Intergral
            ingrl_left      += error_left;   
            ingrl_right     += error_right; 

            //perform calc
            calc_l          = (error_left * Kp_rpm) + (ingrl_left * Ki_rpm) + (deriv_left * Kd_rpm);
            calc_r          = (error_right * Kp_rpm) + (ingrl_right * Ki_rpm) + (deriv_right * Kd_rpm);

            //read current motor values
            motor_spd_l     = motor_read(MOTOR_LEFT);
            motor_spd_r     = motor_read(MOTOR_RIGHT);
                
            //Calculate new Motor Values 
            motor_spd_l     = motor_spd_l + (int)(calc_l * SCALE_RPM);
            motor_spd_r     = motor_spd_r + (int)(calc_r * SCALE_RPM);
                
            //Cap values if they exced MIN/MAX threshold   
            if(motor_spd_r < RPM_MIN) 
                motor_spd_r = RPM_MIN;
            if(motor_spd_l < RPM_MIN)
                 motor_spd_l = RPM_MIN;

            if(motor_spd_r > RPM_MAX)
                motor_spd_r = RPM_MAX;
            if(motor_spd_l > RPM_MAX)
                motor_spd_l = RPM_MAX;
            
            
    
            #ifdef ADV_MOV_DBG
            dbg_printf("motor_spd_l: %d   motor_spd_r: %d\r\n" ,motor_spd_l ,motor_spd_r);
            #endif
            //Set new Motor Values 
            internal_motor_send(MOTOR_LEFT, motor_spd_l);
            internal_motor_send(MOTOR_RIGHT, motor_spd_r);

            //save current error
            prv_error_left  = error_left;
            prv_error_right = error_right;

            #ifdef ADV_MOV_DBG
            dbg_printf("l_rpm_measured: %f   l_rpm_error: %f\r\n" ,g_rpm.left ,error_left);
            dbg_printf("r_rpm_measured: %f   r_rpm_error: %f\r\n" ,g_rpm.right ,error_right);
            #endif


        }
   
        dbg_printf("end loop");
        motors_stop();
}
