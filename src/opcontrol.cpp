#include "main.h"
#include "logging.hpp"
#include "hardware.h"

void block_gyro_reset()
{
  Hardware::drive_system.drive(0, 0);
  Hardware::gyro.reset();
  //delay 1.5 seconds
  delay(1500);
}

int drop_stack_state = 0;

bool drop_stack()
{
  switch (drop_stack_state)
  {
  // Step one: lower the lift
  case 0:
    if (fabs(Hardware::lift.getCurrPos()) < 10)
    {
      Hardware::lift.stop();
      Hardware::horiz_intake.run_intake(false, false);
      drop_stack_state++;
      break;
    }
    // Reverse the horizontal intake to get the wheels out of the way
    Hardware::horiz_intake.run_intake(false, true);
    Hardware::lift.lower(2000);
    break;

  //Step two: open the door
  case 1:
    if (Hardware::vert_intake.is_open())
    {
      drop_stack_state = 0;
      return true;
    }
    Hardware::vert_intake.open();
    break;
  default:
    // Should never get here, but reset the state just in case.
    drop_stack_state = 0;
  }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

//Encoder vals in rotations for different heights
float lift_heights[] = {.2};
float current_height = 0;

float MAX_LIFT = 2.96;
float MIN_LIFT = 0;
int DELAY = 50;

okapi::Timer timer;

//int HEIGHT_MAX = sizeof(lift_heights) /*4*/; //Will need to figure out why this isn't taking size

bool drop_stack_btn = false;

void opcontrol()
{
  logging::clearLogFile();

  int x = 0;

  char const *position_format = "pos: %d, time: %f";
  char position[100];
  Hardware::master.clear();

  while (true)
  {
    // If A is pressed, then start the "drop the stack" semi-auto function.
    if(Hardware::master.get_digital_new_press(DIGITAL_A))
    {
      drop_stack_state = 0;
      drop_stack_btn = true;
    }

    // Stop the "drop the stack" function if the lift is being moved manually
    if (drop_stack_btn)
      if (drop_stack() || fabs(Hardware::master.get_analog(ANALOG_LEFT_Y)) > 20 || Hardware::master.get_digital(DIGITAL_B))
        drop_stack_btn = false;

    // Open / Close the intake doors with one button press
    if (Hardware::master.get_digital_new_press(DIGITAL_X))
      if (Hardware::vert_intake.is_open())
        Hardware::vert_intake.close();
      else if (Hardware::vert_intake.is_closed())
        Hardware::vert_intake.open();

    Hardware::horiz_intake.run_intake(Hardware::master.get_digital(DIGITAL_L1) || Hardware::master.get_digital(DIGITAL_L2),
                                      Hardware::master.get_digital(DIGITAL_R1) || Hardware::master.get_digital(DIGITAL_R2));

    // "Default states" for the lift:
    // If the user presses B, lower the lift while running the vert intake,
    // to suck up a cube into the magazine
    double lift_pos = Hardware::lift.getCurrPos();
    if (Hardware::master.get_digital(DIGITAL_B))
    {
      if (lift_pos > MIN_LIFT)
        Hardware::lift.lower(6000);
      else
        Hardware::lift.stop();

      Hardware::vert_intake.takeIn();
    }
    // If B is released, go to the "above one cube" height if it is below that height.
    // Stop the vertical intake.
    else if (lift_pos < lift_heights[0] - .01)
    {
      current_height = lift_heights[0];
      Hardware::vert_intake.stop_intake();
    }
    // As long as B is not being pressed, do not run the vert intake.
    else
    {
      Hardware::vert_intake.stop_intake();
    }

    // Run the lift manually using the left analog stick
    if (Hardware::master.get_analog(ANALOG_LEFT_Y) > 100 && lift_pos <= MAX_LIFT)
    {
      Hardware::lift.raise(12000);
      current_height = lift_pos;
    }
    else if (Hardware::master.get_analog(ANALOG_LEFT_Y) < -100 && lift_pos >= MIN_LIFT)
    {
      Hardware::lift.raise(-8000);
      current_height = lift_pos;
    }
    // If the B button or analog stick are not being used, hold the lift via PID
    else if (!Hardware::master.get_digital(DIGITAL_B))
    {
      Hardware::lift.hold_pos(current_height);
    }

    okapi::QTime t = timer.millis();
    sprintf(position, position_format, x, t);
    Hardware::master.print(2, 1, position);

    double left = Hardware::master.get_analog(ANALOG_LEFT_Y) / 127.0;
    double right = Hardware::master.get_analog(ANALOG_RIGHT_X) / 127.0;

    //Hardware::drive_system.arcade_drive(left, right);

    //Hardware::horiz_intake.run_intake(Hardware::master.get_digital(DIGITAL_L2), Hardware::master.get_digital(DIGITAL_L1));

    //Log all motors
    //Hardware::drive_system.logDrive();
    //Hardware::lift.logLift();

    pros::delay(DELAY);
  }
}
