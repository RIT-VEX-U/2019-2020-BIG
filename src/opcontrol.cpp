#include "main.h"
#include "logging.hpp"
#include "hardware.h"
#include "utils/spline_path.hpp"
#include "config.h"
#include "subsystems/rgb_controller.hpp"

//RGBController controller(RGBController::rgb_t::Off, 'A', 'B', 'C');

int drop_stack_state = 0;
uint32_t drop_stack_timer = 0;

//Encoder vals in rotations for different heights
float lift_heights[] = {.8};
float current_height = 0;

float MAX_LIFT = 3.0;
float MIN_LIFT = 0.1;

// boolean to see if the last action was pressing the b button OR using the lift with the joystick.
// For setting the default state to either "down" or "just above cube"
bool last_action_b_button = false;

/**
 * State machine for the sequence of events required for dropping off a stack of cubes in the scoring zone:
 * 1. Lower the lift to MIN_LIFT height, while running the horizontal intake wheels in reverse to get them out of the way
 * 2. Run the vertical intake wheels in reverse for a set amount of time to place the cubes on the ground
 * 3. Open the pod bay doors (sorry dave)
 *
 * This is a non-blocking function, and will return true when the sequence has finished, and false otherwise.
 * Make sure when calling this function, the horizontal and vertical intake / lift / doors are not accessed, or
 * chaos may ensue.
 */
