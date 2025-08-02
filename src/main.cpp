#include "main.h"

#include "autons.hpp"
#include "pros/misc.h"
#include "pros/motors.h"

// Drive motors.  These are used in GUI and Template
std::vector<int> left_motors = { -20,-19,8,-10};
std::vector<int> right_motors = {11,12,-16,15};

int write = 0;
float rotateD = 90;
// Chassis constructor
ez::Drive chassis(
    left_motors,   // Left motors
    right_motors,  // Right motors
    6,            // IMU Port
    4.0,           // Wheel size 
    300.0,         // Cart RPM
    1.0,           // Gear ratio
    10.33          // Drive width
);

// GUI Constructor
ez::GUI display(
    {{pros::Motor(left_motors[0]), "left 1"},
     {pros::Motor(left_motors[1]), "left 2"},
     {pros::Motor(right_motors[1]), "right 2"},
     {pros::Motor(right_motors[0]), "right 1"},
     {pros::Motor(left_motors[2]), "left 3"},
     {pros::Motor(left_motors[3]), "left 4"},
     {pros::Motor(right_motors[3]), "right 4"},
     {pros::Motor(right_motors[2]), "right 3"},
     {intake[0], "l inta"},
     {intake[1], "r inta"}},

    {
     {" offense 2 ball, touches bar ", offense2ball},
    });

// Initialize
void initialize() {
  // Print our branding over your terminal :D
  ez::ez_template_print();

  pros::delay(500);  // Stop the user from doing anything while legacy ports configure

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(true);  // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0);    // Sets the active brake kP. We recommend 0.1.
  chassis.opcontrol_curve_default_set(0, 0);     // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)
  default_constants();                           // Set the drive to your own constants from autons.cpp!

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set (pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT); // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y,    pros::E_CONTROLLER_DIGITAL_A);

  // Initialize chassis and auton selector
  chassis.initialize();
  // ez::as::initialize();
  display.enable();
  master.rumble(".");

  // Initialize subsystems
  intake_init();
  matchload_init();
  liftMotor1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  liftMotor2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  intakeMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  intakeMotor2.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  matchLoader.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  matchLoader2.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  matchLoader.tare_position();
  matchLoader2.tare_position();
  liftMotor1.tare_position();
  liftMotor2.tare_position();
}

void intakeOn() {
  intakeMotor.move(127);
  intakeMotor2.move(127);
}

void intakeOff() {
  intakeMotor.brake();
  intakeMotor2.brake();
}

void intakeRev() {
  intakeMotor.move(-127);
  intakeMotor2.move(-127);
}

void wingsOn() {
  rightWing.set_value(true);
  leftWing.set_value(true);
}

void rightWingOn() {
  rightWing.set_value(true);  
}

void leftWingOn() {
  leftWing.set_value(true);
}

void wingsOff() {
  rightWing.set_value(false);
  leftWing.set_value(false);
}

void rightWingOff() {
  rightWing.set_value(false);
}

void leftWingOff() {
  leftWing.set_value(false);
}

float getPos() {
  float sum = 0;
  for(int i = 0; i < 4; i++) {
    sum += chassis.right_motors.at(i).get_position();
    sum += chassis.left_motors.at(i).get_position();
  }
  sum /= 8;
  return sum;
}

float getRightPos() {
  float sum = 0;
  for(int i = 0; i < 4; i++) {
    sum += chassis.right_motors.at(i).get_position();
  }
  sum /= 4;
  return sum;
}

float getLeftPos() {
  float sum = 0;
  for(int i = 0; i < 4; i++) {
    sum += chassis.left_motors.at(i).get_position();
  }
  sum /= 4;
  return sum;
}

// 90 deg rotation = 342

