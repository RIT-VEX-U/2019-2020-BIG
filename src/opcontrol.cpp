#include "main.h"
#include "logging.hpp"
#include "hardware.h"

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
 float lift_heights[] = {0.4, 0.8, 1.2, 1.6};
 int current_height = -1;

 int HEIGHT_MAX = /*lift_heights.size()*/ 4;	//Will need to figure out why this isn't taking size

void opcontrol() {
	logging::clearLogFile();
	//pros::lcd::set_text(1, "Hi, Shane!");
	//pros::lcd::print(2, "Before if statement");
	while (true) {
		if(Hardware::master.get_digital_new_press(DIGITAL_X))
		{
			//Hardware::left_drive.moveAbsolute(5, 100);
			while(Hardware::drive_system.drive_forward(12, .5))
			{
				pros::delay(20);
			}
		}

		int left = Hardware::master.get_analog(ANALOG_LEFT_Y);
		int right = Hardware::master.get_analog(ANALOG_RIGHT_Y);

		Hardware::drive_system.drive(left, right);

    //All functionality for when lift is not holding a position
    if(!Hardware::lift.is_holding()){
        //manual raise & lower
		    if(Hardware::master.get_digital(DIGITAL_R2) && !Hardware::lift.is_holding()){
			       Hardware::lift.raise(100);
		    }
		    else if(Hardware::master.get_digital(DIGITAL_R1) && !Hardware::lift.is_holding()){
			       Hardware::lift.lower(100);
	      }
		    else /*if(Hardware::lift.isMoving())*/{
			       Hardware::lift.stop();
		    }

        //Starting lift holding
        if(Hardware::master.get_digital(DIGITAL_L1)){
          current_height++;
          Hardware::lift.moveTo(lift_heights[current_height], true);
        }

    }

    //All functionality for when the lift is holding a position
    else{
      if(Hardware::master.get_digital(DIGITAL_L1)){
			     if(current_height < HEIGHT_MAX){
				         current_height++;
			     }
		  }
		  else if(Hardware::master.get_digital(DIGITAL_L2)){
		      if(current_height - 1 > -1){
				        current_height--;
			    }
			    else{
                //End lift hold
				        Hardware::lift.release_hold();
			    }
		  }
      Hardware::lift.hold_pos();
    }

		Hardware::horiz_intake.run_intake(Hardware::master.get_digital(DIGITAL_A), Hardware::master.get_digital(DIGITAL_B));

		//Log all motors
		//Hardware::drive_system.logDrive();
		//Hardware::lift.logLift();

		pros::lcd::print(1, "%f, %f, %f, %f", Hardware::lift1.getVoltage() * 100, Hardware::lift2.getVoltage(),
																					Hardware::lift3.getVoltage(), Hardware::lift4.getVoltage());
		pros::delay(20);
	}
}
