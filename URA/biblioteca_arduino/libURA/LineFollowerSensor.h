#ifndef LINEFOLLOWERSENSOR_H
#define LINEFOLLOWERSENSOR_H




class LineFollowerSensor 
{
  public:
    LineFollowerSensor(int pinLeftSensor, int pinRightSensor, int threshold);
    bool leftSensorIsUp();
    bool rightSensorIsUp(); 

  private:

    int _leftSensor, _rightSensor; 
    int _threshold;
  
};

#endif
