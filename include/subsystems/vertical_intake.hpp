#ifndef _VERTICAL_INTAKE_
#define _VERTICAL_INTAKE_
#include "pros/motors.hpp"

class VerticalIntake{
private:

  pros::ADIDigitalOut door;

public:
  //
  //enum door_pos{open = };

  //Constructor
  VerticalIntake(pros::ADIDigitalOut door): door(door){ }

  bool door_is_closed = false;

  //open the door of the intake
  void open(){
    door.set_value(1);
    door_is_closed = false;
  }

  // Returns whether or not the door is open
  bool is_open()
  {
    return !door_is_closed;
  }

  // Returns whether or not the door is closed
  bool is_closed()
  {
    return door_is_closed;
  }

  //close the door of the intake
  void close()
  {
    door.set_value(0);
    door_is_closed = true;
  }

};
#endif
