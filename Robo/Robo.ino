#include <Multiservo.h>

Multiservo myservo;
Multiservo myservo1;
Multiservo myservo2;
Multiservo myservo3;
Multiservo myservo4;


int pos1 = 90;
int pos2 = 90;
int pos3 = 90;
int pos4 = 90;
int max1 = 160; int min1 = 90;
int max2 = 180; int min2 = 0;
int max3 = 180; int min3 = 0;
int max4 = 180; int min4 = 0;

byte descript[9];
int send_num;
String send_val;


void setup() {

  // hand
  myservo1.attach(17);
  myservo2.attach(16);
  myservo3.attach(15);
  myservo4.attach(14);

  //port
  Serial.begin(9600);

  arm_position_5(); // to base arm opened
  delay(1000);
  arm_position_6(); // to 1 lev arm opened
  delay(1000);
  
  arm_position_1(); // 1 lev arm open
  delay(1000);
  arm_position_2(); // 1 lev arm closed
  delay(1000);
  arm_position_3(); // to base arm closed
  delay(1000);
  arm_position_4(); // to 1 lev arm closed
  delay(1000);
  arm_position_1();  // 1 lev arm open
  delay(1000);
  arm_position_5();  // to base arm opened
  delay(1000);
  
}

void arm_position_1(){ // 1 lev arm open
  arm_move( 1, 90 );
  arm_move( 2, 90 );
  arm_move( 3, 140 );
  arm_move( 4, 180 );
  
}

void arm_position_2(){  //1 lev arm closed
  arm_move( 1, 130 );
  arm_move( 2, 90 );
  arm_move( 3, 140 );
  arm_move( 4, 180 );
  
}

void arm_position_3(){ // to base arm closed
  arm_move( 1, 130 );
  arm_move( 2, 90 );
  arm_move( 4, 160 );
  arm_move( 3, 120 );
  arm_move( 4, 140 );
  arm_move( 3, 100 );
  arm_move( 4, 120 );
  arm_move( 3, 80 );
  arm_move( 4, 100 );
  arm_move( 3, 60 );
  arm_move( 4, 80 );
  arm_move( 3, 40 );
  arm_move( 4, 60 );
  arm_move( 3, 0 );
}

void arm_position_4(){ // to 1 lev arm closed
  arm_move( 3, 0 );
  arm_move( 4, 60 );
  arm_move( 3, 40 );
  arm_move( 4, 80 );
  arm_move( 3, 60 );
  arm_move( 4, 100 );
  arm_move( 3, 80 );
  arm_move( 4, 120 );
  arm_move( 3, 100 );
  arm_move( 4, 140 );
  arm_move( 4, 140 );
  arm_move( 3, 120 );
  arm_move( 4, 160 );
  arm_move( 3, 140 );
  arm_move( 4, 180 );
  arm_move( 2, 90 );
  arm_move( 1, 130 );
}

void arm_position_5(){ // to base arm opened
  arm_move( 1, 90 );
  arm_move( 2, 90 );
  arm_move( 4, 160 );
  arm_move( 3, 120 );
  arm_move( 4, 140 );
  arm_move( 3, 100 );
  arm_move( 4, 120 );
  arm_move( 3, 80 );
  arm_move( 4, 100 );
  arm_move( 3, 60 );
  arm_move( 4, 80 );
  arm_move( 3, 40 );
  arm_move( 4, 60 );
  arm_move( 3, 0 );
}

void arm_position_6(){ // to 1 lev arm opened
  arm_move( 3, 0 );
  arm_move( 4, 60 );
  arm_move( 3, 40 );
  arm_move( 4, 80 );
  arm_move( 3, 60 );
  arm_move( 4, 100 );
  arm_move( 3, 80 );
  arm_move( 4, 120 );
  arm_move( 3, 100 );
  arm_move( 4, 140 );
  arm_move( 4, 140 );
  arm_move( 3, 120 );
  arm_move( 4, 160 );
  arm_move( 3, 140 );
  arm_move( 4, 180 );
  arm_move( 2, 90 );
  arm_move( 1, 90 );
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
      arm_move( send_num, value );
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
        //forward(value);
        break;
        
        case 2:
        //back(value);
        break;

        case 3:
        //turn_right(value);
        break;

        case 4:
        //turn_left(value);
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


void arm_move(int serv, int deg){
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
}
