#include "hardware.h"

Motor Hardware::blank(30,false);

okapi::MotorGroup Hardware::left_drive = {1};
okapi::MotorGroup Hardware::right_drive = {3};

TankDrive Hardware::drive_system(Hardware::left_drive, Hardware::right_drive, okapi::AbstractMotor::gearset::green, 4.0);

Motor Hardware::left_intake(8);
Motor Hardware::right_intake(9);

HorizIntake Hardware::horiz_intake(Hardware::left_intake, Hardware::right_intake);

Motor Hardware::liftRight(5,0);
Motor Hardware::liftLeft(6,0);

Motor Hardware::claw(7,0);

// Vision hw::camera;


Controller Hardware::master(E_CONTROLLER_MASTER);
