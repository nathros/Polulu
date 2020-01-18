/**
 * @file uart.c
 * @author Sam Gamble (first version)
 * @author Fufu Fang (clean up and added more functions)
 * @brief UART communication handler
 * @details
 * Send data pointed to by uint8_t* to the robot using
 * UART Port 3, which are using pins 9 & 10 on the
 * MBED board.
 */

#include <lpc17xx_uart.h>
#include <lpc17xx_pinsel.h>

#include "uart.h"

void uart_init()
{
    /*
     * Create all the structs
     */

    UART_CFG_Type uart_cfg;
    UART_FIFO_CFG_Type fifo_fg;
    PINSEL_CFG_Type pin_cfg;

    /*
     * Configure the pins
     */

    //Common settings
    pin_cfg.Funcnum = 2;
    pin_cfg.OpenDrain = 0;
    pin_cfg.Pinmode = 0;
    pin_cfg.Portnum = 0;

    //Configure the two pins
    pin_cfg.Pinnum = 0;
    PINSEL_ConfigPin(&pin_cfg);
    pin_cfg.Pinnum = 1;
    PINSEL_ConfigPin(&pin_cfg);


    /*
     * Configuring UART
     */
    UART_ConfigStructInit(&uart_cfg);
    uart_cfg.Baud_rate = 115200;
    UART_Init(LPC_UART3, &uart_cfg);


    /*
     * Configuring FIFO
     */
    UART_FIFOConfigStructInit(&fifo_fg);
    UART_FIFOConfig(LPC_UART3, &fifo_fg);

    /*
     * Enable UART send
     */
    UART_TxCmd(LPC_UART3, ENABLE);

}

int uart_send_blk(uint8_t * msg, int len)
{
    return UART_Send(LPC_UART3, msg, len, BLOCKING);
}

int uart_recv_blk(uint8_t * msg, int len)
{
    return UART_Receive(LPC_UART3, msg, len, BLOCKING);
}

int uart_send_nblk(uint8_t * msg, int len)
{
    return UART_Send(LPC_UART3, msg, len, NONE_BLOCKING);
}

int uart_recv_nblk(uint8_t * msg, int len)
{
    return UART_Receive(LPC_UART3, msg, len, NONE_BLOCKING);
}

void uart_send_byte(uint8_t byte)
{
    UART_SendByte(LPC_UART3, byte);
}


int uart_recv_byte()
{
    return UART_ReceiveByte(LPC_UART3);
}
