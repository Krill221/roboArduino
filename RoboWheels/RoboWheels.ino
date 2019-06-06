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

/// orientation
int global_deg = 0;
int global_path = 0;

// -1  0  1
//  1    -1
//  0     0
// -1     1
//  1  0 -1

void setup() {
  Serial.begin(9600);

  // wheels init
  myservo1.attach(8); myservo1.write(90);
  myservo2.attach(9); myservo2.write(90);

  // sensors init
  int sensitivity = 220;
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

  for(int i = 0; i< 13; i++) {
    turn_left(300);
    delay(900);
  }
  rotate_to_closer_line();

  delay(2000);

  for(int i = 0; i< 13; i++) {
    turn_right(300);
    delay(900);
  }
  rotate_to_closer_line();
  
  //turn_left(750);
  //delay(750);
  
  //rotate_to_ather_line();
  
  //rotate_to_closer_line();
  
  //move_back(3000);
  //rotate_to_closer_line();
}

void loop() {
    Serial.println("Loop:");
    float top_side = get_side(octoliner_top, 1);;
    float bottom_side = get_side(octoliner_bottom, 2);
    float left_side = get_side(octoliner_left, 4);
    float right_side = get_side(octoliner_right, 3);

    Serial.print(" top: ");
    Serial.print(top_side);
    Serial.print(" bot: ");
    Serial.print(bottom_side);
    Serial.print(" left: ");
    Serial.print(left_side);
    Serial.print(" right: ");
    Serial.print(right_side);
    Serial.println();
    
    delay(3000);
}

int find_current_angle(int top, int bot, int right, int left){
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


int find_need_angle(int top, int bot, int right, int left){
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

int find_xi(int top, int bot, int right, int left){
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

int find_yi(int top, int bot, int right, int left){
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


int find_section(int xi, int yi){
  if( (xi == 0 && yi == 0) || (xi == 1 && yi == 0) || (xi == -1 && yi == 0) ){ Serial.println("section: 0"); return 0; }
  if( (xi == 0 && yi == 1) || (xi == 0 && yi == -1) ){ Serial.println("section: 0"); return 0; }
  if(xi < 0 && yi < 0){ Serial.println("section: 1"); return 1; }
  if(xi < 0 && yi >= 0){ Serial.println("section: 2"); return 2; }
  if(xi >= 0 && yi >= 0){ Serial.println("section: 3"); return 3; }
  if(xi >= 0 && yi < 0){ Serial.println("section: 4"); return 4; }
  return 0;
}

//////
//////

void shift_to_center() { // add error to 10
    // 1. hard shift to center circle (out lines)
    for(int i =0; i < 10; i++){
      Serial.print("hard shift 1 ");
      boolean o_line_top_side = out_line_side(octoliner_top);
      boolean o_line_bottom_side = out_line_side(octoliner_bottom);
      boolean o_line_left_side = out_line_side(octoliner_left);
      boolean o_line_right_side = out_line_side(octoliner_right);
      boolean d_line_top_side = double_line_side(octoliner_top);
      boolean d_line_bottom_side = double_line_side(octoliner_bottom);
      boolean d_line_left_side = double_line_side(octoliner_left);
      boolean d_line_right_side = double_line_side(octoliner_right);
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
      boolean d_line_top_side = double_line_side(octoliner_top);
      boolean d_line_bottom_side = double_line_side(octoliner_bottom);
      boolean d_line_left_side = double_line_side(octoliner_left);
      boolean d_line_right_side = double_line_side(octoliner_right);
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
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 2 ) {  continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 2 ) {  continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 3 ) {  move_back(60); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) < 3 ) {  move_forward(100); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 4 ) {  move_back(100); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 4 ) {  move_forward(100); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 6 ) {  move_back(200); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 6 ) {  move_forward(200); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) > 6 ) {  move_back(300); continue; }
      if ( (left_side > 0 && right_side > 0) && (abs(left_side) > abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) > 6 ) {  move_forward(300); continue; }
      

      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 2  ) {  continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) ) {  move_forward(60); continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) && abs(abs(left_side) - abs(right_side) ) <= 2  ) {  continue; }
      if ( (left_side < 0 && right_side < 0) && (abs(left_side) < abs(right_side) ) ) {  move_back(60); continue; }

      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) < abs(bottom_side) ) && abs(abs(top_side) - abs(bottom_side) ) <= 2  ) {   continue; }
      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) < abs(bottom_side) ) ) { move_left(400); continue; }
      
      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) > abs(bottom_side) ) && abs(abs(top_side) - abs(bottom_side) ) <= 2  ) {   continue; }
      if ( (top_side > 0 && bottom_side > 0) && (abs(top_side) > abs(bottom_side) ) ) { move_right(400); continue; }

      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) < abs(bottom_side) ) && abs(abs(top_side) - abs(bottom_side) ) <= 2  ) {  continue; }
      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) < abs(bottom_side) ) ) { move_right(400); continue; }

      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) > abs(bottom_side) ) && abs(abs(top_side) - abs(bottom_side) ) <= 2  ) {   continue; }
      if ( (top_side < 0 && bottom_side < 0) && (abs(top_side) > abs(bottom_side) ) )  {  move_left(400); continue; }
      
      Serial.println(" --none");
      break;
    }
}

