#include "main.h"
#include <iostream>
#include <fstream>
#include <string>

ez::PID loadPID{5, 0, 0, 0, "Load"};
void matchload_init() {
    matchload.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    matchload.tare_position();
    
}
