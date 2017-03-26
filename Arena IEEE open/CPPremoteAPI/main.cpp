/*
    Client of V-REP simulation server (remoteApi)
    Copyright (C) 2015  Rafael Alceste Berri rafaelberri@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Habilite o server antes na simulação V-REP com o comando lua:
// simExtRemoteApiStart(portNumber) -- inicia servidor remoteAPI do V-REP

extern "C" {
#include "remoteApi/extApi.h"
}

#include <iostream>
#include <string>
#include <VDC.h>


using namespace std;


VDC vdc;

int main(int argc, char **argv) {
    
    string serverIP = "127.0.0.1";
    int serverPort = 19999;
    int clientID = simxStart((simxChar*) serverIP.c_str(), serverPort, true, true, 2000, 5);
    string Joint;
    string sensorNome;
    int proximity_Sensor[8];
    int joint[9];
    
    
   
    
    
  
      
    
    


    // variaveis de cena e movimentação do mamador


    

    if (clientID != -1) {
        cout << "Servidor conectado!" << std::endl;
        
        vdc.setClientID(clientID);

        /* inicialização dos motores traseiros  
                      NOME DO JOIN MOTOR\/    AONDE ELE VAI GUARDAR \/                               */
        vdc.conectJoints("Motor_esquerdo_Tras", joint[0], clientID);
        vdc.conectJoints("Motor_direito_Tras", joint[1], clientID);

        // inicialização dos motores da frente        
        vdc.conectJoints("Motor_esquerdo_frente", joint[2], clientID);
        vdc.conectJoints("Motor_direito_frente", joint[3], clientID);

        //  juntas da GARRA
        for (int i = 4; i < 9; i++) {

            Joint = "Joint#" + to_string(i - 3);

        vdc.conectJoints(Joint, joint[i], clientID);


        }
       
        
          
        


        // inicialização dos sensores de proximidade  (remoteApi)
        for (int i = 0; i < 8; i++) {
            
            
               sensorNome = "Proximity_sensor_" + to_string(i + 1);

               vdc.conectProximitySensors(sensorNome, proximity_Sensor[i], clientID);  
               
            
        }
        
        for(int i=0;i<9;i++){
            vdc.setJoints(joint[i]);
            if(i<8){
                vdc.setSensors(proximity_Sensor[i]);
            }
            
        }
        
        
        
        

        while (simxGetConnectionId(clientID) != -1) // enquanto a simulação estiver ativa
        {

            
            extApi_sleepMs(50);
        }

        simxFinish(clientID); // fechando conexao com o servidor
        cout << "Conexao fechada!" << std::endl;
    } else
        cout << "Problemas para conectar o servidor!" << std::endl;
    return 0;
}
