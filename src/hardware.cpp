#include "hardware.h"
#include "api.h"
#include "pros/apix.h"

using namespace pros;

Motor Hardware::blank(30,false);

TankDrive Hardware::drive_system(Hardware::left_drive, Hardware::right_drive, okapi::AbstractMotor::gearset::green);

Motor Hardware::liftR1(5,0);
Motor Hardware::liftL1(6,0);
Motor Hardware::liftR2(7,0);
Motor Hardware::liftL2(8,0);

Motor Hardware::claw(7,0);

// Vision hw::camera;


Controller Hardware::master(E_CONTROLLER_MASTER);
