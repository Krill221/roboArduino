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

  Serial.println("init RoboHand done");
}


void RoboHand::arm_position_1(){ // 1 lev arm open
  arm_move( 1, 90 );
  arm_move( 2, 90 );
  arm_move( 3, 140 );
  arm_move( 4, 180 );

}

void RoboHand::arm_position_2(){  //1 lev arm closed
  arm_move( 1, 130 );
  arm_move( 2, 90 );
  arm_move( 3, 140 );
  arm_move( 4, 180 );

}

void RoboHand::arm_position_3(){ // to base arm closed
  arm_move( 1, 130 );
  arm_move( 2, 90 );
  arm_move( 4, 160 );
  arm_move( 3, 120 );
  arm_move( 4, 140 );
  arm_move( 3, 100 );
  arm_move( 4, 120 );
  arm_move( 3, 80 );
  arm_move( 4, 100 );
  arm_move( 3, 60 );
  arm_move( 4, 80 );
  arm_move( 3, 40 );
  arm_move( 4, 60 );
  arm_move( 3, 0 );
}

void RoboHand::arm_position_4(){ // to 1 lev arm closed
  arm_move( 3, 0 );
  arm_move( 4, 60 );
  arm_move( 3, 40 );
  arm_move( 4, 80 );
  arm_move( 3, 60 );
  arm_move( 4, 100 );
  arm_move( 3, 80 );
  arm_move( 4, 120 );
  arm_move( 3, 100 );
  arm_move( 4, 140 );
  arm_move( 4, 140 );
  arm_move( 3, 120 );
  arm_move( 4, 160 );
  arm_move( 3, 140 );
  arm_move( 4, 180 );
  arm_move( 2, 90 );
  arm_move( 1, 130 );
}

void RoboHand::arm_position_5(){ // to base arm opened
  arm_move( 1, 90 );
  arm_move( 2, 90 );
  arm_move( 4, 160 );
  arm_move( 3, 120 );
  arm_move( 4, 140 );
  arm_move( 3, 100 );
  arm_move( 4, 120 );
  arm_move( 3, 80 );
  arm_move( 4, 100 );
  arm_move( 3, 60 );
  arm_move( 4, 80 );
  arm_move( 3, 40 );
  arm_move( 4, 60 );
  arm_move( 3, 0 );
}

void RoboHand::arm_position_6(){ // to 1 lev arm opened
  arm_move( 3, 0 );
  arm_move( 4, 60 );
  arm_move( 3, 40 );
  arm_move( 4, 80 );
  arm_move( 3, 60 );
  arm_move( 4, 100 );
  arm_move( 3, 80 );
  arm_move( 4, 120 );
  arm_move( 3, 100 );
  arm_move( 4, 140 );
  arm_move( 4, 140 );
  arm_move( 3, 120 );
  arm_move( 4, 160 );
  arm_move( 3, 140 );
  arm_move( 4, 180 );
  arm_move( 2, 90 );
  arm_move( 1, 90 );
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
