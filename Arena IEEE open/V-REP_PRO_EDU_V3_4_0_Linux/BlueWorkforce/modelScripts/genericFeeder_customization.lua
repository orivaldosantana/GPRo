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
        info['subtype']='feeder'
    end
    if not info['length'] then
        info['length']=0
    end
    if not info['width'] then
        info['width']=0.3
    end
    if not info['height'] then
        info['height']=0
    end
    if not info['frequency'] then
        info['frequency']=1
    end
    if not info['algorithm'] then
        info['algorithm']=''
    end
    if not info['deactivationTime'] then
        info['deactivationTime']=20
    end
    if not info['conveyorDist'] then
        info['conveyorDist']=0.2
    end
    if not info['partDistribution'] then
        info['partDistribution']="{1,'BOX'}"
    end
    if not info['destinationDistribution'] then
        info['destinationDistribution']="{1,'BOX_DESTINATION'}"
    end
    if not info['shiftDistribution'] then
        info['shiftDistribution']="{1,-0.5},{2,-0.25},{4,0},{2,0.25},{1,0.5}"
    end
    if not info['rotationDistribution'] then
        info['rotationDistribution']="{1,-math.pi/2},{2,-math.pi/4},{4,0},{2,math.pi/4},{1,math.pi/2}"
    end
    if not info['weightDistribution'] then
        info['weightDistribution']="{1,'<DEFAULT>'}"
    end
    if not info['labelDistribution'] then
        info['labelDistribution']="{1,1+2+4}"
    end
    if not info['isoSizeScalingDistribution'] then
        info['isoSizeScalingDistribution']="{1,1}"
    end
    if not info['nonIsoSizeScalingDistribution'] then
        info['nonIsoSizeScalingDistribution']="{1,{1,1,1}}"
    end
    if not info['sizeScaling'] then
        info['sizeScaling']=0 -- 0:none, 1=iso, 2=non-iso
    end
    if not info['bitCoded'] then
        info['bitCoded']=3 -- 1=hidden, 2=enabled, 4-31:0=frequency, 4=sensor triggered, 8=user, 12=conveyorTriggered, 16=multi-feeder triggered, 128=show statistics
    end
    if not info['multiFeederTriggerCnt'] then
        info['multiFeederTriggerCnt']=0
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_FEEDER_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_FEEDER_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_FEEDER_INFO','')
    end
end

function setModelSize()
    local c=readInfo()
    local v={c['length'],c['width'],c['height']}
    for i=1,3,1 do
        if v[i]<0.05 then
            v[i]=0.05
        end
    end
    local r,mmin=simGetObjectFloatParameter(model,sim_objfloatparam_objbbox_min_x)
    local r,mmax=simGetObjectFloatParameter(model,sim_objfloatparam_objbbox_max_x)
    local sx=mmax-mmin
    local r,mmin=simGetObjectFloatParameter(model,sim_objfloatparam_objbbox_min_y)
    local r,mmax=simGetObjectFloatParameter(model,sim_objfloatparam_objbbox_max_y)
    local sy=mmax-mmin
    local r,mmin=simGetObjectFloatParameter(model,sim_objfloatparam_objbbox_min_z)
    local r,mmax=simGetObjectFloatParameter(model,sim_objfloatparam_objbbox_max_z)
    local sz=mmax-mmin
    simScaleObject(model,v[1]/sx,v[2]/sy,v[3]/sz)
end

function deactivationTimeChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<1 then v=1 end
        if v>100000 then v=100000 end
        if v~=c['deactivationTime'] then
            modified=true
            c['deactivationTime']=v
            writeInfo(c)
        end
    end
    simExtCustomUI_setEditValue(ui,789,string.format("%.1f",c['deactivationTime']),true)
end

function lengthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0 then v=0 end
        if v>2 then v=2 end
        if v~=c['length'] then
            modified=true
            c['length']=v
            writeInfo(c)
            setModelSize()
        end
    end
    simExtCustomUI_setEditValue(ui,60,string.format("%.0f",c['length']/0.001),true)
end

function widthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0 then v=0 end
        if v>2 then v=2 end
        if v~=c['width'] then
            modified=true
            c['width']=v
            writeInfo(c)
            setModelSize()
        end
    end
    simExtCustomUI_setEditValue(ui,1,string.format("%.0f",c['width']/0.001),true)
end

function heightChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0 then v=0 end
        if v>2 then v=2 end
        if v~=c['height'] then
            modified=true
            c['height']=v
            writeInfo(c)
            setModelSize()
        end
    end
    simExtCustomUI_setEditValue(ui,61,string.format("%.0f",c['height']/0.001),true)
end

function frequencyChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0 then v=0 end
        if v>10 then v=10 end
        if v~=c['frequency'] then
            modified=true
            c['frequency']=v
            writeInfo(c)
        end
    end
    simExtCustomUI_setEditValue(ui,2,string.format("%.2f",c['frequency']),true)
end

function conveyorDistanceChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>2 then v=2 end
        if v~=c['conveyorDist'] then
            modified=true
            c['conveyorDist']=v
            writeInfo(c)
        end
    end
    simExtCustomUI_setEditValue(ui,62,string.format("%.0f",c['conveyorDist']/0.001),true)
end

function sizeScalingClick_callback(ui,id,newVal)
    local c=readInfo()
    c['sizeScaling']=id-70
    modified=true
    writeInfo(c)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg1()
end

function dropAlgorithmClick_callback()

    local s="800 600"
    local p="100 100"
    if algoDlgSize then
        s=algoDlgSize[1]..' '..algoDlgSize[2]
    end
    if algoDlgPos then
        p=algoDlgPos[1]..' '..algoDlgPos[2]
    end
    local xml = [[
        <editor title="Feeder Algorithm" editable="true" searchable="true"
            tabWidth="4" textColor="50 50 50" backgroundColor="190 190 190"
            selectionColor="128 128 255" size="]]..s..[[" position="]]..p..[["
            useVrepKeywords="true" isLua="true">
            <keywords1 color="152 0 0" >
            </keywords1>
            <keywords2 color="220 80 20" >
            </keywords2>
        </editor>
    ]]

    local c=readInfo()
    local initialText=c['algorithm']
    local modifiedText
    modifiedText,algoDlgSize,algoDlgPos=simOpenTextEditor(initialText,xml)
    c['algorithm']=modifiedText
    writeInfo(c)
    modified=true
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

function getAvailableConveyors()
    local l=simGetObjectsInTree(sim_handle_scene,sim_handle_all,0)
    local retL={}
    for i=1,#l,1 do
        local data=simReadCustomDataBlock(l[i],'CONVEYOR_CONF')
        if data then
            retL[#retL+1]={simGetObjectName(l[i]),l[i]}
        end
    end
    return retL
end

function sensorComboChange_callback(ui,id,newIndex)
    local sens=comboSensor[newIndex+1][2]
    bwUtils.setReferencedObjectHandle(model,REF_SENSOR,sens)
    modified=true
end

function conveyorComboChange_callback(ui,id,newIndex)
    local conv=comboConveyor[newIndex+1][2]
    bwUtils.setReferencedObjectHandle(model,REF_CONVEYOR,conv)
    modified=true
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
            modified=true
            writeInfo(prop)
            success=true
        end
    end
    if not success then
        simMsgBox(sim_msgbox_type_warning,sim_msgbox_buttons_ok,'Input Error',"The distribution string is ill-formated.")
    end
end

function partDistribution_callback(ui,id,newVal)
    local parts=bwUtils.getAllPartsFromPartRepository()
    local tmpTxt="a) There are currently no parts available. Do you have a non-empty part repository in the scene?\n\n"
    if parts and #parts>0 then
        tmpTxt="a) Following parts are currently available in this scene:"
        local lcnt=0
        for i=1,#parts,1 do
            if lcnt~=0 then
                tmpTxt=tmpTxt..", "
            end
            if lcnt==4 then
                lcnt=0
            end
            if lcnt==0 then
                tmpTxt=tmpTxt.."\n   "
            end
            tmpTxt=tmpTxt.."'"..parts[i][1].."'"
            lcnt=lcnt+1
        end
        tmpTxt=tmpTxt.."\n\n"
    end
    tmpTxt=tmpTxt..[[
b) Usage:
   {partialProportion1,<partName>},{partialProportion2,<partName>}

c) Example:
   {2.1,'BOX1'},{1.5,'BOX2'} --> out of (2.1+1.5) parts, 2.1 will be 'BOX1', and 1.5 will be 'BOX2' (statistically)

d) You may use the variable 'counter' as in following example:
   {counter%2,'BOX1'},{(counter+1)%2,'BOX2'} --> parts alternate between 'BOX1' and 'BOX2']]
    distributionDlg('Part Distribution','partDistribution',tmpTxt)
