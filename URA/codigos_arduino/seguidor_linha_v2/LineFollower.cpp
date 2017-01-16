#include "Arduino.h"
#include "LineFollower.h"


LineFollower::LineFollower(int pinLeftSensor, int pinRightSensor, int threshold) {
  _leftSensor = pinLeftSensor;
  _rightSensor = pinRightSensor;
  _threshold = threshold; 
  
}

bool LineFollower::leftSensorIsUp(){
  int v = analogRead(_leftSensor);
  if (v > _threshold) 
    return true;
  return false; 
  
}

bool LineFollower::rightSensorIsUp(){
  int v = analogRead(_rightSensor);
  if (v > _threshold) 
    return true;
   return false; 
}

