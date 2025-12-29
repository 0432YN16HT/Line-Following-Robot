#include <LiquidCrystal.h>

// LCD setup
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Ultrasonic sensor pins
const int trigPin = A5;
const int echoPin = A4;

// Motor pins
int IN1 = 2, IN2 = 12, IN3 = 13, IN4 = A1, ENA = 3, ENB = 11;

// Threshold distance (cm)
const int OBSTACLE_DISTANCE = 30;

long duration;
float distance;

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Dist(cm):");

  // Motor pins
  int motorPins[] = {IN1, IN2, IN3, IN4, ENA, ENB};
  for (int i = 0; i < 6; i++) pinMode(motorPins[i], OUTPUT);

  // Ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  lcd.setCursor(10, 0);
  lcd.print("      ");
  lcd.setCursor(10, 0);
  lcd.print(distance);

  // Motor logic
  if (distance < OBSTACLE_DISTANCE) {
    right(110,110);
  } else {
    forward(110, 110);
  }

}

// Motor functions
void forward(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, rightSpeed);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, leftSpeed);
}

void right(int leftSpeed, int rightSpeed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, rightSpeed);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, leftSpeed);
}



