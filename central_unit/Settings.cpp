#include "Settings.h"

#define MAX_SETTING_LEN 50

#include "GUI.h"
extern GUI gui;

#include "RSSI_gatherer.h"
extern RSSI_gatherer rssi_gatherer;


void Settings::Load(){
  gui.SetBrightness(Get("Brightness", 60).toInt());
  if(Get("FlipScreenVertically", 1).toInt() == 1){gui.FlipScreenVertically();}
  rssi_gatherer.SetMeanGatheringTime(/*int seconds*/ Get("RSSI_gathering_time", 10).toInt());
}


bool Settings::Set(String key, String val){
  String path = "/settings/" + key;

  File f = SPIFFS.open(path, "w+");
  if (f) {
    for(int i = 0; i < val.length(); i++){
      f.write(val[i]);
    }
    return true;
  }else{
    Serial.println("Settings::Set - Opening file for writing failed. File path:" + path);
    return false;
  }
}



String Settings::Get(String key, String backup_value){
  String path = "/settings/" + key;
  if(SPIFFS.exists(path)){
    File f = SPIFFS.open(path, "r");
    if (f) {
      f.seek(0,SeekSet);
      char buff[MAX_SETTING_LEN] = {0};
      f.readBytes(buff, f.size());
      buff[f.size()] = 0;
      return String(buff);      
    }else{
      Serial.println("Opening file for reading failed. File path: " + path);
      return backup_value;
    }
  }else{
    Serial.println("Settings::Get - File doesn't exist: " + path);
    Serial.println("Creating that file...");
    Set(key, backup_value);
    return backup_value;
  }
}
