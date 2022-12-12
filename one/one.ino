#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 13
#define HIGHPIN A5
#define LIGHTPIN A2
#define PHPIN A7

int samples = 10;
float adc_resolution = 1024.0;

int portNum = 2;

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

signed long long int timer = -2500;

float checkTemp();
float checkHigh();
float checkLight();
float checkPH();
void checkSensor();

void setup() {
  Serial.begin(9600);

  dht.begin();  // 온도체크 시작
}

void loop() {
  checkSensor();
}

void checkSensor() {

  if (millis() >= timer + 2500) {
    timer = millis();

    DynamicJsonDocument doc(256);

    doc["temp"] = checkTemp();
    doc["level"] = checkHigh();
    doc["light"] = checkLight();
    doc["ph"] = checkPH();
    doc["portNum"] = portNum;

    serializeJson(doc, Serial);

    Serial.println();

    portNum == 2 ? portNum = 3 : portNum = 2;
  }
}

float checkTemp() {

  return dht.readTemperature();
}

float checkHigh() {

  float result = analogRead(HIGHPIN);

  return result;
}

float checkLight() {

  float result = analogRead(A2);

  return result;
}

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