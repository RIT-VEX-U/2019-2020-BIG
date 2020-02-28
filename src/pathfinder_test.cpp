#include "pathfinder_test.h"

using namespace okapi;

void pathfinder_test()
{
  
  Waypoint waypoint_list[] =
      {
          {0, 0, d2r(0)},
          {24, 12, d2r(0)}};

  TrajectoryCandidate candidate;

  pathfinder_prepare(waypoint_list, 2, FIT_HERMITE_CUBIC, PATHFINDER_SAMPLES_LOW, .05, 10, 20, 200, &candidate);

  pros::lcd::print(0, "Created Candidate");

  Segment *center_trajectory = (Segment*) malloc(sizeof(Segment) * candidate.length);

  Segment l_trajectory[candidate.length];
  Segment r_trajectory[candidate.length];

  int traj_len = 2;


  pathfinder_generate(&candidate, center_trajectory);

  pros::lcd::print(0, "Generated Center Trajectory");

  EncoderFollower *l_path_follower = (EncoderFollower*)malloc(sizeof(EncoderFollower));
  EncoderFollower *r_path_follower = (EncoderFollower*)malloc(sizeof(EncoderFollower));

  EncoderConfig enc_conf{
      0, imev5GreenTPR, 4, .1, 0, 0, 1.0 / 10.0, 0};

  pathfinder_modify_tank(center_trajectory, candidate.length, l_trajectory, r_trajectory, 11.5);

  pros::lcd::print(0, "Generated Left and Right Trajs");

  Hardware::left_middle.set_encoder_units(E_MOTOR_ENCODER_COUNTS);
  Hardware::right_middle.set_encoder_units(E_MOTOR_ENCODER_COUNTS);

  pros::lcd::print(0, "Enterred While Loop");

  while (1)
  {
    double l_out = pathfinder_follow_encoder(enc_conf, l_path_follower, l_trajectory, candidate.length, Hardware::left_middle.get_position());
    double r_out = pathfinder_follow_encoder(enc_conf, r_path_follower, r_trajectory, candidate.length, Hardware::right_middle.get_position());

    l_out += (l_path_follower->heading - Hardware::imu.get_rotation()) * .011;
    r_out -= (l_path_follower->heading - Hardware::imu.get_rotation()) * .011;

    pros::lcd::print(0, "Lout: %f, Rout: %f", l_out, r_out);

    Hardware::drive_system.drive(l_out, r_out);

    pros::delay(20);
  }

  

  auto chassis = okapi::ChassisControllerBuilder()
                     .withMotors({20, 8, 19}, {12, 7, 11})
                     .withDimensions(okapi::AbstractMotor::gearset::green, {{4, 11.5}, okapi::imev5GreenTPR})
                     .build();

  auto profile_controller = okapi::AsyncMotionProfileControllerBuilder()
                                .withLimits({1.0, 2.0, 10.0})
                                .withOutput(chassis)
                                .buildMotionProfileController();

  profile_controller->generatePath({{0_in, 0_in, 0_deg}, {12_in, 0_in, 0_deg}}, "A");
  profile_controller->setTarget("A");
  profile_controller->waitUntilSettled();
}