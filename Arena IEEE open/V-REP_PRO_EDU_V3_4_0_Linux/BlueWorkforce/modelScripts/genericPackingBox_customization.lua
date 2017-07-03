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

function setCuboidMassAndInertia(h,sizeX,sizeY,sizeZ,massPerVolume,inertiaFact)
    local transf=simGetObjectMatrix(h,-1)
    local mass=sizeX*sizeY*sizeZ*massPerVolume
    local inertia={(sizeY*sizeY+sizeZ*sizeZ)*mass*inertiaFact/12,0,0,0,(sizeX*sizeX+sizeZ*sizeZ)*mass*inertiaFact/12,0,0,0,(sizeY*sizeY+sizeX*sizeX)*mass*inertiaFact/12}
    simSetShapeMassAndInertia(h,mass,inertia,{0,0,0},transf)
end

function setShapeActive(h,active)
    if active then
        simSetObjectInt32Parameter(h,sim_objintparam_visibility_layer,1+256) -- make it visible
        simSetObjectSpecialProperty(h,sim_objectspecialproperty_collidable+sim_objectspecialproperty_measurable+sim_objectspecialproperty_detectable_all+sim_objectspecialproperty_renderable) -- make it collidable, measurable, detectable, etc.
        simSetObjectInt32Parameter(h,sim_shapeintparam_static,0) -- make it non-static
        simSetObjectInt32Parameter(h,sim_shapeintparam_respondable,1) -- make it respondable
        local p=simBoolOr32(simGetObjectProperty(h),sim_objectproperty_dontshowasinsidemodel)
        simSetObjectProperty(h,p-sim_objectproperty_dontshowasinsidemodel)
    else
        simSetObjectInt32Parameter(h,sim_objintparam_visibility_layer,0) -- make it invisible
        simSetObjectSpecialProperty(h,0) -- make it not collidable, measurable, detectable, etc.
        simSetObjectInt32Parameter(h,sim_shapeintparam_static,1) -- make it static
        simSetObjectInt32Parameter(h,sim_shapeintparam_respondable,0) -- make it non-respondable
        local p=simBoolOr32(simGetObjectProperty(h),sim_objectproperty_dontshowasinsidemodel)
        simSetObjectProperty(h,p)
    end
end

