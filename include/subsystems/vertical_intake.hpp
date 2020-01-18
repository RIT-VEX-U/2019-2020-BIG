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

  //suck a cube
  void takeIn(){
    left.move(-200);
    right.move(200);
  }

  //drop a cube
  void drop(){
    left.move(200);
    right.move(-200);
  }

  // Keep the intake at 0 RPM
  void stop_intake()
  {
    left.move_velocity(0);
    right.move_velocity(0);
  }

  //open the door of the intake
  bool open(){
    door.move_absolute(-550, 200);
    return fabs(350 - door.get_position()) < 10;
  }

  //close the door of the intake
  bool close(){
    door.move_absolute(0, 200);
    return fabs(0 - door.get_position()) < 10;
  }

  int getDoorPos(){
    return door.get_position();
  }

};
#endif
