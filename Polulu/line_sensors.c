/**
 * @file line_sensors.c
 * @brief Pololu line sensor module
 * @details In order to manually calibrate sensors, please call
 * calibrate_and_read. The Pololu slave program does not provide
 * a mechanism to perform calibration without reading.
 * @author Sam Gamble (wrote the first version)
 * @author Fufu Fang (rewrote quite a large portion, massively
 * improved the way results are read from the line sensors)
 */


#include "uart.h"

#include "line_sensors.h"

/* read sensor data commands */
#define READ_RAW_SENSORS                        0x86
#define READ_CALIBRATED_SENSORS	                0x87
#define READ_LINE_POSITION                      0xB6

/* sensor calibration commands */
#define CALIBRATE_AND_READ                      0xB4
#define RESET_CALIBRATION                       0xB5
#define AUTOCALIBRATE                           0xBA


/*DO NOT remove the extra line in the comment block below,
 * it is for Doxygen to create line breaks. */
/**
 * @details This function reads the sensor values into an array. The robot
 * only performs a new reading if we are not in PID mode. Otherwise,
 * it sends the most recent result immediately.
 *
 * Example usage:
 *
 * unsigned int sensor_values[8];
 *
 * sensors.read(sensor_values);
 * @warning the size of result array must be enough to contain 5 uint16_t values,
 * or 10 uint8_t values.
 * @return The values returned (between 0-1000) are measures of reflectance
 * in arbitary units, with * higher values corresponding to lower reflectance.
 * 0 represents white,
 *
 */
void line_read(uint16_t* result)
{
    uint8_t* new_result = (uint8_t*) result;
    uart_send_byte(READ_CALIBRATED_SENSORS);
    uart_recv_blk(new_result, 10);
}

/**
 * @details This function performs the same function as line_read, except
 * it returns the raw readings.
 */
void line_read_raw(uint16_t * result)
{
    uint8_t* new_result = (uint8_t*) result;
    uart_send_byte(READ_RAW_SENSORS);
    uart_recv_blk(new_result, 10);
}

/**
 * @details Reads all five IR sensors using calibrated values and
 * estimates the position of a black line under the robot.
 *
 * For more information, please search send_line_position() and 0xB6
 * in http://www.pololu.com/docs/0J21/10.a
 */
uint16_t line_read_position()
{

    uint16_t result = 0;
    uint8_t* temp = (uint8_t*) &result;

    uart_send_byte(READ_LINE_POSITION);
    uart_recv_blk(temp, 2);

    /* Debug info */
    //dbg_printf("Line Pos recived: %X %X   ",temp[0],temp[1]);
    //dbg_printf("  Convert: %u\r\n",result);
    return result;
}

/**
 * @details This function sends a command byte to resets the line
 * sensor calibration data stored on Pololu
 */
void line_calibration_reset()
{
    uart_send_byte(RESET_CALIBRATION);
}


/**
 * @details Perform one round of calibration, then perform a calibrated
 * read. This are all done using Pololu's internal routines.
 *
 * For more information, please search 0xB4 in
 * http://www.pololu.com/docs/0J21/10.a
 */
void line_calibrate_read(uint16_t * result)
{
    uint8_t* new_result = (uint8_t*) result;
    uart_send_byte(CALIBRATE_AND_READ);
    uart_recv_blk(new_result, 10);
}

/**
 * @details This function cause the Pololu robot to
 * run through an automatic calibration sequence
 * for IR line sensors. It is described in:
 * http://www.pololu.com/docs/0J21/10.a
 *
 * How it works:
 * 1. Rotates right, calibrates sensors
 * 2. Rotates left, calibrates sensors
 * 3. Rotates right, calibrates sensors
 * 4. motors off, Sends confirm byte "c" on completion
 *
 * For information please search auto_calibrate() in
 * http://www.pololu.com/docs/0J21/10.a
 */
int line_autocalibrate()
{
    uint8_t read_buffer;
    uart_send_byte(AUTOCALIBRATE);

    /* The robot will return a single byte, when the calibration is
     * complete */
    if (uart_recv_blk(&read_buffer, 1)) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * @details This function checks if the 3rd line sensor (the central
 * one) is above the black line.
 * @return 0 for no line, 1 for line detected
 */
int line_detect()
{
    uint16_t result[5];
    line_read(result);
    if (result[2] <= 900) {
        return 1;
    } else {
        return 0;
    }

}
