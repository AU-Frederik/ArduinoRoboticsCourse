/**/
#include <Arduino.h>
#include <TwaiCan.h>
#include <MKSServoCAN.h>
#include <ESP32Encoder.h>
#define MOTOR_ID 1

TwaiCan bus(GPIO_NUM_27, GPIO_NUM_26);

#define RES 14400
#define ROUND 360

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 500;  //the value is a number of milliseconds

/*
WORKING MODES
0 - CR_open
1 - CR_closed
2 - CR_vFOC
3 - SR_open
4 - SR_closed
5 - SR_vFoc
*/

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(2500);

  if (!MKSServoCAN::begin(&bus)) {
    Serial.println("CAN init failed");
    while (1);
  }

  MKSServoCAN::setWorkMode(MOTOR_ID, 5);
  // MKSServoCAN::speedMode(MOTOR_ID, 5, 1, 1);

  delay(1000);
  int degree = 360*3;
  int clks = degree*900/101.5;
  MKSServoCAN::posRelative(MOTOR_ID, clks, 50, 5, 0);
  
  // MKSServoCAN::posAbsolute(MOTOR_ID, clks, 50, 5);
}


void loop() {
    currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    MKSServoCAN::pollResponses();
    startMillis = currentMillis;
  }
  /*

  if (currentMillis > 10000) {
    MKSServoCAN::enableMotor(MOTOR_ID, 0);
  }*/
}
