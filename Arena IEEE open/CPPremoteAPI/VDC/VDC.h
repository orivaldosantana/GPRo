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

#include <string>
#include <iostream>

class VDC {
public:
    
    VDC();
    virtual ~VDC();
    VDC(const VDC& orig);
    
    void conectJoints(std::string nameInVrep, int &nameInRemoteAPI, int clientID);
    void conectProximitySensors(std::string nameInVrep, int &nameInRemoteAPI, int clientID);
    void seguidorDeParede();
    void setClientID(int clientID);
    void setJoints(int setJoint);
    void setSensors(int setSensor);
    
    
    
    
    
    
private:
    
    // clientID
    int clientID;
    
    // MOTORES e juntas
    int numbJoints =0;
    int * joint  = new int [numbJoints];
    
    //GARRA e sensores
    int numbSensors=0;
    int* sensors = new int[numbSensors]; 
    
    
    //Sensor vision
    int Webcam;
    // Sensores de 

};

#endif  /* VDC_H */
    