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
        info['subtype']='control'
    end
    if not info['bitCoded'] then
        info['bitCoded']=0 -- 0-3=0:no override, 1:force aux. vis. items not show, 2:force show, 4=override part sleep time, 8=display warning with overlapping models, 16=do not open property dialogs
    end
    if not info['deactivationTime'] then
        info['deactivationTime']=9999
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_OVERRIDE_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_OVERRIDE_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_OVERRIDE_INFO','')
    end
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setCheckboxValue(ui,1,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],4)~=0),true)
        simExtCustomUI_setEditValue(ui,2,string.format("%.0f",config['deactivationTime']),true)
        simExtCustomUI_setEnabled(ui,2,simBoolAnd32(config['bitCoded'],4)~=0,true)
        simExtCustomUI_setCheckboxValue(ui,6,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],8)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,7,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],16)~=0),true)

        simExtCustomUI_setRadiobuttonValue(ui,3,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],3)==0),true)
        simExtCustomUI_setRadiobuttonValue(ui,4,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],3)==1),true)
        simExtCustomUI_setRadiobuttonValue(ui,5,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],3)==2),true)
        local sel=bwUtils.getSelectedEditWidget(ui)
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function visualArtifactsNoOverrideClick_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],1+2)
    c['bitCoded']=c['bitCoded']-3
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function visualArtifactsForceHideClick_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],1+2)
    c['bitCoded']=c['bitCoded']-2
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function visualArtifactsForceShowClick_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],1+2)
    c['bitCoded']=c['bitCoded']-1
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function deactivationTimeToggle_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolXor32(c['bitCoded'],4)
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function overlappingToggle_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolXor32(c['bitCoded'],8)
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function disableDialogs_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolXor32(c['bitCoded'],16)
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function deactivationTimeChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<1 then v=1 end
        if v>1000000 then v=1000000 end
        if v~=c['deactivationTime'] then
            modified=true
            c['deactivationTime']=v
            writeInfo(c)
        end
    end
    setDlgItemContent()
end

function createDlg()
    if not ui then
        local xml =[[

                <checkbox text="Override part deactivation time (s)" onchange="deactivationTimeToggle_callback" id="1" />
                <edit oneditingfinished="deactivationTimeChange_callback" id="2"/>

                <label text="Display warning with overlapping models"/>
                <checkbox text="" onchange="overlappingToggle_callback" id="6" />

                <label text="Disable model property dialogs"/>
                <checkbox text="" onchange="disableDialogs_callback" id="7" />

                <label text="Do not override aux. visualization items"/>
                <radiobutton text="" onclick="visualArtifactsNoOverrideClick_callback" id="3" />

                <label text="Force aux. visualization items to hide"/>
                <radiobutton text="" onclick="visualArtifactsForceHideClick_callback" id="4" />
 
                <label text="Force aux. visualization items to show"/>
                <radiobutton text="" onclick="visualArtifactsForceShowClick_callback" id="5" />
        ]]
        ui=bwUtils.createCustomUi(xml,'Global Model Settings',previousDlgPos,false,nil,false,false,false,'layout="form"')
        setDlgItemContent()
--        updateEnabledDisabledItemsDlg()
--        simExtCustomUI_setCurrentTab(ui,77,dlgMainTabIndex,true)
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
--        dlgMainTabIndex=simExtCustomUI_getCurrentTab(ui,77)
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
    simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,false)
    previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos=bwUtils.readSessionPersistentObjectData(model,"dlgPosAndSize")
    local objs=getObjectsWithTag('XYZ_OVERRIDE_INFO',true)
    if #objs>1 then
        simRemoveObject(model)
        simRemoveObjectFromSelection(sim_handle_all)
        objs=getObjectsWithTag('XYZ_OVERRIDE_INFO',true)
        simAddObjectToSelection(sim_handle_single,objs[1])
    else
        updatePluginRepresentation()
    end
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
    simSetObjectInt32Parameter(model,sim_objintparam_visibility_layer,1)
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    simSetObjectInt32Parameter(model,sim_objintparam_visibility_layer,0)
    removeDlg()
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
    if simIsHandleValid(model)==1 then
        -- the associated object might already have been destroyed
        bwUtils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos)
    end
end