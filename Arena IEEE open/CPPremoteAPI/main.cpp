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
#include <Skills.h>


using namespace std;



SKILLS skills;

int main(int argc, char **argv) {

    string serverIP = "127.0.0.1";
    int serverPort = 19999;
    int clientID = simxStart((simxChar*) serverIP.c_str(), serverPort, true, true, 2000, 5);



    if (clientID != -1) {
        cout << "Servidor conectado!" << std::endl;

        skills.connectToRobot(clientID);



        while (simxGetConnectionId(clientID) != -1) // enquanto a simulação estiver ativa
        {   
            
            if(!skills.seguidorDeParede(clientID))


            extApi_sleepMs(150);
        }

        simxFinish(clientID); // fechando conexao com o servidor
        cout << "Conexao fechada!" << std::endl;
    } else
        cout << "Problemas para conectar o servidor!" << std::endl;
    return 0;
}
