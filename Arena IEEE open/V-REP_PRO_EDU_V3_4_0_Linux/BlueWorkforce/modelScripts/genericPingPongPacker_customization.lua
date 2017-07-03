local bwUtils=require('bwUtils')

function removeFromPluginRepresentation()

end

function updatePluginRepresentation()

end

function setObjectSize(h,x,y,z)
    local r,mmin=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_min_x)
    local r,mmax=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_max_x)
    local sx=mmax-mmin
    local r,mmin=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_min_y)
    local r,mmax=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_max_y)
    local sy=mmax-mmin
    local r,mmin=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_min_z)
    local r,mmax=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_max_z)
    local sz=mmax-mmin
    simScaleObject(h,x/sx,y/sy,z/sz)
end

function getDefaultInfoForNonExistingFields(info)
    if not info['version'] then
        info['version']=_MODELVERSION_
    end
    if not info['subtype'] then
        info['subtype']='pingPong'
    end
    if not info['velocity'] then
        info['velocity']=0.1
    end
    if not info['acceleration'] then
        info['acceleration']=0.01
    end
    if not info['length'] then
        info['length']=1
    end
    if not info['width'] then
        info['width']=0.3
    end
    if not info['height'] then
        info['height']=0.1
    end
    if not info['borderHeight'] then
        info['borderHeight']=0.2
    end
    if not info['bitCoded'] then
        info['bitCoded']=1+2+4+8 -- 64 enabled,128=showPoints,256=rotated
    end
    if not info['wallThickness'] then
        info['wallThickness']=0.005
    end
    if not info['stopRequests'] then
        info['stopRequests']={}
    end
    if not info['cartridge1PosOffset'] then
        info['cartridge1PosOffset']={0,0,0}
    end
    if not info['cartridge2PosOffset'] then
        info['cartridge2PosOffset']=0.5
    end
    if not info['cartridgeWidth'] then
        info['cartridgeWidth']=0.3
    end
    if not info['cartridgeDivisions'] then
        info['cartridgeDivisions']=0
    end
    if not info['cartridgeWallThickness'] then
        info['cartridgeWallThickness']=0.002
    end
    if not info['stopperOffsets'] then
        info['stopperOffsets']=-0.01
    end
    if not info['cartridgeVelocity'] then
        info['cartridgeVelocity']=45*math.pi/180
    end
    if not info['cartridgeAcceleration'] then
        info['cartridgeAcceleration']=45*math.pi/180
    end
    if not info['cartridgeDwellTimeDown'] then
        info['cartridgeDwellTimeDown']=1
    end
    if not info['cartridgeDwellTimeUp'] then
        info['cartridgeDwellTimeUp']=0.5
    end
    if not info['cartridgeRackOffset'] then
        info['cartridgeRackOffset']=0
    end
    if not info['cartridgeRackOffset'] then
        info['cartridgeRackOffset']=0
    end
    if not info['palletData'] then
        info['palletData']={{0,0,0},6,0.05} -- offset, cnt, separation
    end
    if not info['palletItems'] then
        info['palletItems']={{},{}}
    end
    if not info['putCartridgeDown'] then
        info['putCartridgeDown']={{false},{false}}
    end
    if not info['locationName'] then
        info['locationName']='<LOCATION_NAME>'
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'CONVEYOR_CONF')
    if data then
        data=simUnpackTable(data)
    else
        data={}
    end
    getDefaultInfoForNonExistingFields(data)
    return data
end

function writeInfo(data)
    if data then
        simWriteCustomDataBlock(model,'CONVEYOR_CONF',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'CONVEYOR_CONF','')
    end
end

