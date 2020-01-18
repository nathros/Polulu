#include "rit.h"
#include "pid.h"
#include "debug.h"
#include "uart.h"
#include "line_sensors.h"


void RIT_IRQHandler(void)
{
    RIT_GetIntStatus(LPC_RIT);
    pid_check();
    pid_run(1,1,1,40);
}
 
 
 
void main()
{
	debug_frmwrk_init();
	uart_init();	
	dbg_printf("====================== STARTING PID TEST ======================\n\r");

	RIT_Start(250);
    line_calibration_reset(); 
	line_autocalibrate();
	
	while(1)
	{
		dbg_printf("---No Line--- \n\r");
	}
}