end

function destinationDistribution_callback(ui,id,newVal)
    local destinations=bwUtils.getAllPossiblePartDestinations()
    local tmpTxt="a) There are currently no destinations available.\n\n"
    if #destinations>0 then
        tmpTxt="a) Following destinations are currently available in this scene:"
        local lcnt=0
        for i=1,#destinations,1 do
            if lcnt~=0 then
                tmpTxt=tmpTxt..", "
            end
            if lcnt==4 then
                lcnt=0
            end
            if lcnt==0 then
                tmpTxt=tmpTxt.."\n   "
            end
            tmpTxt=tmpTxt.."'"..destinations[i].."'"
            lcnt=lcnt+1
        end
        tmpTxt=tmpTxt.."\n\n"
    end
    tmpTxt=tmpTxt..[[
b) Usage:
   {partialProportion1,<destinationName>},{partialProportion2,<destinationName>}

c) Example:
   {2.1,'TRAY'},{1.5,'PLATE'} --> out of (2.1+1.5) destinations, 2.1 will be 'TRAY', and 1.5 will be 'PLATE' (statistically)

d) You may use the variable 'counter' as in following example:
   {counter%2,'TRAY'},{(counter+1)%2,'PLATE'} --> destinations alternate between 'TRAY' and 'PLATE'
   
e) You may use '<DEFAULT>', to refer to the default destination for a given part, as in following example:
   {1,'<DEFAULT>'},{1,'TRAY'} --> out of 2 destinations, one will be the default (for the part), the other one will be 'TRAY' (statistically)]]
   
    distributionDlg('Destination Distribution','destinationDistribution',tmpTxt)
end

function shiftDistribution_callback(ui,id,newVal)
    local tmpTxt=[[
a) Usage:
   {partialProportion1,<proportionalShiftVector>},{partialProportion2,<proportionalShiftVector>}, where <proportionalShiftVector> is {shiftAlongX,shiftAlongY,shiftAlongZ} (im meters)

b) Example:
   {2.1,{0,0,0}},{1.5,{1,0,0}} --> out of (2.1+1.5) items, 2.1 will be shifted by {0,0,0}*feederDimension, and 1.5 will be shifted by {1,0,0}*feederDimension (statistically)

c) You may use math expressions, as in following example:
   {1,{math.random(),math.random(),math.random()}} --> items are random 3D vectors

d) You may use the variable 'counter' as in following example:
   {counter%2,{0,0,0}},{(counter+1)%2,{1,0,0}} --> items alternate between {0,0,0} and {1,0,0}]]
    distributionDlg('Relative Shift Distribution','shiftDistribution',tmpTxt)
end

function labelDistribution_callback(ui,id,newVal)
    local tmpTxt=[[
a) Usage:
   {partialProportion1,<bitCodedValue>},{partialProportion2,<bitCodedValue>}, where <bitCodedValue>:
    1 is for the first label (usually large top label), 2 and 4 is for the 2nd and 3rd labels (usually small side labels)

b) Example:
   {2.1,0},{1.5,1+2+4} --> out of (2.1+1.5) items, 2.1 will have no labels, and 1.5 will have 3 labels (if the part is configured to have labels)

c) You may use math expressions, as in following example:
   {1,math.floor(math.random()+0.5)} --> random items will have a top label

