#pragma once
#include "Arduino.h"

#define TOTAL_STATES 5
enum StateNames
{
  STATE_WELCOME,
  STATE_TIMER_INPUT,
  STATE_POWER_INPUT,
  STATE_POWERED,
  STATE_SAFETY_ERROR
};
#define BACK_KEY '*'
#define CONFIRM_KEY '#'

#define PWR_EN_PIN 3
#define SAFETY_PIN 2
#define STATUS_LED_PIN 10
#define SAFETY_LED_PIN A5

class LCDPins
{
public:
  static const int pin_RS = 8; 
  static const int pin_EN = 9; 
  static const int pin_d4 = 4;
  static const int pin_d5 = 5; 
  static const int pin_d6 = 6; 
  static const int pin_d7 = 7; 
  static const int pin_BL = 10; 
};

class KeypadPins
{
public:
  static const byte ROWS = 4;
  static const byte COLS = 3;
  static const char hexKeys[ROWS][COLS];
  static const byte rowPins[ROWS]; 
  static const byte colPins[COLS]; 

};

