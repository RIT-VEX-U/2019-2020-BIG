#ifndef _TANK_DRIVE_
#define _TANK_DRIVE_

#include "okapi/impl/device/motor/motorGroup.hpp"
#include "logging.hpp"

class TankDrive
{
private:
  okapi::MotorGroup left_side;
  okapi::MotorGroup right_side;

  okapi::AbstractMotor::gearset gearset;

public:

  void drive(int left_axis, int right_axis)
  {
    int multiplier = (gearset == okapi::AbstractMotor::gearset::red) ? 100 : (gearset == okapi::AbstractMotor::gearset::green) ? 200 : 600;

    left_side.moveVelocity((left_axis / 127.0) * multiplier);
    right_side.moveVelocity((right_axis / 127.0) * multiplier);
  }

//Logging function
  void logDrive(){
    logging::log(left_side.getCurrentDraw(), left_side.getPosition());
    logging::log(right_side.getCurrentDraw(), right_side.getPosition());
  }

  TankDrive(okapi::MotorGroup left_side, okapi::MotorGroup right_side, okapi::AbstractMotor::gearset gearset):
  left_side(left_side), right_side(right_side), gearset(gearset)
  {
    left_side.setGearing(gearset);
    right_side.setGearing(gearset);
  }

};

#endif
