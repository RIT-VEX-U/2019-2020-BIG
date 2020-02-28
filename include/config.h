#ifndef _CONFIG_
#define _CONFIG_

#include "hardware.h"
#include "utils/spline_path.hpp"

namespace config
{
    extern TankDrive::config_t drive_config;

    extern PID::config_t drive_pid_config;
    extern PID::config_t turn_pid_config;

    extern PID::config_t lift_pid_config;

    extern SplinePath::motion_profile_t motion_profile;

    void initialize();
}

#endif
