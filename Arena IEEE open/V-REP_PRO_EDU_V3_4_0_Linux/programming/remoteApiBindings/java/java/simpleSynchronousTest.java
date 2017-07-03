// Copyright 2006-2017 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// -------------------------------------------------------------------
// THIS FILE IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// 
// You are free to use/modify/distribute this file for whatever purpose!
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.4.0 rev. 1 on April 5th 2017

import coppelia.IntW;
import coppelia.IntWA;
import coppelia.remoteApi;

// This small example illustrates how to use the remote API
// synchronous mode. The synchronous mode needs to be
// pre-enabled on the server side. You would do this by
// starting the server (e.g. in a child script) with:
//
// simExtRemoteApiStart(19999,1300,false,true)
//
// But in this example we try to connect on port
// 19997 where there should be a continuous remote API
// server service already running and pre-enabled for
// synchronous mode.
//
// IMPORTANT: for each successful call to simxStart, there
// should be a corresponding call to simxFinish at the end!

public class simpleSynchronousTest
{
    public static void main(String[] args)
    {
        System.out.println("Program started");
        remoteApi vrep = new remoteApi();
        vrep.simxFinish(-1); // just in case, close all opened connections
        int clientID = vrep.simxStart("127.0.0.1",19997,true,true,5000,5);
        if (clientID!=-1)
        {
            System.out.println("Connected to remote API server");   

            // enable the synchronous mode on the client:
            vrep.simxSynchronous(clientID,true);

            // start the simulation:
            vrep.simxStartSimulation(clientID,vrep.simx_opmode_blocking);

            // Now step a few times:
            for (int i=0;i<10;i++)
            {
                System.out.println("Press enter to step the simulation!");
                String input=System.console().readLine();
                vrep.simxSynchronousTrigger(clientID);
            }

            // stop the simulation:
            vrep.simxStopSimulation(clientID,vrep.simx_opmode_blocking);

            // Now close the connection to V-REP:   
            vrep.simxFinish(clientID);
        }
        else
            System.out.println("Failed connecting to remote API server");
        System.out.println("Program ended");
    }
}
            
