/*
  CrawlerWheels.h - Library for robo wheels.
  Created by krill221 , june 2, 2019.
*/

#include "Arduino.h"
#include "Morse.h"

CrawlerWheels::CrawlerWheels(int left, int right, int sensitivity, int brightness)
{
  octoliner_top(42);
  octoliner_bottom(43);
  octoliner_left(44);
  octoliner_right(45);

  last_top = 0;
  last_bottom = 0;
  last_left = 0;
  last_right = 0;

  /// orientation
  global_angle = 0;
  global_step = 0;

  init_wheels(left, right);
  init_sensors(sensitivity, brightness);
  Serial.println("init done");
}


void CrawlerWheels::init_wheels(int l, int r){
  myservo_l.attach(l); myservo_l.write(95);
  myservo_r.attach(r); myservo_r.write(95);

}

void CrawlerWheels::init_sensors(int sensitivity, int brightness){
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
}

void CrawlerWheels::print_coords(){
  float top_side = get_side(octoliner_top, 1);;
  float bottom_side = get_side(octoliner_bottom, 2);
  float left_side = get_side(octoliner_left, 4);
  float right_side = get_side(octoliner_right, 3);

  Serial.print(" t: "); Serial.print(top_side); Serial.print(" b: "); Serial.print(bottom_side); Serial.print(" l: "); Serial.print(left_side); Serial.print(" r: "); Serial.println(right_side);
}


