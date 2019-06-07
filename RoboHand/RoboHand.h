/*
  RoboHand.h - Library for robo hand.
  Created by krill221 , june 7, 2019.
*/

#ifndef RoboHand_h
#define RoboHand_h

#include "Arduino.h"

#include <Multiservo.h>


class RoboHand
{
  public:
    RoboHand();
    void init(int m1, int m2, int m3, int m4);
    void arm_position_1();
    void arm_position_2();
    void arm_position_3();
    void arm_position_4();
    void arm_position_5();
    void arm_position_6();
    void arm_move(int serv, int deg);

  private:
    int pos1;
    int pos2;
    int pos3;
    int pos4;
    int max1; int min1;
    int max2; int min2;
    int max3; int min3;
    int max4; int min4;

    byte descript[9];
    int send_num;
    String send_val;
};

#endif
