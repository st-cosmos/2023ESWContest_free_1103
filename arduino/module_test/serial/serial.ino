#define PIN_LEFT_FORWARD 9
#define PIN_LEFT_BACKWARD 10

char cmd;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LEFT_FORWARD, OUTPUT);
  pinMode(PIN_LEFT_BACKWARD, OUTPUT);
  Serial.begin(9600);
}

void motorForward() {
  digitalWrite(PIN_LEFT_FORWARD, HIGH);
  digitalWrite(PIN_LEFT_BACKWARD, LOW);
}

void motorBackward() {
  digitalWrite(PIN_LEFT_FORWARD, LOW);
  digitalWrite(PIN_LEFT_BACKWARD, HIGH);
}

void motorStop() {
  digitalWrite(PIN_LEFT_FORWARD, LOW);
  digitalWrite(PIN_LEFT_BACKWARD, LOW);
}

void loop() {
  // 컴퓨터로부터 시리얼 통신이 전송되면, 한줄씩 읽어와서 cmd 변수에 입력
  if(Serial.available()){
    cmd = Serial.read(); 

    if(cmd=='f'){
      motorForward();
    }
    else if(cmd=='b'){
      motorBackward();
    }
    else {
      motorStop();
    }
  }
  delay(100);
}