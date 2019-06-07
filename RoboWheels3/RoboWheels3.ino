#include <CrawlerWheels.h>
#include <RoboHand.h>


CrawlerWheels wheels;
RoboHand hand;

void setup() {
  Serial.begin(9600);
  delay(3000);

  hand.init(17, 16, 15, 14);
  wheels.init(7, 6, 220, 255);


  hand.arm_position_5(); // to base arm opened
  delay(1000);
  
  wheels.print_coords();
  wheels.move_step_forward();
  wheels.print_coords();
  wheels.rotate_left_90();
  wheels.print_coords();
  wheels.move_step_forward();
  wheels.print_coords();

  hand.arm_position_6(); // to 1 lev arm opened
  delay(1000);
  hand.arm_position_1(); // 1 lev arm open
  delay(1000);
  hand.arm_position_2(); // 1 lev arm closed
  delay(1000);
  hand.arm_position_3(); // to base arm closed
  delay(1000);
  hand.arm_position_4(); // to 1 lev arm closed
  delay(1000);
  hand.arm_position_1();  // 1 lev arm open
  delay(1000);
  hand.arm_position_5();  // to base arm opened
  delay(1000);

  
  wheels.move_step_back();
  wheels.print_coords();
  wheels.rotate_right_90();
  wheels.print_coords();
  wheels.move_step_back();
  wheels.print_coords();
  
}

void loop() {
}
