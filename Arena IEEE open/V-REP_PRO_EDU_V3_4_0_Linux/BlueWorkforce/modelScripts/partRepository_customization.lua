local bwUtils=require('bwUtils')

function removeFromPluginRepresentation()

end

function updatePluginRepresentation()

end

function createPalletPointsIfNeeded(objectHandle)
    local data=readPartInfo(objectHandle)
    if #data['palletPoints']==0 then
        data['palletPoints']=bwUtils.generatePalletPoints(data)
    end
    writePartInfo(objectHandle,data)
end

function updatePalletPoints(objectHandle)
    local data=readPartInfo(objectHandle)
    if data['palletPattern']~=5 then
        data['palletPoints']={} -- remove them
        writePartInfo(objectHandle,data)
        createPalletPointsIfNeeded(objectHandle)
    end
end

function getDefaultInfoForNonExistingFields(info)
    if not info['version'] then
        info['version']=_MODELVERSION_
    end
    if not info['subtype'] then
        info['subtype']='repository'
    end
    if not info['bitCoded'] then
        info['bitCoded']=0 -- all free for now
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_PARTREPOSITORY_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_PARTREPOSITORY_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_PARTREPOSITORY_INFO','')
    end
end

function readPartInfo(handle)
    local data=bwUtils.readPartInfo(handle)

    -- Additional fields here:
--    if not data['palletPoints'] then
--        data['palletPoints']={}
--    end

    return data
end

function writePartInfo(handle,data)
    return bwUtils.writePartInfo(handle,data)
end

