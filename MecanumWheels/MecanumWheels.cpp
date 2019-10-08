/*
  MecanumWheels.h - Library for robo wheels.
  Created by krill221 , august 30, 2019.
*/

#include "Arduino.h"
#include "MecanumWheels.h"


Multiservo myservo_lf;
Multiservo myservo_rf;
Multiservo myservo_lb;
Multiservo myservo_rb;

Octoliner octoliner_top(42);
Octoliner octoliner_bottom(43);
Octoliner octoliner_left(44);
Octoliner octoliner_right(45);

MecanumWheels::MecanumWheels() { }

void MecanumWheels::init(int sensitivity, int brightness){
  last_top = 0;
  last_bottom = 0;
  last_left = 0;
  last_right = 0;

  /// orientation
  global_angle = 0;
  global_step = 0;

  // init_sensors
  Wire.begin();
  octoliner_bottom.begin();
  octoliner_bottom.setSensitivity(sensitivity); octoliner_bottom.setBrightness(brightness);
  delay(200);
  octoliner_top.begin();
  octoliner_top.setSensitivity(sensitivity); octoliner_top.setBrightness(brightness);
  delay(200);
  octoliner_left.begin();
  octoliner_left.setSensitivity(sensitivity); octoliner_left.setBrightness(brightness);
  delay(200);
  octoliner_right.begin();
  octoliner_right.setSensitivity(sensitivity); octoliner_right.setBrightness(brightness);
  delay(200);
  Serial.println("init MecanumWheels done");
}

void MecanumWheels::init_wheels(int lf, int rf, int lb, int rb){
  myservo_lf.attach(lf); myservo_lf.write(95);
  myservo_rf.attach(rf); myservo_rf.write(95);
  myservo_lb.attach(lb); myservo_lb.write(95);
  myservo_rb.attach(rb); myservo_rb.write(95);
}

void MecanumWheels::free_wheels(){
  myservo_lf.detach();
  myservo_rf.detach();
  myservo_lb.detach();
  myservo_rb.detach();
}


void MecanumWheels::print_coords(){
  float top_side = get_side(octoliner_top, 1);;
  float bottom_side = get_side(octoliner_bottom, 2);
  float left_side = get_side(octoliner_left, 4);
  float right_side = get_side(octoliner_right, 3);

  Serial.print(" t: "); Serial.print(top_side); Serial.print(" b: "); Serial.print(bottom_side); Serial.print(" l: "); Serial.print(left_side); Serial.print(" r: "); Serial.println(right_side);
  //Serial.print("global angle: "); Serial.println(global_angle);
  //Serial.print("global step: "); Serial.println(global_step);
}


void MecanumWheels::set_to_contre(){

  for(int i =0; i < 20; i++){

    for(int i =0; i < 30; i++){
      print_coords();
      float angle = round(find_current_angle1());
      delay(400);
      if( angle == 0.0) break;
      if( angle < -8) { turn_right(60); continue; }
      if( angle <  0) { turn_right(40); continue; }

      if( angle > 8) { turn_left(60); continue; }
      if( angle > 0) { turn_left(40); continue; }

    }

    print_coords();
    int top_side = get_side(octoliner_top, 1);;
    int bottom_side = get_side(octoliner_bottom, 2);
    int left_side = get_side(octoliner_left, 4);
    int right_side = get_side(octoliner_right, 3);
    delay(200);
    if( top_side == 0.0 && left_side == 0.0) break;
    if( top_side > 0) { move_right(50); continue; }
    if( top_side < 0) { move_left(50); continue; }
    if( left_side > 0) { move_forward(50); continue; }
    if( left_side < 0) { move_back(70); continue; }
  }

}

boolean MecanumWheels::is_out_line_side(Octoliner &octoliner) {
  boolean o_line = false; int lines = 0;
  int data[8];
  for (int i = 0; i < 8; i++) {
    data[i] = octoliner.analogRead(i);
  }
  for (int i = 0; i < 8; i++) { if( data[i] < 60 ) lines++;  }
  if(lines == 8) { return true; }
  return false;
}

