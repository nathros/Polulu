#ifndef COMMON_H
#define COMMON_H

/**
 * @file common.h
 * @brief Common mathematics routines (header)
 * @details These maths routines are not included in math.h.
 * @author Fufu Fang (wrote the whole thing)
 */

/**
 * @brief Convert degree to radian
 */
float toRadian(float n);

/**
 * @brief Convert radian to degree
 */
float toDegree(float n);

/**
 * @brief get absolute value
 * @param[in] n a number
 */
float abs(float n);

#endif
