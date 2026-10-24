// Reconstructed control sketch for the vertical lift bridge prototype.
// Assumed driver: L298N, with the ENA jumper fitted.
// Buttons connect D2 and D3 to GND. Driver IN1 -> D8, IN2 -> D9.
// Use a separate motor supply and a common ground with the Nano.

const byte upButton = 2;
const byte downButton = 3;
const byte motorIn1 = 8;
const byte motorIn2 = 9;

int motorDirection = 0;
int lastRequest = 0;
unsigned long requestChangedAt = 0;

void stopMotor() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
}

void setup() {
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  stopMotor();
}

void loop() {
  bool upPressed = digitalRead(upButton) == LOW;
  bool downPressed = digitalRead(downButton) == LOW;

  int requestedDirection = 0;
  if (upPressed && !downPressed) {
    requestedDirection = 1;
  } else if (downPressed && !upPressed) {
    requestedDirection = -1;
  }

  if (requestedDirection != lastRequest) {
    lastRequest = requestedDirection;
    requestChangedAt = millis();
  }

  // Releasing the buttons, or pressing both, stops the motor.
  if (requestedDirection == 0) {
    stopMotor();
    motorDirection = 0;
    return;
  }

  // Give the button contacts a moment to settle before starting.
  if (millis() - requestChangedAt < 25) {
    return;
  }

  if (requestedDirection != motorDirection) {
    stopMotor();
    if (motorDirection != 0) {
      // Pause before reversing. Read the buttons again on the next loop.
      motorDirection = 0;
      delay(100);
      return;
    }

    digitalWrite(motorIn1, requestedDirection == 1 ? HIGH : LOW);
    digitalWrite(motorIn2, requestedDirection == -1 ? HIGH : LOW);
    motorDirection = requestedDirection;
  }
}
