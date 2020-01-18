/**
 * @brief Repetitive Interrupt Timer Controller(Source)
 * @author Sam Gamble
 */

#include <lpc17xx_rit.h>

#include "rit.h"

void RIT_Start(int time)
{
    RIT_Init(LPC_RIT);
    RIT_TimerConfig(LPC_RIT, time);
    NVIC_EnableIRQ(RIT_IRQn);
}


void RIT_Disable(void)
{
    NVIC_DisableIRQ(RIT_IRQn);
}

void RIT_Enable(void)
{
    NVIC_EnableIRQ(RIT_IRQn);
}
