#include <Servo.h>

Servo myservo;
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;

int pos1 = 90;
int pos2 = 90;
int pos3 = 90;
int pos4 = 90;
int pos5 = 90;
int pos6 = 90;
int max1 = 160; int min1 = 90;
int max2 = 180; int min2 = 0;
int max3 = 180; int min3 = 26;
int max4 = 120; int min4 = 0;
int max5 = 160; int min5 = 90;
int max6 = 160; int min6 = 90;

byte descript[9];
int send_num;
String send_val;


void setup() {

  // hand
  myservo1.attach(11);
  myservo2.attach(2);
  myservo3.attach(4);
  myservo4.attach(3);
  myservo5.attach(62);
  myservo6.attach(63);

  // wheels setup
  myservo5.write(90);  
  myservo6.write(90); 

  //port
  Serial.begin(9600);


  delay(500);


}

void loop()  
{  
 if (Serial.available()>8) // ждём дескриптор и нужный символ
 {
  if (Serial.read()=='Y')
   {
    for (byte i=0; i < 9; i++)
      {
         descript[i] = Serial.read();    
      } 
        
 if((descript[0] =='+') && (descript[1] =='=') && (descript[2] =='Z')) 
  {
   char val[3]; val[0] = char(descript[5]); val[1] = char(descript[6]); val[2] = char(descript[7]); 
   send_num = (int)descript[4] - 48;
   send_val = String(val);
   int value = send_val.toInt();
   switch (descript[3])
    {
      case 'S': //
      Serial.print("Arm ");
      Serial.print(send_num);
      Serial.print(" ");
      Serial.print(value);
      Serial.print("\n");
      ser( send_num, value );
      break;
      
      case 'M':
      Serial.print("Motor ");
      Serial.print(send_num);
      Serial.print(" ");
      Serial.print(value);
      Serial.print("\n");
      switch (send_num)
      {
        case 1:
        forward(value);
        break;
        
        case 2:
        back(value);
        break;

        case 3:
        turn_right(value);
        break;

        case 4:
        turn_left(value);
        break;
      }
      break;
    
     }
   }
   
  else
    {
      for(byte i=0; i < 255; i++)
       {
         Serial.read();    
       } 
    } 
   }// конец if (Serial.read()=='Y')
  } // конец чтение порта 
}

void forward(int ms){
  myservo5.write(0); //right rev
  myservo6.write(360); //left            
  delay(ms);
  myservo5.write(90);
  myservo6.write(90);
}

void back(int ms){
  myservo5.write(360); //right rev
  myservo6.write(0); //left            
  delay(ms);
  myservo5.write(90);
  myservo6.write(90);
}

void turn_left(int ms){
  myservo5.write(0); //right rev
  myservo6.write(0); //left            
  delay(ms);
  myservo5.write(90);
  myservo6.write(90);
}

void turn_right(int ms){
  myservo5.write(360); //right rev
  myservo6.write(360); //left            
  delay(ms);
  myservo5.write(90);
  myservo6.write(90);
}

void ser(int serv, int deg){
  int max = 360;
  int min = 0;
  int pos = 0;
  if (serv == 1) {
    myservo = myservo1; max = max1; min = min1;
    if(pos1 > max) pos1 = max;
    if(pos1 < min) pos1 = min;
    pos = pos1;
  }
  if (serv == 2) {
    myservo = myservo2; max = max2; min = min2;
    if(pos2 > max) pos2 = max;
    if(pos2 < min) pos2 = min;
    pos = pos2;
  }
  if (serv == 3) {
    myservo = myservo3; max = max3; min = min3;
    if(pos3 > max) pos3 = max;
    if(pos3 < min) pos3 = min;
    pos = pos3;
  }
  if (serv == 4) {
    myservo = myservo4; max = max4; min = min4;
    if(pos4 > max) pos4 = max;
    if(pos4 < min) pos4 = min;
    pos = pos4;
  }
  if (serv == 5) {
    myservo = myservo5; max = max5; min = min5;
    if(pos5 > max) pos5 = max;
    if(pos5 < min) pos5 = min;
    pos = pos5;
  }
  if (serv == 6) {
    myservo = myservo6; max = max6; min = min6;
    if(pos6 > max) pos6 = max;
    if(pos6 < min) pos6 = min;
    pos = pos6;
  }
  if(deg > max) deg = max;
  if(deg < min) deg = min;
  for (int i = pos; i <= deg; i += 1) {
    //Serial.print(i);
    //Serial.print("\n");
    myservo.write(i);              
    delay(15);
    pos = i;                   
  }
  for (int i = pos; i >= deg; i -= 1) {
    //Serial.print(i);
    //Serial.print("\n");
    myservo.write(i);              
    delay(15);
    pos = i;                   
  }
  if (serv == 1) pos1 = pos;
  if (serv == 2) pos2 = pos;
  if (serv == 3) pos3 = pos;
  if (serv == 4) pos4 = pos;
  if (serv == 5) pos5 = pos;
  if (serv == 6) pos6 = pos;
}
