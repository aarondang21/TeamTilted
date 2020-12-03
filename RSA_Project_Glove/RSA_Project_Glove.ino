#include <RH_RF69.h>
#include <SPI.h>
#include <Wire.h>
#include <LSM6.h>

#define SCK 13 // clock - pin 13
#define MISO 12 // MISO - pin 12
#define MOSI 11 // MOSI - pin 11

#define t1_CS 9 // chip select 1 - pin 9 
#define t1_RST 8 // reset 1 - pin 8
#define t1_INT 3 // interrupt 1 - pin 3

RH_RF69 t1(t1_CS, t1_INT); // creates tranceiver obj 1

LSM6 imu;

void setup() {
  
  Serial.begin(9600);
  Wire.begin();

  if (!imu.init()) {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  
  imu.enableDefault();

  pinMode(t1_RST, OUTPUT); // set reset 1 pin to output mode

  digitalWrite(t1_RST, HIGH); // set reset 1 pin to high
  delay(10); // wait 10ms
  digitalWrite(t1_RST, LOW); // set reset 1 pin to low
  delay(10); // wait 10ms

  if (t1.init() != true) { // initialize transceiver 1 and check if successful
    Serial.println("t1 initialization failed"); 
    while (1) {}
  }
  
  t1.setFrequency(915); // set transciever 1 frequency to 915 MHz
  t1.setTxPower(17, true); // set transciever 1 power to 17
}

void loop() {
  
  imu.read();

  int x = imu.a.x;
  int y = imu.a.y;

  uint8_t data1 =  (uint8_t) map(x, -17000, 17000, 0, 255);
  uint8_t data2 = (uint8_t) map(y, -17000, 17000, 0, 255);
  
  uint8_t data[] = {data1, data2}; 
  t1.send(data, sizeof(data));
  t1.waitPacketSent();

  Serial.println(data1);

  delay(1000);
}