function getPartTable()
    local l=simGetObjectsInTree(originalPartHolder,sim_handle_all,1+2)
    local retL={}
    for i=1,#l,1 do
        local data=simReadCustomDataBlock(l[i],'XYZ_FEEDERPART_INFO')
        if data then
            data=simUnpackTable(data)
            retL[#retL+1]={data['name']..'   ('..simGetObjectName(l[i])..')',l[i]}
        end
    end
    return retL
end

function displayPartProperties()
    if #parts>0 then
        local h=parts[partIndex+1][2]
        local prop=readPartInfo(h)
        simExtCustomUI_setEditValue(ui1,5,prop['name'],true)
        simExtCustomUI_setEditValue(ui1,6,prop['destination'],true)
        simExtCustomUI_setCheckboxValue(ui1,41,bwUtils.getCheckboxValFromBool(simBoolAnd32(prop['bitCoded'],1)~=0),true)
        simExtCustomUI_setCheckboxValue(ui1,42,bwUtils.getCheckboxValFromBool(simBoolAnd32(prop['bitCoded'],2)~=0),true)
    end
end

function comboboxChange_callback(ui,id,newIndex)
    partIndex=newIndex
    displayPartProperties()
end

function getSpacelessString(str)
    return string.gsub(str," ","_")
end

function partName_callback(ui,id,newVal)
    if #parts>0 then
        local h=parts[partIndex+1][2]
        local prop=readPartInfo(h)
        newVal=getSpacelessString(newVal)
        if prop['name']~=newVal and #newVal>0 then
            local allNames=getAllPartNameMap()
            if allNames[newVal] then
                simMsgBox(sim_msgbox_type_warning,sim_msgbox_buttons_ok,'Duplicate naming',"A part named '"..newVal.."' already exists.")
            else
                prop['name']=newVal
                modified=true
                writePartInfo(h,prop)
                local partTable=getPartTable()
                parts,partIndex=customUi_populateCombobox(ui1,4,partTable,nil,newVal..'   ('..simGetObjectName(h)..')',true,nil)
            end
        end
        displayPartProperties()
    end
end

function defaultDestination_callback(ui,id,newVal)
    if #parts>0 then
        local h=parts[partIndex+1][2]
        local prop=readPartInfo(h)
        newVal=getSpacelessString(newVal)
        if  #newVal>0 then
            prop['destination']=newVal
            writePartInfo(h,prop)
            modified=true
        end
        displayPartProperties()
    end
end

function updateEnabledDisabledItemsDlg1()
    if ui1 then
        local enabled=simGetSimulationState()==sim_simulation_stopped
        local config=readInfo()

        if #parts<=0 then
            enabled=false
        end

        simExtCustomUI_setEnabled(ui1,4,enabled,true)
        simExtCustomUI_setEnabled(ui1,5,enabled,true)
        simExtCustomUI_setEnabled(ui1,6,enabled,true)
        simExtCustomUI_setEnabled(ui1,41,enabled,true)
        simExtCustomUI_setEnabled(ui1,42,enabled,true)
        simExtCustomUI_setEnabled(ui1,18,enabled,true)
        simExtCustomUI_setEnabled(ui1,53,enabled,true)
        simExtCustomUI_setEnabled(ui1,56,enabled,true)

    end
end

function invisiblePart_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    c['bitCoded']=simBoolOr32(c['bitCoded'],1)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-1
    end
    modified=true
    writePartInfo(h,c)
end

function invisibleToOtherParts_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-2
    end
    modified=true
    writePartInfo(h,c)
end

function setPalletDlgItemContent()
    if palletUi then
		local h=parts[partIndex+1][2]
        local config=readPartInfo(h)
        local sel=bwUtils.getSelectedEditWidget(palletUi)
        local pattern=config['palletPattern']
        simExtCustomUI_setRadiobuttonValue(palletUi,101,bwUtils.getRadiobuttonValFromBool(pattern==0),true)
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

        simExtCustomUI_setEnabled(palletUi,201,(pattern==0),true)
        simExtCustomUI_setEnabled(palletUi,203,(pattern==2),true)
        simExtCustomUI_setEnabled(palletUi,204,(pattern==3),true)
        simExtCustomUI_setEnabled(palletUi,205,(pattern==4),true)
        simExtCustomUI_setEnabled(palletUi,206,(pattern==5),true)
        bwUtils.setSelectedEditWidget(palletUi,sel)
    end
end

function removePart_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local p=simGetModelProperty(h)
    if simBoolAnd32(p,sim_modelproperty_not_model)>0 then
        simRemoveObject(h)
    else
        simRemoveModel(h)
    end
    modified=true
    partIndex=-1
    removeDlg1() -- triggers a refresh
end

function onVisualizeCloseClicked()
    if visualizeData then
        local x,y=simExtCustomUI_getPosition(visualizeData.ui)
        previousVisualizeDlgPos={x,y}
        simExtCustomUI_destroy(visualizeData.ui)
        simRemoveObject(visualizeData.sensor)
        simRemoveCollection(visualizeData.collection)
        visualizeData=nil
    end
end

function updateVisualizeImage()
    if visualizeData then
    
        simSetObjectPosition(visualizeData.sensor,visualizeData.part,{0,0,0})
        simSetObjectOrientation(visualizeData.sensor,-1,{90*math.pi/180,0,0})
        local m=simGetObjectMatrix(visualizeData.sensor,-1)
        m[4]=m[4]-visualizeData.params[1]*m[3]
        m[8]=m[8]-visualizeData.params[1]*m[7]
        m[12]=m[12]-visualizeData.params[1]*m[11]
        m=simRotateAroundAxis(m,{1,0,0},simGetObjectPosition(visualizeData.part,-1),visualizeData.params[3])
        m=simRotateAroundAxis(m,{0,0,1},simGetObjectPosition(visualizeData.part,-1),visualizeData.params[2])
        simSetObjectMatrix(visualizeData.sensor,-1,m)
        simSetModelProperty(visualizeData.part,0)
        simHandleVisionSensor(visualizeData.sensor)
        simSetModelProperty(visualizeData.part,sim_modelproperty_not_visible+sim_modelproperty_not_renderable+sim_modelproperty_not_showasinsidemodel)
        local img,x,y=simGetVisionSensorCharImage(visualizeData.sensor)
        simExtCustomUI_setImageData(visualizeData.ui,1,img,x,y)
    end
end

function onVisualizeZoomInClicked()
    if visualizeData.params[1]>0.03 then 
        visualizeData.params[1]=visualizeData.params[1]-0.04
    end
end

function onVisualizeZoomOutClicked()
    if visualizeData.params[1]<1 then 
        visualizeData.params[1]=visualizeData.params[1]+0.04
    end
end

function onVisualizeRotLeftClicked()
    visualizeData.params[2]=visualizeData.params[2]-0.1745
end

function onVisualizeRotRightClicked()
    visualizeData.params[2]=visualizeData.params[2]+0.1745
end

function onVisualizeRotUpClicked()
    if visualizeData.params[3]<1.3964 then 
        visualizeData.params[3]=visualizeData.params[3]+0.1745
    end
end

function onVisualizeRotDownClicked()
    if visualizeData.params[3]>0.169 then 
        visualizeData.params[3]=visualizeData.params[3]-0.1745
    end
end

function visualizePart_callback(ui,id,newVal)
    if not visualizeData then
        local xml =[[
            <image width="512" height="512" id="1"/>
            
            <group layout="hbox" flat="true">
                <group layout="vbox" flat="true">
                    <button text="Zoom in" checked="false"  onclick="onVisualizeZoomInClicked" autoRepeat="true" autoRepeatDelay="500" autoRepeatInterval="330"/>
                    <button text="Zoom out" checked="false"  onclick="onVisualizeZoomOutClicked" autoRepeat="true" autoRepeatDelay="500" autoRepeatInterval="330" />
                </group>
                <group layout="hbox" flat="true">
                    <button text="Rotate left" checked="false"  onclick="onVisualizeRotLeftClicked" autoRepeat="true" autoRepeatDelay="500" autoRepeatInterval="330" />
                    <button text="Rotate right" checked="false"  onclick="onVisualizeRotRightClicked" autoRepeat="true" autoRepeatDelay="500" autoRepeatInterval="330" />
                </group>
                <group layout="vbox" flat="true">
                    <button text="Rotate up" checked="false"  onclick="onVisualizeRotUpClicked" autoRepeat="true" autoRepeatDelay="500" autoRepeatInterval="330" />
                    <button text="Rotate down" checked="false"  onclick="onVisualizeRotDownClicked" autoRepeat="true" autoRepeatDelay="500" autoRepeatInterval="330" />
                </group>
            </group>
            <button text="OK" checked="false"  onclick="onVisualizeCloseClicked" />
        ]]
        visualizeData={}
        visualizeData.ui=bwUtils.createCustomUi(xml,"Part and pallet visualization",previousVisualizeDlgPos and previousVisualizeDlgPos or 'center',true,'onVisualizeCloseClicked',true--[[,activate,additionalUiAttribute--]])
        visualizeData.sensor=simCreateVisionSensor(1+2+128,{512,512,0,0},{0.001,5,60*math.pi/180,0.1,0.1,0.1,0.4,0.5,0.5,0,0})
        simSetObjectInt32Parameter(visualizeData.sensor,sim_objintparam_visibility_layer ,0)
        local p=simBoolOr32(simGetObjectProperty(visualizeData.sensor),sim_objectproperty_dontshowasinsidemodel)
        simSetObjectProperty(visualizeData.sensor,p)

        local part=parts[partIndex+1][2]
        local info=readPartInfo(part)
        if simBoolAnd32(simGetModelProperty(part),sim_modelproperty_not_model)>0 then
            part=simCopyPasteObjects({part},0)
            part=part[1]
            simSetObjectInt32Parameter(part,sim_objintparam_visibility_layer,1)
            simSetObjectSpecialProperty(part,sim_objectspecialproperty_renderable+sim_objectspecialproperty_detectable_all)
            local prop=simBoolOr32(simGetObjectProperty(part),sim_objectproperty_dontshowasinsidemodel)-sim_objectproperty_dontshowasinsidemodel
            simSetObjectProperty(part,prop)
            simSetModelProperty(part,sim_modelproperty_not_visible+sim_modelproperty_not_renderable+sim_modelproperty_not_showasinsidemodel) -- makes it a model
        else
            part=simCopyPasteObjects({part},1)
            part=part[1]
            simSetModelProperty(part,sim_modelproperty_not_visible+sim_modelproperty_not_renderable+sim_modelproperty_not_showasinsidemodel)
        end
        visualizeData.part=part
        visualizeData.collection=simCreateCollection('',0)
        simAddObjectToCollection(visualizeData.collection,part,sim_handle_tree,0)
        simSetObjectInt32Parameter(visualizeData.sensor,sim_visionintparam_entity_to_render,visualizeData.collection)
        simSetObjectParent(visualizeData.part,functionalPartHolder,true)
        simSetObjectParent(visualizeData.sensor,visualizeData.part,true)
        visualizeData.params={0.35,math.pi/4,math.pi/4}
        
        if #info['palletPoints']>0 then
            -- Check the detection point (for the z-position of the pallet):
            local res,bbMax=simGetObjectFloatParameter(visualizeData.part,sim_objfloatparam_modelbbox_max_z)
            simSetObjectPosition(proxSensor,visualizeData.part,{0,0,bbMax*1.001})
            simSetObjectOrientation(proxSensor,visualizeData.part,{math.pi,0,0})
            local shapes=simGetObjectsInTree(visualizeData.part,sim_object_shape_type,0)
            local zMin=1
            for i=1,#shapes,1 do
                if simBoolAnd32(simGetObjectSpecialProperty(shapes[i]),sim_objectspecialproperty_detectable_all)>0 then
                    local r,dist=simCheckProximitySensor(proxSensor,shapes[i])
                    if r>0 and dist<zMin then
                        zMin=dist
                    end
                end
            end
            
            -- Now the pallet:
            for i=1,#info['palletPoints'],1 do
                local plpt=info['palletPoints'][i]
                local h=simCreatePureShape(0,4+16,{0.01,0.01,0.01},0.1,nil)
                simSetShapeColor(h,nil,sim_colorcomponent_ambient_diffuse,{1,0,1})
                simSetObjectSpecialProperty(h,sim_objectspecialproperty_renderable)
                simSetObjectPosition(h,visualizeData.part,{plpt['pos'][1],plpt['pos'][2],plpt['pos'][3]+bbMax*1.001-zMin})
                simSetObjectParent(h,visualizeData.part,true)
            end
        end


        updateVisualizeImage()
    end
end

function palletCreation_callback(ui,id,newVal)
    createPalletDlg()
end

function circularPattern_offsetChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
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
    writePartInfo(h,c)
    setPalletDlgItemContent()
end

function circularPattern_radiusChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.5 then v=0.5 end
        if v~=c['circularPatternData3'][2] then
            modified=true
            c['circularPatternData3'][2]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_angleOffsetChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        if v<-359 then v=-359 end
        if v>359 then v=359 end
        v=v*math.pi/180
        if v~=c['circularPatternData3'][4] then
            modified=true
            c['circularPatternData3'][4]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_countChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<2 then v=2 end
        if v>40 then v=40 end
        if v~=c['circularPatternData3'][3] then
            modified=true
            c['circularPatternData3'][3]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_layersChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['circularPatternData3'][6] then
            modified=true
            c['circularPatternData3'][6]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_layerStepChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['circularPatternData3'][7] then
            modified=true
            c['circularPatternData3'][7]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function circularPattern_centerChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    c['circularPatternData3'][5]=(newVal~=0)
    modified=true
    writePartInfo(h,c)
    setPalletDlgItemContent()
end


function linePattern_offsetChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
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
    writePartInfo(h,c)
    setPalletDlgItemContent()
end

function linePattern_rowsChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['linePatternData'][2] then
            modified=true
            c['linePatternData'][2]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_rowStepChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['linePatternData'][3] then
            modified=true
            c['linePatternData'][3]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_colsChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['linePatternData'][4] then
            modified=true
            c['linePatternData'][4]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_colStepChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['linePatternData'][5] then
            modified=true
            c['linePatternData'][5]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_layersChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['linePatternData'][6] then
            modified=true
            c['linePatternData'][6]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function linePattern_layerStepChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['linePatternData'][7] then
            modified=true
            c['linePatternData'][7]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end




function honeyPattern_offsetChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
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
    writePartInfo(h,c)
    setPalletDlgItemContent()
end

function honeyPattern_rowsChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<2 then v=2 end
        if v>10 then v=10 end
        if v~=c['honeycombPatternData'][2] then
            modified=true
            c['honeycombPatternData'][2]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_rowStepChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['honeycombPatternData'][3] then
            modified=true
            c['honeycombPatternData'][3]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_colsChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<2 then v=2 end
        if v>10 then v=10 end
        if v~=c['honeycombPatternData'][4] then
            modified=true
            c['honeycombPatternData'][4]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_colStepChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['honeycombPatternData'][5] then
            modified=true
            c['honeycombPatternData'][5]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_layersChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>10 then v=10 end
        if v~=c['honeycombPatternData'][6] then
            modified=true
            c['honeycombPatternData'][6]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_layerStepChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>0.2 then v=0.2 end
        if v~=c['honeycombPatternData'][7] then
            modified=true
            c['honeycombPatternData'][7]=v
            writePartInfo(h,c)
        end
    end
    setPalletDlgItemContent()
end

function honeyPattern_rowIsOddChange_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    c['honeycombPatternData'][8]=(newVal~=0)
    modified=true
    writePartInfo(h,c)
    setPalletDlgItemContent()
end

function editPatternItems_callback(ui,id,newVal)
    local h=parts[partIndex+1][2]
    local prop=readPartInfo(h)
    local s="600 400"
    local p="200 200"
    if customPalletDlgSize then
        s=customPalletDlgSize[1]..' '..customPalletDlgSize[2]
    end
    if customPalletDlgPos then
        p=customPalletDlgPos[1]..' '..customPalletDlgPos[2]
    end
    local xml = [[ <editor title="Pallet points" size="]]..s..[[" position="]]..p..[[" tabWidth="4" textColor="50 50 50" backgroundColor="190 190 190" selectionColor="128 128 255" useVrepKeywords="true" isLua="true"> <keywords1 color="152 0 0" > </keywords1> <keywords2 color="220 80 20" > </keywords2> </editor> ]]            
    local initialText=bwUtils.palletPointsToString(prop['palletPoints'])

    initialText=initialText.."\n\n--[[".."\n\nFormat as in following example:\n\n"..[[
{{pt1X,pt1Y,pt1Z},{pt1Alpha,pt1Beta,pt1Gamma},pt1Layer},
{{pt2X,pt2Y,pt2Z},{pt2Alpha,pt2Beta,pt2Gamma},pt2Layer}]].."\n\n--]]"

    local modifiedText
    while true do
        modifiedText,customPalletDlgSize,customPalletDlgPos=simOpenTextEditor(initialText,xml)
        local newPalletPoints=bwUtils.stringToPalletPoints(modifiedText)
        if newPalletPoints then
            if not bwUtils.arePalletPointsSame_posOrientAndLayer(newPalletPoints,prop['palletPoints']) then
                prop['palletPoints']=newPalletPoints
                writePartInfo(h,prop)
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
            local h=parts[partIndex+1][2]
            local prop=readPartInfo(h)
            prop['palletPoints']=newPalletPoints
            writePartInfo(h,prop)
            modified=true
        else
            simMsgBox(sim_msgbox_type_warning,sim_msgbox_buttons_ok,'File Read Error',"The specified file could not be read.")
        end
    end
end

function patternTypeClick_callback(ui,id)
    local h=parts[partIndex+1][2]
    local c=readPartInfo(h)
    local changed=(c['palletPattern']~=id-101)
    c['palletPattern']=id-101
--    if c['palletPattern']==5 and changed then
--        c['palletPoints']={} -- clear the pallet points when we select 'imported'
--    end
    modified=true
    writePartInfo(h,c)
    setPalletDlgItemContent()
end

function onPalletCloseClicked()
    if palletUi then
        local x,y=simExtCustomUI_getPosition(palletUi)
        previousPalletDlgPos={x,y}
        simExtCustomUI_destroy(palletUi)
        palletUi=nil
        local h=parts[partIndex+1][2]
        updatePalletPoints(h)
    end
end

function createPalletDlg()
    if not palletUi then
        local xml =[[
    <tabs id="77">
            <tab title="None">
            <radiobutton text="Do not create a pallet" onclick="patternTypeClick_callback" id="101" />
            <group layout="form" flat="true" id="201">
            </group>
                <label text="" style="* {margin-left: 380px;}"/>
            </tab>

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
                <button text="Edit pallet items" checked="false"  onclick="editPatternItems_callback"  id="6000"/>
                <button text="Import pallet items" checked="false"  onclick="importPallet_callback"  id="6001"/>
                <label text="" style="* {margin-left: 380px;}"/>
            </group>
            </tab>

            </tabs>
        ]]

        palletUi=bwUtils.createCustomUi(xml,"Pallet Creation",'center',true,'onPalletCloseClicked',true--[[,resizable,activate,additionalUiAttribute--]])

        setPalletDlgItemContent()
        local h=parts[partIndex+1][2]
        local c=readPartInfo(h)
        local pattern=c['palletPattern']
        local pat={}
        pat[0]=0
        pat[2]=1
        pat[3]=2
        pat[4]=3
        pat[5]=4
        simExtCustomUI_setCurrentTab(palletUi,77,pat[pattern],true)
    end
end

function createDlg1()
    if (not ui1) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
                <combobox id="4" onchange="comboboxChange_callback"> </combobox>

            <group layout="form" flat="true">

                <label text="Name"/>
                <edit oneditingfinished="partName_callback" id="5"/>

                <label text="Default destination"/>
                <edit oneditingfinished="defaultDestination_callback" id="6"/>

                <label text="Pallet creation"/>
                <button text="Adjust" checked="false"  onclick="palletCreation_callback" id="18" />

                <label text="Invisible"/>
                <checkbox text="" onchange="invisiblePart_callback" id="41" />

                <label text="Invisible to other parts"/>
                <checkbox text="" onchange="invisibleToOtherParts_callback" id="42" />
            </group>

            <group layout="hbox" flat="true">
                <button text="Visualize part and pallet" checked="false"  onclick="visualizePart_callback" id="56" />
                <button text="Remove part" checked="false"  onclick="removePart_callback" id="53" />
            </group>
        ]]


        ui1=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlg1Pos--[[,closeable,onCloseFunction,modal,resizable,activate,additionalUiAttribute--]])

        local previousItemName=nil
        if parts and #parts>0 and (partIndex>=0) then
            previousItemName=parts[partIndex+1][1]..'   ('..simGetObjectName(parts[partIndex+1][2])..')'
        end
        local partTable=getPartTable()
        
        parts,partIndex=customUi_populateCombobox(ui1,4,partTable,nil,previousItemName,true,nil)

        displayPartProperties()
        updateEnabledDisabledItemsDlg1()
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
        simExtCustomUI_destroy(ui1)
        ui1=nil
    end
