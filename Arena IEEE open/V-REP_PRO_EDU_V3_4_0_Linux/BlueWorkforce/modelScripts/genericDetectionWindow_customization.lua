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
        info['subtype']='window'
    end
    if not info['width'] then
        info['width']=0.5
    end
    if not info['length'] then
        info['length']=0.5
    end
    if not info['height'] then
        info['height']=0.1
    end
    if not info['maxLabelAngle'] then
        info['maxLabelAngle']=30*math.pi/180
    end
    if not info['detectionDiameter'] then
        info['detectionDiameter']=0.1
    end
    if not info['detectedItems'] then
        info['detectedItems']={}
    end
    if not info['bitCoded'] then
        info['bitCoded']=1 -- 1=hidden,2=console,4=showPts,8=showLabelPts,16=labelSensTop,32=labelSensSide1,64=labelSensSide2,128=colorLabel,256=showStatistics
    end
end

function readInfo()
        simWriteCustomDataBlock(model,'XYZ_PARTDETECTIONWINDOW_INFO','')
    local data=simReadCustomDataBlock(model,'XYZ_DETECTIONWINDOW_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_DETECTIONWINDOW_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_DETECTIONWINDOW_INFO','')
    end
end

function setSizes()
    local c=readInfo()
    local w=c['width']
    local l=c['length']
    local h=c['height']
    local d=c['detectionDiameter']
    setObjectSize(box,w,l,h)
    local r,mmin=simGetObjectFloatParameter(sensor1,sim_objfloatparam_objbbox_min_z)
    local r,mmax=simGetObjectFloatParameter(sensor1,sim_objfloatparam_objbbox_max_z)
    local sz=mmax-mmin
    r,mmin=simGetObjectFloatParameter(sensor1,sim_objfloatparam_objbbox_min_x)
    r,mmax=simGetObjectFloatParameter(sensor1,sim_objfloatparam_objbbox_max_x)
    sd=mmax-mmin
    simScaleObject(sensor1,d/sd,d/sd,(h+0.01)/sz)
    r,mmin=simGetObjectFloatParameter(sensor2,sim_objfloatparam_objbbox_min_z)
    r,mmax=simGetObjectFloatParameter(sensor2,sim_objfloatparam_objbbox_max_z)
    sz=mmax-mmin
    simScaleObject(sensor2,(h+0.01)/sz,(h+0.01)/sz,(h+0.01)/sz)
    simSetObjectPosition(box,model,{0,0,h*0.5})
    simSetObjectPosition(sensor1,model,{0,0,h})
    simSetObjectPosition(sensor2,model,{0,0,h})
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setEditValue(ui,20,string.format("%.0f",config['width']/0.001),true)
        simExtCustomUI_setEditValue(ui,21,string.format("%.0f",config['length']/0.001),true)
        simExtCustomUI_setEditValue(ui,22,string.format("%.0f",config['height']/0.001),true)
        simExtCustomUI_setEditValue(ui,23,string.format("%.0f",config['detectionDiameter']/0.001),true)
        simExtCustomUI_setEditValue(ui,24,string.format("%.0f",180*config['maxLabelAngle']/math.pi),true)
        simExtCustomUI_setCheckboxValue(ui,3,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,4,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],2)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,5,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],4)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,6,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],8)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,30,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],16)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,31,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],32)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,32,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],64)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,33,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],128)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,34,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],256)~=0),true)
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function updateEnabledDisabledItemsDlg()
    if ui then
        local enabled=simGetSimulationState()==sim_simulation_stopped
        simExtCustomUI_setEnabled(ui,20,enabled,true)
        simExtCustomUI_setEnabled(ui,21,enabled,true)
        simExtCustomUI_setEnabled(ui,22,enabled,true)
        simExtCustomUI_setEnabled(ui,23,enabled,true)
        simExtCustomUI_setEnabled(ui,24,enabled,true)
        simExtCustomUI_setEnabled(ui,30,enabled,true)
        simExtCustomUI_setEnabled(ui,31,enabled,true)
        simExtCustomUI_setEnabled(ui,32,enabled,true)
        simExtCustomUI_setEnabled(ui,33,enabled,true)
        simExtCustomUI_setEnabled(ui,34,enabled,true)
        simExtCustomUI_setEnabled(ui,3,enabled,true)
        simExtCustomUI_setEnabled(ui,4,enabled,true)
        simExtCustomUI_setEnabled(ui,5,enabled,true)
        simExtCustomUI_setEnabled(ui,6,enabled,true)
    end
