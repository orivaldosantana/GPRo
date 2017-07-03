local bwUtils=require('bwUtils')

prepareStatisticsDialog=function(enabled)
    if enabled then
        local xml = [[
                <label id="1" text="BLA" style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
        ]]
        statUi=bwUtils.createCustomUi(xml,simGetObjectName(model)..' Statistics','bottomLeft',true--[[,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])
    end
end

updateStatisticsDialog=function()
    if statUi then
        simExtCustomUI_setLabelText(statUi,1,statText..string.format("%.0f",incrementer),true)
    end
end

function getAllParts()
    local l=simGetObjectsInTree(sim_handle_scene,sim_object_shape_type,0)
    local retL={}
    for i=1,#l,1 do
        local isPart,isInstanciated,data=bwUtils.isObjectPartAndInstanciated(l[i])
        if isInstanciated then
            retL[#retL+1]=l[i]
        end
    end
    return retL
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

isEnabled=function()
    local data=simReadCustomDataBlock(model,'XYZ_BINARYSENSOR_INFO')
    data=simUnpackTable(data)
    return simBoolAnd32(data['bitCoded'],1)>0
end

setDetectionState=function(v)
    local data=simReadCustomDataBlock(model,'XYZ_BINARYSENSOR_INFO')
    data=simUnpackTable(data)
    data['detectionState']=v
    simWriteCustomDataBlock(model,'XYZ_BINARYSENSOR_INFO',simPackTable(data))
end

checkSensor=function()
    if detectPartsOnly then
        local p=getAllParts()
        for i=1,#p,1 do
            if isPartDetected(p[i]) then
                return true
            end
        end
    else
        local result=simHandleProximitySensor(sensor)
        return result>0
    end
    return false
end

if (sim_call_type==sim_childscriptcall_initialization) then
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    sensor=simGetObjectHandle('genericBinarySensor_sensor')
    local data=simReadCustomDataBlock(model,'XYZ_BINARYSENSOR_INFO')
    data=simUnpackTable(data)
    detectPartsOnly=simBoolAnd32(data['bitCoded'],2)>0
    onRise=simBoolAnd32(data['bitCoded'],8)>0
    onFall=simBoolAnd32(data['bitCoded'],16)>0
    countForTrigger=data['countForTrigger']
    showStats=simBoolAnd32(data['bitCoded'],32)>0
    statText=data['statText']
    delayForTrigger=data['delayForTrigger']
    prepareStatisticsDialog(showStats)
    previousDetectionState=false
    detectionCount=0
    incrementTimes={}
    incrementer=0
end

if (sim_call_type==sim_childscriptcall_sensing) then
    local t=simGetSimulationTime()
    local dt=simGetSimulationTimeStep()
    if isEnabled() then
        local r=checkSensor()
        if r~=previousDetectionState then
            if r then
                if onRise then
                   detectionCount=detectionCount+1
                end
            else
                if onFall then
                   detectionCount=detectionCount+1
                end
            end
            if detectionCount>=countForTrigger then
                detectionCount=0
                incrementTimes[#incrementTimes+1]=t+delayForTrigger
            end
        end
        previousDetectionState=r
    end

    local changeCol=false
    local i=1
    while i<=#incrementTimes do
        if incrementTimes[i]<t+dt then
            incrementer=incrementer+1
            setDetectionState(incrementer)
            changeCol=true
            table.remove(incrementTimes,i)
        else
            i=i+1
        end
    end

    if changeCol then
        simSetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse,{1,0,0})
    else
        simSetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse,{0,0,1})
    end
    updateStatisticsDialog()
end

if (sim_call_type==sim_childscriptcall_cleanup) then
        simSetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse,{0,0,1})
end