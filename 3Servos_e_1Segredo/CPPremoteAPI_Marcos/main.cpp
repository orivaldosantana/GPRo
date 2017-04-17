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
#include <fstream>

using namespace std;


    
       
  

int main(int argc, char **argv)
{
  string fileName = "Dadossamuel2.csv";
  string aux;  
  string serverIP = "127.0.0.1";
  int serverPort = 19999;
  int clientID=simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);

  int ServoMiddle;
  int ServoLeft;
  int ServoRight;
  double theta1, theta2, theta3;
  ifstream file(fileName.c_str());

  if (clientID!=-1)
  {
    cout << "Servidor conectado!" << std::endl;

    // Exmplo do codigo
    if(simxGetObjectHandle(clientID,(const simxChar*) "ServoMiddle",(simxInt *) &ServoMiddle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "ServoMiddle nao encontrado!" << std::endl;
    else
      cout << "Conectado ao ServoMiddle!" << std::endl;
    
    if(simxGetObjectHandle(clientID,(const simxChar*) "ServoLeft",(simxInt *) &ServoLeft, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "ServoLeft nao encontrado!" << std::endl;
    else
      cout << "Conectado ao ServoLeft!" << std::endl;
    
    if(simxGetObjectHandle(clientID,(const simxChar*) "ServoRight",(simxInt *) &ServoRight, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "ServoRight nao encontrado!" << std::endl;
    else
      cout << "Conectado ao ServoRight!" << std::endl;





    
    while(simxGetConnectionId(clientID)!=-1) // enquanto a simulação estiver ativa
    {
        if(file.good()){
          getline(file,aux, ',');
          theta1 = std::stod(aux);
          cout << "theta1: " << theta1 << std::endl;
          
          
          getline(file,aux, ',');
          theta2 = std::stod(aux); 
          cout << "theta2: " << theta2 << std::endl;
          
          getline(file,aux, ',');
          theta3 = std::stod(aux);
           cout << "theta3: " << theta3 << std::endl;
             
        }
        
        simxSetJointPosition(clientID,ServoMiddle,theta1, simx_opmode_oneshot);
        simxSetJointPosition(clientID,ServoLeft,theta2, simx_opmode_oneshot);
        simxSetJointPosition(clientID,ServoRight,theta3, simx_opmode_oneshot);


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
