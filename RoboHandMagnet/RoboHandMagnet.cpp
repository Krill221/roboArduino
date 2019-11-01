/*
  RoboHandMagnet.h - Library for robo hand.
  Created by krill221 , june 7, 2019.
*/

#include "Arduino.h"
#include "RoboHandMagnet.h"


Multiservo myservo_bottom;
Multiservo myservo_top;

RoboHandMagnet::RoboHandMagnet() { }

void RoboHandMagnet::init(int n1, int bot, int top){

  posbot = 90;
  postop = 90;
  max3 = 180; min3 = 0;
  max4 = 190; min4 = 0;
  magnet = n1; mbot = bot; mtop = top;

  pinMode(magnet, OUTPUT);
  myservo_bottom.attach(mbot);
  myservo_top.attach(mtop);
  delay(300);
  Serial.println("init RoboHandMagnet done");
}

void RoboHandMagnet::pick1(){
  init(magnet, mbot, mtop);
  arm_position_base_open();
  arm_move_level_1_open();
  arm_position_level_1_closed();
  arm_move_base_closed();
  free();
}
void RoboHandMagnet::drop1(){
  init(magnet, mbot, mtop);
  arm_position_base_closed();
  arm_move_level_1_closed();
  arm_position_level_1_open();
  arm_move_base_open();
  free();
}

void RoboHandMagnet::free(){
  //pinMode(magnet, OUTPUT);
  //digitalWrite(magnet, LOW);
  myservo_bottom.detach();
  myservo_top.detach();
  delay(400);
  Serial.println("free RoboHandMagnet done");
}

void RoboHandMagnet::arm_position_bot_90(){ // to base arm opened
  posbot = 90;
  myservo_bottom.write(posbot);
  delay(600);
}
void RoboHandMagnet::arm_position_top_90(){ // to base arm opened
  postop = 90;
  myservo_top.write(postop);
  delay(600);
}

void RoboHandMagnet::arm_position_base_open(){ // to base arm opened
  digitalWrite(magnet, LOW);
  arm_set( mbot, 110 );
  arm_set( mtop, 0 );
}

void RoboHandMagnet::arm_position_base_closed(){ // to base arm
  digitalWrite(magnet, HIGH);
  arm_set( mbot, 110 );
  arm_set( mtop, 0 );
}

void RoboHandMagnet::arm_move_base_open(){ // to base arm opened
  digitalWrite(magnet, LOW);
  arm_move(110, 0);
}

void RoboHandMagnet::arm_move_base_closed(){ // to base arm closed
  digitalWrite(magnet, HIGH);
  arm_move(110, 0);
}

/////////
/////////
void RoboHandMagnet::arm_position_level_1_open(){ // 1 lev arm open
  digitalWrite(magnet, LOW);
  arm_set( mbot, 38 );
  arm_set( mtop, 115 );

}
void RoboHandMagnet::arm_position_level_1_closed(){  //1 lev arm closed
  digitalWrite(magnet, HIGH);
  arm_set( mbot, 38 );
  arm_set( mtop, 115 );

}
void RoboHandMagnet::arm_move_level_1_open(){ // to 1 lev arm opened
  digitalWrite(magnet, LOW);
  arm_move(38, 115);
}
void RoboHandMagnet::arm_move_level_1_closed(){ // to 1 lev arm closed
  digitalWrite(magnet, HIGH);
  arm_move(38, 115);
}
////////
////////


void RoboHandMagnet::arm_move(int bot, int top){
  float step_bot = (posbot < bot) ? 1 : -1;
  float step_top = (postop < top) ? 2.2 : -1;

  for (int i = 0; i <= 200; i++) {
    if(posbot != bot) {
      Serial.print("posbot: "); Serial.println(posbot);
      posbot = posbot + step_bot;
      myservo_bottom.write(posbot);
    }
    if(abs(abs(postop) - abs(top)) > 1) {
      Serial.print("postop: "); Serial.println(postop);
      postop = postop + step_top;
      myservo_top.write(postop);
    }
    delay(15);
  }
}

void RoboHandMagnet::arm_move3(int deg3, int deg4){
  deg3 = (min3 > deg3) ? min3 : deg3;
  deg3 = (max3 < deg3) ? max3 : deg3;
  deg4 = (min4 > deg4) ? min4 : deg4;
  deg4 = (max4 < deg4) ? max4 : deg4;
  float step3 = (posbot < deg3) ? 1 : -1;
  float step4 = (postop < deg4) ? 0.9 : -1;

  for (int i = 0; i <= 200; i++) {
    if(posbot != deg3) {
      posbot = posbot + step3;
      myservo_bottom.write(posbot);
    }
    if(abs(abs(postop) - abs(deg4)) > 1) {
      postop = postop + step4;
      myservo_top.write(postop);
    }
    delay(15);
  }
}

void RoboHandMagnet::arm_set(int serv, int deg){
  if (serv == mbot){ posbot = deg; myservo_bottom.write(deg); }
  if (serv == mtop){ postop = deg; myservo_top.write(deg); }
  delay(600);
}
