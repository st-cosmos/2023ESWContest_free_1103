void initUS() {
    pinMode(PIN_US_F_TRIG, OUTPUT);
    pinMode(PIN_US_F_ECHO, INPUT);
    pinMode(PIN_US_B_TRIG, OUTPUT);
    pinMode(PIN_US_B_ECHO, INPUT);
    pinMode(PIN_US_L_TRIG, OUTPUT);
    pinMode(PIN_US_L_ECHO, INPUT);
    pinMode(PIN_US_R_TRIG, OUTPUT);
    pinMode(PIN_US_R_ECHO, INPUT);
    pinMode(PIN_US_U_TRIG, OUTPUT);
    pinMode(PIN_US_U_ECHO, INPUT);
}

long _getDistance(int trigPin, int echoPin) {
    long duration, distance;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 17 / 1000; // cm scale
    return distance;
}

long getDistance(char index) {
    int trigPin, echoPin;

    switch (index)
    {
    case 'F':
        trigPin = PIN_US_F_TRIG;
        echoPin = PIN_US_F_ECHO;
        break;
    case 'B':
        trigPin = PIN_US_B_TRIG;
        echoPin = PIN_US_B_ECHO;
        break;
    case 'L':
        trigPin = PIN_US_L_TRIG;
        echoPin = PIN_US_L_ECHO;
        break;
    case 'R':
        trigPin = PIN_US_R_TRIG;
        echoPin = PIN_US_R_ECHO;
        break;
    case 'U':
        trigPin = PIN_US_U_TRIG;
        echoPin = PIN_US_U_ECHO;
        break;
    default:
        break;
    }
    return _getDistance(trigPin, echoPin);
}