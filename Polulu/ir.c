/**
 * @file ir.c
 * @author Fufu Fang (wrote all the ADC code, tided up)
 * @author Nathan Bartram (calibrated all the IR sensors)
 * @brief Robot IR receiver handler
 * @details
 * The naming convention for each ADC is similar to the ones in:
 * http://www-users.cs.york.ac.uk/~pcc/MCP/MbedPins.html,
 * where ADC0 is ADC0.0 and so on.
 */

#include <lpc17xx_pinsel.h>
#include <lpc17xx_adc.h>

#include "ir.h"

#define A 105
#define B 500
#define C 0.5
#define D 7

void ir_init()
{
    PINSEL_CFG_Type cfg;

    /* Some other things */
    cfg.OpenDrain = 0;
    cfg.Pinmode = 0;

    // ADC0:
    cfg.Portnum = 0;
    cfg.Pinnum = 23;
    cfg.Funcnum = 1;
    PINSEL_ConfigPin(&cfg);;
    // ADC1:
    cfg.Portnum = 0;
    cfg.Pinnum = 24;
    cfg.Funcnum = 1;
    PINSEL_ConfigPin(&cfg);;
    // ADC2:
    cfg.Portnum = 0;
    cfg.Pinnum = 25;
    cfg.Funcnum = 1;
    PINSEL_ConfigPin(&cfg);
    // ADC3:
    /* WARNING: Conflict with the DAC!!! */
    cfg.Portnum = 0;
    cfg.Pinnum = 26;
    cfg.Funcnum = 1;
    PINSEL_ConfigPin(&cfg);
    // ADC4:
    cfg.Portnum = 1;
    cfg.Pinnum = 30;
    cfg.Funcnum = 3;
    PINSEL_ConfigPin(&cfg);
    // ADC5:
    cfg.Portnum = 1;
    cfg.Pinnum = 31;
    cfg.Funcnum = 3;
    PINSEL_ConfigPin(&cfg);
}

/**
 * @details
 * By using the sensor datasheet
 * http://www-module.cs.york.ac.uk/hapr/resources/mbed_resources/datasheets/gp2y0a21yk0f.pdf
 * I have made an approximate function
 * f(x) = ((100 / (x + 8)) / 2) - 0.25
 * I rearranged it to get voltage to distance in cm
 * x = (100 / (2y + 0.5)) - 8
 *
 * Thank you for listening to comment announcements. Have a nice day.
 * @author Nathan Bartram (Sensor calibration)
 *
 * @details
 * In Nathan's version, y needs to be divided by 1000 first.
 * I simplified the equation further to:
 * x =   (100 / ( y / 500 + 0.5) ) - 8
 * I also decided to replace the numbers in the equation to variables.
 * The final form is:
 * x =  ( a  / ( y /  b  +  c ) ) - d
 *
 * @note The a, b, c, d variables are now defined at the top, they are adjusted too.
 *
 * @details
 * Currently the ADC is running in polling mode, and it has to reinitialised every time something is read.
 * @author Fufu Fang (reading from ADC and clean up)
 */
int ir_read(ir_Sensor sensor)
{
    int i, r = 0;

    ADC_Init(LPC_ADC, 200000);
    ADC_ChannelCmd(LPC_ADC, sensor, ENABLE);
    ADC_StartCmd(LPC_ADC, ADC_START_NOW);

    //takes 5 readings to get a more accurate result
    for (i = 0; i < 5; i++) {
        //while (!ADC_ChannelGetStatus(LPC_ADC, ir, 1));
        r += ADC_ChannelGetData(LPC_ADC, sensor);
    }
    r /= 5;

    r = ((A / ((float) r / B + C)) - D) * 10;
    return r;
}
