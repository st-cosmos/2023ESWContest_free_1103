#define PIN_LED 13

void initLed() {
    pinMode(PIN_LED, OUTPUT);
}

void turnOnLed() {
    digitalWrite(PIN_LED, HIGH);
}

void turnOffLed() {
    digitalWrite(PIN_LED, LOW);
}