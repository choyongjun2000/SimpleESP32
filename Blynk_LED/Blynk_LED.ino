#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
// You should get Auth Token in the Blynk App.
char auth[] = "YuyiVKt1jdqcLzt_9IcdJ8xPtBYMs2bs";
char ssid[] = "NoInternet";
char pass[] = "@1234567#A";
const int LED= 12;
void setup()
{
 // Debug console
 Serial.begin(115200); 
 Blynk.begin(auth, ssid, pass);
 pinMode(LED, OUTPUT); //pinMode(D6, OUTPUT); to add buzzer SWITCH 5 ST3
}
BLYNK_WRITE (V3) 
{
 int pinValue = param.asInt();
 if (pinValue ==1)
 {
 digitalWrite (LED,HIGH); //digitalWrite (D6,HIGH); to add buzzer 
 }
 else
 {
 digitalWrite (LED,LOW); //digitalWrite (D6,LOW); to add buzzer
 }
 }
void loop()
{
 Blynk.run();
}
