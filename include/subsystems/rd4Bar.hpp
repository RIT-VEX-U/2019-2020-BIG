/**
* Purpose: Includes all the functionality for a reverse
*   double four bar
* Usage: include the hardware file where the lift
    is declared, Hardware::[name of rd4Bar].[function]
* @author Dana Colletti
*/
#ifndef _RD_4_BAR_
#define _RD_4_BAR_
#include "okapi/impl/device/motor/motorGroup.hpp"
#include "logging.hpp"
#include "utils/pid.hpp"

class rd4Bar{

private:
  //All lift motors, grouped by the direction they spin
  okapi::MotorGroup lift_motors1;
  okapi::MotorGroup lift_motors2;

  //Pointer to the
  PID *lift_pid;

  //Position it is currently holding, -1 if none
  float holdingPos = 1;

  //The motor velocity needed to hold the lift in a certain position
  int holdingPower;



public:
//Constructor
  rd4Bar(okapi::MotorGroup lift_motors1, okapi::MotorGroup lift_motors2, int holdingPower, PID::config_t *lift_pid_config) : lift_motors1(lift_motors1), lift_motors2(lift_motors2){
    this -> holdingPower = holdingPower;
    lift_pid = new PID(lift_pid_config);
    lift_motors2.setReversed(false);
    lift_motors1.setReversed(true);
    lift_motors1.setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
    lift_motors2.setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
  }

//Set all motors to a positive velcity to raise the lift indefinitely
  void raise(int power){
    lift_motors1.moveVoltage(power);
    lift_motors2.moveVoltage(power);
  }

//Set all motors to a negative velocity to lower the lift indefinitely
  void lower(int power){
    power *= -1;
    lift_motors1.moveVoltage(power);
    lift_motors2.moveVoltage(power);
  }

//Set all motors to a zero velocity to stop the lift from moving
  void stop(){
    lift_motors1.moveVoltage(0);
    lift_motors2.moveVoltage(0);
  }

//Raise/Lower the lift to a specified point and have it keep that poisition
  void moveTo(float encoderVal, bool hold){

    //While it is too high, lower it
    while(fabs(lift_motors1.getPosition()) > encoderVal){
      lift_motors1.moveVoltage(12000);
      lift_motors2.moveVoltage(12000);
    }

    //While it is too low, raise it
    while(fabs(lift_motors1.getPosition()) < encoderVal){
      lift_motors1.moveVoltage(-12000);
      lift_motors2.moveVoltage(-12000);
    }

    //if the lift should maintain this position
    if(hold){
      holdingPos = encoderVal;
      hold_pos(holdingPos);
    }
    else{
      lift_motors1.moveVelocity(0);
      lift_motors2.moveVelocity(0);
    }
  }

  int hold_pos(float newHoldingPos){
    if(holdingPos != newHoldingPos){
      holdingPos = newHoldingPos;
      lift_pid->reset();
    }
    lift_pid -> set_target(newHoldingPos);
    lift_pid -> update(lift_motors1.getPosition());
    int power = lift_pid -> get();
    pros::lcd::print(0, "Error: %f", lift_pid->get_error());
    lift_motors1.moveVoltage(power);
    lift_motors2.moveVoltage(power);
    return power;
  }

  bool is_holding(){ return holdingPos > -1; }

  float getHoldingPos(){ return holdingPos; }

  float getCurrPos(){ return lift_motors1.getPosition(); }

  void release_hold(){
    holdingPos = -1;
  }

  //Logging function
  void log_lift(){
      while(true){
        logging::log(lift_motors1.getCurrentDraw(), lift_motors1.getPosition());
        logging::log(lift_motors2.getCurrentDraw(), lift_motors2.getPosition());
        pros::delay(50);
        //TODO: Log individual motors?
      }
    }


//TODO: broken??
//Checks if there is voltage going to the lift motors
  bool isMoving(){
    return lift_motors1.getVoltage() > 0 || lift_motors2.getVoltage() > 0;
  }

  float getEncoderVal(){
    return lift_motors1.getPosition();
  }

};

#endif
