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
        info['subtype']='binary'
    end
    if not info['width'] then
        info['width']=0.1
    end
    if not info['length'] then
        info['length']=0.1
    end
    if not info['height'] then
        info['height']=0.1
    end
    if not info['bitCoded'] then
        info['bitCoded']=1+4 -- 1=enabled, 2=detect parts only, 4=visible during simulation, 8=rising edge, 16=falling edge, 32=showStatistics
    end
    if not info['countForTrigger'] then
        info['countForTrigger']=1
    end
    if not info['delayForTrigger'] then
        info['delayForTrigger']=0
    end
    if not info['statText'] then
        info['statText']='Produced parts: '
    end
    if not info['detectionState'] then
        info['detectionState']=0
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_BINARYSENSOR_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_BINARYSENSOR_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_BINARYSENSOR_INFO','')
    end
end

function setSizes()
    local c=readInfo()
    local w=c['width']
    local l=c['length']
    local h=c['height']
    setObjectSize(model,w,l,h)
    local r,mmin=simGetObjectFloatParameter(sensor,sim_objfloatparam_objbbox_min_z)
    local r,mmax=simGetObjectFloatParameter(sensor,sim_objfloatparam_objbbox_max_z)
    local sz=mmax-mmin
    r,mmin=simGetObjectFloatParameter(sensor,sim_objfloatparam_objbbox_min_x)
    r,mmax=simGetObjectFloatParameter(sensor,sim_objfloatparam_objbbox_max_x)
    sx=mmax-mmin
    r,mmin=simGetObjectFloatParameter(sensor,sim_objfloatparam_objbbox_min_y)
    r,mmax=simGetObjectFloatParameter(sensor,sim_objfloatparam_objbbox_max_y)
    sy=mmax-mmin
    simScaleObject(sensor,w/sx,l/sy,h/sz)
    simSetObjectPosition(sensor,model,{0,0,-h*0.5})
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setEditValue(ui,1,string.format("%.0f",config['width']/0.001),true)
        simExtCustomUI_setEditValue(ui,2,string.format("%.0f",config['length']/0.001),true)
        simExtCustomUI_setEditValue(ui,3,string.format("%.0f",config['height']/0.001),true)


        simExtCustomUI_setCheckboxValue(ui,4,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,5,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],4)==0),true)
        simExtCustomUI_setCheckboxValue(ui,6,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],32)~=0),true)
        simExtCustomUI_setEditValue(ui,7,config['statText'],true)

        simExtCustomUI_setRadiobuttonValue(ui,10,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],2)==0),true)
        simExtCustomUI_setRadiobuttonValue(ui,11,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],2)~=0),true)
        simExtCustomUI_setRadiobuttonValue(ui,100,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],8+16)==8),true)
        simExtCustomUI_setRadiobuttonValue(ui,101,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],8+16)==16),true)
        simExtCustomUI_setRadiobuttonValue(ui,102,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],8+16)==8+16),true)
        simExtCustomUI_setEditValue(ui,103,string.format("%.0f",config['countForTrigger']),true)
        simExtCustomUI_setEditValue(ui,104,string.format("%.2f",config['delayForTrigger']),true)
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function updateEnabledDisabledItemsDlg()
    if ui then
        local config=readInfo()
        local enabled=simGetSimulationState()==sim_simulation_stopped
        simExtCustomUI_setEnabled(ui,1,enabled,true)
        simExtCustomUI_setEnabled(ui,2,enabled,true)
        simExtCustomUI_setEnabled(ui,3,enabled,true)
        simExtCustomUI_setEnabled(ui,5,enabled,true)
        simExtCustomUI_setEnabled(ui,6,enabled,true)
        simExtCustomUI_setEnabled(ui,7,enabled and simBoolAnd32(config['bitCoded'],32)~=0,true)
        simExtCustomUI_setEnabled(ui,10,enabled,true)
        simExtCustomUI_setEnabled(ui,11,enabled,true)
        simExtCustomUI_setEnabled(ui,100,enabled,true)
        simExtCustomUI_setEnabled(ui,101,enabled,true)
        simExtCustomUI_setEnabled(ui,102,enabled,true)
        simExtCustomUI_setEnabled(ui,103,enabled,true)
        simExtCustomUI_setEnabled(ui,104,enabled,true)
    end