void new_shift() {
  for(int i =0; i < 5; i++){
    if(i == 9) Serial.println("Out of time --------");
    
    Serial.println("new_shift");
    delay(400);
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
    Serial.println("-------");
    Serial.print(top_side); Serial.print(" "); Serial.print(bottom_side); Serial.print(" "); Serial.print(left_side); Serial.print(" "); Serial.println(right_side);
    if(top_side == 0 && bottom_side  == 0 && left_side == 0 && right_side == 0){ Serial.println("done --------------"); break; }
    // new algorithm
    int xi = find_xi(top_side, bottom_side, right_side, left_side);
    int yi = find_yi(top_side, bottom_side, right_side, left_side);
    int section = find_section(xi, yi);
    int need_angle = find_need_angle(top_side, bottom_side, right_side, left_side);
    int current_angle = find_current_angle(top_side, bottom_side, right_side, left_side);
    
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
        
    if (top_side == right_side && bottom_side == left_side ) { turn_left(200); continue; }
    if (top_side == left_side && bottom_side == right_side ) { turn_right(200); continue; }
    
    //if(top_side < 0 && bottom_side < 0 && top_side < bottom_side ) { turn_l_f(100); continue; }
    
    break;
  }
}

// get coordinates
boolean out_line_side(Octoliner &octoliner) {
  boolean o_line = false; int lines = 0;
  int data[8];
  for (int i = 0; i < 8; i++) {
    data[i] = octoliner.analogRead(i);
    //Serial.print(data[i]);
    //Serial.print(" ");
  }
  for (int i = 0; i < 8; i++) { if( data[i] < 60 ) lines++;  }
  if(lines == 8) { return true; }
  return false;
}

// get coordinates
boolean double_line_side(Octoliner &octoliner) {
  boolean d_line = false; float sum = 0; float avg = 0; int lines = 0;
  int data[8];
  //Serial.print("d line: ");
  for (int i = 0; i < 8; i++) {
    data[i] = octoliner.analogRead(i); sum += data[i];
    //Serial.print(data[i]);
    //Serial.print(" ");
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

int get_side(Octoliner &octoliner, int sen){
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

void rotate_left_90() {
    Serial.println("rotate_left_90");
    rotate_to_closer_line();
    turn_left(700);
    delay(1000);
    turn_left(700);
    delay(1000);
    turn_left(700);
    delay(1000);
    turn_left(700);
    delay(1000);
    rotate_to_closer_line();

    global_deg = global_deg - 90;
}

void rotate_right_90() {
    Serial.println("rotate_right_90");
    rotate_to_closer_line();
    turn_right(700);
    delay(1000);
    turn_right(700);
    delay(1000);
    turn_right(700);
    delay(1000);
    turn_right(700);
    delay(1000);
    rotate_to_closer_line();

    global_deg = global_deg + 90;
}


/// wheels moves
void turn_right(int ms){
  Serial.println("turn_right");
  myservo1.write(180); //left
  myservo2.write(180); //right
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_left(int ms){
  Serial.println("turn_left");
  myservo2.write(-90); //right
  myservo1.write(-90); //left
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
  myservo1.write(180);
  myservo2.write(0);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void move_back(int ms){
  Serial.println("move_back");
  myservo1.write(0);
  myservo2.write(180);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_r_f(int ms){
  //Serial.println("turn_r_f");
  myservo2.write(0);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_l_f(int ms){
  //Serial.println("turn_l_f");
  myservo1.write(360);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_r_b(int ms){
  //Serial.println("turn_r_b");
  myservo2.write(360);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
void turn_l_b(int ms){
  //Serial.println("turn_l_b");
  myservo1.write(0);
  delay(ms);
  myservo1.write(90);
  myservo2.write(90);
  delay(400);
}
