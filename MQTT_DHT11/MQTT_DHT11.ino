#include <DHT.h>                                         
#define DHTTYPE DHT11
#define DHTPIN  13
#include <WiFi.h>
#include <PubSubClient.h>                               

DHT dht(DHTPIN, DHTTYPE, 13);
const char* ssid = "NoInternet";
const char* password = "@1234567#A";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;                                    
PubSubClient client(espClient);                          
long lastMsg = 0;                                       
char detail[100];                                    
int value = 0;
float humidity, temp_c;                        

void sendSensor() {
  humidity = dht.readHumidity();  
  temp_c = dht.readTemperature();     
  snprintf(detail, 100, "Temperature is %3.2f C and Humidity is %3.2f %%", temp_c, humidity);
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temp_c)) {
    Serial.println("Failed to read from DHT sensor!");
    client.publish("MESSAGE_OUT", "Failed to read from DHT sensor!");
  }
  else {
  client.publish("MESSAGE_OUT", detail);          //Publish to MQTT client
  Serial.println(detail);
  }
  return;
}

void setup() {
  Serial.begin(115200);                   
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }                
  Serial.println("WiFi connected");       
  client.setServer(mqtt_server, 1883);   
  client.setCallback(callback);          
  dht.begin();
}

void callback(char* topic, byte* message_i_o, unsigned int length) {    
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message_i_o[i]);
  }
  Serial.println();
}

void reconnect() {                                                
  while (!client.connected()) {       // Loop until we're reconnected
  Serial.print("Attempting MQTT connection...");
  String clientId = "ESP32Client-";       // Create a random client ID
  clientId += String(random(0xffff), HEX);
  // Attempt to connect
  // if your MQTT broker has clientID.username and password
  //please change following line to   if(client.connect(clientID,userName,passWord))
  if (client.connect(clientId.c_str())) {
    Serial.println("connected");                                                     
    } else {
      Serial.print("failed, rc=");
      Serial.println(" Trying again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();                                              
  if (now - lastMsg > 5000)  {                                       
    lastMsg = now;
    ++value;
    sendSensor();
  }

}
