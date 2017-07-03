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
        info['subtype']='tagger'
    end
    if not info['width'] then
        info['width']=0.1
    end
    if not info['length'] then
        info['length']=0.1
    end
    if not info['height'] then
        info['height']=0.3
    end
    if not info['bitCoded'] then
        info['bitCoded']=1+4
    end
    if not info['partNameDistribution'] then
        info['partNameDistribution']="{1,'<NEW_PART_NAME>'}"
    end
    if not info['partDestinationNameDistribution'] then
        info['partDestinationNameDistribution']="{1,'<NEW_PART_DESTINATION_NAME>'}"
    end
    if not info['partColorDistribution'] then
        info['partColorDistribution']="{1,{1,0,1}}"
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_PARTTAGGER_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_PARTTAGGER_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_PARTTAGGER_INFO','')
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
        simExtCustomUI_setEditValue(ui,20,string.format("%.0f",config['width']/0.001),true)
        simExtCustomUI_setEditValue(ui,21,string.format("%.0f",config['length']/0.001),true)
        simExtCustomUI_setEditValue(ui,22,string.format("%.0f",config['height']/0.001),true)
        simExtCustomUI_setCheckboxValue(ui,3,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,4,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],8)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,30,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],2)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,31,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],4)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,32,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],16)~=0),true)
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function updateEnabledDisabledItemsDlg()
    if ui then
        local config=readInfo()
        local enabled=simGetSimulationState()==sim_simulation_stopped
        simExtCustomUI_setEnabled(ui,20,enabled,true)
        simExtCustomUI_setEnabled(ui,21,enabled,true)
        simExtCustomUI_setEnabled(ui,22,enabled,true)
        simExtCustomUI_setEnabled(ui,25,(simBoolAnd32(config['bitCoded'],2)~=0)and enabled,true)
        simExtCustomUI_setEnabled(ui,24,(simBoolAnd32(config['bitCoded'],4)~=0)and enabled,true)
        simExtCustomUI_setEnabled(ui,26,(simBoolAnd32(config['bitCoded'],16)~=0)and enabled,true)
        simExtCustomUI_setEnabled(ui,3,enabled,true)
        simExtCustomUI_setEnabled(ui,4,enabled,true)
        simExtCustomUI_setEnabled(ui,30,enabled,true)
        simExtCustomUI_setEnabled(ui,31,enabled,true)
        simExtCustomUI_setEnabled(ui,32,enabled,true)
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

function console_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],8)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-8
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
        if v<0.001 then v=0.001 end
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

function loadTheString()
    local f=loadstring("local counter=0\n".."return "..theString)
    return f
end

function distributionDlg(title,fieldName,tempComment)
    local prop=readInfo()
    local s="800 400"
    local p="200 200"
    if distributionDlgSize then
        s=distributionDlgSize[1]..' '..distributionDlgSize[2]
    end
    if distributionDlgPos then
        p=distributionDlgPos[1]..' '..distributionDlgPos[2]
    end
    local xml = [[ <editor title="]]..title..[[" size="]]..s..[[" position="]]..p..[[" tabWidth="4" textColor="50 50 50" backgroundColor="190 190 190" selectionColor="128 128 255" useVrepKeywords="true" isLua="true"> <keywords1 color="152 0 0" > </keywords1> <keywords2 color="220 80 20" > </keywords2> </editor> ]]            



    local initialText=prop[fieldName]
    if tempComment then
        initialText=initialText.."--[[tmpRem\n\n"..tempComment.."\n\n--]]"
    end
    local modifiedText
    modifiedText,distributionDlgSize,distributionDlgPos=simOpenTextEditor(initialText,xml)
    theString='{'..modifiedText..'}' -- variable needs to be global here
    local bla,f=pcall(loadTheString)
    local success=false
    if f then
        local res,err=xpcall(f,function(err) return debug.traceback(err) end)
        if res then
            modifiedText=bwUtils.removeTmpRem(modifiedText)
            prop[fieldName]=modifiedText
            writeInfo(prop)
            modified=true
            success=true
        end
    end
    if not success then
        simMsgBox(sim_msgbox_type_warning,sim_msgbox_buttons_ok,'Input Error',"The distribution string is ill-formated.")
    end
