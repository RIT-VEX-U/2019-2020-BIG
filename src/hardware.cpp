#include "api.h"
#include "hardware.h"

using namespace pros;

Motor blank(30,false);

Motor driveBackLeft(1,0);
Motor driveBackRight(2,0);
Motor driveFrontLeft(3,0);
Motor driveFrontRight(4,0);

Motor liftRight(5,0);
Motor liftLeft(6,0);

Motor claw(7,0);

Vision camera();

Controller master(E_CONTROLLER_MASTER);
