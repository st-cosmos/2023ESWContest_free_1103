#define PIN_US_ECHO 7
#define PIN_US_TRIG 8


void setup() {
  Serial.begin(9600);
  pinMode(PIN_US_ECHO, INPUT);
  pinMode(PIN_US_TRIG, OUTPUT);
}

void loop() {
  float cycletime;
  float distance = 0;

  digitalWrite(PIN_US_TRIG, HIGH);
  delay(10);
  digitalWrite(PIN_US_TRIG,LOW);

  cycletime = pulseIn(PIN_US_ECHO, HIGH); 
  distance = ((340 * cycletime) / 10000) / 2;  
  
  Serial.println(distance);
  delay(500);
}