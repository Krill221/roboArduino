#include "Arduino.h"
#include "RoboHand.h"

Servo base1_lock1;
Servo base1_lock2;
Servo base2_lock1;
Servo base2_lock2;
Servo base3_lock1;
Servo base3_lock2;
Multiservo base1_turn;
Multiservo base2_turn;
Multiservo base1_joint;
Multiservo base2_joint;
Multiservo middle_joint;
Multiservo pick_joint;


RoboHand::RoboHand(
        int id_base1_lock1,
        int id_base1_lock2,
        int id_base2_lock1,
        int id_base2_lock2,
        int id_base3_lock1,
        int id_base3_lock2,
        int id_base1_turn,
        int id_base2_turn,
        int id_base1_joint,
        int id_base2_joint,
        int id_middle_joint,
        int id_pick_joint
    ) {

    _base1_turn_shift = 0;
    _base2_turn_shift = 0;
    _base1_joint_shift = 0;
    _base2_joint_shift = 0;
    _middle_joint_shift = 0;
    _pick_joint_shift = 0;

    base1_turn_pos = 155;
    base2_turn_pos = 80;
    base1_joint_pos = 134;//154;
    base2_joint_pos = 75;
    middle_joint_pos = 121;//100;
    pick_joint_pos = 30;//100;

    _servo_speed = 30;

    _id_base1_lock1 = id_base1_lock1;
    _id_base1_lock2 = id_base1_lock2;
    _id_base2_lock1 = id_base2_lock1;
    _id_base2_lock2 = id_base2_lock2;
    _id_base3_lock1 = id_base3_lock1;
    _id_base3_lock2 = id_base3_lock2;

    _id_base1_turn = id_base1_turn;
    _id_base2_turn = id_base2_turn;
    _id_base1_joint = id_base1_joint;
    _id_base2_joint = id_base2_joint;
    _id_middle_joint = id_middle_joint;
    _id_pick_joint = id_pick_joint;

}

void RoboHand::fullAttach(){
    base1_lock1.attach(_id_base1_lock1);
    base1_lock2.attach(_id_base1_lock2);
    base2_lock1.attach(_id_base2_lock1);
    base2_lock2.attach(_id_base2_lock2);
    base3_lock1.attach(_id_base3_lock1);
    base3_lock2.attach(_id_base3_lock2);
    base1_turn.attach(_id_base1_turn);
    base2_turn.attach(_id_base2_turn);
    base1_joint.attach(_id_base1_joint);
    base2_joint.attach(_id_base2_joint);
    middle_joint.attach(_id_middle_joint);
    pick_joint.attach(_id_pick_joint);
}

void RoboHand::fullDetach(){
    base1_lock1.detach();
    base1_lock2.detach();
    base2_lock1.detach();
    base2_lock2.detach();
    base3_lock1.detach();
    base3_lock2.detach();
    base1_turn.detach();
    base2_turn.detach();
    base1_joint.detach();
    base2_joint.detach();
    middle_joint.detach();
    //pick_joint.detach();
}

/* LOCKS */
void RoboHand::base1Lock(){
    base1_lock1.attach(_id_base1_lock1);
    base1_lock2.attach(_id_base1_lock2);
    base1_lock1.write(90);
    base1_lock2.write(90);
    delay(1000);
    base1_lock1.detach();
    base1_lock2.detach();
}
void RoboHand::base2Lock(){
    base2_lock1.attach(_id_base2_lock1);
    base2_lock2.attach(_id_base2_lock2);
    base2_lock1.write(90);
    base2_lock2.write(90);
    delay(1000);
    base2_lock1.detach();
    base2_lock2.detach();
}
void RoboHand::base3Lock(){
    base3_lock1.attach(_id_base3_lock1);
    base3_lock2.attach(_id_base3_lock2);
    base3_lock1.write(90);
    base3_lock2.write(90);
    delay(1000);
    base3_lock1.detach();
    base3_lock2.detach();
}
void RoboHand::base1Unlock(){
    base1_lock1.attach(_id_base1_lock1);
    base1_lock2.attach(_id_base1_lock2);
    base1_lock1.write(0);
    base1_lock2.write(0);
    delay(1000);
    base1_lock1.detach();
    base1_lock2.detach();
}
void RoboHand::base2Unlock(){
    base2_lock1.attach(_id_base2_lock1);
    base2_lock2.attach(_id_base2_lock2);
    base2_lock1.write(0);
    base2_lock2.write(0);
    delay(1000);
    base2_lock1.detach();
    base2_lock2.detach();
}
void RoboHand::base3Unlock(){
    base3_lock1.attach(_id_base3_lock1);
    base3_lock2.attach(_id_base3_lock2);
    base3_lock1.write(0);
    base3_lock2.write(0);
    delay(1000);
    base3_lock1.detach();
    base3_lock2.detach();
}
/* END LOCKS */

