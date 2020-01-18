/**
 * @file
 * @brief Test line sensor functions and print values to terminal via debug
 * @author Sam Gamble
 * 
 * 
 */



#include <stdio.h>
#include "../line_sensors.c"
#include "../debug.c"       
#include "../uart.h"




int i;					   //for silly delay
uint16_t values[5];        //to make sure each element is EXACTLY 2bytes 
uint16_t line_location;    // internal line est 



void main()
{
	debug_frmwrk_init();
	uart_init();
		
		
		dbg_printf("Initiazing Line Sensor Test Program\r\n");
		reset_calibration();
	

		dbg_printf("--Starting Auto-calibrate Sequence--\r\n");
		autocalibrate_sensors();

	
	//Read Raw Values 0-2000
		read_sensors_raw(values);
		dbg_printf("Raw Sensor Values: %d %d %d %d %d\r\n",values[0],values[1],values[2],values[3],values[4]);
		
		
		
		
		//read and calibrated values  0-1000 
		read_sensors(values);
		dbg_printf("Calibrated Sensor Values: %d %d %d %d %d\r\n",values[0],values[1],values[2],values[3],values[4]);
		
		
		dbg_printf("\r\n");
		dbg_printf("\r\n");
		
	//begin polling sensor values 

	

	while(1)
	{
		read_sensors(values);
		line_location = read_line_position();  //read line pos  0-4000
		
		dbg_printf("Sensors:  %5u %5u %5u %5u %5u    Line Posistion: %u \r\n",values[0],values[1],values[2],values[3],values[4], line_location);
		
	
		for (i=0;i<1000000;i++);   //silly delay
	}
}
