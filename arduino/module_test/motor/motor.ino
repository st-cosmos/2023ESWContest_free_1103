#define PIN_LEFT_FORWARD 9
#define PIN_LEFT_BACKWARD 10
#define PIN_RIGHT_FORWARD 11
#define PIN_RIGHT_BACKWARD 12

void setup() {
    pinMode(PIN_LEFT_FORWARD, OUTPUT);
    pinMode(PIN_LEFT_BACKWARD, OUTPUT);
    pinMode(PIN_RIGHT_FORWARD, OUTPUT);
    pinMode(PIN_RIGHT_BACKWARD, OUTPUT);
}

void goForward() {
    digitalWrite(PIN_LEFT_FORWARD, HIGH);
    digitalWrite(PIN_LEFT_BACKWARD, LOW);
    digitalWrite(PIN_RIGHT_FORWARD, HIGH);
    digitalWrite(PIN_RIGHT_BACKWARD, LOW);
}

void goBackward() {
    digitalWrite(PIN_LEFT_FORWARD, LOW);
    digitalWrite(PIN_LEFT_BACKWARD, HIGH);
    digitalWrite(PIN_RIGHT_FORWARD, LOW);
    digitalWrite(PIN_RIGHT_BACKWARD, HIGH);
}

void turnLeft() {
    digitalWrite(PIN_LEFT_FORWARD, LOW);
    digitalWrite(PIN_LEFT_BACKWARD, HIGH);
    digitalWrite(PIN_RIGHT_FORWARD, HIGH);
    digitalWrite(PIN_RIGHT_BACKWARD, LOW);
}

void turnRight() {
    digitalWrite(PIN_LEFT_FORWARD, HIGH);
    digitalWrite(PIN_LEFT_BACKWARD, LOW);
    digitalWrite(PIN_RIGHT_FORWARD, LOW);
    digitalWrite(PIN_RIGHT_BACKWARD, HIGH);
}

void stop() {
    digitalWrite(PIN_LEFT_FORWARD, LOW);
    digitalWrite(PIN_LEFT_BACKWARD, LOW);
    digitalWrite(PIN_RIGHT_FORWARD, LOW);
    digitalWrite(PIN_RIGHT_BACKWARD, LOW);
}

void loop() {
    goForward(); delay(500);
    goBackward(); delay(500);
    turnLeft(); delay(500);
    turnRight(); delay(500);
    stop(); delay(1500);
}