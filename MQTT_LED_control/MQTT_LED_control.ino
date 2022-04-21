#include <WiFi.h>
#include <PubSubClient.h>
const int BUTTON_PIN = 15;           //button is connected to GPIO 33
const int led = 12;
const char* ssid = "NoInternet";
const char* password = "@1234567#A";
const char* mqtt_server = "broker.mqtt-dashboard.com";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
void setup_wifi() {
  delay(100);
  Serial.print("Connecting to ");       //connecting to a WIFI network
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0') {
    digitalWrite(led, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(led, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}
void reconnect() {
  while (!client.connected()) {       // Loop until we're reconnected
  Serial.print("Attempting MQTT connection...");
  String clientId = "ESP8266Client-";       // Create a random client ID
  clientId += String(random(0xffff), HEX);
  // Attempt to connect
  // if your MQTT broker has clientID.username and password
  //please change following line to   if(client.connect(clientID,userName,passWord))
  if (client.connect(clientId.c_str())) {
    Serial.println("connected");
    client.subscribe("YongJunCommand");  //once connected to MQTT broker, subcribe command if any
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);    // Wait 5 seconds before retrying
    }
  }
}
void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(BUTTON_PIN,INPUT);
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  int status;
  if (now - lastMsg > 1000) {         //send message every 5 second
    lastMsg = now;
    status=digitalRead(BUTTON_PIN);
    String msg="Button status:  ";
    if(status==LOW){
      msg=msg+"PUSH Button ON";
      char message[58];
      msg.toCharArray(message,58);
      Serial.println(message);
      client.publish("YongJunData",message);   //publish sensor data to MQTT broker
      digitalWrite(led, HIGH);
    } 
    else{
      msg=msg+"Push Button OFF";
      char message[58];
      msg.toCharArray(message,58);
      Serial.println(message);
      client.publish("YongJunData",message);    //publish sensor data to MQTT broker
      digitalWrite(led, LOW);
    }
  }
}
