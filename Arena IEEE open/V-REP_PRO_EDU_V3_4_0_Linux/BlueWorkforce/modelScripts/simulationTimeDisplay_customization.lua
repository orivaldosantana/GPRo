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
        info['subtype']='time'
    end
    if not info['bitCoded'] then
        info['bitCoded']=0 -- 1: simplified display
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_SIMULATIONTIME_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_SIMULATIONTIME_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_SIMULATIONTIME_INFO','')
    end
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setCheckboxValue(ui,1,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        local sel=bwUtils.getSelectedEditWidget(ui)
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function simplified_callback(ui,id)
    local c=readInfo()
    c['bitCoded']=simBoolXor32(c['bitCoded'],1)
    writeInfo(c)
    modified=true
    setDlgItemContent()
end

function createDlg()
    if not ui then
        local xml =[[
                <label text="" style="* {margin-left: 150px;}"/>
                <label text="" style="* {margin-left: 150px;}"/>

                <label text="Simplified display"/>
                <checkbox text="" onchange="simplified_callback" id="1" />
        ]]
        ui=bwUtils.createCustomUi(xml,'Simulation Time',previousDlgPos,false,nil,false,false,false,'layout="form"')

        setDlgItemContent()
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

showOrHideUiIfNeeded=function()
    local s=simGetObjectSelection()
    if s and #s>=1 and s[#s]==model then
        showDlg()
    else
        removeDlg()
    end
end

if (sim_call_type==sim_customizationscriptcall_initialization) then
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    writeInfo(_info)
    simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,false)
    modified=false
    lastT=simGetSystemTimeInMs(-1)
    local objs=getObjectsWithTag('XYZ_SIMULATIONTIME_INFO',true)
    previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos=bwUtils.readSessionPersistentObjectData(model,"dlgPosAndSize")
    if #objs>1 then
        simRemoveObject(model)
        simRemoveObjectFromSelection(sim_handle_all)
        objs=getObjectsWithTag('XYZ_SIMULATIONTIME_INFO',true)
        simAddObjectToSelection(sim_handle_single,objs[1])
    else
        updatePluginRepresentation()
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

