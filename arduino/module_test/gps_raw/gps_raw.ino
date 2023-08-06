#include <SoftwareSerial.h>

#define PIN_GPS_RX 3
#define PIN_GPS_TX 4

SoftwareSerial gps(PIN_GPS_RX, PIN_GPS_TX);

void setup() {
    Serial.begin(9600);
    Serial.println("Start Serial...");  

    gps.begin(9600);
}

void loop() {
    if(gps.available()) {
        Serial.write(gps.read());
    }
}