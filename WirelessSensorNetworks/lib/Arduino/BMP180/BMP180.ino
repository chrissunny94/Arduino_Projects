#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#include "BMP085.h"


// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4

Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(9600);
  //Serial.println("DHTxx test!");
  dht.begin();
  if (!bmp.begin()) {
	//Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
}
  
void loop() {
  
   float h = dht.readHumidity();
   float t = dht.readTemperature();
   float f = dht.readTemperature(true);
   if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
    //Serial.print("\n(DHT)Humidity: ");
    Serial.print(h);
    Serial.print(":");    
    //Serial.print(" %\t");
    //Serial.print("\n(DHT)Temperature: ");
    Serial.print(t);
    Serial.print(":");
    //Serial.print(" *C ");
    //Serial.print(f);
    //Serial.print(":");
    //Serial.print("\n(BMP)Temperature: ");
    Serial.print(bmp.readTemperature());
    Serial.print(":");
    //Serial.println(" *C");
    //Serial.print("\n(BMP)Pressure: ");
    
    Serial.print(bmp.readPressure());
    //Serial.println(" Pa");
    //Serial.print("Altitude : ");
    Serial.print(":");
    Serial.print(bmp.readAltitude());
    //Serial.println(" meters");
    //Serial.print("Pressure at sealevel (calculated) : ");
    //Serial.print(":");
    //Serial.print(bmp.readSealevelPressure());
    //Serial.println(" Pa");
    //Serial.print("Real altitude : ");
    //Serial.print(":");
    //Serial.print(bmp.readAltitude(101500));
    //Serial.println(" meters:");
    Serial.print("\n");
    
    //val = Serial.read(); 
    
    delay(2000);
}
