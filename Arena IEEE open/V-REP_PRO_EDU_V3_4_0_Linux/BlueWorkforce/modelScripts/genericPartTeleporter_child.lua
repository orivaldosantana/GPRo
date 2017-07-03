local bwUtils=require('bwUtils')

function getObjectHandle_noErrorNoSuffixAdjustment(name)
    local err=simGetInt32Parameter(sim_intparam_error_report_mode)
    simSetInt32Parameter(sim_intparam_error_report_mode,0)
    local suff=simGetNameSuffix(nil)
    simSetNameSuffix(-1)
    retVal=simGetObjectHandle(name)
    simSetNameSuffix(suff)
    simSetInt32Parameter(sim_intparam_error_report_mode,err)
    return retVal
end

function getAllParts()
    local l=simGetObjectsInTree(sim_handle_scene,sim_object_shape_type,0)
    local retL={}
    for i=1,#l,1 do
        local data=simReadCustomDataBlock(l[i],'XYZ_FEEDERPART_INFO')
        if data then
            retL[#retL+1]=l[i]
        end
    end
    return retL
end

function isPartDetected(partHandle)
    local p=simGetObjectPosition(partHandle,model)
    return math.abs(p[1])<xSize*0.5 and math.abs(p[2])<ySize*0.5 and math.abs(p[3])<zSize*0.5
end

isEnabled=function()
    local data=simReadCustomDataBlock(model,'XYZ_PARTTELEPORTER_INFO')
    data=simUnpackTable(data)
    return simBoolAnd32(data['bitCoded'],1)>0
end

getSensorPart=function()
    local p=getAllParts()
    for i=1,#p,1 do
        if isPartDetected(p[i]) then
            return p[i]
        end
    end
    return -1
end

if (sim_call_type==sim_childscriptcall_initialization) then
    REF_DESTINATION=1 -- index of referenced destination pod object handle
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    local data=simReadCustomDataBlock(model,'XYZ_PARTTELEPORTER_INFO')
    data=simUnpackTable(data)
    isSource=simBoolAnd32(data['bitCoded'],2)>0
    xSize=data['width']
    ySize=data['length']
    zSize=data['height']
    destinationPod=bwUtils.getReferencedObjectHandle(model,REF_DESTINATION)
    if destinationPod>=0 then
        local dataD=simReadCustomDataBlock(destinationPod,'XYZ_PARTTELEPORTER_INFO')
        if dataD then
            dataD=simUnpackTable(dataD)
            if simBoolAnd32(dataD['bitCoded'],2)==0 then
                xSizeD=dataD['width']
                ySizeD=dataD['length']
                zSizeD=dataD['height']
            else
                destinationPod=-1
            end
        else
            destinationPod=-1
        end
    end
end

if (sim_call_type==sim_childscriptcall_actuation) then
    if isSource and destinationPod>=0 and isEnabled() then
        local part=getSensorPart()
        if part>=0 then
            local m=simGetObjectMatrix(part,model)
            m[4]=xSizeD*m[4]/xSize
            m[8]=ySizeD*m[8]/ySize
            m[12]=zSizeD*m[12]/zSize
            simSetObjectMatrix(part,destinationPod,m)
            local p=simGetModelProperty(part)
            if simBoolAnd32(p,sim_modelproperty_not_model)==0 then
                -- We have a model
                local l=simGetObjectsInTree(part)
                for i=1,#l,1 do
                    simResetDynamicObject(l[i])
                end
            else
                -- We have a shape
                simResetDynamicObject(part)
            end
        end
    end
end
