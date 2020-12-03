#include <SPI.h>
#include <RH_RF69.h>
#include <LIS3MDL.h>
#include <EEPROM.h>
#include <EEWrap.h>
#include <Adafruit_SSD1306.h>

#define SCK 13 // clock - pin 13
#define MISO 12 // MISO - pin 12
#define MOSI 11 // MOSI - pin 11
#define t1_CS 9 // chip select 1 - pin 9 
#define t1_RST 8 // reset 1 - pin 8
#define t1_INT 3 // interrupt 1 - pin 3
#define buttonPin 2 // button interrupt - pin 2
#define SCREEN_WIDTH 128 // OLED display width in pixels
#define SCREEN_HEIGHT 64 // OLED display height in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RH_RF69 t1(t1_CS, t1_INT);

volatile int init_t = 0;
volatile int curr_t = 0;
volatile bool buttonState = false;

//enum States {initialize, play, complete};
//States myState = initialize;

// 1 - initialize, 2 - play, 3 - complete
int myState = 1;

void setup() {

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(2), button, FALLING);

  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  pinMode(buttonPin, INPUT_PULLUP);

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

  Serial.println("HERE");

  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  //t1.waitAvailable();
  //t1.recv(buf, &len);

  switch(myState) {

    case 1: {

      Serial.println("I");

      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("Place ball at start and press button to begin!");
      display.display();

/*
      if (buttonState = true) {
        buttonState = false;
        init_t = millis();
        myState = play;
      }
      */
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
    
    /*

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

  */
    
  }  

/*
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
*/  
}

void button() {

  buttonState = true;
  Serial.println("BUTTON");
  myState = 2;
}
