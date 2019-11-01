#include "hardware.h"

Motor Hardware::blank(30,false);

okapi::MotorGroup Hardware::left_drive = {1, 2};
okapi::MotorGroup Hardware::right_drive = {3, 4};

TankDrive Hardware::drive_system(Hardware::left_drive, Hardware::right_drive, okapi::AbstractMotor::gearset::green);

Motor Hardware::liftRight(5,0);
Motor Hardware::liftLeft(6,0);

Motor Hardware::claw(7,0);

// Vision hw::camera;


Controller Hardware::master(E_CONTROLLER_MASTER);
