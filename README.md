# Environmental Myths: The Fusion of Ancient Mythology and Modern Technology
![IMG_5311 2](https://github.com/user-attachments/assets/1977df56-4330-4e82-9a12-923fb8fb6037)

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

## Installation Guide

### Hardware Installation Steps

#### Environmental Monitoring Station Setup:
- **Hardware Connection Diagram**: [Link to the diagram](#)
- **Code Repository**: [Access the code here](https://github.com/grandy0831/EnvironmentalMyths/tree/main/Code/Final/PWS)
- **Enclosure Printing Model**: [Download the 3D model here](https://www.thingiverse.com/thing:3615016#google_vignette)
- **Internal Setup Images**:


#### Artistic Installation Setup:
- **Layer Model Functionality**:
  - **Top Layer (L0)**: Combines three devices.
  - **Scene Display (L1-1)**: Details the exhibition of scenes.
  - **Continuous Rotation Servo Space (L1-2)**: Houses the internal mechanics.
  - **Rotation Platform (L2-1)**: Supports scene rotation, allowing space for wiring.
  - **Central Servo Fixation (L2-1)**: Holds the central servo controlling stage rotation.
  - **Display and Controls (L2-2-1, L2-2-2, L2-2-3)**: Accommodates the display, PIR sensor, switches, power sockets, and space for physical components and wiring.
- **Model Links**: [Access models here](https://github.com/grandy0831/EnvironmentalMyths/tree/main/Model/STAGE)
- **Assembly Images by Layers**: Visual guides for each layer's assembly.
- **Final Device Presentation**:
<img src="https://github.com/user-attachments/assets/decd7a4b-506a-4040-80a3-0a8d1578b86b" width="280" alt="IMG_5332">
<img src="https://github.com/user-attachments/assets/5f072f3f-1f9c-4695-ac01-a5b150376b31" width="280" alt="IMG_5333">
<img src="https://github.com/user-attachments/assets/5ff41017-0095-45d4-99ee-59d586f4c953" width="280" alt="IMG_5334">
<img src="https://github.com/user-attachments/assets/ec52f5a7-406d-4aeb-af49-7eceeb0e8ad2" width="280" alt="IMG_5335">
<img src="https://github.com/user-attachments/assets/d2c72353-e2b4-4c4b-b0c6-366e5314b58a" width="280" alt="IMG_5336">
<img src="https://github.com/user-attachments/assets/b11ad1e6-8b01-4978-b83a-279a7178badc" width="280" alt="IMG_5337">
<img src="https://github.com/user-attachments/assets/d1a08ef6-c305-4ea9-8716-4c738f78c9e4" width="280" alt="IMG_5338">
<img src="https://github.com/user-attachments/assets/7cd1bc4a-1826-4f67-8bad-02c26242024c" width="280" alt="IMG_5339">
<img src="https://github.com/user-attachments/assets/d9bda0c4-0513-44ba-a8f9-1b07973aa366" width="280" alt="IMG_5340">



#### Important Notes:
- Bullet points detailing critical considerations or warnings during the installation process.