d) You may use the variable 'counter' as in following example:
   {counter%2,0},{(counter+1)%2,1+2+4} --> items will alternatively have no labels, or 3 labels]]
    distributionDlg('Label Count Distribution','labelDistribution',tmpTxt)
end

function rotationDistribution_callback(ui,id,newVal)
    local tmpTxt=[[
a) Usage:
   {partialProportion1,<eulerAngles>},{partialProportion2,<eulerAngles>}, where <eulerAngles> is {rotationAngleAroundXAxis,rotationAngleAroundYAxis,rotationAngleAroundZAxis} (in radians)

b) Example:
   {2.1,{0,0,0}},{1.5,{0,0,math.pi/2}} --> out of (2.1+1.5) items, 2.1 will not be rotated, and 1.5 will be rotated by 90 degrees around their Z axis (statistically)

c) You may use math expressions, as in following example:
   {1,{0,0,math.random()*2*math.pi}} --> items are randomly rotated around their Z axis

d) You may use the variable 'counter' as in following example:
   {counter%2,{0,0,0}},{(counter+1)%2,{0,0,math.pi/2}} --> items alternate between no rotation, and rotation of 90 degrees around their Z axis]]
    distributionDlg('Rotation Distribution','rotationDistribution',tmpTxt)
end

function weightDistribution_callback(ui,id,newVal)
    local tmpTxt=[[
a) Usage:
   {partialProportion1,<mass>},{partialProportion2,<mass>} (mass in Kg)

b) Example:
   {2.1,0.1},{1.5,0.2} --> out of (2.1+1.5) items, 2.1 will have a mass of 0.1Kg, and 1.5 will have a mass of 0.2Kg (statistically)

c) You may use math expressions, as in following example:
   {1,0.1+0.9*math.random()} --> items have a random mass between 0.1Kg and 1Kg

d) You may use the variable 'counter' as in following example:
   {counter%2,0.1},{(counter+1)%2,0.2} --> item masses alternate between 0.1Kg and 0.2Kg
   
e) You may use '<DEFAULT>', to refer to the default value, as in following example:
   {1,'<DEFAULT>'},{1,0.1} --> out of 2 items, one will have its default mass, the other one will have a mass of 0.1Kg (statistically)]]
    distributionDlg('Weight Distribution','weightDistribution',tmpTxt)
end

function isoSizeScalingDistribution_callback(ui,id,newVal)
    local tmpTxt=[[
a) Usage:
   {partialProportion1,<scaling>},{partialProportion2,<scaling>} (valid scaling values are between 0.1 and 10)

b) Example:
   {2.1,0.5},{1.5,1} --> out of (2.1+1.5) items, 2.1 will have a size scaled down by 50%, and 1.5 will have a default size (statistically)

c) You may use math expressions, as in following example:
   {1,0.1+0.9*math.random()} --> items have a random scaling between 0.1 and 1

d) You may use the variable 'counter' as in following example:
   {counter%2,0.5},{(counter+1)%2,1} --> every other item is scaled by 50%]]
    distributionDlg('Isometric Size Scaling Distribution','isoSizeScalingDistribution',tmpTxt)
end

function nonIsoSizeScalingDistribution_callback(ui,id,newVal)
    local tmpTxt=[[
a) Not all parts can be scaled non-isometrically!

b) Usage:
   {partialProportion1,<scaling>},{partialProportion2,<scaling>}, where <scaling> is {xScaling,yScaling,zScaling} (valid values are between 0.1 and 10)

c) Example:
   {2.1,{1,1,2}},{1.5,{1,1,1}} --> out of (2.1+1.5) items, 2.1 will be scaled by a factor 2 along the Z-axis (statistically)

d) You may use math expressions, as in following example:
   {1,{1,1,0.1+0.9*math.random()}} --> items have a random Z-axis scaling between 0.1 and 1

e) You may use the variable 'counter' as in following example:
   {counter%2,{1,1,0.5}},{(counter+1)%2,{1,1,1}} --> every other item is scaled by 50% along its Z-axis]]
    distributionDlg('Non-isometric Size Scaling Distribution','nonIsoSizeScalingDistribution',tmpTxt)
end

