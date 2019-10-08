/*
  MecanumWheels.h - Library for robo wheels.
  Created by krill221 , august 30, 2019.
*/

#include "Arduino.h"
#include "MecanumWheelsStep.h"

Octoliner octoliner_top(42);
Octoliner octoliner_bottom(43);
Octoliner octoliner_left(44);
Octoliner octoliner_right(45);

MecanumWheelsStep::MecanumWheelsStep() { }

void MecanumWheelsStep::init(int sensitivity, int brightness){
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

void MecanumWheelsStep::init_wheels(int s1, int d1, int e1,
  int s2, int d2, int e2,
  int s3, int d3, int e3,
  int s4, int d4, int e4
){
  stepPin1 = s1; directionPin1 = d1; enablePin1 = e1;
  stepPin2 = s2; directionPin2 = d2; enablePin2 = e2;
  stepPin3 = s3; directionPin3 = d3; enablePin3 = e3;
  stepPin4 = s4; directionPin4 = d4; enablePin4 = e4;
  pinMode(stepPin1, OUTPUT);
  pinMode(directionPin1, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(directionPin2, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(directionPin3, OUTPUT);
  pinMode(enablePin3, OUTPUT);
  pinMode(stepPin4, OUTPUT);
  pinMode(directionPin4, OUTPUT);
  pinMode(enablePin4, OUTPUT);
}

void MecanumWheelsStep::hold_wheels(){
  digitalWrite(enablePin1, HIGH);
  digitalWrite(enablePin2, HIGH);
  digitalWrite(enablePin3, HIGH);
  digitalWrite(enablePin4, HIGH);
}
void MecanumWheelsStep::free_wheels(){
  digitalWrite(enablePin1, LOW);
  digitalWrite(enablePin2, LOW);
  digitalWrite(enablePin3, LOW);
  digitalWrite(enablePin4, LOW);
}


void MecanumWheelsStep::print_coords(){
  float top_side = get_side(octoliner_top, 1);;
  float bottom_side = get_side(octoliner_bottom, 2);
  float left_side = get_side(octoliner_left, 4);
  float right_side = get_side(octoliner_right, 3);

  Serial.print(" t: "); Serial.print(top_side); Serial.print(" b: "); Serial.print(bottom_side); Serial.print(" l: "); Serial.print(left_side); Serial.print(" r: "); Serial.println(right_side);
  //Serial.print("global angle: "); Serial.println(global_angle);
  //Serial.print("global step: "); Serial.println(global_step);
}


void MecanumWheelsStep::set_to_contre(){

  for(int i =0; i < 20; i++){

    for(int i =0; i < 30; i++){
      print_coords();
      float angle = round(find_current_angle1());
      delay(400);
      if( angle == 0.0) break;
      if( angle < -8) { turn_right(4); continue; }
      if( angle <  0) { turn_right(1); continue; }

      if( angle > 8) { turn_left(4); continue; }
      if( angle > 0) { turn_left(1); continue; }

    }

    print_coords();
    int top_side = get_side(octoliner_top, 1);;
    int bottom_side = get_side(octoliner_bottom, 2);
    int left_side = get_side(octoliner_left, 4);
    int right_side = get_side(octoliner_right, 3);
    delay(200);
    if( top_side == 0.0 && left_side == 0.0) break;
    if( top_side > 0) { move_right(4); continue; }
    if( top_side < 0) { move_left(4); continue; }
    if( left_side > 0) { move_forward(4); continue; }
    if( left_side < 0) { move_back(4); continue; }
  }

}

int MecanumWheelsStep::get_side(Octoliner &octoliner, int sen){
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

void MecanumWheelsStep::move_step_forward() {
    Serial.println("move_step_forward");
    set_to_contre();
    for(int i = 0; i< 5; i++) {
      move_forward(20);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(left_side < -6 || right_side > 6) break;
      delay(200);
    }
    move_forward(120);
    for(int i = 0; i< 10; i++) {
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(left_side > -5 || right_side < 5) break;
      move_forward(10);
    }
    set_to_contre();
    delay(200);
    global_step = global_step + 1;
}

void MecanumWheelsStep::move_step_back() { // avg load
    Serial.println("move_step_back");
    set_to_contre();
    for(int i = 0; i< 5; i++) {
      move_back(20);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(right_side < -6 || left_side > 6) break;
      delay(200);
    }
    move_back(120);
    for(int i = 0; i< 10; i++) {
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(right_side > -5 || left_side < 5) break;
      move_back(10);
    }
    set_to_contre();
    delay(200);
    global_step = global_step - 1;
}

void MecanumWheelsStep::rotate_left_90() {
    Serial.println("rotate_left_90");
    set_to_contre();
    delay(200);
    turn_left(330);
    delay(200);
    set_to_contre();
    delay(200);
    global_angle = global_angle - 90;
}

void MecanumWheelsStep::rotate_right_90() {
    Serial.println("rotate_right_90");
    set_to_contre();
    delay(200);
    turn_right(330);
    delay(200);
    set_to_contre();
    delay(200);
    global_angle = global_angle + 90;
}

void MecanumWheelsStep::turn_right(int ms){
  Serial.println("turn_right");
  // Подаём питание на двигатель
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
  digitalWrite(enablePin3, HIGH); digitalWrite(enablePin4, HIGH);
  // Задаём направление вращения
  digitalWrite(directionPin1, HIGH); // fr
  digitalWrite(directionPin2, HIGH); // br
  digitalWrite(directionPin3, HIGH); // fl
  digitalWrite(directionPin4, HIGH); //lb
  make_steps(ms);
}
void MecanumWheelsStep::turn_left(int ms){
  Serial.println("turn_left");
  // Подаём питание на двигатель
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
  digitalWrite(enablePin3, HIGH); digitalWrite(enablePin4, HIGH);
  // Задаём направление вращения
  digitalWrite(directionPin1, LOW); // fr
  digitalWrite(directionPin2, LOW); // br
  digitalWrite(directionPin3, LOW); // fl
  digitalWrite(directionPin4, LOW); //lb
  make_steps(ms);
}
void MecanumWheelsStep::move_left(int ms){
  Serial.println("move_left");
  // Подаём питание на двигатель
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
  digitalWrite(enablePin3, HIGH); digitalWrite(enablePin4, HIGH);
  // Задаём направление вращения
  digitalWrite(directionPin1, LOW);
  digitalWrite(directionPin2, HIGH);
  digitalWrite(directionPin3, LOW); //rb
  digitalWrite(directionPin4, HIGH); //lb
  make_steps(ms);
}
void MecanumWheelsStep::move_right(int ms){
  Serial.println("move_right");
  // Подаём питание на двигатель
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
  digitalWrite(enablePin3, HIGH); digitalWrite(enablePin4, HIGH);
  // Задаём направление вращения
  digitalWrite(directionPin1, HIGH);
  digitalWrite(directionPin2, LOW);
  digitalWrite(directionPin3, HIGH); //rb
  digitalWrite(directionPin4, LOW); //lb
  make_steps(ms);
}
void MecanumWheelsStep::move_forward(int ms){
  Serial.println("move_forward");
  // Подаём питание на двигатель
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
  digitalWrite(enablePin3, HIGH); digitalWrite(enablePin4, HIGH);
  // Задаём направление вращения
  digitalWrite(directionPin1, LOW);
  digitalWrite(directionPin2, LOW);
  digitalWrite(directionPin3, HIGH); //rb
  digitalWrite(directionPin4, HIGH); //lb
  make_steps(ms);
}
void MecanumWheelsStep::move_back(int ms){
  Serial.println("move_back");
  // Подаём питание на двигатель
  digitalWrite(enablePin1, HIGH); digitalWrite(enablePin2, HIGH);
  digitalWrite(enablePin3, HIGH); digitalWrite(enablePin4, HIGH);
  // Задаём направление вращения
  digitalWrite(directionPin1, HIGH);
  digitalWrite(directionPin2, HIGH);
  digitalWrite(directionPin3, LOW); //rb
  digitalWrite(directionPin4, LOW); //lb
  make_steps(ms);
}

void MecanumWheelsStep::make_steps(int steps){
  // Делаем 50 шагов
  for (int i = 0; i < steps; ++i) {
    // Делаем шаг
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);
    digitalWrite(stepPin3, HIGH);
    digitalWrite(stepPin4, HIGH);
    delay(10);
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
    digitalWrite(stepPin3, LOW);
    digitalWrite(stepPin4, LOW);
    delay(10);
  }
  delay(200);
}



int MecanumWheelsStep::find_current_angle1(){
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

int MecanumWheelsStep::find_current_angle2(){
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
