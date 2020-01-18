/**
 * @file wall_follow.c
 * @author Oliver Loftus
 *
 * Contains wall following algorithms
 */


#include "wall_follow.h"
#include "ir.h"
#include "motor.h"
#include "delay.h"
#include "gpio.h"


#define TOLERANCE         5    // Tolerance in mm
#define WALL_DISTANCE     200  // Distance to stay from the wall in mm
#define DELAY             100  // Desired delay in ms
#define OBSTACLE          0.3  // Percentage difference to detect an obstacle

short int WALL_FOLLOW_FLAG = 1;



int wallFollowing = 0;

ir_Sensor fSensor, bSensor;
int fReading, bReading;
int prevFReading, prevBReading;


static void update_sensors()
{
  fReading = ir_read(fSensor);
  bReading = ir_read(bSensor);
}

void nav_follow_wall(nav_Wall wall)
{
    if(!WALL_FOLLOW_FLAG)
    {
        return;
    }


  fSensor = wall == WALL_RIGHT ? FRONT_RIGHT : FRONT_LEFT;
  bSensor = wall == WALL_RIGHT ? BACK_RIGHT : BACK_LEFT;

  int speed = motor_read(MOTOR_LEFT);
  int pTurnSpeed = speed*0.6;
  int dTurnSpeed = speed*0.3;
  int delay = (DELAY*10)/speed;

  motors_move(speed); // Start the robot running straight

  wallFollowing = 1;
  while (wallFollowing)
  {
    /***** Staying parallel to the wall/correcting small errors *****/
    update_sensors();
    if (fReading - bReading > TOLERANCE || bReading - fReading > TOLERANCE) // fSensorReading != bSensorReading
    {
      if ((wall == WALL_RIGHT && fReading - bReading > TOLERANCE) || // fReading > bReading
          (wall == WALL_LEFT && bReading - fReading > TOLERANCE)) // fReading < bReading
      {
        motor_set(MOTOR_RIGHT, pTurnSpeed);
      }
      else if ((wall == WALL_RIGHT && bReading - fReading > TOLERANCE) || // fReading < bReading
               (wall == WALL_LEFT && fReading - bReading > TOLERANCE)) // fReading > bReading
      {
        motor_set(MOTOR_LEFT, pTurnSpeed);
      }

      do { update_sensors(); }
      while (fReading - bReading > TOLERANCE || bReading - fReading > TOLERANCE); // fSensorReading != bSensorReading
      motors_move(speed);
    }


    /***** Navigating to the correct distance from the wall *****/
    update_sensors();
    if ((wall == WALL_RIGHT && WALL_DISTANCE - fReading > TOLERANCE) || // Too close
        (wall == WALL_LEFT && fReading - WALL_DISTANCE > TOLERANCE))
    {
      motor_set(MOTOR_LEFT, dTurnSpeed);
      delay_start(delay);
      motor_set(MOTOR_LEFT, speed); // Go in a straight line
    }
    else if ((wall == WALL_RIGHT && fReading - WALL_DISTANCE > TOLERANCE) || // Too far
             (wall == WALL_LEFT && WALL_DISTANCE - fReading > TOLERANCE))
    {
      motor_set(MOTOR_RIGHT, dTurnSpeed);
      delay_start(delay);
      motor_set(MOTOR_RIGHT, speed);
    }
    delay_start(delay); // Wait for the robot to go a little before rechecking


    ///***** Detect a wall ahead *****/
    if (gpio_collision_read())
    {
      motors_stop();
      while (gpio_collision_read()) delay_start(10);
      motors_move(speed);
    }
  }
}

void nav_stop_follow_wall()
{
  wallFollowing = 0;
}


void follow_wall_start()
{
    WALL_FOLLOW_FLAG = 1;
}

void follow_wall_stop()
{
    nav_stop_follow_wall();
    WALL_FOLLOW_FLAG = 0;
}
