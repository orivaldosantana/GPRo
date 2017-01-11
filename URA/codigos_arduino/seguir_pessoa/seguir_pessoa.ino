#include "Ultrasonic.h"
#include "DCMotor.h"

const char C_LED = 'l';
const char C_ULTRASONIC = 'u'; 
const char C_DCMOTOR = 'm'; 

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

  if (distancia > 20 )
    dcMotor.forward();  // andar para frente 
  delay(1000);
  dcMotor.stop(); 

}
