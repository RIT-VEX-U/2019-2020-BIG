#include "hardware.h"
#include "api.h"
#include "pros/apix.h"

/*
MOTOR PORTS:

Tank Drive:
  left: 20, 19
  right: 11, 12

Reverse Double 4 Bar:
  left bottom: 9
  left top: 10
  right bottom: 1
  right top: 2

Intake:
  left: 9
  right: 10

*/

using namespace pros;

okapi::ADIGyro Hardware::gyro(1, .1);

pros::Motor Hardware::left_front(20, false);
pros::Motor Hardware::right_front(11, true);
pros::Motor Hardware::left_rear(19, false);
pros::Motor Hardware::right_rear(12, true);

TankDrive Hardware::drive_system(Hardware::left_front, Hardware::right_front, Hardware::left_rear, Hardware::right_rear, pros::E_MOTOR_GEARSET_18, &Hardware::gyro, &config::drive_config, &config::drive_pid_config, &config::turn_pid_config);

okapi::Motor Hardware::lift1(9);
okapi::Motor Hardware::lift2(10);
okapi::Motor Hardware::lift3(1);
okapi::Motor Hardware::lift4(2);

rd4Bar Hardware::lift({lift1, lift4}, {lift2, lift3}, 50, &config::lift_pid_config);  //6 & 7 Reversed

Motor Hardware::left_intake(3);
Motor Hardware::right_intake(4);

HorizIntake Hardware::horiz_intake(Hardware::left_intake, Hardware::right_intake);

Motor Hardware::v_intake1(5);
Motor Hardware::v_intake2(6);
Motor Hardware::intake_door(7);

VerticalIntake Hardware::vert_intake(Hardware::v_intake1, Hardware::v_intake2, Hardware::intake_door);

Controller Hardware::master(E_CONTROLLER_MASTER);
