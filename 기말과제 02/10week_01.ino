#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define DHTPIN D4
#define DHTTYPE DHT11
#include <DHT.h>

DHT dht(DHTPIN, DHTTYPE);
 
#define FIREBASE_HOST "database-list-74bf2.firebaseio.com"
#define FIREBASE_AUTH "D3bWG37gXZnN0ycCunzBSbC2lXIvbI9PAcn34UTI"
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
  Serial.println(WiFi.localIP());
   
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop() {
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  Serial.print("Temperature = ");
  Serial.println(temp);
  Serial.print("Humidity = ");
  Serial.println(humi);      

  StaticJsonBuffer<200> jsonbuffer;
  JsonObject& root = jsonbuffer.createObject();
  root["temperature"] = temp;
  root["humidity"] = humi;

  String name = Firebase.push("logDHT", root); 
  
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.println("sent data to firebase");
 // Serial.println(name);
  
  delay(5000);
}
