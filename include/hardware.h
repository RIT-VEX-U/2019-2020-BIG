#ifndef HARDWARE
#define HARDWARE
#include "api.h"
#include "subsystems/tank_drive.hpp"
#include "subsystems/rd4Bar.hpp"
#include "subsystems/horiz_intake.hpp"
#include "subsystems/vertical_intake.hpp"
#include "pros/apix.h"
#include "okapi/impl/device/motor/motorGroup.hpp"
#include "okapi/impl/device/rotarysensor/adiGyro.hpp"
#include "config.h"

using namespace pros;

namespace Hardware
{
  extern pros::ADIDigitalOut solenoid;

  extern pros::ADIButton limit_switch;

  extern pros::ADIButton right_button;
  extern pros::ADIButton left_button;

  extern pros::Motor left_front;
  extern pros::Motor right_front;
  extern pros::Motor left_middle;
  extern pros::Motor right_middle;
  extern pros::Motor left_rear;
  extern pros::Motor right_rear;

  extern okapi::ADIGyro gyro;
  extern pros::Imu imu;

  extern TankDrive drive_system;

  extern okapi::Motor lift1;
  extern okapi::Motor lift2;
  extern okapi::Motor lift3;
  extern okapi::Motor lift4;

  extern rd4Bar lift;

  extern Motor left_intake;
  extern Motor right_intake;

  extern HorizIntake horiz_intake;

  //extern Motor v_intake1;
  //extern Motor v_intake2;
  //extern Motor intake_door;

  extern VerticalIntake vert_intake;
  // static Vision camera;

  extern Controller master;
  extern Controller partner;
};
#endif
