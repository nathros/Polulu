#ifndef LCD_H
#define LCD_H

/**
 * @file lcd.h
 * @brief LCD controller header
 * @pre You should call lcd_clear() first before running any other function.
 * This reset the location of the LCD cursor.
 * @author Fufu Fang (wrote the whole thing)
 */

/**
 * @brief printf function for the lcd
 * @warning Each line on the LCD only allows 8 characters. If you go over
 * the 8 characters limit during a function call, characters over the limit
 * will be truncated.
 * @details If the function detects that you have gone over the 8-character
 * limit after a function call. It will automatically move the cursor to the
 * next line.
 * @param *format the formatting string
 * @param ... any variable you want to include in the formatted string
 * @return the number of characters printed
 */
int lcd_printf(const char *format, ...);

/**
 * @brief initialise LCD screen
 * @details Clear LCD screen using a single UART command, then
 * set the internal x,y counters to 0.
 */
void lcd_init();

/**
 * @brief clear LCD screen
 * @details clear LCD screen, the cursor is moved to 0, 0.
 */
void lcd_clear();

/**
 * @brief move LCD cursor
 * @details move LCD cursor to the coordinate x,y.
 * @param x the column
 * @param y the row
 */
void lcd_goto(short x, short y);

/**
 * @brief move to the next line
 * @details move the cursor to the beginning of the next line. If the cursor
 * is at * the first line, it will move to the second line. If the cursor is
 * at the second line, it will move to the first line.
 */
void lcd_next_line();
#endif
