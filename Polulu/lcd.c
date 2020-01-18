/**
 * @file lcd.c
 * @brief LCD controller
 * @author Fufu Fang (wrote all the functions)
 * @author Sam Gamble (typed up the command byte definition)
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "lcd.h"

#include "uart.h"

#define CLEAR_LCD                               0xB7
#define PRINT                                   0xB8
#define LCD_GOTO_XY                             0xB9

static int lcd_x = 0;
static int lcd_y = 0;

int lcd_printf(const char *format, ...)
{
    va_list arg;
    char temp[12];

    temp[2] = '\0';
    strcat(temp, format);
    char *processed = &temp[2];

    va_start(arg, format);
    int n = vsnprintf(processed, 9, format, arg);
    va_end(arg);

    temp[0] = PRINT;
    temp[1] = strlen(temp) - 2;
    temp[11] = '\0';

    uint8_t *data = (uint8_t *) temp;

    uart_send_blk(data, strlen(temp));

    lcd_x += n;

    if (lcd_x > 7) {
        lcd_next_line();
    }

    return n;
}

void lcd_init()
{
    lcd_x = 0;
    lcd_y = 0;
    uart_send_byte(CLEAR_LCD);
}

void lcd_clear()
{
    lcd_goto(0, 0);
    lcd_printf("          ");
    lcd_printf("          ");
    lcd_goto(0, 0);
}

void lcd_goto(short x, short y)
{
    uint8_t cmd[] = { LCD_GOTO_XY, x, y };
    uart_send_blk(cmd, 3);
    lcd_x = x;
    lcd_y = y;
}

void lcd_next_line()
{
    if (lcd_y == 0) {
        lcd_goto(0, 1);
        lcd_x = 0;
        lcd_y = 1;
    } else {
        lcd_goto(0, 0);
        lcd_x = 0;
        lcd_y = 0;
    }
}