function updateEnabledDisabledItemsDlg1()
    if ui1 then
        local enabled=simGetSimulationState()==sim_simulation_stopped
        local config=readInfo()
        local freq=simBoolAnd32(config['bitCoded'],4+8+16)==0
        local sens=simBoolAnd32(config['bitCoded'],4+8+16)==4
        local user=simBoolAnd32(config['bitCoded'],4+8+16)==8
        local conv=simBoolAnd32(config['bitCoded'],4+8+16)==12
        simExtCustomUI_setEnabled(ui1,60,enabled,true)
        simExtCustomUI_setEnabled(ui1,1,enabled,true)
        simExtCustomUI_setEnabled(ui1,61,enabled,true)
        simExtCustomUI_setEnabled(ui1,49,enabled,true)
        simExtCustomUI_setEnabled(ui1,2,enabled and freq,true)
        simExtCustomUI_setEnabled(ui1,999,enabled and sens,true)
        simExtCustomUI_setEnabled(ui1,3,enabled and user,true)
        simExtCustomUI_setEnabled(ui1,998,enabled and conv,true)
        simExtCustomUI_setEnabled(ui1,62,enabled and conv,true)
        simExtCustomUI_setEnabled(ui1,1000,enabled,true)
        simExtCustomUI_setEnabled(ui1,1001,enabled,true)
        simExtCustomUI_setEnabled(ui1,1002,enabled,true)
        simExtCustomUI_setEnabled(ui1,1003,enabled,true)
        simExtCustomUI_setEnabled(ui1,1004,enabled,true)
        simExtCustomUI_setEnabled(ui1,11,enabled,true)
        simExtCustomUI_setEnabled(ui1,12,enabled,true)
        simExtCustomUI_setEnabled(ui1,13,enabled,true)
        simExtCustomUI_setEnabled(ui1,14,enabled,true)
        simExtCustomUI_setEnabled(ui1,30,enabled,true)
      --  simExtCustomUI_setEnabled(ui1,40,enabled,true)
        simExtCustomUI_setEnabled(ui1,50,enabled,true)
        simExtCustomUI_setEnabled(ui1,17,enabled,true)
        simExtCustomUI_setEnabled(ui1,789,enabled,true)
        simExtCustomUI_setEnabled(ui1,73,config['sizeScaling']==1,true)
        simExtCustomUI_setEnabled(ui1,74,config['sizeScaling']==2,true)

        simExtCustomUI_setEnabled(ui1,54,enabled,true) -- config tab
    end
end

function setDlgItemContent()
    if ui1 then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui1)
        simExtCustomUI_setEditValue(ui1,60,string.format("%.0f",config['length']/0.001),true)
        simExtCustomUI_setEditValue(ui1,1,string.format("%.0f",config['width']/0.001),true)
        simExtCustomUI_setEditValue(ui1,61,string.format("%.0f",config['height']/0.001),true)
        simExtCustomUI_setEditValue(ui1,62,string.format("%.0f",config['conveyorDist']/0.001),true)
        simExtCustomUI_setEditValue(ui1,2,string.format("%.2f",config['frequency']),true)
        simExtCustomUI_setEditValue(ui1,789,string.format("%.1f",config['deactivationTime']),true)
        simExtCustomUI_setCheckboxValue(ui1,30,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        simExtCustomUI_setCheckboxValue(ui1,40,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],2)~=0),true)
        simExtCustomUI_setCheckboxValue(ui1,50,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],128)~=0),true)
        simExtCustomUI_setRadiobuttonValue(ui1,1000,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],4+8+16)==0),true)
        simExtCustomUI_setRadiobuttonValue(ui1,1001,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],4+8+16)==4),true)
        simExtCustomUI_setRadiobuttonValue(ui1,1002,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],4+8+16)==8),true)
        simExtCustomUI_setRadiobuttonValue(ui1,1003,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],4+8+16)==12),true)
        simExtCustomUI_setRadiobuttonValue(ui1,1004,bwUtils.getRadiobuttonValFromBool(simBoolAnd32(config['bitCoded'],4+8+16)==16),true)
        
        simExtCustomUI_setRadiobuttonValue(ui1,70,bwUtils.getRadiobuttonValFromBool(config['sizeScaling']==0),true)
        simExtCustomUI_setRadiobuttonValue(ui1,71,bwUtils.getRadiobuttonValFromBool(config['sizeScaling']==1),true)
        simExtCustomUI_setRadiobuttonValue(ui1,72,bwUtils.getRadiobuttonValFromBool(config['sizeScaling']==2),true)
        
        
        bwUtils.setSelectedEditWidget(ui1,sel)
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
end

