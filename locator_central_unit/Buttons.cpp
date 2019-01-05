#include "Buttons.h"


struct ButtonMap{
  int pin;
  int button_id;
  String name_txt;
  std::function<void()> onRelease;
};


/*#define BUTTON_UP 10 //(SD3)
#define BUTTON_DOWN 2 //(D4)
#define BUTTON_A 14 //(D5)

#define BUTTON_LEFT 0 //(D3)
#define BUTTON_RIGHT 13 //(D7)
#define BUTTON_B 12 //(D6)*/

ButtonMap buttonMap[BUTTON_COUNT] = {
 10, BUTTON_UP, "UP", [](){},
 2,  BUTTON_DOWN, "DOWN", [](){},
 0,  BUTTON_LEFT, "LEFT", [](){},
 13, BUTTON_RIGHT, "RIGHT", [](){},
 14, BUTTON_YES, "ACCEPT", [](){},
 12, BUTTON_NO, "DECLINE", [](){},
};

void Buttons::Init(){
  for(int i = 0; i < BUTTON_COUNT; i++){
    pinMode(buttonMap[i].pin, INPUT_PULLUP);
  }
}

void Buttons::Update(){ 
  bool any_pressed = false;
  for(int i = 0; i < BUTTON_COUNT; i++){
    if(digitalRead(buttonMap[i].pin) == LOW){
      if(!last_button_pressed){
        last_button_pressed_time = millis();
      }
      last_button_pressed = buttonMap[i].button_id;
      any_pressed = true;
    }
  }
  
  if(!any_pressed){
    if(last_button_pressed){
      buttonMap[GetButtonIndex(last_button_pressed)].onRelease();
    }
    last_button_pressed = 0;
  }
}

bool Buttons::Limiter(int normal_speed_limit, int hold_speed_limit, int threshold){  
  int limit = (GetHoldingTime() > threshold) ? hold_speed_limit : normal_speed_limit;
  bool ret = ((millis() - last_button_passed_limiter_time) > limit) || (last_button_passed_limiter != last_button_pressed);

  if(ret == true){
    //Serial.println("Buttons::Limiter = true; holding time = " + String(GetHoldingTime()) + "; time since last button passed limiter = " + String(millis() - last_button_passed_limiter_time));
    last_button_passed_limiter_time = millis();
    last_button_passed_limiter = last_button_pressed;
    //if(last_button_pressed > 0){
      //Serial.println("Buttons::Limiter = true && last_button_pressed > 0; time = " + String(last_button_passed_limiter_time));
    //}
  }
  
  return ret;
}

int Buttons::GetButtonIndex(int button_id){
  for(int i = 0; i < BUTTON_COUNT; i++){
    if(buttonMap[i].button_id == button_id){
      return i;
    }
  }
  return 0;
}

String Buttons::GetButtonName(int button_id){
  return buttonMap[GetButtonIndex(button_id)].name_txt;
}

String Buttons::GetLastButtonName(){
  if(last_button_pressed){
    return GetButtonName(last_button_pressed);
  }
  return "-";
}

void Buttons::ResetFunctions(){
  for(int i = 0; i< BUTTON_COUNT; i++){
    buttonMap[i].onRelease = [](){};
  }
}

/*
void Buttons::SetButtonFunction(int button_id, std::function<void()> func){
  for(int i = 0; i < BUTTON_COUNT; i++){
    if(buttonMap[i].button_id == button_id){
      buttonMap[button_id].onRelease = func;
      return;
    }
  }

  Serial.println("Buttons::GetButtonName - NONE FOUND - button_id=" + String(button_id));
}
*/
