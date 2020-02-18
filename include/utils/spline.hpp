#include "okapi/api.hpp"
#include "imu.hpp"
#include "tank_drive.hpp"

class spline
{

struct spline_config
{
  float drive_p = 0, drive_i = 0, drive_d = 0;
  float kv = 0, ka = 0;
  
};

public:

  spline(Waypoint waypoint_list[], int waypoint_num, spline_config *config)
  {
    
  }
};