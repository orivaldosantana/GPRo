require('utils')

local bwUtils={}

function bwUtils.modifyPartDeactivationTime(currentDeactivationTime)
    local objs=simGetObjectsInTree(sim_handle_scene,sim_handle_all)
    for i=1,#objs,1 do
        local dat=simReadCustomDataBlock(objs[i],'XYZ_OVERRIDE_INFO')
        if dat then
            dat=simUnpackTable(dat)
            if simBoolAnd32(dat['bitCoded'],4)>0 then
                return dat['deactivationTime']
            end
            break
        end
    end
    return currentDeactivationTime
end

function bwUtils.modifyAuxVisualizationItems(visualize)
    local objs=simGetObjectsInTree(sim_handle_scene,sim_handle_all)
    for i=1,#objs,1 do
        local dat=simReadCustomDataBlock(objs[i],'XYZ_OVERRIDE_INFO')
        if dat then
            dat=simUnpackTable(dat)
            local v=simBoolAnd32(dat['bitCoded'],1+2)
            if v>0 then
                if v==1 then return false end
                if v==2 then return true end
            end
            break
        end
    end
    return visualize
end

function bwUtils.canOpenPropertyDialog(modelHandle)
    local objs=simGetObjectsInTree(sim_handle_scene,sim_handle_all)
    for i=1,#objs,1 do
        local dat=simReadCustomDataBlock(objs[i],'XYZ_OVERRIDE_INFO')
        if dat then
            dat=simUnpackTable(dat)
            local v=simBoolAnd32(dat['bitCoded'],16)
            if v>0 then
  --              simAddStatusbarMessage("\nInfo: property dialog won't open, since it was disabled in the settings control center.\n")
            end
            return (v==0)
        end
    end
    return true
end

function bwUtils.createOpenBox(size,baseThickness,wallThickness,density,inertiaCorrectionFact,static,respondable,color)
    local parts={}
    local dim={size[1],size[2],baseThickness}
    parts[1]=simCreatePureShape(0,16,dim,density*dim[1]*dim[2]*dim[3])
    simSetObjectPosition(parts[1],-1,{0,0,baseThickness*0.5})
    dim={wallThickness,size[2],size[3]-baseThickness}
    parts[2]=simCreatePureShape(0,16,dim,density*dim[1]*dim[2]*dim[3])
    simSetObjectPosition(parts[2],-1,{(size[1]-wallThickness)*0.5,0,baseThickness+dim[3]*0.5})
    parts[3]=simCreatePureShape(0,16,dim,density*dim[1]*dim[2]*dim[3])
    simSetObjectPosition(parts[3],-1,{(-size[1]+wallThickness)*0.5,0,baseThickness+dim[3]*0.5})
    dim={size[1]-2*wallThickness,wallThickness,size[3]-baseThickness}
    parts[4]=simCreatePureShape(0,16,dim,density*dim[1]*dim[2]*dim[3])
    simSetObjectPosition(parts[4],-1,{0,(size[2]-wallThickness)*0.5,baseThickness+dim[3]*0.5})
    parts[5]=simCreatePureShape(0,16,dim,density*dim[1]*dim[2]*dim[3])
    simSetObjectPosition(parts[5],-1,{0,(-size[2]+wallThickness)*0.5,baseThickness+dim[3]*0.5})
    for i=1,#parts,1 do
        simSetShapeColor(parts[i],'',sim_colorcomponent_ambient_diffuse,color)
    end
    local shape=simGroupShapes(parts)
    if math.abs(1-inertiaCorrectionFact)>0.001 then
        local transf=simGetObjectMatrix(shape,-1)
        local m0,i0,com0=simGetShapeMassAndInertia(shape,transf)
        for i=1,#i0,1 do
            i0[i]=i0[1]*inertiaCorrectionFact
        end
        simSetShapeMassAndInertia(shape,m0,i0,com0,transf)
    end
    if static then
        simSetObjectInt32Parameter(shape,sim_shapeintparam_static,1)
    else
        simSetObjectInt32Parameter(shape,sim_shapeintparam_static,0)
    end
    if respondable then
        simSetObjectInt32Parameter(shape,sim_shapeintparam_respondable,1)
    else
        simSetObjectInt32Parameter(shape,sim_shapeintparam_respondable,0)
    end
    simReorientShapeBoundingBox(shape,-1)
    return shape
