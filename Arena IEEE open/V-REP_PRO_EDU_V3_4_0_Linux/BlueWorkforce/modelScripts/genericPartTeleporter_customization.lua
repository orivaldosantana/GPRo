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
    if x<0.05 then x=0.05 end
    if y<0.05 then y=0.05 end
    if z<0.05 then z=0.05 end
    simScaleObject(h,x/sx,y/sy,z/sz)
end

function getDefaultInfoForNonExistingFields(info)
    if not info['version'] then
        info['version']=_MODELVERSION_
    end
    if not info['subtype'] then
        info['subtype']='teleporter'
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
        info['bitCoded']=1 -- 1=enabled, 2=isOrigin, 4=hiddenDuringSimulation
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_PARTTELEPORTER_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_PARTTELEPORTER_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_PARTTELEPORTER_INFO','')
    end
end

function setSizes()
    local c=readInfo()
    local w=c['width']
    local l=c['length']
    local h=c['height']
    setObjectSize(model,w,l,h)
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

        simExtCustomUI_setRadiobuttonValue(ui,10,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],2)~=0),true)
        simExtCustomUI_setRadiobuttonValue(ui,11,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],2)==0),true)
        local red,green,blue=getColor()
        simExtCustomUI_setSliderValue(ui,7,red*100,true)
        simExtCustomUI_setSliderValue(ui,8,green*100,true)
        simExtCustomUI_setSliderValue(ui,9,blue*100,true)
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function updateEnabledDisabledItemsDlg()
    if ui then
        local c=readInfo()
        local enabled=simGetSimulationState()==sim_simulation_stopped
        simExtCustomUI_setEnabled(ui,1,enabled,true)
        simExtCustomUI_setEnabled(ui,2,enabled,true)
        simExtCustomUI_setEnabled(ui,3,enabled,true)
        simExtCustomUI_setEnabled(ui,5,enabled,true)
        simExtCustomUI_setEnabled(ui,10,enabled,true)
        simExtCustomUI_setEnabled(ui,11,enabled,true)
        simExtCustomUI_setEnabled(ui,12,enabled and simBoolAnd32(c['bitCoded'],2)>0,true)
        simExtCustomUI_setEnabled(ui,7,enabled,true)
        simExtCustomUI_setEnabled(ui,8,enabled,true)
        simExtCustomUI_setEnabled(ui,9,enabled,true)
    end
end

function getAvailableDestinations()
    local l=simGetObjectsInTree(sim_handle_scene,sim_handle_all,0)
    local retL={}
    for i=1,#l,1 do
        local data=simReadCustomDataBlock(l[i],'XYZ_PARTTELEPORTER_INFO')
        if data then
            data=simUnpackTable(data)
            if simBoolAnd32(data['bitCoded'],2)==0 then
                retL[#retL+1]={simGetObjectName(l[i]),l[i]}
            end
        end
    end
    return retL
end

function enabled_callback(ui,id,newVal)
    modified=true
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],1)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-1
    end
    writeInfo(c)
    setDlgItemContent()
end

function visible_callback(ui,id,newVal)
    modified=true
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],4)
    if newVal~=0 then
        c['bitCoded']=c['bitCoded']-4
    end
    writeInfo(c)
    setDlgItemContent()
end

function setColor(red,green,blue,spec)
    simSetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
end

function getColor()
    local r,rgb=simGetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse)
    return rgb[1],rgb[2],rgb[3]
end

function originPodClick_callback(ui,id)
    modified=true
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    writeInfo(c)
    updateEnabledDisabledItemsDlg()
    setDlgItemContent()
end

function destinationPodClick_callback(ui,id)
    modified=true
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    c['bitCoded']=c['bitCoded']-2
    writeInfo(c)
    updateEnabledDisabledItemsDlg()
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

function destinationChange_callback(ui,id,newIndex)
    local newLoc=comboDestination[newIndex+1][2]
    modified=true
    bwUtils.setReferencedObjectHandle(model,REF_DESTINATION,newLoc)
end

function redChange(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(newVal/100,g,b,s)
end

function greenChange(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(r,newVal/100,b,s)
end

function blueChange(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(r,g,newVal/100,s)
end

function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
    <tabs id="77">
    <tab title="Properties" layout="form">
                <label text="Width (mm)"/>
                <edit oneditingfinished="widthChange_callback" id="1"/>

                <label text="Length (mm)"/>
                <edit oneditingfinished="lengthChange_callback" id="2"/>

                <label text="Height (mm)"/>
                <edit oneditingfinished="heightChange_callback" id="3"/>

                <radiobutton text="Pod is source pod, destination pod is" onclick="originPodClick_callback" id="10" />
                <combobox id="12" onchange="destinationChange_callback">
                </combobox>

                <radiobutton text="Pod is destination pod" onclick="destinationPodClick_callback" id="11" />
                <label text=""/>

                <label text="Enabled"/>
                <checkbox text="" onchange="enabled_callback" id="4" />

                <label text="Hidden during simulation"/>
                <checkbox text="" onchange="visible_callback" id="5" />

    </tab>
    <tab title="Colors" layout="form">
                <label text="Red"/>
                <hslider minimum="0" maximum="100" onchange="redChange" id="7"/>
                <label text="Green"/>
                <hslider minimum="0" maximum="100" onchange="greenChange" id="8"/>
                <label text="Blue"/>
                <hslider minimum="0" maximum="100" onchange="blueChange" id="9"/>

                <label text="" style="* {margin-left: 180px;}"/>
                <label text="" style="* {margin-left: 180px;}"/>
    </tab>
    </tabs>
        ]]
        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos--[[,closeable,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])

        local c=readInfo()
        local loc=getAvailableDestinations()
        local exceptItems={}
        exceptItems[simGetObjectName(model)]=true
        comboDestination=customUi_populateCombobox(ui,12,loc,exceptItems,bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_DESTINATION)),true,{{'<NONE>',-1}})

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
    REF_DESTINATION=1 -- index of referenced destination pod object handle
    modified=false
    dlgMainTabIndex=0
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    -- Following for backward compatibility:
    if _info['destination'] then
        bwUtils.setReferencedObjectHandle(model,REF_DESTINATION,getObjectHandle_noErrorNoSuffixAdjustment(_info['destination']))
        _info['destination']=nil
    end
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
--    updateStatusText()
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_simulationpause) then
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_firstaftersimulation) then
    simSetModelProperty(model,0)
    updateEnabledDisabledItemsDlg()
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    local c=readInfo()
    local show=bwUtils.modifyAuxVisualizationItems(simBoolAnd32(c['bitCoded'],4)~=0)
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
    bwUtils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos)
end