setMass=function(m)
    local currentMass=0
    local objects={model}
    while #objects>0 do
        handle=objects[#objects]
        table.remove(objects,#objects)
        local i=0
        while true do
            local h=simGetObjectChild(handle,i)
            if h>=0 then
                objects[#objects+1]=h
                i=i+1
            else
                break
            end
        end
        if simGetObjectType(handle)==sim_object_shape_type then
            local r,p=simGetObjectInt32Parameter(handle,sim_shapeintparam_static)
            if p==0 then
                local m0,i0,com0=simGetShapeMassAndInertia(handle)
                currentMass=currentMass+m0
            end
        end
    end

    local massScaling=m/currentMass

    local objects={model}
    while #objects>0 do
        handle=objects[#objects]
        table.remove(objects,#objects)
        local i=0
        while true do
            local h=simGetObjectChild(handle,i)
            if h>=0 then
                objects[#objects+1]=h
                i=i+1
            else
                break
            end
        end
        if simGetObjectType(handle)==sim_object_shape_type then
            local r,p=simGetObjectInt32Parameter(handle,sim_shapeintparam_static)
            if p==0 then
                local transf=simGetObjectMatrix(handle,-1)
                local m0,i0,com0=simGetShapeMassAndInertia(handle,transf)
                for i=1,9,1 do
                    i0[i]=i0[i]*massScaling
                end
                simSetShapeMassAndInertia(handle,m0*massScaling,i0,com0,transf)
            end
        end
    end
end

function getDefaultInfoForNonExistingFields(info)
    if not info['version'] then
        info['version']=_MODELVERSION_
    end
    if not info['subtype'] then
        info['subtype']='packingBox'
    end
    if not info['width'] then
        info['width']=0.3
    end
    if not info['length'] then
        info['length']=0.4
    end
    if not info['height'] then
        info['height']=0.3
    end
    if not info['thickness'] then
        info['thickness']=0.003
    end
    if not info['bitCoded'] then
        info['bitCoded']=1+2+4 -- 1=partA, 2=partB, 4=textured
    end
    if not info['closePartALength'] then
        info['closePartALength']=0.5
    end
    if not info['closePartAWidth'] then
        info['closePartAWidth']=1
    end
    if not info['closePartBLength'] then
        info['closePartBLength']=0.5
    end
    if not info['closePartBWidth'] then
        info['closePartBWidth']=0.9
    end
    if not info['mass'] then
        info['mass']=0.5
    end
    if not info['inertiaFactor'] then
        info['inertiaFactor']=1
    end
    if not info['lidTorque'] then
        info['lidTorque']=0.1
    end
    if not info['lidSpring'] then
        info['lidSpring']=1
    end
    if not info['lidDamping'] then
        info['lidDamping']=0
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'XYZ_PACKINGBOX_INFO')
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
        simWriteCustomDataBlock(model,'XYZ_PACKINGBOX_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'XYZ_PACKINGBOX_INFO','')
    end
end

function setColor(red,green,blue,spec)
    simSetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
    simSetShapeColor(model,nil,sim_colorcomponent_specular,{spec,spec,spec})
    simSetShapeColor(sides,nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
    simSetShapeColor(sides,nil,sim_colorcomponent_specular,{spec,spec,spec})
    simSetShapeColor(bb,nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
    simSetShapeColor(bb,nil,sim_colorcomponent_specular,{spec,spec,spec})
    for i=1,4,1 do
        simSetShapeColor(lids[i],nil,sim_colorcomponent_ambient_diffuse,{red,green,blue})
        simSetShapeColor(lids[i],nil,sim_colorcomponent_specular,{spec,spec,spec})
    end
end

function getColor()
    local r,rgb=simGetShapeColor(model,nil,sim_colorcomponent_ambient_diffuse)
    local r,spec=simGetShapeColor(model,nil,sim_colorcomponent_specular)
    return rgb[1],rgb[2],rgb[3],(spec[1]+spec[2]+spec[3])/3
end

function updateModel()
    local c=readInfo()
    local w=c['width']
    local l=c['length']
    local h=c['height']
    local th=c['thickness']
    local bitC=c['bitCoded']
    local h2=h-th
    local defMassPerVolume=200
    local inertiaFactor=c['inertiaFactor']
    local maxTorque=c['lidTorque']
    local springK=c['lidSpring']
    local springC=c['lidDamping']
    setObjectSize(model,w,l,th)
    setCuboidMassAndInertia(model,w,l,th,defMassPerVolume,inertiaFactor)
    simRemoveObject(sides)
    local p={}
    p[1]=simCopyPasteObjects({bb},0)[1]
    setObjectSize(p[1],th,l,h)
    setCuboidMassAndInertia(p[1],th,l,h,defMassPerVolume,inertiaFactor)
    simSetObjectPosition(p[1],model,{(w-th)/2,0,(h+th)/2})
    p[2]=simCopyPasteObjects({p[1]},0)[1]
    simSetObjectPosition(p[2],model,{(-w+th)/2,0,(h+th)/2})
    p[3]=simCopyPasteObjects({bb},0)[1]
    setObjectSize(p[3],w-th*2,th,h2)
    setCuboidMassAndInertia(p[3],w-th*2,th,h2,defMassPerVolume,inertiaFactor)
    simSetObjectPosition(p[3],model,{0,(l-th)/2,(h2+th)/2})
    p[4]=simCopyPasteObjects({p[3]},0)[1]
    simSetObjectPosition(p[4],model,{0,(-l+th)/2,(h2+th)/2})

    local textureId=simGetShapeTextureId(bb)
    for i=1,4,1 do
        if simBoolAnd32(bitC,4)>0 then
            simSetShapeTexture(p[i],textureId,sim_texturemap_cube,4+8,{0.3,0.3})
        else
            simSetShapeTexture(p[i],-1,sim_texturemap_cube,4+8,{0.3,0.3})
        end
    end


    sides=simGroupShapes(p)
    setShapeActive(sides,true)
    simSetObjectInt32Parameter(sides,sim_shapeintparam_respondable_mask,65535-1)
    simSetObjectParent(sides,sideConnection,true)
    simSetObjectPosition(joints[1],model,{w/2,0,h+th/2})
    simSetObjectPosition(joints[2],model,{-w/2,0,h+th/2})
    simSetObjectPosition(joints[3],model,{0,l/2,h2+th/2})
    simSetObjectPosition(joints[4],model,{0,-l/2,h2+th/2})
    
    for i=1,4,1 do
        simSetJointForce(joints[i],maxTorque)
        simSetObjectFloatParameter(joints[i],sim_jointfloatparam_kc_k,springK)
        simSetObjectFloatParameter(joints[i],sim_jointfloatparam_kc_c,springC)
    end

    local lidL=c['closePartALength']*w
    local lidW=c['closePartAWidth']*l
    setObjectSize(lids[1],th,lidW,lidL)
    setCuboidMassAndInertia(lids[1],th,lidW,lidL,defMassPerVolume,inertiaFactor)
    simSetObjectPosition(lids[1],joints[1],{0,lidL*0.5,0})
    setObjectSize(lids[2],th,lidW,lidL)
    setCuboidMassAndInertia(lids[2],th,lidW,lidL,defMassPerVolume,inertiaFactor)
    simSetObjectPosition(lids[2],joints[2],{0,lidL*0.5,0})

    lidL=c['closePartBLength']*l
    lidW=c['closePartBWidth']*w
    setObjectSize(lids[3],lidW,th,lidL)
    setCuboidMassAndInertia(lids[3],lidW,th,lidL,defMassPerVolume,inertiaFactor)
    simSetObjectPosition(lids[3],joints[3],{0,lidL*0.5,0})
    setObjectSize(lids[4],lidW,th,lidL)
    setCuboidMassAndInertia(lids[4],lidW,th,lidL,defMassPerVolume,inertiaFactor)
    simSetObjectPosition(lids[4],joints[4],{0,lidL*0.5,0})

    setShapeActive(lids[1],simBoolAnd32(bitC,1)>0)
    setShapeActive(lids[2],simBoolAnd32(bitC,1)>0)
    setShapeActive(lids[3],simBoolAnd32(bitC,2)>0)
    setShapeActive(lids[4],simBoolAnd32(bitC,2)>0)
    simSetObjectInt32Parameter(lids[1],sim_shapeintparam_respondable_mask,65535-254)
    simSetObjectInt32Parameter(lids[2],sim_shapeintparam_respondable_mask,65535-254)
    simSetObjectInt32Parameter(lids[3],sim_shapeintparam_respondable_mask,65535-254)
    simSetObjectInt32Parameter(lids[4],sim_shapeintparam_respondable_mask,65535-254)

    if simBoolAnd32(bitC,4)>0 then
        -- textured
        simSetShapeTexture(model,textureId,sim_texturemap_cube,4+8,{0.3,0.3})
        simSetShapeTexture(lids[1],textureId,sim_texturemap_cube,4+8,{0.3,0.3})
        simSetShapeTexture(lids[2],textureId,sim_texturemap_cube,4+8,{0.3,0.3})
        simSetShapeTexture(lids[3],textureId,sim_texturemap_cube,4+8,{0.3,0.3})
        simSetShapeTexture(lids[4],textureId,sim_texturemap_cube,4+8,{0.3,0.3})
    else
        -- without texture
        simSetShapeTexture(model,-1,sim_texturemap_cube,4+8,{0.3,0.3})
        simSetShapeTexture(lids[1],-1,sim_texturemap_cube,4+8,{0.3,0.3})
        simSetShapeTexture(lids[2],-1,sim_texturemap_cube,4+8,{0.3,0.3})
        simSetShapeTexture(lids[3],-1,sim_texturemap_cube,4+8,{0.3,0.3})
        simSetShapeTexture(lids[4],-1,sim_texturemap_cube,4+8,{0.3,0.3})
    end

    setMass(c['mass'])
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=bwUtils.getSelectedEditWidget(ui)
        simExtCustomUI_setEditValue(ui,1,string.format("%.0f",config['width']/0.001),true)
        simExtCustomUI_setEditValue(ui,2,string.format("%.0f",config['length']/0.001),true)
        simExtCustomUI_setEditValue(ui,3,string.format("%.0f",config['height']/0.001),true)
        simExtCustomUI_setEditValue(ui,4,string.format("%.0f",config['thickness']/0.001),true)

        simExtCustomUI_setCheckboxValue(ui,10,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        simExtCustomUI_setEditValue(ui,11,string.format("%.0f",config['closePartALength']*100),true)
        simExtCustomUI_setEditValue(ui,12,string.format("%.0f",config['closePartAWidth']*100),true)
        simExtCustomUI_setCheckboxValue(ui,13,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],2)~=0),true)
        simExtCustomUI_setEditValue(ui,14,string.format("%.0f",config['closePartBLength']*100),true)
        simExtCustomUI_setEditValue(ui,15,string.format("%.0f",config['closePartBWidth']*100),true)

        simExtCustomUI_setCheckboxValue(ui,888,bwUtils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],4)~=0),true)

        simExtCustomUI_setEditValue(ui,20,string.format("%.2f",config['mass']),true)
        simExtCustomUI_setEditValue(ui,21,string.format("%.2f",config['inertiaFactor']),true)
        simExtCustomUI_setEditValue(ui,22,string.format("%.2f",config['lidTorque']),true)
        simExtCustomUI_setEditValue(ui,23,string.format("%.2f",config['lidSpring']),true)
        simExtCustomUI_setEditValue(ui,24,string.format("%.2f",config['lidDamping']),true)

        local red,green,blue,spec=getColor()
        simExtCustomUI_setSliderValue(ui,30,red*100,true)
        simExtCustomUI_setSliderValue(ui,31,green*100,true)
        simExtCustomUI_setSliderValue(ui,32,blue*100,true)
        simExtCustomUI_setSliderValue(ui,33,spec*100,true)

        simExtCustomUI_setEnabled(ui,11,simBoolAnd32(config['bitCoded'],1)~=0,true)
        simExtCustomUI_setEnabled(ui,12,simBoolAnd32(config['bitCoded'],1)~=0,true)
        simExtCustomUI_setEnabled(ui,14,simBoolAnd32(config['bitCoded'],2)~=0,true)
        simExtCustomUI_setEnabled(ui,15,simBoolAnd32(config['bitCoded'],2)~=0,true)

        bwUtils.setSelectedEditWidget(ui,sel)
    end
end

function widthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.05 then v=0.05 end
        if v>2 then v=2 end
        if v~=c['width'] then
            modified=true
            c['width']=v
            writeInfo(c)
            updateModel()
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
        if v>2 then v=2 end
        if v~=c['length'] then
            modified=true
            c['length']=v
            writeInfo(c)
            updateModel()
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
        if v>2 then v=2 end
        if v~=c['height'] then
            modified=true
            c['height']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function thicknessChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v*0.001
        if v<0.001 then v=0.001 end
        if v>0.02 then v=0.02 end
        if v~=c['thickness'] then
            modified=true
            c['thickness']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function lidA_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],1)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-1
    end
    modified=true
    writeInfo(c)
    updateModel()
    setDlgItemContent()
end

function lidB_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-2
    end
    modified=true
    writeInfo(c)
    updateModel()
    setDlgItemContent()
end

function lidALengthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v/100
        if v<0.1 then v=0.1 end
        if v>1 then v=1 end
        if v~=c['closePartALength'] then
            modified=true
            c['closePartALength']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function lidAWidthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v/100
        if v<0.1 then v=0.1 end
        if v>1 then v=1 end
        if v~=c['closePartAWidth'] then
            modified=true
            c['closePartAWidth']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function lidBLengthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v/100
        if v<0.1 then v=0.1 end
        if v>1 then v=1 end
        if v~=c['closePartBLength'] then
            modified=true
            c['closePartBLength']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function lidBWidthChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        v=v/100
        if v<0.1 then v=0.1 end
        if v>1 then v=1 end
        if v~=c['closePartBWidth'] then
            modified=true
            c['closePartBWidth']=v
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

function inertiaFactorChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0.1 then v=0.1 end
        if v>10 then v=10 end
        if v~=c['inertiaFactor'] then
            modified=true
            c['inertiaFactor']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function torqueChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0.01 then v=0.01 end
        if v>10 then v=10 end
        if v~=c['lidTorque'] then
            modified=true
            c['lidTorque']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function springConstantChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0 then v=0 end
        if v>100 then v=100 end
        if v~=c['lidSpring'] then
            modified=true
            c['lidSpring']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function dampingChange_callback(ui,id,newVal)
    local c=readInfo()
    local v=tonumber(newVal)
    if v then
        if v<0 then v=0 end
        if v>10 then v=10 end
        if v~=c['lidDamping'] then
            modified=true
            c['lidDamping']=v
            writeInfo(c)
            updateModel()
        end
    end
    setDlgItemContent()
end

function redChange(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(newVal/100,g,b,s)
end

function greenChange(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(r,newVal/100,b,s)
end

function blueChange(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(r,g,newVal/100,s)
end

function specularChange(ui,id,newVal)
    modified=true
    local r,g,b,s=getColor()
    setColor(r,g,b,newVal/100)
end

function texture_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],4)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-4
    end
    modified=true
    writeInfo(c)
    updateModel()
    setDlgItemContent()
end

function onCloseClicked()
    if sim_msgbox_return_yes==simMsgBox(sim_msgbox_type_question,sim_msgbox_buttons_yesno,'Finalizing the box',"By closing this customization dialog you won't be able to customize the box anymore. Do you want to proceed?") then
        finalizeModel=true
        simRemoveScript(sim_handle_self)
    end
end


function createDlg()
    if (not ui) and bwUtils.canOpenPropertyDialog() then
        local xml =[[
        <tabs id="77">
            <tab title="General" layout="form">
                <label text="Width (mm)"/>
                <edit oneditingfinished="widthChange_callback" id="1"/>

                <label text="Length (mm)"/>
                <edit oneditingfinished="lengthChange_callback" id="2"/>

                <label text="Height (mm)"/>
                <edit oneditingfinished="heightChange_callback" id="3"/>

                <label text="Thickness (mm)"/>
                <edit oneditingfinished="thicknessChange_callback" id="4"/>

            </tab>
            <tab title="Closing lids">
                <group layout="form" flat="true">
                <checkbox text="Lid A, length (%)" onchange="lidA_callback" id="10" />
                <edit oneditingfinished="lidALengthChange_callback" id="11"/>

                <label text="Lid A, width (%)"/>
                <edit oneditingfinished="lidAWidthChange_callback" id="12"/>
                </group>

                <group layout="form" flat="true">
                <checkbox text="Lid B, length (%)" onchange="lidB_callback" id="13" />
                <edit oneditingfinished="lidBLengthChange_callback" id="14"/>

                <label text="Lid B, width (%)"/>
                <edit oneditingfinished="lidBWidthChange_callback" id="15"/>
                </group>
            </tab>
            <tab title="Colors/Texture" layout="form">
                    <label text="Textured"/>
                    <checkbox text="" onchange="texture_callback" id="888" />
                    
                    <label text="Red"/>
                    <hslider minimum="0" maximum="100" onchange="redChange" id="30"/>
                    <label text="Green"/>
                    <hslider minimum="0" maximum="100" onchange="greenChange" id="31"/>
                    <label text="Blue"/>
                    <hslider minimum="0" maximum="100" onchange="blueChange" id="32"/>
                    <label text="Specular"/>
                    <hslider minimum="0" maximum="100" onchange="specularChange" id="33"/>
            </tab>
            <tab title="More" layout="form">
                <label text="Mass (Kg)"/>
                <edit oneditingfinished="massChange_callback" id="20"/>

                <label text="Inertia adjustment factor"/>
                <edit oneditingfinished="inertiaFactorChange_callback" id="21"/>

                <label text="Lid max. torque"/>
                <edit oneditingfinished="torqueChange_callback" id="22"/>

                <label text="Lid spring constant"/>
                <edit oneditingfinished="springConstantChange_callback" id="23"/>

                <label text="Lid damping"/>
                <edit oneditingfinished="dampingChange_callback" id="24"/>

                <label text="" style="* {margin-left: 150px;}"/>
                <label text="" style="* {margin-left: 150px;}"/>
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
    local data=bwUtils.readPartInfo(model)
    if data['name']=='<partName>' then
        data['name']='PACKINGBOX'
    end
    bwUtils.writePartInfo(model,data)

    bb=simGetObjectHandle('genericPackingBox_bb')
    sideConnection=simGetObjectHandle('genericPackingBox_sideConnection')
    sides=simGetObjectChild(sideConnection,0)
    joints={}
    lids={}
    for i=1,4,1 do
        joints[i]=simGetObjectHandle('genericPackingBox_j'..i)
        lids[i]=simGetObjectChild(joints[i],0)
    end

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
        -- This means the box is part of the part repository or that we want to finalize the model (i.e. won't be customizable anymore)
        local c=readInfo()
        simWriteCustomDataBlock(model,'XYZ_PACKINGBOX_INFO','')
        if simBoolAnd32(c['bitCoded'],1)==0 then
            simRemoveObject(lids[1]) 
            simRemoveObject(lids[2]) 
            simRemoveObject(joints[1]) 
            simRemoveObject(joints[2]) 
        end
        if simBoolAnd32(c['bitCoded'],2)==0 then
            simRemoveObject(lids[3]) 
            simRemoveObject(lids[4]) 
            simRemoveObject(joints[3]) 
            simRemoveObject(joints[4]) 
        end
        simRemoveObject(bb)
    end
    bwUtils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousDlgPos,algoDlgSize,algoDlgPos,distributionDlgSize,distributionDlgPos,previousDlg1Pos)
end
