local bwUtils=require('bwUtils')

function removeFromPluginRepresentation()

end

function updatePluginRepresentation()

end

function getDefaultInfoForNonExistingFields(info)
    if not info['version'] then
        info['version']=_MODELVERSION_
    end
    if not info['subtype'] then
        info['subtype']='D'
    end
    if not info['length'] then
        info['length']=1
    end
    if not info['width'] then
        info['width']=0.4
    end
    if not info['height'] then
        info['height']=0.1
    end
    if not info['velocity'] then
        info['velocity']=0.1
    end
    if not info['acceleration'] then
        info['acceleration']=0.1
    end
    if not info['bitCoded'] then
        info['bitCoded']=0 -- 64: enabled
    end
    if not info['encoderDistance'] then
        info['encoderDistance']=0
    end
    if not info['padSpacing'] then
        info['padSpacing']=0.2
    end
    if not info['padLength'] then
        info['padLength']=0.2
    end
    if not info['padWidth'] then
        info['padWidth']=0.1
    end
    if not info['padThickness'] then
        info['padThickness']=0.005
    end
    if not info['padWallHeight'] then
        info['padWallHeight']=0.01
    end
    if not info['padWallThickness'] then
        info['padWallThickness']=0.005
    end
    if not info['padVerticalOffset'] then
        info['padVerticalOffset']=0
    end
    if not info['stopRequests'] then
        info['stopRequests']={}
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

function setColor1(red,green,blue,spec)
    simSetShapeColor(padBaseShape,nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
    simSetShapeColor(padBaseShape,nil,sim_colorcomponent_specular,{spec,spec,spec})
    i=0
    while true do
        local h=simGetObjectChild(path,i)
        if h>=0 then
            local ch=simGetObjectChild(h,0)
            if ch>=0 then
                simSetShapeColor(ch,'BASE',sim_colorcomponent_ambient_diffuse,{red,green,blue})
                simSetShapeColor(ch,'BASE',sim_colorcomponent_specular,{spec,spec,spec})
            end
            i=i+1
        else
            break
        end
    end
end

function getColor1()
    local r,rgb=simGetShapeColor(padBaseShape,nil,sim_colorcomponent_ambient_diffuse)
    local r,spec=simGetShapeColor(padBaseShape,nil,sim_colorcomponent_specular)
    return rgb[1],rgb[2],rgb[3],(spec[1]+spec[2]+spec[3])/3
end

function setColor2(red,green,blue,spec)
    simSetShapeColor(padWallShape,nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
    simSetShapeColor(padWallShape,nil,sim_colorcomponent_specular,{spec,spec,spec})
    i=0
    while true do
        local h=simGetObjectChild(path,i)
        if h>=0 then
            local ch=simGetObjectChild(h,0)
            if ch>=0 then
                simSetShapeColor(ch,'WALL',sim_colorcomponent_ambient_diffuse,{red,green,blue})
                simSetShapeColor(ch,'WALL',sim_colorcomponent_specular,{spec,spec,spec})
            end
            i=i+1
        else
            break
        end
    end
end

function getColor2()
    local r,rgb=simGetShapeColor(padWallShape,nil,sim_colorcomponent_ambient_diffuse)
    local r,spec=simGetShapeColor(padWallShape,nil,sim_colorcomponent_specular)
    return rgb[1],rgb[2],rgb[3],(spec[1]+spec[2]+spec[3])/3
end

function setShapeSize(h,x,y,z)
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

function getAvailableMasterConveyors()
    local l=simGetObjectsInTree(sim_handle_scene,sim_handle_all,0)
    local retL={}
    for i=1,#l,1 do
        if l[i]~=model then
            local data=simReadCustomDataBlock(l[i],'CONVEYOR_CONF')
            if data then
                retL[#retL+1]={simGetObjectName(l[i]),l[i]}
            end
        end
    end
    return retL
end

function getActualPadSpacing()
    local conf=readInfo()
    local l=simGetPathLength(path)
    local cnt=math.floor(l/conf['padSpacing'])+1
    local dx=l/cnt
    return dx
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

function masterChange_callback(ui,id,newIndex)
    local sens=comboMaster[newIndex+1][2]
    bwUtils.setReferencedObjectHandle(model,REF_MASTER_CONV,sens) -- master
    modified=true
    updateMasterCombobox()
    updateEnabledDisabledItems()
end

function updateStartStopTriggerComboboxes()
    local c=readInfo()
    local loc=getAvailableSensors()
    comboStopTrigger=customUi_populateCombobox(ui,200,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_STOP_SIG)),true,{{'<NONE>',-1}})
    comboStartTrigger=customUi_populateCombobox(ui,201,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_START_SIG)),true,{{'<NONE>',-1}})
