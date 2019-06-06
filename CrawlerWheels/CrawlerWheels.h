/*
  CrawlerWheels.h - Library for robo wheels.
  Created by krill221 , june 2, 2019.
*/

#ifndef CrawlerWheels_h
#define CrawlerWheels_h

#include "Arduino.h"

#include <Wire.h>
#include <Octoliner.h>
#include <Multiservo.h>



// -1  0  1
//  1    -1
//  0     0
// -1     1
//  1  0 -1

class CrawlerWheels
{
  public:
    CrawlerWheels(int left, int right, int sensitivity, int brightness);
    void move_step_forward();
    void move_step_back();
    void rotate_left_90();
    void rotate_right_90();
    void print_coords();


    void init_wheels(int r, int l);
    void init_sensors(int sensitivity, int brightness);
    void shift_to_center();
    void rotate_to_closer_line();
    boolean is_out_line_side(Octoliner &octoliner);
    boolean is_double_line_side(Octoliner &octoliner);
    int get_side(Octoliner &octoliner, int sen);

    void turn_right(int ms);
    void turn_left(int ms);
    void move_left(int ms);
    void move_right(int ms);
    void move_forward(int ms);
    void move_back(int ms);
    void turn_r_f(int ms);
    void turn_r_b(int ms);
    void turn_l_f(int ms);
    void turn_l_b(int ms);

    int find_current_angle(int top, int bot, int right, int left);
    int find_need_angle(int top, int bot, int right, int left);
    int find_xi(int top, int bot, int right, int left);
    int find_yi(int top, int bot, int right, int left);

  private:

    Multiservo myservo_r;
    Multiservo myservo_l;
    Octoliner octoliner_top(42);
    Octoliner octoliner_bottom(43);
    Octoliner octoliner_left(44);
    Octoliner octoliner_right(45);

    int last_top = 0;
    int last_bottom = 0;
    int last_left = 0;
    int last_right = 0;

    /// orientation
    int global_angle = 0;
    int global_step = 0;
};

#endif
