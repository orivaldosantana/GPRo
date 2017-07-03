local bwUtils=require('bwUtils')

function ragnar_startPickTime()
    -- Call this just before starting a pick motion
    _timeMeasurement2Start=simGetSimulationTime()
end

function ragnar_endPickTime(isAuxiliaryWindow)
    -- Call this just after the end of a pick motion
    local dt=simGetSimulationTime()-_timeMeasurement2Start
    _totalPickTime=_totalPickTime+dt
    _totalMovementTime=_totalMovementTime+dt
    local windowH=-1
    local avgPickTm
    if isAuxiliaryWindow then
        _auxTrackingWindowPickTime=(9*_auxTrackingWindowPickTime+dt)/10 -- kind of moving average
        windowH=auxPartTrackingWindowHandle
        avgPickTm=_auxTrackingWindowPickTime
    else
        _trackingWindowPickTime=(9*_trackingWindowPickTime+dt)/10 -- kind of moving average
        windowH=partTrackingWindowHandle
        avgPickTm=_trackingWindowPickTime
    end
    if windowH>=0 then
        local data=simReadCustomDataBlock(windowH,'XYZ_TRACKINGWINDOW_INFO')
        data=simUnpackTable(data)
        data['associatedRobotTrackingCorrectionTime']=avgPickTm
        simWriteCustomDataBlock(windowH,'XYZ_TRACKINGWINDOW_INFO',simPackTable(data))
    end
end

function ragnar_startPlaceTime()
    -- Call this just before starting a place motion
    _timeMeasurement2Start=simGetSimulationTime()
end

function ragnar_endPlaceTime(isOtherLocation)
    -- Call this just after the end of a place motion
    local dt=simGetSimulationTime()-_timeMeasurement2Start
    _totalPlaceTime=_totalPlaceTime+dt
    _totalMovementTime=_totalMovementTime+dt
    if isOtherLocation then
        _otherLocationPlaceTime=(9*_otherLocationPlaceTime+dt)/10 -- kind of moving average
    else
        _targetTrackingWindowPlaceTime=(9*_targetTrackingWindowPlaceTime+dt)/10 -- kind of moving average
        local correctionT=0
        if ragnar_getPickWithoutTarget() or stacking>1 then
            correctionT=_targetTrackingWindowPlaceTime
        else
            local cnt=0
            if _auxTrackingWindowPickTime>0 then
                correctionT=correctionT+_auxTrackingWindowPickTime
                cnt=cnt+1
            end
            if _trackingWindowPickTime>0 then
                correctionT=correctionT+_trackingWindowPickTime
                cnt=cnt+1
            end
            correctionT=correctionT/cnt
            correctionT=correctionT+_targetTrackingWindowPlaceTime
        end

        if locationTrackingWindowHandle>=0 then
            local data=simReadCustomDataBlock(locationTrackingWindowHandle,'XYZ_TRACKINGWINDOW_INFO')
            data=simUnpackTable(data)
            data['associatedRobotTrackingCorrectionTime']=correctionT
            simWriteCustomDataBlock(locationTrackingWindowHandle,'XYZ_TRACKINGWINDOW_INFO',simPackTable(data))
        end
    end
end

function ragnar_startCycleTime()
    -- Call this just before starting (or trying to start) a pick and place cycle. A pick a place cycle is a 1+ pick and 1 place motion
    _timeMeasurement1Start=simGetSimulationTime()
    _totalMovementTimeSaved=_totalMovementTime
end

function ragnar_endCycleTime(didSomething)
    -- Call this just after the end of a succeeded or failed pick and place motion
    local dt=simGetSimulationTime()-_timeMeasurement1Start
    if didSomething then
        local movementTime=_totalMovementTime-_totalMovementTimeSaved
        local lossTime=dt-movementTime
        _cycleTime=(9*_cycleTime+dt)/10 -- kind of moving average
        _lossTime=(9*_lossTime+lossTime)/10 -- kind of moving average
        _totalCycleTime=_totalCycleTime+dt
    end
    if statUi then
        local t=simGetSimulationTime()
        simExtCustomUI_setLabelText(statUi,1,string.format("Average cycle time: %.2f [s]",_cycleTime),true)
        simExtCustomUI_setLabelText(statUi,3,string.format("Average loss time: %.2f [s]",_lossTime),true)
        simExtCustomUI_setLabelText(statUi,2,string.format("Idle time: %.1f [%%]",100*(t-_totalCycleTime)/t),true)
    end
end

function enableDisableStats_fromCustomizationScript(enableIt)
    if statUi then
        simExtCustomUI_destroy(statUi)
        statUi=nil
    end
    prepareStatisticsDialog(enableIt)
end

function getToolHandleAndStacking()
    local toolAttachment=simGetObjectHandle('ragnar_toolAttachment')
    local h=simGetObjectChild(toolAttachment,0)
    if h>=0 then
        local data=simReadCustomDataBlock(h,'XYZ_RAGNARGRIPPER_INFO')
        if data then
            data=simUnpackTable(data)
            local s=data['stacking']
            local ss=data['stackingShift']
            if s<=1 then
                ss=0
            end
            return h,s,ss
        end
    end
    return toolAttachment,1,0
end

