#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include <functional>


#define BUTTON_UP 1
#define BUTTON_DOWN 2
#define BUTTON_LEFT 3
#define BUTTON_RIGHT 4
#define BUTTON_YES 5
#define BUTTON_NO 6

#define BUTTON_COUNT 6

#define STATE_UP 0
#define STATE_DOWN 1

  
class Buttons
{
public: 
  void Init();
  
  void Update();
  int GetLast(){return last_button_pressed;}
  int GetLast(bool use_default_limiter){if(use_default_limiter){return Limiter() ? GetLast() : 0;}else{return GetLast();}}
  
  int GetHoldingTime(){return last_button_pressed > 0 ? millis() - last_button_pressed_time : 0;}
  unsigned long GetLastPressTime(){return last_button_pressed_time;}

  bool Limiter(int normal_speed_limit=250, int hold_speed_limit=125, int threshold=600);

  //void ForceRelease(){
  //  last_button_pressed = 0;
  //}

  String GetLastButtonName();

  String GetButtonName(int button_id);
  String GetButtonName(String button_id){char buff[2] = {button_id[0], 0};return GetButtonName((int)strtol(buff, NULL, 10));}
  String GetButtonName(char button_id) {return GetButtonName((int)button_id);}
  //String GetButtonName(char* button_id);

  void ResetFunctions();
  void SetButtonFunction(int button_id, std::function<void()> func);
private:
  int last_button_pressed = 0;
  unsigned long last_button_pressed_time = 0;

  unsigned long last_button_passed_limiter_time = 0;
  int last_button_passed_limiter = last_button_passed_limiter;
  
  int GetButtonIndex(int button_id);
};













#endif
