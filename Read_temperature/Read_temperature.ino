#include "DHT.h"
#define DHT11PIN 7          // define the digital I/O pin
#define DHT11TYPE DHT11     // DHT 11 
DHT dht11(DHT11PIN, DHT11TYPE);

void setup() {
  Serial.begin(9600);
  dht11.begin();
}

void loop() {
  float h11 = dht11.readHumidity();
  float t11 = dht11.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(t11);
  Serial.println(" Celsius");
  
  Serial.print("Humidity = ");
  Serial.print(h11);
  Serial.println(" %");
  delay(1000);
}