function generatePallet()
    local conf=readInfo()
    local cwidth=conf['cartridgeWidth']
    local divs=conf['cartridgeDivisions']
    for cart=1,2,1 do
        -- Remove all dummies that are targets:
        while true do
            local h=simGetObjectChild(cartridges[cart]['targets'],0)
            if h<0 then break end
            simRemoveObject(h)
        end

        -- Create them again:
        local palletOffset=conf['palletData'][1]
        local palletRows=conf['palletData'][2]
        local palletSeparation=conf['palletData'][3]
        local palletItems={}

        for i=1,palletRows,1 do
            local dy=cwidth/(divs+1)
            local yo=-dy
            if divs==0 then
                yo=0
            end
            if divs==1 then
                yo=-dy/2
            end
            for j=0,divs,1 do
                local item={}
                local pos={palletOffset[1]+(i-1)*palletSeparation,palletOffset[2]+yo,palletOffset[3]}
                local dummy=simCreateDummy(0.01)
                local prop=simBoolOr32(simGetObjectProperty(dummy),sim_objectproperty_dontshowasinsidemodel+sim_objectproperty_selectmodelbaseinstead)
                simSetObjectProperty(dummy,prop)
                simSetObjectPosition(dummy,cartridges[cart]['centerH'],pos)
                simSetObjectOrientation(dummy,cartridges[cart]['centerH'],{0,0,0})
                simSetObjectParent(dummy,cartridges[cart]['targets'],true)
                item['processingStage']=0
                item['ser']=#palletItems
                item['layer']=i
                item['dummyHandle']=dummy
                palletItems[#palletItems+1]=item
                yo=yo+dy
            end
        end
        local cinfo=readInfo()
        cinfo['palletItems'][cart]=palletItems
        writeInfo(cinfo)
    end
end

function updateConveyor()
    local conf=readInfo()
    local length=conf['length']
    local width=conf['width']
    local height=conf['height']
    local borderHeight=conf['borderHeight']
    local bitCoded=conf['bitCoded']
    local wt=conf['wallThickness']
    local cwt=conf['cartridgeWallThickness']
    local cwidth=conf['cartridgeWidth']
    local clength=width*0.90
    local addShift=(width-clength)*0--0.5
    local divs=conf['cartridgeDivisions']
    local stopperOffset=conf['stopperOffsets']
    local rackOffset=conf['cartridgeRackOffset']
    local rotated=simBoolAnd32(bitCoded,256)>0
    local direction=1
    if rotated then direction=-1 end

    local re=simBoolAnd32(bitCoded,16)==0
---[[
    simSetObjectPosition(rotJoints[1],model,{0,-length*0.5,-height*0.5})
    simSetObjectPosition(rotJoints[2],model,{0,length*0.5,-height*0.5})

    setObjectSize(middleParts[1],width,length,height)
    setObjectSize(middleParts[2],width,length,0.001)
    setObjectSize(middleParts[3],width,length,height)
    simSetObjectPosition(middleParts[1],model,{0,0,-height*0.5})
    simSetObjectPosition(middleParts[2],model,{0,0,-0.0005})
    simSetObjectPosition(middleParts[3],model,{0,0,-height*0.5})

    setObjectSize(endParts[1],width,0.083148*height/0.2,0.044443*height/0.2)
    simSetObjectPosition(endParts[1],model,{0,-length*0.5-0.5*0.083148*height/0.2,-0.044443*height*0.5/0.2})

    setObjectSize(endParts[2],width,0.083148*height/0.2,0.044443*height/0.2)
    simSetObjectPosition(endParts[2],model,{0,length*0.5+0.5*0.083148*height/0.2,-0.044443*height*0.5/0.2})

    setObjectSize(endParts[3],width,height*0.5,height)
    simSetObjectPosition(endParts[3],model,{0,-length*0.5-0.25*height,-height*0.5})

    setObjectSize(endParts[4],width,height*0.5,height)
    simSetObjectPosition(endParts[4],model,{0,length*0.5+0.25*height,-height*0.5})

    for i=5,6,1 do
        setObjectSize(endParts[i],height,height,width)
    end

    setObjectSize(sides[1],wt,length,height+borderHeight)
    setObjectSize(sides[2],wt,length,height+borderHeight)
    simSetObjectPosition(sides[1],model,{-(width+wt)*0.5,0,(-height+borderHeight)*0.5})
    simSetObjectPosition(sides[2],model,{(width+wt)*0.5,0,(-height+borderHeight)*0.5})

    if re then
        simSetObjectInt32Parameter(endParts[1],sim_objintparam_visibility_layer,1)
        simSetObjectInt32Parameter(endParts[2],sim_objintparam_visibility_layer,1)
        simSetObjectInt32Parameter(endParts[3],sim_objintparam_visibility_layer,1)
        simSetObjectInt32Parameter(endParts[4],sim_objintparam_visibility_layer,1)
        simSetObjectInt32Parameter(endParts[5],sim_objintparam_visibility_layer,256)
        simSetObjectInt32Parameter(endParts[6],sim_objintparam_visibility_layer,256)
        simSetObjectInt32Parameter(endParts[5],sim_shapeintparam_respondable,1)
        simSetObjectInt32Parameter(endParts[6],sim_shapeintparam_respondable,1)
    else
        simSetObjectInt32Parameter(endParts[1],sim_objintparam_visibility_layer,0)
        simSetObjectInt32Parameter(endParts[2],sim_objintparam_visibility_layer,0)
        simSetObjectInt32Parameter(endParts[3],sim_objintparam_visibility_layer,0)
        simSetObjectInt32Parameter(endParts[4],sim_objintparam_visibility_layer,0)
        simSetObjectInt32Parameter(endParts[5],sim_objintparam_visibility_layer,0)
        simSetObjectInt32Parameter(endParts[6],sim_objintparam_visibility_layer,0)
        simSetObjectInt32Parameter(endParts[5],sim_shapeintparam_respondable,0)
        simSetObjectInt32Parameter(endParts[6],sim_shapeintparam_respondable,0)
    end

    if simBoolAnd32(bitCoded,32)==0 then
        local textureID=simGetShapeTextureId(textureHolder)
        simSetShapeTexture(middleParts[2],textureID,sim_texturemap_plane,12,{0.04,0.04})
        simSetShapeTexture(endParts[1],textureID,sim_texturemap_plane,12,{0.04,0.04})
        simSetShapeTexture(endParts[2],textureID,sim_texturemap_plane,12,{0.04,0.04})
    else
        simSetShapeTexture(middleParts[2],-1,sim_texturemap_plane,12,{0.04,0.04})
        simSetShapeTexture(endParts[1],-1,sim_texturemap_plane,12,{0.04,0.04})
        simSetShapeTexture(endParts[2],-1,sim_texturemap_plane,12,{0.04,0.04})
    end

    simSetJointPosition(baseJoints[1],-rackOffset)
    simSetJointPosition(baseJoints[2],borderHeight)
    simSetJointPosition(baseJoints[3],width*0.5)
    simSetJointPosition(baseJoints[4],width*0.5)
    simSetJointPosition(baseJoints[5],width*0.5)
    if rotated then
        simSetJointPosition(baseJoints[6],math.pi)
        simSetJointPosition(baseJoints[7],math.pi)
    else
        simSetJointPosition(baseJoints[6],0)
        simSetJointPosition(baseJoints[7],0)
    end

    simSetObjectPosition(cartridges[1]['H'],cartridgeDefPosDummy,conf['cartridge1PosOffset'])
    simSetObjectPosition(cartridges[2]['H'],cartridges[1]['H'],{0,direction*conf['cartridge2PosOffset'],0})


    for cart=1,2,1 do
        simSetJointPosition(cartridges[cart]['forwardJointH'],width*0.5-addShift*0.5)

        setObjectSize(cartridges[cart]['wallAH'],cwt,cwidth,0.22)
        local p=simGetObjectPosition(cartridges[cart]['wallAH'],cartridges[cart]['centerH'])
        simSetObjectPosition(cartridges[cart]['wallAH'],cartridges[cart]['centerH'],{-clength*0.5,0,-0.035})

        setObjectSize(cartridges[cart]['flipH'],cwt,cwidth,0.22)
        p=simGetObjectPosition(cartridges[cart]['flipJointH'],-1)
        simSetObjectPosition(cartridges[cart]['flipH'],-1,{p[1],p[2],p[3]+0.04})

        p=simGetObjectPosition(cartridges[cart]['flipJointH'],cartridges[cart]['centerH'])
        simSetObjectPosition(cartridges[cart]['flipJointH'],cartridges[cart]['centerH'],{clength*0.5,0,p[3]})

        setObjectSize(cartridges[cart]['wallBH'],clength,cwt,0.15)
        setObjectSize(cartridges[cart]['div1H'],clength,cwt,0.15)
        setObjectSize(cartridges[cart]['div2H'],clength,cwt,0.15)
        p=simGetObjectPosition(cartridges[cart]['wallBH'],cartridges[cart]['centerH'])
        simSetObjectPosition(cartridges[cart]['wallBH'],cartridges[cart]['centerH'],{0,-cwidth*0.5,p[3]})

        setObjectSize(cartridges[cart]['wallCH'],clength,cwt,0.15)
        p=simGetObjectPosition(cartridges[cart]['wallCH'],cartridges[cart]['centerH'])
        simSetObjectPosition(cartridges[cart]['wallCH'],cartridges[cart]['centerH'],{0,cwidth*0.5,p[3]})

        setObjectSize(cartridges[cart]['baseB1H'],clength*0.96,cwidth*0.48,cwt)
        simSetObjectPosition(cartridges[cart]['baseB1H'],cartridges[cart]['centerH'],{0,-cwidth*0.25,-0.075})

        setObjectSize(cartridges[cart]['baseC1H'],clength*0.96,cwidth*0.48,cwt)
        simSetObjectPosition(cartridges[cart]['baseC1H'],cartridges[cart]['centerH'],{0,cwidth*0.25,-0.075})

        setObjectSize(cartridges[cart]['baseB2H'],clength*0.96,cwt,0.02)
        simSetObjectPosition(cartridges[cart]['baseB2H'],cartridges[cart]['centerH'],{0,-cwidth*0.5,-0.075+0.01})

        setObjectSize(cartridges[cart]['baseC2H'],clength*0.96,cwt,0.02)
        simSetObjectPosition(cartridges[cart]['baseC2H'],cartridges[cart]['centerH'],{0,cwidth*0.5,-0.075+0.01})


        setObjectSize(cartridges[cart]['sensor'],0.001,0.001,width*1.01 )
        simSetObjectPosition(cartridges[cart]['sensor'],model,{-width*0.5,0,borderHeight*0.5})
        p=simGetObjectPosition(cartridges[cart]['sensor'],cartridges[cart]['H'])
        simSetObjectPosition(cartridges[cart]['sensor'],cartridges[cart]['H'],{p[1],0,p[3]})

        setObjectSize(cartridges[cart]['stopper'],width,wt,borderHeight)
        simSetObjectPosition(cartridges[cart]['stopper'],cartridges[cart]['sensor'],{0,direction*(-cwidth*0.5+stopperOffset),0})
        p=simGetObjectPosition(cartridges[cart]['stopper'],model)
        simSetObjectPosition(cartridges[cart]['stopper'],model,{0,p[2],p[3]})

        p=simGetObjectPosition(cartridges[cart]['wallBH'],cartridges[cart]['centerH'])

        if divs==0 then
            simSetObjectPosition(cartridges[cart]['div1H'],cartridges[cart]['wallBH'],{0,0,0})
            simSetObjectPosition(cartridges[cart]['div2H'],cartridges[cart]['wallBH'],{0,0,0})
        end
        if divs==1 then
            simSetObjectPosition(cartridges[cart]['div1H'],cartridges[cart]['centerH'],{0,0,p[3]})
            simSetObjectPosition(cartridges[cart]['div2H'],cartridges[cart]['centerH'],{0,0,p[3]})
        end
        if divs==2 then
            simSetObjectPosition(cartridges[cart]['div1H'],cartridges[cart]['centerH'],{0,-cwidth/6,p[3]})
            simSetObjectPosition(cartridges[cart]['div2H'],cartridges[cart]['centerH'],{0,cwidth/6,p[3]})
        end

    end

    setObjectSize(cartridge2SpecialSensor,0.001,0.001,width*1.01 )
    simSetObjectPosition(cartridge2SpecialSensor,cartridges[2]['sensor'],{0,direction*(-cwidth*0.5+stopperOffset+wt*1),0})

    local p=simGetObjectPosition(baseJoints[6],cartridges[1]['H'])
    simSetObjectPosition(baseJoints[6],cartridges[1]['H'],{p[1],0,p[3]})

    generatePallet()

--]]    
end

function updateEnabledDisabledItemsDlg()
    if ui then
        local simStopped=simGetSimulationState()==sim_simulation_stopped

        -- Conveyor part:
        local c=readInfo()
        local re=simBoolAnd32(c['bitCoded'],16)==0
        simExtCustomUI_setEnabled(ui,2,simStopped,true)
        simExtCustomUI_setEnabled(ui,4,simStopped,true)
        simExtCustomUI_setEnabled(ui,20,simStopped,true)
        simExtCustomUI_setEnabled(ui,21,simStopped,true)
        simExtCustomUI_setEnabled(ui,26,simStopped,true)
        simExtCustomUI_setEnabled(ui,27,simStopped,true)
        simExtCustomUI_setEnabled(ui,100,simStopped,true)
        simExtCustomUI_setEnabled(ui,101,simStopped,true)
        simExtCustomUI_setEnabled(ui,30,simStopped,true)
        simExtCustomUI_setEnabled(ui,31,simStopped,true)
        simExtCustomUI_setEnabled(ui,32,simStopped,true)
        simExtCustomUI_setEnabled(ui,33,simStopped,true)
        simExtCustomUI_setEnabled(ui,34,simStopped,true)
        simExtCustomUI_setEnabled(ui,35,simStopped,true)

        simExtCustomUI_setEnabled(ui,36,simStopped,true)
        simExtCustomUI_setEnabled(ui,37,simStopped,true)
        simExtCustomUI_setEnabled(ui,38,simStopped,true)
        simExtCustomUI_setEnabled(ui,39,simStopped,true)
        simExtCustomUI_setEnabled(ui,40,simStopped,true)
        simExtCustomUI_setEnabled(ui,41,simStopped,true)
        simExtCustomUI_setEnabled(ui,42,simStopped,true)
        simExtCustomUI_setEnabled(ui,43,simStopped,true)
        simExtCustomUI_setEnabled(ui,44,simStopped,true)
        simExtCustomUI_setEnabled(ui,45,simStopped,true)
        simExtCustomUI_setEnabled(ui,46,simStopped,true)
    end
end

function setDlgItemContent()
    if ui then
        local sel=bwUtils.getSelectedEditWidget(ui)
--        local red,green,blue,spec=getColor()

        -- Conveyor part:
        local conveyorConfig=readInfo()
        simExtCustomUI_setEditValue(ui,2,string.format("%.0f",conveyorConfig['length']/0.001),true)
        simExtCustomUI_setEditValue(ui,4,string.format("%.0f",conveyorConfig['width']/0.001),true)
        simExtCustomUI_setEditValue(ui,20,string.format("%.0f",conveyorConfig['height']/0.001),true)
        simExtCustomUI_setEditValue(ui,21,string.format("%.0f",conveyorConfig['borderHeight']/0.001),true)
        simExtCustomUI_setEditValue(ui,26,string.format("%.0f",conveyorConfig['wallThickness']/0.001),true)
        simExtCustomUI_setEditValue(ui,44,conveyorConfig['locationName'],true)
        simExtCustomUI_setCheckboxValue(ui,45,(simBoolAnd32(conveyorConfig['bitCoded'],256)~=0) and 2 or 0,true)

        simExtCustomUI_setCheckboxValue(ui,27,(simBoolAnd32(conveyorConfig['bitCoded'],16)==0) and 2 or 0,true)

        simExtCustomUI_setCheckboxValue(ui,1000,(simBoolAnd32(conveyorConfig['bitCoded'],64)~=0) and 2 or 0,true)
        simExtCustomUI_setEditValue(ui,10,string.format("%.0f",conveyorConfig['velocity']/0.001),true)
        simExtCustomUI_setEditValue(ui,12,string.format("%.0f",conveyorConfig['acceleration']/0.001),true)
        simExtCustomUI_setEditValue(ui,39,string.format("%.0f",conveyorConfig['cartridgeRackOffset']/0.001),true)
        simExtCustomUI_setEditValue(ui,35,string.format("%.0f",conveyorConfig['stopperOffsets']/0.001),true)

        local off=conveyorConfig['cartridge1PosOffset']
        simExtCustomUI_setEditValue(ui,30,string.format("%.0f , %.0f , %.0f",off[1]*1000,off[2]*1000,off[3]*1000),true)
        simExtCustomUI_setEditValue(ui,31,string.format("%.0f",conveyorConfig['cartridge2PosOffset']/0.001),true)
        simExtCustomUI_setEditValue(ui,32,string.format("%.0f",conveyorConfig['cartridgeWidth']/0.001),true)
        simExtCustomUI_setEditValue(ui,33,string.format("%.0f",conveyorConfig['cartridgeWallThickness']/0.001),true)
        simExtCustomUI_setEditValue(ui,34,tostring(conveyorConfig['cartridgeDivisions']),true)

        simExtCustomUI_setEditValue(ui,36,string.format("%.0f",conveyorConfig['cartridgeVelocity']*180/math.pi),true)
        simExtCustomUI_setEditValue(ui,37,string.format("%.0f",conveyorConfig['cartridgeAcceleration']*180/math.pi),true)
        simExtCustomUI_setEditValue(ui,46,string.format("%.2f",conveyorConfig['cartridgeDwellTimeUp']),true)
        simExtCustomUI_setEditValue(ui,38,string.format("%.2f",conveyorConfig['cartridgeDwellTimeDown']),true)

        local pallet=conveyorConfig['palletData']
        simExtCustomUI_setEditValue(ui,40,string.format("%.0f , %.0f , %.0f",pallet[1][1]*1000,pallet[1][2]*1000,pallet[1][3]*1000),true)
        simExtCustomUI_setEditValue(ui,41,string.format("%.0f",pallet[2]),true)
        simExtCustomUI_setEditValue(ui,42,string.format("%.0f",pallet[3]/0.001),true)
        simExtCustomUI_setCheckboxValue(ui,43,(simBoolAnd32(conveyorConfig['bitCoded'],128)~=0) and 2 or 0,true)


        updateStartStopTriggerComboboxes()
        updateEnabledDisabledItemsDlg()

        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function getAvailableSensors()
    local l=simGetObjectsInTree(sim_handle_scene,sim_handle_all,0)
    local retL={}
    for i=1,#l,1 do
        local data=simReadCustomDataBlock(l[i],'XYZ_BINARYSENSOR_INFO')
        if data then
            retL[#retL+1]={simGetObjectName(l[i]),l[i]}
        end
    end
    return retL
end

function lengthChange(ui,id,newVal)
    local conf=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.01 then l=0.01 end
        if l>10 then l=10 end
        if l~=conf['length'] then
            modified=true
            conf['length']=l
            writeInfo(conf)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function widthChange(ui,id,newVal)
    local conf=readInfo()
    local w=tonumber(newVal)
    if w then
        w=w*0.001
        if w<0.16 then w=0.16 end
        if w>2 then w=2 end
        if w~=conf['width'] then
            modified=true
            conf['width']=w
            writeInfo(conf)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function heightChange(ui,id,newVal)
    local conf=readInfo()
    local w=tonumber(newVal)
    if w then
        w=w*0.001
        if w<0.01 then w=0.01 end
        if w>1 then w=1 end
        if w~=conf['height'] then
            modified=true
            conf['height']=w
            writeInfo(conf)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function borderHeightChange(ui,id,newVal)
    local conf=readInfo()
    local w=tonumber(newVal)
    if w then
        w=w*0.001
        if w<0.15 then w=0.15 end
        if w>0.4 then w=0.4 end
        if w~=conf['borderHeight'] then
            modified=true
            conf['borderHeight']=w
            writeInfo(conf)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function wallThicknessChange(ui,id,newVal)
    local conf=readInfo()
    local w=tonumber(newVal)
    if w then
        w=w*0.001
        if w<0.001 then w=0.001 end
        if w>0.02 then w=0.02 end
        if w~=conf['wallThickness'] then
            modified=true
            conf['wallThickness']=w
            writeInfo(conf)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function stopperOffsetChange(ui,id,newVal)
    local conf=readInfo()
    local w=tonumber(newVal)
    if w then
        w=w*0.001
        if w<-0.1 then w=-0.1 end
        if w>0.1 then w=0.1 end
        if w~=conf['stopperOffsets'] then
            modified=true
            conf['stopperOffsets']=w
            writeInfo(conf)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function rackOffsetChange(ui,id,newVal)
    local conf=readInfo()
    local w=tonumber(newVal)
    if w then
        w=w*0.001
        if w<-5 then w=-5 end
        if w>5 then w=5 end
        if w~=conf['cartridgeRackOffset'] then
            modified=true
            conf['cartridgeRackOffset']=w
            writeInfo(conf)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function roundedEndsClicked(ui,id,newVal)
    local conf=readInfo()
    conf['bitCoded']=simBoolOr32(conf['bitCoded'],16)
    if newVal~=0 then
        conf['bitCoded']=conf['bitCoded']-16
    end
    modified=true
    writeInfo(conf)
    updateConveyor()
    setDlgItemContent()
end

function enabledClicked(ui,id,newVal)
    local conf=readInfo()
    conf['bitCoded']=simBoolOr32(conf['bitCoded'],64)
    if newVal==0 then
        conf['bitCoded']=conf['bitCoded']-64
    end
    modified=true
    writeInfo(conf)
    setDlgItemContent()
end

function speedChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<-0.5 then v=-0.5 end
        if v>0.5 then v=0.5 end
        if v~=c['velocity'] then
            modified=true
            c['velocity']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function accelerationChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.001 then v=0.001 end
        if v>1 then v=1 end
        if v~=c['acceleration'] then
            modified=true
            c['acceleration']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function locationNameChange(ui,id,newVal)
    local c=readInfo()
    if newVal~=c['locationName'] then
        modified=true
        c['locationName']=newVal
        writeInfo(c)
    end
    setDlgItemContent()
end

function triggerStopChange_callback(ui,id,newIndex)
    local sens=comboStopTrigger[newIndex+1][2]
    bwUtils.setReferencedObjectHandle(model,REF_STOP_SIG,sens)
    if bwUtils.getReferencedObjectHandle(model,REF_START_SIG)==sens then
        bwUtils.setReferencedObjectHandle(model,REF_START_SIG,-1)
    end
    modified=true
    updateStartStopTriggerComboboxes()
end

function triggerStartChange_callback(ui,id,newIndex)
    local sens=comboStartTrigger[newIndex+1][2]
    bwUtils.setReferencedObjectHandle(model,REF_START_SIG,sens)
    if bwUtils.getReferencedObjectHandle(model,REF_STOP_SIG)==sens then
        bwUtils.setReferencedObjectHandle(model,REF_STOP_SIG,-1)
    end
    modified=true
    updateStartStopTriggerComboboxes()
end

function updateStartStopTriggerComboboxes()
    local c=readInfo()
    local loc=getAvailableSensors()
    comboStopTrigger=customUi_populateCombobox(ui,100,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_STOP_SIG)),true,{{'<NONE>',-1}})
    comboStartTrigger=customUi_populateCombobox(ui,101,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_START_SIG)),true,{{'<NONE>',-1}})
