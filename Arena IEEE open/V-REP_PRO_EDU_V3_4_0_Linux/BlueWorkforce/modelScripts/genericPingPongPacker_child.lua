local bwUtils=require('bwUtils')

function getTriggerType()
    if stopTriggerSensor~=-1 then
        local data=simReadCustomDataBlock(stopTriggerSensor,'XYZ_BINARYSENSOR_INFO')
        if data then
            data=simUnpackTable(data)
            local state=data['detectionState']
            if not lastStopTriggerState then
                lastStopTriggerState=state
            end
            if lastStopTriggerState~=state then
                lastStopTriggerState=state
                return -1 -- means stop
            end
        end
    end
    if startTriggerSensor~=-1 then
        local data=simReadCustomDataBlock(startTriggerSensor,'XYZ_BINARYSENSOR_INFO')
        if data then
            data=simUnpackTable(data)
            local state=data['detectionState']
            if not lastStartTriggerState then
                lastStartTriggerState=state
            end
            if lastStartTriggerState~=state then
                lastStartTriggerState=state
                return 1 -- means restart
            end
        end
    end
    return 0
end

if (sim_call_type==sim_childscriptcall_initialization) then
    REF_STOP_SIG=1 -- index of referenced stop trigger object handle
    REF_START_SIG=2 -- index of referenced start trigger object handle
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
    data=simUnpackTable(data)
    stopTriggerSensor=bwUtils.getReferencedObjectHandle(model,REF_STOP_SIG)
    startTriggerSensor=bwUtils.getReferencedObjectHandle(model,REF_START_SIG)
    getTriggerType()
    local err=simGetInt32Parameter(sim_intparam_error_report_mode)
    simSetInt32Parameter(sim_intparam_error_report_mode,0) -- do not report errors
    textureB=simGetObjectHandle('genericPingPongPacker_textureB')
    textureC=simGetObjectHandle('genericPingPongPacker_textureC')
    jointB=simGetObjectHandle('genericPingPongPacker_jointB')
    jointC=simGetObjectHandle('genericPingPongPacker_jointC')
    simSetInt32Parameter(sim_intparam_error_report_mode,err) -- report errors again
    textureA=simGetObjectHandle('genericPingPongPacker_textureA')
    forwarderA=simGetObjectHandle('genericPingPongPacker_forwarderA')
    sensors={}
    sensors[1]=simGetObjectHandle('genericPingPongPacker_cartridge1_sensor')
    sensors[2]=simGetObjectHandle('genericPingPongPacker_cartridge2_sensor')
    sensors[3]=simGetObjectHandle('genericPingPongPacker_cartridge2_sensor2')

    lastT=simGetSimulationTime()
    beltVelocity=0
    totShift=0
end 

if (sim_call_type==sim_childscriptcall_actuation) then
    local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
    data=simUnpackTable(data)
    maxVel=data['velocity']
    accel=data['acceleration']
    length=data['length']
    height=data['height']
    enabled=simBoolAnd32(data['bitCoded'],64)>0
    if not enabled then
        maxVel=0
    end
    local stopRequests=data['stopRequests']
    local trigger=getTriggerType()
    if trigger>0 then
        stopRequests[model]=nil -- restart
    end
    if trigger<0 then
        stopRequests[model]=true -- stop
    end
    if next(stopRequests) then
        maxVel=0
    end


    t=simGetSimulationTime()
    dt=t-lastT
    lastT=t
    local dv=maxVel-beltVelocity
    if math.abs(dv)>accel*dt then
        beltVelocity=beltVelocity+accel*dt*math.abs(dv)/dv
    else
        beltVelocity=maxVel
    end
    totShift=totShift+dt*beltVelocity
    
    simSetObjectFloatParameter(textureA,sim_shapefloatparam_texture_y,totShift)

    if textureB~=-1 then
        simSetObjectFloatParameter(textureB,sim_shapefloatparam_texture_y,length*0.5+0.041574*height/0.2+totShift)
        simSetObjectFloatParameter(textureC,sim_shapefloatparam_texture_y,-length*0.5-0.041574*height/0.2+totShift)
        local a=simGetJointPosition(jointB)
        simSetJointPosition(jointB,a-beltVelocity*dt*2/height)
        simSetJointPosition(jointC,a-beltVelocity*dt*2/height)
    end
    
    relativeLinearVelocity={0,beltVelocity,0}
    
    simResetDynamicObject(forwarderA)
    m=simGetObjectMatrix(forwarderA,-1)
    m[4]=0
    m[8]=0
    m[12]=0
    absoluteLinearVelocity=simMultiplyVector(m,relativeLinearVelocity)
    simSetObjectFloatParameter(forwarderA,sim_shapefloatparam_init_velocity_x,absoluteLinearVelocity[1])
    simSetObjectFloatParameter(forwarderA,sim_shapefloatparam_init_velocity_y,absoluteLinearVelocity[2])
    simSetObjectFloatParameter(forwarderA,sim_shapefloatparam_init_velocity_z,absoluteLinearVelocity[3])
    data['encoderDistance']=totShift
    simWriteCustomDataBlock(model,'CONVEYOR_CONF',simPackTable(data))
end 

if (sim_call_type==sim_childscriptcall_actuation) then
    for i=1,#sensors,1 do
        simResetProximitySensor(sensors[i])
    end
end