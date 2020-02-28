#include "main.h"
#include "hardware.h"
#include "pathfinder_test.h"

double BLUE = 1.0;
double RED = -1.0;

float auto_lift_hold_pos = 0;

float drive_speed = .5;
float drive_slow_speed = .15;
float turn_speed = 1;
bool done_picking_up = false;

uint32_t pick_up_vert_intake_time = 0;

const double color = RED;

uint32_t back_up_timer = 0;
uint32_t intake_timer = 0;

int drop_stack_state_auto = 0;

using namespace okapi;

enum AUTO_1_STATE
{
  INIT,
  DRIVE1,
  COLLECT_AO,
  TURN1,
  DRIVE2,
  REVERSE1,
  TURN2,
  COLLECT_Q,
  REVERSE2,
  TURN3,
  DRIVE3,
  DROP1,
  REVERSE_AWAY,
  END
};

AUTO_1_STATE auto_1_current = INIT;

enum AUTO_2_STATE
{
  AUTO2_INIT,
  AUTO2_DRIVE1,
  AUTO2_TURN1,
  AUTO2_DRIVE2,
  AUTO2_DROP1,
  AUTO2_REVERSE_AWAY,
  AUTO2_END
};

AUTO_2_STATE auto_2_current = AUTO2_INIT;

void auto_2()
{
  while(!Hardware::drive_system.drive_forward(-24, drive_speed)){pros::delay(50);}
  while(!Hardware::drive_system.drive_forward(24, drive_speed)){pros::delay(50);}
  Hardware::drive_system.drive(0,0);

  return;
  Hardware::solenoid.set_value(0);

  while (1)
  {
    switch (auto_2_current)
    {
    case AUTO2_INIT:

      auto_2_current = AUTO2_DRIVE1;
      break;
    case AUTO2_DRIVE1:

      if (Hardware::drive_system.drive_forward(34, drive_slow_speed) && Hardware::line_tracker.get_value() > 1000)
        auto_2_current = AUTO2_TURN1;
      else if(Hardware::line_tracker.get_value() < 1000 || Hardware::lift.getCurrPos() < 1)
        Hardware::drive_system.drive(0,0);

      if (Hardware::line_tracker.get_value() < 1000 && pros::millis() - intake_timer > 1700)
      {
        intake_timer = pros::millis();
      }
      else if (pros::millis() - intake_timer < 1400)
      {
        if (Hardware::lift.getCurrPos() > -.1)
          Hardware::lift.lower(10000);
        else
          Hardware::lift.lower(0);
      }
      else
      {
        if(Hardware::lift.getCurrPos() < 1.5)
          Hardware::lift.raise(10000);
        else
          Hardware::lift.hold_pos(1.5);
      }

      break;
    case AUTO2_TURN1:
      if(Hardware::drive_system.turn_degrees( color * 135, turn_speed))
        auto_2_current = AUTO2_DRIVE2;

      if(Hardware::lift.getCurrPos() > .1)
        Hardware::lift.lower(3000);
      else
        Hardware::lift.hold_pos(.1);

      break;
    case AUTO2_DRIVE2:
      if(Hardware::drive_system.drive_forward(35, drive_speed))
        auto_2_current = AUTO2_DROP1;

      Hardware::lift.hold_pos(.1);

      break;
    case AUTO2_DROP1:
      if(Hardware::lift.getCurrPos() < .05)
      {
        Hardware::lift.lower(0);
        Hardware::solenoid.set_value(1);
        
        pros::delay(500);
        auto_2_current = AUTO2_REVERSE_AWAY;
      }
      
      Hardware::lift.lower(3000);
      break;
    case AUTO2_REVERSE_AWAY:

      if(Hardware::drive_system.drive_forward(-12, drive_speed))
        auto_2_current = AUTO2_END;

      break;
    case AUTO2_END:
      Hardware::drive_system.drive(0,0);
      Hardware::solenoid.set_value(0);
      break;
    }
    pros::delay(50);
  }
}

enum AUTO_LIFT_STATE
{
  RAISE,
  LOWER,
  HOLD
};

AUTO_LIFT_STATE auto_lift_current = HOLD;

