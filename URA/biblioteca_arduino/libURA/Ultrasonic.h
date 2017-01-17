#ifndef ULTRASONIC_H
#define ULTRASONIC_H

class Ultrasonic 
{
  public:
    Ultrasonic(int trigPin, int echoPin);
    int getDistance(); 
  private:
    int _trigPin;
    int _echoPin; 
};

#endif 