void CrawlerWheels::shift_to_center() { // add error to 10
    // 1. hard shift to center circle (out lines)
    for(int i =0; i < 10; i++){
      Serial.print("hard shift 1 ");
      boolean o_line_top_side = is_out_line_side(octoliner_top);
      boolean o_line_bottom_side = is_out_line_side(octoliner_bottom);
      boolean o_line_left_side = is_out_line_side(octoliner_left);
      boolean o_line_right_side = is_out_line_side(octoliner_right);
      boolean d_line_top_side = is_double_line_side(octoliner_top);
      boolean d_line_bottom_side = is_double_line_side(octoliner_bottom);
      boolean d_line_left_side = is_double_line_side(octoliner_left);
      boolean d_line_right_side = is_double_line_side(octoliner_right);
      if ( o_line_left_side && o_line_right_side ) { Serial.println(" outline l r"); move_back(400); continue;}
      if ( o_line_top_side ) {
        if (d_line_right_side) turn_left(200);
        if (d_line_left_side) turn_right(200);
        Serial.println(" outline t"); move_back(400); continue;
      }
      if ( o_line_bottom_side ) {
        if (d_line_right_side) turn_right(200);
        if (d_line_left_side) turn_left(200);
        Serial.println(" outline t"); move_forward(400); continue;
      }
      if ( d_line_top_side ) { Serial.println(" doubleline t"); move_forward(400); continue;}
      if ( d_line_bottom_side ) { Serial.println(" doubleline b"); move_back(400); continue; }
      if ( o_line_left_side ) { Serial.println(" outline l"); move_right(800); continue; }
      if ( o_line_right_side ) { Serial.println(" outline r"); move_left(800); continue; }
      Serial.println(" --none");
      break;
    }
    // 1. hard shift to center circle (double lines)
    for(int i =0; i < 10; i++){
      Serial.print("hard shift 2 ");
      boolean d_line_top_side = is_double_line_side(octoliner_top);
      boolean d_line_bottom_side = is_double_line_side(octoliner_bottom);
      boolean d_line_left_side = is_double_line_side(octoliner_left);
      boolean d_line_right_side = is_double_line_side(octoliner_right);
      if ( d_line_left_side ) { Serial.println(" doubleline l"); move_left(600); continue; }
      if ( d_line_right_side ) { Serial.println(" doubleline r"); move_right(600); continue; }
      Serial.println(" --none");
      break;
    }
    // 2. soft shift to center circle (same sides)
    for(int i =0; i < 10; i++){
      int top_side = get_side(octoliner_top, 1);;
      int bottom_side = get_side(octoliner_bottom, 2);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      // correction by 4 sides
      if(top_side == bottom_side && top_side == left_side) right_side = top_side;
      if(top_side == bottom_side && top_side == right_side) left_side = top_side;
      if(top_side == left_side && top_side == right_side) bottom_side = top_side;
      if(bottom_side == left_side && bottom_side == right_side) top_side = bottom_side;
      if(top_side == right_side && abs( abs(bottom_side) - abs(left_side) ) < 2 ) bottom_side = left_side;
      if(top_side == left_side && abs( abs(bottom_side) - abs(right_side) ) < 2) bottom_side = right_side;
      Serial.print(top_side); Serial.print(" "); Serial.print(bottom_side); Serial.print(" "); Serial.print(left_side); Serial.print(" "); Serial.println(right_side);
      if(top_side == 0 && bottom_side  == 0 && left_side == 0 && right_side == 0){ Serial.println("done --------------"); break; }

      // same sides
      Serial.print("soft shift same sides ");
      if (left_side > 0 && right_side < 0  && abs(left_side - right_side) <= 1 ) { move_forward(40); continue; }
      if (left_side < 0 && right_side > 0  && abs(left_side - right_side) <= 1 ) { move_back(40); continue; }
      if (left_side > 0 && right_side < 0  && abs(left_side - right_side) <= 2 ) { move_forward(60); continue; }
      if (left_side < 0 && right_side > 0  && abs(left_side - right_side) <= 2 ) { move_back(60); continue; }
      if (left_side > 0 && right_side < 0  && abs(left_side - right_side) <= 4 ) { move_forward(100); continue; }
      if (left_side < 0 && right_side > 0  && abs(left_side - right_side) <= 4 ) { move_back(100); continue; }
      if (left_side > 0 && right_side < 0  && abs(left_side - right_side) <= 5 ) { move_forward(150); continue; }
      if (left_side < 0 && right_side > 0  && abs(left_side - right_side) <= 5 ) { move_back(150); continue; }
      if (left_side > 0 && right_side < 0  && abs(left_side - right_side) <= 6 ) { move_forward(200); continue; }
      if (left_side < 0 && right_side > 0  && abs(left_side - right_side) <= 6 ) { move_back(200); continue; }
      if (left_side > 0 && right_side < 0  && abs(left_side - right_side) <= 8 ) { move_forward(300); continue; }
      if (left_side < 0 && right_side > 0  && abs(left_side - right_side) <= 8 ) { move_back(300); continue; }
      if (left_side > 0 && right_side < 0  && abs(left_side - right_side) > 8 ) { move_forward(400); continue; }
      if (left_side < 0 && right_side > 0  && abs(left_side - right_side) > 8 ) { move_back(400); continue; }

      // TODO find angle
      if (top_side > 0 && bottom_side < 0 && abs(top_side - bottom_side) <= 2) { move_right(200); continue; }
      if (top_side < 0 && bottom_side > 0 && abs(top_side - bottom_side) <= 2) { move_left(200); continue; }
      if (top_side > 0 && bottom_side < 0 && abs(top_side - bottom_side) <= 3) { move_right(300); continue; }
      if (top_side < 0 && bottom_side > 0 && abs(top_side - bottom_side) <= 3) { move_left(300); continue; }
      if (top_side > 0 && bottom_side < 0 && abs(top_side - bottom_side) <= 4) { move_right(400); continue; }
      if (top_side < 0 && bottom_side > 0 && abs(top_side - bottom_side) <= 4) { move_left(400); continue; }
      if (top_side > 0 && bottom_side < 0 && abs(top_side - bottom_side) <= 5) { move_right(500); continue; }
      if (top_side < 0 && bottom_side > 0 && abs(top_side - bottom_side) <= 5) { move_left(500); continue; }
      if (top_side > 0 && bottom_side < 0 && abs(top_side - bottom_side) <= 6) { move_right(600); continue; }
      if (top_side < 0 && bottom_side > 0 && abs(top_side - bottom_side) <= 6) { move_left(600); continue; }
      if (top_side > 0 && bottom_side < 0 && abs(top_side - bottom_side) <= 7) { move_right(700); continue; }
      if (top_side < 0 && bottom_side > 0 && abs(top_side - bottom_side) <= 7) { move_left(700); continue; }
      if (top_side > 0 && bottom_side < 0 && abs(top_side - bottom_side) <= 8) { move_right(800); continue; }
      if (top_side < 0 && bottom_side > 0 && abs(top_side - bottom_side) <= 8) { move_left(800); continue; }
      if (top_side > 0 && bottom_side < 0 && abs(top_side - bottom_side) > 8) { move_right(1000); continue; }
      if (top_side < 0 && bottom_side > 0 && abs(top_side - bottom_side) > 8) { move_left(1000); continue; }
      Serial.println(" --none");

      // different sides
      Serial.print("soft shift different sides ");
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 2 ) { move_back(40);  continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 2 ) { move_forward(40); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 3 ) {  move_back(60); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 3 ) {  move_forward(60); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 4 ) {  move_back(100); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 4 ) {  move_forward(100); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 6 ) {  move_back(200); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 6 ) {  move_forward(200); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) > 6 ) {  move_back(300); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) > 6 ) {  move_forward(300); continue; }


      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 2  ) {  move_forward(40);  continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) ) {  move_forward(60); continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 2  ) { move_back(40);  continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) ) {  move_back(60); continue; }

      if ( (left_side < 0 && right_side < 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 2  ) { move_forward(40);  continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) > abs(right_side) ) ) {  move_forward(60); continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 2  ) { move_back(40); continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) > abs(right_side) ) ) {  move_back(60); continue; }
      Serial.print("zzz ");

      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) < abs(bottom_side) ) && abs(abs(top_side) - abs(bottom_side) ) < 2  ) { move_left(400);  continue; }
      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) < abs(bottom_side) ) ) { move_left(400); continue; }

      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) > abs(bottom_side) ) && abs(abs(top_side) - abs(bottom_side) ) < 2  ) { move_right(400); continue; }
      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) > abs(bottom_side) ) ) { move_right(400); continue; }

      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) < abs(bottom_side) ) && abs(abs(top_side) - abs(bottom_side) ) < 2  ) { move_right(400); continue; }
      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) < abs(bottom_side) ) ) { move_right(400); continue; }

      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) > abs(bottom_side) ) && abs(abs(top_side) - abs(bottom_side) ) <= 2  ) { move_left(400); continue; }
      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) > abs(bottom_side) ) )  {  move_left(400); continue; }

      Serial.println(" --none");
      break;
    }
}

