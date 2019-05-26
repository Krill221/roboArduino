// библиотека для работы с модулями IMU
#include <TroykaIMU.h>
#include <Servo.h>


Compass compass(COMPASS_ADDRESS_V1);


Servo myservo5;
Servo myservo6;

// калибровочные значения компаса
// полученные в калибровочной матрице из примера «compassCalibrateMatrix»
const double compassCalibrationBias[3] = {
  524.21,
  3352.214,
  -1402.236
};
 
const double compassCalibrationMatrix[3][3] = {
  {1.757, 0.04, -0.028},
  {0.008, 1.767, -0.016},
  {-0.018, 0.077, 1.782}
};
 
void setup()
{
  Serial.begin(9600);

  compass.begin();
  compass.calibrateMatrix(compassCalibrationMatrix, compassCalibrationBias);

  myservo5.attach(8);
  myservo6.attach(9);
  
  Serial.println("Initialization completed");


  // wheels setup
  myservo5.write(90);  
  myservo6.write(90);
  delay(500);

  forward(1);
  delay(500);
  turn_left_90();
}
 
void loop()
{
  Serial.println(  get_angle(10)  );
  delay(200);
}


void forward(int ms){

  float avg1 = get_angle(15);  
  myservo5.write(0); //right rev
  myservo6.write(360); //left            
  delay(ms*400);
  myservo5.write(90);
  myservo6.write(90);

  float avg2 = get_angle(15);
  Serial.println("fwd: ");
  Serial.println(avg1);
  Serial.println(avg2);

  Serial.println("");
  
}

void back(int ms){
  myservo5.write(360); //right rev
  myservo6.write(0); //left            
  delay(ms*400);
  myservo5.write(90);
  myservo6.write(90);
}

void turn_left_90(){
  float total = 0;

  
  float avg1 = get_angle(15);
  total = avg1 - 37;
  turn_left(2000);
  for(int i = 0; i < 20; i++ ){
    
    float avg2 = get_angle(15);
    Serial.println("deg: ");
    Serial.println( avg1);
    Serial.println( total );
    Serial.println( avg2 );
    Serial.println("");

    if(round(total*10) == round(avg2*10) ){
      break;
    }
    if(round(total*10) > round(avg2*10)){
      turn_right(200);
      break;
    }
    if(total < avg2 ){
      turn_left(100z. );
    }
    
  }

  
}

void turn_left(int ms){
  myservo5.write(0); //right rev
  myservo6.write(0); //left            
  delay(ms);
  myservo5.write(90);
  myservo6.write(90);
  delay(300);
  
}

void turn_right(int ms){
  myservo5.write(360); //right rev
  myservo6.write(360); //left            
  delay(ms);
  myservo5.write(90);
  myservo6.write(90);
}

int get_angle(int size)
{
  float angle_arr[size];
  for(int i=0; i< size; i++){
    angle_arr[i] = compass.readAzimut();
    delay(100);
  }
  float angle = 0;
  for(int i = 0; i < size; i++) { angle += angle_arr[i]; }
  return round( angle / size );
}
