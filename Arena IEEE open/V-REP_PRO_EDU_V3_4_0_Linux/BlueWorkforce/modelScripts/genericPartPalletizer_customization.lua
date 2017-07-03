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
        info['subtype']='palletizer'
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
    if not info['bitCoded'] then
        info['bitCoded']=1+2+4 -- 1=hidden, 2=enabled, 4=showPts
    end
    if not info['palletPattern'] then
        info['palletPattern']=3 -- 0=none(forbidden here!), 1=single, 2=circular, 3=line (rectangle), 4=honeycomb, 5=custom/imported
    end
    if not info['circularPatternData3'] then
        info['circularPatternData3']={{0,0,0},0.1,6,0,true,1,0.05} -- offset, radius, count,angleOffset, center, layers, layers step
    end
    if not info['customPatternData'] then
        info['customPatternData']=''
    end
    if not info['linePatternData'] then
        info['linePatternData']={{0,0,0},3,0.03,3,0.03,1,0.05} -- offset, rowCnt, rowStep, colCnt, colStep, layers, layers step
    end
    if not info['honeycombPatternData'] then
        info['honeycombPatternData']={{0,0,0},3,0.03,3,0.03,1,0.05,false} -- offset, rowCnt, rowStep, colCnt, colStep, layers, layers step, firstRowOdd
    end
    if not info['palletPoints'] then
        info['palletPoints']={}
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_PARTPALLETIZER_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_PARTPALLETIZER_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_PARTPALLETIZER_INFO','')
    end
end

function createPalletPointsIfNeeded()
    local data=readInfo()
    if #data['palletPoints']==0 then
        data['palletPoints']=bwUtils.generatePalletPoints(data)
    end
    writeInfo(data)
end

function updatePalletPoints()
    local data=readInfo()
    if data['palletPattern']~=5 then
        data['palletPoints']={} -- remove them
        writeInfo(data)
        createPalletPointsIfNeeded()
    end
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

function setSizes()
    local c=readInfo()
    local w=c['width']
    local l=c['length']
    local h=c['height']
    setObjectSize(box,w,l,h)
    simSetObjectPosition(box,model,{0,0,h*0.5})
end