void pick_up()
{
  if (done_picking_up == true)
  {
    if (Hardware::lift.getCurrPos() >= .1)
      Hardware::lift.lower(6000);
    else
      Hardware::lift.stop();
    return;
  }

  if (pros::millis() - pick_up_vert_intake_time > .3 && pros::millis() - pick_up_vert_intake_time < .6)
  {
    Hardware::lift.raise(8000);
  }

  //Drop lift, run intake to suck cube up
  if (Hardware::limit_switch.get_value())
  {
    pick_up_vert_intake_time = pros::millis();
    if (Hardware::lift.getCurrPos() >= .1)
      Hardware::lift.lower(6000);
    else
      Hardware::lift.stop();
  }
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
  //auto_2();
  //return;


  pathfinder_test();
  return;

  Hardware::vert_intake.close();
  while (true)
  {
    pick_up();

    pros::lcd::print(0, "Angle: %f", Hardware::gyro.get());

    // The main autonomous program.
    switch (auto_1_current)
    {
    case INIT:
      Hardware::vert_intake.close();
      // Inititalize / calibrate period
      auto_lift_hold_pos = .2;
      auto_lift_current = HOLD;

      auto_1_current = DRIVE1;
      break;
    case DRIVE1:
      // Drive forward towards cube group A(red) or O(blue)

      if (Hardware::drive_system.drive_forward(5, drive_speed))
        auto_1_current = COLLECT_AO;

      break;
    case COLLECT_AO:
      // Collect the cubes, one at a time, driving forward slowly

      Hardware::horiz_intake.run_intake(true, false);
      if (Hardware::limit_switch.get_value())
        back_up_timer = pros::millis();

      if (pros::millis() - back_up_timer < 500)
        Hardware::drive_system.drive(0, 0);
      else if (Hardware::drive_system.drive_forward(28, drive_slow_speed))
        auto_1_current = TURN1;

      break;
    case TURN1:
      // Turn towards the Q group of cubes
      Hardware::horiz_intake.run_intake(false, false);
      if (Hardware::drive_system.turn_degrees((color * 25), turn_speed))
        auto_1_current = COLLECT_Q;

      break;
    case COLLECT_Q:
      // Collect the single Q cube

      Hardware::horiz_intake.run_intake(true, false);
      if (Hardware::drive_system.drive_forward(11, drive_slow_speed))
        auto_1_current = REVERSE1;

      break;
    case REVERSE1:
      Hardware::horiz_intake.run_intake(false, false);
      done_picking_up = true;
      if (Hardware::drive_system.drive_forward(-11, drive_speed))
      {
        auto_1_current = TURN2;
      }
      break;
    case TURN2:
      if (Hardware::drive_system.turn_degrees((color * -25), turn_speed))
      {
        auto_1_current = REVERSE2;
      }
      break;
    case REVERSE2:
      // Begin reversing to drop off cubes

      if (Hardware::drive_system.drive_forward(-36, drive_speed))
        auto_1_current = TURN3;

      break;
    case TURN3:
      // Turn towards scoring zone
      Hardware::horiz_intake.run_intake(false, false);

      if (Hardware::drive_system.turn_degrees((color * -165), turn_speed))
      {
        auto_1_current = DRIVE3;
        intake_timer = pros::millis();
      }

      break;
    case DRIVE3:
      // Drive to scoring zone, running the horizontal intake for 1 second to get the wheels out of the way
      if (pros::millis() - intake_timer < 1000)
        Hardware::horiz_intake.run_intake(false, true);
      else
        Hardware::horiz_intake.run_intake(false, false);

      if (Hardware::drive_system.drive_forward(30, drive_speed))
        auto_1_current = DROP1;
      break;
    case DROP1:
      // Drop off the 6 stack of cubes

      //Mini state machine for dropping off cubes
      switch (drop_stack_state_auto)
      {
      // First, lower the lift to 0
      case 0:
        if (Hardware::lift.getCurrPos() < .15)
        {
          Hardware::lift.stop();
          intake_timer = pros::millis();
          drop_stack_state_auto++;
        }
        Hardware::lift.lower(4000);
        break;
      // Last, open the doors
      case 1:
        Hardware::vert_intake.open();
        if (Hardware::vert_intake.is_open())
          auto_1_current = REVERSE_AWAY;
        break;
      }

      break;
    case REVERSE_AWAY:
      // Reverse away from the stacked cubes to get credit for the drop
      if (Hardware::drive_system.drive_forward(-10, drive_speed))
        auto_1_current = END;
      break;
    case END:
      // Stop motors and return from function.
      Hardware::drive_system.drive(0, 0);
      return;
    }

    pros::delay(20);
  }
}
