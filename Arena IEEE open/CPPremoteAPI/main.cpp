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







int main(int argc, char **argv) {
    
    

        
    
    SKILLS vrep("127.0.0.1",19999);
    
    
      
    
   
//    vrep.seguirParedeSOM();
             // vrep.testJunta();
              // vrep.seguidorDeParede();
            //  vrep.goToTank();
            // vrep.WhereIsTheCow();
           // vrep.testSetImage();
            vrep.controlTheRobot();
    //vrep.collectDataforNetWork();
   /// vrep.OpenTheClawCloseTheClaw(false);
           // vrep.verDistancia(0);
          // vrep.seguirParedeMLP();
       //  vrep.seguirParedeSOM();
         // vrep.collectDataforNetWork();
           // vrep.verDistancia(0);
          // vrep.testReadCam();
          //vrep.testGetImage();
           
        
            

     
    return 0;
}
