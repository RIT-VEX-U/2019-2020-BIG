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
void opcontrol() {
	logging::clearLogFile();
	while (true) {
		if(Hardware::master.get_digital_new_press(DIGITAL_X))
		{
			//pros::lcd::print(0, "%f", config::drive_pid_config.p);
			
			//Hardware::left_drive.moveAbsolute(5, 100);
			while(!Hardware::drive_system.turn_degrees(90, .5))
			{
				pros::lcd::print(1, "angle: %f", Hardware::gyro.get());
				pros::delay(20);
			}
			
		}

		if(Hardware::master.get_digital(DIGITAL_B))
			Hardware::gyro.reset();

		double left = Hardware::master.get_analog(ANALOG_LEFT_Y) / 127.0;
		double right = Hardware::master.get_analog(ANALOG_RIGHT_Y) / 127.0;


		pros::lcd::print(1, "angle: %f", Hardware::gyro.get());

		if(Hardware::master.get_digital(DIGITAL_R2)){
			Hardware::lift.raise(127);
		}
		else if(Hardware::master.get_digital(DIGITAL_R1)){
			Hardware::lift.lower(127);
		}
		else if(Hardware::lift.isMoving()){
			Hardware::lift.stop();
		}

		Hardware::drive_system.drive(left, right);

		Hardware::horiz_intake.run_intake(Hardware::master.get_digital(DIGITAL_L2), Hardware::master.get_digital(DIGITAL_L1));

		//Log all motors
		//Hardware::drive_system.logDrive();
		Hardware::lift.logLift();


		pros::delay(20);
	}
}
