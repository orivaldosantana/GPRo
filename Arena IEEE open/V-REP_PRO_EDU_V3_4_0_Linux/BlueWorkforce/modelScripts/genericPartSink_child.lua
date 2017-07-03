bwUtils=require('bwUtils')

destroyPartIfPart=function(objH)
    if objH and objH>=0 then
        local isPart,isInstanciated=bwUtils.isObjectPartAndInstanciated(objH)
        if isPart then
            if isInstanciated then
                local p=simGetModelProperty(objH)
                if simBoolAnd32(p,sim_modelproperty_not_model)>0 then
                    simRemoveObject(objH)
                else
                    simRemoveModel(objH)
                end
                return true
            else
                return false
            end
        else
            while objH>=0 do
                objH=simGetObjectParent(objH)
                if objH>=0 then
                    isPart,isInstanciated=bwUtils.isObjectPartAndInstanciated(objH)
                    if isPart then
                        if isInstanciated then
                            simRemoveModel(objH)
                            return true
                        else
                            return false
                        end
                    end
                end
            end
        end
    end
    return false
end

prepareStatisticsDialog=function(enabled)
    if enabled then
        local xml =[[
                <label id="1" text="Part destruction count: 0" style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
        ]]
        statUi=bwUtils.createCustomUi(xml,simGetObjectName(model)..' Statistics','bottomLeft',true--[[,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])
    end
end

updateStatisticsDialog=function(enabled)
    if statUi then
        simExtCustomUI_setLabelText(statUi,1,"Part destruction count: "..destructionCount,true)
    end
end

if (sim_call_type==sim_childscriptcall_initialization) then
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    sensor=simGetObjectHandle('genericPartSink_sensor')
    local data=simReadCustomDataBlock(model,'XYZ_PARTSINK_INFO')
    data=simUnpackTable(data)
    operational=data['status']~='disabled'
    destructionCount=0
    prepareStatisticsDialog(simBoolAnd32(data['bitCoded'],128)>0)
end


if (sim_call_type==sim_childscriptcall_actuation) then
    if operational then
        local shapes=simGetObjectsInTree(sim_handle_scene,sim_object_shape_type)
        for i=1,#shapes,1 do
            if simIsHandleValid(shapes[i])>0 then
                if simBoolAnd32(simGetObjectSpecialProperty(shapes[i]),sim_objectspecialproperty_detectable_all)>0 then
                    local r=simCheckProximitySensor(sensor,shapes[i])
                    if r>0 then
                        if destroyPartIfPart(shapes[i]) then
                            destructionCount=destructionCount+1
                            local data=simReadCustomDataBlock(model,'XYZ_PARTSINK_INFO')
                            data=simUnpackTable(data)
                            data['destroyedCnt']=data['destroyedCnt']+1
                            simWriteCustomDataBlock(model,'XYZ_PARTSINK_INFO',simPackTable(data))
                        end
                    end
                end
            end
        end
    end
    updateStatisticsDialog()
end