void CrawlerWheels::rotate_to_closer_line() {
  for(int i =0; i < 10; i++){
    if(i == 9) Serial.println("Out of time --------");

    shift_to_center();

    Serial.println("rotate_to_closer_line");
    int top_side = get_side(octoliner_top, 1);;
    int bottom_side = get_side(octoliner_bottom, 2);
    int left_side = get_side(octoliner_left, 4);
    int right_side = get_side(octoliner_right, 3);

    // super soft turn
    if(bottom_side == 0 && left_side == 0 && right_side == 0 && top_side < 1) { turn_r_f(40); continue; }
    if(bottom_side == 0 && left_side == 0 && right_side == 0 && top_side > 1) { turn_l_f(40); continue; }
    if(bottom_side == 0 && left_side == 0 && right_side == 0 && top_side < 0) { turn_r_f(20); continue; }
    if(bottom_side == 0 && left_side == 0 && right_side == 0 && top_side > 0) { turn_l_f(20); continue; }

    // correction by 4 sides
    if(top_side == bottom_side && top_side == left_side) right_side = top_side;
    if(top_side == bottom_side && top_side == right_side) left_side = top_side;
    if(top_side == left_side && top_side == right_side) bottom_side = top_side;
    if(bottom_side == left_side && bottom_side == right_side) top_side = bottom_side;
    if(top_side == right_side && abs( abs(bottom_side) - abs(left_side) ) < 2 ) bottom_side = left_side;
    if(top_side == left_side && abs( abs(bottom_side) - abs(right_side) ) < 2) bottom_side = right_side;
    Serial.print(top_side); Serial.print(" "); Serial.print(bottom_side); Serial.print(" "); Serial.print(left_side); Serial.print(" "); Serial.println(right_side);
    if(top_side == 0 && bottom_side  == 0 && left_side == 0 && right_side == 0){ Serial.println("done --------------"); break; }


    //turn
    if (top_side == bottom_side && top_side < 0 ) { turn_left(200); continue; }
    if (top_side == bottom_side && top_side > 0 ) { turn_right(200); continue; }

    if (top_side == 0 && bottom_side < 0 && bottom_side >= -2 ) { turn_r_f(60); continue; }
    if (top_side == 0 && bottom_side < 0 && bottom_side > -4 ) { turn_r_f(100); continue; }
    if (top_side == 0 && bottom_side < 0 && bottom_side <= -4 ) { turn_r_f(200); continue; }
    if (top_side == 0 && bottom_side > 0 && bottom_side <= 2) { turn_l_f(60); continue; }
    if (top_side == 0 && bottom_side > 0 && bottom_side < 4) { turn_l_f(100); continue; }
    if (top_side == 0 && bottom_side > 0 && bottom_side >= 4) { turn_l_f(200); continue; }
    if (bottom_side == 0 && top_side < 0  && top_side >= -2 ) { turn_l_b(60); continue; }
    if (bottom_side == 0 && top_side < 0  && top_side > -4 ) { turn_l_b(100); continue; }
    if (bottom_side == 0 && top_side < 0  && top_side <= -4 ) { turn_l_b(200); continue; }
    if (bottom_side == 0 && top_side > 0 && top_side <= 2 ) { turn_r_b(60); continue; }
    if (bottom_side == 0 && top_side > 0 && top_side < 4 ) { turn_r_b(100); continue; }
    if (bottom_side == 0 && top_side > 0 && top_side >= 4 ) { turn_r_b(200); continue; }

    if (bottom_side == right_side && bottom_side == left_side && bottom_side > top_side  ) { turn_r_f(60); continue; }
    if (bottom_side == right_side && bottom_side == left_side && bottom_side < top_side  ) { turn_l_f(60); continue; }

    if (top_side == right_side && top_side == left_side && top_side > bottom_side  ) { turn_r_b(60); continue; }
    if (top_side == right_side && top_side == left_side && top_side < bottom_side  ) { turn_l_b(60); continue; }

    if (top_side == bottom_side && top_side == right_side && left_side < top_side  ) { turn_l_b(60); continue; }
    if (top_side == bottom_side && top_side == right_side && left_side > top_side  ) { turn_l_f(60); continue; }


    if (top_side == right_side && top_side == left_side && top_side == bottom_side && top_side > 0  ) { turn_right(200); continue; }
    if (top_side == right_side && top_side == left_side && top_side == bottom_side && top_side < 0  ) { turn_left(200); continue; }

    if (top_side == right_side && bottom_side == left_side ) { turn_left(100); continue; }
    if (top_side == left_side && bottom_side == right_side ) { turn_right(100); continue; }


    break;
  }
}


