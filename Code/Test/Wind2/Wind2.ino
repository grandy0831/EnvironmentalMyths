unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 1000;  

int pinInterrupt = 2;  

int Count = 0;  

void onChange() {
  if (digitalRead(pinInterrupt) == LOW)
    Count++;
}

void setup() {
  Serial.begin(115200); 
  pinMode(pinInterrupt, INPUT_PULLUP); 
  
  attachInterrupt(digitalPinToInterrupt(pinInterrupt), onChange, FALLING);
}

void loop() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    
    float windSpeed = (Count * 8.75) / 100;
    
    Serial.print(windSpeed);
    Serial.println(" m/s");
    
    Count = 0;
  }
  delay(1); 
}
