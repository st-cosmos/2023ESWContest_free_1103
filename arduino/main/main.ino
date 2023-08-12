float heading;
float targetHeading;
double latValue;
double lngValue;
double count = 0;

void setup() {
    Serial.begin(9600);
    initMotor();
    initMpu();
    initGps();
}

void loop() {
    count++;
    heading = getHeading();
    getCurrentPosition(latValue, lngValue);

    if(latValue >LAT_RANGE_MIN && latValue < LAT_RANGE_MAX && lngValue > LNG_RANGE_MIN && lngValue < LNG_RANGE_MAX) {
        Serial.println("*** STOP ***");
        stop();
        delay(5000);
    }
    else {
        if(count > 1000) {
            goForward();
            Serial.print("*** GO *** ");
        }
        else if(count <= 1000) {
            targetHeading = atan2(lngValue - LNG_TARGET, latValue - LAT_TARGET) * 180.0 / PI;
            if(targetHeading < 0)  targetHeading += 360.0;

            if(heading > 180) heading -= 360;
            if(targetHeading > 180) targetHeading -=360;

            double headingError = heading - targetHeading;

            if (headingError > 30) turnLeft();
            else if(headingError < -30) trunRight();
        }
        else if(count > 10000) {
            count = 0;
        }
        // if(latValue - LAT_TARGET > 0) Serial.print("to S,");
        // else if(latValue - LAT_TARGET < 0) Serial.print("to N,");
        // else Serial.print("to ,");

        // if(lngValue - LNG_TARGET > 0) Serial.println("to W,");
        // else if(lngValue - LNG_TARGET < 0) Serial.println("to E,");
        // else Serial.println("to ,");
    }
}