/* TURNS */
void RoboHand::base1Turn(int new_pos){

    int pos = base1_turn_pos;
    base1_turn.attach(_id_base1_turn);
    //delay(100);
    if( pos < new_pos) {
      for (; pos <= new_pos; pos += 1) {
        base1_turn.write(pos); delay(_servo_speed);
      }
    } else {
      for (; pos >= new_pos; pos -= 1) {
        base1_turn.write(pos); delay(_servo_speed);
      }
    }
    base1_turn_pos = pos;
    //delay(100);
    //base1_turn.detach();
}
/* END TURN */

/* ANGLE */
void RoboHand::base1Joint(int new_pos){
    int pos = base1_joint_pos;
    base1_joint.attach(_id_base1_joint);
    //delay(100);
    if( pos < new_pos) {
      for (; pos <= new_pos; pos += 1) {
        base1_joint.write(pos); delay(_servo_speed);
      }
    } else {
      for (; pos >= new_pos; pos -= 1) {
        base1_joint.write(pos); delay(_servo_speed);
      }
    }
    base1_joint_pos = pos;
    //delay(100);
    //base1_joint.detach();
}
void RoboHand::base2Joint(int new_pos){
    int pos = base2_joint_pos;
    base2_joint.attach(_id_base2_joint);
    //delay(100);
    if( pos < new_pos) {
      for (; pos <= new_pos; pos += 1) {
        base2_joint.write(pos); delay(_servo_speed);
      }
    } else {
      for (; pos >= new_pos; pos -= 1) {
        base2_joint.write(pos); delay(_servo_speed);
      }
    }
    base2_joint_pos = pos;
    //delay(100);
    //base2_joint.detach();
}
void RoboHand::base2Turn(int new_pos){
    int pos = base2_turn_pos;
    base2_turn.attach(_id_base2_turn);
    //delay(100);
    if( pos < new_pos) {
      for (; pos <= new_pos; pos += 1) {
        base2_turn.write(pos); delay(_servo_speed);
      }
    } else {
      for (; pos >= new_pos; pos -= 1) {
        base2_turn.write(pos); delay(_servo_speed);
      }
    }
    base2_turn_pos = pos;
    //delay(100);
    //base2_turn.detach();
}

void RoboHand::middleJoint(int new_pos){
    int pos = middle_joint_pos;
    middle_joint.attach(_id_middle_joint);
    //delay(100);
    if( pos < new_pos) {
      for (; pos <= new_pos; pos += 1) {
        middle_joint.write(pos); delay(_servo_speed);
      }
    } else {
      for (; pos >= new_pos; pos -= 1) {
        middle_joint.write(pos); delay(_servo_speed);
      }
    }
    middle_joint_pos = pos;
    //delay(100);
    //middle_joint.detach();
}
void RoboHand::pickJoint(int new_pos){
    int pos = pick_joint_pos;
    pick_joint.attach(_id_pick_joint);
    //delay(100);
    if( pos < new_pos) {
      for (; pos <= new_pos; pos += 1) {
        pick_joint.write(pos); delay(_servo_speed);
      }
    } else {
      for (; pos >= new_pos; pos -= 1) {
        pick_joint.write(pos); delay(_servo_speed);
      }
    }
    pick_joint_pos = pos;
    //delay(100);
    //pick_joint.detach();
}

void RoboHand::moveJoint(int new_pos, int new_pos2){

    int pos = middle_joint_pos;
    int pos2 = base1_joint_pos;
    base1_joint.attach(_id_base1_joint);
    middle_joint.attach(_id_middle_joint);
    //delay(100);

    if( pos < new_pos && pos2 < new_pos2 ) {
      for (; pos < new_pos || pos2 < new_pos2;) {
         if(pos < new_pos) pos += 1;
         if(pos2 < new_pos2) pos2 += 1;
         middle_joint.write(pos);
         base1_joint.write(pos2);
         delay(_servo_speed);
      }
    }
    if( pos >= new_pos && pos2 >= new_pos2 ) {
      for (; pos > new_pos || pos2 > new_pos2 ;) {
         if(pos > new_pos) pos -= 1;
         if(pos2 > new_pos2) pos2 -= 1;
         middle_joint.write(pos);
         base1_joint.write(pos2);
         delay(_servo_speed);
      }
    }
    middle_joint_pos = pos;
    base1_joint_pos = pos2;
    //delay(100);

    //middle_joint.detach();
    //base1_joint.detach();
}
void RoboHand::moveJoint2(int new_pos, int new_pos2){

    int pos = middle_joint_pos;
    int pos2 = base2_joint_pos;
    base2_joint.attach(_id_base2_joint);
    middle_joint.attach(_id_middle_joint);
    //delay(100);

    if( pos < new_pos && pos2 >= new_pos2 ) {
      for (; pos < new_pos || pos2 > new_pos2;) {
         if(pos < new_pos) pos += 1;
         if(pos2 > new_pos2) pos2 -= 1;
         middle_joint.write(pos);
         base2_joint.write(pos2);
         delay(_servo_speed);
      }
    }
    if( pos >= new_pos && pos2 < new_pos2 ) {
      for (; pos > new_pos || pos2 < new_pos2 ;) {
         if(pos > new_pos) pos -= 1;
         if(pos2 < new_pos2) pos2 += 1;
         middle_joint.write(pos);
         base2_joint.write(pos2);
         delay(_servo_speed);
      }
    }
    middle_joint_pos = pos;
    base2_joint_pos = pos2;
    //delay(100);

    //middle_joint.detach();
    //base2_joint.detach();
}
void RoboHand::base1TurnDetach(){
    base1_turn.detach();
}
void RoboHand::base2TurnDetach(){
    base2_turn.detach();
}
void RoboHand::middleJointDetach(){
    middle_joint.detach();
}
void RoboHand::pickJointDetach(){
    middle_joint.detach();
}
/* END ANGLE */
/* ACTION */
void RoboHand::uTurn(){
  base1Lock();
  base2Unlock();
  runUp();
  turnRight();
  delay(500);
  runDown();
  base1TurnDetach();
}

