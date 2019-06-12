#include <CrawlerWheels.h>
#include <RoboHand.h>


CrawlerWheels wheels;
RoboHand hand;

void setup() {
  Serial.begin(9600);
  delay(3000);

  hand.init(17, 16, 15, 14);
  hand.arm_position_base_open();
  hand.free();
  
  wheels.init(220, 255);
 
  wheels.init_wheels(7, 6);
  wheels.print_coords();
  wheels.move_step_forward();
  /*wheels.rotate_right_90();
  wheels.print_coords();
  wheels.move_step_forward();
  wheels.print_coords();
  wheels.free_wheels();

  
  hand.init(17, 16, 15, 14);
  hand.arm_position_base_open();
  hand.arm_move_level_1_open();
  hand.arm_position_level_1_closed();
  hand.arm_move_base_closed();
  hand.free();

  wheels.init_wheels(7, 6);
  wheels.move_step_back();
  wheels.print_coords();
  wheels.rotate_left_90();
  wheels.print_coords();
  wheels.move_step_back();
  wheels.print_coords();
  wheels.free_wheels();
  */
  
}

void loop() {
}
