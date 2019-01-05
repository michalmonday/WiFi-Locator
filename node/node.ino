
#define NODE_ID 3

#define SELF_AP


#ifdef SELF_AP
  #define URL "http://192.168.4.1:80/data"
#else
  #define URL "http://192.168.0.22:80/data"
#endif

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient
 
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  #ifdef SELF_AP
    WiFi.begin("locator_ap", "12345678"); 
  #else
    WiFi.begin("MyHomeWifi", "12345678");  
  #endif 
  
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);

    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
   digitalWrite(BUILTIN_LED, LOW);
   
   HTTPClient http;    //Declare object of class HTTPClient
 
   http.begin(URL);      //Specify request destination
   //http.addHeader("Content-Type", "text/plain");  //Specify content-type header
   http.addHeader("Content-Type", "application/x-www-form-urlencoded");

   char content[100] = {0};
   sprintf(content, "rssi=%d&id=%d", WiFi.RSSI(), NODE_ID);
   Serial.println("Post request content = " + String(content));
   int httpCode = http.POST(content);   //Send the request
   String payload = http.getString();                  //Get the response payload
 
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
 
   http.end();  //Close connection
 
 }else{
    digitalWrite(BUILTIN_LED, LOW);
    delay(100);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(100);
    Serial.println("Error in WiFi connection");   
 
 }

 //delay(100);
}