end

function updateMasterCombobox()
    local c=readInfo()
    local loc=getAvailableMasterConveyors()
    comboMaster=customUi_populateCombobox(ui,202,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_MASTER_CONV)),true,{{'<NONE>',-1}})
end

function updateEnabledDisabledItems()
    if ui then
        local c=readInfo()
        local enabled=simGetSimulationState()==sim_simulation_stopped
        simExtCustomUI_setEnabled(ui,2,enabled,true)
        simExtCustomUI_setEnabled(ui,4,enabled,true)
        simExtCustomUI_setEnabled(ui,28,enabled,true)
        simExtCustomUI_setEnabled(ui,21,enabled,true)
        simExtCustomUI_setEnabled(ui,5,enabled,true)
        simExtCustomUI_setEnabled(ui,6,enabled,true)
        simExtCustomUI_setEnabled(ui,7,enabled,true)
        simExtCustomUI_setEnabled(ui,8,enabled,true)

        simExtCustomUI_setEnabled(ui,30,enabled,true)
        simExtCustomUI_setEnabled(ui,31,enabled,true)
        simExtCustomUI_setEnabled(ui,32,enabled,true)
        simExtCustomUI_setEnabled(ui,33,enabled,true)

        simExtCustomUI_setEnabled(ui,100,enabled,true)
        simExtCustomUI_setEnabled(ui,101,enabled,true)
        simExtCustomUI_setEnabled(ui,102,enabled,true)
        simExtCustomUI_setEnabled(ui,103,enabled,true)
        simExtCustomUI_setEnabled(ui,104,enabled,true)
        simExtCustomUI_setEnabled(ui,105,enabled,true)

        simExtCustomUI_setEnabled(ui,1000,bwUtils.getReferencedObjectHandle(model,REF_MASTER_CONV)==-1,true) -- enable
        simExtCustomUI_setEnabled(ui,10,bwUtils.getReferencedObjectHandle(model,REF_MASTER_CONV)==-1,true) -- vel
        simExtCustomUI_setEnabled(ui,12,bwUtils.getReferencedObjectHandle(model,REF_MASTER_CONV)==-1,true) -- accel
        
        simExtCustomUI_setEnabled(ui,200,enabled,true) -- stop trigger
        simExtCustomUI_setEnabled(ui,201,enabled,true) -- restart trigger
        simExtCustomUI_setEnabled(ui,202,enabled,true) -- master
    end
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setEditValue(ui,10,string.format("%.0f",config['velocity']/0.001),true)
        simExtCustomUI_setEditValue(ui,12,string.format("%.0f",config['acceleration']/0.001),true)
        simExtCustomUI_setEditValue(ui,2,string.format("%.0f",config['length']/0.001),true)
        simExtCustomUI_setEditValue(ui,4,string.format("%.0f",config['width']/0.001),true)
        simExtCustomUI_setEditValue(ui,21,string.format("%.0f",config['padSpacing']/0.001),true)
        simExtCustomUI_setEditValue(ui,28,string.format("%.0f",config['height']/0.001),true)
        simExtCustomUI_setCheckboxValue(ui,1000,(simBoolAnd32(config['bitCoded'],64)~=0) and 2 or 0,true)
        simExtCustomUI_setLabelText(ui,26,string.format("%.0f",getActualPadSpacing()/0.001),true)


        simExtCustomUI_setEditValue(ui,100,string.format("%.0f",config['padLength']/0.001),true)
        simExtCustomUI_setEditValue(ui,101,string.format("%.0f",config['padWidth']/0.001),true)
        simExtCustomUI_setEditValue(ui,102,string.format("%.0f",config['padThickness']/0.001),true)
        simExtCustomUI_setEditValue(ui,103,string.format("%.0f",config['padWallHeight']/0.001),true)
        simExtCustomUI_setEditValue(ui,104,string.format("%.0f",config['padWallThickness']/0.001),true)
        simExtCustomUI_setEditValue(ui,105,string.format("%.0f",config['padVerticalOffset']/0.001),true)

        updateStartStopTriggerComboboxes()
        updateMasterCombobox()

        updateEnabledDisabledItems()
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function updateConveyor()
    local conf=readInfo()
    local length=conf['length']
    local width=conf['width']
    local padSpacing=conf['padSpacing']
    local bitCoded=conf['bitCoded']
    local height=conf['height']

    local plength=conf['padLength']
    local pwidth=conf['padWidth']
    local pthickness=conf['padThickness']
    local pwheight=conf['padWallHeight']
    local pwthickness=conf['padWallThickness']
    local pvoffset=conf['padVerticalOffset']

    setShapeSize(base,width,length,height)
    setShapeSize(baseBack,width,width,height)
    setShapeSize(baseFront,width,width,height)
    simSetObjectPosition(path,model,{0,0,0})
    simSetObjectPosition(base,model,{0,0,-height*0.5})
    simSetObjectPosition(baseBack,model,{0,-length*0.5,-height*0.5})
    simSetObjectPosition(baseFront,model,{0,length*0.5,-height*0.5})
    simSetObjectPosition(path,model,{0,0,pvoffset})

    while true do
        local h=simGetObjectChild(path,0)
        if h>=0 then
            simRemoveObject(h)
        else
            break
        end
    end

    simCutPathCtrlPoints(path,-1,0)
    local pts={}
    local prec=30
    for i=0,prec,1 do
        pts[i*11+1]=0
        pts[i*11+2]=-length*0.5-width*0.5*math.sin(i*math.pi/prec)
        pts[i*11+3]=-width*0.5*math.cos(i*math.pi/prec)
        pts[i*11+4]=0 --math.pi-i*math.pi/8
        pts[i*11+5]=0
        pts[i*11+6]=0
        pts[i*11+7]=1
        pts[i*11+8]=0
        pts[i*11+9]=3
        pts[i*11+10]=0.5
        pts[i*11+11]=0.5
    end
    for i=0,prec,1 do
        pts[(i+prec+1)*11+1]=0
        pts[(i+prec+1)*11+2]=length*0.5+width*0.5*math.sin(i*math.pi/prec)
        pts[(i+prec+1)*11+3]=width*0.5*math.cos(i*math.pi/prec)
        pts[(i+prec+1)*11+4]=0 --i*math.pi/8
        pts[(i+prec+1)*11+5]=0
        pts[(i+prec+1)*11+6]=0
        pts[(i+prec+1)*11+7]=1
        pts[(i+prec+1)*11+8]=0
        pts[(i+prec+1)*11+9]=3
        pts[(i+prec+1)*11+10]=0.5
        pts[(i+prec+1)*11+11]=0.5
    end
    simInsertPathCtrlPoints(path,1,0,prec*2+2,pts)
    local l=simGetPathLength(path)
    local cnt=math.floor(l/padSpacing)+1
    local dx=l/cnt

    -- Build first one pad:
    local pad=simCopyPasteObjects({padBaseShape},0)[1]
    setShapeSize(pad,plength,pwidth,pthickness)
    if pwheight>0 then
        local walls={}
        walls[1]=simCopyPasteObjects({padWallShape},0)[1]
        setShapeSize(walls[1],plength,pwthickness,pwheight)
        walls[2]=simCopyPasteObjects({walls[1]},0)[1]

        walls[3]=simCopyPasteObjects({padWallShape},0)[1]
        setShapeSize(walls[3],pwthickness,pwidth-pwthickness*2,pwheight)
        walls[4]=simCopyPasteObjects({walls[3]},0)[1]
        simSetObjectPosition(walls[1],pad,{0,(pwidth-pwthickness)*0.5,(pthickness+pwheight)*0.5})
        simSetObjectPosition(walls[2],pad,{0,-(pwidth-pwthickness)*0.5,(pthickness+pwheight)*0.5})
        simSetObjectPosition(walls[3],pad,{(plength-pwthickness)*0.5,0,(pthickness+pwheight)*0.5})
        simSetObjectPosition(walls[4],pad,{-(plength-pwthickness)*0.5,0,(pthickness+pwheight)*0.5})
        walls[5]=pad
        pad=simGroupShapes(walls)
        simReorientShapeBoundingBox(pad,-1)
    end
    simSetObjectInt32Parameter(pad,sim_objintparam_visibility_layer,1+256)
    simSetObjectProperty(pad,sim_objectproperty_selectable+sim_objectproperty_selectmodelbaseinstead)
    simSetObjectInt32Parameter(pad,sim_shapeintparam_respondable,1)
    simSetObjectSpecialProperty(pad,sim_objectspecialproperty_collidable+sim_objectspecialproperty_measurable+sim_objectspecialproperty_detectable_all+sim_objectspecialproperty_renderable)
    simSetObjectPosition(pad,padBase,{(pthickness+pwheight)*0.5,plength*0.5,0})
    simSetObjectOrientation(pad,padBase,{0,-math.pi/2,math.pi/2})
    -- Duplicate it and put the copies in place:
    for i=0,cnt-1,1 do
        local pb=simCopyPasteObjects({padBase},0)[1]
        local p=simCopyPasteObjects({pad},0)[1]
        simSetObjectParent(p,pb,true)
        simSetObjectParent(pb,path,true)
        simSetObjectInt32Parameter(p,sim_objintparam_visibility_layer,1+256)
        simSetObjectInt32Parameter(p,sim_shapeintparam_respondable,1)
        simSetObjectSpecialProperty(p,sim_objectspecialproperty_collidable+sim_objectspecialproperty_measurable+sim_objectspecialproperty_detectable_all+sim_objectspecialproperty_renderable)
        simSetObjectFloatParameter(pb,sim_dummyfloatparam_follow_path_offset,i*dx)
    end
    -- Delete the first pad:
    simRemoveObject(pad)