bool drop_stack()
{
  last_action_b_button = false;
  Hardware::horiz_intake.retract_intake();
  switch (drop_stack_state)
  {
  // Step one: lower the lift
  case 0:
    if (fabs(Hardware::lift.getCurrPos()) < MIN_LIFT)
    {
      Hardware::lift.stop();
      Hardware::horiz_intake.run_intake(false, false);
      drop_stack_state++;
      drop_stack_timer = pros::millis();
      break;
    }
    // Reverse the horizontal intake to get the wheels out of the way
    Hardware::horiz_intake.run_intake(false, true);
    Hardware::lift.lower(8000);
    break;

  //Step three: open the door
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

  return false;
} // end drop_stack()

int DELAY = 50;

okapi::Timer timer;

//int HEIGHT_MAX = sizeof(lift_heights) /*4*/; //Will need to figure out why this isn't taking size

bool drop_stack_btn = false;
bool test_turn = false;
bool intake_toggle = false;
float drive_multiplier = .7;

SplinePath pather(&Hardware::drive_system, &Hardware::imu, &Hardware::left_middle, &Hardware::right_middle, &config::motion_profile);

/**
 * function opcontrol()
 *
 * Code run when an operator is controlling the robot.
 * OpControl initialization takes place before the while loop.
 *
 * Driver Controls:
 *
 * "Master" Remote:
 *    -Drive control (Arcade):
 *      LeftY: Forward / Backward
 *      RightX: Left / Right
 *    -Drive control (Tank):
 *      LeftY: Directly controls "left" wheels
 *      RightY: Directly controls "right" wheels
 *
 * "Partner" Remote:
 *    LeftY: Raise / Lower
 *    Left Bumpers: Horizontal Intake in
 *    Right Bumpers: Horizontal Intake out
 *    A: Lower lift, drop cubes, and open door
 *    B: Lower lift while running vertical intake up
 *    X: Open / Close door
 *    Y: Run the vertical intake up
 *
 */
void opcontrol()
{

  /*
  Waypoint list[] =
      {
          {0, 0, d2r(0)},
          {24, 0, d2r(45)},
          {24, 24, d2r(90)},
          {24, 48, d2r(45)},
          {48, 48, d2r(0)}
      };

  while (!pather.run_path(list, 5))
  {
    delay(10);
  }

  delay(1000);
  return;
*/
  logging::clearLogFile();

  int x = 0;

  char const *position_format = "pos: %f";
  char position[100];
  Hardware::master.clear();

  Hardware::left_intake.set_reversed(true);
  Hardware::right_intake.set_reversed(true);

  /*while(Hardware::imu.is_calibrating()){
    sprintf(position, position_format, timer.millis());
    Hardware::partner.print(2, 1, position);
  }*/

  while (true)
  {
    lcd::print(2, "Line Tracker: %d", Hardware::line_tracker.get_value());
    /*
    pros::lcd::print(0, "Gyro: %f", Hardware::imu.get_yaw());

    if (Hardware::master.get_digital(DIGITAL_A))
      test_turn = true;

    if (test_turn)
    {
      if (Hardware::drive_system.drive_forward(24, .5) || Hardware::master.get_digital(DIGITAL_B))
        test_turn = false;
    }
    else
      Hardware::drive_system.drive(0, 0);

    pros::delay(50);

    continue;
    */
    // If A is pressed, then start the "drop the stack" semi-auto function.

    sprintf(position, position_format, timer.millis());
    Hardware::partner.print(2, 1, position);

    /*
    if(Hardware::master.get_digital(DIGITAL_LEFT)){
      Hardware::solenoid.set_value(1);
    }
    else{
      Hardware::solenoid.set_value(0);
    }

    */

    if (Hardware::partner.get_digital_new_press(DIGITAL_A))
    {
      drop_stack_state = 0;
      drop_stack_btn = true;
    }

    // Stop the "drop the stack" function if the lift is being moved manually
    if (drop_stack_btn)
      if (drop_stack() || abs(Hardware::partner.get_analog(ANALOG_LEFT_Y)) > 50 || Hardware::partner.get_digital(DIGITAL_B))
        drop_stack_btn = false;

    // Open / Close the intake doors with one button press
    if (Hardware::partner.get_digital_new_press(DIGITAL_X))
      if (Hardware::vert_intake.is_closed())
        Hardware::vert_intake.open();
      else
        Hardware::vert_intake.close();

    // "Default states" for the lift:
    // If the user presses B, lower the lift while running the vert intake,
    // to suck up a cube into the magazine
    double lift_pos = Hardware::lift.getCurrPos();
    if (!drop_stack_btn)
    {
      //Hardware::horiz_intake.run_intake(Hardware::partner.get_digital(DIGITAL_L1) || Hardware::partner.get_digital(DIGITAL_L2),
      //                                  Hardware::partner.get_digital(DIGITAL_R1) || Hardware::partner.get_digital(DIGITAL_R2));

      if (Hardware::partner.get_digital(DIGITAL_B))
      {
        last_action_b_button = true;
        if (lift_pos > MIN_LIFT)
          Hardware::lift.lower(8000);
        else
          Hardware::lift.stop();
      }
      // If B is released, go to the "above one cube" height if it is below that height.
      // Stop the vertical intake.
      else if (lift_pos < lift_heights[0] - .01)
      {
        current_height = lift_heights[0];
      }

      // Run the lift manually using the left analog stick
      if (Hardware::partner.get_analog(ANALOG_LEFT_Y) > 50 && lift_pos <= MAX_LIFT)
      {
        last_action_b_button = false;
        Hardware::lift.raise(12000);
        current_height = lift_pos;
      }
      else if (Hardware::partner.get_analog(ANALOG_LEFT_Y) < -50 && lift_pos >= MIN_LIFT)
      {
        last_action_b_button = false;
        Hardware::lift.raise(-8000);
        current_height = lift_pos;
      }
      // If the B button or analog stick are not being used, hold the lift via PID
      else if (!Hardware::partner.get_digital(DIGITAL_B) && !last_action_b_button && lift_pos <= MIN_LIFT)
      {
        Hardware::lift.stop();
      }
      else if (!Hardware::partner.get_digital(DIGITAL_B))
      {
        Hardware::lift.hold_pos(current_height);
      }
    }
    if (Hardware::partner.get_digital_new_press(DIGITAL_Y))
      intake_toggle = !intake_toggle;

    if (Hardware::partner.get_analog(ANALOG_LEFT_Y) < -50 || Hardware::lift.getCurrPos() < .3 || intake_toggle || Hardware::partner.get_digital(DIGITAL_Y) || drop_stack_btn)
      Hardware::horiz_intake.retract_intake();
    else if (Hardware::partner.get_analog(ANALOG_LEFT_Y) > -50)
      Hardware::horiz_intake.contract_intake();

    Hardware::horiz_intake.run_intake(Hardware::partner.get_digital(DIGITAL_R1) || Hardware::partner.get_digital(DIGITAL_R2),
                                      Hardware::partner.get_digital(DIGITAL_L1) || Hardware::partner.get_digital(DIGITAL_L2));

    //okapi::QTime t = timer.millis();
    //sprintf(position, position_format, x, t);
    //Hardware::partner.print(2, 1, position);

    if(Hardware::master.get_digital_new_press(DIGITAL_X))
      drive_multiplier = (drive_multiplier > .5) ? .2 : .7;

    double left = drive_multiplier * Hardware::master.get_analog(ANALOG_LEFT_Y) / 127.0;
    double right = drive_multiplier * Hardware::master.get_analog(ANALOG_RIGHT_Y) / 127.0;

    if (Hardware::master.get_digital(DIGITAL_DOWN))
      Hardware::drive_system.drive(-.2, -.2);
    else
      Hardware::drive_system.drive(left, right);

    //Log all motors
    //Hardware::drive_system.logDrive();
    //Hardware::lift.logLift();

    pros::delay(DELAY);
  }
}