void rotation(float deg) {
  int dir = (deg > 0) ? 1 : -1;
   float lastError;
   float deltaE;
   int ticks = 0;
   bool done = false;
   float p = 100;
   float d = rotateD; // was 10
   float startPos = getRightPos();
   int doneTick = 0;

  while(!done) {
    
    float target = startPos + (295.0/90.0) * deg;
    float pos = getRightPos();
    float error = target - pos;
    if(ticks != 0) {
      deltaE = (error - lastError) / 50;
    } else {
      deltaE = 0;
    }

    float out = p * (error)/100 + d * deltaE;
    int volt = (int)(max(-127, min(127, (int)out)));
    if(abs(volt) < 10) {
      volt = 0;
    } else if(abs(volt) < 40) {
      volt = volt / abs(volt) * 40;
    }
    chassis.drive_set(-volt,volt);
    lastError = error;

    ticks++;
    if(error < 30) {
      doneTick++;
    } else {
      doneTick = 0;
    }
    if(doneTick >= 4) {
       done = true;
    }
    

    // if(pos-) {
    //   done = true;
    // }
    display.selector_text_set(std::to_string(pos-startPos));
    // display.selector_text_set(std::to_string((pos-target)/target));
    pros::delay(33);
    
  }
}


void moveForward(float inches) { // 850 = 24 inches
 
   int dir = (inches > 0) ? 1 : -1;
   float lastError;
   float deltaE;
   int ticks = 0;
   bool done = false;
   float p = 20;
   float d = 17; // was 10
   float startPos = getPos();
   int doneTick = 0;

  while(!done) {

    // 850 units = 1 tile
    
    float target = startPos + (850.0 / 24.0) * inches;
    float pos = getPos();
    float error = target - pos;
    if(ticks != 0) {
      deltaE = (error - lastError) / 50;
    } else {
      deltaE = 0;
    }

    float out = p * (error)/100 + d * deltaE;
    int volt = (int)(max(-127, min(127, (int)out)));
    if(abs(volt) < 5) {
      volt = 0;
    } else if(abs(volt) < 25) {
      volt = volt / abs(volt) * 25;
    }
    chassis.drive_set(volt,volt);
    lastError = error;

    ticks++;
    if(error < 30) {
      doneTick++;
    } else {
      doneTick = 0;
    }
    if(doneTick >= 4) {
       done = true;
    }
    

    // if(pos-) {
    //   done = true;
    // }
    display.selector_text_set(std::to_string(pos-startPos));
    // display.selector_text_set(std::to_string((pos-target)/target));
    pros::delay(33);
    
  }

}

