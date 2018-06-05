#define _DEBUG_
#define _DISABLE_TLS_

#define BUTTON_PIN 0
#define RELAY_PIN 12
#define LED_PIN 13

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ThingerWifi.h>

#include <Adafruit_Sensor.h>

#include "DHT.h",
#define DHTPIN 14    // modify to the pin we connected
#define DHTTYPE DHT21   // AM2301 

DHT dht(DHTPIN, DHTTYPE);

ThingerWifi thing("kasvuhoone", "eero", "BIKk1ZBIWdTM");

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  thing["led"] << digitalPin(LED_PIN);
  thing["relay"] << digitalPin(RELAY_PIN);

  thing["measures"] >> [](pson &out){ 
    out["temperature"] = dht.readTemperature();
    out["humidity"] = dht.readHumidity();
  };

  thing.add_wifi("Thomson08D374", "katieero123");
}

void loop() {
  thing.handle();
}
