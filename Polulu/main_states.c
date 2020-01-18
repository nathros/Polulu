/**
 * @file main.c
 * @author Oliver Loftus (Initial version)
 * @author Sam Gamble (Major rewrite)
 * @author Fufu Fang (Review and tidying)
 * @note
 *  =================== MAIN Operating modes ===================
 * - 0: (MAIN) waitting: p21 button -> mode = 1
 *      (RIT)  -nothing to do yet-
 * - 1: (MAIN) fwd (x dist), stright or curve
 *      (RIT) looking for Start wall || Line Start: found -> mode = 2
 * - 2: (MAIN) follow wall (LEFT)
 *      (RIT) looking for   End Wall || Line Start: found -> mode = 3
 * - 3: (MAIN) move to coord(x y)
 *      (RIT) looking for Start wall || Line Start: found -> mode = 4
 * - 4: (MAIN) follow wall (RIGHT)
 *      (RIT) looking for   End Wall || Line Start: found -> mode = 5
 * - 5: (MAIN) fwd (x dist), stright
 *      (RIT) looking for Line Start:               found -> mode = 6
 * - 6: (MAIN) Start PID
 *      (RIT) looking for End of corse              found -> mode = 0
 * - x: (MAIN) Error Correction mode {just an idea}
 *      if a mode dosn't meet exit cond take corrective action based on current
 *      mode num and guessed position
 */

#include <lpc17xx_rit.h>

#include "motor.h"
#include "ir.h"
#include "uart.h"
#include "debug.h"
#include "wall_follow.h"
//#include "line_search.h"
#include "config.h"
#include "gpio.h"
#include "pid.h"
#include "advanced_movement.h"
#include "coordinate.h"
#include "rit.h"
#include "line_sensors.h"
#include "utilities.h"
#include "delay.h"

#define INITAL_MODE           0
#define NUM_OF_MODES          6
#define EXIT                  2
#define IR_THRESH_START       250   //start if below 30cm
#define IR_THRESH_STOP        300   //stop if above 35cm
#define IR_AVG_CNT            10    //num over times to sample ir sensors

#define LINE_RVS_CNT          5
#define WALL_TO_FOLLOW  WALL_RIGHT

//#define DEBUG

short int OPMODE = 0;
short int LINE_FOUND_FLAG = 0;
short int MODE_RUNNING_FLAG = 0;    //this ensures each case runs ONCE per mode switch
uint16_t ln_sens[5];

//=================== RIT Functions ===================
//=====================================================

/**
 * @brief Set OPMODE to the next sequental value
 * @details 0..6 -> cycle back around to 0 .. etc
 *
 */
void set_next_mode(short int current_mode)
{
    if(OPMODE == current_mode){
        OPMODE = (OPMODE+1) % (NUM_OF_MODES+1);
        MODE_RUNNING_FLAG = 0;

        play_music("!O6 T70 L6 V10 MS a8");

        #ifdef DEBUG
        dbg_printf("Set OPMODE = %d \n\r",OPMODE);
        #endif
    }
}

/**
 * @brief If Line is detected, advance to next mode
 * @details  'current_mode' is a safe guard
 * to ensure OPMODE is incremented only ONCE
 *
 */
short int rit_detect_line()
{
    #ifdef DEBUG
    dbg_printf("-Detect Line-\n\r");
    #endif

    short int i = 0;
    short int line_detect = 0;

    for(i=0; i<4; i++ ){
       line_detect += pid_line_detect();
    }
    if(LINE_FOUND_FLAG && (line_detect == 4)) {
        return 0;
    }
    if(LINE_FOUND_FLAG && (line_detect < 2)) {
        LINE_FOUND_FLAG = 0;
        return 0;
    }

    if(line_detect == 4) {
        LINE_FOUND_FLAG = 1;
        return 1;
    }
    return 0;
}


/**
 * @brief
 * @details
 *
 */
short int rit_detect_dock()
{
    line_calibrate_read(ln_sens);
    if((ln_sens[0] > 700 && ln_sens[3] > 700)
     ||(ln_sens[1] > 700 && ln_sens[4] > 700)){

        return 1;
    }
    return 0;
}

/**
 * @brief
 * @details
 *
 */
