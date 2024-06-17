#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Timezone.h>
#include <DHT.h>
#include <PubSubClient.h>

#include "password_management.h"

const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqtt_server = SECRET_MQTTSERVER;
const int mqtt_port = SECRET_MQTTPORT;
const char* mqtt_user = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;

#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define WIND_SPEED_PIN 2
volatile int windCount = 0;
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 1000;

TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};
Timezone UK(BST, GMT);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastTime = 0;
unsigned long interval = 10000; // Set the interval time

void IRAM_ATTR detectWind() {
  windCount++;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  timeClient.begin();
  
  pinMode(WIND_SPEED_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WIND_SPEED_PIN), detectWind, FALLING);
}

float calculateWindSpeed() {
    float pulsesPerSecond = windCount / (interval / 1000.0); 
    float windSpeed = pulsesPerSecond * 0.0875; 
    windCount = 0; 
    return windSpeed;
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
    float windSpeed = calculateWindSpeed(); 

    char temp[50], hum[50], windStr[50];
    snprintf(temp, sizeof(temp), "%.2f", t);
    snprintf(hum, sizeof(hum), "%.2f", h);
    snprintf(windStr, sizeof(windStr), "%.2f", windSpeed);

    if (!isnan(h) && !isnan(t)) {
      Serial.printf("Temperature: %s°C, Humidity: %s%%, Wind Speed: %s m/s\n", temp, hum, windStr);
      client.publish("student/EM1/zczqgch/temperature", temp);
      client.publish("student/EM1/zczqgch/humidity", hum);
      client.publish("student/EM1/zczqgch/windspeed", windStr);
    } else {
      Serial.println("Failed to read from DHT sensor!");
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
    char clientId[25];
    snprintf(clientId, sizeof(clientId), "arduinoClient-%lu", random(0, 10000));
    if (client.connect(clientId, mqtt_user, mqtt_password)) {
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
  time_t utc = timeClient.getEpochTime();
  time_t local = UK.toLocal(utc);  

  struct tm * ptm = localtime(&local);
  char timeString[40];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", ptm);
  Serial.print(timeString);
  Serial.print(" - ");
}
