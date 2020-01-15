#include "config.h"

TankDrive::config_t config::drive_config = {};
PID::config_t config::drive_pid_config = {};
PID::config_t config::turn_pid_config = {};
PID::config_t config::lift_pid_config = {};

void config::initialize()
{
    // drive_forward() config
    drive_pid_config.feedforward = 0;
    drive_pid_config.p= .1;
    drive_pid_config.i = .000;
    drive_pid_config.d= .000;
    drive_pid_config.on_target_time = .3;
    drive_pid_config.deadband = .5;

    // turn_degrees() config
    turn_pid_config.feedforward = 0;
    turn_pid_config.p = .01;
    turn_pid_config.i = .0000;
    turn_pid_config.d = .0003;
    turn_pid_config.deadband = .8;
    turn_pid_config.on_target_time = .3;

    lift_pid_config.feedforward = 2000;
    lift_pid_config.p = 30000;
    lift_pid_config.deadband = 0.02;
    lift_pid_config.on_target_time = 0.3;

    //      Drive System Configuration
    drive_config.wheel_size = 4;
    drive_config.omni_config = TankDrive::omni_config_t::center_dr;
}