end

function enabled_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],1)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-1
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function visible_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],4)
    if newVal~=0 then
        c['bitCoded']=c['bitCoded']-4
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function showStats_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],32)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-32
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg()
end

function anythingDetectClick_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    c['bitCoded']=c['bitCoded']-2
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function partsDetectClick_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function riseClick_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],8+16)
    c['bitCoded']=c['bitCoded']-16
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function fallClick_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],8+16)
    c['bitCoded']=c['bitCoded']-8
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function riseAndFallClick_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],8+16)
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function showStatsChange_callback(ui,id,v)
    local c=readInfo()
    if v~=c['statText'] then
        modified=true
        c['statText']=v
        writeInfo(c)
    end
    setDlgItemContent()
end

function widthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.001 then v=0.001 end
        if v>1 then v=1 end
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
        if v<0.001 then v=0.001 end
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
        if v<0.001 then v=0.001 end
        if v>1 then v=1 end
        if v~=c['height'] then
            modified=true
            c['height']=v
            writeInfo(c)
            setSizes()
        end
    end
    setDlgItemContent()
end

function countForTriggerChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v~=c['countForTrigger'] then
            modified=true
            c['countForTrigger']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function delayForTriggerChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0 then v=0 end
        if v>120 then v=120 end
        if v~=c['delayForTrigger'] then
            modified=true
            c['delayForTrigger']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml=[[
        <tabs id="77">
            <tab title="General properties" layout="form">
                <label text="Width (mm)"/>
                <edit oneditingfinished="widthChange_callback" id="1"/>

                <label text="Length (mm)"/>
                <edit oneditingfinished="lengthChange_callback" id="2"/>

                <label text="Height (mm)"/>
                <edit oneditingfinished="heightChange_callback" id="3"/>

                <label text="Enabled"/>
                <checkbox text="" onchange="enabled_callback" id="4" />

                <label text="Hidden during simulation"/>
                <checkbox text="" onchange="visible_callback" id="5" />

                <checkbox text="Show statistics, text:" onchange="showStats_callback" id="6" />
                <edit oneditingfinished="showStatsChange_callback" id="7"/>
            </tab>
            <tab title="Trigger conditions" layout="form">
                <label text="Detect"/>
                <radiobutton text="anything" onclick="anythingDetectClick_callback" id="10" />

                <label text=""/>
                <radiobutton text="Detect parts only" onclick="partsDetectClick_callback" id="11" />

                

                
                <label text="Count"/>
                <radiobutton text="rising edge" onclick="riseClick_callback" id="100" />

                <label text=""/>
                <radiobutton text="falling edge" onclick="fallClick_callback" id="101" />

                <label text=""/>
                <radiobutton text="rising and falling edge" onclick="riseAndFallClick_callback" id="102" />

                

                
                <label text="Increments for trigger"/>
                <edit oneditingfinished="countForTriggerChange_callback" id="103"/>

                <label text="Delay for trigger (s)"/>
                <edit oneditingfinished="delayForTriggerChange_callback" id="104"/>
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
    sensor=simGetObjectHandle('genericBinarySensor_sensor')
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    writeInfo(_info)
    simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,true)
    updatePluginRepresentation()
    previousDlgPos=bwUtils.readSessionPersistentObjectData(model,"dlgPosAndSize")
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
--    updateStatusText()
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_simulationpause) then
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_firstaftersimulation) then
    local c=readInfo()
    c['detectionState']=0
    writeInfo(c)
    simSetModelProperty(model,0)
    updateEnabledDisabledItemsDlg()
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    local c=readInfo()
    local show=bwUtils.modifyAuxVisualizationItems(simBoolAnd32(c['bitCoded'],4)>0)
    if not show then
        simSetModelProperty(model,sim_modelproperty_not_visible)
    end
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
    bwUtils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousDlgPos)
end