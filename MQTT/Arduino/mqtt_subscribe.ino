#include <WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>

//WIFI
const char* ssid = "SSID";
const char* password = "PASSWORD";

//MQTT
#define MQTT_BROKER       "broker.hivemq.com"
#define MQTT_BROKER_PORT  1883
#define MQTT_USERNAME     ""
#define MQTT_KEY          "" 

//DHT
#define DHTTYPE 11      // DHT 11 
#define DHTPIN 27       // Digital pin connected to the DHT sensor

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

// Space to store values to send
char str_sensor[10];

void setup() {

  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  
  setup_wifi();
  setup_mqtt();
}

void loop() {
  reconnect();
  client.loop(); 

  // SUBSCRIBE

  client.subscribe("esp/led");
  delay(10000);
}

void setup_wifi(){  

  WiFi.begin(ssid, password);
  
  //connexion au wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi connecté");
  Serial.print("MAC : ");
  Serial.println(WiFi.macAddress());
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void setup_mqtt(){
  client.setServer(MQTT_BROKER, MQTT_BROKER_PORT);
  client.setCallback(callback);//Déclaration de la fonction de souscription
  reconnect();
}

void reconnect(){
  while (!client.connected()) {
    Serial.println("Connection au serveur MQTT ...");
    if (client.connect("ESPClient", MQTT_USERNAME, MQTT_KEY)) {
      Serial.println("MQTT connecté");
    }
    else {
      Serial.print("echec, code erreur= ");
      Serial.println(client.state());
      Serial.println("nouvel essai dans 2s");
    delay(2000);
    }
  }
}

//Callback doit être présent pour souscrire a un topic et de prévoir une action 
void callback(char* topic, byte *payload, unsigned int length) {
   Serial.println("-------Nouveau message du broker mqtt-----");
   Serial.print("Canal:");
   Serial.println(topic);
   Serial.print("donnee:");
   Serial.write(payload, length);
   Serial.println();
   
   if ((char)payload[0] == '1') {
     Serial.println("LED ON");
     digitalWrite(LED_BUILTIN, HIGH); 
   } else {
     Serial.println("LED OFF");
     digitalWrite(LED_BUILTIN, LOW); 
   }
 }
