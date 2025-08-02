#include "EZ-Template/util.hpp"
#include "main.h"

// These are out of 127
const int DRIVE_SPEED = 110;
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;
const int INTAKE_SPEED = 127;
const int INTAKE_HOLD = 30;
const int OUTAKE_SPEED = -127;

void default_constants() {
  chassis.pid_heading_constants_set(5, 0, 50); // 
  chassis.pid_drive_constants_set(20, 0, 120); // tuning 12 0 120 0
  chassis.pid_turn_constants_set(1.9,.005, 13.5, 15); // old (.8, .3, 6.5, -9.9);  .44, .35, 6.5, -9.9
  // 180 deg turn 1.5 .005 13.5 15
  // 360 deg turn 1.5, 0, 13.5, 0
  // 90 deg turn 1.9 .005 13.5 15
  chassis.pid_swing_constants_set(4, 0, 45); // p 7 -> 4
 
  chassis.pid_tuner_increment_d_set(1);
  chassis.pid_tuner_increment_p_set(1);
  chassis.pid_tuner_increment_start_i_set(.1);

  chassis.pid_turn_exit_condition_set(100_ms, 3_deg, 300_ms, 7_deg, 500_ms, 750_ms);
  chassis.pid_swing_exit_condition_set(100_ms, 3_deg, 300_ms, 7_deg, 500_ms, 750_ms);
  chassis.pid_drive_exit_condition_set(100_ms, 1_in, 300_ms, 3_in, 500_ms, 500_ms);
  chassis.pid_odom_turn_exit_condition_set(100_ms, 3_deg, 300_ms, 7_deg, 500_ms, 750_ms);
  chassis.pid_odom_drive_exit_condition_set(100_ms, 1_in, 300_ms, 3_in, 500_ms, 500_ms);

  chassis.slew_drive_constants_set(7_in, 80);
}

