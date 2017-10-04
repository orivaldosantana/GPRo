/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ToninhoVrep.h
 * Author: samuel
 *
 * Created on 4 de Outubro de 2017, 12:52
 */

#ifndef TONINHOVREP_H
#define TONINHOVREP_H
#include <Toninho.h>
#include <Vdc.h>

class ToninhoVrep : public Toninho {
public:
    ToninhoVrep(std::string serverIP, int serverPort);
    ToninhoVrep(const ToninhoVrep& orig);
    virtual ~ToninhoVrep();
    bool detectedByLeft();
    bool detectedByRight();
    bool getDistanceOfProximitySensor(float &distance);
    void setVelocityInToninho(float rightMotor, float leftMotor);
    void seguirLinha();
    bool loop ();
    void delay(int time);
    
private:
    void conectRobot();
    Vdc *Vrep;
    int leftMotor;
    int rightMotor;
    int leftSensor;
    int rightSensor;
    int proximitySensor;

};

#endif /* TONINHOVREP_H */

