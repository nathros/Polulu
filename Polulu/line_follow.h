#ifndef LINE_FOLLOW_H
#define LINE_FOLLOW_H

/**
 * @file line_follow.h
 * @author Oliver Loftus
 * @brief Contains line following algorithms headers
 */

/**
 * @brief Set the robot to follow a line
 */
void nav_follow_line(int speed);

/**
 * @brief Stop the line following algorithm
 */
void nav_stop_follow_line();

#endif
