/*
  RoboHand.h - Library for robo hand.
  Created by krill221 , june 7, 2019.
*/

#include "Arduino.h"
#include "RoboHand.h"


Multiservo myservo;
Multiservo myservo1;
Multiservo myservo2;
Multiservo myservo3;
Multiservo myservo4;

RoboHand::RoboHand() { }

void RoboHand::init(int m1, int m2, int m3, int m4){

  pos1 = 90;
  pos2 = 90;
  pos3 = 90;
  pos4 = 90;
  max1 = 160; min1 = 90;
  max2 = 180; min2 = 0;
  max3 = 180; min3 = 0;
  max4 = 180; min4 = 0;

  myservo1.attach(m1); myservo3.attach(m3);
  myservo2.attach(m2); myservo4.attach(m4);
  delay(300);
  Serial.println("init RoboHand done");
}

void RoboHand::free(){

  myservo1.detach(); myservo3.detach();
  myservo2.detach(); myservo4.detach();
  delay(400);
  Serial.println("free RoboHand done");
}

void RoboHand::arm_position_base_open(){ // to base arm opened
  arm_set( 1, 90 );
  arm_set( 2, 101 );
  arm_set( 3, 0 );
  arm_set( 4, 60 );
}

void RoboHand::arm_position_base_closed(){ // to base arm opened
  arm_set( 1, 138 );
  arm_set( 2, 101 );
  arm_set( 3, 0 );
  arm_set( 4, 60 );
}

void RoboHand::arm_position_level_1_open(){ // 1 lev arm open
  arm_set( 1, 90 );
  arm_set( 2, 101 );
  arm_set( 3, 140 );
  arm_set( 4, 180 );

}

void RoboHand::arm_position_level_1_closed(){  //1 lev arm closed
  arm_set( 1, 138 );
  arm_set( 2, 101 );
  arm_set( 3, 140 );
  arm_set( 4, 180 );

}

void RoboHand::arm_move_base_closed(){ // to base arm closed
  arm_set( 1, 136 );
  arm_set( 2, 102 );
  arm_move2(0, 60);
}

void RoboHand::arm_move_level_1_closed(){ // to 1 lev arm closed
  arm_set( 1, 136 );
  arm_set( 2, 102 );
  arm_move2(140, 180);
}

void RoboHand::arm_move_base_open(){ // to base arm opened
  arm_set( 1, 90 );
  arm_set( 2, 102 );
  arm_move2(0, 60);
}

void RoboHand::arm_move_level_1_open(){ // to 1 lev arm opened
  arm_set( 1, 90 );
  arm_set( 2, 102 );
  arm_move2(140, 180);
}

void RoboHand::arm_move(int serv, int deg){
  int max = 360;
  int min = 0;
  int pos = 0;
  if (serv == 1) {
    myservo = myservo1; max = max1; min = min1;
    if(pos1 > max) pos1 = max;
    if(pos1 < min) pos1 = min;
    pos = pos1;
  }
  if (serv == 2) {
    myservo = myservo2; max = max2; min = min2;
    if(pos2 > max) pos2 = max;
    if(pos2 < min) pos2 = min;
    pos = pos2;
  }
  if (serv == 3) {
    myservo = myservo3; max = max3; min = min3;
    if(pos3 > max) pos3 = max;
    if(pos3 < min) pos3 = min;
    pos = pos3;
  }
  if (serv == 4) {
    myservo = myservo4; max = max4; min = min4;
    if(pos4 > max) pos4 = max;
    if(pos4 < min) pos4 = min;
    pos = pos4;
  }

  if(deg > max) deg = max;
  if(deg < min) deg = min;

  for (int i = pos; i <= deg; i += 1) {
    myservo.write(i);
    delay(15);
    pos = i;
  }
  for (int i = pos; i >= deg; i -= 1) {
    myservo.write(i);
    delay(15);
    pos = i;
  }
  if (serv == 1) pos1 = pos;
  if (serv == 2) pos2 = pos;
  if (serv == 3) pos3 = pos;
  if (serv == 4) pos4 = pos;
}


void RoboHand::arm_move2(int deg3, int deg4){
  deg3 = (min3 > deg3) ? min3 : deg3;
  deg3 = (max3 < deg3) ? max3 : deg3;
  deg4 = (min4 > deg4) ? min4 : deg4;
  deg4 = (max4 < deg4) ? max4 : deg4;
  float step3 = (pos3 < deg3) ? 1 : -1;
  float step4 = (pos4 < deg4) ? 0.8 : -1;

  for (int i = 0; i <= 180; i++) {
    if(pos3 != deg3) {
      pos3 = pos3 + step3;
      myservo3.write(pos3);
    }
    if(abs(abs(pos4) - abs(deg4)) > 1) {
      pos4 = pos4 + step4;
      myservo4.write(pos4);
    }
    delay(15);
  }
}

void RoboHand::arm_set(int serv, int deg){
  delay(300);
  if (serv == 1){ pos1 = deg; myservo1.write(deg); }
  if (serv == 2){ pos2 = deg; myservo2.write(deg); }
  if (serv == 3){ pos3 = deg; myservo3.write(deg); }
  if (serv == 4){ pos4 = deg; myservo4.write(deg); }
  delay(300);
}
