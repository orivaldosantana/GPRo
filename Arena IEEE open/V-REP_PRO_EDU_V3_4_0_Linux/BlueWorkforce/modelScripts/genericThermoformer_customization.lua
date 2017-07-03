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
        info['subtype']='thermoformer'
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
    if not info['stopRequests'] then
        info['stopRequests']={}
    end
    if not info['extrusionWidth'] then
        info['extrusionWidth']=0.1
    end
    if not info['extrusionLength'] then
        info['extrusionLength']=0.2
    end
    if not info['extrusionDepth'] then
        info['extrusionDepth']=0.05
    end
    if not info['plasticThickness'] then
        info['plasticThickness']=0.002
    end
    if not info['rows'] then
        info['rows']=3
    end
    if not info['rowStep'] then
        info['rowStep']=0.15
    end
    if not info['columns'] then
        info['columns']=2
    end
    if not info['columnStep'] then
        info['columnStep']=0.25
    end
    if not info['color'] then
        info['color']={1,1,0.9}
    end
    if not info['pullLength'] then
        info['pullLength']=0.5
    end
    if not info['cuttingStationIndex'] then
        info['cuttingStationIndex']=3
    end
    if not info['partName'] then
        info['partName']='<partName>'
    end
    if not info['partDestination'] then
        info['partDestination']='<defaultDestination>'
    end
    if not info['placeOffset'] then
        info['placeOffset']={0,0,0}
    end
    if not info['indexesPerMinute'] then
        info['indexesPerMinute']=5
    end
    if not info['deactivationTime'] then
        info['deactivationTime']=60
    end

    if info['palletSpacing'] then
        info['pullLength']=info['palletSpacing']+info['columns']*info['columnStep']
        info['palletSpacing']=nil
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

function readPartInfo(handle)
    local data=bwUtils.readPartInfo(handle)

    -- Additional fields here:
    if not data['thermoformingPart'] then
        data['thermoformingPart']=true
    end

    return data
end

function writePartInfo(handle,data)
    return bwUtils.writePartInfo(handle,data)
end

function computeDwellTime(velOrNil,accelOrNil,spacingOrNil,indPerMinOrNil)
    local c=readInfo()
    local v=c['velocity']
    local a=c['acceleration']
    local s=c['pullLength']
    local im=c['indexesPerMinute']
    if velOrNil then
        v=velOrNil
    end
    if accelOrNil then
        a=accelOrNil
    end
    if spacingOrNil then
        s=spacingOrNil
    end
    if indPerMinOrNil then
        im=indPerMinOrNil
    end
    local tt=2*v/a+(s-v*v/a)/v
    local dt=60/im-tt
print("dwellT, travelT",dt,tt)
    return dt
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
    comboStopTrigger=customUi_populateCombobox(ui,13,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_STOP_SIG)),true,{{'<NONE>',-1}})
    comboStartTrigger=customUi_populateCombobox(ui,14,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_START_SIG)),true,{{'<NONE>',-1}})
end


showSamplesAndPallets=function(show)
    local lay=0
    if show then
        lay=1
    end
    local samples=simGetObjectsInTree(sampleHolder,sim_handle_all,1)
    for i=1,#samples,1 do
        simSetObjectInt32Parameter(samples[i],sim_objintparam_visibility_layer,lay)
    end
    local pallets=simGetObjectsInTree(palletHolder,sim_handle_all,1)
    for i=1,#pallets,1 do
        simSetObjectInt32Parameter(pallets[i],sim_objintparam_visibility_layer,lay)
    end
end

function createPalletPoint(objectHandle)
    local data=readPartInfo(objectHandle)
    if #data['palletPoints']==0 then
        data['palletPoints']=bwUtils.generatePalletPoints(data)
    end
    writePartInfo(objectHandle,data)
end

