#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 13
#define HIGHPIN A5
#define LIGHTPIN A2
#define PHPIN A7
#define DHTTYPE DHT11

// for PH
int samples = 10;
float adc_resolution = 1024.0;

// for portNum in Query
int portNum = 2;

// for temp
DHT dht(DHTPIN, DHTTYPE);

// for timer(interval)
signed long long int timer = -2500;

float checkTemp();
float checkHigh();
float checkLight();
float checkPH();
void checkSensor();

void setup() {
  
  // arduino Serial init
  Serial.begin(9600);

  // dht init
  dht.begin();
}

void loop() {
  

  // check & send JSON sensor value
  checkSensor();
}

void checkSensor() {

  if (millis() >= timer + 2500) {
    timer = millis();


    // make buffer
    DynamicJsonDocument doc(256);

    // send JSON to JAVA
    doc["temp"] = checkTemp();
    doc["level"] = checkHigh();
    doc["light"] = checkLight();
    doc["ph"] = checkPH();
    doc["portNum"] = portNum;
    serializeJson(doc, Serial);

    // for JAVA bufferedReader.readline()
    Serial.println();


    // mix portNum
    portNum == 2 ? portNum = 3 : portNum = 2;
  }
}

// temp
float checkTemp() {

  return dht.readTemperature();
}

// water_depth
float checkHigh() {

  float result = analogRead(HIGHPIN);

  return result;
}

// light
float checkLight() {

  float result = analogRead(A2);

  return result;
}

// water_PH
float checkPH() {
  int mesurings = 0;
  int i = 0;
  float voltage = 5;
  for (i = 0; i < samples; i++) {
    mesurings += analogRead(PHPIN);
    delay(1);
  }

  float result = voltage / adc_resolution * mesurings / samples;

  return result;
}