end

function lengthChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.4 then l=0.4 end
        if l>5 then l=5 end
        if l~=c['length'] then
            modified=true
            c['length']=l
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function widthChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.05 then l=0.05 end
        if l>2 then l=2 end
        if l~=c['width'] then
            modified=true
            c['width']=l
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function heightChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.01 then l=0.01 end
        if l>1 then l=1 end
        if l~=c['height'] then
            modified=true
            c['height']=l
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function padSpacingChange(ui,id,newVal)
    local c=readInfo()
    local w=tonumber(newVal)
    if w then
        w=w*0.001
        if w<0.02 then w=0.02 end
        if w>5 then w=5 end
        if w~=c['padSpacing'] then
            modified=true
            c['padSpacing']=w
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function padLengthChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.02 then l=0.02 end
        if l<c['padWallThickness']*2 then l=c['padWallThickness']*2 end
        if l>2 then l=2 end
        if l~=c['padLength'] then
            modified=true
            c['padLength']=l
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function padWidthChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.02 then l=0.02 end
        if l<c['padWallThickness']*2 then l=c['padWallThickness']*2 end
        if l>2 then l=2 end
        if l~=c['padWidth'] then
            modified=true
            c['padWidth']=l
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function padThicknessChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.001 then l=0.001 end
        if l>0.2 then l=0.2 end
        if l~=c['padThickness'] then
            modified=true
            c['padThickness']=l
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function padWallThicknessChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        local m=math.min(c['padWidth'],c['padLength'])*0.5
        l=l*0.001
        if l<0.001 then l=0.001 end
        if l>m then l=m end
        if l~=c['padWallThickness'] then
            modified=true
            c['padWallThickness']=l
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function padWallHeightChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.001 then l=0 end
        if l>0.2 then l=0.2 end
        if l~=c['padWallHeight'] then
            modified=true
            c['padWallHeight']=l
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function padVerticalOffsetChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<-0.3 then l=-0.3 end
        if l>0.3 then l=0.3 end
        if l~=c['padVerticalOffset'] then
            modified=true
            c['padVerticalOffset']=l
            writeInfo(c)
            updateConveyor()
        end
    end
    setDlgItemContent()
