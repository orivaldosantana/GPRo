# -*- coding: utf-8 -*-
##    Client of V-REP simulation server (remoteApi)
##    Copyright (C) 2015  Rafael Alceste Berri rafaelberri@gmail.com
##
##    This program is free software: you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation, either version 3 of the License, or
##    (at your option) any later version.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with this program.  If not, see <http://www.gnu.org/licenses/>.
##
##
##Habilite o server antes na simulação V-REP com o comando lua:
##simExtRemoteApiStart(portNumber) -- inicia servidor remoteAPI do V-REP


import vrep

#definicoes iniciais
serverIP = '127.0.0.1'
serverPort = 19999
leftMotorHandle = 0
vLeft = 0.
rightMotorHandle = 0
vRight = 0.
sensorHandle = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]


# variaveis de cena e movimentação do pioneer
noDetectionDist=0.5
maxDetectionDist=0.2
detect=[0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.]
braitenbergL=[-0.2,-0.4,-0.6,-0.8,-1,-1.2,-1.4,-1.6, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
braitenbergR=[-1.6,-1.4,-1.2,-1,-0.8,-0.6,-0.4,-0.2, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
v0=2

clientID = vrep.simxStart(serverIP,serverPort,True,True,2000,5)
if clientID <> -1:
    print ('Servidor conectado!')

    # inicialização dos motores
    erro, leftMotorHandle = vrep.simxGetObjectHandle(clientID,'Pioneer_p3dx_leftMotor',vrep.simx_opmode_oneshot_wait)
    if erro <> 0:
        print 'Handle do motor esquerdo nao encontrado!'
    else:
        print 'Conectado ao motor esquerdo!'

    erro, rightMotorHandle = vrep.simxGetObjectHandle(clientID,'Pioneer_p3dx_rightMotor',vrep.simx_opmode_oneshot_wait)
    if erro <> 0:
        print 'Handle do motor direito nao encontrado!'
    else:
        print 'Conectado ao motor direito!'

    #inicialização dos sensores (remoteApi)
    for i in range(16):
        erro, sensorHandle[i] = vrep.simxGetObjectHandle(clientID,"Pioneer_p3dx_ultrasonicSensor%d" % (i+1),vrep.simx_opmode_oneshot_wait)
        if erro <> 0:
            print "Handle do sensor Pioneer_p3dx_ultrasonicSensor%d nao encontrado!" % (i+1)
        else:
            print "Conectado ao sensor Pioneer_p3dx_ultrasonicSensor%d!" % (i+1)
            erro, state, coord, detectedObjectHandle, detectedSurfaceNormalVector = vrep.simxReadProximitySensor(clientID, sensorHandle[i],vrep.simx_opmode_streaming)

    #desvio e velocidade do robo
    while vrep.simxGetConnectionId(clientID) != -1:
        for i in range(16):
            erro, state, coord, detectedObjectHandle, detectedSurfaceNormalVector = vrep.simxReadProximitySensor(clientID, sensorHandle[i],vrep.simx_opmode_buffer)
            if erro == 0:
                dist = coord[2]
                if state > 0 and dist < noDetectionDist:
                    if dist < maxDetectionDist:
                        dist = maxDetectionDist

                    detect[i] = 1-((dist-maxDetectionDist) / (noDetectionDist-maxDetectionDist))
                else:
                    detect[i] = 0
            else:
                detect[i] = 0

        vLeft = v0
        vRight = v0

        for i in range(16):
            vLeft  = vLeft  + braitenbergL[i] * detect[i]
            vRight = vRight + braitenbergR[i] * detect[i]

        # atualiza velocidades dos motores
        erro = vrep.simxSetJointTargetVelocity(clientID, leftMotorHandle, vLeft, vrep.simx_opmode_streaming)
        erro = vrep.simxSetJointTargetVelocity(clientID, rightMotorHandle, vRight, vrep.simx_opmode_streaming)

    vrep.simxFinish(clientID) # fechando conexao com o servidor
    print 'Conexao fechada!'
else:
    print 'Problemas para conectar o servidor!'
