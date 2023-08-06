#define PIN_US_ECHO 7
#define PIN_US_TRIG 8

int output_data = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_US_ECHO, INPUT);
  pinMode(PIN_US_TRIG, OUTPUT);
}

void loop() {
  float cycletime;
  float distance;

  cycletime = pulseIn(PIN_US_ECHO, HIGH); 
  distance = ((340 * cycletime) / 10000) / 2;  
  
  Serial.println(output_data++);
  delay(500);
}