#include <CrawlerWheels.h>

CrawlerWheels wheels;


void setup() {
  Serial.begin(9600);
  delay(3000);
  
  wheels.init(7, 6, 220, 255);
  
  wheels.print_coords();
  wheels.move_step_forward();
  wheels.print_coords();
  wheels.rotate_left_90();
  wheels.print_coords();
  wheels.move_step_forward();
  wheels.print_coords();
  wheels.move_step_back();
  wheels.print_coords();
  wheels.rotate_right_90();
  wheels.print_coords();
  wheels.move_step_back();
  wheels.print_coords();
  
}

void loop() {
}
