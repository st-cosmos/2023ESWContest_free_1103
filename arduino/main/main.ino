#include <QMC5883LCompass.h>

// BLUETOOTH
#define ble Serial2

// GPS
#define gps Serial1

// MOTOR
#define PIN_MOTOR_L_F 4
#define PIN_MOTOR_L_B 5
#define PIN_MOTOR_R_F 6
#define PIN_MOTOR_R_B 7

#define LAT_RANGE_MAX 37.630968
#define LAT_RANGE_MIN 37.630855
#define LAT_TARGET 37.630912

#define LNG_RANGE_MAX 127.079620
#define LNG_RANGE_MIN 127.079512
#define LNG_TARGET 127.079566

#define MAX_STRING_PARTS 6

typedef struct {
    int degree;
    int minute;
    double second;
} GPosition;

QMC5883LCompass compass;

int count = 0;

// functions
void initMotor();
// void goForward();
// void goBackward();
// void turnLeft();
// void turnRight();
// void stop();
void setMotor();

void initCompass();
float getHeading();

void initGps();
void getCurrentPosition(double& lat, double& lng);

void splitString(char* input, char* delimiter, char** output, int max_parts = MAX_STRING_PARTS);
void convertRawToGPos(GPosition& pos, double raw_value);
int getLatAndLng(String& string, GPosition& lat, GPosition& lng);
// void printGPosition(GPosition& pos);
double getGPositionValue(GPosition& pos);

void waitGps(int max_iter = 100);
void alignDirection(int max_iter = 100);
void move(int max_iter = 1000);

void setup() {
    Serial.begin(9600);
    ble.begin(9600);
    initMotor();
    initCompass();
    initGps();
    waitGps(100);
}

void loop() {
    // alignDirection(100);
    // move(1000);
    count++;

    if(ble.available()) {
        char c = ble.read();
        ble.write(c); ble.write('\r'); ble.write('\n');
        switch (c)
        {
        case 'F':
            setMotor(HIGH, LOW, HIGH, LOW);
            break;
        case 'B':
            setMotor(LOW, HIGH, LOW, HIGH);
            break;
        case 'L':
            setMotor(LOW, HIGH, HIGH, LOW);
            break;
        case 'R':
            setMotor(HIGH, LOW, LOW, HIGH);
            break;
        case 'S':
            setMotor(LOW, LOW, LOW, LOW);
            break;        
        default:
            break;
        }
    }

    double latValue, lngValue;
    getCurrentPosition(latValue, lngValue);

    float heading = getHeading();
    double targetHeading = atan2(LNG_TARGET - lngValue, LAT_TARGET - latValue) * 180.0 / PI;
    
    if(count == 100) {
        ble.print("Curr Heading: "); ble.println(heading, 5);
        ble.print("Tar Heading: "); ble.println(targetHeading, 5);

        ble.print("Current Pos: "); ble.print(latValue, 5); ble.print(" : "); ble.println(lngValue, 5);
        ble.print("Target Pos: "); ble.print(LAT_TARGET, 5); ble.print(" : "); ble.println(LNG_TARGET, 5);

        ble.print("[Moving Status] ");
        if(latValue > LAT_RANGE_MIN && latValue < LAT_RANGE_MAX && lngValue > LNG_RANGE_MIN && lngValue < LNG_RANGE_MAX) {
            ble.println("STOP");
        }
        else {
            double headingError = targetHeading - heading;
            if(headingError > 180) { headingError -= 360; }
            else if(headingError < -180) { headingError += 360; }
            
            ble.print("Heading error: "); ble.print(headingError); ble.print(" | ");
            // Serial.println("Heading curr | targ => " + String(heading) + " | " + String(targetHeading));

            if (headingError > 30) {
                // turnRight();        
                // setMotor(HIGH, LOW, LOW, HIGH);
                ble.println("Turn Right");
            }
            else if(headingError < -30) {
                // turnLeft();
                // setMotor(LOW, HIGH, HIGH, LOW);
                // Serial.println("TL");
                ble.println("Turn Left");
            }
            else {
                // stop();                
                // setMotor(LOW, LOW, LOW, LOW);
                // Serial.println("DT");
                // return;
                ble.println("Don't Turn");
            }
        }
        ble.println("-----");
        count = 0;
    }
    // delay(50);
}

void waitGps(int max_iter = 100) {
    double heading;
    double latValue, lngValue;

    for(int i = 0; i < max_iter; i++) {
        heading = getHeading();
        getCurrentPosition(latValue, lngValue);

        if(latValue > 36 && latValue < 38 && lngValue > 126 && lngValue < 128) {
            return;
        }
        
    }
    Serial.println("GX");
}

void alignDirection(int max_iter = 100) {
    for(int i = 0; i < max_iter; i++) {
        double heading = getHeading();
        double latValue, lngValue;
        getCurrentPosition(latValue, lngValue);
        Serial.print("CP, "); Serial.print(latValue, 5); Serial.print(", "); Serial.println(lngValue, 5);
        // Serial.println("CP:" + String(latValue) + "," + String(lngValue)); 
        // Serial.println("Target Pos:\t" + String(LAT_TARGET) + ", " + String(LNG_TARGET)); 

        double targetHeading = atan2(LNG_TARGET - lngValue, LAT_TARGET - latValue) * 180.0 / PI;
        // if(targetHeading < 0)  targetHeading += 360.0;

        // if(heading > 180) heading -= 360;
        // if(targetHeading > 180) targetHeading -=360;

        double headingError = targetHeading - heading;
        if(headingError > 180) { headingError -= 360; }
        else if(headingError < -180) { headingError += 360; }
        
        Serial.println(headingError);
        // Serial.println("Heading curr | targ => " + String(heading) + " | " + String(targetHeading));

        if (headingError > 30) {
            // turnRight();        
            setMotor(HIGH, LOW, LOW, HIGH);
            Serial.println("TR");
        }
        else if(headingError < -30) {
            // turnLeft();
            setMotor(LOW, HIGH, HIGH, LOW);
            Serial.println("TL");
        }
        else {
            // stop();                
            setMotor(LOW, LOW, LOW, LOW);
            Serial.println("DT");
            return;
        }
    }
}

void move(int max_iter = 1000) {
    double heading;
    double latValue, lngValue;
    
    for(int i = 0; i < max_iter; i++) {
        heading = getHeading();
        getCurrentPosition(latValue, lngValue);
        Serial.print("CP, "); Serial.print(latValue, 5); Serial.print(", "); Serial.println(lngValue, 5);
        
        if(latValue > LAT_RANGE_MIN && latValue < LAT_RANGE_MAX && lngValue > LNG_RANGE_MIN && lngValue < LNG_RANGE_MAX) {
            Serial.println("S");
            // stop();
            setMotor(LOW, LOW, LOW, LOW);
            delay(1000);
            return;
        }
        else {
            Serial.println("GF");
            // goForward();        
            setMotor(HIGH, LOW, HIGH, LOW);

            // if(latValue - LAT_TARGET > 0) Serial.print("to S,");
            // else if(latValue - LAT_TARGET < 0) Serial.print("to N,");
            // else Serial.print("to ,");

            // if(lngValue - LNG_TARGET > 0) Serial.println("to W,");
            // else if(lngValue - LNG_TARGET < 0) Serial.println("to E,");
            // else Serial.println("to ,");
        }
    }
}