end

function cartridge1PositionOffsetChange(ui,id,newVal)
    local c=readInfo()
    local i=1
    local t={0,0,0}
    for token in (newVal..","):gmatch("([^,]*),") do
        t[i]=tonumber(token)
        if t[i]==nil then t[i]=0 end
        t[i]=t[i]*0.001
        if t[i]>1 then t[i]=1 end
        if t[i]<-1 then t[i]=-1 end
        i=i+1
    end
    c['cartridge1PosOffset']={t[1],t[2],t[3]}
    modified=true
    writeInfo(c)
    updateConveyor()
    setDlgItemContent()
end

function cartridge2PositionOffsetChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.2 then v=0.2 end
        if v>2 then v=2 end
        if v~=c['cartridge2PosOffset'] then
            modified=true
            c['cartridge2PosOffset']=v
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function cartridgeWidthChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.1 then v=0.1 end
        if v>1 then v=1 end
        if v~=c['cartridgeWidth'] then
            modified=true
            c['cartridgeWidth']=v
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function cartridgeDivisionChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<0 then v=0 end
        if v>2 then v=2 end
        if v~=c['cartridgeDivisions'] then
            modified=true
            c['cartridgeDivisions']=v
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function cartridgeWTChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.001 then v=0.001 end
        if v>0.05 then v=0.05 end
        if v~=c['cartridgeWallThickness'] then
            modified=true
            c['cartridgeWallThickness']=v
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function cartVelocityChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*math.pi/180
        if v<5*math.pi/180 then v=5*math.pi/180 end
        if v>90*math.pi/180 then v=90*math.pi/180 end
        if v~=c['cartridgeVelocity'] then
            modified=true
            c['cartridgeVelocity']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function cartAccelerationChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*math.pi/180
        if v<5*math.pi/180 then v=5*math.pi/180 end
        if v>1000*math.pi/180 then v=1000*math.pi/180 end
        if v~=c['cartridgeAcceleration'] then
            modified=true
            c['cartridgeAcceleration']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function cartDwellTimeDownChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0 then v=0 end
        if v>20 then v=20 end
        if v~=c['cartridgeDwellTimeDown'] then
            modified=true
            c['cartridgeDwellTimeDown']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function cartDwellTimeUpChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0 then v=0 end
        if v>20 then v=20 end
        if v~=c['cartridgeDwellTimeUp'] then
            modified=true
            c['cartridgeDwellTimeUp']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function palletOffsetChange(ui,id,newVal)
    local c=readInfo()
    local i=1
    local t={0,0,0}
    for token in (newVal..","):gmatch("([^,]*),") do
        t[i]=tonumber(token)
        if t[i]==nil then t[i]=0 end
        t[i]=t[i]*0.001
        if t[i]>0.2 then t[i]=0.2 end
        if t[i]<-0.2 then t[i]=-0.2 end
        i=i+1
    end
    c['palletData'][1]={t[1],t[2],t[3]}
    modified=true
    writeInfo(c)
    updateConveyor()
    setDlgItemContent()
