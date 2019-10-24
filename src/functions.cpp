#include "main.h"
#include "functions.h"
#include "api.h"
#include "pros/apix.h"


using namespace pros;

void driveStop()
{
  hw::driveBackLeft.move_velocity(0);
  hw::driveBackRight.move_velocity(0);
  hw::driveFrontLeft.move_velocity(0);
  hw::driveFrontRight.move_velocity(0);
}

void drive()
{
  hw::driveBackLeft.move_velocity(200);
  hw::driveBackRight.move_velocity(200);
  hw::driveFrontLeft.move_velocity(200);
  hw::driveFrontRight.move_velocity(200);
}

void drive(int time)
{
  hw::driveBackLeft.move_velocity(200);
  hw::driveBackRight.move_velocity(200);
  hw::driveFrontLeft.move_velocity(200);
  hw::driveFrontRight.move_velocity(200);
  delay(time);
  driveStop();
}

void drive(double distance, int velocity)
{
  hw::driveBackLeft.set_zero_position(0);
  hw::driveBackRight.set_zero_position(0);
  hw::driveFrontLeft.set_zero_position(0);
  hw::driveFrontRight.set_zero_position(0);
  hw::driveBackLeft.move_absolute(distance, velocity);
  hw::driveBackRight.move_absolute(distance, velocity);
  hw::driveFrontLeft.move_absolute(distance, velocity);
  hw::driveFrontRight.move_absolute(distance, velocity);
}

int getMotorGearsetRPM(Motor m1)
{
  if(m1.get_gearing() == 0)
  {
    //1800 ticks/rev with 36:1 gears - 100rpm
    return 100;
  }
  else if(m1.get_gearing() == 1)
  {
    //900 ticks/rev with 18:1 gears - 200rpm
    return 200;
  }
  else
  {
    //300 ticks/rev with 6:1 gears - 600rpm
    return 600;
  }
}

bool checkMotorState(Motor m1)
{

}

bool checkMotorGearset(Motor m1, Motor m2, Motor m3, Motor m4)
{
  //Takes in raw values of motor gearing.
  //Raw values should be either 0, 1, or 2
  //Otherwise, the motor is either defined as "blank" or internal gearing is broken
  int gears[4];
  gears[0] = m1.get_gearing();
  gears[1] = m2.get_gearing();
  gears[2] = m3.get_gearing();
  gears[3] = m4.get_gearing();

  //New array to keep track of motors with actual gear settings
  int goodFlag = 0;
  int goodGears[4];

  //Loop to add "good" gears to the "goodGears" array  if they are good gears (get_gearing returns a 0, 1, or 2)
  for(int i = 0; i <= 3; i++)
  {
    if(gears[i] <= 2)
    {
      goodGears[goodFlag] = gears[i];
      goodFlag++;
    }
  }

  if(goodFlag == 1)
  {
    return true;
  }
  else
  {
    for(int i = 0; i < goodFlag - 1; i++)
    {
      if(goodGears[0] != goodGears[i+1])
      {
        return false;
      }
    }
  }

  return true; //Return true if no gears are proven to be not equal
}

void gearsetFailure(char motorSubsystem[32])
{
	char buf[64];
  strcat (buf, motorSubsystem);
  strcat (buf, " motors have different gearing!");
	lv_obj_t * label1 = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label1, buf);
	lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

  hw::master.rumble("---");
  // hw::master.clear();
  hw::master.print(0, 0, "%c motors", motorSubsystem);
  // hw::master.set_text(1,0,"Robot may function incorrectly!");
}
