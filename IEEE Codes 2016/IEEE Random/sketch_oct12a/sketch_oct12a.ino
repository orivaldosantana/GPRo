#include <Ultrasonic.h>

const int ultrasTrigger0 = 22;
const int ultrasEcho0 = 24;
const int ultrasTrigger1 = 28;
const int ultrasEcho1 = 30;
const int ultrasTrigger2 = 34;
const int ultrasEcho2 = 36;
const int ultrasTrigger3 = 40;
const int ultrasEcho3 = 42;
const int ultrasTrigger4 = 46;
const int ultrasEcho4 = 48;


Ultrasonic ultra0(ultrasTrigger0, ultrasEcho0);
Ultrasonic ultra1(ultrasTrigger1, ultrasEcho1);
Ultrasonic ultra2(ultrasTrigger2, ultrasEcho2);
Ultrasonic ultra3(ultrasTrigger3, ultrasEcho3);
Ultrasonic ultra4(ultrasTrigger4, ultrasEcho4);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float cmMsec[5];
  long microsec[5];

  microsec[0] = ultra0.timing();
  microsec[1] = ultra1.timing();
  microsec[2] = ultra2.timing();
  microsec[3] = ultra3.timing();
  microsec[4] = ultra4.timing();

  cmMsec[0] = ultra0.convert(microsec[0], Ultrasonic::CM);
  cmMsec[1] = ultra1.convert(microsec[1], Ultrasonic::CM);
  cmMsec[2] = ultra2.convert(microsec[2], Ultrasonic::CM);
  cmMsec[3] = ultra3.convert(microsec[3], Ultrasonic::CM);
  cmMsec[4] = ultra4.convert(microsec[4], Ultrasonic::CM);

  Serial.print("CM0: ");
  Serial.println(cmMsec[0]);
  delay(100);

  Serial.print("CM1: ");
  Serial.println(cmMsec[1]);
  delay(100);

  Serial.print("CM2: ");
  Serial.println(cmMsec[2]);
  delay(100);

  Serial.print("CM3: ");
  Serial.println(cmMsec[3]);
  delay(100);

  Serial.print("CM4: ");
  Serial.println(cmMsec[4]);
  delay(100);

}
