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
    left_side.tarePosition();
    right_side.tarePosition();
    is_checking_standstill = false;
    initialize_func = true;
  }

  double max_speed = fabs(percent_speed);

  //Calculate p, d, and feedforward for deceleration
  float pd = ((inches > 0 ? 1 : -1) * config->drive_feedforward)
            + ((inches - (left_side.getPosition() * (config->wheel_size * PI))) * config->drive_p);
            + (left_side.getActualVelocity() * (config->wheel_size * PI) * config->drive_d);

  pd = pd > 1.0 ? 1.0 : pd < -1.0 ? -1.0 : pd;

  float out = pd * max_speed;

  pros::lcd::print(0, "speed: %f", out);

  left_side.moveVelocity(out * (int)gearset);
  right_side.moveVelocity(out * (int)gearset);

  float delta = fabs((inches / (config->wheel_size * PI)) - left_side.getPosition());

  // Exit condition
  // If the delta between the target position and the current position is within
  // a defined range for {drive_standstill_time} seconds, then return true
  if(delta < config->drive_deadband)
  {
    if(is_checking_standstill == false)
    {
      last_time_2 = (pros::millis() / 1000.0);
      is_checking_standstill = true;
    }else if ((pros::millis() / 1000.0) - last_time_2 > config->drive_standstill_time)
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
/**
Autonomously turn the robot x degrees, using feedback from a gyroscope

For left turns, use negative degrees, for right, use positive.
*/
bool TankDrive::turn_degrees(double degrees, double percent_speed)
{
  //Initialize the function, reset gyro and timers
  if(initialize_func == false)
  {
    gyro.reset();
    turn_last_angle = 0;
    last_time = pros::millis() / 1000.0;
    is_checking_standstill = false;
    initialize_func = true;
  }

  // gathering data from the sensors
  double max_speed = fabs(percent_speed);
  double current_angle = gyro.get();
  double delta = current_angle - turn_last_angle;

  // Calculate the feedback loop: feedforward + (p * delta) + (d * velocity)
  float pd = (config->turn_feedforward)
           + (config->turn_p * (degrees - current_angle))
           + (config->turn_d * (delta / ((pros::millis() / 1000.0) - last_time)));

  // Store values from this iteration for next iteration
  last_time = pros::millis() / 1000.0;
  turn_last_angle = current_angle;

  // Limit the pd value between -1.0 and 1.0
  pd = (pd > 1.0) ? 1.0 : (pd < -1.0) ? -1.0 : pd;

  left_side.moveVelocity((int)gearset * pd * max_speed);
  right_side.moveVelocity((int)gearset * pd * max_speed * -1);

  // Exit condition: if the position is within x deadband for t seconds, return true.
  if(fabs(delta) < config->turn_deadband)
  {
    if(is_checking_standstill == false)
    {
      last_time_2 = pros::millis() / 1000.0;
      is_checking_standstill = true;
    }else if((pros::millis() / 1000.0) - last_time_2 > config->turn_standstill_time)
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
