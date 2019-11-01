/*
  RoboHandMagnet.h - Library for robo hand.
  Created by krill221 , june 7, 2019.
*/

#ifndef RoboHandMagnet_h
#define RoboHandMagnet_h

#include "Arduino.h"

#include <Multiservo.h>
#include <Servo.h>



class RoboHandMagnet
{
  public:
    RoboHandMagnet();
    void init(int magnet, int m3, int mtop);
    void pick1();
    void drop1();
    void free();
    void arm_position_bot_90();
    void arm_position_top_90();
    void arm_position_base_open();
    void arm_position_base_closed();
    void arm_position_level_1_open();
    void arm_position_level_1_closed();
    void arm_move_base_closed();
    void arm_move_base_open();
    void arm_move_level_1_closed();
    void arm_move_level_1_open();

    void arm_move(int bot, int top);
    void arm_move3(int deg3, int deg4);

    void arm_set(int serv, int deg);

  private:
    int magnet; int mbot; int mtop;
    float posbot;
    float postop;
    int max3; int min3;
    int max4; int min4;

    byte descript[9];
    int send_num;
    String send_val;
};

#endif
