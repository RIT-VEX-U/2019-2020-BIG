#ifndef HARDWARE
#define HARDWARE
#include "api.h"
#include "subsystems/tank_drive.hpp"
#include "pros/apix.h"
#include "okapi/impl/device/motor/motorGroup.hpp"

using namespace pros;

namespace Hardware
{
  extern okapi::MotorGroup left_drive;
  extern okapi::MotorGroup right_drive;

  extern TankDrive drive_system;

  extern Motor liftR1;
  extern Motor liftL1;
  extern Motor liftR2;
  extern Motor liftL2;
  extern Motor claw;

  // static Vision camera;

  extern Controller master;
  extern Motor blank;
};
#endif
