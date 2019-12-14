#include "hardware.h"
#include "api.h"
#include "pros/apix.h"

//using namespace pros;

okapi::ADIGyro Hardware::gyro(1, .1);

pros::Motor Hardware::left_front(1, false);
pros::Motor Hardware::right_front(3, true);
pros::Motor Hardware::left_rear(2, false);
pros::Motor Hardware::right_rear(4, true);

TankDrive Hardware::drive_system(Hardware::left_front, Hardware::right_front, Hardware::left_rear, Hardware::right_rear, pros::E_MOTOR_GEARSET_18, &Hardware::gyro, &config::drive_config, &config::drive_pid_config, &config::turn_pid_config);

rd4Bar Hardware::lift({5, -6}, {7, -8}, 20);

Motor Hardware::left_intake(8);
Motor Hardware::right_intake(9);

HorizIntake Hardware::horiz_intake(Hardware::left_intake, Hardware::right_intake);



Controller Hardware::master(E_CONTROLLER_MASTER);
