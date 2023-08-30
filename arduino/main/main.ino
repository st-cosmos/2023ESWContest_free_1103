#include <ArduinoJson.h>
#include <QMC5883LCompass.h>
#include <string.h>
// GPS
#define gps Serial1
#define MAX_STRING_PARTS 6
// 모터
#define PIN_MOTOR_L_F 4
#define PIN_MOTOR_L_B 5
#define PIN_MOTOR_R_F 6
#define PIN_MOTOR_R_B 7
// 초음파
#define PIN_US_F_TRIG 26
#define PIN_US_F_ECHO 27
#define PIN_US_B_TRIG 28
#define PIN_US_B_ECHO 29
#define PIN_US_L_TRIG 30
#define PIN_US_L_ECHO 31
#define PIN_US_R_TRIG 32
#define PIN_US_R_ECHO 33
#define PIN_US_U_TRIG 34
#define PIN_US_U_ECHO 35
// 블루투스
#define ble Serial2

typedef struct {
    int degree;
    int minute;
    double second;
} GPosition;

void initGps();
void getCurrentPosition(double& lat, double& lng);

void splitString(char* input, char* delimiter, char** output, int max_parts = MAX_STRING_PARTS);
void convertRawToGPos(GPosition& pos, double raw_value);
int getLatAndLng(String& string, GPosition& lat, GPosition& lng);
// void printGPosition(GPosition& pos);
double getGPositionValue(GPosition& pos);

void initCompass();
float getHeading();

void initMotor();
void setMotor();

void initUS();
long _getDistance(int trigPin, int echoPin);
long getDistance(char index);

// 메인 함수
void waitGps(int max_iter = 100);
void moveAround();
void goToPosition();
void alignDirection();
void checkDistance();

DynamicJsonDocument doc(1024);
QMC5883LCompass compass;

double LAT_TARGET = 0., LNG_TARGET = 0.;
double latValue, lngValue;

int count = 0;
int moveCount = 0;

void setup() {
    Serial.println(9600);
    ble.begin(9600);
    initMotor();
    initCompass();
    initGps();
    waitGps(100);
}

bool moveAroundMode = false;
void loop() {
    count++;

    getCurrentPosition(latValue, lngValue);

    float heading = getHeading();
    double targetHeading = atan2(LNG_TARGET - lngValue, LAT_TARGET - latValue) * 180.0 / PI;

    if(Serial.available()) {
        String cmd = Serial.readString();
        doc.clear();
        deserializeJson(doc, cmd);

        String method = doc["method"];
        String resource = doc["resource"];
        if(!method.compareTo("get")) {
            if(!resource.compareTo("position")) {
                doc.clear();
                doc["latitude"] = latValue;
                doc["longitude"] = lngValue;
                serializeJson(doc, Serial);
            }
        }
        else if(!method.compareTo("set")) {
            if(!resource.compareTo("position")) {
                LAT_TARGET = doc["latitude"];
                LNG_TARGET = doc["longitude"];
                doc.clear();
                moveAroundMode = false;
                moveCount = 0;
            }
            else if(!method.compareTo("moveAround")) {
                moveAroundMode = true;
                moveCount = 0;
            }
        }
    }

    if(count % 100 == 0) {
        ble.print("Curr Heading: "); ble.println(heading, 5);
        ble.print("Tar Heading: "); ble.println(targetHeading, 5);

        ble.print("Current Pos: "); ble.print(latValue, 5); ble.print(" : "); ble.println(lngValue, 5);
        ble.print("Target Pos: "); ble.print(LAT_TARGET, 5); ble.print(" : "); ble.println(LNG_TARGET, 5);

        double headingError = targetHeading - heading;
        if(headingError > 180) { headingError -= 360; }
        else if(headingError < -180) { headingError += 360; }
        
        ble.print("Heading error: "); ble.print(headingError); ble.println(" | ");
    }

    if(moveAroundMode) {
        moveAround();
    }
    else {
        goToPosition();
    }
    checkDistance();
}

void waitGps(int max_iter = 100) {
    ble.println("Wating GPS...");
    double latValue, lngValue;

    for(int i = 0; i < max_iter; i++) {
        getCurrentPosition(latValue, lngValue);

        if(latValue > 36 && latValue < 38 && lngValue > 126 && lngValue < 128) {
            return;
        }   
    }
}

void moveAround() {
    if(moveCount > 0 && moveCount <= 30) {
        setMotor(HIGH, LOW, LOW, HIGH); // turn left
    }
    else if(moveCount > 30 && moveCount <= 60) {
        setMotor(LOW, HIGH, HIGH, LOW); // turn right
    }
    else if(moveCount > 60 && moveCount <= 120) {
        setMotor(HIGH, LOW, HIGH, LOW); // go forward
    }
    else if(moveCount > 120 && moveCount <= 150) {
        setMotor(HIGH, LOW, LOW, HIGH); // turn left
    }
    else {
        moveCount = 0;
    }
}

void goToPosition() {
    if(moveCount > 0 && moveCount <= 30) {
        alignDirection();
    }
    else if(moveCount > 30 && moveCount <= 120) {
        setMotor(HIGH, LOW, HIGH, LOW); // go forward
    }
    else {
        moveCount = 0;
    }
}


void alignDirection() {
    double heading = getHeading();
    double targetHeading = atan2(LNG_TARGET - lngValue, LAT_TARGET - latValue) * 180.0 / PI;
    
    if(targetHeading < 0)  targetHeading += 360.0;

    if(heading > 180) heading -= 360;
    if(targetHeading > 180) targetHeading -=360;

    double headingError = targetHeading - heading;
    if(headingError > 180) { headingError -= 360; }
    else if(headingError < -180) { headingError += 360; }

    if (headingError > 30) {
        setMotor(HIGH, LOW, LOW, HIGH);
    }
    else if(headingError < -30) {
        setMotor(LOW, HIGH, HIGH, LOW);
    }
    else {
        moveCount = 31;
    }
}

void checkDistance() {
    long distanceF = getDistance('F');
    long distanceL = getDistance('L');
    long distanceR = getDistance('R');
    long distanceB = getDistance('B');
    long distanceU = getDistance('U');

    if(count % 10 == 0) {
        ble.print("[US-FLRBU] ");
        ble.print(distanceF); ble.print(", ");
        ble.print(distanceL); ble.print(", ");
        ble.print(distanceR); ble.print(", ");
        ble.print(distanceB); ble.print(", ");
        ble.println(distanceU);
    }
    
    if(distanceF > 1 && distanceF < 7) {
        if(distanceL > distanceR) {
            ble.println("[Avoid] Turn left");
            setMotor(HIGH, LOW, LOW, HIGH);
            delay(300);
        }
        else {
            ble.println("[Avoid] Turn right");
            setMotor(LOW, HIGH, HIGH, LOW);
            delay(300);
        }
    }

    if(distanceL > 1 && distanceL < 7) {
        ble.println("[Avoid] Turn right");
        setMotor(LOW, HIGH, HIGH, LOW);
        delay(300);
    }

    if(distanceR > 1 && distanceR < 7) {
        ble.println("[Avoid] Turn left");
        setMotor(HIGH, LOW, LOW, HIGH);
        delay(300);
    }

    if(distanceU > 15 && distanceU < 100) {
        ble.println("[Avoid] Go back");
        setMotor(LOW, HIGH, LOW, HIGH);
        delay(300);
    }
}