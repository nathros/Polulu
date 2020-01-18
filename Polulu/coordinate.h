#ifndef COORD_H
#define COORD_H

/**
 * @file coordinate.h
 * @brief Coordinate record module header
 * @author Fufu Fang (wrote the whole thing)
 */

//#define COORD_DBG
//#define DISTANCE_DBG

/**
 * @struct Coordinate
 * @brief Coordinate record structure
 */
typedef struct {
    float x; /**< distance of x in cm */
    float y; /**< distance of y in cm */
    float t; /**< angle in radian */
} Coordinate;

/**
 * @struct Polar
 * @brief Polar Coordinate structure
 */
typedef struct {
    float r; /**< radius */
    float t; /**< angle */
} Polar;

/**
 * @brief set the target angle for the pid line following algorithm
 * @param[in] n the target angle (between 0 - 360 degrees)
 */
void coordinate_angle_set(int n);

/**
 * @brief Clear coordinate record
 */
void coordinate_clear();

/**
 * @brief read global coordinate record
 * @details the global coordinate record contains the coordinate
 * the robot is on. Both x and y are in unit of cm, whereas
 * the angle is in radian. It is normalised between 0 and 2pi.
 */
Coordinate coordinate_read();

/**
 * @brief Update global Coordinate record
 * @details Apply transformation to the coordinate update, and add the
 * changes to the global record.
 * @param update the change in coordinate in local frame of reference
 */
void coordinate_update(Polar update);

/**
 * @brief read total distance travelled
 */
float distance_read();

/**
 * @brief clear distance record.
 */
void distance_clear();

#ifdef COORD_DBG

/**
 * @brief read the change global coordinate record in the last update
 */
Coordinate coordinate_diff();

/**
 * @brief Clear coordinate difference record
 */
void coordinate_diff_clear();

#endif

#endif
