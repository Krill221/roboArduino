/*
  MecanumWheelsStep.h - Library for robo wheels.
  Created by krill221 , august 30, 2019.
*/

#ifndef MecanumWheelsStep_h
#define MecanumWheelsStep_h

#include "Arduino.h"

#include <Wire.h>
#include <Octoliner.h>
#include <Multiservo.h>

// -1  0  1
//  1    -1
//  0     0
// -1     1
//  1  0 -1

class MecanumWheelsStep
{
  public:
    MecanumWheelsStep();

    // sensors
    void init(int sensitivity, int brightness);
    void set_to_contre();
    void move_step_forward();
    void move_step_back();
    void rotate_left_90();
    void rotate_right_90();
    void print_coords();

    // wheels
    void init_wheels(int s1, int d1, int e1,
      int s2, int d2, int e2,
      int s3, int d3, int e3,
      int s4, int d4, int e4
    );
    void hold_wheels();
    void free_wheels();

    void shift_to_center();
    void rotate_to_closer_line();
    boolean is_out_line_side(Octoliner &octoliner);
    boolean is_double_line_side(Octoliner &octoliner);
    int get_side(Octoliner &octoliner, int sen);

    void turn_test(int ms);
    void turn_right(int ms);
    void turn_right_half(int ms);
    void turn_left(int ms);
    void turn_left_half(int ms);
    void move_left(int ms);
    void move_right(int ms);
    void move_forward(int ms);
    void move_back(int ms);
    void make_steps(int steps);

    int find_current_angle1();
    int find_current_angle2();

    int find_need_angle();
    int find_xi(int top, int bot, int right, int left);
    int find_yi(int top, int bot, int right, int left);

  private:

    int last_top;
    int last_bottom;
    int last_left;
    int last_right;

    /// orientation
    int global_angle;
    int global_step;

    int stepPin1; int directionPin1; int enablePin1;
    int stepPin2; int directionPin2; int enablePin2;
    int stepPin3; int directionPin3; int enablePin3;
    int stepPin4; int directionPin4; int enablePin4;
};

#endif
