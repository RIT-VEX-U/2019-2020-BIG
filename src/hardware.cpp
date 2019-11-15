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

okapi::MotorGroup Hardware::left_drive = {1};
okapi::MotorGroup Hardware::right_drive = {3};

TankDrive Hardware::drive_system(Hardware::left_drive, Hardware::right_drive, okapi::AbstractMotor::gearset::green, 4.0);

okapi::Motor Hardware::lift1(5);
okapi::Motor Hardware::lift2(6);
okapi::Motor Hardware::lift3(7);
okapi::Motor Hardware::lift4(8);

rd4Bar Hardware::lift({lift1, lift4}, {lift2, lift3}, -1);  //6 & 7 Reversed

Motor Hardware::left_intake(9);
Motor Hardware::right_intake(10);

HorizIntake Hardware::horiz_intake(Hardware::left_intake, Hardware::right_intake);

Motor Hardware::claw(11,0);

Controller Hardware::master(E_CONTROLLER_MASTER);
