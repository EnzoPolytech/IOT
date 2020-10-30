/* Ce code a été largement inspiré par les codes exemples fourni par Arduino*/

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "WIFI";
const char* password = "PASSWORD";

//Your Domain name with URL path or IP address with path
const char* serverName = "https://url-de-votre-application-node-red/sensor?";
float direction_temp = 0.1;
float temp_data = 24.0;
float hum_data = 51.0;
float direction_hum = 0.7;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST

      // ==== SIMULATING A FAKE DHT =====
      if(temp_data == 30.0){
        direction_temp = -0.1;
      }
      else if(temp_data == 20.0){
        direction_temp = 0.1;
      }
      if(hum_data >= 65.0){
        direction_hum = -0.7;
      }
      else if(hum_data <= 45.0){
        direction_hum = 0.7;
      }
      temp_data += direction_temp;
      hum_data += direction_hum;

      //=======================

      String temperature = String(temp_data);
      String humidity = String(hum_data);
      String temp_flag = "temperature=";
      String and_flag = "&";
      String hum_flag = "humidity=";
      String httpRequestData = temp_flag + temperature + and_flag + hum_flag + humidity;
          
             
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"UnDHTDeffectueux\",\"temperature\":\"24.25\",\"humidity\":\"49.54\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
