  
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define DHTPIN D4
#define DHTTYPE DHT11

#include <DHT.h>
DHT dht(DHTPIN, DHTTYPE);
 
#define FIREBASE_HOST "mydht-sensor.firebaseio.com"
#define FIREBASE_AUTH "KYCRZ7s0kXDuGG4NL4HcJzcxnT00r0Kw6BiySkEB"
#define WIFI_SSID "U+NetDC9B"
#define WIFI_PASSWORD "0110028615"
 
void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  dht.begin(9600);
  
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
   Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
   
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.print("Humidity: ");  
  Serial.print(h);
  String fireHumid = String(h) + String("%");                   //Humidity integer to string conversion
  
  Serial.print("%  Temperature: ");  
  Serial.print(t);  
  Serial.println("°C ");
  String fireTemp = String(t) + String("°C");                  //Temperature integer to string conversion
  delay(5000);   

  Firebase.pushString("/DHT11/Humidity", fireHumid);            //setup path to send Humidity readings
  Firebase.pushString("/DHT11/Temperature", fireTemp);      

  
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
}
