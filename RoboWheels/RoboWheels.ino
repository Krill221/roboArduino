#include <Wire.h>
#include <Octoliner.h>
#include <Servo.h>

Servo myservo1;
Servo myservo2;
Octoliner octoliner_top(42);
Octoliner octoliner_bottom(43);
Octoliner octoliner_left(44);
Octoliner octoliner_right(45);

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
  Wire.begin();
  octoliner_bottom.begin();
  octoliner_bottom.setSensitivity(200); octoliner_bottom.setBrightness(255);
  delay(200);
  octoliner_top.begin();
  octoliner_top.setSensitivity(200); octoliner_top.setBrightness(255);
  delay(200);
  octoliner_left.begin();
  octoliner_left.setSensitivity(200); octoliner_left.setBrightness(255);
  delay(200);
  octoliner_right.begin();
  octoliner_right.setSensitivity(200); octoliner_right.setBrightness(255);
  delay(200);

  Serial.println("init done");

  rotate_to_closer_line();
}

void loop() {
    float top_side = get_top_side();
    float bottom_side = get_bottom_side();
    float left_side = get_left_side();
    float right_side = get_right_side();

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
    Serial.println("hard shift_to_center");
    // 1. hard shift to center circle (double lines)
    for(int i =0; i < 10; i++){
      boolean d_line_top_side = double_line_top_side();
      boolean d_line_bottom_side = double_line_bottom_side();
      boolean d_line_left_side = double_line_left_side();
      boolean d_line_right_side = double_line_right_side();
      if ( d_line_top_side ) { move_back(100); continue;}
      if ( d_line_bottom_side ) { move_forward(100); continue; }
      if ( d_line_left_side ) { move_right(300); continue; }
      if ( d_line_right_side ) { move_left(300); continue; }
      break;
    }
    Serial.println("soft shift_to_center");
    // 2. soft shift to center circle (same sides)
    for(int i =0; i < 10; i++){
      float top_side = get_top_side();
      float bottom_side = get_bottom_side();
      float left_side = get_left_side();
      float right_side = get_right_side();
      // same sides
      if (left_side > 0 && right_side < 0) { move_forward(100); continue; }
      if (left_side < 0 && right_side > 0) { move_back(100); continue; }
      if (top_side > 0 && bottom_side < 0) { move_right(300); continue; }
      if (top_side < 0 && bottom_side > 0) { move_left(300); continue; }
      // different sides
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) ) {  move_back(100); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) ) {  move_forward(100); continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) ) {  move_forward(100); continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) > abs(right_side) ) ) {  move_back(100); continue; }
      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) < abs(bottom_side) ) ) { move_left(300); continue; }
      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) > abs(bottom_side) ) ) { move_right(300); continue; }
      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) < abs(bottom_side) ) ) { move_right(300); continue; }
      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) > abs(bottom_side) ) )  {  move_left(300); continue; }
      break;
    }
}

void rotate_to_closer_line() {
  Serial.println("rotate_to_closer_line");
  //1. turn circle
  for(int i =0; i < 15; i++){
    shift_to_center();
    float top_side = get_top_side();
    float bottom_side = get_bottom_side();
    float left_side = get_left_side();
    float right_side = get_right_side();
    if (top_side == -bottom_side && top_side < 0 ) { turn_left(200); continue; }
    if (-top_side == bottom_side && top_side > 0 ) { turn_right(200); continue; }
    if (top_side == 0 && bottom_side < 0) { turn_r_f(300); continue; }
    if (top_side == 0 && bottom_side > 0) { turn_l_f(300); continue; }
    if (bottom_side == 0 && top_side < 0) { turn_r_b(300); continue; }
    if (bottom_side == 0 && top_side > 0) { turn_l_b(300); continue; }

    if (top_side == right_side && bottom_side == left_side ) { turn_left(200); continue; }
    if (top_side == left_side && bottom_side == right_side ) { turn_right(200); continue; }
    break;
  }
}

void rotate_to_left_line() {
  Serial.println("rotate_to_left_line");
  turn_left(300);
  //1. turn circle
  for(int i =0; i < 10; i++){
    shift_to_center();
    float top_side = get_top_side();
    if (top_side > 0) turn_left(300);
  }
}

