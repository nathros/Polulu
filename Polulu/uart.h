#ifndef UART_H
#define UART_H

/**
 * @file uart.h
 * @author Fufu Fang (wrote the header file)
 * @brief UART communication handler header
 * @pre you need to call uart_init() first to initialise
 * the subsystem before using any other functions!
 * @bug it seems that non-blocking send produces dodgy results.
 */

#include <stdint.h>
/**
 * Initialise UART system
 */
void uart_init();

/**
 * @brief Blocking send function
 * @param msg character pointer of your message
 * @param len the len of your message
 * @return the number of bytes sent
 */
int uart_send_blk(uint8_t * msg, int len);

/**
 * @brief Blocking receive function
 * @param msg character pointer of your message
 * @param len the len of your message
 * @return the number of bytes sent
 */
int uart_recv_blk(uint8_t * msg, int len);

/**
 * @brief Non-blocking send function
 * @param msg character pointer of your message
 * @param len the len of your message
 * @return the number of bytes received
 */
int uart_send_nblk(uint8_t * msg, int len);

/**
 * @brief Non-blocking receive function
 * @param msg character pointer of your message
 * @param len the len of message you are expecting to receive
 * @return the number of bytes received
 */
int uart_recv_nblk(uint8_t * msg, int len);

/**
 * @brief Send a single byte down UART
 * @param byte the byte to be sent down
 */
void uart_send_byte(uint8_t byte);

/**
 * @brief Receive a single byte from UART
 * @return an integer from UART
 */
int uart_recv_byte();
#endif