end

function palletRowChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>20 then v=20 end
        if v~=c['palletData'][2] then
            modified=true
            c['palletData'][2]=v
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function palletSpacingChange(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.001 then v=0.001 end
        if v>0.5 then v=0.5 end
        if v~=c['palletData'][3] then
            modified=true
            c['palletData'][3]=v
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function showPointsClicked(ui,id,newVal)
    local conf=readInfo()
    conf['bitCoded']=simBoolOr32(conf['bitCoded'],128)
    if newVal==0 then
        conf['bitCoded']=conf['bitCoded']-128
    end
    modified=true
    writeInfo(conf)
    setDlgItemContent()
end

function rotatedClicked(ui,id,newVal)
    local conf=readInfo()
    conf['bitCoded']=simBoolOr32(conf['bitCoded'],256)
    if newVal==0 then
        conf['bitCoded']=conf['bitCoded']-256
    end
    modified=true
    writeInfo(conf)
    updateConveyor()
    setDlgItemContent()
end

function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
    <tabs id="77">
    <tab title="General" layout="form">
                <label text="Enabled"/>
                <checkbox text="" onchange="enabledClicked" id="1000"/>

                <label text="Speed (mm/s)"/>
                <edit oneditingfinished="speedChange" id="10"/>

                <label text="Acceleration (mm/s^2)"/>
                <edit oneditingfinished="accelerationChange" id="12"/>

                <label text="Stop on trigger"/>
                <combobox id="100" onchange="triggerStopChange_callback">
                </combobox>

                <label text="Restart on trigger"/>
                <combobox id="101" onchange="triggerStartChange_callback">
                </combobox>

                <label text="Location name"/>
                <edit oneditingfinished="locationNameChange" id="44"/>

                <label text="Rotated"/>
                <checkbox text="" onchange="rotatedClicked" id="45"/>

    </tab>
    <tab title="Conveyor" layout="form">
                <label text="Length (mm)"/>
                <edit oneditingfinished="lengthChange" id="2"/>

                <label text="Width (mm)"/>
                <edit oneditingfinished="widthChange" id="4"/>

                <label text="Base height (mm)"/>
                <edit oneditingfinished="heightChange" id="20"/>

                <label text="Border height (mm)"/>
                <edit oneditingfinished="borderHeightChange" id="21"/>

                <label text="Wall thickness (mm)"/>
                <edit oneditingfinished="wallThicknessChange" id="26"/>

                <label text="Cartridge rack offset (mm)"/>
                <edit oneditingfinished="rackOffsetChange" id="39"/>

                <label text="Stopper offset (mm)"/>
                <edit oneditingfinished="stopperOffsetChange" id="35"/>

                <checkbox text="Rounded ends" onchange="roundedEndsClicked" id="27"/>
                <label text=""/>
    </tab>
    <tab title="Cartridge" layout="form">
                <label text="Cartridge width (mm)"/>
                <edit oneditingfinished="cartridgeWidthChange" id="32"/>

                <label text="Cartridge divisions"/>
                <edit oneditingfinished="cartridgeDivisionChange" id="34"/>

                <label text="Cartridge wall thickness (mm)"/>
                <edit oneditingfinished="cartridgeWTChange" id="33"/>

                <label text="Cartridge1 position offset (mm)"/>
                <edit oneditingfinished="cartridge1PositionOffsetChange" id="30"/>

                <label text="Cartridge2 position offset (mm)"/>
                <edit oneditingfinished="cartridge2PositionOffsetChange" id="31"/>

                <label text="Cartridge velocity (deg/s)"/>
                <edit oneditingfinished="cartVelocityChange" id="36"/>

                <label text="Cartridge acceleration (deg/s^2)"/>
                <edit oneditingfinished="cartAccelerationChange" id="37"/>

                <label text="Cartridge dwell time up (s)"/>
                <edit oneditingfinished="cartDwellTimeUpChange" id="46"/>

                <label text="Cartridge dwell time down (s)"/>
                <edit oneditingfinished="cartDwellTimeDownChange" id="38"/>

    </tab>
    <tab title="Pallet" layout="form">
                <label text="Offset (X, Y, Z, in mm)"/>
                <edit oneditingfinished="palletOffsetChange" id="40"/>

                <label text="Rows (per cartridge division)"/>
                <edit oneditingfinished="palletRowChange" id="41"/>

                <label text="Row spacing"/>
                <edit oneditingfinished="palletSpacingChange" id="42"/>
    </tab>
    <tab title="More" layout="form">
                <label text="Show points"/>
                <checkbox text="" onchange="showPointsClicked" id="43"/>
 
            <label text="" style="* {margin-left: 150px;}"/>
            <label text="" style="* {margin-left: 150px;}"/>
    </tab>
    </tabs>
        ]]

        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos--[[,closeable,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])

        setDlgItemContent()
        simExtCustomUI_setCurrentTab(ui,77,dlgMainTabIndex,true)
    end
