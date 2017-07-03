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
        info['subtype']='gripper'
    end
    if not info['size'] then
        info['size']=0
    end
    if not info['stacking'] then
        info['stacking']=1
    end
    if not info['stackingShift'] then
        info['stackingShift']=0.01
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_RAGNARGRIPPER_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_RAGNARGRIPPER_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_RAGNARGRIPPER_INFO','')
    end
end

function sizeChange_callback(ui,id,newVal)
    local c=readInfo()
    local s=0.042624
    if newVal>=0 then
        s=0.042624*(1+newVal/4)
    else
        s=0.021312*(1+(newVal+4)/4)
    end
    local r,mmin=simGetObjectFloatParameter(shape,sim_objfloatparam_objbbox_min_z)
    local r,mmax=simGetObjectFloatParameter(shape,sim_objfloatparam_objbbox_max_z)
    local sz=mmax-mmin
    simScaleObject(shape,s/sz,s/sz,s/sz)
    simSetObjectPosition(shape,sim_handle_parent,{0,0,-s*0.021416/0.042624})

    c['size']=newVal
    writeInfo(c)
    modified=true
end


function stackingChange_callback(uiHandle,id,newValue)
    local c=readInfo()
    newValue=tonumber(newValue)
    if newValue then
        newValue=math.floor(newValue)
        if newValue<1 then newValue=1 end
        if newValue>20 then newValue=20 end
        if newValue~=c['stacking'] then
            c['stacking']=newValue
            writeInfo(c)
            modified=true
        end
    end
    simExtCustomUI_setEditValue(ui,1,string.format("%.0f",c['stacking']),true)
end

function stackingShiftChange_callback(uiHandle,id,newValue)
    local c=readInfo()
    newValue=tonumber(newValue)
    if newValue then
        newValue=newValue/1000
        if newValue<0 then newValue=0 end
        if newValue>0.1 then newValue=0.1 end
        if newValue~=c['stackingShift'] then
            c['stackingShift']=newValue
            writeInfo(c)
            modified=true
        end
    end
    simExtCustomUI_setEditValue(ui,3,string.format("%.0f",c['stackingShift']*1000),true)
end

function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
                <label text="Size"/>
                <hslider tick-position="above" tick-interval="1" minimum="-4" maximum="4" onchange="sizeChange_callback" id="2"/>

                <label text="Stacking"/>
                <edit oneditingfinished="stackingChange_callback" id="1"/>

                <label text="Stacking shift (mm)"/>
                <edit oneditingfinished="stackingShiftChange_callback" id="3"/>
            <label text="" style="* {margin-left: 150px;}"/>
            <label text="" style="* {margin-left: 150px;}"/>
        ]]
        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos,false,nil,false,false,false,'layout="form"')
        local c=readInfo()
        simExtCustomUI_setSliderValue(ui,2,c['size'],true)
        simExtCustomUI_setEditValue(ui,1,string.format("%.0f",c['stacking']),true)
        simExtCustomUI_setEditValue(ui,3,string.format("%.0f",c['stackingShift']*1000),true)
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
    shape=simGetObjectHandle('genericGripper_shape')
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

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
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
    bwUtils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos)
end