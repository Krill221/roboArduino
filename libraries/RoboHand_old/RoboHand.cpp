/*
  RoboHand.h - Library for robo hand.
  Created by krill221 , june 7, 2019.
*/

#include "Arduino.h"
#include "RoboHand.h"


Multiservo myservo1;
Servo myservo2;
Multiservo myservo3;
Multiservo myservo4;

RoboHand::RoboHand() { }

void RoboHand::init(int n1, int n2, int n3, int n4){

  pos1 = 90;
  pos2 = 90;
  pos3 = 90;
  pos4 = 90;
  max1 = 90; min1 = 0;
  max2 = 180; min2 = 0;
  max3 = 180; min3 = 0;
  max4 = 180; min4 = 0;
  m1 = n1; m2 = n2; m3 = n3; m4 = n4;

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
  arm_set( 1, 10 );
  arm_set( 2, 101 );
  arm_set( 3, 0 );
  arm_set( 4, 60 );
}

void RoboHand::arm_position_base_closed(){ // to base arm opened
  arm_set( 1, 82 );
  arm_set( 2, 101 );
  arm_set( 3, 0 );
  arm_set( 4, 60 );
}

void RoboHand::arm_position_level_1_open(){ // 1 lev arm open
  arm_set( 1, 10 );
  arm_set( 2, 101 );
  arm_set( 3, 130 );
  arm_set( 4, 180 );

}

void RoboHand::arm_position_level_1_closed(){  //1 lev arm closed
  arm_set( 1, 82 );
  arm_set( 2, 101 );
  arm_set( 3, 130 );
  arm_set( 4, 180 );

}

void RoboHand::arm_move_base_closed(){ // to base arm closed
  arm_set( 1, 80 );
  arm_set( 2, 102 );
  arm_move2(0, 60);
}

void RoboHand::arm_move_level_1_closed(){ // to 1 lev arm closed
  arm_set( 1, 80 );
  arm_set( 2, 102 );
  arm_set( 3, 2 ); delay(15);
  arm_set( 3, 4 ); delay(15);
  arm_set( 3, 6 ); delay(15);
  arm_set( 3, 8 ); delay(15);
  arm_set( 3, 10 ); delay(15);
  arm_move2(130, 180);
}

void RoboHand::arm_move_base_open(){ // to base arm opened
  arm_set( 1, 10 );
  arm_set( 2, 102 );
  arm_move2(0, 60);
}

void RoboHand::arm_move_level_1_open(){ // to 1 lev arm opened
  arm_set( 1, 10 );
  arm_set( 2, 102 );
  arm_move3(130, 180);
}

void RoboHand::arm_move2(int deg3, int deg4){
  deg3 = (min3 > deg3) ? min3 : deg3;
  deg3 = (max3 < deg3) ? max3 : deg3;
  deg4 = (min4 > deg4) ? min4 : deg4;
  deg4 = (max4 < deg4) ? max4 : deg4;
  float step3 = (pos3 < deg3) ? 1 : -1;
  float step4 = (pos4 < deg4) ? 0.7 : -1.2;

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

void RoboHand::arm_move3(int deg3, int deg4){
  deg3 = (min3 > deg3) ? min3 : deg3;
  deg3 = (max3 < deg3) ? max3 : deg3;
  deg4 = (min4 > deg4) ? min4 : deg4;
  deg4 = (max4 < deg4) ? max4 : deg4;
  float step3 = (pos3 < deg3) ? 1 : -1;
  float step4 = (pos4 < deg4) ? 0.9 : -1;

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
