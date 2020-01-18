/**
 * @file common.c
 * @brief Common mathematics routines
 * @details These maths routines are not included in math.h.
 * @author Fufu Fang (wrote the whole thing)
 */
#include <math.h>

#include "common.h"

float toRadian(float n)
{
    return n / 180 * M_PI;
}

float toDegree(float n)
{
    return n / M_PI * 180;
}

float abs(float n)
{
    if (n < 0) {
        n = -n;
    }
    return n;
}
