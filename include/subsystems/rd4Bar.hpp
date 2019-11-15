#ifndef _RD_4_BAR_
#define _RD_4_BAR_
#include "okapi/impl/device/motor/motorGroup.hpp"
#include "logging.hpp"

class rd4Bar{

private:
  //Two MotorGroups
  okapi::MotorGroup lift_motors1;
  okapi::MotorGroup lift_motors2;

  //Position it is currently holding, -1 if none
  float holdingPos = -1;

  //The minimum motor velocity needed to hold the lift in a certain position
  int holdingPower;  //This is a guess, will need to be adjusted

public:
//Constructor
  rd4Bar(okapi::MotorGroup lift_motors1, okapi::MotorGroup lift_motors2, int holdingPower) : lift_motors1(lift_motors1), lift_motors2(lift_motors2){
    this -> holdingPower = holdingPower;
    lift_motors2.setReversed(false);
    lift_motors1.setReversed(true);
    lift_motors1.setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
    lift_motors2.setEncoderUnits(okapi::AbstractMotor::encoderUnits::rotations);
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
  void moveTo(float encoderVal, bool hold){
    while(fabs(lift_motors1.getPosition()) < encoderVal){
      lift_motors1.moveVelocity(-100);
      lift_motors2.moveVelocity(-100);
    }
    lift_motors1.moveVelocity(holdingPower);
    lift_motors2.moveVelocity(holdingPower);

    if(hold){
      holdingPos = encoderVal;
      hold_pos(encoderVal);
    }
  }

  void moveHigh(){
    moveTo(highVal, true);
  }

  void moveLow(){
    moveTo(lowVal, true);
  }

  void hold_pos(float pos){
    if(lift_motors1.getPosition() > pos && lift_motors2.getPosition() > pos){
      lift_motors1.moveVoltage(lift_motors1.getVoltage() - 1);
      lift_motors2.moveVoltage(lift_motors2.getVoltage() - 1);
    }
    else if(lift_motors1.getPosition() < pos && lift_motors2.getPosition() < pos){
      lift_motors1.moveVoltage(lift_motors1.getVoltage() + 1);
      lift_motors2.moveVoltage(lift_motors2.getVoltage() + 1);
    }
  }

  //void start_hold_pos(int pos){
    //pros::Task hold_pos_task(void (*hold_pos)(int));
  //}

//  bool isDown(){
//  }

  //Logging function
  void log_lift(){
      while(true){
        logging::log(lift_motors1.getCurrentDraw(), lift_motors1.getPosition());
        logging::log(lift_motors2.getCurrentDraw(), lift_motors2.getPosition());
        pros::delay(50);
        //TODO: Log individual motors?
      }
    }

//Checks if there is voltage going to the lift motors
  bool isMoving(){
    return lift_motors1.getVoltage() > 0 || lift_motors2.getVoltage() > 0;
  }

  float getEncoderVal(){
    return lift_motors1.getPosition();
  }

  float getHoldingPos(){ return holdingPos; }

  /*void setZero(){
    lift_motors1.setZeroPosition();
  }*/

};

/*  void start_log(rd4Bar lift){
    pros::Motor m1(5);
    pros::Motor* m1_ptr = &m1;
    //pros::task_t logLift = task_create(log_lift());
    //pros::Task log_lift_task = new pros::Task(lift.log_lift);
  }*/

#endif
