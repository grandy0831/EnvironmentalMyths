#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Timezone.h>
#include <DHT.h>
#include <PubSubClient.h>

#include "password_management.h"  

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqtt_server = SECRET_MQTTSERVER;
const int mqtt_port = SECRET_MQTTPORT;
const char* mqtt_user = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;

#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};
Timezone UK(BST, GMT);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

WiFiClient espClient;
PubSubClient client(espClient);

const int sensorPin = A0;
float voltageConversionConstant = .004882814;
float voltageMin = .62;
float windSpeedMin = 0;
float voltageMax = 2.0;
float windSpeedMax = 32;
float sensorVoltage = 0;
float windSpeed = 0;

//Interval time control
unsigned long lastTime = 0;
unsigned long interval = 60000;

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  timeClient.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (millis() - lastTime > interval) {
    lastTime = millis();
    timeClient.update();
    printFormattedTime();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    int sensorValue = analogRead(sensorPin);
    sensorVoltage = sensorValue * voltageConversionConstant;
    if (sensorVoltage <= voltageMin) {
      windSpeed = windSpeedMin;
    } else {
      windSpeed = (sensorVoltage - voltageMin) * windSpeedMax / (voltageMax - voltageMin);
    }

    if (!isnan(h) && !isnan(t) && !isnan(windSpeed)) {
      char tempStr[50], humStr[50], windSpeedStr[50];
      snprintf(tempStr, sizeof(tempStr), "%.2f", t);
      snprintf(humStr, sizeof(humStr), "%.2f", h);
      snprintf(windSpeedStr, sizeof(windSpeedStr), "%.2f", windSpeed);

      // 打印所有数据到串口监视器
      Serial.print("Temperature: ");
      Serial.print(tempStr);
      Serial.print("°C, Humidity: ");
      Serial.print(humStr);
      Serial.print("%, Wind Speed: ");
      Serial.print(windSpeed);
      Serial.println(" m/s");

      client.publish("student/EM/zczqgch/temperature", tempStr);
      client.publish("student/EM/zczqgch/humidity", humStr);
      client.publish("student/EM/zczqgch/windspeed", windSpeedStr);
    } else {
      Serial.println("Failed to read from sensors!");
    }
  }
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void printFormattedTime() {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ptm = gmtime(&rawtime);
  char timeString[40];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", ptm);
  Serial.println(timeString);
}
