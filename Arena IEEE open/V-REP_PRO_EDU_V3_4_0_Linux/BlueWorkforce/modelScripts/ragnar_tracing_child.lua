local bwUtils=require('bwUtils')

getRobotHandle=function(objectHandle)
    while true do
        local p=simGetModelProperty(objectHandle)
        if simBoolAnd32(p,sim_modelproperty_not_model)==0 then
            return objectHandle
        end
        objectHandle=simGetObjectParent(objectHandle)
    end
end

getColorFromIntensity=function(intensity)
    local col={0.16,0.16,0.16,0.16,0.16,1,1,0.16,0.16,1,1,0.16}
    if intensity>1 then intensity=1 end
    if intensity<0 then intensity=0 end
    intensity=math.exp(4*(intensity-1))
    local d=math.floor(intensity*3)
    if (d>2) then d=2 end
    local r=(intensity-d/3)*3
    local coll={}
    coll[1]=col[3*d+1]*(1-r)+col[3*(d+1)+1]*r
    coll[2]=col[3*d+2]*(1-r)+col[3*(d+1)+2]*r
    coll[3]=col[3*d+3]*(1-r)+col[3*(d+1)+3]*r
    return coll
end

if (sim_call_type==sim_childscriptcall_initialization) then
    dummy=simGetObjectAssociatedWithScript(sim_handle_self)
    model=getRobotHandle(dummy)
    local ragnarSettings=simReadCustomDataBlock(model,'RAGNAR_CONF')
    ragnarSettings=simUnpackTable(ragnarSettings)
    showTrajectory=bwUtils.modifyAuxVisualizationItems(simBoolAnd32(ragnarSettings['bitCoded'],1)>0)
    if showTrajectory then
        local lineBufferSize=100
        cont=simAddDrawingObject(sim_drawing_lines+sim_drawing_itemcolors+sim_drawing_emissioncolor+sim_drawing_cyclic,3,0,-1,lineBufferSize)
        prevPos=simGetObjectPosition(dummy,-1)
        prevTime=simGetSimulationTime()
    end
end


if (sim_call_type==sim_childscriptcall_sensing) then
    if showTrajectory then
        local ragnarSettings=simReadCustomDataBlock(model,'RAGNAR_CONF')
        ragnarSettings=simUnpackTable(ragnarSettings)
        local maxSpeed=ragnarSettings.maxVel

        local p=simGetObjectPosition(dummy,-1)
        local t=simGetSimulationTime()
        local dx={p[1]-prevPos[1],p[2]-prevPos[2],p[3]-prevPos[3]}
        local dt=t-prevTime
        local l=math.sqrt(dx[1]*dx[1]+dx[2]*dx[2]+dx[3]*dx[3])
        local speed=0
        if dt>0 then
            speed=l/dt
        end
        local c=getColorFromIntensity(speed/maxSpeed)
        local data={prevPos[1],prevPos[2],prevPos[3],p[1],p[2],p[3],c[1],c[2],c[3]}
        simAddDrawingObjectItem(cont,data)
        prevPos={p[1],p[2],p[3]}
        prevTime=t
    end
end