boolean MecanumWheels::is_double_line_side(Octoliner &octoliner) {
  boolean d_line = false; float sum = 0; float avg = 0; int lines = 0;
  int data[8];
  //Serial.print("d line: ");
  for (int i = 0; i < 8; i++) {
    data[i] = octoliner.analogRead(i); sum += data[i];
  }
  avg = sum/8;
  for (int i = 0; i < 8; i++) {
    if( data[i] > avg && (data[i] - avg) > 30) lines++;
  }
  if( lines > 3){
    d_line = true;
    Serial.println(" d line | >3");
  } else {
    if( (data[0] > avg && (data[0] - avg) > 30 ) && (data[7] > avg && (data[7] - avg) > 30 )){
      d_line = true;
      Serial.println(" d line | 1--1");
    } else {
        //Serial.println(" | none");
    }
  }
  return d_line;
}

int MecanumWheels::get_side(Octoliner &octoliner, int sen){
  int data[8]; int sum = 0; int lines = 0; int side = 0;
  for (int i = 0; i < 8; i++) {
    data[i] = octoliner.analogRead(i);
    sum += data[i];
    //Serial.print(data[i]);
    //Serial.print(" ");
  }
  for (int i = 0; i < 8; i++) {
    if( data[i] > sum/8 && (data[i] - sum/8) > 30) lines++;
  }
  if(lines != 0){
    side = round( octoliner.mapLine(data) * 10 );
    if(sen == 1) last_top = side/abs(side)*7; // top
    if(sen == 2) last_bottom = side/abs(side)*7; // bottom
    if(sen == 3) last_right = side/abs(side)*7; // right
    if(sen == 4) last_left = side/abs(side)*7; // left
  } else {
    if(sen == 1) side = last_top;
    if(sen == 2) side = last_bottom;
    if(sen == 3) side = last_right;
    if(sen == 4) side = last_left;
  }
  return side;
}

void MecanumWheels::move_step_forward() {
    Serial.println("move_step_forward");
    set_to_contre();
    for(int i = 0; i< 10; i++) {
      move_forward(200);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(left_side < -6 || right_side > 6) break;
      delay(500);
    }
    move_forward(800);
    delay(500);
    for(int i = 0; i< 10; i++) {
      move_forward(100);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(left_side > -5 || right_side < 5) break;
      delay(500);
    }
    set_to_contre();
    delay(500);
    global_step = global_step + 1;
}

void MecanumWheels::move_step_back() { // avg load
    Serial.println("move_step_back");
    set_to_contre();
    for(int i = 0; i< 10; i++) {
      move_back(200);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(right_side < -6 || left_side > 6) break;
      delay(500);
    }
    move_back(800);
    delay(500);
    for(int i = 0; i< 10; i++) {
      move_back(100);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(right_side > -5 || left_side < 5) break;
      delay(500);
    }
    set_to_contre();
    delay(500);
    global_step = global_step - 1;
}

void MecanumWheels::rotate_left_90() {
    Serial.println("rotate_left_90");
    set_to_contre();
    delay(200);
    turn_left(2600);
    delay(200);
    set_to_contre();
    delay(200);
    global_angle = global_angle - 90;
}

void MecanumWheels::rotate_right_90() {
    Serial.println("rotate_right_90");
    set_to_contre();
    delay(200);
    turn_right(2600);
    delay(200);
    set_to_contre();
    delay(200);
    global_angle = global_angle + 90;
}

