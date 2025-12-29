#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Motor pins
int IN1 = 2, IN2 = 12, IN3 = 13, IN4 = A1, ENA = 3, ENB = 11;

// IR line sensors
int leftDigital = A2, rightDigital = A3;

// Encoders
int leftEncoder = A4, rightEncoder = A5;

// Encoder tracking
int lastLeftState = 0;
int lastRightState = 0;
unsigned long leftPulses = 0;
unsigned long rightPulses = 0;

// Wheel constants
const float wheelCirc = 3.141 * 6.5;
const float distancePerPulse = 0.560;

// Forward pulsing
unsigned long lastPulseTime = 0;
bool motorState = true;
const unsigned long pulseInterval = 25;

bool allowCount = false;

// Pause system
bool paused = false;
bool pauseDone = false;
unsigned long pauseStart = 0;
float frozenDist = 0;

void setup() {
    Serial.begin(9600);

    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.print("Dist: ");
    lcd.setCursor(0,1);
    lcd.print("Time(s): ");

    // Arrays for initialization
    int motorPins[] = {IN1, IN2, IN3, IN4, ENA, ENB};
    int irPins[] = {leftDigital, rightDigital};
    int encoderPins[] = {leftEncoder, rightEncoder};

    // Motor pins
    for (int i = 0; i < 6; i++) pinMode(motorPins[i], OUTPUT);

    // IR pins
    for (int i = 0; i < 2; i++) pinMode(irPins[i], INPUT);

    // Encoder pins
    for (int i = 0; i < 2; i++) pinMode(encoderPins[i], INPUT);

    lastLeftState  = digitalRead(leftEncoder);
    lastRightState = digitalRead(rightEncoder);
}

void loop() {
    unsigned long now = millis();
    int leftD  = digitalRead(leftDigital);
    int rightD = digitalRead(rightDigital);

    updateEncoders();

    float dist = (leftPulses + rightPulses) * 0.5 * distancePerPulse;

    // DISPLAY HANDLING
    lcd.setCursor(6, 0);
    if (paused) {
        lcd.print(frozenDist, 1);     // Show 260.0 during stop
    } else {
        lcd.print(dist, 1);           // Live distance
    }
    lcd.print(" cm   ");

    lcd.setCursor(9, 1);
    lcd.print(now / 1000);
    lcd.print("   ");

    // Pause logic
    if (!paused && !pauseDone && dist >= 260.0) {
        paused = true;
        pauseDone = true;
        pauseStart = now;
        frozenDist = 260.0;           // Freeze display at 260.0 cm
        stop();
    }

    if (paused) {
        if (now - pauseStart >= 3000) {
            paused = false;           // Resume after 3 seconds
        } else {
            stop();
            return;
        }
    }

    // Line following logic
    if (leftD == 1 && rightD == 0) {
        allowCount = false;
        turnRight();
    }
    else if (leftD == 0 && rightD == 1) {
        allowCount = false;
        turnLeft();
    }
    else if (leftD == 1 && rightD == 1) {
        allowCount = true;
        moveForward();
    }
    else {
        allowCount = false;
        stop();
    }
}

// Encoder logic
void updateEncoders() {
    if (!allowCount) return;

    int L = digitalRead(leftEncoder);
    int R = digitalRead(rightEncoder);

    if (L != lastLeftState)  leftPulses++;
    if (R != lastRightState) rightPulses++;

    lastLeftState = L;
    lastRightState = R;
}

// Motor functions
void moveForward() {
    unsigned long now = millis();

    // Forward pulsing control
    if (now - lastPulseTime >= pulseInterval) {
        lastPulseTime = now;
        motorState = !motorState;
    }

    if (motorState) {
        analogWrite(ENA, 180);
        analogWrite(ENB, 180);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    } else {
        analogWrite(ENA, 0);
        analogWrite(ENB, 0);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
    }
}

void turnLeft() {
    analogWrite(ENA, 180);
    analogWrite(ENB, 180);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnRight() {
    analogWrite(ENA, 180);
    analogWrite(ENB, 180);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void stop() {
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}