end

function bwUtils.getOneRawPalletItem()
    local decItem={}
    decItem['pos']={0,0,0}
    decItem['orient']={0,0,0}
    decItem['processingStage']=0
    decItem['ser']=0
    decItem['layer']=1
    return decItem
end

function bwUtils.getSinglePalletPoint(optionalGlobalOffset)
    if not optionalGlobalOffset then
        optionalGlobalOffset={0,0,0}    
    end
    local decItem=bwUtils.getOneRawPalletItem()
    decItem['pos']={optionalGlobalOffset[1],optionalGlobalOffset[2],optionalGlobalOffset[3]}
    return {decItem}
end

function bwUtils.getCircularPalletPoints(radius,count,angleOffset,center,layers,layerStep,optionalGlobalOffset)
    local retVal={}
    if not optionalGlobalOffset then
        optionalGlobalOffset={0,0,0}    
    end
    local da=2*math.pi/count
    for j=1,layers,1 do
        for i=0,count-1,1 do
            local decItem=bwUtils.getOneRawPalletItem()
            local relP={optionalGlobalOffset[1]+radius*math.cos(da*i+angleOffset),optionalGlobalOffset[2]+radius*math.sin(da*i+angleOffset),optionalGlobalOffset[3]+(j-1)*layerStep}
            decItem['pos']=relP
            decItem['ser']=#retVal
            decItem['layer']=j
            retVal[#retVal+1]=decItem
        end
        if center then -- the center point
            local decItem=bwUtils.getOneRawPalletItem()
            local relP={optionalGlobalOffset[1],optionalGlobalOffset[2],optionalGlobalOffset[3]+(j-1)*layerStep}
            decItem['pos']=relP
            decItem['ser']=#retVal
            decItem['layer']=j
            retVal[#retVal+1]=decItem
        end
    end
    return retVal
end

function bwUtils.getLinePalletPoints(rows,rowStep,cols,colStep,layers,layerStep,pointsAreCentered,optionalGlobalOffset)
    local retVal={}
    local goff={0,0,0}
    if optionalGlobalOffset then
        goff={optionalGlobalOffset[1],optionalGlobalOffset[2],optionalGlobalOffset[3]}
    end
    if pointsAreCentered then
        goff[1]=goff[1]-(rows-1)*rowStep*0.5
        goff[2]=goff[2]-(cols-1)*colStep*0.5
    end
    for k=1,layers,1 do
        for j=1,cols,1 do
            for i=1,rows,1 do
                local decItem=bwUtils.getOneRawPalletItem()
                local relP={goff[1]+(i-1)*rowStep,goff[2]+(j-1)*colStep,goff[3]+(k-1)*layerStep}
                decItem['pos']=relP
                decItem['ser']=#retVal
                decItem['layer']=k
                retVal[#retVal+1]=decItem
            end
        end
    end
    return retVal
end

function bwUtils.getHoneycombPalletPoints(rows,rowStep,cols,colStep,layers,layerStep,firstRowIsOdd,pointsAreCentered,optionalGlobalOffset)
    local retVal={}
    local goff={0,0,0}
    if optionalGlobalOffset then
        goff={optionalGlobalOffset[1],optionalGlobalOffset[2],optionalGlobalOffset[3]}
    end
    local rowSize={rows,rows-1}
    if simBoolAnd32(rows,1)==0 then
        -- max row is even
        if firstRowIsOdd then
            rowSize={rows-1,rows}
        end
    else
        -- max row is odd
        if not firstRowIsOdd then
            rowSize={rows-1,rows}
        end
    end
    local colOff=-(cols-1)*colStep*0.5
    local rowOffs={-(rowSize[1]-1)*rowStep*0.5,-(rowSize[2]-1)*rowStep*0.5}

    if not pointsAreCentered then
        goff[1]=goff[1]+(rowSize[1]-1)*rowStep*0.5
        goff[2]=goff[2]+(cols-1)*colStep*0.5
    end

    for k=1,layers,1 do
        for j=1,cols,1 do
            local r=rowSize[1+simBoolAnd32(j-1,1)]
            for i=1,r,1 do
                local rowOff=rowOffs[1+simBoolAnd32(j-1,1)]
                local decItem=bwUtils.getOneRawPalletItem()
                local relP={goff[1]+rowOff+(i-1)*rowStep,goff[2]+colOff+(j-1)*colStep,goff[3]+(k-1)*layerStep}
                decItem['pos']=relP
                decItem['ser']=#retVal
                decItem['layer']=k
                retVal[#retVal+1]=decItem
            end
        end
    end
    return retVal
end

function bwUtils.generatePalletPoints(objectData)
    local isCentered=true
    local allItems={}
    local tp=objectData['palletPattern']
    if tp and tp>0 then
        if tp==1 then -- single
            local d=objectData['singlePatternData']
            allItems=bwUtils.getSinglePalletPoint(d)
        end
        if tp==2 then -- circular
            local d=objectData['circularPatternData3']
            local off=d[1]
            local radius=d[2]
            local cnt=d[3]
            local angleOff=d[4]
            local center=d[5]
            local layers=d[6]
            local layersStep=d[7]
            allItems=bwUtils.getCircularPalletPoints(radius,cnt,angleOff,center,layers,layersStep,off)
        end
        if tp==3 then -- rectangular
            local d=objectData['linePatternData']
            local off=d[1]
            local rows=d[2]
            local rowStep=d[3]
            local cols=d[4]
            local colStep=d[5]
            local layers=d[6]
            local layersStep=d[7]
            allItems=bwUtils.getLinePalletPoints(rows,rowStep,cols,colStep,layers,layersStep,isCentered,off)
        end
        if tp==4 then -- honeycomb
            local d=objectData['honeycombPatternData']
            local off=d[1]
            local rows=d[2]
            local rowStep=d[3]
            local cols=d[4]
            local colStep=d[5]
            local layers=d[6]
            local layersStep=d[7]
            local firstRowIsOdd=d[8]
            allItems=bwUtils.getHoneycombPalletPoints(rows,rowStep,cols,colStep,layers,layersStep,firstRowIsOdd,isCentered,off)
        end
        if tp==5 then -- custom/imported
            allItems=objectData['palletPoints'] -- leave it as it is
        end
    end
    return allItems
end

function bwUtils._getPartDefaultInfoForNonExistingFields(info)
    info['labelInfo']=nil -- not used anymore
    info['weightDistribution']=nil -- not supported anymore (now part of the feeder distribution algo)
    if not info['version'] then
        info['version']=0
    end
    if not info['name'] then
        info['name']='<partName>'
    end
    if not info['destination'] then
        info['destination']='<defaultDestination>'
    end
    if not info['bitCoded'] then
        info['bitCoded']=0 -- 1=invisible, 2=non-respondable to other parts
    end
    if not info['palletPattern'] then
        info['palletPattern']=0 -- 0=none, 1=single, 2=circular, 3=line (rectangle), 4=honeycomb, 5=custom/imported
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

function bwUtils.readPartInfo(handle)
    local data=simReadCustomDataBlock(handle,'XYZ_FEEDERPART_INFO')
    if data then
        data=simUnpackTable(data)
    else
        data={}
    end
    bwUtils._getPartDefaultInfoForNonExistingFields(data)
    return data
end

function bwUtils.writePartInfo(handle,data)
    if data then
        simWriteCustomDataBlock(handle,'XYZ_FEEDERPART_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(handle,'XYZ_FEEDERPART_INFO','')
    end
end

function bwUtils.palletPointsToString(palletPoints)
    local txt=""
    for i=1,#palletPoints,1 do
        local pt=palletPoints[i]
        if i~=1 then
            txt=txt..",\n"
        end
        txt=txt.."{{"..pt['pos'][1]..","..pt['pos'][2]..","..pt['pos'][3].."},"
        txt=txt.."{"..pt['orient'][1]..","..pt['orient'][2]..","..pt['orient'][3].."},"
        txt=txt..pt['layer'].."}"
    end
    return txt
end

function bwUtils.stringToPalletPoints(txt)
    local palletPoints=nil
    local arr=stringToArray(txt)
    if arr then
        palletPoints={}
        for i=1,#arr,1 do
            local item=arr[i]
            if type(item)~='table' then return end
            if #item<3 then return end
            local pos=item[1]
            local orient=item[2]
            local layer=item[3]
            if type(pos)~='table' or #pos<3 then return end
            if type(orient)~='table' or #orient<3 then return end
            for j=1,3,1 do
                if type(pos[j])~='number' then return end
                if type(orient[j])~='number' then return end
            end
            if type(layer)~='number' then return end
            local decItem=bwUtils.getOneRawPalletItem()
            decItem['pos']=pos
            decItem['orient']=orient
            decItem['ser']=#palletPoints
            decItem['layer']=layer
            palletPoints[#palletPoints+1]=decItem
        end
    end
    return palletPoints
end

function bwUtils.arePalletPointsSame_posOrientAndLayer(pall1,pall2)
    if #pall1~=pall2 then return false end
    local distToll=0.0001*0.0001
    local orToll=0.05*math.pi/180
    orToll=orToll*orToll
    for i=1,#pall1,1 do
        local p1=pall1[i]
        local p2=pall2[i]
        if p1['layer']~=p2['layer'] then return false end
        local pos1=p1['pos']
        local pos2=p2['pos']
        local dx={pos1[1]-pos2[1],pos1[2]-pos2[2],pos1[3]-pos2[3]}
        local ll=dx[1]*dx[1]+dx[2]*dx[2]+dx[3]*dx[3]
        if ll>distToll then return false end
        pos1=p1['orient']
        pos2=p2['orient']
        dx={pos1[1]-pos2[1],pos1[2]-pos2[2],pos1[3]-pos2[3]}
        ll=dx[1]*dx[1]+dx[2]*dx[2]+dx[3]*dx[3]
        if ll>orToll then return false end
    end
    return true
end

function bwUtils.readPalletFromFile(file)
    local json=require('dkjson')
    local file="d:/v_rep/qrelease/release/palletTest.txt"
    local f = io.open(file,"rb")
    local retVal=nil
    if f then
        f:close()
        local jsonData=''
        for line in io.lines(file) do
            jsonData=jsonData..line
        end
        jsonData='{ '..jsonData..' }'
        local obj,pos,err=json.decode(jsonData,1,nil)
        if type(obj)=='table' then
            if type(obj.frames)=='table'  then
                for j=1,#obj.frames,1 do
                    local fr=obj.frames[j]
                    if fr.rawPallet then
                        local palletItemList=fr.rawPallet.palletItemList
                        if palletItemList then
                            retVal={}
                            for itmN=1,#palletItemList,1 do
                                local item=palletItemList[itmN]
                                local decItem=bwUtils.getOneRawPalletItem()
                                decItem['pos']={item.location.x*0.001,item.location.y*0.001,item.location.z*0.001}
                                decItem['orient']={item.roll,item.pitch,item.yaw}
                                decItem['ser']=#retVal
                                retVal[#retVal+1]=decItem
                            end
                            break
                        end
                    end
                end
            end
        end
    end
    return retVal
end

function bwUtils.getPartRepositoryHandles()
    local repoP=getObjectsWithTag("XYZ_PARTREPOSITORY_INFO",true)
    if #repoP==1 then
        local repo=repoP[1]
        local suff=simGetNameSuffix(simGetObjectName(repo))
        local nm='partRepository_modelParts'
        if suff>=0 then
            nm=nm..'#'..suff
        end
        local partHolder=getObjectHandle_noErrorNoSuffixAdjustment(nm)
        if partHolder>=0 then
            return repo,partHolder
        end
    end
end

function bwUtils.getAllPartsFromPartRepository()
    local repo,partHolder=bwUtils.getPartRepositoryHandles()
    if repo then
        local retVal={}
        local l=simGetObjectsInTree(partHolder,sim_handle_all,1+2)
        for i=1,#l,1 do
            local data=simReadCustomDataBlock(l[i],'XYZ_FEEDERPART_INFO')
            if data then
                data=simUnpackTable(data)
                retVal[#retVal+1]={data['name'],l[i]}
            end
        end
        return retVal
    end
end

function bwUtils.removeTmpRem(txt)
    while true do
        local s=string.find(txt,"--%[%[tmpRem")
        if not s then break end
        local e=string.find(txt,"--%]%]",s+1)
        if not e then break end
        local tmp=''
        if s>1 then
            tmp=string.sub(txt,1,s-1)
        end
        tmp=tmp..string.sub(txt,e+4)
        txt=tmp
    end
    return txt
end

function bwUtils.getAllPossiblePartDestinations()
    local allDestinations={}
    -- First the parts from the part repository:
    local lst=bwUtils.getAllPartsFromPartRepository()
    if lst then
        for i=1,#lst,1 do
            allDestinations[#allDestinations+1]=lst[i][1]
        end
    end
    -- The pingpong packer destination:
    local lst=getObjectsWithTag('CONVEYOR_CONF',true)
    for i=1,#lst,1 do
        local data=simReadCustomDataBlock(lst[i],'CONVEYOR_CONF')
        data=simUnpackTable(data)
        if data['locationName'] then
            allDestinations[#allDestinations+1]=data['locationName']
        end
    end
    -- The thermoformer destination:
    for i=1,#lst,1 do
        local data=simReadCustomDataBlock(lst[i],'CONVEYOR_CONF')
        data=simUnpackTable(data)
        if data['partName'] then
            allDestinations[#allDestinations+1]=data['partName']
        end
    end
    -- The location destination
    local lst=getObjectsWithTag('XYZ_LOCATION_INFO',true)
    for i=1,#lst,1 do
        local data=simReadCustomDataBlock(lst[i],'XYZ_LOCATION_INFO')
        data=simUnpackTable(data)
        if data['name'] then
            allDestinations[#allDestinations+1]=data['name']
        end
    end
    return allDestinations
end

function bwUtils.isObjectPartAndInstanciated(h)
    local data=simReadCustomDataBlock(h,'XYZ_FEEDERPART_INFO')
    if data then
        data=simUnpackTable(data)
        return true, data['instanciated'], data
    end
    return false, false, nil
end

function bwUtils.checkIfCodeAndModelMatch(modelHandle,codeVersion,modelVersion)
    if codeVersion~=modelVersion then
        simMsgBox(sim_msgbox_type_warning,sim_msgbox_buttons_ok,"Code and Model Version Mismatch","There is a mismatch between the code version and model version for:\n\nModel name: "..simGetObjectName(modelHandle).."\nModel version: "..modelVersion.."\nCode version: "..codeVersion)
    end
end

function bwUtils.getAllPossibleTriggerableFeeders(except)
    local allFeeders={}
    local allObjs=simGetObjectsInTree(sim_handle_scene,sim_handle_all,0)
    for i=1,#allObjs,1 do
        local h=allObjs[i]
        if h~=except then
            local data=simReadCustomDataBlock(h,'XYZ_FEEDER_INFO')
            if data then
                data=simUnpackTable(data)
                if simBoolAnd32(data['bitCoded'],4+8+16)==16 then
                    allFeeders[#allFeeders+1]={simGetObjectName(h),h}
                end
            else
                data=simReadCustomDataBlock(h,'XYZ_MULTIFEEDERTRIGGER_INFO')
                if data then
                    data=simUnpackTable(data)
                    if simBoolAnd32(data['bitCoded'],4+8+16)==16 then
                        allFeeders[#allFeeders+1]={simGetObjectName(h),h}
                    end
                end
            end
        end
    end
    return allFeeders
end

function bwUtils.getReferencedObjectHandle(modelHandle,index)
    local refH=simGetReferencedHandles(modelHandle)
    if refH and #refH>=index then
        return refH[index]
    end
    return -1
end

function bwUtils.setReferencedObjectHandle(modelHandle,index,referencedObjectHandle)
    local refH=simGetReferencedHandles(modelHandle)
    if not refH then
        refH={}
    end
    while #refH<index do
        refH[#refH+1]=-1
    end
    refH[index]=referencedObjectHandle
    simSetReferencedHandles(modelHandle,refH)
end

function bwUtils.getObjectNameOrNone(objectHandle)
    if objectHandle>=0 then
        return simGetObjectName(objectHandle)
    end
    return '<NONE>'
end

function bwUtils.createCustomUi(nakedXml,title,dlgPos,closeable,onCloseFunction,modal,resizable,activate,additionalAttributes,dlgSize)
    -- Call utils function instead once version is stable
    local xml='<ui title="'..title..'" closeable="'
    if closeable then
        if onCloseFunction and onCloseFunction~='' then
            xml=xml..'true" onclose="'..onCloseFunction..'"'
        else
            xml=xml..'true"'
        end
    else
        xml=xml..'false"'
    end
    if modal then
        xml=xml..' modal="true"'
    else
        xml=xml..' modal="false"'
    end
    if resizable then
        xml=xml..' resizable="true"'
    else
        xml=xml..' resizable="false"'
    end
    if activate then
        xml=xml..' activate="true"'
    else
        xml=xml..' activate="false"'
    end
    if additionalAttributes and additionalAttributes~='' then
        xml=xml..' '..additionalAttributes
    end
    if dlgSize then
        xml=xml..' size="'..dlgSize[1]..','..dlgSize[2]..'"'
    end
    if not dlgPos then
        xml=xml..' placement="relative" position="-50,50">'
    else
        if type(dlgPos)=='string' then
            if dlgPos=='center' then
                xml=xml..' placement="center">'
            end
            if dlgPos=='bottomRight' then
                xml=xml..' placement="relative" position="-50,-50">'
            end
            if dlgPos=='bottomLeft' then
                xml=xml..' placement="relative" position="50,-50">'
            end
            if dlgPos=='topLeft' then
                xml=xml..' placement="relative" position="50,50">'
            end
            if dlgPos=='topRight' then
                xml=xml..' placement="relative" position="-50,50">'
            end
        else
            xml=xml..' placement="absolute" position="'..dlgPos[1]..','..dlgPos[2]..'">'
        end
    end
    xml=xml..nakedXml..'</ui>'
    local ui=simExtCustomUI_create(xml)
    --[[
    if dlgSize then
        simExtCustomUI_setSize(ui,dlgSize[1],dlgSize[2])
    end
    --]]
    if not activate then
        if 2==simGetInt32Parameter(sim_intparam_platform) then
            -- To fix a Qt bug on Linux
            simAuxFunc('activateMainWindow')
        end
    end
    return ui
end

function bwUtils.getSelectedEditWidget(ui)
    -- Call utils function instead once version is stable
    local ret=-1
    if simGetInt32Parameter(sim_intparam_program_version)>30302 then
        ret=simExtCustomUI_getCurrentEditWidget(ui)
    end
    return ret
end

function bwUtils.setSelectedEditWidget(ui,id)
    -- Call utils function instead once version is stable
    if id>=0 then
        simExtCustomUI_setCurrentEditWidget(ui,id)
    end
end

function bwUtils.getRadiobuttonValFromBool(b)
    -- Call utils function instead once version is stable
    if b then
        return 1
    end
    return 0
end

function bwUtils.getCheckboxValFromBool(b)
    -- Call utils function instead once version is stable
    if b then
        return 2
    end
    return 0
end


function bwUtils.writeSessionPersistentObjectData(objectHandle,dataName,...)
    -- Call utils function instead once version is stable
    local data={...}
    local nm="___"..simGetScriptHandle()..simGetObjectName(objectHandle)..simGetInt32Parameter(sim_intparam_scene_unique_id)..simGetObjectStringParameter(objectHandle,sim_objstringparam_dna)..dataName
    data=simPackTable(data)
    simWriteCustomDataBlock(sim_handle_app,nm,data)
end

function bwUtils.readSessionPersistentObjectData(objectHandle,dataName)
    -- Call utils function instead once version is stable
    local nm="___"..simGetScriptHandle()..simGetObjectName(objectHandle)..simGetInt32Parameter(sim_intparam_scene_unique_id)..simGetObjectStringParameter(objectHandle,sim_objstringparam_dna)..dataName
    local data=simReadCustomDataBlock(sim_handle_app,nm)
    if data then
        data=simUnpackTable(data)
        return unpack(data)
    else
        return nil
    end
end

return bwUtils