end

function redChange1(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor1()
    setColor1(newVal/100,g,b,s)
end

function greenChange1(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor1()
    setColor1(r,newVal/100,b,s)
end

function blueChange1(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor1()
    setColor1(r,g,newVal/100,s)
end

function specularChange1(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor1()
    setColor1(r,g,b,newVal/100)
end

function redChange2(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor2()
    setColor2(newVal/100,g,b,s)
end

function greenChange2(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor2()
    setColor2(r,newVal/100,b,s)
end

function blueChange2(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor2()
    setColor2(r,g,newVal/100,s)
end

function specularChange2(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor2()
    setColor2(r,g,b,newVal/100)
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

function enabledClicked(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],64)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-64
    end
    modified=true
    writeInfo(c)
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

                <label text="Master conveyor"/>
                <combobox id="202" onchange="masterChange_callback">
                </combobox>

                <label text="Stop on trigger"/>
                <combobox id="200" onchange="triggerStopChange_callback">
                </combobox>

                <label text="Restart on trigger"/>
                <combobox id="201" onchange="triggerStartChange_callback">
                </combobox>

            <label text="" style="* {margin-left: 150px;}"/>
            <label text="" style="* {margin-left: 150px;}"/>
    </tab>
    <tab title="Dimensions"  layout="form">
                <label text="Length (mm)"/>
                <edit oneditingfinished="lengthChange" id="2"/>

                <label text="Width (mm)"/>
                <edit oneditingfinished="widthChange" id="4"/>

                <label text="Height (mm)"/>
                <edit oneditingfinished="heightChange" id="28"/>
    </tab>
    <tab title="Pads"  layout="form">
                <label text="Pad length (mm)"/>
                <edit oneditingfinished="padLengthChange" id="100"/>

                <label text="Pad width (mm)"/>
                <edit oneditingfinished="padWidthChange" id="101"/>

                <label text="Pad thickness (mm)"/>
                <edit oneditingfinished="padThicknessChange" id="102"/>

                <label text="Pad wall height (mm)"/>
                <edit oneditingfinished="padWallHeightChange" id="103"/>

                <label text="Pad wall thickness (mm)"/>
                <edit oneditingfinished="padWallThicknessChange" id="104"/>

                <label text="Pad vertical offset (mm)"/>
                <edit oneditingfinished="padVerticalOffsetChange" id="105"/>

                <label text="Maximum pad spacing (mm)"/>
                <edit oneditingfinished="padSpacingChange" id="21"/>

                <label text="Actual pad spacing (mm)"/>
                <label text="xxx" id="26"/>
    </tab>
    <tab title="Pad colors">
        <tabs>
        <tab title="Pad base" layout="form">
                <label text="Red"/>
                <hslider minimum="0" maximum="100" onchange="redChange1" id="5"/>
                <label text="Green"/>
                <hslider minimum="0" maximum="100" onchange="greenChange1" id="6"/>
                <label text="Blue"/>
                <hslider minimum="0" maximum="100" onchange="blueChange1" id="7"/>
                <label text="Specular"/>
                <hslider minimum="0" maximum="100" onchange="specularChange1" id="8"/>
        </tab>
        <tab title="Pad walls" layout="form">
                <label text="Red"/>
                <hslider minimum="0" maximum="100" onchange="redChange2" id="30"/>
                <label text="Green"/>
                <hslider minimum="0" maximum="100" onchange="greenChange2" id="31"/>
                <label text="Blue"/>
                <hslider minimum="0" maximum="100" onchange="blueChange2" id="32"/>
                <label text="Specular"/>
                <hslider minimum="0" maximum="100" onchange="specularChange2" id="33"/>
        </tab>
        </tabs>
    </tab>
    </tabs>
        ]]
        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos--[[,closeable,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])
 
        local red,green,blue,spec=getColor1()
        simExtCustomUI_setSliderValue(ui,5,red*100,true)
        simExtCustomUI_setSliderValue(ui,6,green*100,true)
        simExtCustomUI_setSliderValue(ui,7,blue*100,true)
        simExtCustomUI_setSliderValue(ui,8,spec*100,true)
        red,green,blue,spec=getColor2()
        simExtCustomUI_setSliderValue(ui,30,red*100,true)
        simExtCustomUI_setSliderValue(ui,31,green*100,true)
        simExtCustomUI_setSliderValue(ui,32,blue*100,true)
        simExtCustomUI_setSliderValue(ui,33,spec*100,true)
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

if (sim_call_type==sim_customizationscriptcall_initialization) then
    REF_STOP_SIG=1 -- index of referenced stop trigger object handle
    REF_START_SIG=2 -- index of referenced start trigger object handle
    REF_MASTER_CONV=3 -- index of referenced master conveyor object handle
    modified=false
    dlgMainTabIndex=0
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
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
    if _info['masterConveyor'] then
        bwUtils.setReferencedObjectHandle(model,REF_MASTER_CONV,getObjectHandle_noErrorNoSuffixAdjustment(_info['masterConveyor']))
        _info['masterConveyor']=nil
    end
    ----------------------------------------
    writeInfo(_info)
    base=simGetObjectHandle('genericConveyorTypeD_base')
    baseBack=simGetObjectHandle('genericConveyorTypeD_baseBack')
    baseFront=simGetObjectHandle('genericConveyorTypeD_baseFront')
    padBase=simGetObjectHandle('genericConveyorTypeD_padBase')
    padBaseShape=simGetObjectHandle('genericConveyorTypeD_padBaseShape')
    padWallShape=simGetObjectHandle('genericConveyorTypeD_padWallShape')
    path=simGetObjectHandle('genericConveyorTypeD_path')

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
        updateEnabledDisabledItems()
    end
    simJustStarted=nil
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_simulationpause) then
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_firstaftersimulation) then
    updateEnabledDisabledItems()
    local conf=readInfo()
    conf['encoderDistance']=0
    conf['stopRequests']={}
    writeInfo(conf)
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    simJustStarted=true
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