void MecanumWheels::turn_right(int ms){
  Serial.println("turn_right");
  myservo_lf.write(185); //f
  myservo_rf.write(185);  //b
  myservo_lb.write(185); //f
  myservo_rb.write(185); //b

  delay(ms);
  myservo_lf.write(0); //f
  myservo_rf.write(0); //f
  myservo_lb.write(0); //f
  myservo_rb.write(0); //f

  delay(20);
  myservo_lf.write(95);
  myservo_rf.write(95);
  myservo_lb.write(95);
  myservo_rb.write(95);
  delay(300);
}
void MecanumWheels::turn_left(int ms){
  Serial.println("turn_left");
  myservo_lf.write(0); //b
  myservo_rf.write(0);  //f
  myservo_lb.write(0); //b
  myservo_rb.write(0); //f

  delay(ms);
  myservo_lf.write(185); //f
  myservo_rf.write(185); //f
  myservo_lb.write(185); //f
  myservo_rb.write(185); //f
  delay(20);

  myservo_lf.write(95);
  myservo_rf.write(95);
  myservo_lb.write(95);
  myservo_rb.write(95);
  delay(300);
}
void MecanumWheels::move_left(int ms){
  Serial.println("move_left");
  myservo_lf.write(0); //b
  myservo_rf.write(0);  //f
  myservo_lb.write(185); //f
  myservo_rb.write(185); //b
  delay(ms);
  myservo_lf.write(185); //f
  myservo_rf.write(185); //f
  myservo_lb.write(0); //f
  myservo_rb.write(0); //f
  delay(20);
  myservo_lf.write(95);
  myservo_rf.write(95);
  myservo_lb.write(95);
  myservo_rb.write(95);
  delay(300);
}
void MecanumWheels::move_right(int ms){
  Serial.println("move_right");
  myservo_lf.write(185); //f
  myservo_rf.write(185); //b
  myservo_lb.write(0); //b
  myservo_rb.write(0); //f
  delay(ms);
  myservo_lf.write(0); //f
  myservo_rf.write(0); //f
  myservo_lb.write(185); //f
  myservo_rb.write(185); //f
  delay(20);
  myservo_lf.write(95);
  myservo_rf.write(95);
  myservo_lb.write(95);
  myservo_rb.write(95);
  delay(300);
}
void MecanumWheels::move_forward(int ms){
  Serial.println("move_forward");
  myservo_lf.write(185); //f
  myservo_rf.write(0); //f
  myservo_lb.write(185); //f
  myservo_rb.write(0); //f
  delay(ms);
  myservo_lf.write(0); //f
  myservo_rf.write(185); //f
  myservo_lb.write(0); //f
  myservo_rb.write(185); //f
  delay(20);
  myservo_lf.write(95);
  myservo_rf.write(95);
  myservo_lb.write(95);
  myservo_rb.write(95);
  delay(300);
}
void MecanumWheels::move_back(int ms){
  Serial.println("move_back");
  myservo_lf.write(0); //b
  myservo_rf.write(185); //b
  myservo_lb.write(0); //b
  myservo_rb.write(185); //b
  delay(ms);
  myservo_lf.write(185); //f
  myservo_rf.write(0); //f
  myservo_lb.write(185); //f
  myservo_rb.write(0); //f
  delay(20);
  myservo_lf.write(95);
  myservo_rf.write(95);
  myservo_lb.write(95);
  myservo_rb.write(95);
  delay(300);
}



int MecanumWheels::find_current_angle1(){
  int top = get_side(octoliner_top, 1);
  int bot = get_side(octoliner_bottom, 2);

  float cur_angle;   int k;
  int X11 = top; int X12 = (bot*-1);
  if(X11 >= X12) k = X11 - X12; if(X11 < X12) k = X12 - X11;
  cur_angle = 180 - 90 - (((atan((float)14/(float)k)) * 180.0)/PI);
  if(X11 > X12) cur_angle = -cur_angle;

  if(X11 == X12) Serial.println(" --||--");
  if(X11 > X12)  Serial.println(" --\\--");
  if(X11 < X12)  Serial.println(" --//--");

  Serial.print("current angle 1: "); Serial.print(cur_angle); Serial.print("\n");

  return cur_angle;
}

int MecanumWheels::find_current_angle2(){
  int left= get_side(octoliner_left, 4);
  int right = get_side(octoliner_right, 3);

  float cur_angle;   int k;
  int X11 = right; int X12 = (left*-1);
  if(X11 >= X12) k = X11 - X12; if(X11 < X12) k = X12 - X11;
  cur_angle = 180 - 90 - (((atan((float)14/(float)k)) * 180.0)/PI);
  if(X11 > X12) cur_angle = -cur_angle;

  if(X11 == X12) Serial.println(" --||--");
  if(X11 > X12)  Serial.println(" --\\--");
  if(X11 < X12)  Serial.println(" --//--");

  Serial.print("current angle 2: "); Serial.print(cur_angle); Serial.print("\n");
  return cur_angle;
}