end

function getPotentialNewParts()
    local p=simGetObjectsInTree(model,sim_handle_all,1+2)
    local i=1
    while i<=#p do
        if p[i]==functionalPartHolder then
            table.remove(p,i)
        else
            i=i+1
        end
    end
    return p
end

function getAllPartNameMap()
    local allNames={}
    local parts=simGetObjectsInTree(originalPartHolder,sim_handle_all,1+2)
    for i=1,#parts,1 do
        local info=readPartInfo(parts[i])
        local nm=info['name']
        allNames[nm]=parts[i]
    end
    return allNames
end

function resolveDuplicateNames()
    local allNames={}
    local parts=simGetObjectsInTree(originalPartHolder,sim_handle_all,1+2)
    for i=1,#parts,1 do
        local info=readPartInfo(parts[i])
        local nm=info['name']
        if nm=='<partName>' then
            nm=simGetObjectName(parts[i])
        end
        while allNames[nm] do
            nm=nm..'_COPY'
        end
        allNames[nm]=true
        info['name']=nm
        writePartInfo(parts[i],info)
    end
end

if (sim_call_type==sim_customizationscriptcall_initialization) then
    partToEdit=-1
    modified=false
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    writeInfo(_info)
    originalPartHolder=simGetObjectHandle('partRepository_modelParts')
    functionalPartHolder=simGetObjectHandle('partRepository_functional')
    proxSensor=simGetObjectHandle('partRepository_sensor')
	simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,false)

    
    -- Following because of a bug in V-REP V3.3.3 and before:
    local p=simBoolOr32(simGetModelProperty(originalPartHolder),sim_modelproperty_scripts_inactive)
    if simGetInt32Parameter(sim_intparam_program_version)>30303 then
        simSetModelProperty(originalPartHolder,p)
    else
        simSetModelProperty(originalPartHolder,p-sim_modelproperty_scripts_inactive)
    end
    
    -- Following for backward compatibility:
    local parts=simGetObjectsInTree(originalPartHolder,sim_handle_all,1+2)
    for i=1,#parts,1 do
        createPalletPointsIfNeeded(parts[i])
    end
    
    -- Following for backward compatibility:
    resolveDuplicateNames()

    previousPalletDlgPos,algoDlgSize,algoDlgPos,previousDlg1Pos=bwUtils.readSessionPersistentObjectData(model,"dlgPosAndSize")

    -- Allow only one part repository per scene:
    local objs=getObjectsWithTag('XYZ_PARTREPOSITORY_INFO',true)
    if #objs>1 then
        simRemoveModel(model)
        simRemoveObjectFromSelection(sim_handle_all)
        objs=getObjectsWithTag('XYZ_PARTREPOSITORY_INFO',true)
        simAddObjectToSelection(sim_handle_single,objs[1])
    else
        updatePluginRepresentation()
    end
