/**
 * \file main.h
 *
 * Contains common definitions and header files used throughout your PROS
 * project.
 *
 * Copyright (c) 2017-2021, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

/**
 * If defined, some commonly used enums will have preprocessor macros which give
 * a shorter, more convenient naming pattern. If this isn't desired, simply
 * comment the following line out.
 *
 * For instance, E_CONTROLLER_MASTER has a shorter name: CONTROLLER_MASTER.
 * E_CONTROLLER_MASTER is pedantically correct within the PROS styleguide, but
 * not convienent for most student programmers.
 */
#include "pros/misc.h"
#include "pros/misc.hpp"
#define PROS_USE_SIMPLE_NAMES

/**
 * If defined, C++ literals will be available for use. All literals are in the
 * pros::literals namespace.
 *
 * For instance, you can do `4_mtr = 50` to set motor 4's target velocity to 50
 */
#define PROS_USE_LITERALS

#include "api.h"

/**
 * You should add more #includes here
 */
//#include "okapi/api.hpp"
//#include "pros/api_legacy.h"
#include "EZ-Template/api.hpp"
#include "EZ-GUI/api.hpp"

// More includes here...
#include "intake.hpp"
#include "matchload.hpp"
#include "autons.hpp"


/**
 * If you find doing pros::Motor() to be tedious and you'd prefer just to do
 * Motor, you can use the namespace with the following commented out line.
 *
 * IMPORTANT: Only the okapi or pros namespace may be used, not both
 * concurrently! The okapi namespace will export all symbols inside the pros
 * namespace.
 */
// using namespace pros;
// using namespace pros::literals;
// using namespace okapi;
// using namespace ez;
using namespace okapi::literals;

/**
 * Prototypes for the competition control tasks are redefined here to ensure
 * that they can be called from user code (i.e. calling autonomous from a
 * button press in opcontrol() for testing purposes).
 */

#ifdef __cplusplus
extern "C" {
#endif
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);

// LIFT MOTORS

inline pros::Motor liftMotor1(-9);
inline pros::Motor liftMotor2(1);

// INTAKE MOTORS

inline pros::Motor intakeMotor(17);
inline pros::Motor intakeMotor2(-2);

// CATCHER MOTORS (not use)

inline pros::Motor catcher1(16);
inline pros::Motor catcher2(-14);

// MATCH LOADER

inline pros::Motor matchLoader(18);
inline pros::Motor matchLoader2(-14);


// PNEUMATIC DECLARATIONS

inline pros::ADIDigitalOut rightWing('B');
inline pros::ADIDigitalOut leftWing('C');
inline pros::ADIDigitalOut liftLock('A');

// BUTTONS

inline pros::controller_digital_e_t controlSwitchButton = pros::E_CONTROLLER_DIGITAL_X;

inline pros::controller_digital_e_t rightWingButton = pros::E_CONTROLLER_DIGITAL_L1;
inline pros::controller_digital_e_t leftWingButton = pros::E_CONTROLLER_DIGITAL_L1;
inline pros::controller_digital_e_t intakeButton = pros::E_CONTROLLER_DIGITAL_R1;
inline pros::controller_digital_e_t outtakeButton = pros::E_CONTROLLER_DIGITAL_R2;
inline pros::controller_digital_e_t liftUpButton = pros::E_CONTROLLER_DIGITAL_Y;
inline pros::controller_digital_e_t liftDownButton = pros::E_CONTROLLER_DIGITAL_RIGHT;
inline pros::controller_digital_e_t latchButton = pros::E_CONTROLLER_DIGITAL_A;
inline pros::controller_digital_e_t outputButton = pros::E_CONTROLLER_DIGITAL_X;
//pros::controller_digital_e_t unlatchButton = pros::E_CONTROLLER_DIGITAL_Y;
inline pros::controller_digital_e_t matchLoadButton = pros::E_CONTROLLER_DIGITAL_L2;







#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/**
 * You can add C++-only headers here
 */
//#include <iostream>
#endif

#endif  // _PROS_MAIN_H_
