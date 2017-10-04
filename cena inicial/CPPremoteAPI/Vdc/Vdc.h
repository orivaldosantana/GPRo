/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Vdc.h
 * Author: samuel
 *
 * Created on 4 de Outubro de 2017, 10:12
 */

#ifndef VDC_H
#define VDC_H
extern "C" {
  #include <extApi.h>
}
#include <string>
#include <iostream>
#include <vector>


class Vdc {
public:
    Vdc(std::string serverIP,int serverPort);
    void conectpiece(std::string nameInVrep, int &nameInRemoteAPI);
    void finish();
    bool connection_is_OK ();
    bool simulationIsActive ();
    void delay(int time);
    void setclientID(std::string serverIP,int serverPort);
    void setDebug(bool _debug);
    
    void setJointVelocity(int joint,float velocity);
    template <class T>
    void setJointPosition(int joint, T angle);
    bool readVisionSensor(int sensor , std::vector <float> &auxValues, int sizeOfauxValues);
    bool readProximitySensor(int sensor, float &distance);
   
    
    
    Vdc(const Vdc& orig);
    virtual ~Vdc();
private:
    template <class T>
     T degree(T Degree);
    
    int clientID;
    bool debug = false; //deputar tudo !!!
    
    
    
};

#endif /* VDC_H */
