#include <CrawlerWheels.h>
#include <RoboHand.h>

#define BUFFER_SIZE 512
#define SSID  "ARTOFNOISE"
#define PASS  "corbis42"
#define PORT  "8080"
#define esp Serial1

char buffer[BUFFER_SIZE];
char OKrn[] = "OK\r\n";
byte wait_for_esp_response(int timeout, char* term=OKrn);

CrawlerWheels wheels;
RoboHand hand;

void setup() {
  esp.begin(9600);
  Serial.begin(9600);
  Serial.println("begin");
    
  setupWiFi();

  hand.init(17, 16, 15, 14);
  hand.arm_position_base_open();
  hand.free();

  wheels.init(220, 255);
  wheels.init_wheels(7, 6);
}


void loop() {
  int ch_id, packet_len;
  char *pb;  
  if(read_till_eol()) {
    if(strncmp(buffer, "+IPD,", 5)==0) {
      sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
      if (packet_len > 0) {
        pb = buffer+5;
        while(*pb!=':') pb++;
        pb++;
        if (strncmp(pb, "GET /", 5) == 0) {
          String pb_str = String(pb).substring(0,11);
          if (pb_str == "GET /?path=" ) {
            
            String req_str = String(pb).substring(11);
            Serial.println("--------");
            req_str.trim();
            int index = req_str.indexOf(' ');
            req_str.trim();
            String path_str = req_str.substring(0, index);
            int pathCount = req_str.length()/3;
            for(int i = 0; i < pathCount; i++){
              String item = getValue(path_str, ';', i);
              String item1 = item.substring(0,1);
              String item2 = item.substring(1);
              Serial.println( item1 + ":" + item2 );
              if( item1 == "R" ){
                if(item2 == "L"){
                  Serial.println("turn left");
                  wheels.rotate_left_90();
                }
                if(item2 == "R"){
                  Serial.println("turn right");
                  wheels.rotate_right_90();
                }
              }
              if( item1 == "F" ){
                int steps = item2.toInt();
                for(int i=0; i < steps; i++){
                  wheels.move_step_forward();
                }
              }
              if( item1 == "B" ){
                int steps = item2.toInt();
                for(int i=0; i < steps; i++){
                  wheels.move_step_back();
                }
              }
              if( item1 == "P" ){
                if( item2 == "1"){
                  hand.init(17, 16, 15, 14);
                  hand.arm_position_base_open();
                  hand.arm_move_level_1_open();
                  hand.arm_position_level_1_closed();
                  hand.arm_move_base_closed();
                  hand.free();
                }
              }
              if( item1 == "D" ){
                if( item2 == "1"){
                  hand.init(17, 16, 15, 14);
                  hand.arm_position_base_closed();
                  hand.arm_move_level_1_closed();
                  hand.arm_position_level_1_open();
                  hand.arm_move_base_open();
                  hand.free();
                }
              }
              
              
            }
            Serial.println("--------");
          }
          get_headers();
          serve_homepage(ch_id);
        }
      }
    }
  }
}


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


byte wait_for_esp_response(int timeout, char* term=OKrn) {
  unsigned long t=millis();
  bool found=false;
  int i=0;
  int len=strlen(term);
  while(millis()<t+timeout) {
    if(esp.available()) {
      buffer[i++]=esp.read();
      if(i>=len) {
        if(strncmp(buffer+i-len, term, len)==0) {
          found=true;
          break;
        }
      }
    }
  }
  buffer[i]=0;
  Serial.print(buffer);
  return found;
}

void get_headers() {
  unsigned long t=millis();
  bool found=false;
  int i=0;
  int timeout = 1000;
  char term[] = "OK\r\n";
  int len=strlen(term);
  while(millis()<t+timeout) {
    if(esp.available()) {
      buffer[i++]=esp.read();
      if(i>=len) {
        if(strncmp(buffer+i-len, term, len)==0) {
          found=true;
          break;
        }
      }
    }
  }
  buffer[i]=0;
}

bool read_till_eol() {
  static int i=0;
  if(esp.available()) {
    buffer[i++]=esp.read();
    if(i==BUFFER_SIZE)  i=0;
    if(i>1 && buffer[i-2]==13 && buffer[i-1]==10) {
      buffer[i]=0;
      i=0;
      Serial.print(buffer);
      return true;
    }
  }
  return false;
}

void serve_homepage(int ch_id) {
  String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n";

  String content="<div>OK</div>\n";

  header += "Content-Length:";
  header += (int)(content.length());
  header += "\r\n\r\n";
  esp.print("AT+CIPSEND=");
  esp.print(ch_id);
  esp.print(",");
  esp.println( header.length() + content.length() );
  char bolshe [] = "> ";
  if( wait_for_esp_response(2000, bolshe ) ) {
    esp.print(header);
    esp.print(content);
  } else {
    esp.print("AT+CIPCLOSE=");
    esp.println(ch_id);
  }
}


void setupWiFi() {
  // try empty AT command
  esp.println("AT");
  wait_for_esp_response(1000);

  // set mode 1 (client)
  esp.println("AT+CWMODE=1");
  wait_for_esp_response(1000);  

  // reset WiFi module
  esp.print("AT+RST\r\n");
  wait_for_esp_response(1500);
  delay(3000);
 
  // join AP
  esp.print("AT+CWJAP=\"");
  esp.print(SSID);
  esp.print("\",\"");
  esp.print(PASS);
  esp.println("\"");
  // this may take a while, so wait for 5 seconds
  wait_for_esp_response(5000);
  
  esp.println("AT+CIPSTO=30");  
  wait_for_esp_response(1000);

  // start server
  esp.println("AT+CIPMUX=1");
  wait_for_esp_response(1000);
  
  esp.print("AT+CIPSERVER=1,"); // turn on TCP service
  esp.println(PORT);
  wait_for_esp_response(1000);

  // print device IP address
  Serial.println("WIFI device on");
  esp.println("AT+CIFSR");
  wait_for_esp_response(1000);
    
}
