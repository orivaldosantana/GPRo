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
  float vLeft = 0;
  float vRight = 0;
  string sensorNome[6];
  int proximity_sensor[6];
  // MOTORES
  int motor_tras_direito = 0;
  int motor_tras_esquerdo = 0;
  int motor_frente_direito =0;
  int motor_frente_esquerdo =0;
  //GARRA
  string Joint[5];
  int joint[5];
  int Garra_sensor;
  float angulo = 0;
  float angulo2 =0;
  float angulo3 =0;
  float angulo4 =0;
  //Sensor vision
    int Webcam;




  // variaveis de cena e movimentação do mamador


  int clientID=simxStart((simxChar*)serverIP.c_str(),serverPort,true,true,2000,5);

  if (clientID!=-1)
  {
    cout << "Servidor conectado!" << std::endl;

    // inicialização dos motores traseiros           NOME DO JOIN MOTOR\/    AONDE ELE VAI GUARDAR \/
    if(simxGetObjectHandle(clientID,(const simxChar*) "Motor_esquerdo_Tras",(simxInt *) &motor_tras_esquerdo, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Motor_esquerdo_Tras nao encontrado!" << std::endl;
    else
      cout << "Conectado ao Motor_esquerdo_Tras!" << std::endl;

    if(simxGetObjectHandle(clientID,(const simxChar*) "Motor_direito_Tras",(simxInt *) &motor_tras_direito, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Motor_direito_Tras nao encontrado!" << std::endl;
    else
      cout << "Conectado ao Motor_direito_Tras!" << std::endl;

      // inicialização dos motores da frente
    if(simxGetObjectHandle(clientID,(const simxChar*) "Motor_esquerdo_frente",(simxInt *) &motor_frente_esquerdo, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Motor_esquerdo_Frente nao encontrado!" << std::endl;
    else
      cout << "Conectado ao Motor_esquerdo_Frente!" << std::endl;

    if(simxGetObjectHandle(clientID,(const simxChar*) "Motor_direito_frente",(simxInt *) &motor_frente_direito, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Motor_direito_Frente nao encontrado!" << std::endl;
    else
      cout << "Conectado ao Motor_direito_Frente!" << std::endl;



        // GARRA
for(int i=0; i<5 ;i++)
{

Joint[i] = "Joint#" + to_string(i+1);

    if(simxGetObjectHandle(clientID,(const simxChar*) Joint[i].c_str(),(simxInt *) &joint[i], (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
      cout << "Joint#"<< i << " não encontrado" << std::endl;
    else
      cout << "Conectado ao Joint#"<< i+1 << std::endl;


}


   if(simxGetObjectHandle(clientID,(const simxChar*) "Garra_sensor",(simxInt *) &Garra_sensor, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
     cout << "Garra_sensor nao encontrado!" << endl;
   else
     {
       cout << "Conectado ao Garra_sensor " << endl;
       simxReadProximitySensor(clientID,Garra_sensor,NULL,NULL,NULL,NULL,simx_opmode_streaming);
     }









                // camera
    if(simxGetObjectHandle(clientID,(const simxChar*) " Webcam ",(simxInt *) &Webcam, (simxInt) simx_opmode_streaming) != simx_return_ok)
      cout << "Webcam nao encontrado!" << std::endl;
    else
      cout << "Conectado ao Webcam" << std::endl;




    // inicialização dos sensores (remoteApi)
    for(int i = 0; i < 6; i++)
    {
      sensorNome[i] = "Proximity_sensor_" + to_string(i + 1);

      if(simxGetObjectHandle(clientID,(const simxChar*) sensorNome[i].c_str(),(simxInt *) &proximity_sensor[i], (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
	cout << "Proximity_sensor_" << sensorNome[i] << " nao encontrado!" << std::endl;
      else
      {
        cout << "Conectado ao sensor " << sensorNome[i] << std::endl;
	simxReadProximitySensor(clientID,proximity_sensor[i],NULL,NULL,NULL,NULL,simx_opmode_streaming);
      }
    }


    while(simxGetConnectionId(clientID)!=-1) // enquanto a simulação estiver ativa
    {

    vLeft  = 1;
    vRight = 1;

      for(int i = 0; i < 6; i++)
      {
	simxUChar state;
	simxFloat coord[3]; //cordenada x,y,z


        // LENDO OS SENSORES             \/ Variavel que você declarou
	if (simxReadProximitySensor(clientID,proximity_sensor[i],&state,coord,NULL,NULL,simx_opmode_buffer)==simx_return_ok)
	{
	  float dist = coord[2]; //    no caso, pegamos as informações da cordenada   Z


	  if(state > 0) // State fica >0  se o sensor achou algo
	  {            // State ficar <0  deu ruim
                  // State fica 0 quando não acha nada







         if (i > 4 && dist <=((+0.335576)/5)) // IMPAR== DIREITO
        {
          vLeft  = 0;
          vRight = 0;


        }

        else
        {
          vLeft  = 2;
          vRight = 2;

        }


	  }


	}

      }
/* simxGetVisionSensorImage(clientID,Webcam,simxInt* resolution,simxUChar** image,simxUChar options,simxInt operationMode)
int resolution[2];
resolution[0]=1280;
resolution[1]=720;
char **image=NULL;                      //ENTENDER
char options;

if (simxGetVisionSensorImage(clientID,Webcam,resolution,(simxUChar**)image,(simxUChar)options,simx_opmode_buffer) )
{

    cout<<"ok"<<endl;

}

*/

    // vez da Garra

    simxUChar state;
    simxFloat coord[3];

if (simxReadProximitySensor(clientID,Garra_sensor,&state,coord,NULL,NULL,simx_opmode_buffer)==simx_return_ok)
  {
    if(state>0)
    {

       if(coord[2]<=0.0413 && angulo<0.700411169)
       {
         angulo++;
          simxSetJointPosition(clientID,joint[2],angulo, simx_opmode_oneshot);

       }


    }



  }

  for(int i=0;i<5;i++)
  {
    if(i==1 && angulo3<0.030)
     {
       angulo3+=0.0003;
       simxSetJointPosition(clientID,joint[i],angulo3, simx_opmode_oneshot);

     }

     if (i==2 && angulo<1.57079633 && angulo3 >= 0.030) // Joint#3
     {
       angulo+=0.010;
       simxSetJointPosition(clientID,joint[i],angulo, simx_opmode_oneshot);
     }

    if(i==0 && angulo2>-1.57079633 && angulo3 >= 0.030 && angulo >= 1.57079633) //joint#1
   {
     angulo2-=0.01;
     simxSetJointPosition(clientID,joint[i],angulo2, simx_opmode_oneshot);
   }

   if(i==3) //joint#1
  {

    simxSetJointPosition(clientID,joint[i],angulo4, simx_opmode_oneshot);
  }



  }














     // atualiza velocidades dos motores (motores da frente, só ativar)
      simxSetJointTargetVelocity(clientID, motor_tras_esquerdo, (simxFloat) vLeft, simx_opmode_streaming);
      simxSetJointTargetVelocity(clientID, motor_tras_direito, (simxFloat) vRight, simx_opmode_streaming);
      simxSetJointTargetVelocity(clientID, motor_frente_esquerdo, (simxFloat) vLeft, simx_opmode_streaming);
      simxSetJointTargetVelocity(clientID, motor_frente_direito, (simxFloat) vRight, simx_opmode_streaming);





      // espera um pouco antes de reiniciar a leitura dos sensores
      extApi_sleepMs(50);
    }

    simxFinish(clientID); // fechando conexao com o servidor
    cout << "Conexao fechada!" << std::endl;
  }
  else
    cout << "Problemas para conectar o servidor!" << std::endl;
  return 0;
}
