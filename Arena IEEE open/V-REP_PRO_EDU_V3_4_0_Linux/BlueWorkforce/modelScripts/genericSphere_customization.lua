local bwUtils=require('bwUtils')

function removeFromPluginRepresentation()

end

function updatePluginRepresentation()

end

function setSphereSize(h,diameter)
    local r,mmin=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_min_x)
    local r,mmax=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_max_x)
    local sx=mmax-mmin
    local r,mmin=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_min_y)
    local r,mmax=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_max_y)
    local sy=mmax-mmin
    local r,mmin=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_min_z)
    local r,mmax=simGetObjectFloatParameter(h,sim_objfloatparam_objbbox_max_z)
    local sz=mmax-mmin
    simScaleObject(h,diameter/sx,diameter/sy,diameter/sz)
end

function setSphereMassAndInertia(h,diameter,mass,inertiaFact)
    local inertiaFact=1
    local transf=simGetObjectMatrix(h,-1)
    local I=2*mass*inertiaFact*(diameter*0.5)*(diameter*0.5)/5
    local inertia={I,0,0,0,I,0,0,0,I}
    simSetShapeMassAndInertia(h,mass,inertia,{0,0,0},transf)
end

function getDefaultInfoForNonExistingFields(info)
    if not info['version'] then
        info['version']=_MODELVERSION_
    end
    if not info['subtype'] then
        info['subtype']='sphere'
    end
    if not info['diameter'] then
        info['diameter']=0.1
    end
    if not info['count'] then
        info['count']=1
    end
    if not info['offset'] then
        info['offset']=1
    end
    if not info['bitCoded'] then
        info['bitCoded']=0 -- all free
    end
    if not info['mass'] then
        info['mass']=0.5
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_SPHERE_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_SPHERE_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_SPHERE_INFO','')
    end
end

function readPartInfo()
    local data=bwUtils.readPartInfo(model)
    return data
end

function writePartInfo(data)
    return bwUtils.writePartInfo(model,data)
end

function setColor(red,green,blue,spec)
    simSetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
    for i=1,3,1 do
        simSetShapeColor(auxSpheres[i],nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
    end
end

function getColor()
    local r,rgb=simGetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse)
    return rgb[1],rgb[2],rgb[3]
end

function updateModel()
    local c=readInfo()
    local d=c['diameter']
    local count=c['count']
    local offset=c['offset']*d
    local mass=c['mass']
    setSphereSize(model,d)
    setSphereMassAndInertia(model,d,mass/count,2)
    for i=1,3,1 do
        setSphereSize(auxSpheres[i],d)
        setSphereMassAndInertia(auxSpheres[i],d,mass/count,2)
        simSetObjectPosition(auxSpheres[i],model,{0,0,0})
    end
    if count>=2 then
        simSetObjectPosition(auxSpheres[1],model,{offset,0,0})
    end
    if count>=3 then
        simSetObjectPosition(auxSpheres[2],model,{offset*0.5,0.866*offset,0})
    end
    if count==4 then
        simSetObjectPosition(auxSpheres[3],model,{offset*0.5,0.288*offset,0.81*offset})
    end
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setEditValue(ui,1,string.format("%.0f",config['diameter']/0.001),true)
        simExtCustomUI_setSliderValue(ui,3,config['count'],true)
        simExtCustomUI_setEditValue(ui,2,string.format("%.2f",config['offset']),true)
        simExtCustomUI_setEditValue(ui,20,string.format("%.2f",config['mass']),true)
        local red,green,blue=getColor()
        simExtCustomUI_setSliderValue(ui,30,red*100,true)
        simExtCustomUI_setSliderValue(ui,31,green*100,true)
        simExtCustomUI_setSliderValue(ui,32,blue*100,true)
        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function diameterChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.01 then v=0.01 end
        if v>2 then v=2 end
        if v~=c['diameter'] then
            modified=true
            c['diameter']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end


function countChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=math.floor(v)
        if v<1 then v=1 end
        if v>4 then v=4 end
        if v~=c['count'] then
            modified=true
            c['count']=v
            if v>1 and c['offset']==0 then
                c['offset']=0.1
            end
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function offsetChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0 then v=0 end
        if v>1 then v=1 end
        if v~=c['offset'] then
            modified=true
            c['offset']=v
            if v==0 and c['count']>1 then
                c['count']=1
            end
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function massChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0.01 then v=0.01 end
        if v>10 then v=10 end
        if v~=c['mass'] then
            modified=true
            c['mass']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function redChange(ui,id,newVal)
    modified=true
    local r,g,b=getColor()
    setColor(newVal/100,g,b)
end

function greenChange(ui,id,newVal)
    modified=true
    local r,g,b=getColor()
    setColor(r,newVal/100,b)
end

function blueChange(ui,id,newVal)
    modified=true
    local r,g,b=getColor()
    setColor(r,g,newVal/100)
end


function onCloseClicked()
    if sim_msgbox_return_yes==simMsgBox(sim_msgbox_type_question,sim_msgbox_buttons_yesno,'Finalizing the sphere',"By closing this customization dialog you won't be able to customize the sphere anymore. Do you want to proceed?") then
        finalizeModel=true
        simRemoveScript(sim_handle_self)
    end
end

function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
        <tabs id="77">
            <tab title="General" layout="form">
                <label text="Diameter (mm)"/>
                <edit oneditingfinished="diameterChange_callback" id="1"/>

                <label text="Mass (Kg)"/>
                <edit oneditingfinished="massChange_callback" id="20"/>


                <label text="" style="* {margin-left: 150px;}"/>
                <label text="" style="* {margin-left: 150px;}"/>
            </tab>
            <tab title="Roundness" layout="form">
                <label text="multi-sphere count"/>
                <hslider minimum="1" maximum="4" onchange="countChange_callback" id="3"/>

                <label text="offset (in % of diameter)"/>
                <edit oneditingfinished="offsetChange_callback" id="2"/>
            </tab>
            <tab title="Colors" layout="form">
                    <label text="Red"/>
                    <hslider minimum="0" maximum="100" onchange="redChange" id="30"/>
                    <label text="Green"/>
                    <hslider minimum="0" maximum="100" onchange="greenChange" id="31"/>
                    <label text="Blue"/>
                    <hslider minimum="0" maximum="100" onchange="blueChange" id="32"/>
            </tab>
        </tabs>
        ]]

        ui=bwUtils.createCustomUi(xml,simGetObjectName(model),previousDlgPos,true,'onCloseClicked'--[[,modal,resizable,activate,additionalUiAttribute--]])

        setDlgItemContent()
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
    modified=false
    dlgMainTabIndex=0
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    _MODELVERSION_=0
    _CODEVERSION_=0
    local _info=readInfo()
    bwUtils.checkIfCodeAndModelMatch(model,_CODEVERSION_,_info['version'])
    writeInfo(_info)
    auxSpheres={}
    for i=1,3,1 do
        auxSpheres[i]=simGetObjectHandle('genericSphere_auxSphere'..i)
    end
    local data=readPartInfo()
    if data['name']=='<partName>' then
        data['name']='SPHERE'
    end
    writePartInfo(data)
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
    showOrHideUiIfNeeded()
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
    local repo,modelHolder=bwUtils.getPartRepositoryHandles()
    if (repo and (simGetObjectParent(model)==modelHolder)) or finalizeModel then
        -- This means the sphere is part of the part repository or that we want to finalize the model (i.e. won't be customizable anymore)
        local c=readInfo()
        simWriteCustomDataBlock(model,'XYZ_SPHERE_INFO','')
        local fs=simGetObjectsInTree(model,sim_object_forcesensor_type,1+2)
        for i=1,#fs,1 do
            simRemoveObject(fs[i])
        end
        if c['count']<4 then
            simRemoveObject(auxSpheres[3])
        end
        if c['count']<3 then
            simRemoveObject(auxSpheres[2])
        end
        if c['count']<2 then
            simRemoveObject(auxSpheres[1])
        else
            local dummy=simCreateDummy(0.01)
            simSetObjectOrientation(dummy,model,{0,0,0})
            local oss=simGetObjectsInTree(model,sim_object_shape_type,1)
            oss[#oss+1]=model
            local r=simGroupShapes(oss)
            simReorientShapeBoundingBox(r,dummy)
            simRemoveObject(dummy)
        end
    end
    bwUtils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos)
end
