#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
// You should get Auth Token in the Blynk App.
char auth[] = "M3vraVDLDbiZegHNFSvOhsZJ6RoElIAh";
char ssid[] = "NoInternet";
char pass[] = "@1234567#A";
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)){
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5,t);
  Blynk.virtualWrite(V6,h);  
}
void setup()
{
 // Debug console
 Serial.begin(115200); 
 Blynk.begin(auth, ssid, pass);
 dht.begin();
 timer.setInterval(1000L,sendSensor);
}
void loop()
{
 Blynk.run();
 timer.run();
}
