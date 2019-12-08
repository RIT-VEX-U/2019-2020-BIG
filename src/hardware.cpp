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

okapi::ADIGyro Hardware::gyro(1, .1);

Motor Hardware::left_front(1, false);
Motor Hardware::right_front(3, true);
Motor Hardware::left_rear(2, false);
Motor Hardware::right_rear(4, true);

TankDrive Hardware::drive_system(Hardware::left_front, Hardware::right_front, Hardware::left_rear, Hardware::right_rear, pros::E_MOTOR_GEARSET_18, &Hardware::gyro, &config::drive_config, &config::drive_pid_config, &config::turn_pid_config);

okapi::Motor Hardware::lift1(5);
okapi::Motor Hardware::lift2(6);
okapi::Motor Hardware::lift3(7);
okapi::Motor Hardware::lift4(8);

rd4Bar Hardware::lift({lift1, lift4}, {lift2, lift3}, 50);  //6 & 7 Reversed

Motor Hardware::left_intake(9);
Motor Hardware::right_intake(10);

HorizIntake Hardware::horiz_intake(Hardware::left_intake, Hardware::right_intake);

Motor Hardware::liftRight(11,0);
Motor Hardware::liftLeft(12,0);


Controller Hardware::master(E_CONTROLLER_MASTER);
