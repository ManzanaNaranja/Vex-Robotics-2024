#pragma once

#include "api.h"

inline pros::MotorGroup matchload({17, -14});
void matchload_init();
void updatePID();
void matchload_opcontrol();
void writeToFile(int);

