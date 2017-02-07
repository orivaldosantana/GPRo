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

using namespace std;

int main(int argc, char **argv)
{
  string serverIP = "127.0.0.1";
  int serverPort = 19999;
  int motor_esquerdo;
  int motor_direito;
  int sensor[2];
  float velocidade_motor_esquerdo;
  float velocidade_motor_direito;
  simxUChar detectionState;
  simxFloat* auxValues;
  simxInt* auxValuesCount;






  int clientID=simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);

  if (clientID!=-1)
  {
    cout << "Servidor conectado!" << std::endl;

    // inicialização das peças
    if(simxGetObjectHandle(clientID,(const simxChar*) "LeftSensor",(simxInt *) &sensor[0], (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "sensor esquerdo nao encontrado!" << std::endl;
    else
      cout << "Conectado ao sensor esquerdo!" << std::endl;

    if(simxGetObjectHandle(clientID,(const simxChar*) "RightSensor",(simxInt *) &sensor[1], (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "sensor direito nao encontrado!" << std::endl;
    else
      cout << "Conectado ao sensor direito!" << std::endl;


    if(simxGetObjectHandle(clientID,(const simxChar*) "DynamicLeftJoint",(simxInt *) &motor_esquerdo, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "motor esquerdo nao encontrado!" << std::endl;
    else
      cout << "Conectado ao motor esquerdo!" << std::endl;

    if(simxGetObjectHandle(clientID,(const simxChar*) "DynamicRightJoint",(simxInt *) &motor_direito, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "motor direito nao encontrado!" << std::endl;
    else
      cout << "Conectado ao motor direito!" << std::endl;









    while(simxGetConnectionId(clientID)!=-1) // enquanto a simulação estiver ativa
    {
        velocidade_motor_esquerdo  = 2;
        velocidade_motor_direito   = 2;






        for(int i=0;i<2;i++)
        {
          simxReadVisionSensor(clientID,sensor[i],NULL,NULL,NULL,simx_opmode_streaming);

          simxReadVisionSensor(clientID,sensor[i],&detectionState,&auxValues,&auxValuesCount,simx_opmode_buffer);
              if(auxValues[11]<0.3)
              {
                      if(i==0)
                      {
                        velocidade_motor_esquerdo =1;
                        velocidade_motor_direito  =2.5;
                      }
                      else
                        {
                          velocidade_motor_direito  =1;
                          velocidade_motor_esquerdo =2.5;
                        }
              }
            


        }
      // atualiza velocidades dos motores
      simxSetJointTargetVelocity(clientID, motor_esquerdo, (simxFloat) velocidade_motor_esquerdo, simx_opmode_streaming);
      simxSetJointTargetVelocity(clientID, motor_direito, (simxFloat) velocidade_motor_direito, simx_opmode_streaming);

      // espera um pouco antes de reiniciar a leitura dos sensores
      extApi_sleepMs(5);
    }

    simxFinish(clientID); // fechando conexao com o servidor
    cout << "Conexao fechada!" << std::endl;
  }
  else
    cout << "Problemas para conectar o servidor!" << std::endl;
  return 0;
}
