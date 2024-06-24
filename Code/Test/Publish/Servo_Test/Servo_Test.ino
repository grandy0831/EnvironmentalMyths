#include <Servo.h>

// Create servo control objects for four servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  // Attach each servo signal line to the specified digital pin
  servo1.attach(9);
  servo2.attach(8);
  servo3.attach(7);
  servo4.attach(6);
}

void loop() {
  // Rotate servo 1
  servo1.write(180);
  delay(2000); // Rotate for 2 seconds
  servo1.write(90); // Stop rotation

  // Rotate servo 2
  servo2.write(180);
  delay(2000); // Rotate for 2 seconds
  servo2.write(90); // Stop rotation

  // Rotate servo 3
  servo3.write(180);
  delay(2000); // Rotate for 2 seconds
  servo3.write(90); // Stop rotation

  // Rotate servo 4
  servo4.write(180);
  delay(2000); // Rotate for 2 seconds
  servo4.write(90); // Stop rotation

  delay(1000); // Wait 1 second after all stop before starting the next rotation cycle
}
