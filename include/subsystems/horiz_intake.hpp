/*
* Purpose:
* Usage: include the hardware file where the intake
    is declared, Hardware::[name of horizontal intake].[function]
* @author Ryan McGee
*/

#ifndef _HORIZ_INTAKE_
#define _HORIZ_INTAKE_

#include "pros/motors.hpp"

using namespace pros;

class HorizIntake
{
private:
  Motor left, right, puller;

  //
  int motor_speed = 1;

public:
  //
  void run_intake(bool in_button, bool out_button)
  {
    int direction = (in_button == true) ? 1 : (out_button == true) ? -1 : 0;

    left.move_voltage(-12000 * direction * motor_speed);
    right.move_voltage(12000 * direction * motor_speed);
  }

  /**
   * Brings the intake out of the way for scoring
   */
  void retract_intake()
  {
    puller.move_voltage(12000 * (3 - puller.get_position()));
  }

  /**
   * Resets the intake to it's inital "suck in cubes" position
   */
  void contract_intake()
  {
    if (puller.get_position() > 0)
      puller.move_voltage(-6000);
    else
      puller.move_voltage(0);
  }

  //Constructor
  HorizIntake(Motor left_motor, Motor right_motor, Motor intake_puller) : left(left_motor), right(right_motor), puller(intake_puller)
  {
    puller.set_gearing(E_MOTOR_GEARSET_06);
    puller.set_encoder_units(E_MOTOR_ENCODER_ROTATIONS);
  }
};

#endif
