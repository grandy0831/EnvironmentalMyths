// Include necessary libraries for WiFi, time, sensor data handling, and MQTT communication
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Timezone.h>
#include <DHT.h>
#include <PubSubClient.h>

// Include secret passwords and configurations
#include "password_management.h"

// Define network and MQTT broker details, pulled from password_management.h
const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqtt_server = SECRET_MQTTSERVER;
const int mqtt_port = SECRET_MQTTPORT;
const char* mqtt_user = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;

// DHT sensor setup
#define DHTPIN 5 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT22 type sensor
DHT dht(DHTPIN, DHTTYPE);

// Wind speed measurement setup
#define WIND_SPEED_PIN 2 // Digital pin for wind speed sensor
volatile int windCount = 0; // Counts the wind sensor signal interruptions
unsigned long lastDebounceTime = 0; // Timestamp of last signal debounce
unsigned long debounceDelay = 1000; // Debounce delay in milliseconds

// Time zone configuration for UK
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60}; // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0}; // Greenwich Mean Time
Timezone UK(BST, GMT);

// NTP Client for time synchronization
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

// MQTT client setup
WiFiClient espClient;
PubSubClient client(espClient);

// Time tracking for sensor data publication
unsigned long lastTime = 0;
unsigned long interval = 30000; // Set the interval time to 30 seconds

// Interrupt service routine for detecting wind speed
void IRAM_ATTR detectWind() {
  windCount++; // Increment wind count on each interrupt
}

// Initial setup function
void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud rate
  dht.begin(); // Initialize DHT sensor
  setup_wifi(); // Connect to WiFi
  client.setServer(mqtt_server, mqtt_port); // Configure MQTT server
  timeClient.begin(); // Start NTP client
  
  pinMode(WIND_SPEED_PIN, INPUT_PULLUP); // Setup wind speed pin
  attachInterrupt(digitalPinToInterrupt(WIND_SPEED_PIN), detectWind, FALLING); // Attach interrupt

  // Enable deep sleep with a wakeup time of 60 seconds
  esp_sleep_enable_timer_wakeup(60 * 1000000);
}

// Calculate wind speed based on sensor interrupts
float calculateWindSpeed() {
    float pulsesPerSecond = windCount / (interval / 1000.0);
    float windSpeed = pulsesPerSecond * 0.0875;
    windCount = 0;
    return windSpeed;
}

// Main loop function
void loop() {
  if (!client.connected()) {
    reconnect(); // Reconnect to MQTT if disconnected
  }
  client.loop(); // Maintain MQTT connection

  if (millis() - lastTime > interval) {
    lastTime = millis(); // Update last time
    timeClient.update(); // Update NTP client time
    printFormattedTime(); // Print formatted time

    float t = dht.readTemperature(); // Read temperature from DHT sensor
    float h = dht.readHumidity(); // Read humidity from DHT sensor
    float windSpeed = calculateWindSpeed(); // Calculate wind speed

    // Prepare payload for MQTT
    char temp[50], hum[50], windStr[50];
    snprintf(temp, sizeof(temp), "%.2f", t);
    snprintf(hum, sizeof(hum), "%.2f", h);
    snprintf(windStr, sizeof(windStr), "%.2f", windSpeed);

    // Check sensor readings for validity before publishing
    if (!isnan(h) && !isnan(t)) {
      Serial.printf("Temperature: %s°C, Humidity: %s%%, Wind Speed: %s m/s\n", temp, hum, windStr);
      client.publish("student/EM1/zczqgch/temperature", temp, true);
      client.publish("student/EM1/zczqgch/humidity", hum, true);
      client.publish("student/EM1/zczqgch/windspeed", windStr, true);
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }

    // Enter deep sleep mode after publishing
    delay(2000); // Delay to ensure message is sent before sleeping
    Serial.println("Entering deep sleep mode now");
    esp_deep_sleep_start();
  }
}

// Setup WiFi connection
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

// Attempt to reconnect to MQTT server
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

// Print the formatted current time
void printFormattedTime() {
  time_t utc = timeClient.getEpochTime();
  time_t local = UK.toLocal(utc);  

  struct tm * ptm = localtime(&local);
  char timeString[40];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", ptm);
  Serial.print(timeString);
  Serial.print(" - ");
}
