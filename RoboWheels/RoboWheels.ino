#include <Wire.h>
#include <Octoliner.h>
#include <Servo.h>

Servo myservo1;
Servo myservo2;
Octoliner octoliner_top(42);
Octoliner octoliner_bottom(43);
Octoliner octoliner_left(44);
Octoliner octoliner_right(45);
int last_top = 0;
int last_bottom = 0;
int last_left = 0;
int last_right = 0;

// -1  0  1
//  1    -1
//  0     0
// -1     1
//  1  0 -1

void setup() {
  Serial.begin(9600);

  // wheels init
  myservo1.attach(5); myservo1.write(90);
  myservo2.attach(6); myservo2.write(90);

  // sensors init
  int sensitivity = 200;
  int brightness = 255;
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

  Serial.println("init done");

  rotate_to_closer_line();
  rotate_to_left_line();
  rotate_to_closer_line();
}

void loop() {
    Serial.println("Loop:");
    float top_side = get_side(octoliner_top, 1);;
    float bottom_side = get_side(octoliner_bottom, 2);
    float left_side = get_side(octoliner_left, 4);
    float right_side = get_side(octoliner_right, 3);

    Serial.print(" top_side: ");
    Serial.print(top_side);
    Serial.print(" bottom_side: ");
    Serial.print(bottom_side);
    Serial.print(" left_side: ");
    Serial.print(left_side);
    Serial.print(" right_side: ");
    Serial.print(right_side);
    Serial.println();
    delay(500);
}

//////
//////

void shift_to_center() { // add error to 10
    // 1. hard shift to center circle (double lines)
    for(int i =0; i < 10; i++){
      Serial.println("hard shift_to_center");
      boolean d_line_top_side = double_line_side(octoliner_top);;
      boolean d_line_bottom_side = double_line_side(octoliner_bottom);;
      boolean d_line_left_side = double_line_side(octoliner_left);;
      boolean d_line_right_side = double_line_side(octoliner_right);;
      if ( d_line_top_side ) { move_forward(100); continue;}
      if ( d_line_bottom_side ) { move_back(100); continue; }
      if ( d_line_left_side ) { move_left(600); continue; }
      if ( d_line_right_side ) { move_right(600); continue; }
      break;
    }
    // 2. soft shift to center circle (same sides)
    for(int i =0; i < 10; i++){
      Serial.println("soft shift_to_center");
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
      if (left_side > 0 && right_side < 0) { move_forward(100); continue; }
      if (left_side < 0 && right_side > 0) { move_back(100); continue; }
      if (top_side > 0 && bottom_side < 0) { move_right(600); continue; }
      if (top_side < 0 && bottom_side > 0) { move_left(600); continue; }
      // different sides
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) ) {  move_back(100); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) ) {  move_forward(100); continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) ) {  move_forward(100); continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) > abs(right_side) ) ) {  move_back(100); continue; }
      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) < abs(bottom_side) ) ) { move_left(600); continue; }
      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) > abs(bottom_side) ) ) { move_right(600); continue; }
      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) < abs(bottom_side) ) ) { move_right(600); continue; }
      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) > abs(bottom_side) ) )  {  move_left(600); continue; }
      break;
    }
}

void rotate_to_closer_line() {
  for(int i =0; i < 10; i++){
    if(i == 9) Serial.println("Out of time --------");
    shift_to_center();
    Serial.println("rotate_to_closer_line");
    int top_side = get_side(octoliner_top, 1);;
    int bottom_side = get_side(octoliner_bottom, 2);
    int left_side = get_side(octoliner_left, 4);
    int right_side = get_side(octoliner_right, 3);
    // correction by 4 sides
    if(top_side == bottom_side && top_side  == left_side && abs( abs(right_side) - abs(top_side) ) < 2 ) right_side = top_side;
    if(top_side == bottom_side && top_side == right_side && abs( abs(left_side) - abs(top_side) ) < 2 ) left_side = top_side;
    if(top_side == left_side && top_side == right_side && abs( abs(top_side) - abs(bottom_side) ) < 2 ) bottom_side = top_side;
    if(bottom_side == left_side && bottom_side == right_side && abs( abs(bottom_side) - abs(top_side) ) < 2) top_side = bottom_side;
    if(top_side == right_side && abs( abs(bottom_side) - abs(left_side) ) < 2 ) bottom_side = left_side;
    if(top_side == left_side && abs( abs(bottom_side) - abs(right_side) ) < 2) bottom_side = right_side;
    
    Serial.print(top_side); Serial.print(" "); Serial.print(bottom_side); Serial.print(" "); Serial.print(left_side); Serial.print(" "); Serial.println(right_side);
    if(top_side == 0 && bottom_side  == 0 && left_side == 0 && right_side == 0){ Serial.println("done --------------"); break; }
    //turn
    if (top_side == bottom_side && top_side < 0 ) { turn_left(200); continue; }
    if (top_side == bottom_side && top_side > 0 ) { turn_right(200); continue; }
    if (top_side == 0 && bottom_side < 0) { turn_r_f(100); continue; }
    if (top_side == 0 && bottom_side > 0) { turn_l_f(100); continue; }
    if (bottom_side == 0 && top_side < 0) { turn_l_b(100); continue; }
    if (bottom_side == 0 && top_side > 0) { turn_r_b(100); continue; }
    
    if (bottom_side == right_side && bottom_side == left_side && bottom_side > top_side  ) { turn_r_f(100); continue; }
    if (bottom_side == right_side && bottom_side == left_side && bottom_side < top_side  ) { turn_l_f(100); continue; }
    if (top_side == right_side && top_side == left_side && top_side > bottom_side  ) { turn_r_b(100); continue; }
    if (top_side == right_side && top_side == left_side && top_side < bottom_side  ) { turn_l_b(100); continue; }

    if (top_side == right_side && top_side == left_side && top_side == bottom_side && top_side > 0  ) { turn_right(200); continue; }
    if (top_side == right_side && top_side == left_side && top_side == bottom_side && top_side < 0  ) { turn_left(200); continue; }
        
    if (top_side == right_side && bottom_side == left_side ) { turn_left(200); continue; }
    if (top_side == left_side && bottom_side == right_side ) { turn_right(200); continue; }
    
    break;
  }
}

