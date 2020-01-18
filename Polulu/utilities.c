/**
 * @file utilities.c
 * @brief Polulu control utilities (source)
 * @details This module handles miscellaneous commands to AVR processor on the Polulu
 * @author Sam Gamble
 * @author Fufu Fang (tidying up)
 */

#include <string.h>

#include "uart.h"
#include "debug.h"

#include "utilities.h"

#define SIGNATURE                               0x81
#define TRIMPOT                                 0xB0
#define BATTERY                                 0xB1
#define PLAY_MUSIC                              0xB3


int request_sig(char *read_sig)
{
    uint8_t *data = (uint8_t *) read_sig;
    uart_send_byte(SIGNATURE);
    return uart_recv_blk(data, 6);
}

int read_trimpot()
{
    uint16_t reading;
    uint8_t *data = (uint8_t *) & reading;
    uart_send_byte(TRIMPOT);
    uart_recv_blk(data, 2);
    dbg_printf("Trimpot reading: %d\r\n");
    return reading;
}

int read_battery()
{
    uint16_t reading;
    uint8_t *data = (uint8_t *) & reading;
    uart_send_byte(BATTERY);
    uart_recv_blk(data, 2);
    dbg_printf("Battery voltage: %d mV");
    return reading;
}

void play_music(char *score)
{
    uint8_t score_length = strlen(score);
    //command (1 byte), length of music string(1 bytes), music(100 bytes MAX)
    uint8_t send_score[score_length + 2];
    int i;
    send_score[0] = 0xB3;
    send_score[1] = score_length;
    for (i = 0; i < score_length + 2; i++) {
        send_score[i + 2] = score[i];
    }
    /* Debugging info */
    dbg_printf("Music Output Stream: %X %d ", send_score[0],
               send_score[1]);
    for (i = 2; i < score_length + 2; i++) {
        dbg_printf("%c", send_score[i]);
    }
    dbg_printf("    Send Length = %d", score_length + 2);
    dbg_printf("\n\r");
    uart_send_blk(send_score, score_length + 2);
}
