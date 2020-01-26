#include "main.h"
#include "hardware.h"

enum AUTO_COLOR
{
	BLUE = 1,
	RED = -1
};

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
AUTO_COLOR color = BLUE;

enum AUTO_LIFT_STATE
{
  RAISE, LOWER, HOLD
};

AUTO_LIFT_STATE auto_lift_current = HOLD;
float auto_lift_hold_pos = 0;

float drive_speed = .5;
float drive_slow_speed = .2;
float turn_speed = 1;

void pick_up(){
  //Position the robot so that the cube is ready to be sucked up
  while(!Hardware::limit_switch.get_value()){
    Hardware::lift.hold_pos(0.2);
    //Check if the door needs to be opened
    if(Hardware::vert_intake.is_closed()){
      Hardware::vert_intake.open();
    }
    //Drive until cube sets off limit switch
    Hardware::drive_system.drive(50, 50);
  }

  //Drop lift, run intake to suck cube up
  while(Hardware::limit_switch.get_value()){
    Hardware::vert_intake.takeIn();
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

	while (true)
	{

    // State machine that controls the lift parallel to the main program.
    // Allows the lift to hold it's "state" between main automous states. For example,
    // if the lift is raising when the main program changes states, it will continue raising
    // into the next main program state.
    switch(auto_lift_current)
    {
      // Only raise if it has not reached it's max height
      case RAISE:
        if(Hardware::lift.getCurrPos() < 2.96)
          Hardware::lift.raise(12000);
        else
          Hardware::lift.stop();
      break;

      case LOWER:
        // Only lower if it has not reached it's min height
        if(Hardware::lift.getCurrPos() > 0)
          Hardware::lift.lower(6000);
        else
          Hardware::lift.stop();
      break;

      // Hold the lift where it is using the PID loop
      default:
      case HOLD:
        Hardware::lift.hold_pos(auto_lift_hold_pos);
      break;
    }

		pros::lcd::print(0, "Angle: %f", Hardware::gyro.get());

    // The main autonomous program.
		switch (auto_1_current)
		{
		case INIT:
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

			if (Hardware::drive_system.drive_forward(22, drive_slow_speed))
				auto_1_current = TURN1;

			break;
		case TURN1:
			// Turn towards the Q group of cubes
			Hardware::horiz_intake.run_intake(false, false);
			if (Hardware::drive_system.turn_degrees(color * 15, turn_speed))
				auto_1_current = COLLECT_Q;

			break;
		case COLLECT_Q:
			// Collect the single Q cube

			Hardware::horiz_intake.run_intake(true, false);
			if (Hardware::drive_system.drive_forward(1, drive_slow_speed))
				auto_1_current = REVERSE1;

			break;
    case REVERSE1:
      Hardware::horiz_intake.run_intake(false,false);
      if(Hardware::drive_system.drive_forward(-14, drive_speed))
      {
        auto_1_current = TURN2;
      }
    break;
    case TURN2:
      if(Hardware::drive_system.turn_degrees(-15, turn_speed))
      {
        auto_1_current = REVERSE2;
      }
      break;
		case REVERSE2:
			// Begin reversing to drop off cubes

			if (Hardware::drive_system.drive_forward(-15, drive_speed))
				auto_1_current = TURN3;

			break;
		case TURN3:
			// Turn towards scoring zone
      Hardware::horiz_intake.run_intake(false, false);

			if (Hardware::drive_system.turn_degrees(color * -150, turn_speed))
				auto_1_current = DRIVE3;

			break;
		case DRIVE3:
			// Drive to scoring zone

			break;
		case DROP1:
			// Drop off the 6 stack of cubes

			break;
		case REVERSE_AWAY:
			// Reverse away from the stacked cubes to get credit for the drop

			break;
		case END:
			// Stop motors and return from function.

			return;
		}

		pros::delay(20);
	}
}