void rotate_to_left_line() {
  turn_left(600);
  for(int i =0; i < 5; i++){
    //shift_to_center();
    Serial.println("rotate_to_left_line");
    float top_side = get_side(octoliner_top, 1);;
    if (top_side > 0) turn_left(300);
  }
  turn_left(300);
}

void rotate_to_right_line() {
  turn_right(600);
  for(int i =0; i < 10; i++){
    //shift_to_center();
    Serial.println("rotate_to_right_line");
    float top_side = get_side(octoliner_top, 1);;
    if (top_side < 0) turn_right(300);
  }
}

void rotate_left_90() {
    Serial.println("rotate_left_90");
    rotate_to_left_line();
    rotate_to_closer_line();
}

void rotate_right_90() {
    Serial.println("rotate_right_90");
    rotate_to_right_line();
    rotate_to_closer_line();
}


// get coordinates
boolean double_line_side(Octoliner &octoliner) {
  boolean d_line = false; float sum = 0; float avg = 0; int lines = 0;
  int data[8];
  Serial.print("d line: ");
  for (int i = 0; i < 8; i++) {
    data[i] = octoliner.analogRead(i); sum += data[i];
    Serial.print(data[i]);
    Serial.print(" ");
  }
  avg = sum/8;
  for (int i = 0; i < 8; i++) {
    if( data[i] > avg && (data[i] - avg) > 30) lines++;
  }
  if( lines > 3){
    d_line = true;
    Serial.println(" | >3");
  } else {
    if( (data[0] > avg && (data[0] - avg) > 30 ) && (data[7] > avg && (data[7] - avg) > 30 )){
      d_line = true;
      Serial.println(" | 1--1");
    } else {
        Serial.println(" | none");
    }
  }
  return d_line;
}

int get_side(Octoliner &octoliner, int sen){
  int data[8]; int sum = 0; int lines = 0; int side = 0;
  for (int i = 0; i < 8; i++) {
    data[i] = octoliner.analogRead(i);
    sum += data[i];
    Serial.print(data[i]);
    Serial.print(" ");
  }
  for (int i = 0; i < 8; i++) {
    if( data[i] > sum/8 && (data[i] - sum/8) > 30) lines++;
  }
  Serial.print(" | ");
  Serial.print(lines);
  if(lines != 0){
    side = round( octoliner.mapLine(data) * 10 );
    if(sen == 1) last_top = side/abs(side)*10; // top
    if(sen == 2) last_bottom = side/abs(side)*10; // bottom
    if(sen == 3) last_right = side/abs(side)*10; // right
    if(sen == 4) last_left = side/abs(side)*10; // left
  } else {
    if(sen == 1) side = last_top;
    if(sen == 2) side = last_bottom;
    if(sen == 3) side = last_right;
    if(sen == 4) side = last_left;
  }
  Serial.print(" | ");
  Serial.println(side);
  return side;
}

/// wheels moves
void turn_right(int ms){
  Serial.println("turn_right");
  myservo1.write(360); //right rev
  myservo2.write(360); //left
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_left(int ms){
  Serial.println("turn_left");
  myservo1.write(0); //right rev
  myservo2.write(0); //left
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void move_left(int ms){
  Serial.println("move_left");
  turn_r_f(ms);
  turn_l_f(ms);
  turn_r_b(ms);
  turn_l_b(ms);
}
void move_right(int ms){
  Serial.println("move_right");
  turn_l_f(ms);
  turn_r_f(ms);
  turn_l_b(ms);
  turn_r_b(ms);
}
void move_forward(int ms){
  Serial.println("move_forward");
  myservo1.write(360);
  myservo2.write(0);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void move_back(int ms){
  Serial.println("move_back");
  myservo1.write(0);
  myservo2.write(360);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_r_f(int ms){
  Serial.println("turn_r_f");
  myservo2.write(0);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_l_f(int ms){
  Serial.println("turn_l_f");
  myservo1.write(360);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_r_b(int ms){
  Serial.println("turn_r_b");
  myservo2.write(360);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_l_b(int ms){
  Serial.println("turn_l_b");
  myservo1.write(0);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
