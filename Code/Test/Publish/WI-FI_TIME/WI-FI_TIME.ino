#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Timezone.h>
#include <DHT.h>

// WiFi credentials
const char* ssid     = "***";
const char* password = "***";

// Timezone setup for UK (UTC + 0 hours)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};  // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};   // Standard Time
Timezone UK(BST, GMT);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);  // 0 offset for UTC

// DHT Sensor setup
#define DHTPIN 5  // What digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
}

void loop() {
  timeClient.update();
  time_t utc = timeClient.getEpochTime();
  time_t local = UK.toLocal(utc);

  Serial.print("Local date/time: ");
  Serial.println(ctime(&local));

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read humidity (percentage)
  float h = dht.readHumidity();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C ");

  delay(10000);  // Update every 10 seconds
}

