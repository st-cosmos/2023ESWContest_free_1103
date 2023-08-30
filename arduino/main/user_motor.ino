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