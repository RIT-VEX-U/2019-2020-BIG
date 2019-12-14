#ifndef _VERTICAL_INTAKE_
#define _VERTICAL_INTAKE_
#include "pros/motors.hpp"
class VerticalIntake{
private:
  //
  pros::Motor left, right, door;

public:
  //Constructor
  VerticalIntake(pros::Motor left, pros::Motor right, pros::Motor door):left(left), right(right), door(door){

  }

  //suck in a cube
  void takeIn(){
    left.move(200);
    right.move(200);
  }

  //drop a cube
  void drop(){
    left.move(-200);
    right.move(-200);
  }

  //open the door of the intake
  void open(){
    while(door.get_position() < 0){ //!!! WILL NEED ADJUSTMENT
      door.move(200);
    }
    door.move(0);
  }

  //close the door of the intake
  void close(){
    while(door.get_position() > 0){ //!!! WILL NEED ADJUSTMENT
      door.move(-200);
    }
    door.move(0);
  }

};
#endif
