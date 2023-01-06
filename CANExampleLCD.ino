//Arduino with MCP2515 and 16x2 I2C LCD to show info from other arduinos on CAN Bus

#include <SPI.h>
#include <mcp2515.h>
#include <stdlib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define col 16
#define lin  2
#define address  0x3F

const long interval = 2000;

struct can_frame canMsgRcvd;
struct can_frame canMsgSend;
MCP2515 mcp2515(10);
LiquidCrystal_I2C lcd(address,col,lin);
int temp = 0;
int hum = 0;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  SPI.begin();   

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
  unsigned long currentMillis = millis();
  bool msgRet = false;
    
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    lcd.setCursor(1,0);
    lcd.print("Requesting new data ");

    sendCANMessage(1,1);
    sendCANMessage(2,1);

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
      if(canMsgRcvd.data[0] == 1){
        switch(canMsgRcvd.can_id){
          case 0x04:
            temp = canMsgRcvd.data[0];
            break;
          case 0x05:
            hum = canMsgRcvd.data[0];
            break;
        }
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Temperature = ");
        lcd.print(temp);
        lcd.setCursor(2,0);
        lcd.print("Humidity = ");
        lcd.print(hum);
      }
    }
  }
}

void sendCANMessage(int canID,int value){
  canMsgSend.can_id = canID;
  canMsgSend.data[0] = value;  
  mcp2515.sendMessage(&canMsgSend);
}