function updateEnabledDisabledItemsDlg()
    if ui then
        local enabled=simGetSimulationState()==sim_simulation_stopped
        simExtCustomUI_setEnabled(ui,20,enabled,true)
        simExtCustomUI_setEnabled(ui,21,enabled,true)
        simExtCustomUI_setEnabled(ui,22,enabled,true)
        simExtCustomUI_setEnabled(ui,3,enabled,true)
        simExtCustomUI_setEnabled(ui,4,enabled,true)
        simExtCustomUI_setEnabled(ui,5,enabled,true)
    end
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setEditValue(ui,20,string.format("%.0f",config['width']/0.001),true)
        simExtCustomUI_setEditValue(ui,21,string.format("%.0f",config['length']/0.001),true)
        simExtCustomUI_setEditValue(ui,22,string.format("%.0f",config['height']/0.001),true)
        simExtCustomUI_setCheckboxValue(ui,3,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,4,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],2)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,5,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],4)~=0),true)
        updateEnabledDisabledItemsDlg()
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function setPalletDlgItemContent()
    if palletUi then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(palletUi)
        local pattern=config['palletPattern']
        simExtCustomUI_setRadiobuttonValue(palletUi,103,bwUtils.getRadiobuttonValFromBool(pattern==2),true)
        simExtCustomUI_setRadiobuttonValue(palletUi,104,bwUtils.getRadiobuttonValFromBool(pattern==3),true)
        simExtCustomUI_setRadiobuttonValue(palletUi,105,bwUtils.getRadiobuttonValFromBool(pattern==4),true)
        simExtCustomUI_setRadiobuttonValue(palletUi,106,bwUtils.getRadiobuttonValFromBool(pattern==5),true)
        local circular=config['circularPatternData3']
        local off=circular[1]
        simExtCustomUI_setEditValue(palletUi,3004,string.format("%.0f , %.0f , %.0f",off[1]*1000,off[2]*1000,off[3]*1000),true) --offset
        simExtCustomUI_setEditValue(palletUi,3000,string.format("%.0f",circular[2]/0.001),true) -- radius
        simExtCustomUI_setEditValue(palletUi,3001,string.format("%.0f",circular[3]),true) -- count
        simExtCustomUI_setEditValue(palletUi,3002,string.format("%.0f",180*circular[4]/math.pi),true) -- angle off
        simExtCustomUI_setCheckboxValue(palletUi,3003,bwUtils.getCheckboxValFromBool(circular[5]),true) --center
        simExtCustomUI_setEditValue(palletUi,3005,string.format("%.0f",circular[6]),true) -- layers
        simExtCustomUI_setEditValue(palletUi,3006,string.format("%.0f",circular[7]/0.001),true) -- layer step

        local lin=config['linePatternData']
        off=lin[1]
        simExtCustomUI_setEditValue(palletUi,4000,string.format("%.0f , %.0f , %.0f",off[1]*1000,off[2]*1000,off[3]*1000),true) --offset
        simExtCustomUI_setEditValue(palletUi,4001,string.format("%.0f",lin[2]),true) -- rows
        simExtCustomUI_setEditValue(palletUi,4002,string.format("%.0f",lin[3]/0.001),true) -- row step
        simExtCustomUI_setEditValue(palletUi,4003,string.format("%.0f",lin[4]),true) -- cols
        simExtCustomUI_setEditValue(palletUi,4004,string.format("%.0f",lin[5]/0.001),true) -- col step
        simExtCustomUI_setEditValue(palletUi,4005,string.format("%.0f",lin[6]),true) -- layers
        simExtCustomUI_setEditValue(palletUi,4006,string.format("%.0f",lin[7]/0.001),true) -- layer step

        local honey=config['honeycombPatternData']
        off=honey[1]
        simExtCustomUI_setEditValue(palletUi,5000,string.format("%.0f , %.0f , %.0f",off[1]*1000,off[2]*1000,off[3]*1000),true) --offset
        simExtCustomUI_setEditValue(palletUi,5001,string.format("%.0f",honey[2]),true) -- rows
        simExtCustomUI_setEditValue(palletUi,5002,string.format("%.0f",honey[3]/0.001),true) -- row step
        simExtCustomUI_setEditValue(palletUi,5003,string.format("%.0f",honey[4]),true) -- cols
        simExtCustomUI_setEditValue(palletUi,5004,string.format("%.0f",honey[5]/0.001),true) -- col step
        simExtCustomUI_setEditValue(palletUi,5005,string.format("%.0f",honey[6]),true) -- layers
        simExtCustomUI_setEditValue(palletUi,5006,string.format("%.0f",honey[7]/0.001),true) -- layer step
        simExtCustomUI_setCheckboxValue(palletUi,5007,bwUtils.getCheckboxValFromBool(honey[8]),true) -- firstRowOdd

        simExtCustomUI_setEnabled(palletUi,203,(pattern==2),true)
        simExtCustomUI_setEnabled(palletUi,204,(pattern==3),true)
        simExtCustomUI_setEnabled(palletUi,205,(pattern==4),true)
        simExtCustomUI_setEnabled(palletUi,206,(pattern==5),true)
        bwUtils.setSelectedEditWidget(palletUi,sel)
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

function enabled_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-2
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
        if v<0.05 then v=0.05 end
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

function circularPattern_offsetChange_callback(ui,id,newVal)
    local c=readInfo()
    local i=1
    local t={0,0,0}
    for token in (newVal..","):gmatch("([^,]*),") do
        t[i]=tonumber(token)
        if t[i]==nil then t[i]=0 end
        t[i]=t[i]*0.001
        if t[i]>0.2 then t[i]=0.2 end
        if t[i]<-0.2 then t[i]=-0.2 end
        i=i+1
    end
    c['circularPatternData3'][1]={t[1],t[2],t[3]}
    modified=true
    writeInfo(c)
    setPalletDlgItemContent()
end

function circularPattern_radiusChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.5 then v=0.5 end
        if v~=c['circularPatternData3'][2] then
            modified=true
            c['circularPatternData3'][2]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_angleOffsetChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<-359 then v=-359 end
        if v>359 then v=359 end
        v=v*math.pi/180
        if v~=c['circularPatternData3'][4] then
            modified=true
            c['circularPatternData3'][4]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_countChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<2 then v=2 end
        if v>40 then v=40 end
        if v~=c['circularPatternData3'][3] then
            modified=true
            c['circularPatternData3'][3]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_layersChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['circularPatternData3'][6] then
            modified=true
            c['circularPatternData3'][6]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_layerStepChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['circularPatternData3'][7] then
            modified=true
            c['circularPatternData3'][7]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_centerChange_callback(ui,id,newVal)
    local c=readInfo()
    c['circularPatternData3'][5]=(newVal~=0)
    modified=true
    writeInfo(c)
    setPalletDlgItemContent()
