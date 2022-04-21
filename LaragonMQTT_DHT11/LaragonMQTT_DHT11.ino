#include <WiFi.h>
#include <SimpleDHT.h>
#include <MQTT.h>

//DHT pin
int pinDHT11 = 13;
SimpleDHT11 dht11;
const int LED1 = 12;
const int LED2 = 32;
const int LED3 = 27;
//Replace xxxxxx with your SSID and Password
const char ssid[] = "AVINA$H";
const char pass[] = "avinashbala13";

WiFiClient net;
MQTTClient mqtt;

//initial last milis zero
unsigned long lastMillis = 0;

//setting up the wifi
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() !=WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!mqtt.connect("nodemcuESP32")) {
  delay(1000);
  }

  Serial.println("\nconnected!");

//Setting up the mqqt

  mqtt.subscribe("control/led1");
  mqtt.subscribe("control/led2");
  mqtt.subscribe("control/led3");
}


void messageReceived(String &topic, String &payload) {

  //incoming: control/led1 - 1 :0

  Serial.println("incoming: " + topic + " - " + payload);

  if(topic=="control/led1" && payload. toInt() ==1) {
    digitalWrite (LED1,HIGH);
  }
  else if(topic=="control/led1" && payload. toInt() ==0) {
    digitalWrite (LED1,LOW);
  }
  if(topic=="control/led2" && payload. toInt() ==1) {
    digitalWrite (LED2,HIGH);
  }
  else if(topic=="control/led2" && payload. toInt() ==0) {
    digitalWrite (LED2,LOW);
  }
  if(topic=="control/led3" && payload. toInt() ==1) {
    digitalWrite (LED3,HIGH);
  }
  else if(topic=="control/led3" && payload. toInt() ==0) {
    digitalWrite (LED3,LOW);
  }

}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,pass);

  pinMode (LED1, OUTPUT);  //led1
  pinMode (LED2, OUTPUT);  //led2
  pinMode (LED3, OUTPUT);  //led3

  mqtt.begin("broker.mqttdashboard.com", net);
  mqtt.onMessage(messageReceived);

  connect();
}

void loop() {
  mqtt.loop();
  delay(10);

  if (!mqtt.connected()) {
    connect();
  }

  byte temperature = 0;
  byte humidity = 0;

  dht11.read (pinDHT11, &temperature, &humidity, NULL);
  delay(1500);

  //publish a message roughly every second

  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    mqtt.publish("monitoring/temperature", String(temperature));
    mqtt.publish("monitoring/humidity", String(humidity)); 
  }
}
