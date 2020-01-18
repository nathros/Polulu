#ifndef MOTOR_H
#define MOTOR_H

/**
 * @file motor.h
 * @brief Polulu motor controller header
 * @author Fufu Fang (wrote the header file)
 * @author Oliver Loftus (Minor name changes)
 * @note
 * - SPEED_LIMIT: The top speed of the motor during general movement,
 * Any speed above will be clipped to SPEED_LIMIT.
 * - ROTATION_TIME: The time it takes for the robot to rotate 180
 * degrees in miliseconds.
 * - ROTATION_WARMUP: A constant time added to the ROTATION_TIME to warm
 * up the motor.
 * - ROTATION_SPEED: The speed of rotation, it is independent to
 * SPEED_LIMIT.
 * - INERTIA_A: the linear component for inertia correction in mouse rotation
 * - INERTIA_B: the quadratic component for inertia correction in mouse rotation
 */


/* Conditional Compilation Settings */
//#define MOTOR_DEBUG
//#define TIMED_ROTATION
#define COORD_ROTATION

/* General definitions */
#define SPEED_LIMIT 30
#define ROTATION_SPEED 40

/* Timed rotation definitions */
#ifdef TIMED_ROTATION
#define ROTATION_TIME 670
#define ROTATION_WARMUP 0
#endif

/* Mouse rotation definitions */
#ifdef COORD_ROTATION
#define INERTIA_A 0.9
#define INERTIA_B 0.014
#endif

/**
 * Two motors, left and right
 */
typedef enum {
    MOTOR_LEFT,
    MOTOR_RIGHT
} Motor;

/**
 * @brief set motor bias
 * @details set extra motor bias on top of the bias set by calibration, so the
 * final bias depending on motor is LEFT_BIAS_BASE + n or RIGHT_BIAS_BASE+n.
 * @param[in] motor the motor to set
 * @param[in] n the extra bias to set
 */
void motor_bias_set(Motor motor, float n);

/**
 * @brief disable motor bias
 * @details set LEFT_BIAS and RIGHT_BIAS to 1.
 */
void motor_bias_disable_all();

/**
 * @brief reset motor bias
 * @details set LEFT_BIAS to LEFT_BIAS_BASE, and RIGHT_BIAS to RIGHT_BIAS_BASE.
 */
void motor_bias_reset_all();

/**
 * @brief private function: send a command to motor
 * @warning This function has NO speed limit.
 * @details The lack of speed limit is to allow the robot
 * to rotate faster.
 */
void internal_motor_send(Motor motor, int n);

/**
 * @brief Set motor speed
 * @details Set the motor speed via UART and change the speed
 * record in an static int variable. The motor speed is limited
 * to SPEED_LIMIT.
 * @param motor either LEFT or RIGHT
 * @param n the new speed of the motor
 */
void motor_set(Motor motor, int n);

/**
 * @brief Read motor speed
 * @details Returns the current motor speed from a static
 * int variable.
 * @param motor either LEFT or RIGHT
 */
int motor_read(Motor motor);

/**
 * @brief Stop both motors
 */
void motors_stop();

/**
 * @brief Move both motors
 * @param[in] n the speed of motors
 */
void motors_move(int n);

/**
 * @brief rotate the Pololu bot for arbitary angle
 * @param angle the angle of rotation
 */
void motor_rot_rel(short angle);

/**
 * @brief rotate the Pololu bot to an absolute bearing
 * @param angle the angle of the absolute bearing
 */
void motor_rot_abs(short angle);

/**
 * @brief get current angle of the vehicle
 * @note this angle record might be different to the one in
 * coordinate.c
 * @return the current angle of the motor
 */
int motor_angle();

/**
 * @brief reset the vehicle angle record
 */
void motor_angle_rest();

#ifdef COORD_ROTATION
/**
 * @brief sync the motor.c angle record with coordinate.c
 */
void motor_angle_sync(int n);
#endif

#endif