end

function showDlg()
    if not ui then
        createDlg()
    end
end

function removeDlg()
    if ui then
        local x,y=simExtCustomUI_getPosition(ui)
        previousDlgPos={x,y}
        dlgMainTabIndex=simExtCustomUI_getCurrentTab(ui,77)
        simExtCustomUI_destroy(ui)
        ui=nil
    end
end

enableStopper=function(handle,enable)
    if enable then
        simSetObjectInt32Parameter(handle,sim_objintparam_visibility_layer,1) -- make it visible
        simSetObjectSpecialProperty(handle,sim_objectspecialproperty_collidable+sim_objectspecialproperty_measurable+sim_objectspecialproperty_detectable_all+sim_objectspecialproperty_renderable) -- make it collidable, measurable, detectable, etc.
        simSetObjectInt32Parameter(handle,sim_shapeintparam_respondable,1) -- make it respondable
        simResetDynamicObject(handle)
    else
        simSetObjectInt32Parameter(handle,sim_objintparam_visibility_layer,0)
        simSetObjectSpecialProperty(handle,0)
        simSetObjectInt32Parameter(handle,sim_shapeintparam_respondable,0)
        simResetDynamicObject(handle)
    end
end

makeVisible=function(handle,visible)
    if visible then
        simSetObjectInt32Parameter(handle,sim_objintparam_visibility_layer,1) -- make it visible
    else
        simSetObjectInt32Parameter(handle,sim_objintparam_visibility_layer,0)
    end
