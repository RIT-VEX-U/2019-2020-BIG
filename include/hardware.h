#ifndef HARDWARE
#define HARDWARE
#include "api.h"
#include "pros/apix.h"

using namespace pros;

  class hw
  {
  public:

    static Motor driveBackLeft;
    static Motor driveBackRight;
    static Motor driveFrontLeft;
    static Motor driveFrontRight;

    static Motor liftR1;
    static Motor liftL1;
    static Motor liftR2;
    static Motor liftL2;

    static Motor claw;

    // static Vision camera;

    static Controller master;

    static Motor blank;
  };
#endif
