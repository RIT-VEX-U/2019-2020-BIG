#include "config.h"

TankDrive::config_t config::drive_config = {};
PID::config_t config::drive_pid_config = {};
PID::config_t config::turn_pid_config = {};

void config::initialize()
{
    // drive_forward() config
    drive_pid_config.feedforward = 0;
    drive_pid_config.p= .1;
    drive_pid_config.d= .1;
    drive_pid_config.on_target_time = .3;
    drive_pid_config.deadband = .1;  

    // turn_degrees() config
    turn_pid_config.feedforward = 0;
    turn_pid_config.p = .005;
    turn_pid_config.i = .0001;
    turn_pid_config.d = .0001;
    turn_pid_config.deadband = .2;
    turn_pid_config.on_target_time = .3;

    //      Drive System Configuration
    drive_config.wheel_size = 4;
    drive_config.omni_config = TankDrive::omni_config_t::center_dr;

    drive_config.drive_pid = &drive_pid_config;
    drive_config.turn_pid = &turn_pid_config;


}