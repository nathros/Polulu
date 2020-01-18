#include <stdio.h>
#include <stdint.h>    

#define Kp              45
#define Ki              10000
#define Kd              500
#define PID_FACTOR      22.222
#define SPEED_LIMIT     30
#define SPEED_OFFSET    0


int g_pid_angle = 0;
int g_target_angle = 0;
int normalised_angle = 0;

void coordinate_angle_set(int n)
{
    /* defensive programming, avoid crazy input */
    if (n < 0 || n > 360) {
        n = 0;
    }
    g_target_angle = n - 180;
}



void adv_pid_motor_set(int power_diff)
{
    short int l_speed  = 0;
    short int r_speed = 0;

    if( power_diff > SPEED_LIMIT ) {
        power_diff = SPEED_LIMIT;
    }
    if( power_diff < -SPEED_LIMIT ) {
        power_diff = -SPEED_LIMIT;
    }

    if(power_diff < 0) {
        l_speed  = (short int)(SPEED_OFFSET + SPEED_LIMIT +power_diff);
        r_speed = (short int)(SPEED_OFFSET + SPEED_LIMIT);
    } else {
        l_speed  = (short int)(SPEED_OFFSET + SPEED_LIMIT);
        r_speed = (short int)(SPEED_OFFSET + SPEED_LIMIT -power_diff);
    }

    printf("\n\r");
    printf("Set Motors: L_Motor = %d,  R_Motor = %d \n\r" ,l_speed ,r_speed);

    printf("---------------------------------------------- \n\r");

}




int adv_pid_calc(int prop_current, int *prop_last, int *integral, int *derivative)
{
    printf("last_proportional  %d\n\r", *prop_last);
        
    *derivative = prop_current - *prop_last;
    *prop_last = prop_current;
    *integral += prop_current;
    int power_difference = (prop_current/Kp + *integral/Ki  + (*derivative *3/Kd));   

    printf("proportional       %d\n\r", prop_current);
    printf("integral           %d\n\r",*integral);
    printf("derivative         %d\n\r",*derivative);
    printf("power_difference   %d\n\r", power_difference);
    
    return power_difference;
}




adv_pid_move(int target_ang)
{
     
    
    
    int last_p      = 0;
    int i           = 0;
    int d           = 0;
    int power_diff  = 0;
    float travelled = 0;
    
        
    coordinate_angle_set(target_ang);
    power_diff = adv_pid_calc(normalised_angle, &last_p, &i, &d); 
    adv_pid_motor_set(power_diff);
        
} 


void main()
{
            

    for(g_pid_angle=-90; g_pid_angle < 90; g_pid_angle += 1){
        normalised_angle = (g_pid_angle - 180 - g_target_angle) * PID_FACTOR;
        printf("Target Ang:%d    Current Ang:%d\n\r" ,g_target_angle +180 ,g_pid_angle);
        adv_pid_move(0);
    }
}
