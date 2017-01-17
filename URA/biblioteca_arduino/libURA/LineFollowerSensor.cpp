#include "Arduino.h"
#include "LineFollowerSensor.h"


LineFollowerSensor::LineFollowerSensor(int pinLeftSensor, int pinRightSensor, int threshold) {
  _leftSensor = pinLeftSensor;
  _rightSensor = pinRightSensor;
  _threshold = threshold; 
  
}

bool LineFollowerSensor::leftSensorIsUp(){
  int v = analogRead(_leftSensor);
  if (v > _threshold) 
    return true;
  return false; 
  
}

bool LineFollowerSensor::rightSensorIsUp(){
  int v = analogRead(_rightSensor);
  if (v > _threshold) 
    return true;
   return false; 
}

