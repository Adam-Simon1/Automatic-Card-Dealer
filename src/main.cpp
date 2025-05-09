#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <PCF8574.h>

#include "display.h"
#include "movement.h"

#define ROTATE_MOTOR_1 12
#define ROTATE_MOTOR_2 13
#define CARD_MOTOR_1 27
#define CARD_MOTOR_2 14

int speed = 0;

void setup()
{
  Serial.begin(921600);
  Wire.begin();
  displayInit();

  pinMode(ROTATE_MOTOR_1, OUTPUT);
  pinMode(ROTATE_MOTOR_2, OUTPUT);

  pinMode(CARD_MOTOR_1, OUTPUT);
  pinMode(CARD_MOTOR_2, OUTPUT);

  delay(1000);
  home();
  delay(300);
  speed = measureSpeed();
  delay(300);
}

void loop()
{
  display(speed);
}
