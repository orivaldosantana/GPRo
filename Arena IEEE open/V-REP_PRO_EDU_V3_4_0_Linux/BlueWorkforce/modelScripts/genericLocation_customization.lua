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
        info['subtype']='location'
    end
    if not info['name'] then
        info['name']='LOCATION1'
    end
    if not info['status'] then
        info['status']='free'
    end
    if not info['bitCoded'] then
        info['bitCoded']=1
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_LOCATION_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_LOCATION_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_LOCATION_INFO','')
    end
end

function setColor(red,green,blue,spec)
    simSetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
    simSetShapeColor(model,nil,sim_colorcomponent_specular,{spec,spec,spec})
end

function getColor()
    local r,rgb=simGetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse)
    local r,spec=simGetShapeColor(model,nil,sim_colorcomponent_specular)
    return rgb[1],rgb[2],rgb[3],(spec[1]+spec[2]+spec[3])/3
end

function updateStatusText()
    if ui then
        local config=readInfo()
        local oldVal=simExtCustomUI_getEditValue(ui,2)
        if oldVal~=config['status'] then
            simExtCustomUI_setEditValue(ui,2,config['status'],true)
        end
    end
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setEditValue(ui,1,config['name'],true)
        simExtCustomUI_setCheckboxValue(ui,3,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        local red,green,blue,spec=getColor()
        simExtCustomUI_setSliderValue(ui,11,red*100,true)
        simExtCustomUI_setSliderValue(ui,12,green*100,true)
        simExtCustomUI_setSliderValue(ui,13,blue*100,true)
        simExtCustomUI_setSliderValue(ui,14,spec*100,true)
        updateStatusText()
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function updateEnabledDisabledItemsDlg()
    if ui then
        local enabled=simGetSimulationState()==sim_simulation_stopped
        simExtCustomUI_setEnabled(ui,1,enabled,true)
        simExtCustomUI_setEnabled(ui,3,enabled,true)
        simExtCustomUI_setEnabled(ui,11,enabled,true)
        simExtCustomUI_setEnabled(ui,12,enabled,true)
        simExtCustomUI_setEnabled(ui,13,enabled,true)
        simExtCustomUI_setEnabled(ui,14,enabled,true)

        simExtCustomUI_setEnabled(ui,2,false,true)
    end
end

function nameChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=newVal
    if v then
        if v~=c['name'] then
            modified=true
            c['name']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
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

function redChange_callback(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(newVal/100,g,b,s)
end

function greenChange_callback(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(r,newVal/100,b,s)
end

function blueChange_callback(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(r,g,newVal/100,s)
end

function specularChange_callback(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(r,g,b,newVal/100)
end

function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
                <label text="Location name"/>
                <edit oneditingfinished="nameChange_callback" id="1"/>

                <label text="Hidden during simulation"/>
                <checkbox text="" onchange="hidden_callback" id="3" />

                <label text="Status"/>
                <edit id="2"/>

            <label text="" style="* {margin-left: 180px;}"/>
            <label text="" style="* {margin-left: 180px;}"/>

            <label text="Red"/>
                <hslider minimum="0" maximum="100" onchange="redChange_callback" id="11"/>
                <label text="Green"/>
                <hslider minimum="0" maximum="100" onchange="greenChange_callback" id="12"/>
                <label text="Blue"/>
                <hslider minimum="0" maximum="100" onchange="blueChange_callback" id="13"/>
                <label text="Specular"/>
                <hslider minimum="0" maximum="100" onchange="specularChange_callback" id="14"/>
        ]]
        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos,false,nil,false,false,false,'layout="form"')

        setDlgItemContent()
        updateEnabledDisabledItemsDlg()
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
        simExtCustomUI_destroy(ui)
        ui=nil
    end
end

if (sim_call_type==sim_customizationscriptcall_initialization) then
    modified=false
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    writeInfo(_info)
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
    updateStatusText()
    showOrHideUiIfNeeded()
end


if (sim_call_type==sim_customizationscriptcall_simulationpause) then
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_firstaftersimulation) then
    local c=readInfo()
    c['status']='free'
    writeInfo(c)
    simSetModelProperty(model,0)
    updateStatusText()
    updateEnabledDisabledItemsDlg()
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    simJustStarted=true
    local c=readInfo()
    if simBoolAnd32(c['bitCoded'],1)~=0 then
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