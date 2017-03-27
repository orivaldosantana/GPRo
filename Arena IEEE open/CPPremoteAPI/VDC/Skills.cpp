#include <Skills.h>
#include <Utilities.h>

VDC vdc;

SKILLS::SKILLS() {
    sensor[8] = {};
    joint[9] = {};
    angle[5] = {};



}

void SKILLS::connectToRobot(int clientID) {






    vdc.setClientID(clientID);

    /* inicialização dos motores do lado esquerdo 
    NOME DO JOIN MOTOR  \/ AONDE ELE VAI GUARDAR \/                               */
    vdc.conectJoints("Motor_esquerdo_Tras", joint[1]);
    vdc.conectJoints("Motor_esquerdo_frente", joint[3]);


    // inicialização dos motores do lado direito 
    vdc.conectJoints("Motor_direito_frente", joint[0]);
    vdc.conectJoints("Motor_direito_Tras", joint[2]);



    //  juntas da GARRA
    for (int i = 4; i < 9; i++) {
        vdc.conectJoints("Joint#" + std::to_string(i - 3), joint[i]);


    }

    // inicialização dos sensores de proximidade  (remoteApi)
    for (int i = 0; i < 8; i++) {
        vdc.conectProximitySensors("Proximity_sensor_" + std::to_string(i + 1), sensor[i]);
    }






}

bool SKILLS::seguidorDeParede(int clientID) {
    //joint#1 == 4| joint#2 == 5 | joint#3 == 6 ...
    float velocityLeft = 0.75;
    float velocityRight = 0.75;
    float dist[8];
    vdc.setClientID(clientID);

    for (int i = 0; i < 8; i++) {

        dist[i] = vdc.getDistance(sensor[i]);

    }
    
    if(dist[2] < 0.2){
        velocityLeft = 2.0*degree(15);
        velocityRight =1.75;
        
    }
    if (dist[2] > 0.22 && dist[2]!= 500){
        velocityLeft = 1.75;
        velocityRight =2.0*degree(15);
                
    }
    
    if (dist[4] < 0.33){
        velocityLeft =-1;
        velocityRight =1;
        
    }

    if(dist[0] <0.25 && dist[2] < 0.31 && dist[7] < 0.10){
        velocityLeft  = 0.35;
        velocityRight = 0.25;
        
        if(angle[2] > -degree(5)){
            angle[2]-=degree(1.25);
            vdc.setJointPosition(joint[6],angle[2]);
        }
        
          else if(angle[4] <degree(65)){
                angle[4]+=degree(2.5);
                vdc.setJointPosition(joint[8],angle[4]);    
            }
        
        if(dist[7] <= 0.045)
            return true;
    
            
       
              
    }
    
    std::cout <<"dist[0]: " << dist[0] << " dist[2]: " << dist[2] << " dist[7]: " << dist[7] << std::endl;

    // atualiza velocidades dos motores
    vdc.setJointVelocity(joint[0], velocityRight);
    vdc.setJointVelocity(joint[2], velocityRight);

    vdc.setJointVelocity(joint[1], velocityLeft);
    vdc.setJointVelocity(joint[3], velocityLeft);
    //*/

    return false;




}

/*
void SKILLS::setJoints(int joint){
    this->joint[numbJoints] = joint;
    this->numbJoints += 1;
    
    
}

void SKILLS::setSensor(int sensor){
    this->sensor[numbSensors] = sensor;
    this->numbSensors += 1;
    
}

void SKILLS::setNumbAngles(){
    this->angle[numbAngles] = 0;
    this->numbAngles ++;
    
}
 */

void SKILLS::testJunta(int clientID) {

    vdc.setClientID(clientID);

    for (int i = 4; i < 9; i++) {
        if (i == 5 && angle[1] < 0.030) //Joint#2 movimento linear
        {
            angle[1] += 0.0003;
            vdc.setJointPosition(joint[i], angle[1]);

        } else if (i == 6 && angle[2] < degree(90) && angle[1] >= 0.030) // Joint#3
        {
            angle[2] += 0.01;
            vdc.setJointPosition(joint[i], angle[2]);
        } else if (i == 4 && angle[0]>-degree(90) && angle[4] >= 0.030 && angle[2] >= degree(90)) //joint#1
        {
            angle[0] -= 0.003;
            vdc.setJointPosition(joint[i], angle[0]);

        }
        //
        //joint#5

        if (i == 8) {
            angle[4] += 0.01;
            vdc.setJointPosition(joint[8], angle[4]);

        }





    }
}