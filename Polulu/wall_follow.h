#ifndef WALL_FOLLOW_H
#define WALL_FOLLOW_H

/**
 * @file wall_follow.h
 * @author Oliver Loftus
 *
 * Wall following algorithm headers
 */

/**
 * @brief Which wall?
 */
typedef enum { WALL_LEFT, WALL_RIGHT } nav_Wall;

/**
 * @brief Set the robot to follow a wall
 */
void nav_follow_wall(nav_Wall wall);

/**
 * @brief Stop the wall following algorithm
 */
void nav_stop_follow_wall();


/**
 * @brief External start and stop commands for wall follow
 */
void follow_wall_stop();
void follow_wall_start();

#endif
