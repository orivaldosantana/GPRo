/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ToninhoVrep.cpp
 * Author: samuel
 * 
 * Created on 4 de Outubro de 2017, 12:52
 */

#include "ToninhoVrep.h"

ToninhoVrep::ToninhoVrep(std::string serverIP, int serverPort) {

    Vrep = new Vdc(serverIP.c_str(), serverPort);
    //Vdc vrep(serverIP.c_str(), serverPort);
    //Vrep = &vrep;

    conectRobot();

}

ToninhoVrep::ToninhoVrep(const ToninhoVrep& orig) {
}

ToninhoVrep::~ToninhoVrep() {
}

void ToninhoVrep::conectRobot() {
    Vrep->conectpiece("DynamicLeftJoint#0", leftMotor);
    Vrep->conectpiece("DynamicRightJoint#0", rightMotor);
    Vrep->conectpiece("LeftSensor#0", leftSensor);
    Vrep->conectpiece("RightSensor#0", rightSensor);
    Vrep->conectpiece("UltrasonicSensor#0", proximitySensor);
}

bool ToninhoVrep::detectedByLeft() {
    std::vector<float> auxLeftSensor;
    if (Vrep->readVisionSensor(leftSensor, auxLeftSensor, 12))
        if (auxLeftSensor[11] < 0.3)
            return true;

    return false;
}

bool ToninhoVrep::detectedByRight() {
    std::vector<float> auxRightSensor;
    if (Vrep->readVisionSensor(rightSensor, auxRightSensor, 12) and auxRightSensor[11] < 0.3)
        return true;

    return false;



}

bool ToninhoVrep::getDistanceOfProximitySensor(float &distance) {
    if (Vrep->readProximitySensor(proximitySensor, distance))
        return true;

    return false;


}

void ToninhoVrep::setVelocityInToninho(float _rightMotor, float _leftMotor) {
    Vrep->setJointVelocity(rightMotor, _rightMotor);
    Vrep->setJointVelocity(leftMotor, _leftMotor);
}

void ToninhoVrep::seguirLinha() {
    float rightVelocity = 2;
    float leftVelocity = 2;
  


    while ( loop() ) {

       
        (detectedByLeft())  ? leftVelocity = 1 : leftVelocity = 2;
        (detectedByRight()) ? rightVelocity  = 1 : rightVelocity  = 2;

         
        setVelocityInToninho(rightVelocity, leftVelocity);
        
        delay(100);

        

    }
}

bool ToninhoVrep::loop(){
    if (Vrep->simulationIsActive())
        return true;
   
    return false;
}

void ToninhoVrep::delay(int time){
    Vrep->delay(time);
    
}