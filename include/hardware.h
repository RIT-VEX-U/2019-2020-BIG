#ifndef HARDWARE
#define HARDWARE
#include "api.h"

using namespace pros;

  class hw
  {
  public:

    static Motor driveBackLeft;
    static Motor driveBackRight;
    static Motor driveFrontLeft;
    static Motor driveFrontRight;

    static Motor liftRight;
    static Motor liftLeft;

    static Motor claw;

    static Vision camera;

    static Controller master;

    static Motor blank;
  };
#endif
