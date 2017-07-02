extern "C" {
#include "extApi.h"
}
#include <VDC.h>


VDC::VDC() {
    

}

VDC::VDC(const VDC& orig) {
}

void VDC::conectJoints(std::string nameInVrep, int &nameInRemoteAPI) {

    if (simxGetObjectHandle(clientID, (const simxChar*) nameInVrep.c_str(), (simxInt *) & nameInRemoteAPI, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        std::cout << nameInVrep << " nao encontrado!" << std::endl;
    else
        std::cout << "Conectado ao " << nameInVrep << std::endl;
    
    
    
    

  
}

void VDC::conectProximitySensors(std::string nameInVrep, int &nameInRemoteAPI) {


    if (simxGetObjectHandle(clientID, (const simxChar*) nameInVrep.c_str(), (simxInt *) & nameInRemoteAPI, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        std::cout << nameInVrep << " nao encontrado!" << std::endl;
    else {
        std::cout << "Conectado ao " << nameInVrep << std::endl;
        simxReadProximitySensor(clientID, nameInRemoteAPI, NULL, NULL, NULL, NULL, simx_opmode_streaming);

    }

 
}



void VDC::setClientID(int clientID) {
    this->clientID = clientID;
}





double VDC::getDistance(int sensor) {
    simxUChar state;
    simxFloat coord[3];

    if (simxReadProximitySensor(clientID, sensor, &state, coord, NULL, NULL, simx_opmode_buffer) == simx_return_ok) {
        if (state > 0) {
            if (coord[2] > 0)
                return coord[2];


        }
        else if (state == 0)
            return 500;

    }
    return -1;



}

void VDC::setJointPosition(int joint, double angle){
    simxSetJointPosition(clientID, joint, angle, simx_opmode_oneshot);
    
}

void VDC::setJointVelocity(int joint,float velocity){
    simxSetJointTargetVelocity(clientID, joint, (simxFloat)velocity, simx_opmode_streaming);
    
    
}

void VDC::finish(){
    simxFinish(clientID);
}

bool VDC::connection_is_OK(){
    
    if (clientID !=-1)
        return true;
    else
        return false;
}

bool VDC::simulationIsActive(){
    
    if (simxGetConnectionId(clientID)!=-1)
        return true;
    else
        return false;
    
    
}
void VDC::delay(int time){
    
     extApi_sleepMs(time);
}


int VDC::getClientID(){
    return this->clientID;
}

void VDC::getImageVisionSensor(int Webcam){
   // simxInt clientID;
   
    simxInt* resolution;
    simxUChar** image;
 
    simxGetVisionSensorImage(clientID,Webcam,resolution,image,NULL,simx_opmode_streaming );
    simxGetVisionSensorImage(clientID,Webcam,resolution,image,NULL,simx_opmode_buffer );
   
}
void VDC::setImageVisionSensor(int Webcam){
    
    simxUChar* image;
    simxInt bufferSize;
    simxUChar options;
    simxInt operationMode;
    simxSetVisionSensorImage(clientID,Webcam,image,bufferSize,NULL,simx_opmode_oneshot);
}

VDC::~VDC() {

}

