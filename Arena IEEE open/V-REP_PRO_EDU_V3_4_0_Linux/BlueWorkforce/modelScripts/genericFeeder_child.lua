local bwUtils=require('bwUtils')

function getPartToDrop(distributionExtent,partDistribution,destinationDistribution,shiftDistribution,rotationDistribution,massDistribution,scalingDistribution,partsData)
    local errString=nil
    local dropName=getDistributionValue(partDistribution)
    local thePartD=partsData[dropName]
    local partToDrop=nil
    if thePartD then
        local destinationName=getDistributionValue(destinationDistribution)
        local dropShift=getDistributionValue(shiftDistribution)
        if not dropShift then dropShift={0,0,0} end
        dropShift[1]=dropShift[1]*distributionExtent[1]
        dropShift[2]=dropShift[2]*distributionExtent[2]
        dropShift[3]=dropShift[3]*distributionExtent[3]
        local dropRotation=getDistributionValue(rotationDistribution)
        local dropMass=getDistributionValue(massDistribution)
        local dropScaling=nil
        if scalingDistribution then
            dropScaling=getDistributionValue(scalingDistribution)
        end
        partToDrop={dropName,destinationName,dropShift,dropRotation,dropMass,dropScaling}
    else
        errString="Warning: part '"..dropName.."' was not found in the part repository (or part repository was not found)."
    end
    return partToDrop,errString
end

setItemMass=function(handle,m)
    if m~=nil then -- Mass can be nil (for a default mass)
        -- Remember, the item can be a shape, or a model containing several shapes
        local currentMass=0
        local objects={handle}
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

        local massScaling=m/currentMass

        local objects={handle}
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
                    local transf=simGetObjectMatrix(handle,-1)
                    local m0,i0,com0=simGetShapeMassAndInertia(handle,transf)
                    for i=1,9,1 do
                        i0[i]=i0[i]*massScaling
                    end
                    simSetShapeMassAndInertia(handle,m0*massScaling,i0,com0,transf)
                end
            end
        end
    end
end

deactivatePart=function(handle,isModel)
    if isModel then
        local p=simGetModelProperty(handle)
        p=simBoolOr32(p,sim_modelproperty_not_dynamic)
        simSetModelProperty(handle,p)
    else
        simSetObjectInt32Parameter(handle,sim_shapeintparam_static,1) -- we make it static now!
    end
    simResetDynamicObject(handle) -- important, otherwise the dynamics engine doesn't notice the change!
end

removePart=function(handle,isModel)
    if isModel then
        simRemoveModel(handle)
    else
        simRemoveObject(handle)
    end
end

