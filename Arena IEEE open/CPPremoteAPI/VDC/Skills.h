#include <VDC.h>
#include <string>
#include <iostream>


#ifndef SKILLS_H
#define SKILLS_H

class SKILLS: public VDC {
public:
    
    SKILLS();
     
    bool seguidorDeParede(int clientID);
    void connectToRobot(int clientID);
    void testJunta(int clientID);
    
    /*
    void setNumbAngles();
    void setJoints(int joint);
    void setSensor(int sensor);
     */   
        
        
    
    
    

    
    
    
    
    
private:
    
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