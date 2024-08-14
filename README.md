# Environmental Myths: The Fusion of Ancient Mythology and Modern Technology

## Project Overview
The Environmental Myths project brings together ancient Chinese myths and modern technology to demonstrate how ancient people interpreted natural phenomena. Utilising sensors to monitor temperature, humidity, and wind, it transforms this environmental data into dynamic displays through artistic installations featuring mechanical structures. This innovative approach not only educates the public about traditional culture and environmental changes but also revitalises ancient stories, fostering an appreciation for cultural heritage and environmental science.

## Research Questions
**Main Question:**
- How can traditional Chinese myths be integrated with modern environmental sensor technology to visualise changes in the environment?

**Sub-Questions:**
- How can a reliable outdoor environmental monitoring device be designed and constructed for long-term operation?
- How can artistic installations be designed to display environmental data captured by sensors?

## Technical Details

### Personal Weather Station Design
- **Sensor Selection**
  - DHT22 for humidity and temperature measurement.
  - Pulse Output Wind Speed Sensor for wind speed detection.
  
- **Hardware Assembly**
  - Powered by a 3.7V 1000mAh rechargeable lithium polymer battery.
  - Supported by a 1W 80*100 solar panel.
  - Managed by the DFRobot Solar Power Manager 5V.
  - Controlled by Arduino NANO ESP32 Nano for efficient data processing.

- **Enclosure**
  - Stevenson screen model 3D-printed in PETG material, which is noted for its high strength and impact resistance, ensuring durability in outdoor environments.

- **Code Implementation**
  - Incorporates Wi-Fi connectivity and an MQTT client for seamless data transmission.
  - Utilises ESP32 deep sleep mode to significantly reduce power consumption during idle times, ensuring long-term deployment without frequent maintenance.

### Artistic Installation Device Design

- **Hardware Assembly**
  - **Controller**: Utilises Arduino GIGA R1 WI-FI for robust control, featuring an extensive number of GPIOs and dedicated connectors.
  - **Servo Motors**: Three continuous rotation servos to control the movements of characters on the stage. One 25kg torque servo for angular control, tasked with switching between different scenes.
  - **Display**: Integrates a GIGA Display Shield to introduce and explain each scene.
  - **Presence Detection**: Employs a PIR sensor to detect the presence of viewers, activating the scene's movements when someone is nearby.

- **Code Implementation**
  - **Graphics and Video**: Uses ArduinoGraphics and Arduino_H7_Video.h libraries to display images.
  - **Connectivity**: Connects to a MQTT server via WI-FI to receive data from the weather station.
  - **Actuation**: Controls the servos based on received environmental data to switch scenes dynamically as environmental conditions change.

- **Enclosure Design**
  - **Stage Design**: Entirely original design created in FUSION360 for a rotatable stage that transitions between three scenes.
  - **Manufacturing**: Constructed using 3D printing, assembling the multi-scene stage prototype for interactive display.


