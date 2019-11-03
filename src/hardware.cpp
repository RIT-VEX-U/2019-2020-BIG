#include "hardware.h"
#include "api.h"
#include "pros/apix.h"

using namespace pros;

Motor Hardware::blank(30,false);

okapi::MotorGroup Hardware::left_drive = {1};
okapi::MotorGroup Hardware::right_drive = {3};

TankDrive Hardware::drive_system(Hardware::left_drive, Hardware::right_drive, okapi::AbstractMotor::gearset::green, 4.0);

okapi::MotorGroup Hardware::lift_motors = {5, -6, 7, -8};

rd4Bar Hardware::lift(Hardware::lift_motors, 20);

Motor Hardware::left_intake(8);
Motor Hardware::right_intake(9);

HorizIntake Hardware::horiz_intake(Hardware::left_intake, Hardware::right_intake);

Motor Hardware::liftRight(5,0);
Motor Hardware::liftLeft(6,0);

Motor Hardware::claw(7,0);

// Vision hw::camera;


Controller Hardware::master(E_CONTROLLER_MASTER);
