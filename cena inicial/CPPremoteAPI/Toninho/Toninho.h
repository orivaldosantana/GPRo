/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Toninho.h
 * Author: samuel
 *
 * Created on 4 de Outubro de 2017, 10:43
 */

#ifndef TONINHO_H
#define TONINHO_H

class Toninho {
public:


    virtual bool detectedByLeft() = 0;
    virtual bool detectedByRight() = 0;
    virtual bool getDistanceOfProximitySensor(float &distance) = 0;
    virtual void setVelocityInToninho(float rightMotor, float leftMotor) =0;
    virtual void seguirLinha() = 0;
    virtual bool loop() =0 ;
    virtual void delay(int time) = 0;
    // virtual void desviarObstaculo() =0;




private:

};

#endif /* TONINHO_H */