function enabled_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-2
    end
    modified=true
    writeInfo(c)
end

function showStatisticsClick_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],128)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-128
    end
    modified=true
    writeInfo(c)
end

function triggerTypeClick_callback(ui,id)
    local c=readInfo()
    local w={4+8+16,8+16,4+16,16,4+8}
    local v=w[id-1000+1]
    c['bitCoded']=simBoolOr32(c['bitCoded'],4+8+16)-v
    modified=true
    writeInfo(c)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg1()
end

function createDlg1()
    if (not ui1) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
    <tabs id="77">
    <tab title="Properties" layout="form">
                <label text="Distribution width (mm)"/>
                <edit oneditingfinished="widthChange_callback" id="1"/>

                <label text="Distribution length (mm)"/>
                <edit oneditingfinished="lengthChange_callback" id="60"/>

                <label text="Distribution height (mm)"/>
                <edit oneditingfinished="heightChange_callback" id="61"/>

                <radiobutton text="Drop frequency (1/s)" onclick="triggerTypeClick_callback" id="1000" />
                <edit oneditingfinished="frequencyChange_callback" id="2"/>

                <radiobutton text="Sensor triggered" onclick="triggerTypeClick_callback" id="1001" />
                <combobox id="999" onchange="sensorComboChange_callback">
                </combobox>

                <radiobutton text="Conveyor belt triggered" onclick="triggerTypeClick_callback" id="1003" />
                <combobox id="998" onchange="conveyorComboChange_callback">
                </combobox>

                <label text="Distance for trigger (mm)" style="* {margin-left: 20px;}"/>
                <edit oneditingfinished="conveyorDistanceChange_callback" id="62"/>

                <radiobutton text="User defined algorithm" onclick="triggerTypeClick_callback" id="1002" />
                <button text="Edit" checked="false"  onclick="dropAlgorithmClick_callback" id="3" />

                <radiobutton text="Multi-feeder triggered" onclick="triggerTypeClick_callback" id="1004" />
                <label text=""/>

                <label text="Parts deactivation time (s)"/>
                <edit oneditingfinished="deactivationTimeChange_callback" id="789"/>

               <label text="Enabled"/>
                <checkbox text="" onchange="enabled_callback" id="40" />

                <label text="Hidden during simulation"/>
                <checkbox text="" onchange="hidden_callback" id="30" />

                <label text="Show statistics"/>
                 <checkbox text="" checked="false" onchange="showStatisticsClick_callback" id="50"/>
    </tab>
    <tab title="Configuration" layout="form" id="54">
                <label text="Part distribution"/>
                <button text="Edit" checked="false"  onclick="partDistribution_callback" id="11" />

                <label text="Destination distribution"/>
                <button text="Edit" checked="false"  onclick="destinationDistribution_callback"  id="49"/>

                <label text="Relative position distribution"/>
                <button text="Edit" checked="false"  onclick="shiftDistribution_callback" id="13" />

                <label text="Rotation distribution"/>
                <button text="Edit" checked="false"  onclick="rotationDistribution_callback" id="14" />

                <label text="Weight distribution"/>
                <button text="Edit" checked="false"  onclick="weightDistribution_callback" id="12" />

                <label text="Label distribution"/>
                <button text="Edit" checked="false"  onclick="labelDistribution_callback" id="17" />

                <radiobutton text="No size scaling" onclick="sizeScalingClick_callback" id="70" />
                <label text=""/>

                <radiobutton text="Isometric size scaling distribution" onclick="sizeScalingClick_callback" id="71" />
                <button text="Edit" checked="false"  onclick="isoSizeScalingDistribution_callback" id="73" />

                <radiobutton text="Non-isometric size scaling distribution" onclick="sizeScalingClick_callback" id="72" />
                <button text="Edit" checked="false"  onclick="nonIsoSizeScalingDistribution_callback" id="74" />

                <label text="" style="* {margin-left: 190px;}"/>
                <label text="" style="* {margin-left: 190px;}"/>
    </tab>
    </tabs>
        ]]

        ui1=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlg1Pos--[[,closeable,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])

        local c=readInfo()
        local loc=getAvailableConveyors()
        comboConveyor=customUi_populateCombobox(ui1,998,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_CONVEYOR)),true,{{'<NONE>',-1}})
        loc=getAvailableSensors()
        comboSensor=customUi_populateCombobox(ui1,999,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_SENSOR)),true,{{'<NONE>',-1}})
        
        setDlgItemContent()
        updateEnabledDisabledItemsDlg1()
        simExtCustomUI_setCurrentTab(ui1,77,dlgMainTabIndex,true)

    end