void rotate_to_right_line() {
  Serial.println("rotate_to_right_line");
  turn_right(300);
  //1. turn circle
  for(int i =0; i < 10; i++){
    shift_to_center();
    float top_side = get_top_side();
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
boolean double_line_top_side() {
  boolean d_line = false; float sum = 0; float avg = 0; int lines = 0;
  int data[8];
  for (int i = 0; i < 8; i++) { data[i] = octoliner_top.analogRead(i); sum += data[i];}
  avg = sum/8;
  for (int i = 0; i < 8; i++) { if( data[i] > avg && (data[i] - avg) > 20) lines++; }
  if( lines > 2) d_line = true; if( data[0] > avg && data[7] > avg) d_line = true;
  return d_line;
}
boolean double_line_bottom_side() {
  boolean d_line = false; float sum = 0; float avg = 0; int lines = 0;
  int data[8];
  for (int i = 0; i < 8; i++) { data[i] = octoliner_bottom.analogRead(i); sum += data[i];}
  avg = sum/8;
  for (int i = 0; i < 8; i++) { if( data[i] > avg && (data[i] - avg) > 20) lines++; }
  if( lines > 2) d_line = true; if( data[0] > avg && data[7] > avg) d_line = true;
  return d_line;
}
boolean double_line_left_side() {
  boolean d_line = false; float sum = 0; float avg = 0; int lines = 0;
  int data[8];
  for (int i = 0; i < 8; i++) { data[i] = octoliner_left.analogRead(i); sum += data[i];}
  avg = sum/8;
  for (int i = 0; i < 8; i++) { if( data[i] > avg && (data[i] - avg) > 20) lines++; }
  if( lines > 2) d_line = true; if( data[0] > avg && data[7] > avg) d_line = true;
  return d_line;
}
boolean double_line_right_side() {
  boolean d_line = false; float sum = 0; float avg = 0; int lines = 0;
  int data[8];
  for (int i = 0; i < 8; i++) { data[i] = octoliner_right.analogRead(i); sum += data[i];}
  avg = sum/8;
  for (int i = 0; i < 8; i++) { if( data[i] > avg && (data[i] - avg) > 20) lines++; }
  if( lines > 2) d_line = true; if( data[0] > avg && data[7] > avg) d_line = true;
  return d_line;
}

float get_top_side(){
  //Serial.println("get_top_side");
  int dataSensors_top[8];
  for (int i = 0; i < 8; i++) {
    dataSensors_top[i] = octoliner_top.analogRead(i);
  }
  float top = round( octoliner_top.mapLine(dataSensors_top) * 10 );
  return top;
}
float get_bottom_side(){
  //Serial.println("get_bottom_side");
  int dataSensors_bottom[8];
  for (int i = 0; i < 8; i++) {
    dataSensors_bottom[i] = octoliner_bottom.analogRead(i);
  }
  float bottom = round( octoliner_bottom.mapLine(dataSensors_bottom) * 10 );
  return bottom;
}
float get_left_side(){
  //Serial.println("get_left_side");
  int dataSensors_left[8];
  for (int i = 0; i < 8; i++) {
    dataSensors_left[i] = octoliner_left.analogRead(i);
  }
  float left = round( octoliner_left.mapLine(dataSensors_left) * 10 );
  return left;
}
float get_right_side(){
  //Serial.println("get_right_side");
  int dataSensors_right[8];
  for (int i = 0; i < 8; i++) {
    dataSensors_right[i] = octoliner_right.analogRead(i);
  }
  float right = round( octoliner_right.mapLine(dataSensors_right) * 10 );
  return right;
}


/// wheels moves
void turn_right(int ms){
  Serial.println("turn_right");
  myservo1.write(360); //right rev
  myservo2.write(360); //left
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(200);
}
void turn_left(int ms){
  Serial.println("turn_left");
  myservo1.write(0); //right rev
  myservo2.write(0); //left
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(200);
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
  delay(200);
}
void move_back(int ms){
  Serial.println("move_back");
  myservo1.write(0);
  myservo2.write(360);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(200);
}
void turn_r_f(int ms){
  Serial.println("turn_r_f");
  myservo2.write(0);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(200);
}
void turn_l_f(int ms){
  Serial.println("turn_l_f");
  myservo1.write(360);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(200);
}
void turn_r_b(int ms){
  Serial.println("turn_r_b");
  myservo2.write(360);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(200);
}
void turn_l_b(int ms){
  Serial.println("turn_l_b");
  myservo1.write(0);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(200);
}
