#include "hardware.h"
#include "api.h"
#include "pros/apix.h"

using namespace pros;

Motor Hardware::blank(30,false);

//All the motors controlling the left & right sides of the drive train
okapi::MotorGroup Hardware::left_drive = {1, 2};
okapi::MotorGroup Hardware::right_drive = {3, 4};
//The
TankDrive Hardware::drive_system(Hardware::left_drive, Hardware::right_drive, okapi::AbstractMotor::gearset::green);

//All the motors controlling the lift
okapi::MotorGroup Hardware::lift_motors = {5, -6, 7, -8};

rd4Bar Hardware::lift(Hardware::lift_motors, 20);

Motor Hardware::claw(7,0);

// Vision hw::camera;


Controller Hardware::master(E_CONTROLLER_MASTER);
