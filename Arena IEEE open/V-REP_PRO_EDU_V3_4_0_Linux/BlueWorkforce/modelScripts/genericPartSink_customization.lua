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
        info['subtype']='sink'
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
    if not info['status'] then
        info['status']='operational'
    end
    if not info['bitCoded'] then
        info['bitCoded']=1 -- 1=visibleDuringSimulation, 128=show statistics
    end
    if not info['destroyedCnt'] then
        info['destroyedCnt']=0
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_PARTSINK_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_PARTSINK_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_PARTSINK_INFO','')
    end
end

function setSizes()
    local c=readInfo()
    local w=c['width']
    local l=c['length']
    local h=c['height']
    setObjectSize(model,w,l,h)
    setObjectSize(sensor,w,l,h)
    local p=simGetObjectPosition(model,-1)
    --simSetObjectPosition(model,-1,{p[1],p[2],h*0.5})
    simSetObjectPosition(sensor,model,{0,0,-h*0.5+0.001})
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setEditValue(ui,20,string.format("%.0f",config['width']/0.001),true)
        simExtCustomUI_setEditValue(ui,21,string.format("%.0f",config['length']/0.001),true)
        simExtCustomUI_setEditValue(ui,22,string.format("%.0f",config['height']/0.001),true)
        simExtCustomUI_setCheckboxValue(ui,3,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,4,bwUtils.getCheckboxValFromBool(config['status']~='disabled'),true)
        simExtCustomUI_setCheckboxValue(ui,6,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],128)~=0),true)
        bwUtils.setSelectedEditWidget(ui,sel)
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

function showStatisticsClick_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],128)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-128
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
end

function enabled_callback(ui,id,newVal)
    local c=readInfo()
    if newVal==0 then
        c['status']='disabled'
    else
        c['status']='operational'
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
        if v<0.2 then v=0.2 end
        if v>5 then v=5 end
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
        if v<0.2 then v=0.2 end
        if v>5 then v=5 end
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
        if v<0.01 then v=0.01 end
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

function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
                <label text="Width (mm)"/>
                <edit oneditingfinished="widthChange_callback" id="20"/>

                <label text="Length (mm)"/>
                <edit oneditingfinished="lengthChange_callback" id="21"/>

                <label text="Height (mm)"/>
                <edit oneditingfinished="heightChange_callback" id="22"/>

                <label text="Enabled"/>
                <checkbox text="" onchange="enabled_callback" id="4" />

                <label text="Hidden during simulation"/>
                <checkbox text="" onchange="hidden_callback" id="3" />

                <label text="Show statistics"/>
                 <checkbox text="" checked="false" onchange="showStatisticsClick_callback" id="6"/>
        ]]
        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos,false,nil,false,false,false,'layout="form"')

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

if (sim_call_type==sim_customizationscriptcall_initialization) then
    modified=false
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    writeInfo(_info)
    sensor=simGetObjectHandle('genericPartSink_sensor')
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
    if wasDisabledBeforeSimulation then
        c['status']='disabled'
    else
        c['status']='operational'
    end
   c['destroyedCnt']=0
    writeInfo(c)
    simSetModelProperty(model,0)
    showOrHideUiIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    removeDlg()
    local c=readInfo()
    wasDisabledBeforeSimulation=c['status']=='disabled'
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