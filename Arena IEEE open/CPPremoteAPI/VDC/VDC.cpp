extern "C" {
#include "extApi.h"
}
#include <VDC.h>





 VDC::VDC(){
 }
 
VDC::VDC(const VDC& orig){
}

void VDC::conectJoints(std::string nameInVrep, int &nameInRemoteAPI, int clientID) {

    if (simxGetObjectHandle(clientID, (const simxChar*) nameInVrep.c_str(), (simxInt *) & nameInRemoteAPI, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        std::cout << nameInVrep << " nao encontrado!" << std::endl;
    else
        std::cout << "Conectado ao " << nameInVrep << std::endl;
}

void VDC::conectProximitySensors(std::string nameInVrep, int &nameInRemoteAPI, int clientID) {


    if (simxGetObjectHandle(clientID, (const simxChar*) nameInVrep.c_str(), (simxInt *) & nameInRemoteAPI, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        std::cout << nameInVrep << " nao encontrado!" << std::endl;
    else {
        std::cout << "Conectado ao " << nameInVrep << std::endl;
        simxReadProximitySensor(clientID, nameInRemoteAPI, NULL, NULL, NULL, NULL, simx_opmode_streaming);

    }

}

void VDC::seguidorDeParede(){
    
    
}

void VDC::setClientID(int clientID) {
    this->clientID = clientID;
}

void VDC::setJoints(int setJoint) {
    this->numbJoints +=1;
    this->joint[numbJoints] = setJoint;
}

void VDC::setSensors(int setSensor) {
    this->numbSensors +=1;
    this->sensors[numbSensors] = setSensor;
}


VDC::~VDC(){
    
}



/*
            vLeft = 2;
            vRight = 1;

            for (int i = 0; i < 6; i++) {
                simxUChar state;
                simxFloat coord[3]; //cordenada x,y,z


                // LENDO OS SENSORES             \/ Variavel que você declarou
                if (simxReadProximitySensor(clientID, proximity_sensor[i], &state, coord, NULL, NULL, simx_opmode_buffer) == simx_return_ok) {
                    float dist = coord[2]; //    no caso, pegamos as informações da cordenada   Z


                    if (state > 0) // State fica >0  se o sensor achou algo
                    { // State ficar <0  deu ruim
                        // State fica 0 quando não acha nada







                        if (i > 4 && dist <= ((+0.335576) / 5)) // IMPAR== DIREITO
                        {
                            vLeft = 0;
                            vRight = 0;


                        } else {
                            vLeft = 2;
                            vRight = 2;

                        }


                    }


                }

            }
             
               
             simxGetVisionSensorImage(clientID,Webcam,simxInt* resolution,simxUChar** image,simxUChar options,simxInt operationMode)
            int resolution[2];
            resolution[0]=1280;
            resolution[1]=720;
            char **image=NULL;                      //ENTENDER
            char options;

            if (simxGetVisionSensorImage(clientID,Webcam,resolution,(simxUChar**)image,(simxUChar)options,simx_opmode_buffer) )
            {

                cout<<"ok"<<endl;

            }

             

            // vez da Garra

            simxUChar state;
            simxFloat coord[3];

            if (simxReadProximitySensor(clientID, Garra_sensor, &state, coord, NULL, NULL, simx_opmode_buffer) == simx_return_ok) {
                if (state > 0) {

                    if (coord[2] <= 0.0413 && angulo < 0.700411169) {
                        angulo++;
                        simxSetJointPosition(clientID, joint[2], angulo, simx_opmode_oneshot);

                    }


                }



            }

            for (int i = 0; i < 5; i++) {
                if (i == 1 && angulo3 < 0.030) //Joint#2
                {
                    angulo3 += 0.0003;
                    simxSetJointPosition(clientID, joint[i], angulo3, simx_opmode_oneshot);

                } else if (i == 2 && angulo < 1.57079633 && angulo3 >= 0.030) // Joint#3
                {
                    angulo += 0.01;
                    simxSetJointPosition(clientID, joint[i], angulo, simx_opmode_oneshot);
                } else if (i == 0 && angulo2>-1.57079633 && angulo3 >= 0.030 && angulo >= 1.57079633) //joint#1
                {
                    angulo2 -= 0.001 + cont;
                    simxSetJointPosition(clientID, joint[i], angulo2, simx_opmode_oneshot);
                    if (cont < 0.003)
                        cont += 0.001;
                }
                //
                if (i == 4) //joint#5
                {
                    angulo4 += 0.01;
                    simxSetJointPosition(clientID, joint[i], angulo4, simx_opmode_oneshot);
                }



            }














            // atualiza velocidades dos motores
            simxSetJointTargetVelocity(clientID, motor_tras_esquerdo, (simxFloat) vLeft, simx_opmode_streaming);
            simxSetJointTargetVelocity(clientID, motor_tras_direito, (simxFloat) vRight, simx_opmode_streaming);
            simxSetJointTargetVelocity(clientID, motor_frente_esquerdo, (simxFloat) vLeft, simx_opmode_streaming);
            simxSetJointTargetVelocity(clientID, motor_frente_direito, (simxFloat) vRight, simx_opmode_streaming);
            //*/




// espera um pouco antes de reiniciar a leitura dos sensores