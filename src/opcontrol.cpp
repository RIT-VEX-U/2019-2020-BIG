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

void opcontrol()
{
  logging::clearLogFile();

  int x = 0;

  char const *position_format = "pos: %d, time: %f";
  char position[100];
  Hardware::master.clear();

  while (true)
  {

    // Open / Close the intake doors
    if (Hardware::master.get_digital(DIGITAL_X))
      Hardware::vert_intake.open();
    else if (Hardware::master.get_digital(DIGITAL_Y))
      Hardware::vert_intake.close();

    Hardware::horiz_intake.run_intake(Hardware::master.get_digital(DIGITAL_L1), Hardware::master.get_digital(DIGITAL_L2));

    //All functionality for when lift is not holding a position
    //if (!Hardware::lift.is_holding())
    //{

      //manual raise & lower
      double lift_pos = Hardware::lift.getCurrPos();
      if(Hardware::master.get_digital(DIGITAL_B))
      {
        if(lift_pos > MIN_LIFT)
          Hardware::lift.lower(6000);
        else
          Hardware::lift.stop();

        Hardware::vert_intake.takeIn();
      }else if (lift_pos < lift_heights[0] - .01)
      {
        current_height = lift_heights[0];
        Hardware::vert_intake.stop_intake();
      }else
      {
        Hardware::vert_intake.stop_intake();
      }

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
      else if (!Hardware::master.get_digital(DIGITAL_B))
      {
        Hardware::lift.hold_pos(current_height);
      }
      //pros::lcd::print(0, "lift: %f", Hardware::lift.getCurrPos());
    /*
      //Starting lift holding
      if (Hardware::master.get_digital(DIGITAL_L1))
      {
        current_height++;
        Hardware::lift.hold_pos(lift_heights[current_height]);
      }
    }
  
    //All functionality for when the lift is holding a position
    else
    {
      if (Hardware::master.get_digital(DIGITAL_L1))
      {
        if (current_height < HEIGHT_MAX)
        {
          Hardware::lift.moveTo(lift_heights[current_height++], true);
        }
      }
      else if (Hardware::master.get_digital(DIGITAL_L2))
      {
        if (current_height > 0)
        {
          Hardware::lift.moveTo(lift_heights[current_height--], true);
        }
        else
        {
          //End lift hold
          Hardware::lift.release_hold();
        }
      }
      Hardware::lift.hold_pos(lift_heights[current_height]);
    }
    */
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
