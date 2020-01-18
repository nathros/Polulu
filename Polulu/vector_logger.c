/**
 * @file vector_logger.c
 * @brief Vector logger module
 * @author Fufu Fang (wrote the whole thing)
 * @warning obsolete code!
 * @details This module record the vector of the robot, including
 * the direction, speed and time.
 */
#include <lpc17xx_timer.h>

#include "debug.h"
#include "motor.h"
#include "led.h"

#include "vector_logger.h"

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
static Vector_Record record[LOG_SIZE];

/**
 * @brief Pop a record from the ring buffer
 * @details if there is no valid record left, halt the motor for
 * at least 1 second.
 */
Vector_Record vector_logger_pop()
{
    if (valid_record > 0) {
        Vector_Record i = record[index];

        index--;
        if (index < 1) {
            index = LOG_SIZE - 1;
        }
        valid_record--;

        dbg_printf("Popped, speed %d, angle: %d, time %d, index %d, valid_record %d\r\n", i.speed, i.angle, i.time, index, valid_record);
        return i;
    } else {
        Vector_Record i = { 0, 0, 1000 };
        dbg_printf("WARNING: Buffer underflow\r\n");
        return i;
    }
}

/**
 * @details add record into the stack, maintain stack index, and
 * the valid record count.
 */
void vector_logger_push()
{
    index++;
    if (index > (LOG_SIZE - 1)) {
        index = 1;
    }

    record[index].speed = motors_speed();
    record[index].angle = motor_angle();
    record[index].time = 0 ;

    if (valid_record < LOG_SIZE) {
        valid_record++;
    }
    dbg_printf("Pushed, speed %d, angle: %d, index %d, valid_record %d\r\n", record[index].speed, record[index].angle, index, valid_record);
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
void vector_logger_init()
{
    TIM_TIMERCFG_Type timer_config;
    TIM_MATCHCFG_Type match_config;

    /* The timer is configured to tick once every 1000 microseconds (1ms) */
    timer_config.PrescaleOption = TIM_PRESCALE_USVAL;
    timer_config.PrescaleValue = 1000;

    /* Interrupt is triggered once every 1 ticks (1ms or 0.001s) */
    match_config.MatchValue = 1;

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

void vector_logger_playback()
{
    global_mode = REPLAYING;
    dbg_printf("Before reversion, Current vehicle angle is: %d\n\r", motor_angle());
    motor_rotate(180);
    dbg_printf("After reversion, current vehicle angle is: %d\n\r", motor_angle());

    while (valid_record > 0) {
        Vector_Record i = vector_logger_pop();
//         dbg_printf("Logger: rotating to %d \n\r", i.angle);
        motor_rotate(i.angle+180-motor_angle());
        dbg_printf("Current vehicle angle is: %d\n\r", motor_angle());
        /* Note, timer only starts when you have finished rotation */
        global_time = i.time;
        motors_move(i.speed);
        while (global_time) {
            led_on(LED_3);
        }
        led_off(LED_3);
        motors_stop();
    }
    global_mode = RECORDING;
}

void vector_logger_clear()
{
    index = 0;
    valid_record = 0;
}

void vector_logger_off()
{
    TIM_DeInit(LPC_TIM3);
}
