/**
 * @file mouse.c
 * @brief mouse module source code
 * @details This module handles everything that's got to do with the mouse.
 * @note Correction factors for the mouse:
 * - SHIFT_T corrects for the bias in rotation,
 * - SHIFT_L corrects for the bias in distance,
 * - T_PER_DEGREE is the T reading from the mouse when the vehicle rotate about its
 * centre for 180 degrees
 * - L_PER_CM is the distance the R reading from the mouse when the vehicle moves
 * in a straight line for 1 meter.
 * @author Fufu Fang (wrote the whole thing)
 * @author Nathan Bartram  (for trying but failing to link our binary to Jamie's library)
 * @author Sam Gamble (for trying to but failing to link our binary to Jamie's library)
 */
#include <math.h>

#include <lpc17xx_timer.h>

#include "KeyboardHost.h"

#include "config.h"

#include "common.h"
#include "lcd.h"
#include "led.h"
#include "debug.h"
#include "coordinate.h"

#include "mouse.h"

/* Mouse related definition */
#define SHIFT_L -0.466
#define SHIFT_T -14.1
#define T_PER_DEGREE 45
#define L_PER_CM 395

#ifdef MOUSE_DEBUG
static Mouse_dbg dbg_record = {0, 0, 0, 0, 0};

Mouse_dbg mouse_dbg_dump()
{
    return dbg_record;
}

void mouse_dbg_clear()
{
    dbg_record.raw_l = 0;
    dbg_record.raw_t = 0;
    dbg_record.cal_l = 0;
    dbg_record.cal_t = 0;
    dbg_record.count = 0;
}
#endif

/**
 * @brief Private function: set up the mouse timer.
 * @details This function essentially initialise the timer and the
 * associated interrupt settings.
 * @bug The interrupt priority is currently the same as timer 3!!!
 */
static void mouse_timer_init()
{
    TIM_TIMERCFG_Type timer_config;
    TIM_MATCHCFG_Type match_config;

    /* The timer is configured to tick once every 1000 microseconds (1ms) */
    timer_config.PrescaleOption = TIM_PRESCALE_USVAL;
    timer_config.PrescaleValue = 500;

    /* Interrupt is triggered once every 1 ticks (0.5ms) */
    match_config.MatchValue = 1;

    /* Using timer 2 */
    match_config.MatchChannel = 2;
    match_config.IntOnMatch = TRUE;
    match_config.ResetOnMatch = TRUE;
    match_config.StopOnMatch = FALSE;
    match_config.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

    /* Initialise timer */
    TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &timer_config);

    /* Configure interrupt triggering */
    TIM_ConfigMatch(LPC_TIM2, &match_config);

    /* Set interrupt priority, preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(TIMER2_IRQn, ((0x01 << 3) | 0x01));

    /* Enable interrupt for timer 2 */
    NVIC_EnableIRQ(TIMER2_IRQn);

    /* Start timer 2 */
    TIM_Cmd(LPC_TIM2, ENABLE);
}

/**
 * @details This interrupt service routine polls the mouse.
 */
void TIMER2_IRQHandler(void)
{
    led_on(LED_2);

    mouse_poll();

    led_off(LED_2);
    TIM_ClearIntPending(LPC_TIM2, TIM_MR2_INT);
}

/**
 * @details The mouse always move forwards in one axis, and
 * rotate on another axis, hence it works in polar coordinate.
 */
void mouse_callback(uint8_t button, int8_t l, int8_t t)
{
    (void) button;

    /* All correction factors are in x_per_reading format. */
    float tempt_t = ((float)t - 1/SHIFT_T) / T_PER_DEGREE;
    float d_l = ((float)l - 1/SHIFT_L) / L_PER_CM;

#ifdef MOUSE_DEBUG
    /* populating debugging record */
    dbg_record.raw_l += l;
    dbg_record.raw_t += t;
    dbg_record.cal_l += d_l;
    dbg_record.cal_t += tempt_t;
    dbg_record.count += 1;
//     dbg_printf("Mouse.c:  ");
//     dbg_print_mouse(dbg_record);
#endif

    float d_t = toRadian(tempt_t);
    Polar update;
    update.r = d_l;
    /*Clockwise rotation should have a positive value */
    update.t = -d_t;

    coordinate_update(update);
}

void mouse_attach()
{
    dbg_printf("Mouse attached\r\n");
}

void mouse_detach()
{
    dbg_printf("Mouse detached\r\n");
}

void mouse_setup()
{
    #ifdef WHEEL_ENCODER
        dbg_printf("The robot is set to use wheel encoder, ");
        dbg_printf("you can disable it in config.h.");
        dbg_printf("The mouse is NOT configured!\r\n");
        return;
    #endif
    mouse_init(mouse_callback, mouse_attach, mouse_detach);
    mouse_timer_init();
}
