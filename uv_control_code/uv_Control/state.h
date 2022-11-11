#pragma once
#include <Keypad.h>
#include <LiquidCrystal.h>

class State
{
  public:
    State(LiquidCrystal *lcd):
      lcd(lcd)
      {}
    
    enum class Result
    {
      WAITING,
      SUCCESS,
      FAILURE
    };

    //virtual void SetArguments(int argc,int *argv[]);
    virtual void Setup() = 0;
    virtual Result KeyCallback(KeypadEvent key) = 0;
    virtual int GetOutput() = 0;

protected:
  LiquidCrystal *lcd;
  Result result;
};