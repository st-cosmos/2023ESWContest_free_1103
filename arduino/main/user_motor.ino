#define PIN_MOTOR_L_F 5
#define PIN_MOTOR_L_B 4
#define PIN_MOTOR_R_F 3
#define PIN_MOTOR_R_B 2

void initMotor() {
    pinMode(PIN_MOTOR_L_F, OUTPUT);
    pinMode(PIN_MOTOR_L_B, OUTPUT);
    pinMode(PIN_MOTOR_R_F, OUTPUT);
    pinMode(PIN_MOTOR_R_B, OUTPUT);
    stop();
}

void goForward() {
    digitalWrite(PIN_MOTOR_L_F, HIGH);
    digitalWrite(PIN_MOTOR_L_B, LOW);
    digitalWrite(PIN_MOTOR_R_F, HIGH);
    digitalWrite(PIN_MOTOR_R_B, LOW);
}

void goBackward() {
    digitalWrite(PIN_MOTOR_L_F, LOW);
    digitalWrite(PIN_MOTOR_L_B, HIGH);
    digitalWrite(PIN_MOTOR_R_F, LOW);
    digitalWrite(PIN_MOTOR_R_B, HIGH);
}

void turnLeft() {
    digitalWrite(PIN_MOTOR_L_F, HIGH);
    digitalWrite(PIN_MOTOR_L_B, LOW);
    digitalWrite(PIN_MOTOR_R_F, LOW);
    digitalWrite(PIN_MOTOR_R_B, HIGH);
}

void turnRight() {
    digitalWrite(PIN_MOTOR_L_F, LOW);
    digitalWrite(PIN_MOTOR_L_B, HIGH);
    digitalWrite(PIN_MOTOR_R_F, HIGH);
    digitalWrite(PIN_MOTOR_R_B, LOW);
}

void stop() {
    digitalWrite(PIN_MOTOR_L_F, LOW);
    digitalWrite(PIN_MOTOR_L_B, LOW);
    digitalWrite(PIN_MOTOR_R_F, LOW);
    digitalWrite(PIN_MOTOR_R_B, LOW);
}