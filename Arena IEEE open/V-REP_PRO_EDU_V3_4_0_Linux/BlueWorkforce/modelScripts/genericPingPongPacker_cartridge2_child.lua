putCartridgeDown=function()
    simWait(dwellTimeUp)
    simRMLMoveToJointPositions({j},-1,{0},{0},{maxVel},{maxAccel},{9999},{0},{0})
    simWait(dwellTimeDown)
    simRMLMoveToJointPositions({j},-1,{0},{0},{maxVel},{maxAccel},{9999},{-45*math.pi/180},{0})
end

enableStopper=function(enable)
    if enable then
        simSetObjectInt32Parameter(stopper,sim_objintparam_visibility_layer,1) -- make it visible
        simSetObjectSpecialProperty(stopper,sim_objectspecialproperty_collidable+sim_objectspecialproperty_measurable+sim_objectspecialproperty_detectable_all+sim_objectspecialproperty_renderable) -- make it collidable, measurable, detectable, etc.
        simSetObjectInt32Parameter(stopper,sim_shapeintparam_respondable,1) -- make it respondable
        simResetDynamicObject(stopper)
    else
        simSetObjectInt32Parameter(stopper,sim_objintparam_visibility_layer,0)
        simSetObjectSpecialProperty(stopper,0)
        simSetObjectInt32Parameter(stopper,sim_shapeintparam_respondable,0)
        simResetDynamicObject(stopper)
    end
end

waitForSensor=function(ind,signal)
    while true do
        local r=simHandleProximitySensor(sens[ind])
        if signal then
            if r>0 then break end
        else
            if r<=0 then break end
        end
        simSwitchThread()
    end
end

enableConveyor=function(enable)
    simSetThreadAutomaticSwitch(false)
    local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
    data=simUnpackTable(data)
    local r=data['stopRequests']
    if enable then
        r[objectHandle]=nil
    else
        r[objectHandle]=true
    end
    data['stopRequests']=r
    simWriteCustomDataBlock(model,'CONVEYOR_CONF',simPackTable(data))
    simSetThreadAutomaticSwitch(true)
end

waitForCartridgeFull=function()
    while true do
        local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
        data=simUnpackTable(data)
        if data['putCartridgeDown'][2] then
            break
        end
        simSwitchThread()
    end
end

setCartridgeEmpty=function()
    simSetThreadAutomaticSwitch(false)
    local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
    data=simUnpackTable(data)
    data['putCartridgeDown'][2]=false
    simWriteCustomDataBlock(model,'CONVEYOR_CONF',simPackTable(data))
    simSetThreadAutomaticSwitch(true)
end

objectHandle=simGetObjectAssociatedWithScript(sim_handle_self)
model=simGetObjectHandle('genericPingPongPacker')
local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
data=simUnpackTable(data)
maxVel=data['cartridgeVelocity']
maxAccel=data['cartridgeAcceleration']
dwellTimeDown=data['cartridgeDwellTimeDown']
dwellTimeUp=data['cartridgeDwellTimeUp']
j=simGetObjectHandle('genericPingPongPacker_cartridge2_upDownJoint')
sens={}
sens[1]=simGetObjectHandle('genericPingPongPacker_cartridge2_sensor')
sens[2]=simGetObjectHandle('genericPingPongPacker_cartridge2_sensor2')
stopper=simGetObjectHandle('genericPingPongPacker_cartridge2_stopper')

while simGetSimulationState()~=sim_simulation_advancing_abouttostop do
    waitForSensor(1,true)
    waitForSensor(1,false)
    waitForSensor(1,true)
    enableStopper(true)
    waitForSensor(2,true)
    enableConveyor(false)
    waitForCartridgeFull()
    putCartridgeDown()
    setCartridgeEmpty()
    enableStopper(false)
    enableConveyor(true)
    waitForSensor(1,false)
end