boolean CrawlerWheels::is_out_line_side(Octoliner &octoliner) {
  boolean o_line = false; int lines = 0;
  int data[8];
  for (int i = 0; i < 8; i++) {
    data[i] = octoliner.analogRead(i);
  }
  for (int i = 0; i < 8; i++) { if( data[i] < 60 ) lines++;  }
  if(lines == 8) { return true; }
  return false;
}

boolean CrawlerWheels::is_double_line_side(Octoliner &octoliner) {
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

int CrawlerWheels::get_side(Octoliner &octoliner, int sen){
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

void CrawlerWheels::move_step_forward() {
    Serial.println("move_step_forward");
    rotate_to_closer_line();
    for(int i = 0; i< 10; i++) {
      move_forward(200);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(left_side < -6 || right_side > 6) break;
      delay(500);
    }
    move_forward(400);
    delay(500);
    move_forward(400);
    delay(500);
    move_forward(400);
    delay(500);
    for(int i = 0; i< 10; i++) {
      move_forward(200);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(left_side > -5 || right_side < 5) break;
      delay(500);
    }
    rotate_to_closer_line();
    delay(500);
    global_step = global_step + 1;
}

void CrawlerWheels::move_step_back() { // avg load
    Serial.println("move_step_back");
    rotate_to_closer_line();
    for(int i = 0; i< 10; i++) {
      move_back(200);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(right_side < -6 || left_side > 6) break;
      delay(500);
    }
    move_back(400);
    delay(500);
    move_back(400);
    delay(500);
    move_back(400);
    delay(500);
    for(int i = 0; i< 10; i++) {
      move_back(200);
      int left_side = get_side(octoliner_left, 4);
      int right_side = get_side(octoliner_right, 3);
      if(right_side > -5 || left_side < 5) break;
      delay(500);
    }
    rotate_to_closer_line();
    delay(500);
    global_step = global_step - 1;
}

void CrawlerWheels::rotate_left_90() {
    Serial.println("rotate_left_90");
    rotate_to_closer_line();
    for(int i = 0; i< 13; i++) {
      turn_left(300);
      delay(900);
    }
    rotate_to_closer_line();
    delay(900);
    global_angle = global_angle - 90;
}

void CrawlerWheels::rotate_right_90() {
    Serial.println("rotate_right_90");
    rotate_to_closer_line();
    for(int i = 0; i< 13; i++) {
      turn_right(300);
      delay(900);
    }
    rotate_to_closer_line();
    delay(900);
    global_angle = global_angle + 90;
}


void CrawlerWheels::turn_right(int ms){
  Serial.println("turn_right");
  myservo_r.write(185); //left
  myservo_l.write(185); //right
  delay(ms);
  myservo_r.write(95);
  myservo_l.write(95);
  delay(400);
}
void CrawlerWheels::turn_left(int ms){
  Serial.println("turn_left");
  myservo_l.write(5); //right
  myservo_r.write(5); //left
  delay(ms);
  myservo_r.write(95);
  myservo_l.write(95);
  delay(400);
}
void CrawlerWheels::move_left(int ms){
  Serial.println("move_left");
  turn_r_f(ms);
  turn_l_f(ms);
  turn_r_b(ms);
  turn_l_b(ms);
}
void CrawlerWheels::move_right(int ms){
  Serial.println("move_right");
  turn_l_f(ms);
  turn_r_f(ms);
  turn_l_b(ms);
  turn_r_b(ms);
}
void CrawlerWheels::move_forward(int ms){
  Serial.println("move_forward");
  myservo_r.write(0);
  myservo_l.write(185);
  delay(ms);
  myservo_r.write(95);
  myservo_l.write(95);
  delay(400);
}
void CrawlerWheels::move_back(int ms){
  Serial.println("move_back");
  myservo_r.write(185);
  myservo_l.write(0);
  delay(ms);
  myservo_r.write(95);
  myservo_l.write(95);
  delay(400);
}
void CrawlerWheels::turn_r_f(int ms){
  myservo_r.write(0);
  delay(ms);
  myservo_r.write(95);
  delay(400);
}
void CrawlerWheels::turn_l_f(int ms){
  myservo_l.write(360); //- f
  delay(ms);
  myservo_l.write(95);
  delay(400);
}
void CrawlerWheels::turn_r_b(int ms){
  myservo_r.write(360); //- back
  delay(ms);
  myservo_r.write(95);
  delay(400);
}
void CrawlerWheels::turn_l_b(int ms){
  myservo_l.write(0);
  delay(ms);
  myservo_l.write(95);
  delay(400);
}



int CrawlerWheels::find_current_angle(int top, int bot, int right, int left){
  float cur_angle;   int k;
  int X11 = top; int X12 = (bot*-1);
  if(X11 >= X12) k = X11 - X12; if(X11 < X12) k = X12 - X11;
  cur_angle = 180 - 90 - (((atan((float)14/(float)k)) * 180.0)/PI);
  if(X11 > X12) cur_angle = -cur_angle;
  Serial.print("current angle: "); Serial.print(cur_angle);
  if(X11 == X12) Serial.println(" --||--");
  if(X11 > X12)  Serial.println(" --\\--");
  if(X11 < X12)  Serial.println(" --//--");
  return cur_angle;
}


int CrawlerWheels::find_need_angle(int top, int bot, int right, int left){
  int xi; int yi;
  float need_angle;
  // координаты отрезков
  int X11 = top; int Y11 = 7;    int X12 = (bot*-1); int Y12 = -7;
  int X21 = -7;  int Y21 = left; int X22 = 7;        int Y22 = (right*-1);
  // параметры отрезков
  int a1 = Y11 - Y12; int b1 = X12 - X11;
  int a2 = Y21 - Y22; int b2 = X22 - X21;
  // координаты точки пересечения
  int d = a1 * b2 - a2 * b1;
  if( d != 0 ) {
    int c1 = Y12 * X11 - X12 * Y11;
    int c2 = Y22 * X21 - X22 * Y21;
    xi = (b1 * c2 - b2 * c1) / d;
    yi = (a2 * c1 - a1 * c2) / d;
  } else { xi = 0; yi = 0; }
  if(yi != 0 ) {
    need_angle = (((atan((float)-xi/(float)-yi)) * 180.0)/PI);
  }
  if(yi == 0 ) {
    need_angle = (((atan((float)(-(xi+1))/(float)(-(yi+1)))) * 180.0)/PI);
  }
  if(xi == 0 && yi < 0) need_angle =  -45;
  if(xi == 0 && yi > 0) need_angle =  45;
  if(xi == 0 && yi == 0) need_angle =  0;
  if( (xi == 0 && yi == 0) || (xi == 1 && yi == 0) || (xi == -1 && yi == 0) ) need_angle =  0;
  if( (xi == 0 && yi == 1) || (xi == 0 && yi == -1) ) need_angle =  0;
  Serial.print("need angle:  "); Serial.println(need_angle);
  return need_angle;
}

int CrawlerWheels::find_xi(int top, int bot, int right, int left){
  int xi; int yi;
  // координаты отрезков
  int X11 = top; int Y11 = 7;    int X12 = (bot*-1); int Y12 = -7;
  int X21 = -7;  int Y21 = left; int X22 = 7;        int Y22 = (right*-1);

  // параметры отрезков
  int a1 = Y11 - Y12; int b1 = X12 - X11;
  int a2 = Y21 - Y22; int b2 = X22 - X21;

  // координаты точки пересечения
  int d = a1 * b2 - a2 * b1;
  if( d != 0 ) {
    int c1 = Y12 * X11 - X12 * Y11;
    int c2 = Y22 * X21 - X22 * Y21;
    xi = (b1 * c2 - b2 * c1) / d;
    yi = (a2 * c1 - a1 * c2) / d;
  } else {
     xi = 0;
     yi = 0;
  }
  Serial.print("xi: "); Serial.println(xi);
  return xi;
}

int CrawlerWheels::find_yi(int top, int bot, int right, int left){
  int xi; int yi;
  // координаты отрезков
  int X11 = top; int Y11 = 7;    int X12 = (bot*-1); int Y12 = -7;
  int X21 = -7;  int Y21 = left; int X22 = 7;        int Y22 = (right*-1);

  // параметры отрезков
  int a1 = Y11 - Y12; int b1 = X12 - X11;
  int a2 = Y21 - Y22; int b2 = X22 - X21;

  // координаты точки пересечения
  int d = a1 * b2 - a2 * b1;
  if( d != 0 ) {
    int c1 = Y12 * X11 - X12 * Y11;
    int c2 = Y22 * X21 - X22 * Y21;
    xi = (b1 * c2 - b2 * c1) / d;
    yi = (a2 * c1 - a1 * c2) / d;
  } else {
     xi = 0;
     yi = 0;
  }
  Serial.print("yi: "); Serial.println(yi);
  return yi;
}
