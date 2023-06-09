#include <Wire.h>
#include "Adafruit_SGP30.h"
#include <AM2320_asukiaaa.h>

#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

#define SECRET_SSID "suhsd-device"
#define SECRET_PASS "suhsd-D3v1c3"

Adafruit_SGP30 sgp;
AM2320_asukiaaa mySensor;

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);#include <Wire.h>
#include "Adafruit_SGP30.h"
#include <AM2320_asukiaaa.h>

#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

#define SECRET_SSID "suhsd-device"
#define SECRET_PASS "suhsd-D3v1c3"

Adafruit_SGP30 sgp;
AM2320_asukiaaa mySensor;

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
// const char broker[] = "broker.hivemq.com";
int        port     = 1883;
const char topic[]  = "tide-lm-data-send";

const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

uint32_t getAbsoluteHumidity(float temperature, float humidity) {
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
  return absoluteHumidityScaled;
}

void reset_temp() {
  Serial.println("resetting temp humid sensor");
  AM2320_asukiaaa mySensor;
}

void connect_Wifi() {
Serial.print("Attempting to connect to WPA SSID: ");
Serial.println(ssid);
Serial.print("Disconnecting -- ");
WiFi.disconnect();
Serial.println("Reconnecting");
while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
  // failed, retry
  Serial.print(".");
  delay(5000);
}

Serial.println("You're connected to the network");
Serial.println();
}
int retries = 0;
void connect_MQTT() {
// connect_Wifi();

Serial.print("Attempting to connect to the MQTT broker: ");
Serial.println(broker);

if(mqttClient.connect(broker, port)){
  Serial.println("Connected to MQTT Broker");
}
else{
    Serial.print("Connection to MQTT Broker failed...");
    Serial.println(mqttClient.connectError());
    if (retries > 5){
      retries = 0;
      connect_Wifi();
    }
    retries++;
    Serial.println("retrying MQTT connection number " + String(retries));
}

}

void setup() {
Serial.begin(9600);
while (!Serial) { delay(10); } // Wait for serial console to open!

Serial.println("SGP30 test");

if (! sgp.begin()){
  Serial.println("Sensor not found :(");
  delay(3000);
  setup();
}
Serial.print("Found SGP30 serial #");
Serial.print(sgp.serialnumber[0], HEX);
Serial.print(sgp.serialnumber[1], HEX);
Serial.println(sgp.serialnumber[2], HEX);

Wire.begin();
mySensor.setWire(&Wire);

// connect_MQTT();
connect_Wifi();
}

int counter = 0;

void loop() {
  connect_MQTT();

  mqttClient.poll();
  unsigned long currentMillis = millis();

  if (! sgp.IAQmeasure()) {
    // Serial.println("Measurement failed");
    return;
  }
  // Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  // Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  if (! sgp.IAQmeasureRaw()) {
    // Serial.println("Raw Measurement failed");
    return;
  }
  // Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
  // Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");

 if (mySensor.update() != 0) {
  // Serial.println("Error: Cannot update sensor values.");
  reset_temp();
} else {
  // Serial.println("temperatureC: " + String(mySensor.temperatureC) + " C");
  // Serial.println("temperatureF: " + String(mySensor.temperatureF) + " F");
  // Serial.println("humidity: " + String(mySensor.humidity) + " %");
}

counter++;
if (counter == 30) {
  counter = 0;

  uint16_t TVOC_base, eCO2_base;
  if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
    // Serial.println("Failed to get baseline readings");
    return;
  }
  // Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
  // Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
}

 int temp = mySensor.temperatureC;
 int humid = mySensor.humidity;
 int ec02 = sgp.eCO2;
 int tvoc = sgp.TVOC;
 int h2 = sgp.rawH2;
 int ethanol = sgp.rawEthanol;
//  Serial.println("sending mqtt message to " + String(topic));
 mqttClient.beginMessage(topic);
 mqttClient.print("LM-TIDE:group3:" + String(temp) + ":" + String(humid) + ":" + String(ec02) + ":" + String(tvoc) + ":" + String(h2) + ":" + String(ethanol));
 mqttClient.endMessage();
 Serial.println("LM-TIDE:group3:" + String(temp) + ":" + String(humid) + ":" + String(ec02) + ":" + String(tvoc) + ":" + String(h2) + ":" + String(ethanol));


