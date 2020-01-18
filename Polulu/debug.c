/**
 * @file debug.c
 * @author Fufu Fang (wrote the whole thing)
 * @brief debug framework enhancer
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "common.h"
#include "lcd.h"

#include "debug.h"

int dbg_printf(const char *format, ...)
{
    va_list arg;
    char processed[256];
    va_start(arg, format);
    int n = vsnprintf(processed, 256, format, arg);
    va_end(arg);
    processed[255] = '\0';
    _DBG(processed);
    return n;
}

void lcd_print_coordinate(Coordinate record)
{
    lcd_clear();
    lcd_printf("x:%d", (int)record.x);
    lcd_next_line();
    lcd_printf("y:%d", (int)record.y);
}

void dbg_print_coordinate(Coordinate record)
{
    dbg_printf("x: %f\ty: %f\tt: %f\r\n", record.x, record.y, toDegree(record.t));
}

#ifdef MOUSE_DEBUG
void dbg_print_mouse(Mouse_dbg record)
{
    dbg_printf("raw_l: %d\traw_t: %d\tcal_l: %f\tcal_t: %f\tcount: %d\r\n", record.raw_l,
               record.raw_t, record.cal_l, record.cal_t, record.count);
}
#endif