end

function hidden_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],1)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-1
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function showPoints_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],4)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-4
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function console_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-2
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function label_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],8)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-8
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function labelSensorTop_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],16)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-16
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function labelSensorSide1_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],32)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-32
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function labelSensorSide2_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],64)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-64
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function colorLabel_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],128)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-128
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function showStats_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],256)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-256
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function widthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.1 then v=0.1 end
        if v>2 then v=2 end
        if v~=c['width'] then
            modified=true
            c['width']=v
            writeInfo(c)
            setSizes()
        end
    end
    setDlgItemContent()
end

function lengthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>1 then v=1 end
        if v~=c['length'] then
            modified=true
            c['length']=v
            writeInfo(c)
            setSizes()
        end
    end
    setDlgItemContent()
end

function heightChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.05 then v=0.05 end
        if v>0.5 then v=0.5 end
        if v~=c['height'] then
            modified=true
            c['height']=v
            writeInfo(c)
            setSizes()
        end
    end
    setDlgItemContent()
end

function diameterChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.001 then v=0.001 end
        if v>0.5 then v=0.5 end
        if v~=c['detectionDiameter'] then
            modified=true
            c['detectionDiameter']=v
            writeInfo(c)
            setSizes()
        end
    end
    setDlgItemContent()
end

function labelAngleChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<1 then v=1 end
        if v>80 then v=80 end
        v=v*math.pi/180
        if v~=c['maxLabelAngle'] then
            modified=true
            c['maxLabelAngle']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
        <tabs id="77">
            <tab title="General properties" layout="form">
                <label text="Width (mm)"/>
                <edit oneditingfinished="widthChange_callback" id="20"/>

                <label text="Length (mm)"/>
                <edit oneditingfinished="lengthChange_callback" id="21"/>

                <label text="Height (mm)"/>
                <edit oneditingfinished="heightChange_callback" id="22"/>

                <label text="Detection diameter (mm)"/>
                <edit oneditingfinished="diameterChange_callback" id="23"/>

                <label text="Max. label detection angle (deg)"/>
                <edit oneditingfinished="labelAngleChange_callback" id="24"/>

                <label text="Label sensor, from top"/>
                <checkbox text="" onchange="labelSensorTop_callback" id="30" />

                <label text="Label sensor, from side 1"/>
                <checkbox text="" onchange="labelSensorSide1_callback" id="31" />

                <label text="Label sensor, from side 2"/>
                <checkbox text="" onchange="labelSensorSide2_callback" id="32" />
            </tab>

            <tab title="More" layout="form">
                <label text="Color detected labels"/>
                <checkbox text="" onchange="colorLabel_callback" id="33" />

                <label text="Show detected labels"/>
                <checkbox text="" onchange="label_callback" id="6" />

                <label text="Visualize detected items"/>
                <checkbox text="" onchange="showPoints_callback" id="5" />

                <label text="Hidden during simulation"/>
                <checkbox text="" onchange="hidden_callback" id="3" />

                <label text="Show debug console"/>
                <checkbox text="" onchange="console_callback" id="4" />

                <label text="Show statistics"/>
                <checkbox text="" onchange="showStats_callback" id="34" />

                <label text="" style="* {margin-left: 190px;}"/>
                <label text="" style="* {margin-left: 190px;}"/>
            </tab>
        </tabs>
        ]]
        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos--[[,closeable,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])

        setDlgItemContent()
        updateEnabledDisabledItemsDlg()
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
    modified=false
    dlgMainTabIndex=0
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    writeInfo(_info)
    box=simGetObjectHandle('genericDetectionWindow_box')
    sensor1=simGetObjectHandle('genericDetectionWindow_sensor1')
    sensor2=simGetObjectHandle('genericDetectionWindow_sensor2')
    simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,false)
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


if (sim_call_type==sim_customizationscriptcall_firstaftersimulation) then
    local c=readInfo()
    c['detectedItems']={}
    writeInfo(c)
    simSetModelProperty(model,0)
    updateEnabledDisabledItemsDlg()
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    removeDlg()
    local c=readInfo()
    local show=bwUtils.modifyAuxVisualizationItems(simBoolAnd32(c['bitCoded'],1)==0)
    if not show then
        simSetModelProperty(model,sim_modelproperty_not_visible)
    end
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