short int rit_detect_wall_start(ir_Sensor selected_ir_sensor)
{
    #ifdef DEBUG
    dbg_printf("-detect wall start-\n\r");
    #endif

    int i,wall_debounce = 0;

  

    for(i=0; i < 4; i++){
        #ifdef DEBUG
        dbg_printf("IR Sensor = %d\n\r",ir_read(selected_ir_sensor));
        #endif
        if (ir_read(selected_ir_sensor) < IR_THRESH_START){
            wall_debounce++;
        }
    }

    #ifdef DEBUG
    dbg_printf("wall_debounce = %d\n\r", wall_debounce);
    #endif

    if(wall_debounce > 3) {
        return 1;
    }
    return 0;

    /*
    for(i=0; i < IR_AVG_CNT; i++){
        #ifdef DEBUG
        dbg_printf("IR Sensor = %d\n\r",ir_read(selected_ir_sensor));
        #endif
        wall_debounce += ir_read(selected_ir_sensor);
    }

    wall_debounce = wall_debounce/IR_AVG_CNT;
     if(wall_debounce > IR_THRESH_START) {
        return 1;
    }
    return 0;
  */
}

/**
 * @brief
 * @details
 *
 */
short int rit_detect_wall_end(ir_Sensor selected_ir_sensor)
{
    #ifdef DEBUG
    dbg_printf("-detect wall end-\n\r");
    #endif

    int i,wall_debounce = 0;


  
    for(i=0;i<4;i++) {
        #ifdef DEBUG
        dbg_printf("IR Sensor = %d\n\r",ir_read(selected_ir_sensor));
        #endif

        if (ir_read(selected_ir_sensor) >  IR_THRESH_STOP) {
            wall_debounce++;
        }
    }

    #ifdef DEBUG
    dbg_printf("wall_debounce = %d\n\r",wall_debounce);
    #endif

    if (wall_debounce > 3) {
        return 1;
    }
    return 0;
    
      /*
    


    for(i=0; i < IR_AVG_CNT; i++){
        #ifdef DEBUG
        dbg_printf("IR Sensor = %d\n\r",ir_read(selected_ir_sensor));
        #endif
        wall_debounce += ir_read(selected_ir_sensor);
    }

    wall_debounce = wall_debounce/IR_AVG_CNT;
     if(wall_debounce < IR_THRESH_STOP) {
        return 1;
    }
    return 0;
    */
}

/**
 * @brief on detect obj go into trap state
 * @details   Save current motor values, wait in trap loop,
 * on obj removed restore motor values
 */
void rit_detect_collision()
{
    #ifdef DEBUG
    dbg_printf("-detect collision-\n\r");
    #endif


    if(gpio_collision_read()) {
        short int l_motor_speed = motor_read(MOTOR_LEFT);
        short int r_motor_speed = motor_read(MOTOR_RIGHT);

        motors_stop();
        while(gpio_collision_read());

        motor_set(MOTOR_LEFT, l_motor_speed);
        motor_set(MOTOR_RIGHT, r_motor_speed);
    }
}

/**
 * @brief  RIT Intrupt hander
 * @details used to poll sensors and switch to the next operating mode depending on values
 */
void RIT_IRQHandler(void)
{
    #ifdef DEBUG
    dbg_printf("-poll sensors-  OPMODE = %d\n\r",OPMODE);
    #endif

    RIT_GetIntStatus(LPC_RIT);
    rit_detect_collision();

    switch (OPMODE)
    {

//-do nothing--------------------------------------------------------*/
        case 0:
            //
            break;

//-detecting wall(left) ---------------------------------------------*/
        case 1:
            if((rit_detect_wall_start(BACK_LEFT)
             && rit_detect_wall_start(FRONT_LEFT))
           /*  || rit_detect_line() */ ) {

                #ifdef DEBUG
                dbg_printf("FLAG - start wall_follow\n\r");
                #endif
                adv_exit();
                follow_wall_start();
                set_next_mode(1);
            }
            break;

//-detecting wall end OR line----------------------------------------*/
        case 2:
            if( rit_detect_wall_end(BACK_LEFT)
             || rit_detect_wall_end(FRONT_LEFT)
             || rit_detect_line()) {

                #ifdef DEBUG
                dbg_printf("FLAG - stop wall_follow\n\r");
                #endif
                follow_wall_stop();

                if(EXIT == 1){
                    OPMODE = 5;
                } else {
                    set_next_mode(2);
                }
            }
            break;

//-detecting wall start OR line--------------------------------------*/
        case 3:
            if((rit_detect_wall_start(BACK_RIGHT)
             && rit_detect_wall_start(FRONT_RIGHT))
             || rit_detect_line()  ) {

                #ifdef DEBUG
                dbg_printf("FLAG - start wall_follow\n\r");
                #endif
                adv_exit();
                follow_wall_start();
                set_next_mode(3);
            }
            break;

//-detecting wall end OR line----------------------------------------*/
        case 4:
            if( rit_detect_wall_end(FRONT_RIGHT)
             || rit_detect_line()) {

                #ifdef DEBUG
                dbg_printf("FLAG - stop wall_follow\n\r");
                #endif
                follow_wall_stop();
                set_next_mode(4);
            }
            break;

//-detecting line----------------------------------------------------*/
        case 5:
            if(rit_detect_line()) {
                #ifdef DEBUG
                dbg_printf("FLAG - start pid\n\r");
                #endif
                adv_exit();
                pid_set();
                set_next_mode(5);
            }
            break;

//-detecting dock----------------------------------------------------*/
        case 6:
            if(rit_detect_dock()) {
                #ifdef DEBUG
                dbg_printf("FLAG - stop pid\n\r");
                #endif
                pid_clear();
                set_next_mode(6);
            }
            break;

//-------------------------------------------------------------------*/
        default:
            #ifdef DEBUG
            dbg_printf("ERROR: OPMODE out of bounds!\n\r");
            #endif
            break;
    }
}

