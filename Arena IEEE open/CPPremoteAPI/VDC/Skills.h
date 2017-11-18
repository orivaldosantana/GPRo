

#ifndef SKILLS_H
#define SKILLS_H

#include "Vdc.h"
#include "VrepToOpencv.h"
#include <string>
#include <iostream>
extern "C" {
#include "extApi.h"
}


class SKILLS {
public:
    
    SKILLS(std::string serverIP, int serverPort);
     
    void seguidorDeParede();
    void connectToRobot();
    void testJunta();
    void testGetImage();
    void testSetImage();
    void testReadCam ();
    void goToTank();
    void WhereIsTheCow();
    void obr();
    void seguirParedeMLP();
    void verDistancia(int i);
    void collectDataforNetWork();
    void controlTheRobot();
    void seguirParedeSOM();
    void OpenTheClawCloseTheClaw(bool OpenTheClawCloseTheClaw);
    void camilaSeguirLinha();

    
    
private:
    Vdc* vrep;
    VrepToOpencv* visionSensor;
    bool visionInfo();
    void trainingSOM(int size,std::string input);
    bool controlerRobot();
    void setVelocityInRobot(float velocityRight, float velocityLeft);
    void setVelocityForControler(float velocityRight, float velocityLeft);
    void setPositionForControler(int joint , bool positive);
    void takePhotos();
    double degree(double Degree);
    bool trained = false;
    bool debug;
    std::string controlData;
    std::string visionData;
    int countImage =0;
    
     // MOTORES e juntas
   // int numbJoints;
    int joint [9];
    
    //GARRA e sensores
    //int  numbSensors;;
    int sensor[8]; 
    
    //int numbAngles;
    double angle[4];
    
 
    
    //Sensor vision
    int Webcam;
    
   













};





















#endif