#include "subsystems/tank_drive.hpp"

void TankDrive::stop()
{
  left_front.move_velocity(0);
  left_middle.move_velocity(0);
  left_rear.move_velocity(0);

  right_front.move_velocity(0);
  right_middle.move_velocity(0);
  right_rear.move_velocity(0);
}

/**
Drive the robot with differential drive.
Left axis is the Y axis on the left joystick, same for the right joystick.

Takes into account that the joystick outputs between -127.0 and +127, and the
motors input between -(min rpm) and +(max rpm) based on the gearset used.
*/
void TankDrive::drive(double left_in, double right_in)
{
  double drive_mod = fabs(sin((left_in + right_in) * _PI / 4.0));

  double lf_out = left_in, lr_out = left_in;
  double rf_out = right_in, rr_out = right_in;

  if(config->omni_config == rear_wheel_dr)
  {
    lr_out *= drive_mod;
    rr_out *= drive_mod;
  }else if(config->omni_config == front_wheel_dr)
  {
    lf_out *= drive_mod;
    rf_out *= drive_mod;
  }

  left_front.move_velocity(lf_out * gearset);
  right_front.move_velocity(rf_out * gearset);
  left_rear.move_velocity(lr_out * gearset);
  right_rear.move_velocity(rr_out * gearset);
}

void TankDrive::arcade_drive(double left_y, double right_x)
{
  double left = left_y + right_x;
  double right = left_y - right_x;

  // Limit the output to between -1 and +1
  double left_out = (left > 1) ? 1 : (left < -1) ? -1 : left;
  double right_out = (right > 1) ? 1 : (right < -1) ? -1 : right;

  left_front.move_velocity(left_out * gearset);
  right_front.move_velocity(right_out * gearset);
  left_middle.move_velocity(left_out * gearset);
  right_middle.move_velocity(right_out * gearset);
  left_rear.move_velocity(left_out * gearset);
  right_rear.move_velocity(right_out * gearset);
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

    left_front.tare_position();
    drive_pid->reset();
    drive_pid->set_limits(-fabs(percent_speed), fabs(percent_speed));
    drive_pid->set_target(inches);
    initialize_func = true;
  }

  drive_pid->update(left_front.get_position() * _PI * config->wheel_size);
  drive(drive_pid->get(), drive_pid->get());


  // Exit condition
  // If the delta between the target position and the current position is within
  // a defined range for {drive_standstill_time} seconds, then return true
  if(drive_pid->is_on_target())
  {
    drive(0,0);
    initialize_func = false;
    return true;
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

    turn_pid->reset();
    turn_pid->set_limits(-fabs(percent_speed), fabs(percent_speed));
    turn_pid->set_target(degrees - imu->get_rotation());

    initialize_func = true;
  }

  turn_pid->update(imu->get_rotation());
  drive(turn_pid->get(), -turn_pid->get());

  // Exit condition: if the position is within x deadband for t seconds, return true.
  if(turn_pid->is_on_target())
  {
    initialize_func = false;
    drive(0,0);
    return true;
  }
  return false;
}
