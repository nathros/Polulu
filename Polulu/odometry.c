/**
 * @file odometry.c
 * @brief Wheel encoder based odometry module (source)
 * @author Fufu Fang (Personal part of the project)
 */

#include <lpc17xx_timer.h>

#include "advanced_movement.h"
#include "common.h"
#include "coordinate.h"
#include "debug.h"
#include "led.h"
#include "motor.h"

#include "odometry.h"

#define WHEEL_BASE 8.7

/* How many clicks trigger a position calculation? */
#define TRIGGER 2

/*
 * Wheel diameter is 3.2cm, with 6 stripes,
 * 1/(3.2*3.14/6) = 0.597
 * with 12 stripes,
 * 1/(3.2*3.14/12) = 1.194
 */
#define L_PER_CM 1.194
#define R_PER_CM 1.194

#define CLICK_PER_WHEEL 12

/* Sampling rate for RPM calculation (in second) */
#define SAMPLE_RATE 0.5

static int g_left_count = 0;
static int g_right_count = 0;
static int g_total_count = 0;

static int g_left_rpm_count = 0;
static int g_right_rpm_count = 0;

static RPM g_rpm = {0, 0};

/**
 * @brief submit wheel encoder's data
 * @details calculate the distance moved and the angle changed, call
 * coordinate_update with the data.
 */
static void encoder_submit()
{
    float left_cm = (float)g_left_count / L_PER_CM;
    float right_cm = (float)g_right_count / R_PER_CM;
    Polar update;
    update.r = (left_cm + right_cm) /2;
    update.t = (left_cm - right_cm) / WHEEL_BASE;
    coordinate_update(update);
    #ifdef ODOMETRY_DBG
        dbg_printf("Odometry.c:\tleft_cm: %f\tright_cm: %f\tdistance: %f\tangle: %f\r\n",
                   left_cm, right_cm, update.r, toDegree(update.t));
    #endif
}

static void encoder_rpm_calculate()
{
    g_rpm.left = (float)g_left_rpm_count/CLICK_PER_WHEEL/SAMPLE_RATE*60;
    g_rpm.right = (float)g_right_rpm_count/CLICK_PER_WHEEL/SAMPLE_RATE*60;
    #ifdef RPM_DBG
        dbg_printf("Odometry.c:\tLeft RPM: %f\tRight RPM: %f\r\n",
                   g_rpm.left, g_rpm.right);
    #endif
}

/**
 * @brief clear encoder rpm click count
 */
static void encoder_rpm_clear()
{
    g_left_rpm_count = 0;
    g_right_rpm_count = 0;
    #ifdef ODOMETRY_DBG
        dbg_printf("Odometry.c:\tRPM counters cleared!\r\n");
    #endif
}

/**
 * @brief clear encoder distance click count
 */
void encoder_clear()
{
    g_left_count = 0;
    g_right_count = 0;
    g_total_count = 0;
    #ifdef ODOMETRY_DBG
        dbg_printf("Odometry.c:\tCounters cleared!\r\n");
    #endif
}

void encoder_update(Encoder encoder)
{
    int motor_speed;
    g_total_count++;
    if (encoder == LEFT_ENCODER) {
        motor_speed = motor_read(MOTOR_LEFT);
        if (motor_speed > 0) {
            g_left_count++;
            g_left_rpm_count++;
        } else {
            g_left_count--;
            g_left_rpm_count--;
        }
    } else {
        motor_speed = motor_read(MOTOR_RIGHT);
        if (motor_speed > 0) {
            g_right_count++;
            g_right_rpm_count++;
        } else {
            g_right_count--;
            g_right_rpm_count--;
        }
    }
    if (g_total_count >= TRIGGER) {
        encoder_submit();
        encoder_clear();
    }

    #ifdef UPDATE_DBG
        dbg_printf("Odometry.c:\tleft_count: %d\tright_count: %d\ttotal_count: %d\r\n",
                   g_left_count, g_right_count, g_total_count);
    #endif
}

/**
 * @details This function essentially initialise the timer and the
 * associated interrupt settings.
 */
void encoder_rpm_init()
{
    TIM_TIMERCFG_Type timer_config;
    TIM_MATCHCFG_Type match_config;

    /* The timer is configured to tick once every 1000 microseconds (1ms) */
    timer_config.PrescaleOption = TIM_PRESCALE_USVAL;
    timer_config.PrescaleValue = 1000;

    /* Interrupt is triggered once every 100 ticks (500ms or 0.5s) */
    match_config.MatchValue = 500;

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

    /* Enable interrupt for timer 3 */
    NVIC_EnableIRQ(TIMER3_IRQn);

    /* Start timer 3 */
    TIM_Cmd(LPC_TIM3, ENABLE);
}

/**
 * @details The interrupt service routine changes the time record,
 * based on the execution mode.
 */
void TIMER3_IRQHandler()
{
    led_on(LED_3);

    encoder_rpm_calculate();
    adv_rpm_update(g_rpm);
    encoder_rpm_clear();

    led_off(LED_3);
    TIM_ClearIntPending(LPC_TIM3, TIM_MR3_INT);
}

float encoder_rpm(Encoder encoder)
{
    if (encoder == LEFT_ENCODER) {
        return g_rpm.left;
    } else {
        return g_rpm.right;
    }
}

void encoder_rpm_off()
{
    TIM_DeInit(LPC_TIM3);
}
