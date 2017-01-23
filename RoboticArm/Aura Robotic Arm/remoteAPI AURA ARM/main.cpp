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

  // JUNTAS
  int Joint1;
  int Joint2;
  int Joint3;

/*GARRA                   NÃO SEI COMO USAR !!
  int Garra; */
  //Sensor vision
  int Webcam;




  // variaveis de cena e movimentação do mamador


  int clientID=simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);

  if (clientID!=-1)
  {
    cout << "Servidor conectado!" << std::endl;

    // inicialização das juntas
    if(simxGetObjectHandle(clientID,(const simxChar*) "Joint#1",(simxInt *) &Joint1, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Joint#1 nao encontrado!" << std::endl;
    else
      cout << "Conectado ao Joint#1!" << std::endl;

    if(simxGetObjectHandle(clientID,(const simxChar*) "Joint#2",(simxInt *) &Joint2, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Joint#2 nao encontrado!" << std::endl;
    else
      cout << "Conectado ao Joint#2!" << std::endl;

      // inicialização dos motores da frente
    if(simxGetObjectHandle(clientID,(const simxChar*) "Join#3",(simxInt *) &Joint3, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Joint#3 nao encontrado!" << std::endl;
    else
      cout << "Conectado ao Joint#3!" << std::endl;



                // camera
    if(simxGetObjectHandle(clientID,(const simxChar*) " Webcam ",(simxInt *) &Webcam, (simxInt) simx_opmode_streaming) != simx_return_ok)
      cout << "Webcam nao encontrado!" << std::endl;
    else
      cout << "Conectado ao Webcam" << std::endl;







    while(simxGetConnectionId(clientID)!=-1) // enquanto a simulação estiver ativa
    {
        /*        FUNÇOES PÁRA DAR UMA OLHADA !!

        simxGetJointForce:
    simxInt simxGetJointForce(simxInt clientID,simxInt jointHandle,simxFloat* force,simxInt operationMode)

        simxGetJointMatrix:
    simxInt simxGetJointMatrix(simxInt clientID,simxInt jointHandle,simxFloat* matrix,simxInt operationMode)

        simxGetJointPosition:
    simxInt simxGetJointPosition(simxInt clientID,simxInt jointHandle,simxFloat* position,simxInt operationMode)

        simxSetJointForce:
    simxInt simxSetJointForce(simxInt clientID,simxInt jointHandle,simxFloat force,simxInt operationMode)

        simxSetJointPosition:
    simxInt simxSetJointPosition(simxInt clientID,simxInt jointHandle,simxFloat position,simxInt operationMode)

        simxSetJointTargetPosition
    simxInt simxSetJointTargetPosition(simxInt clientID,simxInt jointHandle,simxFloat targetPosition,simxInt operationMode)




*/
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
