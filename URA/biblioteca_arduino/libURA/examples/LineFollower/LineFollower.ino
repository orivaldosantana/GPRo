
#include "DCMotor.h"
#include "LineFollower.h"

DCMotor dcMotor(5, 4, 3, 2);
LineFollower lineFollower(A0,A2,500); 

int firstSensor = 0;    // first analog sensor
int secondSensor = 0;   // second analog sensor


void setup() {
  Serial.begin(9600); 
 
}

void loop() {


  firstSensor = analogRead(A0);
  secondSensor = analogRead(A2);
  Serial.print(firstSensor);
  Serial.print(",");
  Serial.print(secondSensor);
  Serial.println("");

  
  //if ((firstSensor > 500) && (secondSensor > 500) ) {
  if (lineFollower.leftSensorIsUp()  &&  lineFollower.rightSensorIsUp() ) {
    dcMotor.stop();
  }
  else if (firstSensor > 500) {
    Serial.println("Vire para a direita");
    dcMotor.right();
  }
  else if (secondSensor > 500) {
    Serial.println("Vire para a esquerda");
    dcMotor.left();
  }
  else {
    Serial.println("Ré");
    dcMotor.backward();

  }
  delay(100);
  dcMotor.stop();
  delay(300);
}

