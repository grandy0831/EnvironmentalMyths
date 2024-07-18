#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// Includes secret passwords and configurations
#include "password_management.h"

// Defines network and MQTT broker details, sourced from configuration files
const char* ssid     = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqtt_server = SECRET_MQTTSERVER;
const int mqtt_port = SECRET_MQTTPORT;
const char* mqtt_user = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;

WiFiClient espClient;
PubSubClient client(espClient);
Servo angleServo;  // Servo for angle control
Servo servoOne;    
Servo servoTwo;    
Servo servoThree;  
int pirPin = 2; // Pin for PIR sensor
int switchPin = 3; // Pin for switch
bool pirState = false; // Current state of the PIR sensor
unsigned long lastMsgTime = 0;  // Tracks the time of the last message request
long interval = 60000;          // Interval for message requests (60 seconds)
int lastTemperature = 0;  // Stores the last temperature value
int delayTime = 60;     // Delay time to control servo movement speed

void setup_wifi() {
    Serial.begin(115200);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("."); // Prints a dot during the connection attempt
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1]; // Creates a string to store the message content
    for (int i = 0; i < length; i++) {
        message[i] = (char)payload[i]; // Converts payload to string
    }
    message[length] = '\0'; // Ensures the string is null-terminated

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(message);

    // If the message is related to temperature
    if (strcmp(topic, "student/EM/zczqgch/temperature") == 0) {
        lastTemperature = atoi(message); // Converts the message to an integer
        int targetAngle = lastTemperature < 20 ? 0 : (lastTemperature >= 20 && lastTemperature <= 30 ? 80 : 160);
        smoothMoveServo(angleServo, targetAngle); // Smoothly moves the servo to the target angle
    }
}

// A function to smoothly move a servo to a specified angle
void smoothMoveServo(Servo servo, int targetAngle) {
    int currentAngle = servo.read(); // Reads the current angle
    int step = (currentAngle < targetAngle) ? 1 : -1; // Determines the direction of rotation
    while (currentAngle != targetAngle) {
        currentAngle += step;
        servo.write(currentAngle); // Gradually changes the angle
        delay(delayTime); // Delay to control the speed of the movement
    }
}

void reconnect() {
    // Attempts to reconnect if not connected to the MQTT server
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "arduinoClient-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
            Serial.println("connected");
            client.subscribe("student/EM/zczqgch/temperature", 1); // Subscribes to the temperature topic
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup() {
    setup_wifi();
    pinMode(pirPin, INPUT); // Sets the PIR sensor pin as input
    pinMode(switchPin, INPUT_PULLUP); // Sets the switch pin as input with pull-up
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback); // Sets the MQTT message reception callback function
    angleServo.attach(9);  
    servoOne.attach(10);   
    servoTwo.attach(11);   
    servoThree.attach(12);
    reconnect(); // Connects to the MQTT server
}

unsigned long pirLastTriggered = 0; // Time when the PIR sensor was last triggered
const long pirHoldTime = 10000; // Duration for which the PIR sensor activation is considered

void loop() {
    if (digitalRead(switchPin) == LOW) { // If the switch is pressed
        if (!client.connected()) {
            reconnect(); // Ensures a continuous connection to the MQTT server
        }
        client.loop();

        bool currentPirState = digitalRead(pirPin);
        if (currentPirState) {
            pirLastTriggered = millis(); // Updates the time the PIR sensor was triggered
        }

        if (millis() - pirLastTriggered < pirHoldTime) {
            // Controls other servos based on temperature
            servoOne.writeMicroseconds(lastTemperature < 20 ? 1000 : 1500);
            servoTwo.writeMicroseconds(lastTemperature >= 20 && lastTemperature <= 30 ? 1000 : 1500);
            servoThree.writeMicroseconds(lastTemperature > 30 ? 1000 : 1500);
        } else {
            // If the PIR sensor is not retriggered within the specified time, reset servos to neutral position
            servoOne.writeMicroseconds(1500);
            servoTwo.writeMicroseconds(1500);
            servoThree.writeMicroseconds(1500);
        }
    } else {
        // Set all servos to neutral position if the switch is not pressed
        servoOne.writeMicroseconds(1500);
        servoTwo.writeMicroseconds(1500);
        servoThree.writeMicroseconds(1500);
    }
}
