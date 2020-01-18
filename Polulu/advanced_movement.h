#ifndef ADVMNT_H
#define ADVMNT_H

/**
 * @file advanced_movement.h
 * @author Fufu Fang
 * @author Sam Gamble
 * @brief Pololu advanced movement module header
 */

#include "coordinate.h"
#include "odometry.h"

/**
 * @brief set the abort / exit flag for advanced movement functions
 * @details If the system is in a travel loop, it will break out the loop.
 * @author Sam Gamble
 */
void adv_exit();

/**
 * @brief update the local rpm record
 * @details odometry.c calls this function when there is a new rpm update.
 */
void adv_rpm_update(RPM rpm);

/**
 * @brief update the normalised angle value for the pid angle following
 * @param[in] n the target angle (between -2000 to 2000)
 * @author Fufu Fang
 */
void adv_angle_update(int n);

/**
 * @brief Move the robot for certain distance
 * @param[in] distance the distance to move in cm.
 * @author Fufu Fang
 */
void adv_travel(float distance);

/**
 * @brief Rotate and move (Non-PID version)
 * @details Rotates to face desired angle of travel, then travel for a desired
 * distance
 * @param[in] 'distance' Distance of the straight line to follow (in cm)
 * @param[in] 'bearing' the target bearing for the robot (in degrees)
 * @author Fufu Fang
 */
void adv_move_polar(float distance, int bearing);

/**
 * @brief move to a location (Non-PID version)
 * @details Rotates to face angle of travel, then move a certain
 * distance, then rotate to face the final angle.
 * @param[in] target the target coordinate
 * @author Fufu Fang
 */
void adv_move(Coordinate target);

/**
 * @brief Rotate and move (PID version)
 * @details This version is similar to adv_move_polar, except it uses
 * the PID algorithm to travel in a straight line.
 * @author Sam Gamble
 */
void adv_move_pid_polar(float distance, int bearing);

/**
 * @brief Implementing function for straight line movement (based on mouse data)
 * @details This version is similar to adv_move, except it uses
 * the PID algorithm to travel in a straight line.
 * @param[in] target the target coordinate
 * @author Fufu Fang (basically wrapped around Sam's adv_move_pid_polar)
 */
void adv_move_pid(Coordinate target);

/**
 * @brief Implementing PID function for straight line movement based on wheel encoders
 * @details untested and crazy stuff might happen depeding on the PID constancts set
 * @param[in] Distance to travel (Float)
 * @author Sam Gamble
 */
void adv_motors_rpm(float distance);

/**
 * @brief Print readings from RPM
 * @author Sam Gamble
 */
void adv_print_rmp();
#endif
