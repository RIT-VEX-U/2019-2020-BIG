#ifndef _CONFIG_
#define _CONFIG_

#include "hardware.h"

namespace config
{
    extern TankDrive::config_t drive_config;

    extern PID::config_t drive_pid_config;
    extern PID::config_t turn_pid_config;

    void initialize();
}

#endif