/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Vdc.cpp
 * Author: samuel
 * 
 * Created on 4 de Outubro de 2017, 10:12
 */

#include "Vdc.h"

Vdc::Vdc(const Vdc& orig) {
}

Vdc::Vdc(std::string serverIP, int serverPort) {
    this->clientID = simxStart((simxChar*) serverIP.c_str(), serverPort, true, true, 2000, 5);
    debug = true;
}

Vdc::~Vdc() {
}

void Vdc::conectpiece(std::string nameInVrep, int& nameInRemoteAPI) {

    if (simxGetObjectHandle(clientID, (const simxChar*) nameInVrep.c_str(), (simxInt *) & nameInRemoteAPI, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok) {
        if (debug)
            std::cout << nameInVrep << " nao encontrado!" << std::endl;

    } else {
        if (debug)
            std::cout << "Conectado ao " << nameInVrep << std::endl;
    }

}

void Vdc::setJointVelocity(int joint, float velocity) {
    simxSetJointTargetVelocity(clientID, joint, (simxFloat) velocity, simx_opmode_streaming);
}

template <class T>
T Vdc::degree(T Degree) {
    return (Degree * 3.14159265359) / 180;
}

template <class T>
void Vdc::setJointPosition(int joint, T angle) {
    angle = Vdc::degree(angle);
    simxSetJointPosition(clientID, joint, angle, simx_opmode_oneshot);
}

void Vdc::finish() {
    simxFinish(clientID);
}

bool Vdc::connection_is_OK() {
    if (clientID != -1)
        return true;
    else
        return false;
}

bool Vdc::simulationIsActive() {

    if (simxGetConnectionId(clientID) != -1)
        return true;
    else
        return false;

}

void Vdc::delay(int time) {
    extApi_sleepMs(time);

}

void Vdc::setclientID(std::string serverIP, int serverPort) {
    clientID = simxStart((simxChar*) serverIP.c_str(), serverPort, true, true, 2000, 5);
}

void Vdc::setDebug(bool _debug) {
    debug = _debug;
}

bool Vdc::readVisionSensor(int sensor, std::vector<float>& VectorAuxValues, int sizeOfauxValues) {
    simxUChar detectionState;
    simxFloat* auxValues;
    simxInt* auxValuesCount;
    float aux;

    simxReadVisionSensor(clientID, sensor, NULL, NULL, NULL, simx_opmode_streaming);

    if (simxReadVisionSensor(clientID, sensor, &detectionState, &auxValues, &auxValuesCount, simx_opmode_buffer) == simx_return_ok) {
        for (int i = 0; i < sizeOfauxValues; i++) {
            aux = (float) auxValues[i];
            
            VectorAuxValues.push_back(aux);
        }
        return true;
    }
    return false;

}

bool Vdc::readProximitySensor(int sensor, float &distance){
     simxFloat coord[3];
     simxUChar state;
     //std::cout << "oi " << std::endl;
      simxReadProximitySensor(clientID, sensor, NULL, NULL, NULL, NULL, simx_opmode_streaming);
       

    if (simxReadProximitySensor(clientID, sensor, &state, coord, NULL, NULL, simx_opmode_buffer) == simx_return_ok) {
        if (state > 0) {
            distance = (float)coord[2]; // coord[2]  == z
            return true;
        } 

    }
    return  false;

}