#include "main.h"

void intake_init() {
  intake.set_brake_modes(MOTOR_BRAKE_HOLD);
}

void set_intake(int input) {
  intake = input;
}

void intake_opcontrol() {
  // if (master.get_digital(intakeButton)) {
  //   set_intake(127);
  //   // matchload.move_absolute(20,85);
  // } else if (master.get_digital(outtakeButton)) {
  //   // matchload.move_absolute(60,85);
  //   set_intake(-127);
  // } else {
  //   set_intake(0);
  //   // matchload.move_absolute(20,85);
  // }


}