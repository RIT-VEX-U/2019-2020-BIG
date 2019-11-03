#ifndef _RD_4_BAR_
#define _RD_4_BAR_
#include "okapi/impl/device/motor/motorGroup.hpp"
#include "logging.hpp"

class rd4Bar{

private:
  //One MotorGroup since all the motors always move together
  okapi::MotorGroup lift_motors;

  //The minimum motor velocity needed to hold the lift in a certain position
  int holdingPower;  //This is a guess, will need to be adjusted

public:
//Constructor
  rd4Bar(okapi::MotorGroup lift_motors, int holdingPower) : lift_motors(lift_motors){
    this -> holdingPower = holdingPower;
  }

//Set all motors to a positive velcity (127) to raise the lift indefinitely
  void raise(int power){
    lift_motors.moveVelocity(power);
  }

//Set all motors to a negative velocity (-127) to lower the lift indefinitely
  void lower(int power){
    lift_motors.moveVelocity(-power);
  }

//Set all motors to a zero velocity to stop the lift from moving
  void stop(){
    lift_motors.moveVelocity(0);
  }

//Raise/Lower the lift to a specified point and have it keep that poisition
  void moveTo(int encoderVal){
    lift_motors.moveAbsolute(encoderVal, 100);
    lift_motors.moveVelocity(holdingPower);
  }

//Checks if there is voltage going to the lift motors
  bool isMoving(){
    return lift_motors.getVoltage() > 0;
  }

//Logging function
  void logLift(){
    logging::log(lift_motors.getCurrentDraw(), lift_motors.getPosition());
    //TODO: Log individual motors?
  }

};

#endif
