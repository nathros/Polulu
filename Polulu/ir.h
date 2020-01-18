#ifndef IR_H
#define IR_H

/**
 * @file ir.h
 * @author Fufu Fang (wrote the header)
 * @brief Robot IR receiver handler header
 * @pre you need to call ir_init() first to initialise
 * the subsystem first, before using any other functions.
 */

#include "config.h"

/**
 * @brief IR initialisation
 * @details In fact, this initialise the pins on ADC.
 */
void ir_init();

/**
 * @brief Read data off IR sensors
 * @param sensor the name of the IR sensor
 * @return integer reading of the IR sensor
 */
int ir_read(ir_Sensor sensor);

#endif
