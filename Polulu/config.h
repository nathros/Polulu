#ifndef CONFIG_H
#define CONFIG_H
/**
 * @file config.h
 * @brief Centralised Configuration File
 * @details This file contains the configuration data for IR sensor,
 * collision sensor, motor, and the mouse.
 *
 * @note For more details on pin configuration, please visit
 * http://www-users.cs.york.ac.uk/~pcc/MCP/MbedPins.html.
 *
 * @note Motor related definitions:
 * - LEFT_BIAS and RIGHT_BIAS: these are multiplied to the speed of the
 * repective motors.
 *
 * @note For our robot (6):
 * - define COLLISION_SENSOR_PORT 0
 * - define COLLISION_SENSOR_PIN_NUM 16
 * - define COLLISION_SENSOR_PIN (1 << COLLISION_SENSOR_PIN_NUM)
 *
 * - define LEFT_BIAS 1
 * - define RIGHT_BIAS 0.85
 *
 * @author Fufu Fang (Started writing the file)
 * @author Oliver Loftus (Added more definitions)
 */

/* Use wheel encoder? */
#define WHEEL_ENCODER

/* Collision sensor definitions */
#define COLLISION_SENSOR_PORT 0
#define COLLISION_SENSOR_PIN_NUM 16
#define COLLISION_SENSOR_PIN (1 << COLLISION_SENSOR_PIN_NUM)

/* Motor related definitions */
#define LEFT_BIAS_BASE 1
#define RIGHT_BIAS_BASE 0.80

/**
 * @brief enumerated IR type
 * @details Well, apparently the technician connected the
 * IR sensors randomly to different ADCs.You have to change
 * the pins manually.
 *
 * Pin 15 = 0
 * Pin 16 = 1
 * Pin 17 = 2
 * Pin 18 = 3
 * Pin 19 = 4
 * Pin 20 = 5
 *
 * @note For our robot (6):
 * - FRONT_LEFT = 2,
 * - FRONT_RIGHT = 3,
 * - BACK_LEFT = 1,
 * - BACK_RIGHT = 0
 */
typedef enum {
    IR_FRONT = 3,
    IR_LEFT = 2,
    IR_RIGHT = 0,
    IR_BACK = 1,
} ir_Sensor;

#endif