end


function partNameDistribution_callback(ui,id,newVal)
    local tmpTxt=[[
a) Usage:
   {partialProportion1,<partName>},{partialProportion2,<partName>}

b) Example:
   {2.1,"CUBE"},{1.5,"SPHERE"} --> out of (2.1+1.5) items, 2.1 will be named "CUBE", and 1.5 will be named "SPHERE" (statistically)

c) You may use the variable 'counter' as in following example:
   {counter%2,"CUBE"},{(counter+1)%2,"SPHERE"} --> items names alternate between "CUBE" and "SPHERE"
   
d) You may use '<DEFAULT>', to refer to the default value, as in following example:
   {1,"<DEFAULT>"},{1,"SPHERE"} --> out of 2 items, one will keep its name, the other will be renamed to "SPHERE" (statistically)]]
    distributionDlg('Part Name Distribution','partNameDistribution',tmpTxt)
end

function partDestinationNameDistribution_callback(ui,id,newVal)
    local tmpTxt=[[
a) Usage:
   {partialProportion1,<partDestination>},{partialProportion2,<partDestination>}

b) Example:
   {2.1,"PLATE"},{1.5,"BASKET"} --> out of (2.1+1.5) items, 2.1 will have a destination "PLATE", and 1.5 will have a destination "BASKET" (statistically)

c) You may use the variable 'counter' as in following example:
   {counter%2,"PLATE"},{(counter+1)%2,"BASKET"} --> destinations alternate between "PLATE" and "BASKET"
   
d) You may use '<DEFAULT>', to refer to the default value, as in following example:
   {1,"<DEFAULT>"},{1,"BASKET"} --> out of 2 items, one will keep its destination, the other will have a destination "BASKET" (statistically)]]
    distributionDlg('Part Destination Name Distribution','partDestinationNameDistribution',tmpTxt)
end

function partColorDistribution_callback(ui,id,newVal)
    local tmpTxt=[[
a) Usage:
   {partialProportion1,<rgbColor>},{partialProportion2,<rgbColor>} where <rgbColor> is a color value of type {red,green,blue}, where each color component can vary between 0 and 1

b) Example:
   {2.1,{1,0,0}},{1.5,{0,0,1}} --> out of (2.1+1.5) items, 2.1 will be red, and 1.5 will be blue (statistically)

c) You may use the variable 'counter' as in following example:
   {counter%2,{1,0,0}},{(counter+1)%2,{0,0,1}} --> colors alternate between red and blue
   
d) You may use '<DEFAULT>', to refer to the default value, as in following example:
   {1,"<DEFAULT>"},{1,{0,0,1}} --> out of 2 items, one will keep its color, the other turn blue (statistically)]]
    distributionDlg('Part Color Distribution','partColorDistribution',tmpTxt)
end

function partNameEnable_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-2
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg()
end

function partDestinationNameEnable_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],4)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-4
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg()
end

function partColorEnable_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],16)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-16
    end
    modified=true
    writeInfo(c)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg()
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

                <checkbox text="Change part name, distribution" onchange="partNameEnable_callback" id="30" />
                <button text="Edit" checked="false"  onclick="partNameDistribution_callback" id="25" />

                <checkbox text="Change part destination, distribution" onchange="partDestinationNameEnable_callback" id="31" />
                <button text="Edit" checked="false"  onclick="partDestinationNameDistribution_callback" id="24" />

                <checkbox text="Change part color, distribution" onchange="partColorEnable_callback" id="32" />
                <button text="Edit" checked="false"  onclick="partColorDistribution_callback" id="26" />

                <label text="Hidden during simulation"/>
                <checkbox text="" onchange="hidden_callback" id="3" />

                <label text="Show debug console"/>
                <checkbox text="" onchange="console_callback" id="4" />
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