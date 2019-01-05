#include "MenuOrganizer.h"
MenuOrganizer menuOrganizer;

#include "GUI.h"
GUI gui; 
/*
  [OLED -> NodeMCU]
    SDA -> D1 
    SCL -> D2
 */

#include "Buttons.h"
Buttons buttons;

#include "NamePicker.h"
NamePicker namePicker;

#include "Notification.h"
Notification notification;

#include "ProgressBar.h"
ProgressBar progressBar;

#include "Settings.h"
Settings settings;

#include "RSSI_gatherer.h"
RSSI_gatherer rssi_gatherer;

#define SELF_AP

#ifdef SELF_AP
  #define URL "http://192.168.4.1:80/data"
#else
  #define URL "http://192.168.0.22:80/data"
#endif

void setup(){
  Serial.begin(115200);
  
  SPIFFS.begin();
  //SPIFFS.format();
  buttons.Init();
  gui.Init();
  rssi_gatherer.Init("locator_ap", "12345678");

  //Serial.println("Before settings load");
  settings.Load(); 
  //Serial.println("After settings load");
}


void loop() {
  buttons.Update();
  gui.Update();
  
}
