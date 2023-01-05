# Arduino CAN Example #
This Example uses 3 Arduinos as ECUs 
- 2 equipped with DHT11 Temp and Humidity Sensor
- 1 with String Generator
this configuration can be changed via jumpers on D0 (Temp), D1 (Humidity) and D2 (String Gen).

the idea here is to use a PC with any sort of CAN adapter and send a CAN message with ID 0x01 (Temp) , 0x02 (Humidity) or 0x03 (String) with bit 1 on byte 0 to request the values from the Arduino ECU's on the CAN BUS.

# Dependencies #
- [MCP2515 Lib from AutoWP](https://github.com/autowp/arduino-mcp2515)
- [DHT11 Lib from AdaFruit](https://github.com/adafruit/DHT-sensor-library)
- [Adafruit Unified Driver from Adafruit](https://github.com/adafruit/Adafruit_Sensor)

# Schematics #
![image](https://user-images.githubusercontent.com/17009943/210840281-2bbf5742-b14b-4bd0-9204-a7b6d7a8929d.png)
![image](https://user-images.githubusercontent.com/17009943/210841721-305bc9f5-b6a4-4bdf-803a-f3f9f8cf08b0.png)