end

showOrHideUi1IfNeeded=function()
    local s=simGetObjectSelection()
    if s and #s>=1 and s[#s]==model then
        showDlg1()
    else
        removeDlg1()
    end
end

removeAssociatedCustomizationScriptIfAvailable=function(h)
    local sh=simGetCustomizationScriptAssociatedWithObject(h)
    if sh>0 then
        simRemoveScript(sh)
    end
end

checkPotentialNewParts=function(potentialParts)
    local retVal=false -- true means update the part list in the dialog (i.e. rebuild the dialog's part combo)
    local functionType=0 -- 0=question, 1=make parts, 2=make orphans
    for modC=1,#potentialParts,1 do
        local h=potentialParts[modC]
        local data=simReadCustomDataBlock(h,'XYZ_FEEDERPART_INFO')
        if not data then
            -- This is not yet flagged as part
            modified=true
            if functionType==0 then
                local msg="Detected new children of object '"..simGetObjectName(model).."'. Objects attached to that object should be repository parts. Do you wish to turn those new objects into repository parts? If you click 'no', then those new objects will be made orphan. If you click 'yes', then those new objects will be adjusted appropriately. Only shapes or models can be turned into repository parts." 
                local ret=simMsgBox(sim_msgbox_type_question,sim_msgbox_buttons_yesno,'Part Definition',msg)
                if ret==sim_msgbox_return_yes then
                    functionType=1
                else
                    functionType=2
                end
            end
            if functionType==1 then
                -- We want to accept it as a part
                local allNames=getAllPartNameMap()
                data=readPartInfo(h)
                local nm=simGetObjectName(h)
                while true do
                    if not allNames[nm] then
                        data['name']=nm -- ok, that name doesn't exist yet!
                        break
                    end
                    nm=nm..'_COPY'
                end
                writePartInfo(h,data) -- attach the XYZ_FEEDERPART_INFO tag
                simSetObjectPosition(h,model,{0,0,0}) -- keep the orientation as it is

                if simBoolAnd32(simGetModelProperty(h),sim_modelproperty_not_model)>0 then
                    -- Shape
                    local p=simBoolOr32(simGetObjectProperty(h),sim_objectproperty_dontshowasinsidemodel)
                    simSetObjectProperty(h,p)
                else
                    -- Model
                    local p=simBoolOr32(simGetModelProperty(h),sim_modelproperty_not_showasinsidemodel)
                    simSetModelProperty(h,p)
                end
                createPalletPointsIfNeeded(h)
                removeAssociatedCustomizationScriptIfAvailable(h)
                simSetObjectParent(h,originalPartHolder,true)
                retVal=true
            end
            if functionType==2 then
                -- We reject it as a part
                simSetObjectParent(h,-1,true)
            end
        else
            -- This is already flagged as part
            data=readPartInfo(h)
            
            local allNames=getAllPartNameMap()
            local nm=data['name']
            while true do
                if not allNames[nm] then
                    data['name']=nm -- ok, that name doesn't exist yet!
                    break
                end
                nm=nm..'_COPY'
            end
            writePartInfo(h,data) -- append additional tags that were maybe missing previously
            -- just in case we are adding an item that was already tagged previously
            simSetObjectPosition(h,model,{0,0,0}) -- keep the orientation as it is
            -- Make the model static, non-respondable, non-collidable, non-measurable, non-visible, etc.
            if simBoolAnd32(simGetModelProperty(h),sim_modelproperty_not_model)>0 then
                -- Shape
                local p=simBoolOr32(simGetObjectProperty(h),sim_objectproperty_dontshowasinsidemodel)
                simSetObjectProperty(h,p)
            else
                -- Model
                local p=simBoolOr32(simGetModelProperty(h),sim_modelproperty_not_showasinsidemodel)
                simSetModelProperty(h,p)
            end
            createPalletPointsIfNeeded(h)
            removeAssociatedCustomizationScriptIfAvailable(h)
            simSetObjectParent(h,originalPartHolder,true)
            retVal=true
        end
    end
    return retVal
end

if (sim_call_type==sim_customizationscriptcall_nonsimulation) then
    showOrHideUi1IfNeeded()
    updateVisualizeImage()
    if simGetSystemTimeInMs(lastT)>3000 then
        lastT=simGetSystemTimeInMs(-1)
        local potentialNewParts=getPotentialNewParts()
        if #potentialNewParts>0 then
            if checkPotentialNewParts(potentialNewParts) then
                removeDlg1() -- we need to update the dialog with the new parts
            end
        end
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
    removeDlg1()
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
    if simIsHandleValid(model)==1 then
        -- The associated model might already have been destroyed
        bwUtils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousPalletDlgPos,algoDlgSize,algoDlgPos,previousDlg1Pos)
    end
end