function updateThermoformer()
    local conf=readInfo()
    local extrusionWidth=conf['extrusionWidth']
    local extrusionLength=conf['extrusionLength']
    local extrusionDepth=conf['extrusionDepth']
    local wt=conf['plasticThickness']
    local rows=conf['rows']
    local rowStep=conf['rowStep']
    local columns=conf['columns']
    local columnStep=conf['columnStep']
    local palletSpacing=conf['pullLength']-columns*columnStep
    local cuttingStationIndex=conf['cuttingStationIndex']

    local samples=simGetObjectsInTree(sampleHolder,sim_handle_all,1)
    for i=1,#samples,1 do
        simRemoveObject(samples[i])
    end
    --local bitCoded=conf['bitCoded']
    local objRelPos={-(rows-1)*0.5*rowStep,(columns-1)*0.5*columnStep,-0.5*(extrusionDepth+wt)}
    for col=1,columns,1 do
        objRelPos[1]=-(rows-1)*0.5*rowStep
        for row=1,rows,1 do
            local boxModel=bwUtils.createOpenBox({extrusionWidth+wt*2,extrusionLength+wt*2,extrusionDepth+wt},wt,wt,200,1,true,false,conf['color'])
            simSetObjectSpecialProperty(boxModel,0)
            local p=simGetObjectProperty(boxModel)
            p=simBoolOr32(p,sim_objectproperty_selectmodelbaseinstead)
            p=simBoolOr32(p,sim_objectproperty_dontshowasinsidemodel)
            simSetObjectProperty(boxModel,p)
            simSetObjectPosition(boxModel,sampleHolder,objRelPos)
            simSetObjectParent(boxModel,sampleHolder,true)
            objRelPos[1]=objRelPos[1]+rowStep
            local info=readPartInfo(boxModel)
            info['name']=conf['partName']
            info['destination']=conf['partDestination']
            info['singlePatternData']=conf['placeOffset']
            info['palletPattern']=1
            writePartInfo(boxModel,info)
            createPalletPoint(boxModel)
        end
        objRelPos[2]=objRelPos[2]-columnStep
    end
    local len=columns*columnStep*cuttingStationIndex+palletSpacing*(cuttingStationIndex-1)

    simSetObjectPosition(sampleHolder,model,{0,-len*0.5+columns*columnStep*0.5,0})

    setObjectSize(baseShape,rows*rowStep,len,extrusionDepth+wt)
    simSetObjectPosition(baseShape,model,{0,0,-0.5*(extrusionDepth+wt)})

    local pallets=simGetObjectsInTree(palletHolder,sim_handle_all,1)
    for i=2,#pallets,1 do
        simRemoveObject(pallets[i])
    end
    pallets={pallets[1]}
    local pos={0,0,-0.5*(extrusionDepth+wt)}
    for i=1,cuttingStationIndex,1 do
        local h=pallets[1]
        if i~=1 then
            h=simCopyPasteObjects({h},0)[1]
            simSetObjectParent(h,palletHolder,true)
            pallets[i]=h
        else
            setObjectSize(h,rows*rowStep*0.99,columns*columnStep*0.99,extrusionDepth+wt)
        end
        
        simSetObjectPosition(h,sampleHolder,pos)
        pos[2]=pos[2]+columns*columnStep+palletSpacing
    end
    local ww=math.min(extrusionWidth,extrusionLength)
    setObjectSize(sensor,ww,ww,extrusionDepth*2)
end


function velocityChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.001 then l=0.001 end
        if l>0.3 then l=0.3 end
        if l~=c['velocity'] then
            local dwellTime=computeDwellTime(l,nil,nil,nil)
            if dwellTime>=0 then
                modified=true
                c['velocity']=l
                c['dwellTime']=dwellTime
                writeInfo(c)
            else
                simMsgBox(sim_msgbox_type_info,sim_msgbox_buttons_ok,"Negative Dwell Time","The value you entered would result in a negative dwell time.")
            end
        end
    end
    setDlgItemContent()
end

function accelerationChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.01 then l=0.01 end
        if l>10 then l=10 end
        if l~=c['acceleration'] then
            local dwellTime=computeDwellTime(nil,l,nil,nil)
            if dwellTime>=0 then
                modified=true
                c['acceleration']=l
                c['dwellTime']=dwellTime
                writeInfo(c)
            else
                simMsgBox(sim_msgbox_type_info,sim_msgbox_buttons_ok,"Negative Dwell Time","The value you entered would result in a negative dwell time.")
            end
        end
    end
    setDlgItemContent()
end

function indexPerMinuteChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        if l<0.01 then l=0.01 end
        if l>200 then l=200 end
        if l~=c['indexesPerMinute'] then
            local dwellTime=computeDwellTime(nil,nil,nil,l)
            if dwellTime>=0 then
                modified=true
                c['indexesPerMinute']=l
                c['dwellTime']=dwellTime
                writeInfo(c)
            else
                simMsgBox(sim_msgbox_type_info,sim_msgbox_buttons_ok,"Negative Dwell Time","The value you entered would result in a negative dwell time.")
            end
        end
    end
    setDlgItemContent()
