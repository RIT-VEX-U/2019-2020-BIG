#include "hardware.h"
#include "api.h"
#include "pros/apix.h"

using namespace pros;

Motor hw::blank(30,false);

Motor hw::driveBackLeft(1,0);
Motor hw::driveBackRight(2,0);
Motor hw::driveFrontLeft(3,0);
Motor hw::driveFrontRight(4,0);

Motor hw::liftRight(5,0);
Motor hw::liftLeft(6,0);

Motor hw::claw(7,0);

// Vision hw::camera;


Controller hw::master(E_CONTROLLER_MASTER);
