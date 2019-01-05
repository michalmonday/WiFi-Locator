
#define NODE_ID 0

#define GPS_Serial Serial

#define SELF_AP

#ifdef SELF_AP
  #define URL "http://192.168.4.1:80/data"
#else
  #define URL "http://192.168.0.22:80/data"
#endif

#include <TinyGPS++.h>
TinyGPSPlus gps;

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

bool gps_requested = false;


//https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient
 
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);

  GPS_Serial.begin(9600);
  
  WiFi.mode(WIFI_STA);
  #ifdef SELF_AP
    WiFi.begin("locator_ap", "DTRUTDSS"); 
  #else
    WiFi.begin("MyHomeWiFi", "12345678");  
  #endif 
  
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
  }
 
}
 
void loop() {
 
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    digitalWrite(BUILTIN_LED, LOW);
    
    HTTPClient http;    //Declare object of class HTTPClient
    
    http.begin(URL);      //Specify request destination
    //http.addHeader("Content-Type", "text/plain");  //Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    char content[150] = {0};

    
    
  if(gps_requested){
    if(GPS_Serial.available()){
      unsigned long start_time = millis();
      do{
        while (GPS_Serial.available()){
          gps.encode(GPS_Serial.read());
        }
      }while(millis() - start_time < 300);
      
  
      if(gps.location.isValid() && gps.satellites.isValid()){
        sprintf(content, "rssi=%d&id=%d&lat=%f&lng=%f&satellites=%d", 
          WiFi.RSSI(), 
          NODE_ID,
          gps.location.lat(),
          gps.location.lng(),
          gps.satellites.value()
          );
      }else{
        sprintf(content, "rssi=%d&id=%d", WiFi.RSSI(), NODE_ID);
      }
    }else{
      sprintf(content, "rssi=%d&id=%d", WiFi.RSSI(), NODE_ID);
    }
  }else{
    while(Serial.available()){char c = GPS_Serial.read();} // just read and get rid of it (if it's not needed)
    sprintf(content, "rssi=%d&id=%d", WiFi.RSSI(), NODE_ID);
  }
  
  
   
  int httpCode = http.POST(content);   //Send the request
  String payload = http.getString();   //Get the response payload
  if(payload.indexOf("send GPS too") >= 0){
    gps_requested = true;
  }else{
    gps_requested = false;
  }
  
 
   http.end();  //Close connection
 
 }else{
    digitalWrite(BUILTIN_LED, LOW);
    delay(100);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(100);
 }
}
