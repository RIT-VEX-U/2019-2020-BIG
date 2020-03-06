#include "hardware.h"
#include "api.h"
#include "pros/apix.h"

/*
MOTOR PORTS:

Tank Drive:
  left Front: 20
  Left Back: 19
  Right Front: 11, reversed
  Right Back: 12, reversed

Reverse Double 4 Bar:
  left bottom: 9 reversed
  left top: 10,
  right bottom: 1
  right top: 2, reversed

Intake:
  Horizontal left: 13
  Horizontal right: 14

  Vertical: 17, 18
  Door: 16

SENSOR PORTS:

Gyro: 1
Limit Switch: H
Right Bottom Button: G
Left Bottom Button: F
*/

using namespace pros;

okapi::ADIGyro Hardware::gyro(1, .1);

pros::ADIDigitalOut Hardware::solenoid('H');
pros::Imu Hardware::imu(13);

ADIButton Hardware::limit_switch('B');
// ADIAnalogIn Hardware::line_tracker('C');

ADIButton Hardware::right_button('G');
ADIButton Hardware::left_button('F');

Motor Hardware::left_front(20, false);
Motor Hardware::right_front(12, true);
Motor Hardware::left_middle(8, false);
Motor Hardware::right_middle(7, true);
Motor Hardware::left_rear(19, false);
Motor Hardware::right_rear(11, true);

TankDrive Hardware::drive_system(Hardware::left_front, Hardware::right_front, Hardware::left_middle, Hardware::right_middle, Hardware::left_rear, Hardware::right_rear, pros::E_MOTOR_GEARSET_18, &Hardware::imu, &config::drive_config, &config::drive_pid_config, &config::turn_pid_config);

okapi::Motor Hardware::lift1(10);
okapi::Motor Hardware::lift2(9);
okapi::Motor Hardware::lift3(2);
okapi::Motor Hardware::lift4(1);

rd4Bar Hardware::lift({lift1, lift4}, {lift2, lift3}, 50, &config::lift_pid_config);  //6 & 7 Reversed

Motor Hardware::left_intake(15);
Motor Hardware::right_intake(14);

Motor Hardware::intake_puller(16);

HorizIntake Hardware::horiz_intake(Hardware::left_intake, Hardware::right_intake, Hardware::intake_puller);

//Motor Hardware::v_intake1(17,0);
//Motor Hardware::v_intake2(18,0);

//Motor Hardware::intake_door(16,0);

VerticalIntake Hardware::vert_intake(Hardware::solenoid);

Controller Hardware::master(E_CONTROLLER_MASTER);
Controller Hardware::partner(E_CONTROLLER_PARTNER);
