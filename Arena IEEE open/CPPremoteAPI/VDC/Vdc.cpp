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

bool Vdc::conectpiece(std::string nameInVrep, int& nameInRemoteAPI) {

    if (simxGetObjectHandle(clientID, (const simxChar*) nameInVrep.c_str(), (simxInt *) & nameInRemoteAPI, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok) {
        if (debug)
            std::cout << nameInVrep << " nao encontrado!" << std::endl;
        nameInRemoteAPI = -1;
        return false;
    } else {
        if (debug)
            std::cout << "Conectado ao " << nameInVrep << std::endl;

        return true;
    }

}

void Vdc::setJointVelocity(int joint, float velocity) {
    simxSetJointTargetVelocity(clientID, joint, (simxFloat) velocity, simx_opmode_streaming);
}

template <class T>
T Vdc::degree(T Degree) {
    return (Degree * 3.14159265359) / 180;
}

void Vdc::setJointPosition(int joint, double angle) {
    //    angle = Vdc::degree(angle);  caso queria setar em graus ao invez de radianos descomente
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

bool Vdc::readProximitySensor(int sensor, float &distance) {
    simxFloat coord[3];
    simxUChar state;
    //std::cout << "oi " << std::endl;
    simxReadProximitySensor(clientID, sensor, NULL, NULL, NULL, NULL, simx_opmode_streaming);


    if (simxReadProximitySensor(clientID, sensor, &state, coord, NULL, NULL, simx_opmode_buffer) == simx_return_ok) {
        if (state > 0) {
            distance = (float) coord[2]; // coord[2]  == z
            return true;
        }

    }
    return false;

}

int Vdc::getClientID() {
    return clientID;
}

bool Vdc::getObjectPosition(int object, float& x, float& y, float& z) {

    simxFloat position[3];
    
    int ok = simxGetObjectPosition(clientID, object, -1, position, simx_opmode_streaming);

    while (ok == simx_return_novalue_flag) {

        ok = simxGetObjectPosition(clientID, object, -1, position, simx_opmode_buffer);
    }
    
    if ( ok !=simx_return_ok )
        return false;
    
    x = (float) position[0];
    y = (float) position[1];
    z = (float) position[2];
    
    return true;

    //std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;

}

void Vdc::debugerOfReturns(int simxreturn) {

    switch (simxreturn) {
        case simx_return_novalue_flag:
            std::cout << "erro 1" << std::endl;
            break;

        case simx_return_timeout_flag:
            std::cout << "erro 2" << std::endl;
            break;

        case simx_return_illegal_opmode_flag:
            std::cout << "erro 3" << std::endl;
            break;

        case simx_return_remote_error_flag:
            std::cout << "erro 4" << std::endl;
            break;

        case simx_return_split_progress_flag:
            std::cout << "erro 5" << std::endl;
            break;

        case simx_return_local_error_flag:
            std::cout << "erro 6" << std::endl;
            break;

        case simx_return_initialize_error_flag:
            std::cout << "erro 7 " << std::endl;
            break;
    }

    //          simx_return_ok				=0x000000,
    //		simx_return_novalue_flag		=0x000001,		/* input buffer doesn't contain the specified command. Maybe you forgot to enable data streaming, or streaming hasn't started yet */
    //		simx_return_timeout_flag		=0x000002,		/* command reply not received in time for simx_opmode_oneshot_wait operation mode */
    //		simx_return_illegal_opmode_flag		=0x000004,		/* command doesn't support the specified operation mode */
    //		simx_return_remote_error_flag		=0x000008,		/* command caused an error on the server side */
    //		simx_return_split_progress_flag		=0x000010,		/* previous similar command not yet fully processed (applies to simx_opmode_oneshot_split operation modes) */
    //		simx_return_local_error_flag		=0x000020,		/* command caused an error on the client side */
    //		simx_return_initialize_error_flag	=0x000040	


}

void Vdc::setObjectPosition(int object, float x, float y, float z){
    const simxFloat position[3] = {x,y,z};
    simxSetObjectPosition(clientID,object ,-1,position,simx_opmode_oneshot);    
}

bool Vdc::getJointPosition(int joint, float& position){
    
    int ok =  simxGetJointPosition(clientID, joint, &position, simx_opmode_streaming);
    
    while (ok == simx_return_novalue_flag) {

        ok =  simxGetJointPosition(clientID, joint, &position, simx_opmode_streaming);
    }
    if ( ok != simx_return_ok )
        return false;
    
    return true;
    
    
}
