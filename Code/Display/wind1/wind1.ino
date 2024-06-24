#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

const char* ssid = "4958 Hyperoptic Fibre Broadband";
const char* password = "gPz3kA9JShLJ";
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;
const char* mqtt_user = "student";
const char* mqtt_password = "ce2021-mqtt-forget-whale";

WiFiClient espClient;
PubSubClient client(espClient);
Servo angleServo;  
Servo servoOne;    
Servo servoTwo;    
Servo servoThree;  

void setup_wifi() {
    Serial.begin(115200);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    char message[length + 1];
    for (int i = 0; i < length; i++) {
        message[i] = (char)payload[i];
    }
    message[length] = '\0';  // Null-terminate the string

    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(message);

    if (strcmp(topic, "student/EM1/zczqgch/windspeed") == 0) {
        int windspeed = atoi(message);
        if (windspeed < 3) {
            angleServo.write(0);  
            servoOne.write(0);  
            servoTwo.writeMicroseconds(1500); 
            servoThree.writeMicroseconds(1500); 
        } else if (windspeed >= 3 && windspeed <= 6) {
            angleServo.write(80);  
            servoOne.writeMicroseconds(1500); 
            servoTwo.write(0);  
            servoThree.writeMicroseconds(1500); 
        } else if (windspeed > 6) {
            angleServo.write(160);  
            servoOne.writeMicroseconds(1500); 
            servoTwo.writeMicroseconds(1500); 
            servoThree.write(0);  
        }
    }
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "arduinoClient-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
            Serial.println("connected");
            client.subscribe("student/EM1/zczqgch/windspeed");
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
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    angleServo.attach(9);  
    servoOne.attach(10);   
    servoTwo.attach(11);   
    servoThree.attach(12); 
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
