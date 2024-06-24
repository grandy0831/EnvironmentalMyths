const int sensorPin = A0;  //Defines the pin that the anemometer output is connected to
int sensorValue = 0;       //Variable stores the value direct from the analog pin
float sensorVoltage = 0;   //Variable that stores the voltage (in Volts) from the anemometer being sent to the analog pin
float windSpeed = 0;       // Wind speed in meters per second (m/s)

const float voltageConversionConstant = 3.3 / 1023.0; //This constant maps the value provided from the analog read function, which ranges from 0 to 1023, to actual voltage, which ranges from 0V to 5V
int sensorDelay = 1000;                        //Delay between sensor readings, measured in milliseconds (ms)

//Anemometer Technical Variables
//The following variables correspond to the anemometer sold by Adafruit, but could be modified to fit other anemometers.

float voltageMin = 1.42;   // Mininum output voltage from anemometer in mV.
float windSpeedMin = 0;  // Wind speed in meters/sec corresponding to minimum voltage

float voltageMax = 3.0;   // Maximum output voltage from anemometer in mV.
float windSpeedMax = 32;  // Wind speed in meters/sec corresponding to maximum voltage



void setup() {
  Serial.begin(9600);  //Start the serial connection
}


void loop() {
  sensorValue = analogRead(sensorPin);  // Get a value between 0 and 1023 from the analog pin connected to the anemometer
  sensorVoltage = sensorValue * voltageConversionConstant;  // Convert sensor value to actual voltage

  // Convert voltage value to wind speed using the range of max and min voltages and wind speeds for the anemometer
  if (sensorVoltage <= voltageMin) {
    windSpeed = 0;  // If voltage is below minimum value, set wind speed to zero.
  } else {
    windSpeed = (sensorVoltage - voltageMin) * windSpeedMax / (voltageMax - voltageMin);  // For voltages above minimum value, use the linear relationship to calculate wind speed.
  }

  // Print voltage and wind speed to serial
  Serial.print("Voltage: ");
  Serial.print(sensorVoltage);
  Serial.print("\tWind speed: ");
  Serial.println(windSpeed);

  delay(sensorDelay);
}
