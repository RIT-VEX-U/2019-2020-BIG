#ifndef HARDWARE
#define HARDWARE
#include "api.h"
#include "subsystems/tank_drive.hpp"
#include "subsystems/rd4Bar.hpp"
#include "subsystems/horiz_intake.hpp"
#include "pros/apix.h"
#include "okapi/impl/device/motor/motorGroup.hpp"

using namespace pros;

namespace Hardware
{
  extern okapi::MotorGroup left_drive;
  extern okapi::MotorGroup right_drive;

  extern TankDrive drive_system;

  extern okapi::MotorGroup lift_motors;

  extern rd4Bar lift;

  extern Motor left_intake;
  extern Motor right_intake;

  extern HorizIntake horiz_intake;

  extern Motor liftRight;
  extern Motor liftLeft;
  extern Motor claw;

  // static Vision camera;

  extern Controller master;
  extern Motor blank;
};
#endif