//=================== MAIN Functions ==================
//=====================================================
/*NOTE! dont forget
 *
 * 1, need to flags for exit conds
 * 2, preamble needed when entering a new mode,
 * e.g. motor_stop() ... and such
 */

void init()
{
    debug_frmwrk_init();
    uart_init();
    ir_init();
    gpio_init();
    gpio_int_enable();
    mouse_setup();
    line_autocalibrate();
    RIT_Start(400);
}

/**
 * @brief Wait until GPIO button press
 * @details Loop until button detected, when detected advance to next mode
 */
void main_hold()
{
    while(!gpio_button_read());
}

void main_initial_mode()
{
    main_hold();
    coordinate_clear();
    set_next_mode(0);
    //OPMODE = 5;
}

void main_fwd_move(float distance, int angle_dir)
{
    if(MODE_RUNNING_FLAG) {
        return;
    }
    MODE_RUNNING_FLAG = 1;
    
    adv_move_polar(distance, angle_dir);


   // adv_move(target_pos);

    //Take action if sensors don't trigger next mode, possbly a Seach() func
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


void main()
{
    init();
    OPMODE = INITAL_MODE;

    while(1)
    {
        #ifdef DEBUG
        dbg_printf("-Enter MAIN loop-  OPMODE = %d\n\r",OPMODE);
        #endif


        switch (OPMODE)
        {
//-Holding State-----------------------------------------------------*/
            case 0:
                main_initial_mode();
                break;

//-Fwd move 220cm----------------------------------------------------*/
            case 1:
                #ifdef DEBUG
                dbg_printf("RUN: Case 1\n\r");
                #endif
                main_fwd_move(220,0);

                break;

//-Wall follow-------------------------------------------------------*/
            case 2:
                #ifdef DEBUG
                dbg_printf("RUN: Case 2\n\r");
                #endif
                main_follow_wall(WALL_LEFT);
                break;

//-Mov (x,y)---------------------------------------------------------*/
            case 3:
                #ifdef DEBUG
                dbg_printf("RUN: Case 3\n\r");
                #endif
                main_fwd_move(120,90);
                main_fwd_move(220,0);
                break;

//-Wall Follow-------------------------------------------------------*/
            case 4:
                #ifdef DEBUG
                dbg_printf("RUN: Case 4\n\r");
                #endif
                main_follow_wall(WALL_RIGHT);
                break;

//-Fwd move 55cm-----------------------------------------------------*/
            case 5:
                #ifdef DEBUG
                dbg_printf("RUN: Case 5\n\r");
                #endif
                main_fwd_move(55,0);
                break;

//-Line Follow-------------------------------------------------------*/
            case 6:
                #ifdef DEBUG
                dbg_printf("RUN: Case 6\n\r");
                #endif
                main_line_follow();
                break;

//-------------------------------------------------------------------*/
            default:
                #ifdef DEBUG
                dbg_printf("ERROR: OPMODE out of bounds!\n\r");
                #endif
                break;

        }
    }
}