end

function showDlg1()
    if not ui1 then
        createDlg1()
    end
end

function removeDlg1()
    if ui1 then
        local x,y=simExtCustomUI_getPosition(ui1)
        previousDlg1Pos={x,y}
        dlgMainTabIndex=simExtCustomUI_getCurrentTab(ui1,77)
        simExtCustomUI_destroy(ui1)
        ui1=nil
    end
end

if (sim_call_type==sim_customizationscriptcall_initialization) then
    REF_SENSOR=1 -- index of referenced sensor object handle
    REF_CONVEYOR=2 -- index of referenced conveyor object handle
    modified=false
    dlgMainTabIndex=0
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    -- Following for backward compatibility:
    if _info['sensor'] then
        bwUtils.setReferencedObjectHandle(model,REF_SENSOR,getObjectHandle_noErrorNoSuffixAdjustment(_info['sensor']))
        _info['sensor']=nil
    end
    if _info['conveyor'] then
        bwUtils.setReferencedObjectHandle(model,REF_CONVEYOR,getObjectHandle_noErrorNoSuffixAdjustment(_info['conveyor']))
        _info['conveyor']=nil
    end
    ----------------------------------------
    writeInfo(_info)
    functionalPartHolder=simGetObjectHandle('genericFeeder_functional')
	simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,true)
    updatePluginRepresentation()
    previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos=bwUtils.readSessionPersistentObjectData(model,"dlgPosAndSize")
end

showOrHideUi1IfNeeded=function()
    local s=simGetObjectSelection()
    if s and #s>=1 and s[#s]==model then
        showDlg1()
    else
        removeDlg1()
    end
end

if (sim_call_type==sim_customizationscriptcall_nonsimulation) then
    showOrHideUi1IfNeeded()
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
        updateEnabledDisabledItemsDlg1()
    end
    simJustStarted=nil
    showOrHideUi1IfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_simulationpause) then
    showOrHideUi1IfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_firstaftersimulation) then
    updateEnabledDisabledItemsDlg1()
    simSetObjectInt32Parameter(model,sim_objintparam_visibility_layer,1)
    local conf=readInfo()
    conf['multiFeederTriggerCnt']=0
    writeInfo(conf)
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    simJustStarted=true
    local conf=readInfo()
    conf['multiFeederTriggerCnt']=0
    writeInfo(conf)
    local show=bwUtils.modifyAuxVisualizationItems(simBoolAnd32(conf['bitCoded'],1)==0)
    if not show then
        simSetObjectInt32Parameter(model,sim_objintparam_visibility_layer,0)
    end
end

if (sim_call_type==sim_customizationscriptcall_lastbeforeinstanceswitch) then
    removeDlg1()
    removeFromPluginRepresentation()
end

if (sim_call_type==sim_customizationscriptcall_firstafterinstanceswitch) then
    updatePluginRepresentation()
end

if (sim_call_type==sim_customizationscriptcall_cleanup) then
    removeDlg1()
    removeFromPluginRepresentation()
    bwUtils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos)
end