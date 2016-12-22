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
  int leftMotorHandle = 0;
  float vLeft = 0;
  int rightMotorHandle = 0;
  float vRight = 0;
  string sensorNome[16];
  int sensorHandle[16];

  // variaveis de cena e movimentação do pioneer
  float noDetectionDist=0.5;
  float maxDetectionDist=0.2;
  float detect[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  float braitenbergL[16]={-0.2,-0.4,-0.6,-0.8,-1,-1.2,-1.4,-1.6, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  float braitenbergR[16]={-1.6,-1.4,-1.2,-1,-0.8,-0.6,-0.4,-0.2, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  float v0=2;

  // variaveis do papelao, o seguidor de linha
  int DynamicLeftJoint= 0;
  int DynamicRightJoint= 0;
/*int LeftJoint= 0;
  int RightJoint= 0;*/
  int Sensor[2];
//int rightSensor;
  float paul_walker= 3;
  float paul_walker_nao_morreu= 3;
  bool sensorReading[2]= {false,false};
//int cont= 0;

  // só pra organizar
  bool organizar= true;   // serve apenas para encolher as linhas de codigo do pioneer



  int clientID=simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);

  if (clientID!=-1)
  {
    cout << "Servidor conectado!" << std::endl;

    // inicialização dos motores
    if(simxGetObjectHandle(clientID,(const simxChar*) "Pioneer_p3dx_leftMotor",(simxInt *) &leftMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Handle do motor esquerdo nao encontrado!" << std::endl;
    else
      cout << "Conectado ao motor esquerdo!" << std::endl;


    if(simxGetObjectHandle(clientID,(const simxChar*) "Pioneer_p3dx_rightMotor",(simxInt *) &rightMotorHandle, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Handle do motor direito nao encontrado!" << std::endl;
    else
      cout << "Conectado ao motor direito!" << std::endl;



            // inicialização dos motores de papelao

      if(simxGetObjectHandle(clientID,(const simxChar*) "DynamicLeftJoint#0",(simxInt *) &DynamicLeftJoint, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
       cout << "DynamicLeftJoint  do papelao nao encontrado!" << std::endl;
    else
       cout << "DynamicLeftJoint do papelao encontrado!"<< std::endl;

    if(simxGetObjectHandle(clientID,(const simxChar*) "DynamicRightJoint#0",(simxInt *) &DynamicRightJoint, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
       cout << "DynamicRightJoint do papelao nao encontrado!" << std::endl;
    else
       cout << "DynamicRightJoint do papelao encontrado!"<< std::endl;


                            // AINDA NÃO SEI PARA QUE SERVE, SE SERVE DE ALGO           ('-')
/*    if(simxGetObjectHandle(clientID,(const simxChar*) "LeftJoint#0",(simxInt *) &LeftJoint, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
       cout << "LeftJoint  do papelao nao encontrado!" << std::endl;
    else
       cout << "LeftJoint do papelao encontrado!"<< std::endl;

    if(simxGetObjectHandle(clientID,(const simxChar*) "RightJoint#0",(simxInt *) &RightJoint, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
       cout << "RightJoint  do papelao nao encontrado!" << std::endl;
    else
       cout << "RightJoint do papelao encontrado!"<< std::endl;
*/



      // inicializando sensores de papelao

      if(simxGetObjectHandle(clientID,(const simxChar*) "LeftSensor#0" ,(simxInt *) &Sensor[0], (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        cout << "Sensor esquerdo do papelao nao encotrado !!" << std::endl;
      else
      {
      cout << "Sensor esquerdo do papelao encotrado !!" <<std::endl;
      simxReadVisionSensor(clientID,Sensor[0],NULL,NULL,NULL,simx_opmode_streaming);
      }


      if(simxGetObjectHandle(clientID,(const simxChar*) "RightSensor#0" ,(simxInt *) &Sensor[1], (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        cout << "Sensor direito do papelao nao encotrado !!" << std::endl;
      else
      {
        cout << "Sensor direito do papelao encotrado !!" <<std::endl;
        simxReadVisionSensor(clientID,Sensor[1],NULL,NULL,NULL,simx_opmode_streaming);
      }


    // inicialização dos sensores (remoteApi)
    for(int i = 0; i < 16; i++)
    {
      sensorNome[i] = "Pioneer_p3dx_ultrasonicSensor" + to_string(i + 1);

      if(simxGetObjectHandle(clientID,(const simxChar*) sensorNome[i].c_str(),(simxInt *) &sensorHandle[i], (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
	cout << "Handle do sensor " << sensorNome[i] << " nao encontrado!" << std::endl;
      else
      {
        cout << "Conectado ao sensor " << sensorNome[i] << std::endl;
	simxReadProximitySensor(clientID,sensorHandle[i],NULL,NULL,NULL,NULL,simx_opmode_streaming);
      }
    }

    // desvio e velocidade do robô
    while(simxGetConnectionId(clientID)!=-1) // enquanto a simulação estiver ativa
    {

    if(organizar)
    {
      for(int i = 0; i < 16; i++)
      {
	simxUChar state;
	simxFloat coord[3];

	if (simxReadProximitySensor(clientID,sensorHandle[i],&state,coord,NULL,NULL,simx_opmode_buffer)==simx_return_ok)
	{
	  float dist = coord[2];
	  if(state > 0 && (dist<noDetectionDist))
	  {
	    if(dist<maxDetectionDist)
	    {
	      dist=maxDetectionDist;
	    }

	    detect[i]=1-((dist-maxDetectionDist)/(noDetectionDist-maxDetectionDist));
	  }
	  else
	    detect[i] = 0;
	}
	else
	  detect[i] = 0;
      }

      vLeft = v0;
      vRight = v0;

      for(int i = 0; i < 16; i++)
      {
        vLeft=vLeft+braitenbergL[i]*detect[i];
        vRight=vRight+braitenbergR[i]*detect[i];
      }

      // atualiza velocidades dos motores
      simxSetJointTargetVelocity(clientID, leftMotorHandle, (simxFloat) vLeft, simx_opmode_streaming);
      simxSetJointTargetVelocity(clientID, rightMotorHandle, (simxFloat) vRight, simx_opmode_streaming);
      }


                                                        // THE TIME OF PAPELAO

    paul_walker=3;
    paul_walker_nao_morreu=3;
    for(int j=0;j<2;j++)
    {
        simxFloat* auxValues;
        simxInt* auxValuesCount;

        sensorReading[j]=false;


            if(simxReadVisionSensor(clientID,Sensor[j],NULL,&auxValues,&auxValuesCount,simx_opmode_buffer)==simx_return_ok);
            {
              if(auxValues[11]<0.3)
                {
                    sensorReading[j]=true;
                }

            }

    }
     if(sensorReading[0])
        {
        paul_walker=0.9;
        paul_walker_nao_morreu=10;

        }

     if(sensorReading[1])
     {
        paul_walker=10;
        paul_walker_nao_morreu=0.9;
     }


      simxSetJointTargetVelocity(clientID, DynamicLeftJoint, (simxFloat) paul_walker, simx_opmode_streaming);
      simxSetJointTargetVelocity(clientID, DynamicRightJoint, (simxFloat) paul_walker_nao_morreu, simx_opmode_streaming);






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
