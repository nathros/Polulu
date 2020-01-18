#ifndef MOUSE_H
#define MOUSE_H

/**
 * @file mouse.h
 * @brief header file for mouse module
 * @author Fufu Fang (wrote the whole file)
 */


#include <stdint.h>

#include "coordinate.h"

//#define MOUSE_DEBUG

/**
 * @brief callback function for mouse movement
 */
void mouse_callback(uint8_t button, int8_t l, int8_t t);

/**
 * @brief callback function for mouse attachment
 */
void mouse_attach();

/**
 * @brief callback function for mouse detachment
 */
void mouse_detach();

/**
 * @brief Interrupt Service Routine for Timer 2
 * @details This ISR polls the mouse.
 */
void TIMER2_IRQHandler();

/**
 * @brief Setup the mouse subsystem
 * @details setup the function pointers for Jamie's library,
 * then initialise the timer using a private function.
 */
void mouse_setup();

#ifdef MOUSE_DEBUG
/**
 * @brief Mouse debugging record data structure
 */
typedef struct {
    int raw_l; /** raw arc length reading */;
    int raw_t; /** raw angle reading */
    float cal_l; /** calibrated arc length reading */
    float cal_t; /** calibrated angle reading */
    int count; /** How many times the function had been called*/
} Mouse_dbg;

/**
 * @brief dump mouse debug record.
 */
Mouse_dbg mouse_dbg_dump();

/**
 * @brief clear mouse debug record.
 */
void mouse_dbg_clear();
#endif

#endif
