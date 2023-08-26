#include <QMC5883LCompass.h>

QMC5883LCompass compass;

void setup() {
  Serial.begin(9600);
  compass.init();
  compass.setCalibrationOffsets(-631.00, -465.00, -490.00);
  compass.setCalibrationScales(0.98, 1.10, 0.93);
}

void loop() {
  int x, y, z;
  
  // Read compass values
  compass.read();

  // Return XYZ readings
  x = compass.getX();
  y = compass.getY();
  z = compass.getZ();
  
  // Serial.print("X: ");
  // Serial.print(x);
  // Serial.print(" Y: ");
  // Serial.print(y);
  // Serial.print(" Z: ");
  // Serial.print(z);
  // Serial.println();
  float heading = atan2(y, x) + 0.72;
  if (heading < 0) {
    heading += 2 * PI;
  }
  if (heading > 2 * PI) {
    heading -= 2 * PI;
  }

  float headingDegrees = heading * 180.0 / PI;

  Serial.println(headingDegrees);
  
  // delay(250);
}