end

function pullLengthChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        local columns=c['columns']
        local columnStep=c['columnStep']
        if l<columns*columnStep then l=columns*columnStep end
        if l>5 then l=5 end
        if l~=c['pullLength'] then
            local dwellTime=computeDwellTime(nil,nil,l,nil)
            if dwellTime>=0 then
                modified=true
                c['pullLength']=l
                c['dwellTime']=dwellTime
                writeInfo(c)
                updateThermoformer()
            else
                simMsgBox(sim_msgbox_type_info,sim_msgbox_buttons_ok,"Negative Dwell Time","The value you entered would result in a negative dwell time.")
            end
        end
    end
    setDlgItemContent()
end

function deactivationTime_callback(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        if l<1 then l=1 end
        if l>100000 then l=100000 end
        if l~=c['deactivationTime'] then
            modified=true
            c['deactivationTime']=l
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function cuttingStationIndexChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=math.floor(l)
        if l<3 then l=3 end
        if l>10 then l=10 end
        if l~=c['cuttingStationIndex'] then
            modified=true
            c['cuttingStationIndex']=l
            writeInfo(c)
            updateThermoformer()
        end
    end
    setDlgItemContent()
end

function extrWidthChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.02 then l=0.02 end
        if l>0.5 then l=0.5 end
        local wt=c['plasticThickness']
        local rowStep=c['rowStep']
        if rowStep<l+3*wt then rowStep=l+3*wt end
        if l~=c['extrusionWidth'] or rowStep~=c['rowStep'] then
            modified=true
            c['extrusionWidth']=l
            c['rowStep']=rowStep
            writeInfo(c)
            updateThermoformer()
        end
    end
    setDlgItemContent()
end

function extrLengthChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.02 then l=0.02 end
        if l>0.5 then l=0.5 end
        local wt=c['plasticThickness']
        local columnStep=c['columnStep']
        if columnStep<l+3*wt then columnStep=l+3*wt end
        if l~=c['extrusionLength'] or columnStep~=c['columnStep'] then
            modified=true
            c['extrusionLength']=l
            c['columnStep']=columnStep
            writeInfo(c)
            updateThermoformer()
        end
    end
    setDlgItemContent()
end

function extrDepthChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.01 then l=0.01 end
        if l>0.3 then l=0.3 end
        if l~=c['extrusionDepth'] then
            modified=true
            c['extrusionDepth']=l
            writeInfo(c)
            updateThermoformer()
        end
    end
    setDlgItemContent()
end

function plasticThicknessChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.001 then l=0.001 end
        if l>0.01 then l=0.01 end
        if l~=c['plasticThickness'] then
            modified=true
            c['plasticThickness']=l
            writeInfo(c)
            updateThermoformer()
        end
    end
    setDlgItemContent()
end

function rowChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=math.floor(l)
        if l<1 then l=1 end
        if l>10 then l=10 end
        if l~=c['rows'] then
            modified=true
            c['rows']=l
            writeInfo(c)
            updateThermoformer()
        end
    end
    setDlgItemContent()
end

function rowStepChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        if l<0.02 then l=0.02 end
        if l>0.5 then l=0.5 end
        local wt=c['plasticThickness']
        local extrW=c['extrusionWidth']
        if l<extrW+3*wt then l=extrW+3*wt end
        if l~=c['rowStep'] then
            modified=true
            c['rowStep']=l
            writeInfo(c)
            updateThermoformer()
        end
    end
    setDlgItemContent()
end

function columnChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=math.floor(l)
        local columnStep=c['columnStep']
        local pullLength=c['pullLength']
        if l<1 then l=1 end
        if l>pullLength/columnStep then l=pullLength/columnStep end
        if l~=c['columns'] then
            modified=true
            c['columns']=l
            writeInfo(c)
            updateThermoformer()
        end
    end
    setDlgItemContent()
end

function columnStepChange(ui,id,newVal)
    local c=readInfo()
    local l=tonumber(newVal)
    if l then
        l=l*0.001
        local columns=c['columns']
        local pullLength=c['pullLength']
        if l<0.02 then l=0.02 end
        if l>pullLength/columns then l=pullLength/columns end
        local wt=c['plasticThickness']
        local extrL=c['extrusionLength']
        if l<extrL+3*wt then l=extrL+3*wt end
        if l~=c['columnStep'] then
            modified=true
            c['columnStep']=l
            writeInfo(c)
            updateThermoformer()
        end
    end
    setDlgItemContent()
end

function redChange(ui,id,newVal)
    modified=true
    local c=readInfo()
    c['color'][1]=newVal/100
    writeInfo(c)
    updateThermoformer()
end

function greenChange(ui,id,newVal)
    modified=true
    local c=readInfo()
    c['color'][2]=newVal/100
    writeInfo(c)
    updateThermoformer()
end

function blueChange(ui,id,newVal)
    modified=true
    local c=readInfo()
    c['color'][3]=newVal/100
    writeInfo(c)
    updateThermoformer()
end

function enabledClicked(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],64)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-64
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function partName_callback(ui,id,newVal)
    local c=readInfo()
    c['partName']=newVal
    modified=true
    writeInfo(c)
    updateThermoformer()
    setDlgItemContent()
