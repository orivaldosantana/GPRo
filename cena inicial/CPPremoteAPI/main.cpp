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



#include <iostream>
#include <string>
#include <Toninho.h>
#include <ToninhoVrep.h>

using namespace std;

int main(int argc, char **argv)
{
  string serverIP = "127.0.0.1";
  int serverPort = 19999;
  
  Toninho* ura;
  ura = new ToninhoVrep(serverIP.c_str(), serverPort);
  
  ura->seguirLinha();
  
  
  
    

}
