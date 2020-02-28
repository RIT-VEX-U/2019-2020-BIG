#include "config.h"

TankDrive::config_t config::drive_config = {};
PID::config_t config::drive_pid_config = {};
PID::config_t config::turn_pid_config = {};
PID::config_t config::lift_pid_config = {};
SplinePath::motion_profile_t config::motion_profile = {};

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
    turn_pid_config.p = .011;
    turn_pid_config.i = .000;
    turn_pid_config.d = .001;
    turn_pid_config.deadband = 1.5;
    turn_pid_config.on_target_time = .3;

    lift_pid_config.feedforward = 2000;
    lift_pid_config.p = 4000;
    //lift_pid_config.i = 40;
    lift_pid_config.deadband = 0.02;
    lift_pid_config.on_target_time = 0.1;

    //      Drive System Configuration
    drive_config.wheel_size = 4;
    drive_config.omni_config = TankDrive::omni_config_t::center_dr;

    motion_profile.drive_p = .01;
    motion_profile.drive_i = 0;
    motion_profile.drive_d = 0;
    motion_profile.turn_p = 0;//.005;

    motion_profile.max_v = 6;
    motion_profile.max_a = 5;

    motion_profile.kv = 1.0/6.0;
    motion_profile.ka = 0;

    motion_profile.dt = .02;

}
