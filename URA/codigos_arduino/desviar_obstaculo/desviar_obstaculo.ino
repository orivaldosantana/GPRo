#include "Ultrasonic.h"
#include "DCMotor.h"

#define trigPin 12
#define echoPin 11


Ultrasonic ultrasonic(trigPin,echoPin); 
DCMotor dcMotor(5,4,3,2);

int distancia; 


void setup() {
  Serial.begin(9600);

}

void loop() {

  distancia = ultrasonic.getDistance();
  if (distancia < 20 )
    dcMotor.stop(); 
  else if (distancia < 40 )
    dcMotor.left();  // vira para esquerda 
  else 
    dcMotor.forward(); 
  delay(100);
  dcMotor.stop(); 

}
