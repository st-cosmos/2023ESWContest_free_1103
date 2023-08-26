#include <SoftwareSerial.h>

// #define PIN_BLE_RX 40
// #define PIN_BLE_TX 41

// SoftwareSerial ble(PIN_BLE_RX, PIN_BLE_TX);
#define ble Serial2

void setup() {
    Serial.begin(9600);
    ble.begin(9600);
}

void loop() {
    if(Serial.available()) {
        ble.write(Serial.read());
    }

    if(ble.available()) {
        Serial.write(ble.read());
    }
}