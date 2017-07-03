function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_LOCATION_INFO')
    if data then
        data=simUnpackTable(data)
    else
        data={}
    end
    return data
end

function writeInfo(data)
    if data then
        simWriteCustomDataBlock(model,'XYZ_LOCATION_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_LOCATION_INFO','')
    end
end

function getAvailableBuckets()
    local l=simGetObjectsInTree(sim_handle_scene,sim_handle_all,0)
    local retL={}
    for i=1,#l,1 do
        local data=simReadCustomDataBlock(l[i],'XYZ_BUCKET_INFO')
        if data then
            retL[#retL+1]=l[i]
        end
    end
    return retL
end

function getAvailableTransporters()
    local l=simGetObjectsInTree(sim_handle_scene,sim_handle_all,0)
    local retL={}
    for i=1,#l,1 do
        local data=simReadCustomDataBlock(l[i],'XYZ_TRANSPORTER_INFO')
        if data then
            retL[#retL+1]=l[i]
        end
    end
    return retL
end

function isABucketWithinRange()
    for i=1,#buckets,1 do
        local p=simGetObjectPosition(buckets[i],model)
        local d=math.sqrt(p[1]*p[1]+p[2]*p[2])
        if d<0.1 then
            return true
        end
    end
    return false
end

function isATransporterWithinRange()
    for i=1,#transporters,1 do
        local p=simGetObjectPosition(transporters[i],model)
        local d=math.sqrt(p[1]*p[1]+p[2]*p[2])
        if d<0.1 then
            return true
        end
    end
    return false
end

if (sim_call_type==sim_childscriptcall_initialization) then
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    buckets=getAvailableBuckets()
    transporters=getAvailableTransporters()
end

if (sim_call_type==sim_childscriptcall_sensing) then
    local data=readInfo()
    if isABucketWithinRange() or isATransporterWithinRange() then
        data['status']='occupied'
    else
        if data['status']~='reserved' then
            data['status']='free'
        end
    end
    writeInfo(data)
end


if (sim_call_type==sim_childscriptcall_cleanup) then

	-- Put some restoration code here

end