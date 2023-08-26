void initMotor() {
    pinMode(PIN_MOTOR_L_F, OUTPUT);
    pinMode(PIN_MOTOR_L_B, OUTPUT);
    pinMode(PIN_MOTOR_R_F, OUTPUT);
    pinMode(PIN_MOTOR_R_B, OUTPUT);
    setMotor(LOW, LOW, LOW, LOW);
}

void setMotor(bool lf, bool lb, bool rf, bool rb) {
        digitalWrite(PIN_MOTOR_L_F, lf);
        digitalWrite(PIN_MOTOR_L_B, lb);
        digitalWrite(PIN_MOTOR_R_F, rf);
        digitalWrite(PIN_MOTOR_R_B, rb);
}

// void goForward() {
//     digitalWrite(PIN_MOTOR_L_F, HIGH);
//     digitalWrite(PIN_MOTOR_L_B, LOW);
//     digitalWrite(PIN_MOTOR_R_F, HIGH);
//     digitalWrite(PIN_MOTOR_R_B, LOW);
// }

// void goBackward() {
//     digitalWrite(PIN_MOTOR_L_F, LOW);
//     digitalWrite(PIN_MOTOR_L_B, HIGH);
//     digitalWrite(PIN_MOTOR_R_F, LOW);
//     digitalWrite(PIN_MOTOR_R_B, HIGH);
// }

// void turnLeft() {
//     digitalWrite(PIN_MOTOR_L_F, HIGH);
//     digitalWrite(PIN_MOTOR_L_B, LOW);
//     digitalWrite(PIN_MOTOR_R_F, LOW);
//     digitalWrite(PIN_MOTOR_R_B, HIGH);
// }

// void turnRight() {
//     digitalWrite(PIN_MOTOR_L_F, LOW);
//     digitalWrite(PIN_MOTOR_L_B, HIGH);
//     digitalWrite(PIN_MOTOR_R_F, HIGH);
//     digitalWrite(PIN_MOTOR_R_B, LOW);
// }

// void stop() {
//     digitalWrite(PIN_MOTOR_L_F, LOW);
//     digitalWrite(PIN_MOTOR_L_B, LOW);
//     digitalWrite(PIN_MOTOR_R_F, LOW);
//     digitalWrite(PIN_MOTOR_R_B, LOW);
// }