#include "RSSI_gatherer.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

#include "GUI.h"
extern GUI gui;
extern SH1106Wire display;  // idk what would be the best way to use display (only in GUI.cpp? But having it here too with separate "Draw" function within RSSI_gatherer class seems more handy...)

#include "Buttons.h"
extern Buttons buttons;

#include "MenuOrganizer.h"
extern MenuOrganizer menuOrganizer;

ESP8266WebServer server(80);

#define SELF_AP

void RSSI_gatherer::Init(char *ssid, char *password){
  Serial.printf("Connection status: %d\n", WiFi.status());
  Serial.printf("Connecting to %s\n", ssid);

  char ip_str[20] = {0};
  #ifdef SELF_AP
    Serial.println("Creating AP");
    if(WiFi.softAP(ssid, password)){
      Serial.println("AP was created :)"); 
    }else{
      Serial.println("AP was not created for some reason :(");
    }
    
    IPAddress ip = WiFi.softAPIP();
    Serial.println("IP received");
  #else
    WiFi.begin(ssid, password);
    Serial.printf("Connection status: %d\n", WiFi.status());
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.printf("\nConnection status: %d\n", WiFi.status());
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    IPAddress ip = WiFi.localIP();
  #endif

  server.on("/", [](){
    server.send(200, "text/plain", "Connected");
  });

  server.on("/data", HTTP_POST, [this](){
    if( ! server.hasArg("rssi") || server.arg("rssi") == NULL ||  ! server.hasArg("id") || server.arg("id") == NULL) { 
      Serial.println("Post request to /data did not have rssi or id fields.");
      server.send(400, "text/plain", "400: Invalid Request");
    }else{
      int id = String(server.arg("id")).toInt();
      rssi[id] =  String(server.arg("rssi")).toInt();
      last_rec[id] = millis();

      if(rssi_mode == RSSI_MODE_GPS){
        server.send(200, "text/plain", "OK - send GPS too");
      }else{
        server.send(200, "text/plain", "OK"); 
      }
      
      if(gather_mean_end){
        rssi_total[id] += rssi[id];
        rssi_total_count[id]++;
      }

      if(rssi_mode == RSSI_MODE_GPS && server.hasArg("lat") &&  server.hasArg("lng")  &&  server.hasArg("satellites") ) { 
        gps_line = String(server.arg("lat")) + "," + String(server.arg("lng")) + "," + String(server.arg("satellites"));
        Serial.println("gps_line = " + gps_line);

        
        /* NOTE
          At this point the gps info should be saved into SPIFFS (possibly already formatted to be easily used with Google Maps API)
          https://developers.google.com/maps/documentation/javascript/datalayer
          https://developers.google.com/maps/documentation/javascript/shapes#circle_add
          https://developers.google.com/maps/documentation/javascript/examples/circle-simple
          https://developers.google.com/maps/documentation/javascript/examples/layer-heatmap
        */
      }

    }
  });

  server.begin();
}



void RSSI_gatherer::Update(){
  //display.drawString(64,32, "--- TEST ---");
  server.handleClient();

  if(gather_mean_end && millis() > gather_mean_end){
    gather_mean_end = 0;
    CalculateMean();
  }

  if(rssi_mode == RSSI_MODE_GPS && gps_line){
    
  }

  HandleButtons();
}

void RSSI_gatherer::CalculateMean(){
  if(!rssi_total_count[0] || !rssi_total_count[1] || !rssi_total_count[2] || !rssi_total_count[3]){
    menuOrganizer.Notify("Not enough data", 1000);
  }else{
    for(int i = 0; i < 4; i++){
      last_mean_rssi[i] = rssi_total[i] / rssi_total_count[i];
    }  
  }
}

