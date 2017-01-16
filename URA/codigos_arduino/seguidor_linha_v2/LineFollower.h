#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H




class LineFollower 
{
  public:
    LineFollower(int pinLeftSensor, int pinRightSensor, int threshold);
    bool leftSensorIsUp();
    bool rightSensorIsUp(); 

  private:

    int _leftSensor, _rightSensor; 
    int _threshold;
  
};

#endif
