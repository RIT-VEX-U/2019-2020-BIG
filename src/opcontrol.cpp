#include "main.h"
#include "logging.hpp"
#include "hardware.h"

void block_gyro_reset(){
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
 float lift_heights[] = {0.4, 0.8, 1.2, 1.6};
 int current_height = -1;

 okapi::Timer timer;

 int HEIGHT_MAX = sizeof(lift_heights) /*4*/;	//Will need to figure out why this isn't taking size

void opcontrol() {
	logging::clearLogFile();

	int x = 0;

  char const *position_format = "pos: %d, time: %f";
	char position[100];
	Hardware::master.clear();

	while (true) {

		if(Hardware::master.get_digital_new_press(DIGITAL_X))
		{
			//pros::lcd::print(0, "%f", config::drive_pid_config.p);

			//Hardware::left_drive.moveAbsolute(5, 100);
			/*while(!Hardware::drive_system.turn_degrees(90, 1))
			{
				//pros::lcd::print(1, "angle: %f", Hardware::gyro.get());
				sprintf(angle, angle_format, Hardware::gyro.get());
				//Hardware::master.set_text(1, 1, angle);
				Hardware::master.print(2, 1, angle);
				pros::delay(53);
			}*/

		}

		//Intake Door functionality
		if(Hardware::master.get_digital(DIGITAL_A)){
			Hardware::intake_door.move(100);
			x = Hardware::vert_intake.getDoorPos();
		}
		else if(Hardware::master.get_digital(DIGITAL_Y)){
			Hardware::intake_door.move(-100);
			x = Hardware::vert_intake.getDoorPos();
		}

		if(Hardware::master.get_digital(DIGITAL_B))
			Hardware::gyro.reset();

		double left = Hardware::master.get_analog(ANALOG_LEFT_Y) / 127.0;
		double right = Hardware::master.get_analog(ANALOG_RIGHT_Y) / 127.0;
    Hardware::drive_system.drive(left, right);

    //All functionality for when lift is not holding a position
    if(!Hardware::lift.is_holding()){

        //manual raise & lower
		    if(Hardware::master.get_digital(DIGITAL_R2)){
			       Hardware::lift.raise(12000);
		    }
		    else if(Hardware::master.get_digital(DIGITAL_R1)){
			       Hardware::lift.lower(12000);
	      }
		    else /*if(Hardware::lift.isMoving())*/{
			       Hardware::lift.stop();
		    }

        //Starting lift holding
        if(Hardware::master.get_digital(DIGITAL_L1)){
          current_height++;
          /*x = */Hardware::lift.hold_pos(lift_heights[current_height]);
        }

    }

    //All functionality for when the lift is holding a position
    else{
      if(Hardware::master.get_digital(DIGITAL_L1)){
			     if(current_height < HEIGHT_MAX){
                 Hardware::lift.moveTo(lift_heights[current_height++], true);
			     }
		  }
		  else if(Hardware::master.get_digital(DIGITAL_L2)){
		      if(current_height > 0){
                Hardware::lift.moveTo(lift_heights[current_height--], true);
			    }
			    else{
                //End lift hold
				        Hardware::lift.release_hold();
			    }
		  }
      /*x = */Hardware::lift.hold_pos(lift_heights[current_height]);
    }

		okapi::QTime t = timer.millis();
		sprintf(position, position_format, x, t);
		Hardware::master.print(2, 1, position);

		Hardware::drive_system.drive(left, right);

		Hardware::horiz_intake.run_intake(Hardware::master.get_digital(DIGITAL_L2), Hardware::master.get_digital(DIGITAL_L1));

		//Log all motors
		//Hardware::drive_system.logDrive();
		//Hardware::lift.logLift();

		pros::delay(53);
	}
}
