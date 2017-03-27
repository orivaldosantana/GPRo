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
    
    
    
    
    
    
    
private:
    
    // clientID
    int clientID;
    
   
 
    
 

};

#endif  /* VDC_H */
    