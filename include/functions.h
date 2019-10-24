#include "main.h"
#include "hardware.h"
#include "logging.hpp"

using namespace pros;

void driveStop();
void drive();
void drive(int t); //Time in milliseconds
void drive(double distance, int velocity); //Distance in centimeteres

int getMotorGearsetRPM(Motor m1);
bool checkMotorGearset(Motor m1, Motor m2, Motor m3, Motor m4);
void gearsetFailure(char motorSubsystem[32]);