delay(1000 * 10);
 
}

const char broker[] = "test.mosquitto.org";
// const char broker[] = "broker.hivemq.com";
int        port     = 1883;
const char topic[]  = "tide-lm-data-send";

const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

uint32_t getAbsoluteHumidity(float temperature, float humidity) {
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
  return absoluteHumidityScaled;
}

void reset_temp() {
  Serial.println("resetting temp humid sensor");
  AM2320_asukiaaa mySensor;
}

void connect_Wifi() {
Serial.print("Attempting to connect to WPA SSID: ");
Serial.println(ssid);
Serial.print("Disconnecting -- ");
WiFi.disconnect();
Serial.println("Reconnecting");
while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
  // failed, retry
  Serial.print(".");
  delay(5000);
}

Serial.println("You're connected to the network");
Serial.println();
}
int retries = 0;
void connect_MQTT() {
// connect_Wifi();

Serial.print("Attempting to connect to the MQTT broker: ");
Serial.println(broker);

if(mqttClient.connect(broker, port)){
  Serial.println("Connected to MQTT Broker");
}
else{
    Serial.print("Connection to MQTT Broker failed...");
    Serial.println(mqttClient.connectError());
    if (retries > 5){
      retries = 0;
      connect_Wifi();
    }
    retries++;
    Serial.println("retrying MQTT connection number " + String(retries));
}

}

void setup() {
Serial.begin(9600);
while (!Serial) { delay(10); } // Wait for serial console to open!

Serial.println("SGP30 test");

if (! sgp.begin()){
  Serial.println("Sensor not found :(");
  delay(3000);
  setup();
}
Serial.print("Found SGP30 serial #");
Serial.print(sgp.serialnumber[0], HEX);
Serial.print(sgp.serialnumber[1], HEX);
Serial.println(sgp.serialnumber[2], HEX);

Wire.begin();
mySensor.setWire(&Wire);

// connect_MQTT();
// connect_Wifi();
}

int counter = 0;

void loop() {
  // connect_MQTT();

  // mqttClient.poll();
  unsigned long currentMillis = millis();

  if (! sgp.IAQmeasure()) {
    // Serial.println("Measurement failed");
    return;
  }
  // Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  // Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  if (! sgp.IAQmeasureRaw()) {
    // Serial.println("Raw Measurement failed");
    return;
  }
  // Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
  // Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");

 if (mySensor.update() != 0) {
  // Serial.println("Error: Cannot update sensor values.");
  reset_temp();
} else {
  // Serial.println("temperatureC: " + String(mySensor.temperatureC) + " C");
  // Serial.println("temperatureF: " + String(mySensor.temperatureF) + " F");
  // Serial.println("humidity: " + String(mySensor.humidity) + " %");
}

counter++;
if (counter == 30) {
  counter = 0;

  uint16_t TVOC_base, eCO2_base;
  if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
    // Serial.println("Failed to get baseline readings");
    return;
  }
  // Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
  // Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
}

 int temp = mySensor.temperatureC;
 int humid = mySensor.humidity;
 int ec02 = sgp.eCO2;
 int tvoc = sgp.TVOC;
 int h2 = sgp.rawH2;
 int ethanol = sgp.rawEthanol;
//  Serial.println("sending mqtt message to " + String(topic));
//  mqttClient.beginMessage(topic);
//  mqttClient.print("LM-TIDE:group3:" + String(temp) + ":" + String(humid) + ":" + String(ec02) + ":" + String(tvoc) + ":" + String(h2) + ":" + String(ethanol));
//  mqttClient.endMessage();
 Serial.println("LM-TIDE:group1:" + String(temp) + ":" + String(humid) + ":" + String(ec02) + ":" + String(tvoc) + ":" + String(h2) + ":" + String(ethanol));


delay(1000 * 10);
 
}