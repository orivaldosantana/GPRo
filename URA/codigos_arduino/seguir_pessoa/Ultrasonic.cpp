#include "Ultrasonic.h"
#include "Arduino.h"

Ultrasonic::Ultrasonic(int trigPin, int echoPin){
  _trigPin = trigPin; 
  _echoPin = echoPin; 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

int Ultrasonic::getDistance() {
  int duration, distance;
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(_trigPin, LOW);
  duration = pulseIn(_echoPin, HIGH);
  // v = d / t => d = v * t 
  //distance = (duration/2) / 29.1;
  distance = (duration/2) * 0.03434; // velocidade do som 340,29 m/s ou 0.034029 cm/ss (centímetros por microsegundos) 
  if (distance >= 200 || distance <= 0){
    return 200;
  }
  else {
    return distance; 
  }
}