// auton for competition
void autonomous() {
  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency
  chassis.odom_pose_set({0, 0, 0});
  chassis.drive_odom_enable(true);
  chassis.SPACING = 0.5;
  pros::delay(10);

  matchLoader.move_absolute(80, 127);
  matchLoader2.move_absolute(80, 127);
  pros::delay(500);
  matchLoader.move_absolute(40, 127);
  matchLoader2.move_absolute(40, 127);
  pros::delay(500);
  rightWingOn();
  intakeOn();

chassis.pid_drive_set(3,100);
chassis.pid_wait();
chassis.pid_odom_smooth_pp_set({{{0, 32}, fwd, 127},{{-1, 36}, fwd, 127},{{-3, 40}, fwd, 127},{{-5, 45}, fwd, 127},{{-8, 49}, fwd, 127}});
chassis.pid_wait();

intakeRev();
chassis.pid_odom_smooth_pp_set({{{-8, 50}, fwd, 127},{{-10, 51}, fwd, 127},{{-12, 52}, fwd, 127}});
chassis.pid_wait();
wingsOff();
chassis.pid_odom_smooth_pp_set({{{-13, 49}, fwd, 127},{{-13, 47}, fwd, 127},{{-12, 44}, fwd, 127},{{-13, 38}, fwd, 127},{{-19, 30}, fwd, 127}});
chassis.pid_wait();
intakeOn();

chassis.pid_odom_smooth_pp_set({{{-32, 22}, fwd, 127},{{-35, 20}, fwd, 127},{{-39, 18}, fwd, 127},{{-42, 15}, fwd, 127},{{-42, 14.5}, fwd, 127}});
chassis.pid_wait();
chassis.pid_odom_pp_set({{{-42, 18}, rev, 127}});
chassis.pid_wait();
chassis.pid_odom_pp_set({{{-44, 18}, fwd, 127}});
chassis.pid_wait();

leftWingOn();
chassis.pid_odom_smooth_pp_set({{{-44, 20}, fwd, 127},{{-44, 23}, fwd, 50}});
chassis.pid_wait();
rightWingOn();
intakeOff();
chassis.pid_odom_smooth_pp_set({{{-44, 46}, fwd, 127}}); // 46 -> 46.5
chassis.pid_wait();

wingsOff(); // added
pros::delay(500); // added



chassis.pid_odom_smooth_pp_set({{{-34, 29}, fwd, 127}});
chassis.pid_wait();
wingsOff();
intakeOn();


chassis.pid_odom_smooth_pp_set({{{-26, 17}, fwd, 80},{{-23, 11}, fwd, 127},{{-32, 19}, rev, 80},{{-24, 20}, fwd, 60}});
chassis.pid_wait();
leftWingOn();
chassis.pid_odom_smooth_pp_set({{{-18, 29}, fwd, 127}}); // edited path
chassis.pid_wait();

rightWingOn(); // added
chassis.pid_odom_smooth_pp_set({{{-8, 53}, fwd, 127},{{-10, 55}, fwd, 127},{{-13, 55}, fwd, 127},{{-24, 55}, fwd, 127}});
chassis.pid_wait();
intakeRev();

chassis.pid_odom_pp_set({{{-11, 55}, rev, 127}});
chassis.pid_wait();

chassis.pid_odom_smooth_pp_set({{{-24, 55}, fwd, 127}});
chassis.pid_wait();


intakeOff();
wingsOff();

  matchLoader.move_absolute(-30, 127);
  matchLoader2.move_absolute(-30, 127);


  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency
  chassis.odom_pose_set({0, 0, 0});
  chassis.drive_odom_enable(true);
  chassis.SPACING = 0.5;
  pros::delay(10);

  chassis.pid_odom_smooth_pp_set({{{0, -10}, rev, 127}});
  chassis.pid_wait();
// add stuff here
  chassis.pid_odom_smooth_pp_set({{{-6, -8}, rev, 127},{{-22, -8}, rev, 127},{{-23, -8}, rev, 127}});
  chassis.pid_wait();
   pros::delay(1000);
  chassis.drive_set(-127,-127);
  pros::delay(1800);
  chassis.drive_set(0,0); 

  matchLoader.move_absolute(-40, 100);
  matchLoader2.move_absolute(-40, 100);
  
}