void RoboHand::stepForward1(){

  // LOCK
    base1Lock();
    base2Unlock();
    delay(500);
    pickJoint(30);
    base1Joint(134);
    base2Joint(75);
    middleJoint(121);

    //base1Joint(160);
    //base2Joint(99);
    //middleJoint(110);
    base1Joint(178);
    base2Joint(96);
    middleJoint(98);
    moveJoint(67, 111);
    base2Joint(99);
    middleJoint(67);
    base1Joint(111);

    delay(500);
    base2Lock();
    delay(500);
    base1Unlock();
    delay(500);

    //// back
    base2Joint(80);
    moveJoint2(121, 50);
    base1Joint(134);
    base2Joint(75);

    /// DETACH
    delay(500);
    base1Unlock();
    base2Unlock();
    fullDetach();

    Serial.println("stepForward1 done");
}


void RoboHand::stepBack1(){

  // LOCK
    base2Lock();
    base1Unlock();
    pickJoint(30);

    base2Joint(49);
    base1Joint(111);
    middleJoint(110);
    moveJoint2(67, 99);

    ////// lock
    delay(500);
    base1Lock();
    delay(500);
    base2Unlock();
    delay(500);

    //// back
    base1Joint(123);
    //middleJoint(121);
    moveJoint(111, 170); //moveJoint2(121, 75);
    moveJoint2(121, 82);
    middleJoint(121);
    base2Joint(72);
    base1Joint(134);
    base2Joint(75);

    /// DETACH
    delay(500);
    base1Unlock();
    base2Unlock();
    fullDetach();
    Serial.println("stepBack1 done");
}

void RoboHand::stepBase2(){

  // LOCK
    base2Lock();
    base1Unlock();

    runUpBase2();
    runDownBase2();

    /// DETACH
    base1TurnDetach();
    middleJointDetach();
    delay(500);
    base1Lock();
    base2Unlock();
    fullDetach();
    delay(500);
}
void RoboHand::runUpBase2(){
    base1Joint(134);
    middleJoint(121);
    base2Joint(30);
}
void RoboHand::runDownBase2(){
    base1Joint(134);
    middleJoint(121);
    base2Joint(75);
}
void RoboHand::runUp(){
    base1Joint(180);
    base2Joint(75);
    middleJoint(121);
}
void RoboHand::runDown(){
    middleJoint(121);
    base2Joint(75);
    base1Joint(134);
}
void RoboHand::turnRight(){
    base1Turn(28);
    //base2Joint(75);
}
void RoboHand::turnCenter(){
    base1Turn(90);
    //base2Joint(75);
}
void RoboHand::turnLeft(){
    base1Turn(155);
    //base2Joint(75);
}

void RoboHand::putFrontToRight(){

  // LOCK
    base1Lock();
    base2Lock();

    runUp();
    turnRight();
    runDown();

    // PUT
    delay(1000);
    base2Unlock();
    delay(1000);

    runUp();
    turnCenter();
    runDown();

    /// DETACH
    base1TurnDetach();
    middleJointDetach();
    delay(500);
    base2Unlock();
    fullDetach();
    delay(500);
}


void RoboHand::putRightToFront(){

  // LOCK
    base1Lock();
    base2Unlock();

    runUp();
    turnRight();
    runDown();

    // PUT
    delay(1000);
    base2Lock();
    delay(1000);

    runUp();
    turnCenter();
    runDown();

    /// DETACH
    base1TurnDetach();
    middleJointDetach();
    delay(500);
    base2Unlock();
    fullDetach();
    delay(500);
}


void RoboHand::pickBox(){

  // LOCK
    pickJoint(30);
    base1Lock();
    base2Lock();
    base3Unlock();

    pickJoint(117);
    delay(500);
    base3Lock();
    delay(500);
    pickJoint(30);

    delay(500);
    base2Unlock();
}

void RoboHand::putBox(){

  // LOCK
    pickJoint(30);
    base1Lock();
    base2Lock();
    base3Lock();

    pickJoint(117);
    delay(500);
    base3Unlock();
    delay(500);
    pickJoint(30);

    delay(500);
    base2Unlock();
}
