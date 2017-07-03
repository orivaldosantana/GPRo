local bwUtils=require('bwUtils')

if (sim_call_type==sim_childscriptcall_initialization) then
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    version=simGetInt32Parameter(sim_intparam_program_version)
    local data=simUnpackTable(simReadCustomDataBlock(model,'XYZ_OVERRIDE_INFO'))
    if simBoolAnd32(data['bitCoded'],8)>0 then
        if version>30303 then
            -- Check if some models are coincident
            -- We check only models with tags that start with 'XYZ_' or tags that are 'RAGNAR_CONF' or 'CONVEYOR_CONF'
            local objects=simGetObjectsInTree(sim_handle_scene)
            local modelsWithTags={}
            for i=1,#objects,1 do
                local h=objects[i]
                local p=simGetModelProperty(h)
                if simBoolAnd32(p,sim_modelproperty_not_model)==0 then
                    local tags=simReadCustomDataBlockTags(h)
                    if tags then
                        for j=1,#tags,1 do
                            local tag=tags[j]
                            if string.find(tag,'XYZ_')==1 or tag=='RAGNAR_CONF' or tag=='CONVEYOR_CONF' then
                                if tag~='XYZ_FEEDERPART_INFO' and tag~='XYZ_PARTLABEL_INFO' then
                                    if modelsWithTags[tag]==nil then
                                        modelsWithTags[tag]={}
                                    end
                                    modelsWithTags[tag][#modelsWithTags[tag]+1]=h
                                    break
                                end
                            end
                        end
                    end
                end
            end
            -- We now loop through models with same tags and check if they are coincident (i.e. with a tolerance)
            local hh1=-1
            local hh2=-1
            for key,value in pairs(modelsWithTags) do
                if #value>1 then
                    for i=1,#value-1,1 do
                        local h1=value[i]
                        for j=i+1,#value,1 do
                            local h2=value[j]
                            local p=simGetObjectPosition(h1,h2)
                            local e=simGetObjectOrientation(h1,h2)
                            local dl=math.sqrt(p[1]*p[1]+p[2]*p[2]+p[3]*p[3])
                            local da=math.max(e[1],math.max(e[2],e[3]))
                            if dl<0.01 and da<5*math.pi/180 then
                                hh1=h1
                                hh2=h2
                                break
                            end
                        end
                        if hh1>=0 then
                            break
                        end
                    end
                end
            end
            if hh1>=0 then
                local msg="Detected at least two coincident models: '"..simGetObjectName(hh1).."' and '"..simGetObjectName(hh2).."'. Simulation might not run as expected."
                simMsgBox(sim_msgbox_type_warning,sim_msgbox_buttons_ok,"Coincident Models",msg)
            end
        end
    end
end