getDistributionValue=function(distribution)
    -- Distribution string could be:
    -- {} --> returns nil
    -- {{}} --> returns nil
    -- a,a,b,c --> returns a,b, or c
    -- {{2,a},{1,b},{1,c}} --> returns a,b, or c
    if #distribution>0 then
        if (type(distribution[1])~='table') or (#distribution[1]>0) then
            if (type(distribution[1])=='table') and (#distribution[1]==2) then
                local cnt=0
                for i=1,#distribution,1 do
                   cnt=cnt+distribution[i][1] 
                end
                local p=simGetFloatParameter(sim_floatparam_rand)*cnt
                cnt=0
                for i=1,#distribution,1 do
                    if cnt+distribution[i][1]>=p then
                        return distribution[i][2]
                    end
                    cnt=cnt+distribution[i][1] 
                end
            else
                local cnt=#distribution
                local p=1+math.floor(simGetFloatParameter(sim_floatparam_rand)*cnt-0.0001)
                return distribution[p]
            end
        end
    end
end

getLabels=function(partH)
    -- There can be up to 3 labels in this part:
    local possibleLabels=simGetObjectsInTree(partH,sim_object_shape_type,1)
    local labels={}
    for objInd=1,#possibleLabels,1 do
        local h=possibleLabels[objInd]
        local data=simReadCustomDataBlock(h,'XYZ_PARTLABEL_INFO')
        if data then
            labels[#labels+1]=h
        end
    end
    return labels
end

adjustSizeData=function(partH,sx,sy,sz)
    local data=simUnpackTable(simReadCustomDataBlock(partH,'XYZ_FEEDERPART_INFO'))
    local labelData=data['labelData']
    if labelData then
        local s=labelData['smallLabelSize']
        labelData['smallLabelSize']={s[1]*sx,s[2]*sy}
        local s=labelData['largeLabelSize']
        labelData['largeLabelSize']={s[1]*sx,s[2]*sy}
        local s=labelData['boxSize']
        labelData['boxSize']={s[1]*sx,s[2]*sy,s[3]*sz}
        data['labelData']=labelData
        simWriteCustomDataBlock(partH,'XYZ_FEEDERPART_INFO',simPackTable(data))
    end
end

regenerateOrRemoveLabels=function(partH,enabledLabels)
    -- There can be up to 3 labels in this part:
    local possibleLabels=simGetObjectsInTree(partH,sim_object_shape_type,1)
    local labelData=simUnpackTable(simReadCustomDataBlock(partH,'XYZ_FEEDERPART_INFO'))['labelData']
    for ind=1,3,1 do
        for objInd=1,#possibleLabels,1 do
            local h=possibleLabels[objInd]
            if h>=0 then
                local data=simReadCustomDataBlock(h,'XYZ_PARTLABEL_INFO')
                if data then
                    data=simUnpackTable(data)
                    if data['labelIndex']==ind then
                        local bits={1,2,4}
                        if (simBoolAnd32(bits[ind],enabledLabels)>0) then
                            -- We want to regenerate the position of this label
                            if labelData then
                                local bitC=labelData['bitCoded']
                                local smallLabelSize=labelData['smallLabelSize']
                                local largeLabelSize=labelData['largeLabelSize']
                                local useLargeLabel=(simBoolAnd32(bitC,64*(2^(ind-1)))>0)
                                local labelSize=smallLabelSize
                                if useLargeLabel then
                                    labelSize=largeLabelSize
                                end
                                local code=labelData['placementCode'][ind]
                                local toExecute='local boxSizeX='..labelData['boxSize'][1]..'\n'
                                toExecute=toExecute..'local boxSizeY='..labelData['boxSize'][2]..'\n'
                                toExecute=toExecute..'local boxSizeZ='..labelData['boxSize'][3]..'\n'
                                toExecute=toExecute..'local labelSizeX='..labelSize[1]..'\n'
                                toExecute=toExecute..'local labelSizeY='..labelSize[2]..'\n'
                                toExecute=toExecute..'local labelRadius='..(0.5*math.sqrt(labelSize[1]*labelSize[1]+labelSize[2]*labelSize[2]))..'\n'

                                toExecute=toExecute..'return {'..code..'}'
                                local theTable=executeCode(toExecute)
                                simSetObjectPosition(h,partH,theTable[1])
                                simSetObjectOrientation(h,partH,theTable[2])
                            end
                        else
                            simRemoveObject(h) -- we do not want this label
                            possibleLabels[objInd]=-1
                        end
                    end
                end
            end
        end
    end
end

makeInvisibleOrNonRespondableToOtherParts=function(handle,invisible,nonRespondableToOtherParts)
    if invisible then
        local objs=simGetObjectsInTree(handle)
        for i=1,#objs,1 do
            simSetObjectInt32Parameter(objs[i],sim_objintparam_visibility_layer,0)
            local p=simGetObjectSpecialProperty(objs[i])
            local p=simBoolOr32(p,sim_objectspecialproperty_renderable)-sim_objectspecialproperty_renderable
            simSetObjectSpecialProperty(objs[i],p)
        end
    end
    objs=simGetObjectsInTree(handle,sim_object_shape_type)
    for i=1,#objs,1 do
        local r,m=simGetObjectInt32Parameter(objs[i],sim_shapeintparam_respondable_mask)
        if nonRespondableToOtherParts then
            simSetObjectInt32Parameter(objs[i],sim_shapeintparam_respondable_mask,simBoolOr32(m,65280)-32512)
        else
            simSetObjectInt32Parameter(objs[i],sim_shapeintparam_respondable_mask,simBoolOr32(m,65280)-32768)
        end
    end
end

getSensorState=function()
    if sensorHandle>=0 then
        local data=simReadCustomDataBlock(sensorHandle,'XYZ_BINARYSENSOR_INFO')
        data=simUnpackTable(data)
        return data['detectionState']
    end
    return 0
end

getConveyorDistanceTrigger=function()
    if conveyorHandle>=0 then
        local data=simReadCustomDataBlock(conveyorHandle,'CONVEYOR_CONF')
        if data then
            data=simUnpackTable(data)
            local d=data['encoderDistance']
            if d then
                if not lastConveyorDistance then
                    lastConveyorDistance=d
                end
                if math.abs(lastConveyorDistance-d)>conveyorTriggerDist then
                    lastConveyorDistance=d
                    return true
                end
            end
        end
    end
    return false
end

prepareStatisticsDialog=function(enabled)
    if enabled then
        local xml =[[
                <label id="1" text="Part production count: 0" style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
        ]]
        statUi=bwUtils.createCustomUi(xml,simGetObjectName(model)..' Statistics','bottomLeft',true--[[,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])
    end
end

updateStatisticsDialog=function(enabled)
    if statUi then
        simExtCustomUI_setLabelText(statUi,1,"Part production count: "..productionCount,true)
    end
end

wasMultiFeederTriggered=function()
    local data=simUnpackTable(simReadCustomDataBlock(model,'XYZ_FEEDER_INFO'))
    local val=data['multiFeederTriggerCnt']
    if val and val~=multiFeederTriggerLastState then
        multiFeederTriggerLastState=val
        return true
    end
    return false
end

if (sim_call_type==sim_childscriptcall_initialization) then
    REF_SENSOR=1 -- index of referenced sensor object handle
    REF_CONVEYOR=2 -- index of referenced conveyor object handle
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    producedPartsDummy=simGetObjectHandle('genericFeeder_ownedParts')
    smallLabel=simGetObjectHandle('genericFeeder_smallLabel')
    largeLabel=simGetObjectHandle('genericFeeder_largeLabel')
    local data=simReadCustomDataBlock(model,'XYZ_FEEDER_INFO')
    data=simUnpackTable(data)
    prepareStatisticsDialog(simBoolAnd32(data['bitCoded'],128)>0)
    productionCount=0
    sensorHandle=bwUtils.getReferencedObjectHandle(model,REF_SENSOR)
    conveyorHandle=bwUtils.getReferencedObjectHandle(model,REF_CONVEYOR)
    conveyorTriggerDist=data['conveyorDist']
    mode=0 -- 0=frequency, 1=sensor, 2=user, 3=conveyor, 4=multi-feeder
    local tmp=simBoolAnd32(data['bitCoded'],4+8+16)
    if tmp==4 then mode=1 end
    if tmp==8 then mode=2 end
    if tmp==12 then mode=3 end
    if tmp==16 then mode=4 end
    sensorLastState=0
    multiFeederTriggerLastState=0

    local parts=bwUtils.getAllPartsFromPartRepository()
    partsData={}
    if parts then
        for i=1,#parts,1 do
            local h=parts[i][2]
            local data=simReadCustomDataBlock(h,'XYZ_FEEDERPART_INFO')
            data=simUnpackTable(data)
            data['handle']=h
            partsData[data['name']]=data
        end
    else
        simAddStatusbarMessage('\nWarning: no part repository found in the scene.\n')
    end
    allProducedParts={}
    timeForIdlePartToDeactivate=bwUtils.modifyPartDeactivationTime(data['deactivationTime'])
    counter=0
end

if (sim_call_type==sim_childscriptcall_actuation) then
    local t=simGetSimulationTime()
    local dt=simGetSimulationTimeStep()

    local data=simReadCustomDataBlock(model,'XYZ_FEEDER_INFO')
    data=simUnpackTable(data)
    local distributionExtent={data['length'],data['width'],data['height']}
    local dropFrequency=data['frequency']
    local feederAlgo=data['algorithm']
    if simBoolAnd32(data['bitCoded'],2)>0 then
        -- The feeder is enabled
        local partDistribution='{'..data['partDistribution']..'}'
        local f=loadstring("return "..partDistribution)
        partDistribution=f()
        local destinationDistribution='{'..data['destinationDistribution']..'}'
        local f=loadstring("return "..destinationDistribution)
        destinationDistribution=f()
        local shiftDistribution='{'..data['shiftDistribution']..'}'
        local f=loadstring("return "..shiftDistribution)
        shiftDistribution=f()
        local rotationDistribution='{'..data['rotationDistribution']..'}'
        local f=loadstring("return "..rotationDistribution)
        rotationDistribution=f()
        local massDistribution='{'..data['weightDistribution']..'}'
        local f=loadstring("return "..massDistribution)
        massDistribution=f()
        local labelDistribution='{'..data['labelDistribution']..'}'
        local f=loadstring("return "..labelDistribution)
        labelDistribution=f()

        local scalingDistribution=nil
        if data['sizeScaling'] and data['sizeScaling']>0 then
            if data['sizeScaling']==1 then
                scalingDistribution='{'..data['isoSizeScalingDistribution']..'}'
            end
            if data['sizeScaling']==2 then
                scalingDistribution='{'..data['nonIsoSizeScalingDistribution']..'}'
            end
            local f=loadstring("return "..scalingDistribution)
            scalingDistribution=f()
        end

        if true then --feederAlgo then
            local sensorState=getSensorState()
            local partToDrop=nil
            local errStr=nil
            local t=simGetSimulationTime()
            if mode==0 then
                -- Frequency triggered
                if not lastDropTime then
                    lastDropTime=t-9999
                end
                if t-lastDropTime>(1/dropFrequency) then
                    lastDropTime=t
                    partToDrop,errStr=getPartToDrop(distributionExtent,partDistribution,destinationDistribution,shiftDistribution,rotationDistribution,massDistribution,scalingDistribution,partsData)
                end
            end
            if mode==1 then
                -- Sensor triggered
                if sensorState~=sensorLastState then
                    partToDrop,errStr=getPartToDrop(distributionExtent,partDistribution,destinationDistribution,shiftDistribution,rotationDistribution,massDistribution,scalingDistribution,partsData)
                end
            end
            if mode==3 and getConveyorDistanceTrigger() then
                -- Conveyor belt distance triggered
                partToDrop,errStr=getPartToDrop(distributionExtent,partDistribution,destinationDistribution,shiftDistribution,rotationDistribution,massDistribution,scalingDistribution,partsData)
            end
            if mode==2 then
                -- User triggered
                local algo=assert(loadstring(feederAlgo))
                if algo() then
                    partToDrop,errStr=getPartToDrop(distributionExtent,partDistribution,destinationDistribution,shiftDistribution,rotationDistribution,massDistribution,scalingDistribution,partsData)
                end
            end
            if mode==4 then
                -- Multi-feeder triggered
                if wasMultiFeederTriggered() then
                    partToDrop,errStr=getPartToDrop(distributionExtent,partDistribution,destinationDistribution,shiftDistribution,rotationDistribution,massDistribution,scalingDistribution,partsData)
                end
            end
            if errStr then
                simAddStatusbarMessage('\n'..errStr..'\n')
            end
            sensorLastState=sensorState
            if partToDrop then
                counter=counter+1
                local itemName=partToDrop[1]
                local itemDestination=partToDrop[2]
                local itemPosition=partToDrop[3]
                local itemOrientation=partToDrop[4]
                local itemMass=partToDrop[5]
                local itemScaling=partToDrop[6]
                local dat=partsData[itemName]
                if dat then
                    productionCount=productionCount+1
                    local h=dat['handle']
                    local p=simGetModelProperty(h)
                    local isModel=simBoolAnd16(p,sim_modelproperty_not_model)==0
                    local tble
                    if isModel then
                        tble=simCopyPasteObjects({h},1)
                    else
                        tble=simCopyPasteObjects({h},0)
                    end
                    h=tble[1]
                    simSetObjectParent(h,producedPartsDummy,true)
                    local data=simReadCustomDataBlock(h,'XYZ_FEEDERPART_INFO')
                    data=simUnpackTable(data)
                    local invisible=simBoolAnd32(data['bitCoded'],1)>0
                    local nonRespondableToOtherParts=simBoolAnd32(data['bitCoded'],2)>0
                    makeInvisibleOrNonRespondableToOtherParts(h,invisible,nonRespondableToOtherParts)
                    
                    -- Destination:
                    if itemDestination and itemDestination~='<DEFAULT>' then
                        data['destination']=itemDestination
                    end

                    -- Size scaling:
                    if itemScaling then
                        local itemLabels=getLabels(h)
                        for j=1,#itemLabels,1 do
                            simSetObjectParent(itemLabels[j],-1,true)
                        end
                        if type(itemScaling)~='table' then
                            -- iso-scaling
                            adjustSizeData(h,itemScaling,itemScaling,itemScaling)
                            simScaleObjects({h},itemScaling,false)
                        else
                            -- non-iso-scaling
                            adjustSizeData(h,itemScaling[1],itemScaling[2],itemScaling[3])
                            if isModel then
                                if canScaleModelNonIsometrically(h,itemScaling[1],itemScaling[2],itemScaling[3]) then
                                    scaleModelNonIsometrically(h,itemScaling[1],itemScaling[2],itemScaling[3])
                                end
                            else
                                if canScaleObjectNonIsometrically(h,itemScaling[1],itemScaling[2],itemScaling[3]) then
                                    simScaleObject(h,itemScaling[1],itemScaling[2],itemScaling[3],0)
                                end
                            end
                        end
                        for j=1,#itemLabels,1 do
                            simSetObjectParent(itemLabels[j],h,true)
                        end
                    end
                    
                    -- Mass:
                    if itemMass and itemMass~='<DEFAULT>' then
                        setItemMass(h,itemMass)
                    end

                   -- Labels:
                    local enabledLabels=getDistributionValue(labelDistribution)
                    if invisible then
                        enabledLabels=0
                    end
                    if enabledLabels and enabledLabels>=0 then
                        regenerateOrRemoveLabels(h,enabledLabels)
                    end
                    if not itemPosition then
                        itemPosition={0,0,0} -- default
                    end
                    if not itemOrientation then
                        itemOrientation={0,0,0} -- default
                    end

                    -- Position:
                    simSetObjectPosition(h,model,itemPosition)

                    -- Orientation:
                    simSetObjectOrientation(h,model,itemOrientation)

                    data['instanciated']=true
                    simWriteCustomDataBlock(h,'XYZ_FEEDERPART_INFO',simPackTable(data))
                    
                    p=simGetObjectPosition(h,-1)
                    local partData={h,t,p,isModel,true} -- handle, lastMovingTime, lastPosition, isModel, isActive
                    allProducedParts[#allProducedParts+1]=partData
                end
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
                    deactivatePart(h,allProducedParts[i][4])
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
    updateStatisticsDialog()
end 