end

makeTargetDummiesVisible=function(visible)
    for cart=1,2,1 do
        local i=0
        while true do
            local h=simGetObjectChild(cartridges[cart]['targets'],i)
            if h<0 then break end
            if visible then
                simSetObjectInt32Parameter(h,sim_objintparam_visibility_layer,4)
            else
                simSetObjectInt32Parameter(h,sim_objintparam_visibility_layer,0)
            end
            i=i+1
        end
    end
end

prepareAndClearTrackingWIndowInfo=function()
    local info={}
    info['version']=0
    info['trackedItemsInWindow']={}
    info['trackedTargetsInWindow']={}
    info['itemsToRemoveFromTracking']={}
    info['targetPositionsToMarkAsProcessed']={}
    simWriteCustomDataBlock(targetTracking,'XYZ_TRACKINGWINDOW_INFO',simPackTable(info))
end

if (sim_call_type==sim_customizationscriptcall_initialization) then
    REF_STOP_SIG=1 -- index of referenced stop trigger object handle
    REF_START_SIG=2 -- index of referenced start trigger object handle
    modified=false
    dlgMainTabIndex=0
    lastT=simGetSystemTimeInMs(-1)
    version=simGetInt32Parameter(sim_intparam_program_version)
    revision=simGetInt32Parameter(sim_intparam_program_revision)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    targetTracking=simGetObjectHandle('genericPingPongPacker_targetTracking')
    prepareAndClearTrackingWIndowInfo()

    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    -- Following for backward compatibility:
    if _info['stopTrigger'] then
        bwUtils.setReferencedObjectHandle(model,REF_STOP_SIG,getObjectHandle_noErrorNoSuffixAdjustment(_info['stopTrigger']))
        _info['stopTrigger']=nil
    end
    if _info['startTrigger'] then
        bwUtils.setReferencedObjectHandle(model,REF_START_SIG,getObjectHandle_noErrorNoSuffixAdjustment(_info['startTrigger']))
        _info['startTrigger']=nil
    end
    ----------------------------------------
    writeInfo(_info)
    rotJoints={}
    rotJoints[1]=simGetObjectHandle('genericPingPongPacker_jointB')
    rotJoints[2]=simGetObjectHandle('genericPingPongPacker_jointC')

    middleParts={}
    middleParts[1]=simGetObjectHandle('genericPingPongPacker_sides')
    middleParts[2]=simGetObjectHandle('genericPingPongPacker_textureA')
    middleParts[3]=simGetObjectHandle('genericPingPongPacker_forwarderA')
    
    endParts={}
    endParts[1]=simGetObjectHandle('genericPingPongPacker_textureB')
    endParts[2]=simGetObjectHandle('genericPingPongPacker_textureC')
    endParts[3]=simGetObjectHandle('genericPingPongPacker_B')
    endParts[4]=simGetObjectHandle('genericPingPongPacker_C')
    endParts[5]=simGetObjectHandle('genericPingPongPacker_forwarderB')
    endParts[6]=simGetObjectHandle('genericPingPongPacker_forwarderC')

    sides={}
    sides[1]=simGetObjectHandle('genericPingPongPacker_leftSide')
    sides[2]=simGetObjectHandle('genericPingPongPacker_rightSide')

    baseJoints={}
    baseJoints[1]=simGetObjectHandle('genericPingPongPacker_j0')
    baseJoints[2]=simGetObjectHandle('genericPingPongPacker_j1')
    baseJoints[3]=simGetObjectHandle('genericPingPongPacker_j2A')
    baseJoints[4]=simGetObjectHandle('genericPingPongPacker_j2B')
    baseJoints[5]=simGetObjectHandle('genericPingPongPacker_j2B')
    baseJoints[6]=simGetObjectHandle('genericPingPongPacker_jrot')
    baseJoints[7]=simGetObjectHandle('genericPingPongPacker_jholderRot')

    textureHolder=simGetObjectHandle('genericPingPongPacker_textureHolder')

    cartridge1={}
    cartridge2={}
    cartridges={cartridge1,cartridge2}
    cartridgeDefPosDummy=simGetObjectHandle('genericPingPongPacker_cartridge1_defPos')
    cartridge2SpecialSensor=simGetObjectHandle('genericPingPongPacker_cartridge2_sensor2')
    for cartr=1,2,1 do
        cartridges[cartr]['forwardJointH']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_forwardJoint')
        cartridges[cartr]['H']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr)
        cartridges[cartr]['centerH']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_center')
        cartridges[cartr]['wallAH']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_wallA')
        cartridges[cartr]['wallBH']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_wallB')
        cartridges[cartr]['wallCH']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_wallC')
        cartridges[cartr]['flipJointH']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_flipJoint')
        cartridges[cartr]['flipH']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_flip')
        cartridges[cartr]['flipJointBH']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_flipJointB')
        cartridges[cartr]['baseB1H']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_baseB1')
        cartridges[cartr]['baseB2H']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_baseB2')
        cartridges[cartr]['flipJointCH']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_flipJointC')
        cartridges[cartr]['baseC1H']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_baseC1')
        cartridges[cartr]['baseC2H']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_baseC2')
        cartridges[cartr]['div1H']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_divWall1')
        cartridges[cartr]['div2H']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_divWall2')
        cartridges[cartr]['sensor']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_sensor')
        cartridges[cartr]['stopper']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_stopper')
        cartridges[cartr]['targets']=simGetObjectHandle('genericPingPongPacker_cartridge'..cartr..'_targets')
    end

	simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,true)
    updatePluginRepresentation()
    previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos=bwUtils.readSessionPersistentObjectData(model,"dlgPosAndSize")
