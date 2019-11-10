/*
Condition	Controller LCD	Brain LCD	Controller Rumble
Low Brain Battery	Yes	Yes	Yes

Low Controller Battery	Yes	No	Yes
std::int32_t pros::Controller::get_battery_level ( )

Motor Over Current	Yes	No	Yes
BB Lift to Desired Height	Yes	No	Yes
Motor Unplugged	Yes	Yes	Yes
Sensor Not Calibrated	Yes	Yes	Yes
BB Intake-Gate Open	Yes	No	Yes
*/
#include "main.h"
#include "hardware.h"

// rumble the controller
void get_ready_to_rumble(){
  Hardware::master.print(3, 0, ".            ");
}
// print to controller
void controller_display(char *errmsg){
  Hardware::master.print(0, 0, errmsg);
}

//print to brain
void brain_display(std::string errmsg){
  //pros::lcd::initialize(); //This only needs to be called once at the start of you program.
  pros::lcd::set_text(0, errmsg);
}

//Condition	Controller LCD	Brain LCD	Controller Rumble
//Low Brain Battery	Yes	Yes	Yes


//Low Controller Battery	Yes	No	Yes
void error_checking(){
if(Hardware::master.get_battery_level() / Hardware::master.get_battery_capacity() < 0.2){
  get_ready_to_rumble();
  controller_display("Controller Battery Low\n");
}

//Motor Over Current	Yes	No	Yes
//Hardware::left_drive.getCurrentDraw() // isOverCurrent

//BB Lift to Desired Height	Yes	No	Yes
//Motor Unplugged	Yes	Yes	Yes
//Sensor Not Calibrated	Yes	Yes	Yes
//BB Intake-Gate Open	Yes	No	Yes
}
