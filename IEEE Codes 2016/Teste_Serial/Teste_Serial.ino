void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}
char x;
void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    x = Serial.read();
    if (x == 'w'){
      digitalWrite(13, HIGH);
      delay(2000);
      digitalWrite(13, LOW);
      delay(2000);
    }
  }
}
