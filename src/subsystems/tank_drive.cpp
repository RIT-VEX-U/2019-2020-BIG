#include "subsystems/tank_drive.hpp"

/**
Drive the robot with differential drive.
Left axis is the Y axis on the left joystick, same for the right joystick.

Takes into account that the joystick outputs between -127.0 and +127, and the
motors input between -(min rpm) and +(max rpm) based on the gearset used.
*/
void TankDrive::drive(int left_axis, int right_axis)
{
  left_side.moveVelocity((left_axis / 127.0) * (int)gearset);
  right_side.moveVelocity((right_axis / 127.0) * (int)gearset);
}

/**
Autonomously drive the robot x inches forward (or negative inches for reverse)
at percent_speed (between 0.0 and 1.0).

Will stop and return true after reaching the target.
*/
bool TankDrive::drive_forward(double inches, double percent_speed)
{
  if(initialize_func == false)
  {
    //Reset the class timer, reset absolute position to 0 on startup.
    last_time = timer.millis().getValue();
    last_time_2 = 0;
    left_side.tarePosition();
    right_side.tarePosition();
    is_checking_standstill = false;
    initialize_func = true;
  }

  double max_speed = fabs(percent_speed);

  //Calculate p, d, and feedforward for deceleration
  float pd = ((inches > 0 ? 1 : -1) * drive_feedforward)
            + ((inches - (left_side.getPosition() * wheel_size)) * drive_p);
            + (left_side.getActualVelocity() * wheel_size * drive_d);

  pd = pd > 1.0 ? 1.0 : pd < -1.0 ? -1.0 : pd;

  float out = pd * percent_speed;

  pros::lcd::print(0, "speed: %f", out);

  left_side.moveVelocity(out * (int)gearset);
  right_side.moveVelocity(out * (int)gearset);

  float delta = fabs((inches / wheel_size) - left_side.getPosition());

  // Exit condition
  // If the delta between the target position and the current position is within
  // a defined range for {drive_standstill_time} seconds, then return true
  if(delta < drive_deadband)
  {
    if(is_checking_standstill == false)
    {
      last_time_2 = timer.millis().getValue();
      is_checking_standstill = true;
    }else if (timer.millis().getValue() - last_time_2 > drive_standstill_time)
    {
      left_side.moveVelocity(0);
      right_side.moveVelocity(0);
      is_checking_standstill = false;
      initialize_func = false;
      return true;
    }
  }else
  {
    is_checking_standstill = false;
  }

  return false;
}
