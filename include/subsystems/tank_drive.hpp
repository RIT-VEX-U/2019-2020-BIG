#ifndef _TANK_DRIVE_
#define _TANK_DRIVE_

#include "okapi/impl/device/motor/motorGroup.hpp"
#include "okapi/impl/util/timer.hpp"
#include "okapi/impl/device/rotarysensor/adiGyro.hpp"
#include <ctime>
#include "logging.hpp"

class TankDrive
{
private:
  okapi::MotorGroup left_side;
  okapi::MotorGroup right_side;

  okapi::AbstractMotor::gearset gearset;

  okapi::ADIGyro gyro;

  float wheel_size;

  //Autonomous control variables
  //As in y=mx+b, deceleration = p * (target - current) + (velocity * d)+ feedforward
  // drive_forward() variables
  float drive_feedforward = 0.0;
  float drive_p = .1;
  float drive_d = .05;
  float drive_standstill_time = .3;
  float drive_deadband = .1;

  // turn_degrees() variables
  float turn_feedforward = 0.0;
  float turn_p = .1;
  float turn_d = .05;
  float turn_last_angle = 0;
  float turn_deadband = 1.0;
  float turn_standstill_time = .3;

  // common variables
  bool initialize_func = false;
  bool is_checking_standstill = false;
  double last_time = 0;
  double last_time_2 = 0;

public:

  /**
  Drive the robot with differential drive.
  Left axis is the Y axis on the left joystick, same for the right joystick.

  Takes into account that the joystick outputs between -127.0 and +127, and the
  motors input between -(min rpm) and +(max rpm) based on the gearset used.
  */
  void drive(int left_axis, int right_axis);

  /**
  Autonomously drive the robot x inches forward (or negative inches for reverse)
  at percent_speed (between 0.0 and 1.0).

  Will stop and return true after reaching the target.
  */
  bool drive_forward(double inches, double percent_speed);

  /**
  Autonomously turn the robot x degrees, using feedback from a gyroscope

  For left turns, use negative degrees, for right, use positive.
  */
  bool turn_degrees(double degrees, double percent_speed);

  /**
  Create the tank drive object from two separate motor groups, what gearset they
  are using, and the size of the wheel.

  Initialize the drive encoders to measure in number of rotations, to make computing
  the distance traveled based on the wheel size easier.
  */
  TankDrive(okapi::MotorGroup left_side, okapi::MotorGroup right_side, okapi::AbstractMotor::gearset gearset, float wheel_size, okapi::ADIGyro gyro):
  left_side(left_side), right_side(right_side), gearset(gearset), wheel_size(wheel_size * 3.1415926), gyro(gyro)
  {
    left_side.setGearing(gearset);
    right_side.setGearing(gearset);

    left_side.setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
    right_side.setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
  }

};

#endif
