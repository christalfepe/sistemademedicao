#include <Arduino.h>

#define flowsensor 21 
#define trigPin 23
#define echoPin 22


#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>  // 6.14.1
#include <Wire.h>


#define VERSION 1


// WiFi
const char *ssid = "Galaxy_A51_766E";  // Enter your WiFi name
const char *password = "fpio9676";    // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "0.tcp.sa.ngrok.io";

String _topic = "sensor/" + WiFi.macAddress() + "/out"; 


const char *topic = _topic.c_str(); 
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 14824;


WiFiClient espClient;
PubSubClient client(espClient);

 //topic = &String(WiFi.macAddress());

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

double flow; //Water flow L/Min 
unsigned long currentTime;
unsigned long lastTime;
unsigned long pulse_freq;
long duration, distance, old_distance, distancia; 
 
void pulse () // Interrupt function
{
   pulse_freq++;
}

void setup()
{
  // Definir entradas e saídas:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(flowsensor, INPUT);
  
  attachInterrupt(0, pulse, RISING); // Setup Interrupt
  currentTime = millis();
  lastTime = currentTime;

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  //connecting to a mqtt broker

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  // publish and subscribe
   //client.publish(topic, "hello");

   //client.subscribe(topic);
}

float vazao()
{
  currentTime = millis();
  // Every second, calculate and print L/Min
  if(currentTime >= (lastTime + 1000))
  {
    lastTime = currentTime; 
    // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
    flow = (pulse_freq / 7.5); 
    pulse_freq = 0; // Reset Counter
    Serial.print(flow, DEC); 
    Serial.println(" L/Min");    
  }
  return flow;
}

float ultrassom() {
  // Limpe o trigPin configurando-o como LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  // Acione o sensor definindo o trigPin alto por 10 microssegundos:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Leia o echoPin, pulseIn() retorna a duração (comprimento do pulso) em microssegundos:
  duration = pulseIn(echoPin, HIGH);
  // Calcule a distância:
  distance = duration/58.82;
  distancia = 17 - distance; 

  if(distancia){ //old_distance != distancia && old_distance +1 && old_distance != distancia -1){
    // Imprima a distância no Monitor Serial (Ctrl + Shift + M):
    Serial.print("Distancia = ");
    Serial.print(distancia);
    Serial.println(" cm");
    old_distance = distancia;
  }    
  delay(500);

  return distancia;
}

void loop ()
{
  client.loop();

  char buffer[256];
  float Dados = ultrassom();
  float Dado = vazao();
  //char dado_string[8];
  //dtostrf(Dados, 5, 2, dado_string);


  StaticJsonDocument<96> doc;

  String Mac = String(WiFi.macAddress());

  
  doc["version"] = VERSION;
  doc["level"] = Dados;
  doc["flow"] = Dado;
  
  serializeJson(doc, buffer);
  //serializeJson(doc, Serial);
  Serial.println();



  client.publish(topic, buffer);
  client.subscribe(topic);
  delay(5000);
}
