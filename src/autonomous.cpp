#include "main.h"
#include "hardware.h"

enum AUTO_COLOR
{
    BLUE = 1,
    RED = -1
};

enum AUTO_1_STATE
{
    INIT, DRIVE1, COLLECT_AO, TURN1, DRIVE2, TURN2, COLLECT_Q,
    REVERSE1, TURN3, DRIVE3, DROP1, REVERSE_AWAY, END
};


AUTO_1_STATE auto_1_current = INIT; 
AUTO_COLOR color = BLUE;

float drive_speed = .5;
float drive_slow_speed = .2;
float turn_speed = .5; 

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

    while(true)
    {
        pros::lcd::print(0, "Angle: %f", Hardware::gyro.get());
        
        switch(auto_1_current)
        {
            case INIT:
            // Inititalize / calibrate period


            auto_1_current = DRIVE1;
            break;
            case DRIVE1:
            // Drive forward towards cube group A(red) or O(blue)

            if(Hardware::drive_system.drive_forward(5, drive_speed))
                auto_1_current = COLLECT_AO;

            break;
            case COLLECT_AO:
            // Collect the cubes, one at a time, driving forward slowly

            Hardware::horiz_intake.run_intake(true, false);

            if(Hardware::drive_system.drive_forward(22, drive_slow_speed))
                auto_1_current = TURN1;

            break;
            case TURN1:
            // Turn towards the Q group of cubes
            Hardware::horiz_intake.run_intake(false, false);
            if(Hardware::drive_system.turn_degrees(color * 60, turn_speed))
                auto_1_current = DRIVE2;

            break;
            case DRIVE2:
            // Drive to complete a lateral translation towards Q

            if(Hardware::drive_system.drive_forward(11.5, drive_speed))
                auto_1_current = TURN2;

            break;
            case TURN2:
            // Turn again to complete the 2 point turn

            if(Hardware::drive_system.turn_degrees(color * -60, turn_speed))
                auto_1_current = COLLECT_Q;

            break;
            case COLLECT_Q:
            // Collect the single Q cube

            Hardware::horiz_intake.run_intake(true, false);
            if(Hardware::drive_system.drive_forward(3, drive_slow_speed))
                auto_1_current = REVERSE1;

            break;
            case REVERSE1:
            // Begin reversing to drop off cubes

            if(Hardware::drive_system.drive_forward(-32, drive_speed))
                auto_1_current = TURN3;

            break;
            case TURN3:
            // Turn towards scoring zone

            if(Hardware::drive_system.turn_degrees(color * -135, turn_speed))
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