end


function linePattern_offsetChange_callback(ui,id,newVal)
    local c=readInfo()
    local i=1
    local t={0,0,0}
    for token in (newVal..","):gmatch("([^,]*),") do
        t[i]=tonumber(token)
        if t[i]==nil then t[i]=0 end
        t[i]=t[i]*0.001
        if t[i]>0.2 then t[i]=0.2 end
        if t[i]<-0.2 then t[i]=-0.2 end
        i=i+1
    end
    c['linePatternData'][1]={t[1],t[2],t[3]}
    modified=true
    writeInfo(c)
    setPalletDlgItemContent()
end

function linePattern_rowsChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['linePatternData'][2] then
            modified=true
            c['linePatternData'][2]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_rowStepChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['linePatternData'][3] then
            modified=true
            c['linePatternData'][3]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_colsChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['linePatternData'][4] then
            modified=true
            c['linePatternData'][4]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_colStepChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['linePatternData'][5] then
            modified=true
            c['linePatternData'][5]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_layersChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['linePatternData'][6] then
            modified=true
            c['linePatternData'][6]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_layerStepChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['linePatternData'][7] then
            modified=true
            c['linePatternData'][7]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end




function honeyPattern_offsetChange_callback(ui,id,newVal)
    local c=readInfo()
    local i=1
    local t={0,0,0}
    for token in (newVal..","):gmatch("([^,]*),") do
        t[i]=tonumber(token)
        if t[i]==nil then t[i]=0 end
        t[i]=t[i]*0.001
        if t[i]>0.2 then t[i]=0.2 end
        if t[i]<-0.2 then t[i]=-0.2 end
        i=i+1
    end
    c['honeycombPatternData'][1]={t[1],t[2],t[3]}
    modified=true
    writeInfo(c)
    setPalletDlgItemContent()
end

