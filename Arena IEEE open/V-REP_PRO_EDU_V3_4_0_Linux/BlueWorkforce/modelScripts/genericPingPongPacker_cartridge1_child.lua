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

waitForSensor=function(signal)
    while true do
        local r=simHandleProximitySensor(sens)
        if signal then
            if r>0 then break end
        else
            if r<=0 then break end
        end
        simSwitchThread()
    end
end

waitForCartridgeFull=function()
    while true do
        local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
        data=simUnpackTable(data)
        if data['putCartridgeDown'][1] then
            break
        end
        simSwitchThread()
    end
end

setCartridgeEmpty=function()
    simSetThreadAutomaticSwitch(false)
    local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
    data=simUnpackTable(data)
    data['putCartridgeDown'][1]=false
    simWriteCustomDataBlock(model,'CONVEYOR_CONF',simPackTable(data))
    simSetThreadAutomaticSwitch(true)
end

model=simGetObjectHandle('genericPingPongPacker')
local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
data=simUnpackTable(data)
maxVel=data['cartridgeVelocity']
maxAccel=data['cartridgeAcceleration']
dwellTimeDown=data['cartridgeDwellTimeDown']
dwellTimeUp=data['cartridgeDwellTimeUp']
j=simGetObjectHandle('genericPingPongPacker_cartridge1_upDownJoint')
sens=simGetObjectHandle('genericPingPongPacker_cartridge1_sensor')
stopper=simGetObjectHandle('genericPingPongPacker_cartridge1_stopper')

while simGetSimulationState()~=sim_simulation_advancing_abouttostop do
    waitForSensor(true)
    enableStopper(true)
    simWait(1)
    waitForCartridgeFull()
    putCartridgeDown()
    setCartridgeEmpty()
    enableStopper(false)
    waitForSensor(false)
    waitForSensor(true)
    waitForSensor(false)
end


