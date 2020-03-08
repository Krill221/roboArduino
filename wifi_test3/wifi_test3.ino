#include <RoboHand.h>

#define SSID  "ARTOFNOISE"
#define PASS  "corbis42"
#define ESP8266 Serial1

RoboHand robo(8,7,6,5,9,10,
     13, 12,
     14, 11,
     10,
     9
);

String ans;
unsigned long lastTimeMillis = 0;

void printResponse() {
  while (ESP8266.available()) {
    Serial.println(ESP8266.readStringUntil('\n')); 
  }
}

void readResponse() {
  while (ESP8266.available()) {
    ans = ESP8266.readString();
  }
  ans = ans.substring(ans.indexOf("robo_s") + 6, ans.indexOf("robo_e"));
}

void setup() {
  Serial.begin(9600);   
  ESP8266.begin(9600);
  delay(2000);

  Serial.println("setupWiFi");
  ESP8266.println("AT");
  delay(1000);
  printResponse();
  // set mode 1 (client)
  ESP8266.println("AT+CWMODE=1");
  delay(1000);
  printResponse(); 
  // reset WiFi module
  ESP8266.print("AT+RST\r\n");
  delay(10000);
  printResponse();
  // join AP
  ESP8266.print("AT+CWJAP=\"");
  ESP8266.print(SSID);
  ESP8266.print("\",\"");
  ESP8266.print(PASS);
  ESP8266.println("\"");
  delay(10000);
  printResponse();
  
  ESP8266.println("AT+CIPMUX=1");
  delay(5000);
  printResponse();

}

void loop() {

  if (millis() - lastTimeMillis > 40000) {
    lastTimeMillis = millis();

    ESP8266.println("AT+CIPMUX=1");
    delay(1000);
    printResponse();

    ESP8266.println("AT+CIPSTART=4,\"TCP\",\"192.168.0.102\",3000");
    delay(1000);
    printResponse();

    String cmd = "GET / HTTP/1.1";
    ESP8266.println("AT+CIPSEND=4," + String(cmd.length() + 4));
    delay(1000);

    ESP8266.println(cmd);
    delay(1000);
    ESP8266.println("");
    
    readResponse();
    Serial.println("---");
    for(int i=0; i< ans.length(); i=i+2){
      run_command(ans.substring(i,i+2));
      delay(1000);
    }
    Serial.println("---");
    
  }

}



void run_command(String a){
  Serial.println(a);

  if( a == "R1" ){
    robo.putFrontToRight();
  }

  if( a == "F1" ){
    robo.putRightToFront();
  }

  if( a == "P1" ){
    robo.pickBox();
  }

  if( a == "D1" ){
    robo.putBox();
  }
  
}
