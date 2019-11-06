#include "hardware.h"
#include "api.h"
#include "pros/apix.h"

/*
MOTOR PORTS:

Tank Drive:
  left: 1, 2
  right: 3, 4

Reverse Double 4 Bar:
  left bottom: 5
  left top: 6
  right bottom: 7
  right top: 8

Intake:
  left: 9
  right: 10

*/

using namespace pros;

Motor Hardware::blank(30,false);

//All the motors controlling the left & right sides of the drive train
okapi::MotorGroup Hardware::left_drive = {1, 2};
okapi::MotorGroup Hardware::right_drive = {3, 4};
//The
TankDrive Hardware::drive_system(Hardware::left_drive, Hardware::right_drive, okapi::AbstractMotor::gearset::green);

//Lift motors separated based on reversal
//okapi::MotorGroup Hardware::lift_motors1 = {6, 8};
//okapi::MotorGroup Hardware::lift_motors2 = {5, 7};  //reversed

rd4Bar Hardware::lift({6, 8}, {5, 7}, 20);

Motor Hardware::left_intake(9);
Motor Hardware::right_intake(10);

HorizIntake Hardware::horiz_intake(Hardware::left_intake, Hardware::right_intake);

Motor Hardware::claw(11,0);

Controller Hardware::master(E_CONTROLLER_MASTER);
