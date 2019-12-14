#ifndef _VERTICAL_INTAKE_
#define _VERTICAL_INTAKE_
#include "pros/motors.hpp"
class VerticalIntake{
private:
  //
  pros::Motor left, right, door;

public:
  //
  //enum door_pos{open = };

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
  bool open(){
    door.move(200);
    return door.get_position() >= 0.05;
  }

  //close the door of the intake
  bool close(){
    door.move(-75);
    return door.get_position() <= 0.05;
  }

  int getDoorPos(){
    return door.get_position();
  }

};
#endif
