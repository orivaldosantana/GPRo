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

getPartMass=function(part)
    local currentMass=0
    local objects={part}
    while #objects>0 do
        handle=objects[#objects]
        table.remove(objects,#objects)
        local i=0
        while true do
            local h=simGetObjectChild(handle,i)
            if h>=0 then
                objects[#objects+1]=h
                i=i+1
            else
                break
            end
        end
        if simGetObjectType(handle)==sim_object_shape_type then
            local r,p=simGetObjectInt32Parameter(handle,sim_shapeintparam_static)
            if p==0 then
                local m0,i0,com0=simGetShapeMassAndInertia(handle)
                currentMass=currentMass+m0
            end
        end
    end
    return currentMass
end

function isPartDetected(partHandle)
    local shapesToTest={}
    if simBoolAnd32(simGetModelProperty(partHandle),sim_modelproperty_not_model)>0 then
        -- We have a single shape which is not a model. Is the shape detectable?
        if simBoolAnd32(simGetObjectSpecialProperty(partHandle),sim_objectspecialproperty_detectable_all)>0 then
            shapesToTest[1]=partHandle -- yes, it is detectable
        end
    else
        -- We have a model. Does the model have the detectable flags overridden?
        if simBoolAnd32(simGetModelProperty(partHandle),sim_modelproperty_not_detectable)==0 then
            -- No, now take all model shapes that are detectable:
            local t=simGetObjectsInTree(partHandle,sim_object_shape_type,0)
            for i=1,#t,1 do
                if simBoolAnd32(simGetObjectSpecialProperty(t[i]),sim_objectspecialproperty_detectable_all)>0 then
                    shapesToTest[#shapesToTest+1]=t[i]
                end
            end
        end
    end
    for i=1,#shapesToTest,1 do
        if simCheckProximitySensor(sensor,shapesToTest[i])>0 then
            return true
        end
    end
    return false
end

function getAllParts()
    local l=simGetObjectsInTree(sim_handle_scene,sim_object_shape_type,0)
    local retL={}
    for i=1,#l,1 do
        local isPart,isInstanciated,data=bwUtils.isObjectPartAndInstanciated(l[i])
        if isInstanciated then
            if not data['thermoformingPart'] then
                retL[#retL+1]=l[i]
            end
        end
    end
    return retL
end

checkSensor=function()
    local p=getAllParts()
    for i=1,#p,1 do
        if isPartDetected(p[i]) then
            return p[i]
        end
    end
    return -1
end

handlePartAtLocation=function(h)
    local wrappingMass=simGetShapeMassAndInertia(h)
    simSetObjectPosition(sensor,h,{0,0,-depth})
    local part=checkSensor()
    local mass=0
    if part>=0 then
        mass=getPartMass(part)
        simRemoveObject(part)
    end
    local s=simCreatePureShape(0,8,{width,length,depth},mass+wrappingMass)
    simSetObjectPosition(s,h,{0,0,0})
    simSetObjectOrientation(s,h,{0,0,0})
    simSetShapeColor(s,'',sim_colorcomponent_ambient_diffuse,color)
    simSetObjectInt32Parameter(s,sim_objintparam_visibility_layer,1+256)
    simSetObjectSpecialProperty(s,sim_objectspecialproperty_collidable+sim_objectspecialproperty_measurable+sim_objectspecialproperty_detectable_all+sim_objectspecialproperty_renderable)
    local p=simGetObjectProperty(s)
    p=simBoolOr32(p,sim_objectproperty_dontshowasinsidemodel)
    simSetObjectProperty(s,p)
    local data=simReadCustomDataBlock(h,'XYZ_FEEDERPART_INFO')
    data=simUnpackTable(data)
    data['palletPattern']=0 -- none
    simWriteCustomDataBlock(s,'XYZ_FEEDERPART_INFO',simPackTable(data))
    simSetObjectParent(s,partHolder,true)
    local partData={s,simGetSimulationTime(),simGetObjectPosition(s,-1),false,true} -- handle, lastMovingTime, lastPosition, isModel, isActive
    allProducedParts[#allProducedParts+1]=partData
    simRemoveObject(h)
end

if (sim_call_type==sim_childscriptcall_initialization) then
    REF_STOP_SIG=1 -- index of referenced stop trigger object handle
    REF_START_SIG=2 -- index of referenced start trigger object handle
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
    data=simUnpackTable(data)
    stopTriggerSensor=bwUtils.getReferencedObjectHandle(model,REF_STOP_SIG)
    startTriggerSensor=bwUtils.getReferencedObjectHandle(model,REF_START_SIG)
    local columns=data['columns']
    local columnStep=data['columnStep']
    local palletSpacing=data['pullLength']-columns*columnStep
    movementDist=columns*columnStep+palletSpacing
    cuttingStationIndex=data['cuttingStationIndex']
    getTriggerType()
    width=data['extrusionWidth']
    length=data['extrusionLength']
    depth=data['extrusionDepth']
    color=data['color']
    dwellTime=data['dwellTime']
    timeForIdlePartToDeactivate=bwUtils.modifyPartDeactivationTime(data['deactivationTime'])
    sampleHolder=simGetObjectHandle('genericThermoformer_sampleHolder')
    partHolder=simGetObjectHandle('genericThermoformer_partHolder')
    sensor=simGetObjectHandle('genericThermoformer_sensor')
    lastT=simGetSimulationTime()
    beltVelocity=0
    totShift=0
    movementUnderway=false
    dwellStart=0
    partsToMove={} -- the static parts (open boxes)
    allProducedParts={} -- the dynamic parts (boxes)
end 

if (sim_call_type==sim_childscriptcall_actuation) then
    local t=simGetSimulationTime()
    local dt=simGetSimulationTimeStep()
    if movementUnderway then
        local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
        data=simUnpackTable(data)
        local enabled=simBoolAnd32(data['bitCoded'],64)>0
        local stopRequests=data['stopRequests']
        local trigger=getTriggerType()
        if trigger>0 then
            stopRequests[model]=nil -- restart
        end
        if trigger<0 then
            stopRequests[model]=true -- stop
        end
        if next(stopRequests) then
            enabled=false
        end
        if enabled then
            if not rmlPosObj then
                posVelAccel={posVelAccel[1],0,0}
                rmlPosObj=simRMLPos(1,0.0001,-1,posVelAccel,{maxVel,accel,999999},{1},{movementDist,0})
            end
            res,posVelAccel=simRMLStep(rmlPosObj,dt)
            if res>0 then
                simRMLRemove(rmlPosObj)
                rmlPosObj=nil
                movementUnderway=false
                dwellStart=t
            end
        else
            if rmlPosObj then
                simRMLRemove(rmlPosObj)
                rmlPosObj=nil
                rmlVelObj=simRMLVel(1,0.0001,-1,posVelAccel,{accel,999999},{1},{0})
            end
            if rmlVelObj then
                res,posVelAccel=simRMLStep(rmlVelObj,dt)
                if res>0 then
                    simRMLRemove(rmlVelObj)
                    rmlVelObj=nil
                end
            end
        end

        local dShift=posVelAccel[1]-lastRelPos
        lastRelPos=posVelAccel[1]
        totShift=totShift+dShift
        for i=1,#partsToMove,1 do
            local p=simGetObjectPosition(partsToMove[i],model)
            p[2]=p[2]+dShift
            simSetObjectPosition(partsToMove[i],model,p)
        end
        local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
        data=simUnpackTable(data)
        data['encoderDistance']=totShift
        simWriteCustomDataBlock(model,'CONVEYOR_CONF',simPackTable(data))

    else
        local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
        data=simUnpackTable(data)
        enabled=simBoolAnd32(data['bitCoded'],64)>0
        if enabled then
            if t-dwellStart>dwellTime then
                local i=1
                while i<=#partsToMove do
                    local h=partsToMove[i]
                    local p=simGetObjectPosition(h,sampleHolder)
                    if p[2]>(cuttingStationIndex-1.5)*movementDist then
                        table.remove(partsToMove,i)
                        local h2=handlePartAtLocation(h)
                        simSetObjectInt32Parameter(h,sim_shapeintparam_static,0)
                    else
                        i=i+1
                    end
                end


                local samples=simGetObjectsInTree(sampleHolder,sim_handle_all,1)
                local objects=simCopyPasteObjects(samples,0)
                for i=1,#objects,1 do
                    simSetObjectInt32Parameter(objects[i],sim_objintparam_visibility_layer,1+256)
                    simSetObjectSpecialProperty(objects[i],sim_objectspecialproperty_collidable+sim_objectspecialproperty_measurable+sim_objectspecialproperty_detectable_all+sim_objectspecialproperty_renderable)
                    local p=simGetObjectProperty(objects[i])
                    p=simBoolOr32(p,sim_objectproperty_selectmodelbaseinstead+sim_objectproperty_dontshowasinsidemodel)-sim_objectproperty_selectmodelbaseinstead-sim_objectproperty_dontshowasinsidemodel
                    simSetObjectProperty(objects[i],p)
                    simSetObjectInt32Parameter(objects[i],sim_shapeintparam_respondable,1)
                    simSetObjectParent(objects[i],partHolder,true)
                    partsToMove[#partsToMove+1]=objects[i]
                    local dta=simReadCustomDataBlock(objects[i],'XYZ_FEEDERPART_INFO')
                    dta=simUnpackTable(dta)
                    dta['instanciated']=true
                    simWriteCustomDataBlock(objects[i],'XYZ_FEEDERPART_INFO',simPackTable(dta))
                end
                maxVel=data['velocity']
                accel=data['acceleration']
                posVelAccel={0,0,0}
                lastRelPos=0
                rmlPosObj=simRMLPos(1,0.0001,-1,posVelAccel,{maxVel,accel,999999},{1},{movementDist,0})
                movementUnderway=true
            end
        end
    end

    i=1
    while i<=#allProducedParts do
        local h=allProducedParts[i][1]
        if simIsHandleValid(h)>0 then
            local data=simReadCustomDataBlock(h,'XYZ_FEEDERPART_INFO')
            data=simUnpackTable(data)
            local p=simGetObjectPosition(h,-1)
            if allProducedParts[i][5] then
                -- The part is still active
                local deactivate=data['deactivate']
                local dp={p[1]-allProducedParts[i][3][1],p[2]-allProducedParts[i][3][2],p[3]-allProducedParts[i][3][3]}
                local l=math.sqrt(dp[1]*dp[1]+dp[2]*dp[2]+dp[3]*dp[3])
                if (l>0.01*dt) then
                    allProducedParts[i][2]=t
                end
                allProducedParts[i][3]=p
                if (t-allProducedParts[i][2]>timeForIdlePartToDeactivate) then
                    deactivate=true
                end
                if deactivate then
                    simSetObjectInt32Parameter(h,sim_shapeintparam_static,1) -- we make it static now!
                    simResetDynamicObject(h) -- important, otherwise the dynamics engine doesn't notice the change!
                    allProducedParts[i][5]=false
                end
            end
            -- Does it want to be destroyed?
            if data['destroy'] or p[3]<-1000 or data['giveUpOwnership'] then
                if not data['giveUpOwnership'] then
                    removePart(h,allProducedParts[i][4])
                end
                table.remove(allProducedParts,i)
            else
                i=i+1
            end
        else
            table.remove(allProducedParts,i)
        end
    end
end 
