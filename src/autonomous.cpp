#include "main.h"

enum AUTO_1_STATE
{
    INIT, DRIVE1, COLLECT_AO, TURN1, DRIVE2, TURN2, DRIVE3, COLLECT_Q,
    REVERSE1, TURN3, DRIVE4, DROP1, REVERSE_AWAY, END
};


AUTO_1_STATE auto_1_current = INIT; 

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
        switch(auto_1_current)
        {
            case INIT:
            // Inititalize / calibrate period

            break;
            case DRIVE1:
            // Drive forward towards cube group A(red) or O(blue)

            break;
            case COLLECT_AO:
            // Collect the cubes, one at a time, driving forward slowly

            break;
            case TURN1:
            // Turn towards the Q group of cubes

            break;
            case DRIVE2:
            // Drive to complete a lateral translation towards Q

            break;
            case TURN2:
            // Turn again to complete the 2 point turn

            break;
            case DRIVE3:
            // Drive towards the Q group

            break;
            case COLLECT_Q:
            // Collect the single Q cube

            break;
            case REVERSE1:
            // Begin reversing to drop off cubes

            break;
            case TURN3:
            // Turn towards scoring zone

            break;
            case DRIVE4:
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
    }
    
}
