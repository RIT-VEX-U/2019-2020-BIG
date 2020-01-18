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

Motor Hardware::left_front(20, false);
Motor Hardware::right_front(12, true);
Motor Hardware::left_rear(19, false);
Motor Hardware::right_rear(11, true);

TankDrive Hardware::drive_system(Hardware::left_front, Hardware::right_front, Hardware::left_rear, Hardware::right_rear, pros::E_MOTOR_GEARSET_18, &Hardware::gyro, &config::drive_config, &config::drive_pid_config, &config::turn_pid_config);

okapi::Motor Hardware::lift1(-10);
okapi::Motor Hardware::lift2(9);
okapi::Motor Hardware::lift3(-2);
okapi::Motor Hardware::lift4(1);

rd4Bar Hardware::lift({lift1, lift4}, {lift2, lift3}, 50, &config::lift_pid_config);  //6 & 7 Reversed

Motor Hardware::left_intake(13);
Motor Hardware::right_intake(14);

HorizIntake Hardware::horiz_intake(Hardware::left_intake, Hardware::right_intake);

Motor Hardware::v_intake1(17,0);
Motor Hardware::v_intake2(18,0);

Motor Hardware::intake_door(16,0);

VerticalIntake Hardware::vert_intake(Hardware::v_intake1, Hardware::v_intake2, Hardware::intake_door);

Controller Hardware::master(E_CONTROLLER_MASTER);
Controller Hardware::partner(E_CONTROLLER_PARTNER);