// Opcontrol
void opcontrol() {
  // This is preference to what you like to drive on

  matchLoader.tare_position();
  matchLoader2.tare_position();

  int leftWingCounter = 10;
  int rightWingCounter = 10;
  bool leftWingExtended = false;
  bool rightWingExtended = false;
  bool climbing = false;
  bool armDown = false;
  bool liftValue = true;
  bool loweringClimbThing = false;
  bool doneLoweringClimb = false;
  bool raisingClimbThing = false;
  

  // config vars
  bool climbEnabled = true;
  bool testingAuton = false;
  bool pidTunerEnabled = !climbEnabled;
  

  // intake vars
  bool intaking = false;
  int Xticks = 0;

  liftLock.set_value(liftValue);

  chassis.drive_brake_set(MOTOR_BRAKE_COAST);
  chassis.drive_odom_enable(false);


  while (true) {
    // PID Tuner
    // After you find values that you're happy with, you'll have to set them in auton.cpp
   
    if(pidTunerEnabled) {
      if (!pros::competition::is_connected()) {
        // Enable / Disable PID Tuner
        //  When enabled:
        //  * use A and Y to increment / decrement the constants
        //  * use the arrow keys to navigate the constants
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
          chassis.pid_tuner_toggle();
          if (chassis.pid_tuner_enabled())
            pros::lcd::set_background_color(LV_COLOR_HEX(0xFFC0CB));
        }

        // Trigger the selected autonomous routine
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
          autonomous();
          chassis.drive_brake_set(MOTOR_BRAKE_COAST);
          chassis.drive_odom_enable(false);
        }
      }
      chassis.pid_tuner_iterate();  // Allow PID Tuner to iterate
    }

    // if (master.get_digital_new_press(DIGITAL_B)) {
    //   autonomous();
    //   chassis.drive_brake_set(MOTOR_BRAKE_COAST);
    //   chassis.drive_odom_enable(false);
    // }

    intake_opcontrol();
    matchload_opcontrol();

    chassis.opcontrol_tank();  // Tank control

    if (master.get_digital(rightWingButton) && (rightWingCounter >= 30)) {
      rightWingExtended = !rightWingExtended;
      rightWingCounter = 0;
    }

    if (master.get_digital(leftWingButton) && (leftWingCounter >= 30)) {
      leftWingExtended = !leftWingExtended;
      leftWingCounter = 0;
    }

    if (rightWingExtended) {
      rightWing.set_value(true);
    }
    if (!rightWingExtended) {
      rightWing.set_value(false);
    }

    if (leftWingExtended) {
      leftWing.set_value(true);
    }
    if (!leftWingExtended) {
      leftWing.set_value(false);
    }
    rightWingCounter++;
    leftWingCounter++;



    // PNEUMATIC LIFT LOCK
    
    if(master.get_digital_new_press(latchButton)){
      liftValue = !liftValue;
      liftLock.set_value(liftValue);
    }


    bool outtake = false;

    
    if (master.get_digital(outtakeButton)) {
      intakeMotor.move(-127);
      intakeMotor2.move(-127);
      // intakeMotor.move

      matchLoader.move_absolute(60, 110);
      matchLoader2.move_absolute(60, 110);
      outtake = true;

    } else if (master.get_digital(intakeButton)) {
      intakeMotor.move(127);
      intakeMotor2.move(127);
    } else {
      intakeMotor.brake();
      intakeMotor2.brake();
      // intakeMotor.move(40);
      // intakeMotor2.move(40);
    }

    if(!testingAuton) {
      if(master.get_digital(matchLoadButton)) {
        matchLoader.move_absolute(235, 110);
        matchLoader2.move_absolute(235, 110);
      } else if(!outtake && !loweringClimbThing && !doneLoweringClimb) {
        matchLoader.move_absolute(20,85);
        matchLoader2.move_absolute(20,85);
      }
    }

    // climb code
    if(climbEnabled) {
      if(master.get_digital(liftDownButton)) {
        loweringClimbThing = true;
      }
      if(master.get_digital(liftUpButton) || liftMotor1.get_position() >= 7216) {
        doneLoweringClimb = true;
      }
      
      if (loweringClimbThing && !doneLoweringClimb) {
        matchLoader.move_absolute(170,110);
        matchLoader2.move_absolute(170, 110);
        liftMotor1.move(127);
        liftMotor2.move(127);
        climbing = false;
      }else if (master.get_digital(liftUpButton)) {
        liftMotor1.move(-127);
        liftMotor2.move(-127);
        matchLoader.move_absolute(235,110);
        matchLoader2.move_absolute(235, 110);
      }  else {
        liftMotor1.brake();
        liftMotor2.brake();
      }
    }

    // for setting position
    //   if (master.get_digital(liftDownButton)) {
    //     loweringClimbThing = true;
    //     matchLoader.move_absolute(170,110);
    //     matchLoader2.move_absolute(170, 110);
    //     liftMotor1.move(127);
    //     liftMotor2.move(127);
    //   }else if (master.get_digital(liftUpButton)) {
    //     liftMotor1.move(-127);
    //     liftMotor2.move(-127);
    //   }  else {
    //     liftMotor1.brake();
    //     liftMotor2.brake();
    //   }
    // }

    pros::delay(ez::util::DELAY_TIME);  // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}

void disabled() {
  liftLock.set_value(false);
}
void competition_initialize() {
}