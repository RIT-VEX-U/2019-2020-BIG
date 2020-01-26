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
    left.move(-100);
    right.move(100);
  }

  //drop a cube
  void drop(){
    left.move(100);
    right.move(-100);
  }

  // Keep the intake at 0 RPM
  void stop_intake()
  {
    left.move_relative(0, 200);
    right.move_relative(0, 200);
  }

  int open_pos = -550;
  int close_pos = 0;

  //open the door of the intake
  bool open(){
    door.move_absolute(open_pos, 200);
    return fabs(open_pos - door.get_position()) < 10;
  }

  // Returns whether or not the door is open
  bool is_open()
  {
    return fabs(open_pos - door.get_position()) < 10;
  }

  // Returns whether or not the door is closed
  bool is_closed()
  {
    return fabs(close_pos - door.get_position()) < 10;
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
