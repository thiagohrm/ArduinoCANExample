//Arduino with MCP2515 and DHT11

#include <SPI.h>
#include <mcp2515.h>
#include <DHT.h> 
#include <stdlib.h>

#define DHTPIN A0
#define DHTTYPE DHT11
#define TEMP_CONFIG 0
#define HUMI_CONFIG 1
#define STR_CONFIG 2

struct can_frame canMsgRcvd;
struct can_frame canMsgSend;
MCP2515 mcp2515(10);
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(115200);

  pinMode(TEMP_CONFIG, INPUT_PULLUP);
  pinMode(HUMI_CONFIG, INPUT_PULLUP);
  pinMode(STR_CONFIG, INPUT_PULLUP);
  
  SPI.begin();   
  dht.begin();  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");

  canMsgSend.can_id  = 0x00;           
  canMsgSend.can_dlc = 8;               
  canMsgSend.data[0] = 0x00;     
  canMsgSend.data[1] = 0x00;  
  canMsgSend.data[2] = 0x00;          
  canMsgSend.data[3] = 0x00;
  canMsgSend.data[4] = 0x00;
  canMsgSend.data[5] = 0x00;
  canMsgSend.data[6] = 0x00;
  canMsgSend.data[7] = 0x00;
}

void loop() {
  if (mcp2515.readMessage(&canMsgRcvd) == MCP2515::ERROR_OK) {
    Serial.print(canMsgRcvd.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsgRcvd.can_dlc, HEX); // print DLC
    Serial.print(" ");
    
    for (int i = 0; i<canMsgRcvd.can_dlc; i++)  {  // print the data
      Serial.print(canMsgRcvd.data[i],HEX);
      Serial.print(" ");
    }

    Serial.println();   
    switch(canMsgRcvd.can_id){
      case 0x01:
        if(digitalRead(TEMP_CONFIG)){
          sendCANMessage(0x04,dht.readTemperature());            
        }
        break;
      case 0x02:
        if(digitalRead(HUMI_CONFIG)){
          sendCANMessage(0x05,dht.readHumidity());
        }
        break;
      case 0x03:
        if(digitalRead(STR_CONFIG)){
          sendCANMessageStrings(0x06);
        }
        break;
    }
  }
}

void sendCANMessage(int canID,int value){
  canMsgSend.can_id = canID;
  canMsgSend.data[0] = value;  
  mcp2515.sendMessage(&canMsgSend);
}

void sendCANMessageStrings(int canID){
  canMsgSend.can_id = canID;
  canMsgSend.data[0] = atoi("F");
  canMsgSend.data[1] = atoi("O");
  canMsgSend.data[2] = atoi("R");
  canMsgSend.data[3] = atoi("D");
  canMsgSend.data[4] = atoi("-");
  canMsgSend.data[5] = random(255);
  canMsgSend.data[6] = random(255);
  canMsgSend.data[7] = random(255);
  mcp2515.sendMessage(&canMsgSend);
}