end

showOrHideUiIfNeeded=function()
    local s=simGetObjectSelection()
    if s and #s>=1 and s[#s]==model then
        showDlg()
    else
        removeDlg()
    end
end

if (sim_call_type==sim_customizationscriptcall_nonsimulation) then
    showOrHideUiIfNeeded()
    if simGetSystemTimeInMs(lastT)>3000 then
        lastT=simGetSystemTimeInMs(-1)
        if modified then
            simAnnounceSceneContentChange() -- to have an undo point
            modified=false
        end
    end
end

if (sim_call_type==sim_customizationscriptcall_simulationsensing) then
    if simJustStarted then
        updateEnabledDisabledItemsDlg()
    end
    simJustStarted=nil
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_simulationpause) then
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_firstaftersimulation) then
    prepareAndClearTrackingWIndowInfo()
    updateEnabledDisabledItemsDlg()
    local conf=readInfo()
    conf['encoderDistance']=0
    conf['stopRequests']={}
    conf['putCartridgeDown']={false,false}
    writeInfo(conf)
    enableStopper(cartridges[1]['stopper'],true)
    enableStopper(cartridges[2]['stopper'],true)
    makeVisible(cartridges[1]['sensor'],true)
    makeVisible(cartridges[2]['sensor'],true)
    makeVisible(cartridge2SpecialSensor,true)
    makeTargetDummiesVisible(true)
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    prepareAndClearTrackingWIndowInfo()
    simJustStarted=true
    enableStopper(cartridges[1]['stopper'],false)
    enableStopper(cartridges[2]['stopper'],false)
    makeVisible(cartridges[1]['sensor'],false)
    makeVisible(cartridges[2]['sensor'],false)
    makeVisible(cartridge2SpecialSensor,false)
    local conf=readInfo()
    conf['putCartridgeDown']={false,false}
    writeInfo(conf)
    generatePallet()
    makeTargetDummiesVisible(false)
end

if (sim_call_type==sim_customizationscriptcall_lastbeforeinstanceswitch) then
    removeDlg()
    removeFromPluginRepresentation()
end

if (sim_call_type==sim_customizationscriptcall_firstafterinstanceswitch) then
    updatePluginRepresentation()
end

if (sim_call_type==sim_customizationscriptcall_cleanup) then
    removeDlg()
    removeFromPluginRepresentation()
    bwUtils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos)
end