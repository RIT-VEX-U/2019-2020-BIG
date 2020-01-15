#ifndef _PID_
#define _PID_

#include "pros/rtos.h"
#include <cmath>

/**
 * Creates a PID type object.
 * 
 * A PID loop is a closed feedback loop where a "setpoint" is set by the user, and the PID calculates
 * a value sent to the motor that will try and reach that setpoint as fast as possible, without overshooting.
 * 
 * The equation is:
 * Kout = kP*error + kI*integral(error) + kD*derivative(error)
 * 
 * error = setpoint - current_sensor_val
 * kP, kI, kD are constants that are found through tuning.
 * 
 * When tuning PID loops, start with tuning P, proportional. Using Kout = kP * error, or kP = Kout/error, you can
 * figure out a starting point for kP. For example, if you want the output to be .7 when the error is 2, then kP will
 * be .7/2 = .35.
 * 
 * If the position never reaches the setpoint, increase kP. If it overshoots and oscillates, lower kP.
 * 
 * If the position is accelerating too fast, add D. In general, the starting point for kD is 10 to 100 times kP.
 * If the position 
 * 
 * @author Ryan McGee 
 */
class PID
{
public:
  // Configuration of the PID loop
  struct config_t
  {
    double p = 0;
    double i = 0;
    double d = 0;
    double feedforward = 0;
    double deadband = 0;
    double on_target_time = 0;
  };

private:
  config_t *config;

  double last_error = 0;
  double last_time = 0;
  double on_target_last_time = 0;
  bool is_checking_on_target = false;

  double lower_limit = 0, upper_limit = 0;
  double target = 0, sensorVal = 0, out = 0;
  double accum_error = 0;

public:
  /**
 * Update the PID loop by taking the time difference from last update,
 * and running the PID formula with the new sensor data
 */
  void update(double sensorVal);

  /**
 * Reset the PID loop by resetting time since 0 and accumulated error.
 */
  void reset();

  /**
 * Gets the current PID out value, from when update() was last run
 */
  double get();

  /**
 * Get the delta between the current sensor data and the target
 */
  double get_error();

  /**
 * Set the target for the PID loop, where the robot is trying to end up
 */
  void set_target(double target);

  /**
 * Set the limits on the PID out. The PID out will "clip" itself to be 
 * between the limits.
 */
  void set_limits(double lower, double upper);

  /**
 * Returns true if the loop is within [deadband] for [on_target_time]
 * seconds
 */
  bool is_on_target();

  /**
 * Create the PID object with a configuration.
 */
  PID(config_t *config) : config(config)
  {
  }
};

#endif