#include <VDC.h>
#include <string>
#include <iostream>
extern "C" {
#include "extApi.h"
}


#ifndef SKILLS_H
#define SKILLS_H

class SKILLS: public VDC {
public:
    
    SKILLS(std::string serverIP,int serverPort){
        
        this->clientID =simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);
        angle[5] = { };
        
    }
     
    void seguidorDeParede();
    void connectToRobot();
    void testJunta();
    void testGetImage();
    void testSetImage();
    void testReadCam ();
    void goToTank();
    void WhereIsTheCow();

    
    
private:
    
    void setVelocityInRobot(float velocityRight, float velocityLeft);
    
     // MOTORES e juntas
   // int numbJoints;
    int joint [9];
    
    //GARRA e sensores
    //int  numbSensors;;
    int sensor[8]; 
    
    //int numbAngles;
    double angle[5];
    
 
    
    //Sensor vision
    int Webcam;
    
   













};





















#endif