void RSSI_gatherer::HandleButtons(){
  int key = buttons.GetLast(true);
  switch(rssi_mode){
    case RSSI_MODE_REAL_TIME:
      if(key == BUTTON_NO){
        gui.SetMode(MODE_MENU);
      }
      break;
      
    case RSSI_MODE_MEAN:
      if(key == BUTTON_NO){
        if(IsMeanBeingGathered()){
          gather_mean_end = 0;
          CalculateMean();
        }else{
          gui.SetMode(MODE_MENU);
        }      
      }
      break;

    case RSSI_MODE_GPS:
      if(key == BUTTON_NO){
        gui.SetMode(MODE_MENU);
      }
      break;
  }
}




void RSSI_gatherer::DrawRSSI(bool use_mean){
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  
  int inactivity_circle_radius = 10;
  
  int radius = (int)(RES_Y * 0.3);
  int center_y = RES_Y / 2;
  int center_x = RES_X / 2;

  float screen_pos[4][2] = {
    {center_x, center_y - radius}, // front node (0)
    {center_x, center_y + radius}, // back node (1)
    {center_x - radius, center_y}, // left node (2)
    {center_x + radius, center_y}  // right node (3)
  };  

  int rssi[4];
  for(int i = 0; i < 4; i++){
    if(use_mean){
      rssi[i] = rssi_gatherer.GetNodeMeanRSSI(i);
    }else{
      rssi[i] = rssi_gatherer.GetNodeRSSI(i);
    }
  }

  char s[10] = {0};
  for(int i = 0; i < 4; i++){
    sprintf(s, "%d", rssi[i]);
    display.drawString(screen_pos[i][0], screen_pos[i][1], s);
    if(!use_mean && millis() - rssi_gatherer.GetNodeLastUpdateTime(i) > 1000){
      display.drawCircle(screen_pos[i][0], screen_pos[i][1], inactivity_circle_radius);
    } 
  }

  if(rssi[0] && rssi[1] && rssi[2] && rssi[3]){
    int forwards = rssi[1] - rssi[0];
    int sideways = rssi[3] - rssi[2];

    int rssi_dist = int(sqrt(forwards * forwards + sideways * sideways));
    float dir = atan2(sideways, forwards); // in radians

    display.setTextAlignment(TEXT_ALIGN_LEFT);
 
    display.drawString(0,0, "Ang=" + String((int)(dir * RAD_TO_DEG)));
    display.drawString(0,54, "Dist=" + String(rssi_dist));
    
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);

    int line_x_destination = center_x + int(sin(dir) * float(radius));
    int line_y_destination = center_y + int(cos(dir) * float(radius));

    display.drawLine(center_x, center_y, line_x_destination, line_y_destination);
  }else{
    display.drawString(center_x, center_y, "?");
  }
  
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}

void RSSI_gatherer::Draw(){
  switch(rssi_mode){
    case RSSI_MODE_REAL_TIME:
      DrawRSSI(false);
      
      break;
    case RSSI_MODE_MEAN:
      if(IsMeanBeingGathered()){
        display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
        display.drawString(RES_X / 2, RES_Y / 2 - 12, "Gathering rssi values");
        char counts[40] = {0}; sprintf(counts, "RSSI's: %d %d %d %d", rssi_total_count[0], rssi_total_count[1], rssi_total_count[2], rssi_total_count[3]);
        display.drawString(RES_X / 2, RES_Y / 2, counts);
        unsigned long ms = millis(); 
        ms = (ms > gather_mean_end ? gather_mean_end : ms); // to avoid small bug (because at one point millis becomes higher than gather_mean_end, they're both unsigned so it can't happen
        display.drawString(RES_X / 2, RES_Y / 2 + 12, "Time left: " + String((gather_mean_end - ms) / 1000 + 1) + " sec");
      }else{
        DrawRSSI(true);
      }
      break;
    case RSSI_MODE_GPS:

      break;
  }
}
  

void RSSI_gatherer::Reset(){
  for(int i = 0; i < 4l; i++){
    rssi[i] = 0;
    last_rec[i] = 0;
    last_mean_rssi[i] = 0;
    rssi_total[i] = 0;
    rssi_total_count[i] = 0;
  }
  gather_mean_end = 0;
}
