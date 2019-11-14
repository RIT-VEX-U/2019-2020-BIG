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
	int count = 0;

	while (true) {
        // Only update motors every 2ms
	    if(!(count % 2)) {
            if (Hardware::controller.get_digital_new_press(DIGITAL_X)) {
                //Hardware::left_drive.moveAbsolute(5, 100);
                while (!Hardware::drive_system.drive_forward(12, .5)) {
                    pros::delay(20);
                }
            }

            int left = Hardware::controller.get_analog(ANALOG_LEFT_Y);
            int right = Hardware::controller.get_analog(ANALOG_RIGHT_Y);

            Hardware::drive_system.drive(left, right);

            if (Hardware::controller.get_digital(DIGITAL_R2)) {
                Hardware::lift.raise(200);
            } else if (Hardware::controller.get_digital(DIGITAL_R1)) {
                Hardware::lift.lower(200);
            } else /*if(Hardware::lift.isMoving())*/{
                Hardware::lift.stop();
            }

            Hardware::horiz_intake.run_intake(Hardware::controller.get_digital(DIGITAL_A),
                                              Hardware::controller.get_digital(DIGITAL_B));
        }

	    // Log motors every 25ms
	    if(!(count % 25)) {
            //Log all motors
            //Hardware::drive_system.logDrive();
            Hardware::lift.logLift();
        }

		pros::delay(1);
		count++;

		if (count > 2000) {
		    count = 1;
		}
	}
}
