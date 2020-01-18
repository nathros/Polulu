/**
 * @file motor.c
 * @brief Polulu motor controller
 * @author Sam Gamble (typed up the command byte definition, created the 1st version, which is now the internal_motor_send function.)
 * @author Oliver Loftus (minor name changes related to function names.)
 * @author Fufu Fang  (rewrote everything, because of people's feature demands.)
 *
 */

#include "config.h"
#include "debug.h"
#include "uart.h"
#include "delay.h"
#include "coordinate.h"
#include "motor.h"

#define L_FWD                                   0xC1
#define L_REV                                   0xC2
#define R_FWD                                   0xC5
#define R_REV                                   0xC6

static short g_left_speed = 0;
static short g_right_speed = 0;
static volatile int g_angle = 0;

static float LEFT_BIAS = LEFT_BIAS_BASE;
static float RIGHT_BIAS = RIGHT_BIAS_BASE;

void motor_bias_set(Motor motor, float n)
{
    if (motor == MOTOR_LEFT) {
        LEFT_BIAS = LEFT_BIAS_BASE + n;
    } else {
        RIGHT_BIAS = RIGHT_BIAS_BASE + n;
    }
}

void motor_bias_disable_all()
{
    LEFT_BIAS = 1;
    RIGHT_BIAS = 1;
}

void motor_bias_reset_all()
{
    LEFT_BIAS = LEFT_BIAS_BASE;
    RIGHT_BIAS = RIGHT_BIAS_BASE;
}

/**
 * @brief private function: send a command to motor
 * @warning This function has NO speed limit.
 * @details The lack of speed limit is to allow the robot
 * to rotate faster.
 */
void internal_motor_send(Motor motor, int n)
{
    char command[2];
    uint8_t *data = (uint8_t *) command;

    if (n > 0) {
        if (motor == MOTOR_LEFT) {
            command[0] = L_FWD;
            g_left_speed = n;
            n = (float) n * LEFT_BIAS;
        } else {
            command[0] = R_FWD;
            g_right_speed = n;
            n = (float) n * RIGHT_BIAS;
        }
    } else {
        /* turn negative n possitive for the 2nd command byte */
        n = -n;
        if (motor == MOTOR_LEFT) {
            command[0] = L_REV;
            g_left_speed = -n;
            n = (float) n * LEFT_BIAS;
        } else {
            command[0] = R_REV;
            g_right_speed = -n;
            n = (float) n * RIGHT_BIAS;
        }
    }

    command[1] = n;
    uart_send_blk(data, 2);
}

/**
 * @brief private function: command the motor to rotate
 * @warning This function does not modify the internal g_angle record.
 */
static void internal_motor_rotate(volatile short angle)
{
    enum Direction {Clockwise, Anticlockwise};
    enum Direction direction;
    #ifdef MOTOR_DEBUG
        dbg_printf("Motor: Internal: Entering, Rotating %d \n\r", angle);
    #endif

    /* Defensive programming, sanity check */
    if (angle == 0) {
        return;
    }

    #ifdef TIMED_ROTATION
        int delay = (float) angle / 180 * ROTATION_TIME;
        delay += ROTATION_WARMUP;
    #endif

    if (angle > 0) {
        direction = Clockwise;
        internal_motor_send(MOTOR_LEFT, ROTATION_SPEED);
        internal_motor_send(MOTOR_RIGHT, -ROTATION_SPEED);


    } else {
        direction = Anticlockwise;
        internal_motor_send(MOTOR_LEFT, -ROTATION_SPEED);
        internal_motor_send(MOTOR_RIGHT, ROTATION_SPEED);
    }

    #ifdef TIMED_ROTATION
        if (delay < 0) {
            delay = -delay;
        }
        delay_start(delay);
    #endif

    #ifdef COORD_ROTATION
        /* deviation formula is : 0.9x+(0.014x)^2, by empirical observation */
        int target = g_angle + angle * INERTIA_A + (angle * INERTIA_B) * (angle * INERTIA_B);
        //dbg_printf("Motor: Internal: Fresh Target: %d\r\n", target);
        int roll_over = target / 360;
        target %= 360;
        if (target < 0 && direction == Anticlockwise) {
            target += 360;
            roll_over = 1;
        }

        #ifdef MOTOR_DEBUG
            dbg_printf("Motor: Internal: Starting: Current %d, Target %d, Roll_over %d\r\n", g_angle, target, roll_over);
        #endif

        if (direction == Clockwise) {
            if(!roll_over) {
                while (g_angle < target) {
                    #ifdef MOTOR_DEBUG
                    dbg_printf("Motor: Clockwise, !roll_over: Target %d\r\n", target);
                    #endif
                }
            } else {
                while (g_angle > target) {
                    #ifdef MOTOR_DEBUG
                    dbg_printf("Motor: Clockwise, roll_over(1): Target %d\r\n", target);
                    #endif
                }
                while (g_angle < target) {
                    #ifdef MOTOR_DEBUG
                    dbg_printf("Motor: Clockwise, roll_over(2): Target %d\r\n", target);
                    #endif
                }
            }
        } else {
            if(!roll_over) {
                while (g_angle > target) {
                    #ifdef MOTOR_DEBUG
                    dbg_printf("Motor: Clockwise, !roll_over: Target %d\r\n", target);
                    #endif
                }
            } else {
                while (g_angle < target) {
                    #ifdef MOTOR_DEBUG
                    dbg_printf("Motor: Anticlockwise, roll_over(1): Target %d\r\n", target);
                    #endif
                }
                while (g_angle > target) {
                    #ifdef MOTOR_DEBUG
                    dbg_printf("Motor: Anticlockwise, roll_over(2): Target %d\r\n", target);
                    #endif
                }
            }
        }
    #endif

    motors_stop();
    #ifdef MOTOR_DEBUG
        dbg_printf("Motor: Internal: Exiting\r\n", target);
    #endif
}

