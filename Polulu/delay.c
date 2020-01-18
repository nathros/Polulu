/**
 * @file delay.c
 * @brief Timer functions for causing delay (source)
 * @author Sam Gamble
 * @author Fufu Fang (tidying up)
 */
#include <lpc17xx_timer.h>
#include <lpc17xx_pinsel.h>

#include "led.h"
#include "debug.h"

#include "delay.h"

static void delay_init(int milli_secs)
{

    TIM_TIMERCFG_Type Timer_Config;
    /* changes clock to use micro sec */
    Timer_Config.PrescaleOption = TIM_PRESCALE_USVAL;
    /* will send a pulse to Timer every 1 ms */
    Timer_Config.PrescaleValue = 1000;

    TIM_MATCHCFG_Type Match_Config;
    Match_Config.MatchChannel = 0;
    Match_Config.IntOnMatch = TRUE;
    Match_Config.ResetOnMatch = TRUE;
    Match_Config.StopOnMatch = TRUE;
    Match_Config.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
    /* every n milliseconds pulses the timer recives from the Prescaler an IRQ will gen */
    Match_Config.MatchValue = milli_secs;

    /* Set configuration for Tim_config and Tim_MatchConfig */
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &Timer_Config);
    TIM_ConfigMatch(LPC_TIM0, &Match_Config);
}


void delay_start(int delay_length)
{
    /**
     * crash avoidance -- if you ask the timer to wait
     * for 0, it will crash. (Added by Fufu)
     */
    if (delay_length == 0){
        dbg_printf("WARNING:You have just asked the timer to wait for 0 miliseconds.\r\n");
        return;
    }
    led_on(LED_0);
    delay_init(delay_length);
    /* Start timer */
    TIM_Cmd(LPC_TIM0, ENABLE);

    while (!TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT))
        ;
    TIM_ClearIntPending(LPC_TIM0, 0);
    TIM_DeInit(LPC_TIM0);
    led_off(LED_0);
}

void delay_music(int tempo, int beats_per_bar, int num_bars)
{
    dbg_printf("Calculating time delay --  Tempo: %d  BPB: %d  num bars: %d   \n\r", tempo, beats_per_bar, num_bars);
    float music_duration = ((60 / (float) tempo) * (float) beats_per_bar * (float) num_bars) * 1000.0;
    int delay_length = (int) music_duration;
    dbg_printf("Result (Milliseconds):  %d   \n\r", delay_length);
    delay_start(delay_length);
}
