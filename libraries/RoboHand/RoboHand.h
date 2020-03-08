/*
  RoboHand.h - Library for single robo hand.
  Created by krill221 , 16 jan, 2020.
*/

#ifndef RoboHand_h
#define RoboHand_h

#include "Arduino.h"
#include <Servo.h>
#include <Multiservo.h>


class RoboHand
{
  private:
    int _id_base1_lock1;
    int _id_base1_lock2;
    int _id_base2_lock1;
    int _id_base2_lock2;
    int _id_base3_lock1;
    int _id_base3_lock2;

    int _id_base1_turn;
    int _id_base2_turn;
    int _id_base1_joint;
    int _id_base2_joint;
    int _id_middle_joint;
    int _id_pick_joint;

    int base1_turn_pos;
    int base2_turn_pos;
    int base1_joint_pos;
    int base2_joint_pos;
    int middle_joint_pos;
    int pick_joint_pos;

    int _servo_speed;

  public:
    int _base1_turn_shift;
    int _base2_turn_shift;
    int _base1_joint_shift;
    int _base2_joint_shift;
    int _middle_joint_shift;
    int _pick_joint_shift;

    RoboHand(
     int id_b1_lock1,
     int id_b1_lock2,
     int id_b2_lock1,
     int id_b2_lock2,
     int id_b3_lock1,
     int id_b3_lock2,
     int id_b1_turn,
     int id_b2_turn,
     int id_b1_joint,
     int id_b2_joint,
     int id_m_joint,
     int id_p_joint
    );
    void fullAttach();
    void fullDetach();
    void base1Lock();
    void base2Lock();
    void base3Lock();
    void base1Unlock();
    void base2Unlock();
    void base3Unlock();
    void base1Turn(int deg);
    void base2Turn(int deg);
    void base1Joint(int deg);
    void base2Joint(int deg);
    void middleJoint(int deg);
    void pickJoint(int deg);
    void middleJointDetach();
    void pickJointDetach();
    void base1TurnDetach();
    void base2TurnDetach();

    void runUp();
    void runDown();
    void runUpBase2();
    void runDownBase2();
    void turnCenter();
    void turnRight();
    void putFrontToRight();
    void putRightToFront();
    void stepBase2();
    void stepForward1();
    void pickBox();
    void putBox();

};

#endif
