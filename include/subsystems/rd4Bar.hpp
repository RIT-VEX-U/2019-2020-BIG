#ifndef _RD_4_BAR_
#define _RD_4_BAR_
#include "okapi/impl/device/motor/motorGroup.hpp"
#include "logging.hpp"

class rd4Bar{

private:
  //Two MotorGroups
  okapi::MotorGroup lift_motors1;
  okapi::MotorGroup lift_motors2;

  //The minimum motor velocity needed to hold the lift in a certain position
  int holdingPower;  //This is a guess, will need to be adjusted

public:
//Constructor
  rd4Bar(okapi::MotorGroup lift_motors1, okapi::MotorGroup lift_motors2, int holdingPower) : lift_motors1(lift_motors1), lift_motors2(lift_motors2){
    this -> holdingPower = holdingPower;
    lift_motors2.setReversed(true);
    lift_motors1.setReversed(false);
  }

//Set all motors to a positive velcity to raise the lift indefinitely
  void raise(int power){
    lift_motors1.moveVelocity(power);
    lift_motors2.moveVelocity(power);
  }

//Set all motors to a negative velocity to lower the lift indefinitely
  void lower(int power){
    power *= -1;
    lift_motors1.moveVelocity(power);
    lift_motors2.moveVelocity(power);
  }

//Set all motors to a zero velocity to stop the lift from moving
  void stop(){
    lift_motors1.moveVelocity(0);
    lift_motors2.moveVelocity(0);
  }

//Raise/Lower the lift to a specified point and have it keep that poisition
/*
  void moveTo(int encoderVal){
    lift_motors.moveAbsolute(encoderVal, 100);
    lift_motors.moveVelocity(holdingPower);
  }*/

//Checks if there is voltage going to the lift motors
  bool isMoving(){
    return lift_motors1.getVoltage() > 0 || lift_motors2.getVoltage() > 0;
  }

//Logging function
  void logLift(){
    logging::log(lift_motors1.getCurrentDraw(), lift_motors1.getPosition());
    logging::log(lift_motors2.getCurrentDraw(), lift_motors2.getPosition());
    //TODO: Log individual motors?
  }

};

#endif
