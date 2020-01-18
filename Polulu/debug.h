#ifndef DEBUG_H
#define DEBUG_H

/**
 * @file debug.h
 * @author Fufu Fang (wrote the whole thing)
 * @brief debug framework enhancer header
 */

#include <debug_frmwrk.h>

#include "coordinate.h"
#include "mouse.h"

/** Macro Functions **/
#define dbg_getchar() _DG

/**
 * @brief printf function for the debugging console
 * @pre you need to call debug_frmwrk_init() first!
 * @param *format the formatting string
 * @param ... any variable you want to include in the formatted string
 * @return the number of characters printed
 */
int dbg_printf(const char *format, ...);

/**
 * @brief print a coordinate record on LCD
 * @param[in] record the record to be printed
 */
void lcd_print_coordinate(Coordinate record);

/**
 * @brief print a coordinate record to the console
 * @param[in] record the record to be printed
 */
void dbg_print_coordinate(Coordinate record);

#ifdef MOUSE_DEBUG
/**
 * @brief print a mouse record to the console
 * @param[in] record the record to be printed
 */
void dbg_print_mouse(Mouse_dbg record);
#endif

#endif
