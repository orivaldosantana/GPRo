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
  int Joint[8];
  float alfa;
  float beta;
  bool Esquerda_Direita = true;

  int clientID=simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);



  if (clientID!=-1)
  {
    string joints[2];

    cout << "Servidor conectado!" << std::endl;

    // Exmplo do codigo
    for(int i=0;i<8;i++){

      joints[0]= "joint "+ to_string(i+1);

      joints[1]= ""+to_string(i+1);

      if(simxGetObjectHandle(clientID,(const simxChar*) joints[1].c_str(),(simxInt *) &Joint[i], (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        cout << joints << " nao encontrado!" << std::endl;
      else
        cout << "Conectado a " << joints[0] << std::endl;


    }







    // desvio e velocidade do robô
    while(simxGetConnectionId(clientID)!=-1) // enquanto a simulação estiver ativa
    {  // 9° = 0.157079633  35° = 0.610865238



      alfa = 0.157079633;   //9°
      beta = 0.610865238;  // 35°


      if(Esquerda_Direita){

          // SUBIR !!!
        simxSetJointPosition(clientID,Joint[4],alfa, simx_opmode_oneshot);
        simxSetJointPosition(clientID,Joint[6],alfa, simx_opmode_oneshot);


        simxSetJointPosition(clientID,Joint[0],beta, simx_opmode_oneshot);
        simxSetJointPosition(clientID,Joint[2],beta, simx_opmode_oneshot);

        // DESCER...
        simxSetJointPosition(clientID,Joint[4],0, simx_opmode_oneshot);
        simxSetJointPosition(clientID,Joint[6],0, simx_opmode_oneshot);


        simxSetJointPosition(clientID,Joint[1],0, simx_opmode_oneshot);
        simxSetJointPosition(clientID,Joint[3],0, simx_opmode_oneshot);

        Esquerda_Direita =false;
      }


      else{
        // SUBIR !!!
      simxSetJointPosition(clientID,Joint[5],alfa, simx_opmode_oneshot);
      simxSetJointPosition(clientID,Joint[7],alfa, simx_opmode_oneshot);


      simxSetJointPosition(clientID,Joint[1],beta, simx_opmode_oneshot);
      simxSetJointPosition(clientID,Joint[3],beta, simx_opmode_oneshot);

      // DESCER...
      simxSetJointPosition(clientID,Joint[5],0, simx_opmode_oneshot);
      simxSetJointPosition(clientID,Joint[7],0, simx_opmode_oneshot);


      simxSetJointPosition(clientID,Joint[0],0, simx_opmode_oneshot);
      simxSetJointPosition(clientID,Joint[2],0, simx_opmode_oneshot);

        Esquerda_Direita = true;
      }







      // espera um pouco antes de reiniciar
      extApi_sleepMs(5);

    }

    simxFinish(clientID); // fechando conexao com o servidor
    cout << "Conexao fechada!" << std::endl;
  }
  else
    cout << "Problemas para conectar o servidor!" << std::endl;
  return 0;
}
