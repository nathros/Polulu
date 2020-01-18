/**
 * @file motor_logger.c
 * @brief Motor motion logger source
 * @author Fufu Fang (wrote the whole thing)
 * @author Sam Gamble (advice, inspiration and moral support)
 * @warning Obsolete code!
 * @details The function in this file should allow the robot to
 * trace its path backward to where it started.
 */
#include <lpc17xx_timer.h>

#include "debug.h"
#include "motor.h"
#include "led.h"

#include "motor_logger.h"

/**
 * @brief The size of the ring buffer
 * @details The actual log size = LOG_SIZE -1,
 * record[0] is never used.
 */
#define LOG_SIZE 500

/**
 * @brief Execution modes of the logging module
 */
typedef enum { RECORDING, REPLAYING } EXECUTION_MODE;

/* Time value for the current action */
static short global_time = 0;

/* Ring buffer index */
static short index = 0;

/* The number of valid record*/
static short valid_record = 0;

/* current execution mode */
static EXECUTION_MODE global_mode = RECORDING;

/* The ring buffer itself */
static Motor_Record record[LOG_SIZE];

/**
 * @brief Pop a record from the ring buffer
 * @details if there is no valid record left, halt the motor for
 * at least 1 second.
 */
Motor_Record motor_logger_pop()
{
    if (valid_record > 0) {
        Motor_Record i = record[index];

        index--;
        if (index < 1) {
            index = LOG_SIZE - 1;
        }
        valid_record--;

//         dbg_printf("Popped, left: %d, right %d, time %d, index %d, valid_record %d\r\n", i.left_speed, i.right_speed, i.time, index, valid_record);
        return i;
    } else {
        Motor_Record i = { 0, 0, 1000 };
        dbg_printf("WARNING: Buffer underflow\r\n");
        return i;
    }
}

/**
 * @details add record into the stack, maintain stack index, and
 * the valid record count.
 */
void motor_logger_push()
{
    index++;
    if (index > (LOG_SIZE - 1)) {
        index = 1;
    }

    record[index].left_speed = motor_read(MOTOR_LEFT);
    record[index].right_speed = motor_read(MOTOR_RIGHT);
    record[index].time = 0;

    if (valid_record < LOG_SIZE) {
        valid_record++;
    }
//     dbg_printf("Pushed, left: %d, right %d, index %d, valid_record %d\r\n", i.left_speed, i.right_speed, index, valid_record);
}


/**
 * @details The interrupt service routine changes the time record,
 * based on the execution mode.
 */
void TIMER3_IRQHandler(void)
{
    led_on(LED_3);
    if (global_mode == RECORDING) {
        /* Change the global_time in current record */
        record[index].time++;
    } else {
        /* Change the global time variable */
        global_time--;
    }
    led_off(LED_3);
    TIM_ClearIntPending(LPC_TIM3, TIM_MR3_INT);
}

/**
 * @details This function essentially initialise the timer and the
 * associated interrupt settings.
 */
void motor_logger_init()
{
    TIM_TIMERCFG_Type timer_config;
    TIM_MATCHCFG_Type match_config;

    /* The timer is configured to tick once every 1000 microseconds (1ms) */
    timer_config.PrescaleOption = TIM_PRESCALE_USVAL;
    timer_config.PrescaleValue = 1000;

    /* Interrupt is triggered once every 10 ticks (10ms or 0.01s) */
    match_config.MatchValue = 10;

    /* Using timer 3 */
    match_config.MatchChannel = 3;
    match_config.IntOnMatch = TRUE;
    match_config.ResetOnMatch = TRUE;
    match_config.StopOnMatch = FALSE;
    match_config.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

    /* Initialise timer */
    TIM_Init(LPC_TIM3, TIM_TIMER_MODE, &timer_config);

    /* Configure interrupt triggering */
    TIM_ConfigMatch(LPC_TIM3, &match_config);

    /* Set interrupt priority, preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(TIMER3_IRQn, ((0x01 << 3) | 0x01));

    /* Enable interrupt for timer 3 */
    NVIC_EnableIRQ(TIMER3_IRQn);

    /* Start timer 3 */
    TIM_Cmd(LPC_TIM3, ENABLE);

    index = 0;
    valid_record = 0;
}

void motor_logger_playback()
{
    global_mode = REPLAYING;
    while (valid_record > 0) {
        Motor_Record i = motor_logger_pop();
        motor_set(MOTOR_LEFT, -i.left_speed);
        motor_set(MOTOR_RIGHT, -i.right_speed);
        global_time = i.time;
        while (global_time) {
            led_on(LED_3);
        }
        led_off(LED_3);
    }
    global_mode = RECORDING;
}

void motor_logger_clear()
{
    index = 0;
    valid_record = 0;
}

void motor_logger_off()
{
    TIM_DeInit(LPC_TIM3);
}
