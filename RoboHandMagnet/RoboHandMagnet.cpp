/*
  RoboHandMagnet.h - Library for robo hand.
  Created by krill221 , june 7, 2019.
*/

#include "Arduino.h"
#include "RoboHandMagnet.h"


Multiservo myservo3;
Multiservo myservo4;

RoboHandMagnet::RoboHandMagnet() { }

void RoboHandMagnet::init(int n1, int n3, int n4){

  pos3 = 90;
  pos4 = 90;
  max3 = 180; min3 = 0;
  max4 = 190; min4 = 0;
  m1 = n1; m3 = n3; m4 = n4;

  pinMode(m1, OUTPUT);
  myservo3.attach(m3);
  myservo4.attach(m4);
  delay(300);
  Serial.println("init RoboHandMagnet done");
}

void RoboHandMagnet::pick1(){
  init(m1, m3, m4);
  arm_position_base_open();
  arm_move_level_1_open();
  arm_position_level_1_closed();
  arm_move_base_closed();
  free();
}
void RoboHandMagnet::drop1(){
  init(m1, m3, m4);
  arm_position_base_closed();
  arm_move_level_1_closed();
  arm_position_level_1_open();
  arm_move_base_open();
  free();
}

void RoboHandMagnet::free(){
  //pinMode(m1, OUTPUT);
  //digitalWrite(m1, LOW);
  myservo3.detach();
  myservo4.detach();
  delay(400);
  Serial.println("free RoboHandMagnet done");
}

void RoboHandMagnet::arm_position_base_open(){ // to base arm opened
  digitalWrite(m1, LOW);
  arm_set( 3, 0 );
  arm_set( 4, 60 );
}

void RoboHandMagnet::arm_position_base_closed(){ // to base arm
  digitalWrite(m1, HIGH);
  arm_set( 3, 0 );
  arm_set( 4, 60 );
}

void RoboHandMagnet::arm_position_level_1_open(){ // 1 lev arm open
  digitalWrite(m1, LOW);
  arm_set( 3, 125 );
  arm_set( 4, 190 );

}

void RoboHandMagnet::arm_position_level_1_closed(){  //1 lev arm closed
  digitalWrite(m1, HIGH);
  arm_set( 3, 125 );
  arm_set( 4, 190 );

}

void RoboHandMagnet::arm_move_base_closed(){ // to base arm closed
  digitalWrite(m1, HIGH);
  arm_move2(0, 60);
}

void RoboHandMagnet::arm_move_level_1_closed(){ // to 1 lev arm closed
  digitalWrite(m1, HIGH);
  arm_set( 3, 2 ); delay(15);
  arm_set( 3, 4 ); delay(15);
  arm_set( 3, 6 ); delay(15);
  arm_set( 3, 8 ); delay(15);
  arm_set( 3, 10 ); delay(15);
  arm_move2(125, 190);
}

void RoboHandMagnet::arm_move_base_open(){ // to base arm opened
  digitalWrite(m1, LOW);
  arm_move2(0, 60);
}

void RoboHandMagnet::arm_move_level_1_open(){ // to 1 lev arm opened
  digitalWrite(m1, LOW);
  arm_move3(125, 190);
}

void RoboHandMagnet::arm_move2(int deg3, int deg4){
  deg3 = (min3 > deg3) ? min3 : deg3;
  deg3 = (max3 < deg3) ? max3 : deg3;
  deg4 = (min4 > deg4) ? min4 : deg4;
  deg4 = (max4 < deg4) ? max4 : deg4;
  float step3 = (pos3 < deg3) ? 1 : -1;
  float step4 = (pos4 < deg4) ? 0.7 : -1.2;

  for (int i = 0; i <= 200; i++) {

    if(pos3 != deg3) {
      Serial.print("pos3: "); Serial.println(pos3);
      pos3 = pos3 + step3;
      myservo3.write(pos3);
    }
    if(abs(abs(pos4) - abs(deg4)) > 1) {
      Serial.print("pos4: "); Serial.println(pos4);
      pos4 = pos4 + step4;
      myservo4.write(pos4);
    }
    delay(15);
  }
}

void RoboHandMagnet::arm_move3(int deg3, int deg4){
  deg3 = (min3 > deg3) ? min3 : deg3;
  deg3 = (max3 < deg3) ? max3 : deg3;
  deg4 = (min4 > deg4) ? min4 : deg4;
  deg4 = (max4 < deg4) ? max4 : deg4;
  float step3 = (pos3 < deg3) ? 1 : -1;
  float step4 = (pos4 < deg4) ? 0.9 : -1;

  for (int i = 0; i <= 200; i++) {
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

void RoboHandMagnet::arm_set(int serv, int deg){
  delay(300);
  if (serv == 3){ pos3 = deg; myservo3.write(deg); }
  if (serv == 4){ pos4 = deg; myservo4.write(deg); }
  delay(300);
}