/**
 * @brief Speed limiter
 * @details Clip off speed that is too fast.
 */
static int internal_limiter(int n)
{
    int s = 1;
    if (n < 0) {
        s = -1;
        n = -n;
    }
    if (n > SPEED_LIMIT) {
        n = SPEED_LIMIT;
    }

    n = n * s;
    return n;
}

/**
 * @details This essentially wraps around internal_motor_send.
 * This allows motor_rotate to have a different speed limit to
 * motor_set.
 */
void motor_set(Motor motor, int n)
{
    n=internal_limiter(n);
    internal_motor_send(motor, n);
}

int motor_read(Motor motor)
{
    if (motor == MOTOR_LEFT) {
        return g_left_speed;
    } else {
        return g_right_speed;
    }
}

void motors_stop()
{
    motor_set(MOTOR_LEFT, 0);
    motor_set(MOTOR_RIGHT, 0);
}

void motors_move(int n)
{
    n=internal_limiter(n);
    motor_set(MOTOR_LEFT, n);
    motor_set(MOTOR_RIGHT, n);
}

/**
 * @details In timed rotation mode, This function essential wraps around
 * internal_motor_rotate. If the robot is to rotate above 360 degrees,
 * it rotate back to 0 degree first, then rotate to the resulting angle.
 *
 * The robot can rotate back to the starting position quite accurately
 * by rotating in the reverse direction for the same amount of time.
 * However asking the robot to rotate 360 degrees does not result in
 * accurate 360 degrees rotation.
 */
void motor_rot_rel(short angle)
{
    #ifdef MOTOR_DEBUG
        dbg_printf("Motor: Rel: Rotating %d \n\r", angle);
    #endif

    #ifdef TIMED_ROTATION
        /* Code for rotating back to the starting position */
        int new_angle = g_angle + angle;
        int full_rotation = new_angle / 360;
        if (full_rotation) {
            internal_motor_rotate(-g_angle);
            g_angle = 0;
            angle = new_angle % 360;
        }
    #endif

    #ifdef MOTOR_DEBUG
        dbg_printf("Motor: Rel: Calling %d \n\r", angle);
    #endif
    internal_motor_rotate((volatile short)angle);

    #ifdef TIMED_ROTATION
        g_angle += angle;
    #endif

    #ifdef MOTOR_DEBUG
        dbg_printf("Motor: Rel: Rotated to: %d \n\r\n\r", g_angle);
    #endif
}

void motor_rot_abs(short target)
{
    #ifdef MOTOR_DEBUG
        dbg_printf("Motor: Abs: Target = %d \n\r", target);
    #endif
    /* handling weird inputs */
    if (target > 360) {
        target %= 360;
    }
    else if (target < 0) {
        target %= 360;
        target += 360;
    }

    /* defensive programming - normalise internal record */
    g_angle %= 360;
    /* rotation angle should never be over 180 degrees,
     * otherwise it is inefficient.                     */
    int rotation_angle = target - g_angle;
    if (rotation_angle > 180) {
        rotation_angle = -(360-target) - g_angle;
    }
    else if (rotation_angle < -180) {
        rotation_angle = (360-g_angle) + target;
    }
    motor_rot_rel(rotation_angle);
}

void motor_angle_rest()
{
    g_angle = 0;
}

void motor_angle_sync(int n)
{
    g_angle = n;
    #ifdef MOTOR_DEBUG
        dbg_printf("Current %d \n\r", g_angle);
    #endif
}

int motor_angle()
{
    #ifdef MOTOR_DEBUG
        dbg_printf("Motor: Angle: %d \n\r", g_angle);
    #endif
    return g_angle;
}
