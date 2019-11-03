#ifndef _TANK_DRIVE_
#define _TANK_DRIVE_

#include "okapi/impl/device/motor/motorGroup.hpp"
#include "okapi/impl/util/timer.hpp"
#include <ctime>

class TankDrive
{
private:
  okapi::MotorGroup left_side;
  okapi::MotorGroup right_side;

  okapi::AbstractMotor::gearset gearset;

  float wheel_size;

  //Autonomous control variables
  //acceleration measured in seconds until full power
  //As in y=mx+b, deceleration = p * (target - current) + feedforward
  float drive_accel = .8;
  float drive_feedforward = .1;
  float drive_p = .5;

  bool initialize_func = false;
  okapi::Timer timer;
  double last_time;

public:

  /**
  Drive the robot with differential drive.
  Left axis is the Y axis on the left joystick, same for the right joystick.

  Takes into account that the joystick outputs between -127.0 and +127, and the
  motors input between -(min rpm) and +(max rpm) based on the gearset used.
  */
  void drive(int left_axis, int right_axis)
  {
    left_side.moveVelocity((left_axis / 127.0) * (int)gearset);
    right_side.moveVelocity((right_axis / 127.0) * (int)gearset);
  }

  /**
  Autonomously drive the robot x inches forward (or negative inches for reverse)
  at percent_speed (between 0.0 and 1.0).

  Will stop and return true after reaching the target.
  */
  bool drive_forward(double inches, double percent_speed)
  {
    if(initialize_func == false)
    {
      //Reset the class timer, reset absolute position to 0 on startup.
      last_time = timer.millis().getValue();
      left_side.tarePosition();
      right_side.tarePosition();
      initialize_func = true;
    }

    //Calculate acceleration between 0.0 and 1.0
    double acceleration = (timer.millis().getValue() - last_time) * (1 / drive_accel);
    acceleration = (acceleration > 1.0) ? 1.0 : acceleration;

    //Calculate p and feedforward for deceleration
    float deceleration = drive_feedforward + ((fabs(inches) - fabs(left_side.getPosition())) * drive_p);
    deceleration = (deceleration > 1.0) ? 1.0 : deceleration;

    float speed = (inches > 0 ? 1 : -1) * acceleration * deceleration * percent_speed * (int)gearset;

    pros::lcd::print(0, "inches: %f", inches / wheel_size);

    left_side.moveVelocity(speed);
    right_side.moveVelocity(speed);

    //The motors have finished moving
    if(fabs((inches / wheel_size) - left_side.getPosition()) < (.5 / wheel_size))
    {
      left_side.moveVelocity(0);
      right_side.moveVelocity(0);
      return true;
    }

    return false;
  }

  /**
  Create the tank drive object from two separate motor groups, what gearset they
  are using, and the size of the wheel.

  Initialize the drive encoders to measure in number of rotations, to make computing
  the distance traveled based on the wheel size easier.
  */
  TankDrive(okapi::MotorGroup left_side, okapi::MotorGroup right_side, okapi::AbstractMotor::gearset gearset, float wheel_size):
  left_side(left_side), right_side(right_side), gearset(gearset), wheel_size(wheel_size)
  {
    left_side.setGearing(gearset);
    right_side.setGearing(gearset);

    left_side.setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
    right_side.setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
  }

};

#endif