end

function defaultDestination_callback(ui,id,newVal)
    local c=readInfo()
    c['partDestination']=newVal
    modified=true
    writeInfo(c)
    updateThermoformer()
    setDlgItemContent()
end


function placeOffsetChange_callback(ui,id,newVal)
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
    c['placeOffset']={t[1],t[2],t[3]}
    modified=true
    writeInfo(c)
    updateThermoformer()
    setDlgItemContent()
end

function updateEnabledDisabledItemsDlg()
    if ui then
        local enabled=simGetSimulationState()==sim_simulation_stopped
        simExtCustomUI_setEnabled(ui,10000,enabled,true)
        simExtCustomUI_setEnabled(ui,10001,enabled,true)
        simExtCustomUI_setEnabled(ui,10003,enabled,true)

        simExtCustomUI_setEnabled(ui,13,enabled,true)
        simExtCustomUI_setEnabled(ui,14,enabled,true)
        simExtCustomUI_setEnabled(ui,9,enabled,true)
        simExtCustomUI_setEnabled(ui,10,enabled,true)
        simExtCustomUI_setEnabled(ui,19,enabled,true)
    end
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setCheckboxValue(ui,15,(simBoolAnd32(config['bitCoded'],64)~=0) and 2 or 0,true)
        simExtCustomUI_setEditValue(ui,11,string.format("%.0f",config['velocity']/0.001),true)
        simExtCustomUI_setEditValue(ui,12,string.format("%.0f",config['acceleration']/0.001),true)
        simExtCustomUI_setEditValue(ui,9,string.format("%.0f",config['pullLength']/0.001),true)
        simExtCustomUI_setEditValue(ui,19,string.format("%.2f",config['indexesPerMinute']),true)
        simExtCustomUI_setLabelText(ui,21,string.format("%.1f",config['dwellTime']),true) -- actually computed
        simExtCustomUI_setEditValue(ui,10,string.format("%.0f",config['cuttingStationIndex']),true)
        simExtCustomUI_setEditValue(ui,1,string.format("%.0f",config['extrusionWidth']/0.001),true)
        simExtCustomUI_setEditValue(ui,2,string.format("%.0f",config['extrusionLength']/0.001),true)
        simExtCustomUI_setEditValue(ui,3,string.format("%.0f",config['extrusionDepth']/0.001),true)
        simExtCustomUI_setEditValue(ui,4,string.format("%.0f",config['plasticThickness']/0.001),true)
        simExtCustomUI_setEditValue(ui,5,string.format("%.0f",config['rows']),true)
        simExtCustomUI_setEditValue(ui,6,string.format("%.0f",config['rowStep']/0.001),true)
        simExtCustomUI_setEditValue(ui,7,string.format("%.0f",config['columns']),true)
        simExtCustomUI_setEditValue(ui,8,string.format("%.0f",config['columnStep']/0.001),true)
        simExtCustomUI_setEditValue(ui,16,config['partName'],true)
        simExtCustomUI_setEditValue(ui,17,config['partDestination'],true)
        simExtCustomUI_setEditValue(ui,20,string.format("%.0f",config['deactivationTime']),true)
        local off=config['placeOffset']
        simExtCustomUI_setEditValue(ui,18,string.format("%.0f , %.0f , %.0f",off[1]*1000,off[2]*1000,off[3]*1000),true)

        updateStartStopTriggerComboboxes()
        updateEnabledDisabledItemsDlg()
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
    <tabs id="77">
    <tab title="General" layout="form" id="10002">
                <label text="Enabled"/>
                <checkbox text="" onchange="enabledClicked" id="15"/>

                <label text="Max. velocity (mm/s)"/>
                <edit oneditingfinished="velocityChange" id="11"/>

                <label text="Acceleration (mm/s^2)"/>
                <edit oneditingfinished="accelerationChange" id="12"/>

                <label text="Stop on trigger"/>
                <combobox id="13" onchange="triggerStopChange_callback">
                </combobox>

                <label text="Restart on trigger"/>
                <combobox id="14" onchange="triggerStartChange_callback">
                </combobox>

                <label text="Indexes / minute"/>
                <edit oneditingfinished="indexPerMinuteChange" id="19"/>

                <label text="Pull length (mm)"/>
                <edit oneditingfinished="pullLengthChange" id="9"/>

                <label text="Cutting station index"/>
                <edit oneditingfinished="cuttingStationIndexChange" id="10"/>

                <label text="Calculated dwell time (s)"/>
                <label text="0" id="21"/>
    </tab>
    <tab title="Layout" layout="form" id="10000">
                <label text="Extrusion width (mm)"/>
                <edit oneditingfinished="extrWidthChange" id="1"/>

                <label text="Extrusion length (mm)"/>
                <edit oneditingfinished="extrLengthChange" id="2"/>

                <label text="Extrusion depth (mm)"/>
                <edit oneditingfinished="extrDepthChange" id="3"/>

                <label text="Plastic thickness (mm)"/>
                <edit oneditingfinished="plasticThicknessChange" id="4"/>

                <label text="Rows"/>
                <edit oneditingfinished="rowChange" id="5"/>

                <label text="Row step (mm)"/>
                <edit oneditingfinished="rowStepChange" id="6"/>

                <label text="Columns"/>
                <edit oneditingfinished="columnChange" id="7"/>

                <label text="Column step (mm)"/>
                <edit oneditingfinished="columnStepChange" id="8"/>
    </tab>
    <tab title="Name and Destination" layout="form" id="10003">

                <label text="Place offset (X, Y, Z, in mm)"/>
                <edit oneditingfinished="placeOffsetChange_callback" id="18"/>

                <label text="Location name"/>
                <edit oneditingfinished="partName_callback" id="16"/>

                <label text="Default destination"/>
                <edit oneditingfinished="defaultDestination_callback" id="17"/>

                <label text="Part deactivation time (s)"/>
                <edit oneditingfinished="deactivationTime_callback" id="20"/>
    </tab>

    <tab title="Color" layout="form" id="10001">
                <label text="Red"/>
                <hslider minimum="0" maximum="100" onchange="redChange" id="60"/>
                <label text="Green"/>
                <hslider minimum="0" maximum="100" onchange="greenChange" id="61"/>
                <label text="Blue"/>
                <hslider minimum="0" maximum="100" onchange="blueChange" id="62"/>

            <label text="" style="* {margin-left: 150px;}"/>
            <label text="" style="* {margin-left: 150px;}"/>
    </tab>
    </tabs>
        ]]

        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos--[[,closeable,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])

        local conf=readInfo()
        simExtCustomUI_setSliderValue(ui,60,conf['color'][1]*100,true)
        simExtCustomUI_setSliderValue(ui,61,conf['color'][2]*100,true)
        simExtCustomUI_setSliderValue(ui,62,conf['color'][3]*100,true)

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
    ----------------------------------------
    writeInfo(_info)
    baseShape=simGetObjectHandle('genericThermoformer_base')
    sampleHolder=simGetObjectHandle('genericThermoformer_sampleHolder')
    palletHolder=simGetObjectHandle('genericThermoformer_palletHolder')
    sensor=simGetObjectHandle('genericThermoformer_sensor')
	simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,true)

    -- For backward compatibility:
    local parts=simGetObjectsInTree(sampleHolder,sim_handle_all,1+2)
    for i=1,#parts,1 do
        createPalletPoint(parts[i])
    end
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
    updateEnabledDisabledItemsDlg()
--    showOrHideUiIfNeeded()
    showSamplesAndPallets(true)
    local conf=readInfo()
    conf['encoderDistance']=0
    conf['stopRequests']={}
    writeInfo(conf)
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    simJustStarted=true
    showSamplesAndPallets(false)
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