/**
 * @file coordinate.c
 * @brief Coordinate record module
 * @author Fufu Fang (wrote the whole thing)
 * @note
 * PID_FACTOR scales the pid value, so it is between (-2000 .. 0 .. 2000)
 * - 5.555    ->  (-180 ..  0  ..180)
 * - 11.111   ->  (-90  ..  0  .. 90)
 * - 22.222   ->  (-45  ..  0  .. 45)
 */

#include <math.h>
#include "debug.h"
#include "lcd.h"
#include "motor.h"
#include "common.h"
#include "advanced_movement.h"
#include "coordinate.h"

#define PID_FACTOR      11.111

#define PID_CAP

static Coordinate g_coord = {0, 0, 0};
static Coordinate g_diff = {0, 0, 0};
static float g_distance = 0;
static int g_target_angle = 0;

void coordinate_angle_set(int n)
{
    /* defensive programming, avoid crazy input */
    if (n < 0 || n > 360) {
        n = 0;
    }
    g_target_angle = n - 180;
}

void coordinate_clear()
{
    g_coord.x = 0;
    g_coord.y = 0;
    g_coord.t = 0;
}

void coordinate_update(Polar update)
{
    g_diff.t = update.t;
    g_coord.t += g_diff.t;

    /* Code for normalising angle record
     * between 0 to 2pi */
    if (g_coord.t < 0) {
        g_coord.t += 2*M_PI;
    }
    if (g_coord.t > (2*M_PI)) {
        g_coord.t -= 2*M_PI;
    }

    g_distance += update.r;

    g_diff.x = update.r * sin(g_coord.t);
    g_diff.y = update.r * cos(g_coord.t);
    g_coord.x += g_diff.x;
    g_coord.y += g_diff.y;

    int degree = (int)toDegree(g_coord.t);
    motor_angle_sync(degree);

    int pid_angle = (degree - 180 - g_target_angle) * PID_FACTOR;
    /* capping normalised_angle */
    #ifdef PID_CAP
    if (pid_angle > 2000) {
        pid_angle = 2000;
    } else if (pid_angle < -2000) {
        pid_angle = -2000;
    }
    #endif
    adv_angle_update(pid_angle);

    lcd_print_coordinate(g_coord);

    #ifdef COORD_DBG
        dbg_printf("Coordinate.c:  ");
        dbg_print_coordinate(g_coord);
    #endif
}

Coordinate coordinate_read()
{
    return g_coord;
}

void distance_clear()
{
    g_distance = 0;
}

float distance_read()
{
    #ifdef DISTANCE_DBG
        dbg_printf("Coordinate.c: Distance: %f\r\n", g_distance);
    #endif
    return g_distance;
}

#ifdef COORD_DBG
Coordinate coordinate_diff()
{
    return g_diff;
}

void coordinate_diff_clear()
{
    g_diff.x = 0;
    g_diff.y = 0;
    g_diff.t = 0;
}
#endif
