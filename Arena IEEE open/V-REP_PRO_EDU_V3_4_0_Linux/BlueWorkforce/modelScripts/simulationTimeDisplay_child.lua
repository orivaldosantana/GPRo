local bwUtils=require('bwUtils')

if (sim_call_type==sim_childscriptcall_initialization) then
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    local data=simReadCustomDataBlock(model,'XYZ_SIMULATIONTIME_INFO')
    data=simUnpackTable(data)
    simplified=simBoolAnd32(data['bitCoded'],1)==1
    if simplified then
        local xml =[[
                <label text="Time " style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
                <label id="1" text="" style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
        ]]
        ui=bwUtils.createCustomUi(xml,'Time','bottomLeft',true,nil,false,false,false,'layout="form"')
    else
        local xml =[[
                <label text="Simulation time " style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
                <label id="1" text="" style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
                <label text="Real-time " style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
                <label id="2" text="" style="* {font-size: 20px; font-weight: bold; margin-left: 20px; margin-right: 20px;}"/>
        ]]
        ui=bwUtils.createCustomUi(xml,'Simulation Time','bottomLeft',true,nil,false,false,false,'layout="form"')
    end
    startTime=simGetSystemTimeInMs(-1)
end

if (sim_call_type==sim_childscriptcall_sensing) then
    local t={simGetSimulationTime(),simGetSystemTimeInMs(startTime)/1000}
    local cnt=2
    if simplified then
        cnt=1
    end
    for i=1,cnt,1 do
        local v=t[i]
        local hour=math.floor(v/3600)
        v=v-3600*hour
        local minute=math.floor(v/60)
        v=v-60*minute
        local second=math.floor(v)
        v=v-second
        local hs=math.floor(v*100)
        local str=string.format("%02d",hour)..':'..string.format("%02d",minute)..':'..string.format("%02d",second)..'.'..string.format("%02d",hs)
        simExtCustomUI_setLabelText(ui,i,str,true)
    end
end


if (sim_call_type==sim_childscriptcall_cleanup) then
    simExtCustomUI_destroy(ui)
end