#include <Servo.h>
#include <SPI.h>
#include <RH_RF69.h>
#include <LIS3MDL.h>
#include <EEPROM.h>
#include <EEWrap.h>

#define SCK 13 // clock - pin 13
#define MISO 12 // MISO - pin 12
#define MOSI 11 // MOSI - pin 11
#define t1_CS 9 // chip select 1 - pin 9 
#define t1_RST 8 // reset 1 - pin 8
#define t1_INT 3 // interrupt 1 - pin 3
#define button 2 // button interrupt - pin 2

RH_RF69 t1(t1_CS, t1_INT);
Servo servo_y;
Servo servo_x;

//int last;

//enum States {initialize, play, complete};
//States myState = initialize;

//1 - initialize, 2 - play, 3 - complete
//int myState = 1;

int initial_t;

void setup() {

  Serial.begin(9600);

  //Serial.println("A");
  servo_y.attach(5);
  servo_x.attach(4);
  //Serial.println("B");

  pinMode(button, INPUT_PULLUP);

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

  Serial.println("Press button to start!");
  while (digitalRead(button) == HIGH) {}
  Serial.println("GO!");
  initial_t = millis();

}

void loop() {

  //Serial.println("C");

  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  //Serial.println("D");
  t1.waitAvailable();
  //Serial.println("E");
  t1.recv(buf, &len);
  //Serial.println("F");

  //int x = (int) buf[0];
  //int y = (int) buf[1];

  int x = map(buf[0], 0, 255, 180, 0);
  int y = map(buf[1], 0, 255, 0, 180);
  //Serial.println("G");

  //Serial.println("H");


  servo_y.write(y);
  servo_x.write(x);
  
  int sensor = analogRead(A5);
  if (sensor < 450 || sensor > 600) {
    int final_t = (millis() - initial_t) / 1000;
    servo_y.write(90);
    servo_x.write(90);
    Serial.println("You win!");
    Serial.print("Time: ");
    Serial.println(final_t); // print time here
    Serial.println("Place ball at the beginning and press the button to reset.");
    while (digitalRead(button) == HIGH) {}
    initial_t = millis();  
  }
  
}
/*
  Serial.println("HERE");

  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  t1.waitAvailable();
  t1.recv(buf, &len);

  switch(myState) {

    case 1: {

      Serial.println("I");

      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Place ball at start and press button to begin!");
      display.display();


      if (buttonState = true) {
        buttonState = false;
        init_t = millis();
        myState = play;
      }
    }

    case 2: {

      Serial.println("P");

      display.clearDisplay();
      display.setCursor(0, 0);
      display.print(buf[0]);
      display.print(", ");
      display.print(buf[1]);
      display.print(" : ");

      curr_t = (init_t - millis()) / 1000;
      display.print(curr_t);
        
      display.display();

      int sensor = analogRead(A0);
      if (sensor < 500 || sensor > 550) {
        myState = 3;  
      }
    }

    case 3: {
      Serial.println("DONE");
    }
    

    case complete: {

      Serial.println("C");

      display.print("You Win! Time: ");
      display.println(curr_t);
      display.print("Press button to play again");
      while (buttonState == false) {}
      buttonState = false;
      init_t = millis();
      myState = play;
    }
    
  }  

  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  t1.waitAvailable(); 
  if (t1.recv(buf, &len)) {

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(buf[0]);
    display.print(", ");
    display.print(buf[1]);
    display.display();
  }
  int sensor = analogRead(A0);
  Serial.println(sensor);
  if (sensor < 500 || sensor > 550) {
    Serial.println("You Win!");
  }
}
*/  
