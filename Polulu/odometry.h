#ifndef ODOMETRY
#define ODOMETRY

/**
 * @file odometry.h
 * @brief Wheel encoder based odometery module (header file)
 * @author Fufu Fang (Personal part of the project)
 */

//#define ODOMETRY_DBG
//#define RPM_DBG
//#define UPDATE_DBG


typedef enum {
    LEFT_ENCODER,
    RIGHT_ENCODER
} Encoder;

/**
 * @struct RPM
 * @brief Motor RPM record structure
 */
typedef struct {
    float left; /**< rpm of left motor */
    float right; /**< rpm of right motor */
} RPM;

/**
 * @brief clear encoder record
 */
void encoder_clear();

/**
 * @brief increase the internal encoder count
 */
void encoder_update(Encoder encoder);

/**
 * @brief Initialise the timer for odometer rpm calculation
 */
void encoder_rpm_init();

/**
 * @brief Interrupt service routine for Timer 3
 */
void TIMER3_IRQHandler();

/**
 * @brief return motor rpm
 */
float encoder_rpm(Encoder encoder);

/**
 * @brief turn off motor rpm timer
 */
void encoder_rpm_off();

#endif
