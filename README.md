This is the source code used for a line following robot with some additional features, the following is the hardware used for this project:
  1. Arduino UNO (x1)
  2. L298N Motor driver (x1)
  3. IR sensors (x2)
  4. 2x16 LCD display (x1)
  5. Rotary encoders (x2)
  6. HC-SR04 Ultrasonic sensor module (x1)
  7. MPU-6050 Accelerometer-gyroscope module (x1)
  8. HC-05 Bluetooth module (x1)

The main objective of the robot is to follow a black line on a track with white background, but it has some additional features too:
  1. Display time and distance on the LCD display
  2. Avoid obstacles within a set range
  3. Display pitch angle on LCD display
  4. Bluetooth control for movement

Inside the source code folder, there are 5 programs. Not all features can be present at once as each program is designed to execute a specific task due to hardware limitations.
