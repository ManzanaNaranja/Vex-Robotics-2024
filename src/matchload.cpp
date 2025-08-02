#include "main.h"
#include <iostream>
#include <fstream>
#include <string>

ez::PID loadPID{5, 0, 0, 0, "Load"};
void matchload_init() {
    matchload.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    matchload.tare_position();
    
}
void updatePID() {
   std::ifstream file("pid.txt");
   std::string line;
   getline(file, line);
   float p;
   sscanf(line.c_str(), "%f", &p);
   getline(file, line);
   float i;
   sscanf(line.c_str(), "%f", &i);
   getline(file, line);
   float d;
   sscanf(line.c_str(), "%f", &d);

   loadPID.constants_set(p,i,d);
   file.close();
}

void writeToFile(int num) {
  ofstream myfile;
  myfile.open ("output.txt");
  string s = std::to_string(num);
  myfile << s;
  myfile.close();
}
void matchload_opcontrol() {

//  if(master.get_digital(matchLoadButton)) {
//     loadPID.target_set(180);
//  } else {
//     loadPID.target_set(30);
//  }
//   matchload = loadPID.compute(matchload.get_positions().at(0));
//   pros::delay(ez::util::DELAY_TIME);


   // if(master.get_digital(matchLoadButton)) {
   //  matchload.move_absolute(200, 85);
   // } else {
   //  matchload.move_absolute(20,85);
   // }
}