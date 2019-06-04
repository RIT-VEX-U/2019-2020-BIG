#include "main.h"
#include "functions.h"
#include "hardware.h"
#include "api.h"

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
  if(m1.get_gearing() == 1)
  {
    //1800 ticks/rev with 36:1 gears - 100rpm
    return 100;
  }
  else if(m1.get_gearing() == 2)
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

bool checkMotorGearset(Motor m1, Motor m2 = hw::blank, Motor m3 = hw::blank, Motor m4 = hw::blank)
{
  if(m1.get_gearing() == m2.get_gearing() == m3.get_gearing() == m4.get_gearing())
  {
    return true;
  }
  return false;

}
