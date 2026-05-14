#pragma once
#include "Arduino.h"
#include "static.h"
#include "state.h"

#define FIXED_POWER_PERCENTAGE 125

class PoweredState : public State
{
public:
  PoweredState(LiquidCrystal *lcd,int * outputs):
    State(lcd),
    outputs(outputs),
    done(false),
    millis_left(0)
  {

  }

  virtual void Setup()
  {
    State::Setup();
    total_seconds = outputs[STATE_TIMER_INPUT];
    power_percentage = outputs[STATE_POWER_INPUT];
    Serial.write(String(total_seconds).c_str());

    //If coming from a safety error, dont reset timer if the user presses continue
    bool reset_timer = !static_cast<bool>(outputs[STATE_SAFETY_ERROR]);

    lcd->clear();
    lcd->setCursor(0,0);
    lcd->print("UV Powered");
    lcd->setCursor(0,1);

    if(reset_timer)
    {
      millis_left = total_seconds * 1000;
      done = false;
      String msg = "Reset timer to " + String(millis_left) + " millis";
      Serial.write(msg.c_str());
    }

    //analogWrite(PWR_EN_PIN,power_percentage);
    analogWrite(PWR_EN_PIN,FIXED_POWER_PERCENTAGE);
  }

  virtual void KeyCallback(KeypadEvent key)
  {
    if(key == BACK_KEY || key == CONFIRM_KEY)
    {
      result = State::Result::FAILURE;
      next_state = STATE_WELCOME;
      output = 0;
      analogWrite(PWR_EN_PIN,0);
    }
  }

  virtual int GetOutput()
  {
    return output;
  }

  virtual int GetNextState()
  {
    return next_state;
  }

  virtual void Update(int delta_millis)
  {
    if(!done)
    {
      millis_left -= delta_millis;
      int seconds_left = static_cast<float>(millis_left) / 1000;
      if(seconds_left != last_seconds_left)
      {
        if(seconds_left > 0)
        {
          int minutes = static_cast<int>(seconds_left) / 60;
          int seconds = static_cast<int>(seconds_left) % 60;
          char minutes_str[2];
          char seconds_str[2];
          sprintf(minutes_str, "%02d", minutes);
          sprintf(seconds_str, "%02d", seconds);
          lcd->setCursor(0,1);
          lcd->print(minutes_str[0]);
          lcd->print(minutes_str[1]);
          lcd->print(":");
          lcd->print(seconds_str[0]);
          lcd->print(seconds_str[1]);
        }
        else
        {
          lcd->setCursor(0,1);
          lcd->print("00:00 Done!");
          digitalWrite(PWR_EN_PIN,LOW);
          output = 1;
          done = true;
        }
      }

      //Check safety
      int safety_ok = digitalRead(SAFETY_PIN);
      if(!safety_ok)
      {
        result = State::Result::FAILURE;
        next_state = STATE_SAFETY_ERROR;
        analogWrite(PWR_EN_PIN,0);
        output = 0;
      }

      last_seconds_left = seconds_left;
    }
  }

private:
  String time_remaining;
  int next_state;
  int * outputs;
  int output;
  bool done;

  unsigned long total_seconds;
  int power_percentage;
  int last_seconds_left;
  unsigned long millis_left;
};