function getListOfSelectedLocationsOrBuckets(ragnarSettings)
    local retL={}
    for i=1,4,1 do
        local h=bwUtils.getReferencedObjectHandle(model,REF_DROP_LOCATION1+i-1)
        if h>=0 then
            local data=simReadCustomDataBlock(h,'XYZ_BUCKET_INFO')
            if data then
                data=simUnpackTable(data)
                local dimension={data['width'],data['length'],data['height']}
                retL[#retL+1]={h,1,dimension} -- 0 is location, 1 is bucket
            else
                data=simReadCustomDataBlock(h,'XYZ_LOCATION_INFO')
                if data then
                    data=simUnpackTable(data)
                    retL[#retL+1]={h,0,data['name']} -- 0 is location, 1 is bucket
                end
            end
        end
    end
    return retL
end

function ragnar_getDropLocationInfo(locationName)
    local effLoc={}
    local retV={}
    for loc=1,#allSelectedLocationsOrBuckets,1 do
        if allSelectedLocationsOrBuckets[loc][2]==1 then
            -- This is a bucket
            -- Is that bucket the right bucket for the part, and is it operational? i.e. does it currently accept items?
            local data=simUnpackTable(simReadCustomDataBlock(allSelectedLocationsOrBuckets[loc][1],'XYZ_BUCKET_INFO'))
            if (data['locationWhenEmpty']==locationName) and (data['status']=='needToFill') then
                -- yes
                local info={}
                local p=simGetObjectPosition(allSelectedLocationsOrBuckets[loc][1],model)
                info['pos']=p
                info['isBucket']=true
                retV[#retV+1]=info
            end
            -- Here we need to make sure that we will ignore a possible location that was
            -- specified, and that coincides with the bucket:
            for ll=1,#allSelectedLocationsOrBuckets,1 do
                if (allSelectedLocationsOrBuckets[ll][2]==0) and (allSelectedLocationsOrBuckets[ll][3]==locationName) then
                    p=simGetObjectPosition(allSelectedLocationsOrBuckets[ll][1],allSelectedLocationsOrBuckets[loc][1])
                    if math.sqrt(p[1]*p[1]+p[2]*p[2])<0.02 then
                        effLoc[allSelectedLocationsOrBuckets[ll][1]]=true
                    end
                end
            end
        end
    end
    for loc=1,#allSelectedLocationsOrBuckets,1 do
        if allSelectedLocationsOrBuckets[loc][2]==0 then
            -- This is a location
            -- The right location for the current part, and not yet used via a bucket?
            if allSelectedLocationsOrBuckets[loc][3]==locationName and (not effLoc[allSelectedLocationsOrBuckets[loc][1]]) then
                -- yes
                local info={}
                local p=simGetObjectPosition(allSelectedLocationsOrBuckets[loc][1],model)
                info['pos']=p
                info['isBucket']=false
                retV[#retV+1]=info
            end
        end
    end
    return retV
end

function ragnar_getTrackingLocationInfo(locationName,processingStage)
    return getAllTargetsInTrackingWindow(locationTrackingWindowHandle,locationName,processingStage)
end

function ragnar_incrementTrackedLocationProcessingStage(trackingLocation)
    if locationTrackingWindowHandle>=0 then
--        simSetThreadAutomaticSwitch(false)
        local data=simReadCustomDataBlock(locationTrackingWindowHandle,'XYZ_TRACKINGWINDOW_INFO')
        data=simUnpackTable(data)
        local tbl=data['targetPositionsToMarkAsProcessed']
        tbl[#tbl+1]=trackingLocation['dummyHandle']
        data['targetPositionsToMarkAsProcessed']=tbl
        simWriteCustomDataBlock(locationTrackingWindowHandle,'XYZ_TRACKINGWINDOW_INFO',simPackTable(data))
--        simSetThreadAutomaticSwitch(true)
    end
end

getAllTargetsInTrackingWindow=function(trackingWindowHandle,locationName,processingStage)
    local ret={}
    if trackingWindowHandle>=0 then
        local data=simReadCustomDataBlock(trackingWindowHandle,'XYZ_TRACKINGWINDOW_INFO')
        data=simUnpackTable(data)
        local trackedTargets=data['trackedTargetsInWindow']
        local m=simGetObjectMatrix(model,-1)
        local windowTransfRot=simGetObjectMatrix(trackingWindowHandle,-1)
        windowTransfRot[4]=0
        windowTransfRot[8]=0
        windowTransfRot[12]=0
        simInvertMatrix(m)
        simInvertMatrix(windowTransfRot)
        m[4]=0
        m[8]=0
        m[12]=0
        local cumulVelVectY=0
        local cnt=0
        for key,value in pairs(trackedTargets) do
            if value['processingStage']==processingStage and value['partName']==locationName then
                local dat={}
                dat['dummyHandle']=key
                dat['partHandle']=value['partHandle']
                local p=simGetObjectPosition(key,model)
                local pW=simGetObjectPosition(key,trackingWindowHandle)
                dat['pos']=p
                local v=simMultiplyVector(m,value['velocityVect'])
                local w=simMultiplyVector(windowTransfRot,value['velocityVect'])
                dat['velocityVect']=v
                dat['sort']=pW[2]
                dat['ser']=value['ser']
                ret[#ret+1]=dat
                cumulVelVectY=cumulVelVectY+w[2]
                cnt=cnt+1
            end
        end
        if #ret>1 then
            local ascending=true
            if math.abs(cumulVelVectY/cnt)<0.001 then
                -- very slow. Try to use a previous direction
                local done=false
                if windowsAndDirections then
                    local val=windowsAndDirections[trackingWindowHandle]
                    if val then
                        ascending=(val<0)
                        done=true
                    end
                end
                if not done then
                    ascending=(cumulVelVectY/cnt<0)
                end
            else
                -- fast enough.
                if not windowsAndDirections then
                    windowsAndDirections={}
                end
                windowsAndDirections[trackingWindowHandle]=cumulVelVectY/cnt
                ascending=(cumulVelVectY/cnt<0)
            end
            if ascending then
                table.sort(ret,function(a,b) if math.abs(a['sort']-b['sort'])>0.002 then return a['sort']<b['sort'] else return a['ser']>b['ser'] end end)
            else
                table.sort(ret,function(a,b) if math.abs(a['sort']-b['sort'])>0.002 then return a['sort']>b['sort'] else return a['ser']>b['ser'] end end)
            end
        end
    end
    return ret
end



getAllPartsInTrackingWindow=function(trackingWindowHandle)
    local ret={}
    if trackingWindowHandle>=0 then
        local data=simReadCustomDataBlock(trackingWindowHandle,'XYZ_TRACKINGWINDOW_INFO')
        data=simUnpackTable(data)
        local trackedParts=data['trackedItemsInWindow']
        -- Make all data relative to the robot's ref frame (is right now absolute):
        local transf=simGetObjectMatrix(model,-1)
        local transfRot=simGetObjectMatrix(model,-1)
        local windowTransf=simGetObjectMatrix(trackingWindowHandle,-1)
        local windowTransfRot=simGetObjectMatrix(trackingWindowHandle,-1)
        transfRot[4]=0
        transfRot[8]=0
        transfRot[12]=0
        windowTransfRot[4]=0
        windowTransfRot[8]=0
        windowTransfRot[12]=0
        simInvertMatrix(transf)
        simInvertMatrix(transfRot)
        simInvertMatrix(windowTransf)
        simInvertMatrix(windowTransfRot)
        local cumulVelVectY=0
        local cnt=0
        for key,value in pairs(trackedParts) do
            local ppos=simMultiplyVector(transf,value['pickPos'])
            local pposW=simMultiplyVector(windowTransf,value['pickPos'])
            value['pickPos']=ppos
            local v=simMultiplyVector(transfRot,value['velocityVect'])
            local w=simMultiplyVector(windowTransfRot,value['velocityVect'])
            value['velocityVect']=v
            value['normalVect']=simMultiplyVector(transfRot,value['normalVect'])
            value['partHandle']=key -- we add that data
            value['sort']=pposW[2]
            ret[#ret+1]=value
            cumulVelVectY=cumulVelVectY+w[2]
            cnt=cnt+1
        end
        if #ret>1 then
            local ascending=true
            if math.abs(cumulVelVectY/cnt)<0.001 then
                -- very slow. Try to use a previous direction
                local done=false
                if windowsAndDirections then
                    local val=windowsAndDirections[trackingWindowHandle]
                    if val then
                        ascending=(val<0)
                        done=true
                    end
                end
                if not done then
                    ascending=(cumulVelVectY/cnt<0)
                end
            else
                -- fast enough.
                if not windowsAndDirections then
                    windowsAndDirections={}
                end
                windowsAndDirections[trackingWindowHandle]=cumulVelVectY/cnt
                ascending=(cumulVelVectY/cnt<0)
            end
            if ascending then
                table.sort(ret,function(a,b) return a['sort']<b['sort'] end)
            else
                table.sort(ret,function(a,b) return a['sort']>b['sort'] end)
            end
        end
    end
    return ret
end


removePartFromTrackingWindow=function(trackingWindowHandle,partHandle)
    if trackingWindowHandle>=0 then
--        simSetThreadAutomaticSwitch(false)
        local data=simReadCustomDataBlock(trackingWindowHandle,'XYZ_TRACKINGWINDOW_INFO')
        data=simUnpackTable(data)
        local tbl=data['itemsToRemoveFromTracking']
        tbl[#tbl+1]=partHandle
        data['itemsToRemoveFromTracking']=tbl

        -- Remove it also from current simulation step data, otherwise we might pick the same again:
        local trackedParts=data['trackedItemsInWindow']
        trackedParts[partHandle]=nil
        data['trackedItemsInWindow']=trackedParts

        simWriteCustomDataBlock(trackingWindowHandle,'XYZ_TRACKINGWINDOW_INFO',simPackTable(data))
    end
end

ragnar_stopTrackingPart=function(part)
    local partHandle=part['partHandle']
    if part['auxWin'] then
        lastPartFromAuxWindow=true
        removePartFromTrackingWindow(auxPartTrackingWindowHandle,partHandle)
    else
        lastPartFromAuxWindow=false
        removePartFromTrackingWindow(partTrackingWindowHandle,partHandle)
    end
end

shiftStackingParts=function(theStackingShift)
    if #attachedParts>0 and theStackingShift>0 then
        local p=simGetObjectPosition(attachedParts[1],model)
        p[3]=p[3]+theStackingShift
        simSetObjectPosition(attachedParts[1],model,p)
    end
end


ragnar_attachPart=function(part)
    local partHandle=part['partHandle']
    if simIsHandleValid(partHandle)>0 then
        local p=simGetModelProperty(partHandle)
        if simBoolAnd32(p,sim_modelproperty_not_model)==0 then
            -- We have a model
            p=simBoolOr32(p,sim_modelproperty_not_dynamic)
            simSetModelProperty(partHandle,p)
        else
            -- We have a shape
            simSetObjectInt32Parameter(partHandle,sim_shapeintparam_static,1)
            simResetDynamicObject(partHandle)
        end
        if #attachedParts==0 then
            previousPartParent=simGetObjectParent(partHandle)
            simSetObjectParent(partHandle,ikModeTipDummy,true)
            attachedParts[1]=partHandle
        else
            if ragnar_getAttachToTarget() then
                local sens=simCreateForceSensor(0,{0,1,1,0,0},{0.001,1,1,0,0})
                simSetObjectInt32Parameter(sens,sim_objintparam_visibility_layer,0) -- hidden
                simSetObjectPosition(sens,attachedParts[1],{0,0,0})
                simSetObjectParent(attachedParts[1],sens,true)
                simSetObjectParent(sens,partHandle,true)
            else
                simSetObjectParent(attachedParts[1],partHandle,true)
            end
            previousPartParent=simGetObjectParent(partHandle)
            simSetObjectParent(partHandle,ikModeTipDummy,true)
            table.insert(attachedParts,1,partHandle)
        end
    end
end

ragnar_detachPart=function()
    if #attachedParts>0 then
    
        if simIsHandleValid(attachedParts[1])>0 then
            local p=simGetModelProperty(attachedParts[1])
            if simBoolAnd32(p,sim_modelproperty_not_model)==0 then
                -- We have a model
                p=simBoolOr32(p,sim_modelproperty_not_dynamic)-sim_modelproperty_not_dynamic
                simSetModelProperty(attachedParts[1],p)
            else
                -- We have a shape
                simSetObjectInt32Parameter(attachedParts[1],sim_shapeintparam_static,0)
                simResetDynamicObject(attachedParts[1])
            end
            simSetObjectParent(attachedParts[1],previousPartParent,true)
            for i=2,#attachedParts,1 do
                -- the child parts (in the stacking):
                local p=simGetModelProperty(attachedParts[i])
                if simBoolAnd32(p,sim_modelproperty_not_model)==0 then
                    -- We have a model
                    p=simBoolOr32(p,sim_modelproperty_not_dynamic)-sim_modelproperty_not_dynamic
                    simSetModelProperty(attachedParts[i],p)
                else
                    -- We have a shape
                    simSetObjectInt32Parameter(attachedParts[i],sim_shapeintparam_static,0)
                    simResetDynamicObject(attachedParts[i])
                end
                
                if ragnar_getAttachToTarget() then
                    -- Give up ownership of the child parts in the stacking:
                    local data=simReadCustomDataBlock(attachedParts[i],'XYZ_FEEDERPART_INFO')
                    data=simUnpackTable(data)
                    data['giveUpOwnership']=true
                    simWriteCustomDataBlock(attachedParts[i],'XYZ_FEEDERPART_INFO',simPackTable(data))
                else
                    simSetObjectParent(attachedParts[i],previousPartParent,true)
                end
            end
            if #attachedParts>1 and (simBoolAnd32(attachedParts[1],sim_modelproperty_not_model)>0) and ragnar_getAttachToTarget() then
                -- We need to turn the main stack parent into model:
                local p=simGetModelProperty(attachedParts[1])
                p=simBoolOr32(p,sim_modelproperty_not_model)-sim_modelproperty_not_model
                simSetModelProperty(attachedParts[1],p)
            end
            attachedParts={}
        else
            attachedParts={}
        end

    end
end

attachPart1ToPart2=function(part1,part2)
    if (simIsHandleValid(part1)>0) and (simIsHandleValid(part2)>0) then
--        simSetThreadAutomaticSwitch(false)
        local f=simCreateForceSensor(0,{0,1,1,0,0},{0.001,0,0,0,0})
        simSetObjectInt32Parameter(f,sim_objintparam_visibility_layer,256)
        simSetObjectPosition(f,part1,{0,0,0})
        simSetObjectParent(part1,f,true)
        simSetObjectParent(f,part2,true)
        
        local data=simUnpackTable(simReadCustomDataBlock(part1,'XYZ_FEEDERPART_INFO'))
        data['giveUpOwnership']=true
        simWriteCustomDataBlock(part1,'XYZ_FEEDERPART_INFO',simPackTable(data))
        
        local objs=simGetObjectsInTree(part1,sim_object_shape_type)
        for i=1,#objs,1 do
            local r,p=simGetObjectInt32Parameter(part2,sim_shapeintparam_respondable_mask)
            p=simBoolAnd32(p,65535-255)
            simSetObjectInt32Parameter(objs[i],sim_shapeintparam_respondable_mask,p)
            simResetDynamicObject(objs[i])
        end
--        simSetThreadAutomaticSwitch(true)
    end
end

handleKinematics=function()
    local res=simHandleIkGroup(mainIkTask)
    if res==sim_ikresult_fail then
        if kinematicsFailedDialogHandle==-1 then
            kinematicsFailedDialogHandle=simDisplayDialog("IK failure report","IK solver failed.",sim_dlgstyle_message,false,"",nil,{1,0.8,0,0,0,0})
        end
    else
        if kinematicsFailedDialogHandle~=-1 then
            simEndDialog(kinematicsFailedDialogHandle)
            kinematicsFailedDialogHandle=-1
        end
    end
end

setFkMode=function()
    -- disable the platform positional constraints:
    simSetIkElementProperties(mainIkTask,ikModeTipDummy,0)
    -- Set the driving joints into passive mode (not taken into account during IK resolution):
    simSetJointMode(fkDrivingJoints[1],sim_jointmode_passive,0)
    simSetJointMode(fkDrivingJoints[2],sim_jointmode_passive,0)
    simSetJointMode(fkDrivingJoints[3],sim_jointmode_passive,0)
    simSetJointMode(fkDrivingJoints[4],sim_jointmode_passive,0)
    -- In FK mode, we want Ik to be handled automatically
    simSetExplicitHandling(mainIkTask,0)
    simSwitchThread()
end

setIkMode=function()
    simSwitchThread()
    -- In IK mode, we want Ik to be handled in this script:
    simSetExplicitHandling(mainIkTask,1)
    simSwitchThread()
    -- Make sure the target dummy has the same pose as the tip dummy:
    simSetObjectPosition(ikModeTargetDummy,ikModeTipDummy,{0,0,0})
    simSetObjectOrientation(ikModeTargetDummy,ikModeTipDummy,{0,0,0})
    -- enable the platform positional constraints:
    simSetIkElementProperties(mainIkTask,ikModeTipDummy,sim_ik_x_constraint+sim_ik_y_constraint+sim_ik_z_constraint+sim_ik_alpha_beta_constraint+sim_ik_gamma_constraint)
    -- Set the base joints into ik mode (taken into account during IK resolution):
    simSetJointMode(fkDrivingJoints[1],sim_jointmode_ik,0)
    simSetJointMode(fkDrivingJoints[2],sim_jointmode_ik,0)
    simSetJointMode(fkDrivingJoints[3],sim_jointmode_ik,0)
    simSetJointMode(fkDrivingJoints[4],sim_jointmode_ik,0)
end

--[[
RobMove = function(blend,nulling)
    
    mDone = 0
    while (mDone < 3) do
        local dt=simGetSimulationTimeStep()

        local rmlObj=simRMLPos(4,0.0001,-1,curPVA,MaxVAJ,selVec,tarPV)
        res,nextPVA=simRMLStep(rmlObj,dt)
        simRMLRemove(rmlObj)
--        res,nextPVA,syncTime =simRMLPosition(4,dt,-1,curPVA,MaxVAJ,selVec,tarPV)

        newPos = {nextPVA[1],nextPVA[2],nextPVA[3]}
        simSetObjectPosition(ikModeTargetDummy,model,newPos)
        simSetObjectOrientation(ikModeTargetDummy,model,{0,0,nextPVA[4]})
        handleKinematics()
        --simSetObjectOrientation(ikModeTargetDummy2,model,{0,0,-nextPVA[4]})
        --newPos[4] = -newPos[4]
        --simSetObjectPosition(ikModeTargetDummy2,model,newPos)
        curPVA = nextPVA;
    --    txt = string.format(" newPos ( %.2f,%.2f,%.2f) %.2f",newPos[1],newPos[2],newPos[3],dist2go)
        --simAddStatusbarMessage(txt)
        dist2go = math.sqrt((nextPVA[1]-tarPV[1])*(nextPVA[1]-tarPV[1]) + (tarPV[2]-nextPVA[2])*(tarPV[2]-nextPVA[2]) + (tarPV[3]-nextPVA[3])*(tarPV[3]-nextPVA[3]))--*1000.0
        if (mDone == 0) and(dist2go < blend) then
            mDone =1    
            dist2go = 100.0--denom*proMov
            tarPV[3] = tarPV[3]-appHight
        end    
        if (mDone == 1) and(dist2go < nulling) then
            mDone =2    
            dist2go = 100.0--denom*proMov
            tarPV[3] = tarPV[3]+appHight
        end
        if (mDone == 2) and(dist2go < blend) then
            mDone =3
        end
        simSwitchThread() -- Important, in order to have the thread synchronized with the simulation loop!
    end
end
--]]
createDummyToFollowWithOffset=function(parentDummy,posOffset)
    local m=simGetObjectMatrix(toolHandle,-1)
    local dummyHandleToFollow=simCreateDummy(0.001)
    simSetObjectInt32Parameter(dummyHandleToFollow,sim_objintparam_visibility_layer,1024)
    local v={posOffset[1]*m[1]+posOffset[2]*m[2]+posOffset[3]*m[3],posOffset[1]*m[5]+posOffset[2]*m[6]+posOffset[3]*m[7],posOffset[1]*m[9]+posOffset[2]*m[10]+posOffset[3]*m[11]}
    local p=simGetObjectPosition(parentDummy,-1)
    simSetObjectPosition(dummyHandleToFollow,-1,{p[1]+v[1],p[2]+v[2],p[3]+v[3]})
    simSetObjectOrientation(dummyHandleToFollow,parentDummy,{0,0,0})
    simSetObjectParent(dummyHandleToFollow,parentDummy,true)
    return dummyHandleToFollow
end

RobPick = function(partData,attachPart,theStackingShift,approachHeight,blend,nulling,dwTime)
    local version=simGetInt32Parameter(sim_intparam_program_version)
    local _dummyHandleToFollow=partData['dummyHandle']
    local dummyHandleToFollow=createDummyToFollowWithOffset(_dummyHandleToFollow,pickOffset)

    if attachPart then
        shiftStackingParts(theStackingShift)    
    end
    
    mDone = 0
    app = approachHeight
    while (mDone < 4) do
        local dt=simGetSimulationTimeStep()
        partPos = simGetObjectPosition(dummyHandleToFollow,model)
        partOr =  simGetObjectOrientation(dummyHandleToFollow,model)
        partVel,partorV = simGetObjectVelocity(dummyHandleToFollow)
        if version<=30302 then -- this to fix a problem in versions prior to V3.4.0
            if version>0 then
                version=0
            else
                version=30400
            end
            partVel={0,0,0}
            partorV={0,0,0}
        end
--partOr[3]/angularGain
        partPV = {partPos[1]+partVel[1]*dt,partPos[2]+partVel[2]*dt,partPos[3]+partVel[3]*dt+app,0.0,
             partVel[1]*dt,partVel[2]*dt,partVel[3]*dt,0.0}
        
        local rmlObj=simRMLPos(4,0.0001,-1,curPVA,MaxVAJ,selVec,partPV)
        res,nextPVA=simRMLStep(rmlObj,dt)
        simRMLRemove(rmlObj)

        newPos = {nextPVA[1],nextPVA[2],nextPVA[3]}
        simSetObjectPosition(ikModeTargetDummy,model,newPos)
--ikModeTipDummy
        simSetObjectOrientation(ikModeTargetDummy,model,{0,0,nextPVA[4]})
        handleKinematics()
        curPVA = nextPVA;
    --    txt = string.format(" newPos ( %.2f,%.2f,%.2f) %.2f",newPos[1],newPos[2],newPos[3],dist2go)
        --simAddStatusbarMessage(txt)
        dist2go = math.sqrt((nextPVA[1]-partPV[1])*(nextPVA[1]-partPV[1]) + (partPV[2]-nextPVA[2])*(partPV[2]-nextPVA[2]) + (partPV[3]-nextPVA[3])*(partPV[3]-nextPVA[3]))--*1000.0
        if (mDone == 0) and(dist2go < blend) then
            mDone =1    
            dist2go = 100.0--denom*proMov
            app = .0
        end    
        if (mDone == 1) and(dist2go < nulling) then
            mDone =2    
            dist2go = 100.0--denom*proMov
            t2=simGetSimulationTime()+dwTime
            if attachPart then
                ragnar_attachPart(partData)
            end
        end
        if (mDone == 2) and(t2 < simGetSimulationTime()) then
            mDone =3
            dist2go = 100.0--denom*proMov
            app = approachHeight
        end
        if (mDone == 3) and(dist2go < blend) then
            mDone =4    
        end
        simSwitchThread() -- Important, in order to have the thread synchronized with the simulation loop!
    end
    simRemoveObject(dummyHandleToFollow)
end

RobPlace = function(TrackPart,detachPart,approachHeight,blend,nulling,dwTime,attachToTrackingLocation)
    local version=simGetInt32Parameter(sim_intparam_program_version)
    local dummyHandleToFollow=createDummyToFollowWithOffset(TrackPart,placeOffset)
    mDone = 0
    app = approachHeight
    inFront = 0.0
    placeHeight = 0.01
--    simSetThreadAutomaticSwitch(false)
    while (mDone < 4) do
        local dt=simGetSimulationTimeStep()
        if( mDone < 3 ) then -- only update unitl picked 
            partPos = simGetObjectPosition(dummyHandleToFollow,model)
           -- local RobPos = simGetObjectPosition(model,-1)
            partPos[1] = partPos[1]--dropPos[1]-RobPos[1] --pickupHeight+
            partPos[2] = partPos[2]--dropPos[2]-RobPos[2] --pickupHeight+
            partPos[3] = partPos[3]---0.65+placeHeight --pickupHeight+
            partOr = simGetObjectOrientation(dummyHandleToFollow,model)
            partVel,partorV = simGetObjectVelocity(dummyHandleToFollow)
            if version<=30302 then -- this to fix a problem in versions prior to V3.4.0
                if version>0 then
                    version=0
                else
                    version=30400
                end
                partVel={0,0,0}
                partorV={0,0,0}
            end
        end
        partPV = {partPos[1]+partVel[1]*dt,partPos[2]+partVel[2]*dt + inFront,partPos[3]+partVel[3]*dt+app,0.0,
             partVel[1]*dt,partVel[2]*dt,partVel[3]*dt,0.0} --partOr[3]/angularGain
      --  if (partVel[1] > linearVelocity/4/dt) or (partVel[2] > linearVelocity/4/dt) or (partVel[3] > linearVelocity/4/dt) then
        --    PickOK = false
          --  simAddStatusbarMessage("too fast part")
            --return
        --end


        local rmlObj=simRMLPos(4,0.0001,-1,curPVA,MaxVAJ,selVec,partPV)
        res,nextPVA=simRMLStep(rmlObj,dt)
        simRMLRemove(rmlObj)




        newPos = {nextPVA[1],nextPVA[2],nextPVA[3]}
        simSetObjectPosition(ikModeTargetDummy,model,newPos)
        --simSetObjectOrientation(ikModeTargetDummy,-1,{0,0,nextPVA[4]})
        handleKinematics()
        --simSetObjectOrientation(ikModeTargetDummy2,model,{0,0,-nextPVA[4]})
        --newPos[4] = -newPos[4]
        --simSetObjectPosition(ikModeTargetDummy2,model,newPos)
        curPVA = nextPVA;
    --    txt = string.format(" newPos ( %.2f,%.2f,%.2f) %.2f",newPos[1],newPos[2],newPos[3],dist2go)
        --simAddStatusbarMessage(txt)
        dist2go = math.sqrt((nextPVA[1]-partPV[1])*(nextPVA[1]-partPV[1]) + (partPV[2]-nextPVA[2])*(partPV[2]-nextPVA[2]) + (partPV[3]-nextPVA[3])*(partPV[3]-nextPVA[3]))--*1000.0
        if (mDone == 0) and(dist2go < blend) then
--simSetThreadAutomaticSwitch(true)
            mDone =1    
            dist2go = 100.0--denom*proMov
            app = .0
--simSetThreadAutomaticSwitch(false)
        end    
        if (mDone == 1) and(dist2go < nulling) then
--simSetThreadAutomaticSwitch(true)
            mDone =2    
            dist2go = 100.0--denom*proMov
            t2=simGetSimulationTime()+dwTime
            if detachPart then
                local attachedPartSaved=attachedParts[1]
                ragnar_detachPart()
                if attachToTrackingLocation then
                    attachPart1ToPart2(attachedPartSaved,attachToTrackingLocation)
                end
            end
--            detachPart(partHandleToPick)
           -- simSetScriptSimulationParameter(simGetScriptAssociatedWithObject(suctionPad),'active','false')
--simSetThreadAutomaticSwitch(false)
        end
        if (mDone == 2) and(t2 < simGetSimulationTime()) then
            mDone =3
            dist2go = 100.0--denom*proMov
            app = approachHeight
        end
        if (mDone == 3) and(dist2go < blend) then
--simSetThreadAutomaticSwitch(true)
            mDone =4    
        end
        simSwitchThread() -- Important, in order to have the thread synchronized with the simulation loop!
    end
    simRemoveObject(dummyHandleToFollow)
--simSetThreadAutomaticSwitch(true)
end

ragnar_moveToPickLocation=function(partData,attachPart,theStackingShift)
    cycleStartTime=simGetSimulationTime()
    RobPick(partData,attachPart,theStackingShift,pickApproachHeight,pickRounding,pickNulling,dwellTime)
end

ragnar_moveToDropLocation=function(dropLocationInfo,detachPart)
    local dropPos=dropLocationInfo['pos']

    xth = 0*math.pi/180 --0.0--
 --   tarPV = {dropPos[1],dropPos[2],dropPos[3],xth,0.0,0.0,0.0,0.0}
 --   RobMove(0.05,0.005)
    local dropDum=simCreateDummy(0.001)
    simSetObjectInt32Parameter(dropDum,sim_objintparam_visibility_layer,0)
    simSetObjectPosition(dropDum,model,dropPos)
    local p=simGetObjectPosition(dropDum,-1)
 --   p[3]=0.9 -- hard-code the drop height for now
    simSetObjectPosition(dropDum,-1,p)
    RobPlace(dropDum,detachPart,placeApproachHeight,placeRounding,placeNulling,dwellTime)
    simRemoveObject(dropDum)
end

ragnar_moveToTrackingLocation=function(trackingLocationInfo,detachPart,attachToTrackingLocation)
    local partHandle=-1
    if attachToTrackingLocation then
        partHandle=trackingLocationInfo['partHandle']
    end
    RobPlace(trackingLocationInfo['dummyHandle'],detachPart,placeApproachHeight,placeRounding,placeNulling,dwellTime,partHandle)
end

ragnar_getAllTrackedParts=function()
    local r=getAllPartsInTrackingWindow(partTrackingWindowHandle)
    for i=1,#r,1 do
        r[i]['auxWin']=false
    end
    local r2=getAllPartsInTrackingWindow(auxPartTrackingWindowHandle)
    for i=1,#r2,1 do
        r2[i]['auxWin']=true
        r[#r+1]=r2[i]
    end
    return r
end

ragnar_getAttachToTarget=function()
    local data=simReadCustomDataBlock(model,'RAGNAR_CONF')
    data=simUnpackTable(data)
    return(simBoolAnd32(data['bitCoded'],1024)>0)
end

ragnar_getStacking=function()
    return stacking,stackingShift
end

ragnar_getPickWithoutTarget=function()
    local data=simReadCustomDataBlock(model,'RAGNAR_CONF')
    data=simUnpackTable(data)
    return(simBoolAnd32(data['bitCoded'],2048)>0)
end

ragnar_getEnabled=function()
    local data=simReadCustomDataBlock(model,'RAGNAR_CONF')
    data=simUnpackTable(data)
    return(simBoolAnd32(data['bitCoded'],64)>0)
end

--[[
ragnar_getAllTrackedLocations=function()
    return getAllPartsInTrackingWindow(locationTrackingWindowHandle)
end
--]]

prepareStatisticsDialog=function(enabled)
    if enabled then
        local xml =[[
                <label id="1" text="Average cycle time: 0.00 [s]" style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
                <label id="3" text="Average loss time: 0.00 [s]" style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
                <label id="2" text="Idle time: 100.0 [%]" style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
        ]]
        statUi=bwUtils.createCustomUi(xml,simGetObjectName(model)..' Statistics','bottomLeft',true--[[,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])
    end
end
--[[
updateStatisticsDialog=function(cycleTime,auxCycleTime,totalCycleTime)
    if statUi then
        if totalCycleTime>0 then
            local ct=0
            local cnt=0
            if cycleTime then
                ct=ct+cycleTime
                cnt=cnt+1
            end
            if auxCycleTime then
                ct=ct+auxCycleTime
                cnt=cnt+1
            end
            ct=ct/cnt
            local t=simGetSimulationTime()-startTime
            simExtCustomUI_setLabelText(statUi,1,string.format("Average cycle time: %.2f [s]",ct),true)
            simExtCustomUI_setLabelText(statUi,2,string.format("Idle time: %.1f [%%]",100*(t-totalCycleTime)/t),true)
        else
            simExtCustomUI_setLabelText(statUi,1,"Average cycle time: 0.00 [s]",true)
            simExtCustomUI_setLabelText(statUi,2,"Idle time: 100.0 [%]",true)
        end
    end
end
--]]
updateMotionParameters=function()
    -- 1. Read the current motion settings for the Ragnar:
    local ragnarSettings=simUnpackTable(simReadCustomDataBlock(model,'RAGNAR_CONF'))
    local mVel=ragnarSettings['maxVel']
    local mAccel=ragnarSettings['maxAccel']
    MaxVAJ = {mVel,mVel,mVel,angularVelocity,mAccel,mAccel,mAccel,angularAccel,2000,2000,2000,1000} -- pos,vel,acc128*dt
--    local trackingTimeShift=ragnarSettings['trackingTimeShift']
    
    dwellTime=ragnarSettings['dwellTime']

    -- Read the current tool offsets:
    pickOffset=ragnarSettings['pickOffset']
    placeOffset=ragnarSettings['placeOffset']
    -- And rounding, nulling and approachHeight:
    pickRounding=ragnarSettings['pickRounding']
    placeRounding=ragnarSettings['placeRounding']
    pickNulling=ragnarSettings['pickNulling']
    placeNulling=ragnarSettings['placeNulling']
    pickApproachHeight=ragnarSettings['pickApproachHeight']
    placeApproachHeight=ragnarSettings['placeApproachHeight']

    -- 2. Update the max vel/accel/jerk vector:
end

-- Begin of the thread code:
REF_PART_TRACKING1=1 -- primary part tracking window
REF_PART_TRACKING2=2 -- aux part tracking window
-- Free spots here
REF_TARGET_TRACKING1=11 -- target tracking window
-- Free spots here
REF_DROP_LOCATION1=21 -- drop location 1
REF_DROP_LOCATION2=22 -- drop location 2
REF_DROP_LOCATION3=23 -- drop location 3
REF_DROP_LOCATION4=24 -- drop location 4
simSetThreadAutomaticSwitch(false)
-- simSetThreadSwitchTiming(200)
model=simGetObjectAssociatedWithScript(sim_handle_self)
mainIkTask=simGetIkGroupHandle('Ragnar')
ikModeTipDummy=simGetObjectHandle('Ragnar_InvKinTip')
ikModeTargetDummy=simGetObjectHandle('Ragnar_InvKinTarget')
-- Following are the joints that we control when in FK mode:
fkDrivingJoints={-1,-1,-1,-1}
fkDrivingJoints[1]=simGetObjectHandle('Ragnar_A1DrivingJoint1')
fkDrivingJoints[2]=simGetObjectHandle('Ragnar_A1DrivingJoint2')
fkDrivingJoints[3]=simGetObjectHandle('Ragnar_A1DrivingJoint3')
fkDrivingJoints[4]=simGetObjectHandle('Ragnar_A1DrivingJoint4')
-- Following are the joints that we control when in IK mode (we use joints in order to be able to use the simMoveToJointPositions command here too):
ikDrivingJoints={-1,-1,-1,-1}
ikDrivingJoints[1]=simGetObjectHandle('Ragnar_T_X')
ikDrivingJoints[2]=simGetObjectHandle('Ragnar_T_Y')
ikDrivingJoints[3]=simGetObjectHandle('Ragnar_T_X')
ikDrivingJoints[4]=simGetObjectHandle('Ragnar_T_TH')

local ragnarSettings=simReadCustomDataBlock(model,'RAGNAR_CONF')
ragnarSettings=simUnpackTable(ragnarSettings)
toolHandle,stacking,stackingShift=getToolHandleAndStacking()
partTrackingWindowHandle=bwUtils.getReferencedObjectHandle(model,REF_PART_TRACKING1)
auxPartTrackingWindowHandle=bwUtils.getReferencedObjectHandle(model,REF_PART_TRACKING2)
locationTrackingWindowHandle=bwUtils.getReferencedObjectHandle(model,REF_TARGET_TRACKING1)
allSelectedLocationsOrBuckets=getListOfSelectedLocationsOrBuckets(ragnarSettings)
lastPartFromAuxWindow=false
attachedParts={}


_totalPickTime=0
_totalPlaceTime=0
_totalMovementTime=0
_totalCycleTime=0

-- Following 6 are the moving averages:
_auxTrackingWindowPickTime=0
_trackingWindowPickTime=0
_targetTrackingWindowPlaceTime=0
_otherLocationPlaceTime=0
_cycleTime=0
_lossTime=0 


if simBoolAnd32(ragnarSettings['bitCoded'],4096)>0 then
    setFkMode()
    simExtRemoteApiStart(19999)
else



totalCycleTime=0
--totalCycles=0
prepareStatisticsDialog(simBoolAnd32(ragnarSettings['bitCoded'],128)>0)

kinematicsFailedDialogHandle=-1
angularVelocity=2.84*math.pi
angularAccel=15.5*math.pi
linearVelocity=4--0.1        
linearAccel=12.5--0.145
angularGain =0.0 -- FixedPlatfom1

--proMov = 0.9 -- compeltion of approach/depart
--appHight = .1 --meters

-- First, make sure we are in initial position:
setFkMode()
simMoveToJointPositions(fkDrivingJoints,{0,0,0,0},angularVelocity,angularAccel)
setIkMode()
initialPosition=simGetObjectPosition(ikModeTipDummy,model)

--Now with rotation rotation part - meaning DOF 4
curPVA = {initialPosition[1],initialPosition[2],initialPosition[3],0.0,
          0.0,0.0,0.0,0.0,
          0.0,0.0,0.0,0.0} -- pos,vel,acc
nextPVA = curPVA
MaxVAJ = {linearVelocity,linearVelocity,linearVelocity,angularVelocity,
            linearAccel,linearAccel,linearAccel,angularAccel,
            2000,2000,2000,1000} -- pos,vel,acc128*dt
selVec = {1,1,1,1}


    -- 3. Do we have a tracking window and a pick-and-place algorithm?
    local pickAndPlaceAlgo=ragnarSettings['algorithm']

    if (partTrackingWindowHandle>=0 or auxPartTrackingWindowHandle>=0) and pickAndPlaceAlgo then
        -- 4. Load and run the pick-and-place algorithm:
        local algo=assert(loadstring(pickAndPlaceAlgo))
--        cycleStartTime=-1
        algo() -- We stay in here until the end
--[[
        if cycleStartTime>=0 then
            if not startTime then
                startTime=cycleStartTime
            end
            didSomething=true
            local thisCycleTime=simGetSimulationTime()-cycleStartTime
            totalCycleTime=totalCycleTime+thisCycleTime
            if lastPartFromAuxWindow then
                if auxCycleTime then
                    auxCycleTime=(9*auxCycleTime+thisCycleTime)/10 -- kind of moving average calc
                else
                    auxCycleTime=thisCycleTime
                end
            else
                if cycleTime then
                    cycleTime=(9*cycleTime+thisCycleTime)/10 -- kind of moving average calc
                else
                    cycleTime=thisCycleTime
                end
            end
        end
        --]]
    end

    --[[
    -- 5. Avoid using too much processor time when idle
    if not didSomething then
        simSwitchThread()
    end

    updateStatisticsDialog(cycleTime,auxCycleTime,totalCycleTime)
    communicateCycleTimeToAssociatedTrackingWindows(cycleTime,auxCycleTime) 
    --]]
--end
end


