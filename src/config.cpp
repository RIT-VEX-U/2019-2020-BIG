#include "config.h"

TankDrive::config_t config::drive_config = {};

void config::initialize()
{
    //      Drive System Configuration
    drive_config.wheel_size = 4;

    // drive_forward() config
    drive_config.drive_feedforward = 0;
    drive_config.drive_p = .1;
    drive_config.drive_d = .1;
    drive_config.drive_standstill_time = .3;
    drive_config.drive_deadband = .1;

    // turn_degrees() config
    drive_config.turn_feedforward = 0;
    drive_config.turn_p = .1;
    drive_config.turn_d = 0;
    drive_config.turn_deadband = .5;
    drive_config.turn_standstill_time = .3;

}