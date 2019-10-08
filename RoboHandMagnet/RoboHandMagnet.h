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
    void init(int m1, int m3, int m4);
    void pick1();
    void drop1();
    void free();
    void arm_position_base_open();
    void arm_position_base_closed();
    void arm_position_level_1_open();
    void arm_position_level_1_closed();
    void arm_move_base_closed();
    void arm_move_base_open();
    void arm_move_level_1_closed();
    void arm_move_level_1_open();

    void arm_move(int serv, int deg);
    void arm_move2(int deg3, int deg4);
    void arm_move3(int deg3, int deg4);

    void arm_set(int serv, int deg);

  private:
    int m1; int m3; int m4;
    float pos3;
    float pos4;
    int max1; int min1;
    int max2; int min2;
    int max3; int min3;
    int max4; int min4;

    byte descript[9];
    int send_num;
    String send_val;
};

#endif
