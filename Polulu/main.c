

#include "motor.h"
#include "ir.h"
#include "uart.h"
#include "debug.h"
#include "line_search.h"
#include "config.h"
#include "gpio.h"
#include "pid.h"

#include "coordinate.h"
#include "rit.h"
#include "advanced_movement.h"
#include <lpc17xx_rit.h>



char CurrentState = 'f';
uint8_t LeftSpeed = 20;
uint8_t RightSpeed = 20;
char CharBuf[8] = {0,0,0,0,0,0,0,0};
uint8_t CharCount = 0;
int IsOutput = 1;
int IsCorrecting = 0;


//screen /dev/ttyACM0
//ctrl + a, shift + h for record

void RIT_IRQHandler(void)
{
    RIT_GetIntStatus(LPC_RIT);
    if (IsCorrecting) return;   
    uint8_t Front = ir_read(IR_FRONT);
    uint8_t Left = ir_read(IR_LEFT);
    uint8_t Right = ir_read(IR_RIGHT);
    uint8_t Back = ir_read(IR_BACK);     
    dbg_printf("I %d %d %d %d\r\n",Front,Left,Right,Back);
    switch (CurrentState) 
    {
       case 'w': adv_motors_rpm(1); break;
       case 's': adv_motors_rpm(0); break;
    }     
}


int main()
{
  init();
  while(1)
  {
    
    switch(dbg_getchar())
    {
      case 'y':
        if (IsCorrecting == 0) IsCorrecting = 1;
        else IsCorrecting = 0;
        dbg_printf("M %d %d\r\n",0,0);
        motor_set(MOTOR_LEFT,0);
        motor_set(MOTOR_RIGHT,0);
        break;
        
      case 'w':
        //motor_set(MOTOR_LEFT,LeftSpeed);
        //motor_set(MOTOR_RIGHT,RightSpeed);
        CurrentState = 'w';
        adv_motors_rpm(1);
        if (IsCorrecting) break;
        RIT_Disable();
        dbg_printf("M %d %d\r\n",30,30);
        RIT_Enable();
        break;
      
      case 'd':
        //motor_set(MOTOR_LEFT,LeftSpeed);
        //motor_set(MOTOR_RIGHT,-RightSpeed);
        CurrentState = 'd';
        motor_set(MOTOR_LEFT,30);
        motor_set(MOTOR_RIGHT,-30);
        if (IsCorrecting) break;
        RIT_Disable();
        dbg_printf("M %d %d\r\n",30,-30);
        RIT_Enable();
        break;
        
      case 'a':
        //motor_set(MOTOR_LEFT,-LeftSpeed);
        //motor_set(MOTOR_RIGHT,RightSpeed);
        CurrentState = 'a';
        motor_set(MOTOR_LEFT,-30);
        motor_set(MOTOR_RIGHT,30);
        if (IsCorrecting) break;
        RIT_Disable();
        dbg_printf("M %d %d\r\n",-30,30);
        RIT_Enable();
        break; 
        
      case ' ': 
        CurrentState = ' ';
        motors_stop();
        if (IsCorrecting) break;
        RIT_Disable();
        dbg_printf("M %d %d\r\n",0,0); 
        RIT_Enable();
        break;
        
      case 's':
        if (IsCorrecting) break;
        //motor_set(MOTOR_LEFT,-LeftSpeed);
        //motor_set(MOTOR_RIGHT,-RightSpeed);
        CurrentState = 's';
        adv_motors_rpm(0);
        RIT_Disable();
        dbg_printf("M %d %d\r\n",-30,-30);
        RIT_Enable();
        break;
        
      case 13:
        dbg_printf("Entered config Mode\r\n");
        CharCount = 0;
        char tempchar;
        while(1)
        {
          tempchar = dbg_getchar();
          if(tempchar != 13)
          {
            CharBuf[CharCount] = tempchar;
            CharCount++;
          }
          else break;
        }
        switch(CharBuf[0])
        {
          case 'l':
            CharBuf[0] = 13;
            CharBuf[CharCount] = '\0';
            LeftSpeed = atoi(CharBuf);
            dbg_printf("Left Motor now is: %d\r\n", LeftSpeed);
            break;
            
          case 'r':
            CharBuf[0] = 13;
            CharBuf[CharCount] = '\0';
            RightSpeed = atoi(CharBuf);
            dbg_printf("Right Motor now is: %d\r\n", RightSpeed);
            break;
        }
        break; 
    }
    
  }        
  
  
   
}

void init()
{
    encoder_rpm_init();
    debug_frmwrk_init();
    uart_init();
    ir_init();
    dbg_printf("-- Start of execution! --\r\n");
    
    RIT_Start(100);
    //char temp = dbg_getchar();
    //dbg_printf("@@@%d\r\n",temp);
    motors_stop();
}