function honeyPattern_rowsChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<2 then v=2 end
        if v>10 then v=10 end
        if v~=c['honeycombPatternData'][2] then
            modified=true
            c['honeycombPatternData'][2]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_rowStepChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['honeycombPatternData'][3] then
            modified=true
            c['honeycombPatternData'][3]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_colsChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<2 then v=2 end
        if v>10 then v=10 end
        if v~=c['honeycombPatternData'][4] then
            modified=true
            c['honeycombPatternData'][4]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_colStepChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['honeycombPatternData'][5] then
            modified=true
            c['honeycombPatternData'][5]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_layersChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['honeycombPatternData'][6] then
            modified=true
            c['honeycombPatternData'][6]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_layerStepChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['honeycombPatternData'][7] then
            modified=true
            c['honeycombPatternData'][7]=v
            writeInfo(c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_rowIsOddChange_callback(ui,id,newVal)
    local c=readInfo()
    c['honeycombPatternData'][8]=(newVal~=0)
    modified=true
    writeInfo(c)
    setPalletDlgItemContent()
end

function conveyorChange_callback(ui,id,newIndex)
    local newLoc=comboConveyor[newIndex+1][2]
    bwUtils.setReferencedObjectHandle(model,REF_CONVEYOR,newLoc)
    modified=true
end

function editPatternCode_callback(ui,id,newVal)
    local prop=readInfo()
    local s="600 100"
    local p="200 200"
    if distributionDlgSize then
        s=distributionDlgSize[1]..' '..distributionDlgSize[2]
    end
    if distributionDlgPos then
        p=distributionDlgPos[1]..' '..distributionDlgPos[2]
    end
    local xml = [[ <editor title="Pallet points" size="]]..s..[[" position="]]..p..[[" tabWidth="4" textColor="50 50 50" backgroundColor="190 190 190" selectionColor="128 128 255" useVrepKeywords="true" isLua="true"> <keywords1 color="152 0 0" > </keywords1> <keywords2 color="220 80 20" > </keywords2> </editor> ]]            
    local initialText=bwUtils.palletPointsToString(prop['palletPoints'])

    initialText=initialText.."\n\n--[[".."\n\nFormat as in following example:\n\n"..[[
{{pt1X,pt1Y,pt1Z},{pt1Alpha,pt1Beta,pt1Gamma},pt1Layer},
{{pt2X,pt2Y,pt2Z},{pt2Alpha,pt2Beta,pt2Gamma},pt2Layer}]].."\n\n--]]"

    local modifiedText
    while true do
        modifiedText,distributionDlgSize,distributionDlgPos=simOpenTextEditor(initialText,xml)
        local newPalletPoints=bwUtils.stringToPalletPoints(modifiedText)
        if newPalletPoints then
            if not bwUtils.arePalletPointsSame_posOrientAndLayer(newPalletPoints,prop['palletPoints']) then
                prop['palletPoints']=newPalletPoints
                writeInfo(prop)
                modified=true
            end
            break
        else
            if sim_msgbox_return_yes==simMsgBox(sim_msgbox_type_warning,sim_msgbox_buttons_yesno,'Input Error',"The input is not formated correctly. Do you wish to discard the changes?") then
                break
            end
            initialText=modifiedText
        end
    end
end

function importPallet_callback(ui,id,newVal)
    local file=simFileDialog(sim_filedlg_type_load,'Loading pallet items','','','pallet items','txt')
    if file then
        local newPalletPoints=bwUtils.readPalletFromFile(file)
        if newPalletPoints then
            local prop=readInfo()
            prop['palletPoints']=newPalletPoints
            writeInfo(prop)
            modified=true
        else
            simMsgBox(sim_msgbox_type_warning,sim_msgbox_buttons_ok,'File Read Error',"The specified file could not be read.")
        end
    end
end

function patternTypeClick_callback(ui,id)
    local c=readInfo()
    local changed=(c['palletPattern']~=id-101)
    c['palletPattern']=id-101
 --   if c['palletPattern']==5 and changed then
 --       c['palletPoints']={} -- clear the pallet points when we select 'imported'
 --   end
    modified=true
    writeInfo(c)
    setPalletDlgItemContent()
end

function palletCreation_callback(ui,id,newVal)
    createPalletDlg()
end

function onPalletCloseClicked()
    if palletUi then
        local x,y=simExtCustomUI_getPosition(palletUi)
        previousPalletDlgPos={x,y}
        simExtCustomUI_destroy(palletUi)
        palletUi=nil
        updatePalletPoints()
        modified=true
    end
end

function createPalletDlg()
    if not palletUi then
        local xml =[[
    <tabs id="77">
            <tab title="Circular type">
            <radiobutton text="Create a pallet with items arranged in a circular pattern" onclick="patternTypeClick_callback" id="103" />
            <group layout="form" flat="true"  id="203">
                <label text="Offset (X, Y, Z, in mm)"/>
                <edit oneditingfinished="circularPattern_offsetChange_callback" id="3004"/>

                <label text="Items on circumference"/>
                <edit oneditingfinished="circularPattern_countChange_callback" id="3001"/>

                <label text="Angle offset (deg)"/>
                <edit oneditingfinished="circularPattern_angleOffsetChange_callback" id="3002"/>

                <label text="Radius (mm)"/>
                <edit oneditingfinished="circularPattern_radiusChange_callback" id="3000"/>

                <label text="Center in use"/>
                <checkbox text="" onchange="circularPattern_centerChange_callback" id="3003" />

                <label text="Layers"/>
                <edit oneditingfinished="circularPattern_layersChange_callback" id="3005"/>

                <label text="Layer step (mm)"/>
                <edit oneditingfinished="circularPattern_layerStepChange_callback" id="3006"/>
            </group>
            </tab>

            <tab title="Line type">
            <radiobutton text="Create a pallet with items arranged in a rectangular pattern" onclick="patternTypeClick_callback" id="104" />
            <group layout="form" flat="true"  id="204">
                <label text="Offset (X, Y, Z, in mm)"/>
                <edit oneditingfinished="linePattern_offsetChange_callback" id="4000"/>

                <label text="Rows"/>
                <edit oneditingfinished="linePattern_rowsChange_callback" id="4001"/>

                <label text="Row step (mm)"/>
                <edit oneditingfinished="linePattern_rowStepChange_callback" id="4002"/>

                <label text="Columns"/>
                <edit oneditingfinished="linePattern_colsChange_callback" id="4003"/>

                <label text="Columns step (mm)"/>
                <edit oneditingfinished="linePattern_colStepChange_callback" id="4004"/>

                <label text="Layers"/>
                <edit oneditingfinished="linePattern_layersChange_callback" id="4005"/>

                <label text="Layer step (mm)"/>
                <edit oneditingfinished="linePattern_layerStepChange_callback" id="4006"/>
            </group>
            </tab>

            <tab title="Honeycomb type">
            <radiobutton text="Create a pallet with items arranged in a honeycomb pattern" onclick="patternTypeClick_callback" id="105" />
            <group layout="form" flat="true"  id="205">
                <label text="Offset (X, Y, Z, in mm)"/>
                <edit oneditingfinished="honeyPattern_offsetChange_callback" id="5000"/>

                <label text="Rows (longest)"/>
                <edit oneditingfinished="honeyPattern_rowsChange_callback" id="5001"/>

                <label text="Row step (mm)"/>
                <edit oneditingfinished="honeyPattern_rowStepChange_callback" id="5002"/>

                <label text="Columns"/>
                <edit oneditingfinished="honeyPattern_colsChange_callback" id="5003"/>

                <label text="Columns step (mm)"/>
                <edit oneditingfinished="honeyPattern_colStepChange_callback" id="5004"/>

                <label text="Layers"/>
                <edit oneditingfinished="honeyPattern_layersChange_callback" id="5005"/>

                <label text="Layer step (mm)"/>
                <edit oneditingfinished="honeyPattern_layerStepChange_callback" id="5006"/>

                <label text="1st row is odd"/>
                <checkbox text="" onchange="honeyPattern_rowIsOddChange_callback" id="5007" />
            </group>
            </tab>

            <tab title="Custom/imported">
            <radiobutton text="Create a pallet with items arranged in a customized pattern" onclick="patternTypeClick_callback" id="106" />
            <group layout="vbox" flat="true"  id="206">
                <button text="Edit pallet items" checked="false"  onclick="editPatternCode_callback"  id="6000"/>
                <button text="Import pallet items" checked="false"  onclick="importPallet_callback"  id="6001"/>
                <label text="" style="* {margin-left: 380px;}"/>
            </group>
            </tab>

        </tabs>
        ]]
        palletUi=bwUtils.createCustomUi(xml,"Pallet Creation",'center',true,'onPalletCloseClicked',true--[[,,resizable,activate,additionalUiAttribute--]])
        setPalletDlgItemContent()
        local c=readInfo()
        local pattern=c['palletPattern']
        local pat={}
        pat[2]=0
        pat[3]=1
        pat[4]=2
        pat[5]=3
        simExtCustomUI_setCurrentTab(palletUi,77,pat[pattern],true)
    end
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

                <label text="Conveyor belt"/>
                <combobox id="10" onchange="conveyorChange_callback">
                </combobox>

                <label text="Pallet creation"/>
                <button text="Adjust" checked="false"  onclick="palletCreation_callback" id="18" />

                <label text="Enabled"/>
                <checkbox text="" onchange="enabled_callback" id="4" />

                 <label text="Hidden during simulation"/>
                <checkbox text="" onchange="hidden_callback" id="3" />

                <label text="Visualize tracked items" />
                <checkbox text="" onchange="showPoints_callback" id="5" />

                <label text="" style="* {margin-left: 150px;}"/>
                <label text="" style="* {margin-left: 150px;}"/>
       ]]

        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos,false,nil,false,false,false,'layout="form"')

        local c=readInfo()
        local loc=getAvailableConveyors()
        comboConveyor=customUi_populateCombobox(ui,10,loc,{},bwUtils.getObjectNameOrNone(bwUtils.getReferencedObjectHandle(model,REF_CONVEYOR)),true,{{'<NONE>',-1}})

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
    REF_CONVEYOR=1 -- index of referenced conveyor object handle
    modified=false
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    -- Following for backward compatibility:
    if _info['conveyor'] then
        bwUtils.setReferencedObjectHandle(model,REF_CONVEYOR,getObjectHandle_noErrorNoSuffixAdjustment(_info['conveyor']))
        _info['conveyor']=nil
    end
    writeInfo(_info)
    box=simGetObjectHandle('genericPartPalletizer_box')
    simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,false)
    -- Following for backward compatibility:
    createPalletPointsIfNeeded()
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