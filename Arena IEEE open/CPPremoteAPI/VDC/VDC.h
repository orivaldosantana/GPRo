/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VDC.h
 * Author: samuel
 *
 * Created on 25 de Março de 2017, 19:53
 */

#ifndef VDC_H
#define VDC_H
extern "C" {
  #include "extApi.h"
}

#include <string>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class VDC {
    friend class SKILLS;
public:
    
    VDC();
    virtual ~VDC();
    VDC(const VDC& orig);
    void setJointPosition(int joint, double angle);
    void conectJoints(std::string nameInVrep, int &nameInRemoteAPI);
    void conectProximitySensors(std::string nameInVrep, int &nameInRemoteAPI);
    void setClientID(int clientID);
    void setJointVelocity(int joint,float velocity);
    double getDistance(int sensor);
    void finish();
    bool connection_is_OK ();
    bool simulationIsActive ();
    void delay(int time);
    void CamilaCode(int Webcam);
    void getImageVisionSensor(int Webcam);
    void getOpencvImageVisionSensor(int Webcam);
    void setImageVisionSensor(int Webcam);
    void readVisionSensor(int cam);
    void opencvVisionInfo(int cam);
    int getClientID();
  
    
    
    
    
private:
    cv::Mat convertVrepToOpenCV(simxUChar image[], simxInt resX, simxInt resY);
    
    // clientID
    int clientID;
    
   
 
    
 

};

#endif  /* VDC_H */
    