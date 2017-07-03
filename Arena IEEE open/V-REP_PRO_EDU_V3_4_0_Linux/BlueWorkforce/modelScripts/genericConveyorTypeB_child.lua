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

function overrideMasterMotionIfApplicable(override)
    if masterConveyor>=0 then
        local data=simReadCustomDataBlock(masterConveyor,'CONVEYOR_CONF')
        if data then
            data=simUnpackTable(data)
            local stopRequests=data['stopRequests']
            if override then
                stopRequests[model]=true
            else
                stopRequests[model]=nil
            end
            data['stopRequests']=stopRequests
            simWriteCustomDataBlock(masterConveyor,'CONVEYOR_CONF',simPackTable(data))
        end
    end
end

function getMasterDeltaShiftIfApplicable()
    if masterConveyor>=0 then
        local data=simReadCustomDataBlock(masterConveyor,'CONVEYOR_CONF')
        if data then
            data=simUnpackTable(data)
            local totalShift=data['encoderDistance']
            local retVal=totalShift
            if previousMasterTotalShift then
                retVal=totalShift-previousMasterTotalShift
            end
            previousMasterTotalShift=totalShift
            return retVal
        end
    end
end

if (sim_call_type==sim_childscriptcall_initialization) then
    REF_STOP_SIG=1 -- index of referenced stop trigger object handle
    REF_START_SIG=2 -- index of referenced start trigger object handle
    REF_MASTER_CONV=3 -- index of referenced master conveyor object handle
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
    data=simUnpackTable(data)
    stopTriggerSensor=bwUtils.getReferencedObjectHandle(model,REF_STOP_SIG)
    startTriggerSensor=bwUtils.getReferencedObjectHandle(model,REF_START_SIG)
    masterConveyor=bwUtils.getReferencedObjectHandle(model,REF_MASTER_CONV)
    getTriggerType()
    path=simGetObjectHandle('genericConveyorTypeB_path')
    lastT=simGetSimulationTime()
    beltVelocity=0
    totShift=0
end 

if (sim_call_type==sim_childscriptcall_actuation) then
    local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
    data=simUnpackTable(data)
    maxVel=data['velocity']
    accel=data['acceleration']
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
        overrideMasterMotionIfApplicable(true)
    else
        overrideMasterMotionIfApplicable(false)
    end

    t=simGetSimulationTime()
    dt=t-lastT
    lastT=t

    local masterDeltaShift=getMasterDeltaShiftIfApplicable()
    if masterDeltaShift then
        totShift=totShift+masterDeltaShift
        beltVelocity=masterDeltaShift/dt
    else
        local dv=maxVel-beltVelocity
        if math.abs(dv)>accel*dt then
            beltVelocity=beltVelocity+accel*dt*math.abs(dv)/dv
        else
            beltVelocity=maxVel
        end
        totShift=totShift+dt*beltVelocity
    end
    
    simSetPathPosition(path,totShift)
    data['encoderDistance']=totShift
    simWriteCustomDataBlock(model,'CONVEYOR_CONF',simPackTable(data))
end 