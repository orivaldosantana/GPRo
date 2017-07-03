local utils=require('utils')

getMinMax=function(minMax1,minMax2)
    if not minMax1 then
        return minMax2
    end
    if not minMax2 then
        return minMax1
    end
    local ret={math.min(minMax1[1],minMax2[1]),math.max(minMax1[2],minMax2[2]),math.min(minMax1[3],minMax2[3]),math.max(minMax1[4],minMax2[4])}
    return(ret)
end

clearCurves=function()
    if plotUi then
        for pl=1,#plots,1 do
            local ii=plots[pl]
            for key,value in pairs(curves[ii]) do
                simExtCustomUI_clearCurve(plotUi,ii,key)
            end
        end
    end
end

enableMouseInteractions=function(enable)
    if plotUi then
        for pl=1,#plots,1 do
            local ii=plots[pl]
            simExtCustomUI_setMouseOptions(plotUi,ii,enable,enable,enable,enable)
        end
    end
end

function onclickCurve(ui,id,name,index,x,y)
    local msg=string.format("Point on curve '%s': (%.4f,%.4f)",name,x,y)
    simExtCustomUI_setLabelText(ui,3,msg)
end

function onCloseModal_callback()
    if modalDlg then
        simExtCustomUI_destroy(modalDlg)
        modalDlg=nil
    end
    selectedCurve=nil
end

function toClipboardClick_callback()
    simAuxFunc("curveToClipboard",model,selectedCurve[2],selectedCurve[1])
    onCloseModal_callback()
end

function toStaticClick_callback()
    simAuxFunc("curveToStatic",model,selectedCurve[2],selectedCurve[1])
    onCloseModal_callback()
    prepareCurves()
end

function removeStaticClick_callback()
    simAuxFunc("removeStaticCurve",model,selectedCurve[2],selectedCurve[1])
    onCloseModal_callback()
    prepareCurves()
end

function onlegendclick(ui,id,curveName)
    if simGetSimulationState()==sim_simulation_stopped then
        local c={}
        local i=1
        for token in string.gmatch(curveName,"[^%s]+") do
            c[i]=token
            i=i+1
        end
        selectedCurve={c[1],id-1}
        if c[2]=='(STATIC)' then
            selectedCurve[2]=id+2
        end

        local xml=[[
        <button text="Copy curve to clipboard" onclick="toClipboardClick_callback"/>
                <label text="" style="* {margin-left: 350px;font-size: 1px;}"/>
        ]]
        if c[2]=='(STATIC)' then
            xml=xml..'<button text="Remove static curve" onclick="removeStaticClick_callback"/>'
        else
            xml=xml..'<button text="Duplicate curve to static curve" onclick="toStaticClick_callback"/>'
        end
        modalDlg=utils.createCustomUi(xml,"Operation on Selected Curve","center",true,"onCloseModal_callback",true)
    end
end

updateCurves=function()
    if plotUi then
        for pl=1,#plots,1 do
            local minMax=nil
            local ii=plots[pl]
            local index=0
            while true do
                local label,curveType,curveColor,xData,yData,minMaxT=simGetGraphCurve(model,ii-1,index)
                if not label then
                    break
                end
                minMax=getMinMax(minMax,minMaxT)
                if curves[ii][label] then
                    simExtCustomUI_clearCurve(plotUi,ii,label)
                    if ii==1 then
                        simExtCustomUI_addCurveTimePoints(plotUi,ii,label,xData,yData)
                        if minMaxT and (minMaxT[2]-minMaxT[1]==0 or minMaxT[4]-minMaxT[3]==0) then
                            simExtCustomUI_addCurveTimePoints(plotUi,ii,label,{xData[#xData]+0.000000001},{yData[#yData]+0.000000001})
                        end
                    else
                        local seq={}
                        for i=1,#xData,1 do
                            seq[i]=i
                        end
                        simExtCustomUI_addCurveXYPoints(plotUi,ii,label,seq,xData,yData)
                        if minMaxT and (minMaxT[2]-minMaxT[1]==0 or minMaxT[4]-minMaxT[3]==0) then
                            simExtCustomUI_addCurveXYPoints(plotUi,ii,label,{seq[#seq]+1},{xData[#xData]+0.000000001},{yData[#yData]+0.000000001})
                        end
                    end
                    if curveType<2 then
                        simExtCustomUI_rescaleAxes(plotUi,ii,label,index~=0,index~=0) -- for non-static curves
                    end
                end
                index=index+1
            end
--            simExtCustomUI_rescaleAxesAll(plotUi,ii,false,false)
            if minMax then
                local rangeS={minMax[2]-minMax[1],minMax[4]-minMax[3]}
                simExtCustomUI_growPlotXRange(plotUi,ii,rangeS[1]*0.01,rangeS[1]*0.01)
                simExtCustomUI_growPlotYRange(plotUi,ii,rangeS[2]*0.01,rangeS[2]*0.01)
            end
            simExtCustomUI_replot(plotUi,ii)
        end
    end
end

prepareCurves=function()
    if plotUi then
        for pl=1,#plots,1 do
            local minMax=nil
            local ii=plots[pl]
            for key,value in pairs(curves[ii]) do
                simExtCustomUI_removeCurve(plotUi,ii,key)
            end
            curves[ii]={}
            local index=0
            while true do
                local label,curveType,curveColor,xData,yData,minMaxT=simGetGraphCurve(model,ii-1,index)
                if not label then
                    break
                end
                local curveStyle
                local scatterShape
                if curveType==0 then
                    -- Non-static line
                    curveStyle=sim_customui_curve_style_line
                    scatterShape={scatter_shape=sim_customui_curve_scatter_shape_none,scatter_size=5,line_size=1}
                end
                if curveType==1 then
                    -- Non-static scatter
                    curveStyle=sim_customui_curve_style_scatter
                    scatterShape={scatter_shape=sim_customui_curve_scatter_shape_square,scatter_size=4,line_size=1}
                end
                if curveType==2 then
                    -- Static line
                    curveStyle=sim_customui_curve_style_line
                    scatterShape={scatter_shape=sim_customui_curve_scatter_shape_none,scatter_size=5,line_size=1,line_style=sim_customui_line_style_dashed}
                end
                if curveType==3 then
                    -- Static scatter
                    curveStyle=sim_customui_curve_style_scatter
                    scatterShape={scatter_shape=sim_customui_curve_scatter_shape_plus,scatter_size=4,line_size=1}
                end
                if ii==1 then
                    simExtCustomUI_addCurve(plotUi,ii,sim_customui_curve_type_time,label,{curveColor[1]*255,curveColor[2]*255,curveColor[3]*255},curveStyle,scatterShape)
                else
                    simExtCustomUI_addCurve(plotUi,ii,sim_customui_curve_type_xy,label,{curveColor[1]*255,curveColor[2]*255,curveColor[3]*255},curveStyle,scatterShape)
                end
                simExtCustomUI_setLegendVisibility(plotUi,ii,true)
                curves[ii][label]=true
                index=index+1
            end
        end
    end
    updateCurves()
end

function getDefaultInfoForNonExistingFields(info)
    if not info['bitCoded'] then
        info['bitCoded']=1+2+4+8 -- 1=visible during simulation, 2=visible during non-simul, 4=show time plots, 8=show xy plots, 16=1:1 proportion for xy plots
    end
end

function readInfo()
    local data=simReadCustomDataBlock(model,'ABC_GRAPH_INFO')
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
        simWriteCustomDataBlock(model,'ABC_GRAPH_INFO',simPackTable(data))
    else
        simWriteCustomDataBlock(model,'ABC_GRAPH_INFO','')
    end
end

function setDlgItemContent()
    if ui then
        local config=readInfo()
        local sel=utils.getSelectedEditWidget(ui)
        simExtCustomUI_setCheckboxValue(ui,1,utils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],2)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,2,utils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],1)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,3,utils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],4)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,4,utils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],8)~=0),true)
        simExtCustomUI_setCheckboxValue(ui,5,utils.getCheckboxValFromBool(simBoolAnd32(config['bitCoded'],16)~=0),true)
        utils.setSelectedEditWidget(ui,sel)
    end
end

function updateEnabledDisabledItemsDlg()
end

function visibleDuringSimulation_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],1)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-1
    end
    modified=true
    writeInfo(c)
    createOrRemovePlotIfNeeded(false)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg()
end

function visibleDuringNonSimulation_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],2)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-2
    end
    modified=true
    writeInfo(c)
    createOrRemovePlotIfNeeded(false)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg()
end

function timeOnly_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],4)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-4
        c['bitCoded']=simBoolOr32(c['bitCoded'],8)
    end
    modified=true
    writeInfo(c)
    removePlot()
    createOrRemovePlotIfNeeded(false)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg()
end

function xyOnly_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],8)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-8
        c['bitCoded']=simBoolOr32(c['bitCoded'],4)
    end
    modified=true
    writeInfo(c)
    removePlot()
    createOrRemovePlotIfNeeded(false)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg()
end

function squareXy_callback(ui,id,newVal)
    local c=readInfo()
    c['bitCoded']=simBoolOr32(c['bitCoded'],16)
    if newVal==0 then
        c['bitCoded']=c['bitCoded']-16
    end
    modified=true
    writeInfo(c)
    removePlot()
    createOrRemovePlotIfNeeded(false)
    setDlgItemContent()
    updateEnabledDisabledItemsDlg()
end

function createDlg()
    if (not ui) then
        local xml=[[
                <label text="Visible while simulation not running"/>
                <checkbox text="" onchange="visibleDuringNonSimulation_callback" id="1" />

                <label text="Visible while simulation running"/>
                <checkbox text="" onchange="visibleDuringSimulation_callback" id="2" />

                <label text="Show time plots"/>
                <checkbox text="" onchange="timeOnly_callback" id="3" />

                <label text="Show X/Y plots"/>
                <checkbox text="" onchange="xyOnly_callback" id="4" />

                <label text="X/Y plots keep 1:1 aspect ratio"/>
                <checkbox text="" onchange="squareXy_callback" id="5" />
        ]]
        ui=utils.createCustomUi(xml,simGetObjectName(model),previousDlgPos,false,nil,false,false,false,'layout="form"')
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

function removePlot()
    if plotUi then
        local x,y=simExtCustomUI_getPosition(plotUi)
        previousPlotDlgPos={x,y}
        local x,y=simExtCustomUI_getSize(plotUi)
        previousPlotDlgSize={x,y}
        plotTabIndex=simExtCustomUI_getCurrentTab(plotUi,77)
        simExtCustomUI_destroy(plotUi)
        plotUi=nil
    end
end

function onClosePlot_callback()
    if simGetSimulationState()==sim_simulation_stopped then
        local c=readInfo()
        c['bitCoded']=simBoolOr32(c['bitCoded'],2)-2
        writeInfo(c)
        setDlgItemContent()
        updateEnabledDisabledItemsDlg()
    end
    removePlot()
end

function createPlot()
    if not plotUi then
        local c=readInfo()
        plots={}

        local xml='<tabs id="77">'
        if (simBoolAnd32(c['bitCoded'],4)~=0) then
            xml=xml..[[
            <tab title="Time graph">
            <plot id="1" onclick="onclickCurve" onlegendclick="onlegendclick" max-buffer-size="100000" cyclic-buffer="false" background-color="25,25,25" foreground-color="150,150,150"/>
            </tab>
            ]]
            plots={1}
        end
        if (simBoolAnd32(c['bitCoded'],8)~=0) then
            local squareAttribute=''
            if (simBoolAnd32(c['bitCoded'],16)~=0) then
                squareAttribute='square="true"'
            end
            xml=xml..[[
            <tab title="X/Y graph">
            <plot id="2" onclick="onclickCurve" onlegendclick="onlegendclick" max-buffer-size="100000" cyclic-buffer="false" background-color="25,25,25" foreground-color="150,150,150"]]..squareAttribute..[[/>
            </tab>
            ]]
            plots[#plots+1]=2
        end
        xml=xml..[[
        </tabs>
        <br/>
        <label id="3" />
        ]]

        if not previousPlotDlgPos then
            previousPlotDlgPos="bottomRight"
            if graphDlgNumber==2 then
                previousPlotDlgPos="bottomLeft"
            end
        end
        plotUi=utils.createCustomUi(xml,simGetObjectName(model),previousPlotDlgPos,true,"onClosePlot_callback",false,true,false,'layout="grid"',previousPlotDlgSize)
        if (simBoolAnd32(c['bitCoded'],4)~=0) then
            simExtCustomUI_setPlotLabels(plotUi,1,"Time (seconds)","")
        end
        if (simBoolAnd32(c['bitCoded'],8)~=0) then
            simExtCustomUI_setPlotLabels(plotUi,2,"X","Y")
        end
        if #plots==1 then
            plotTabIndex=0
        end
        simExtCustomUI_setCurrentTab(plotUi,77,plotTabIndex,true)

        curves={{},{}}
        prepareCurves()

        local s=simGetSimulationState()
        enableMouseInteractions( (s==sim_simulation_stopped)or(s==sim_simulation_paused) )
    end
end

createOrRemovePlotIfNeeded=function(forSimulation)
    local c=readInfo()
    if forSimulation then
        if (simBoolAnd32(c['bitCoded'],1)==0) then
            removePlot()
        else
            createPlot()
        end
    else
        if (simBoolAnd32(c['bitCoded'],2)==0) then
            removePlot()
        else
            createPlot()
        end
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
    modified=false
    plotTabIndex=0
    lastT=simGetSystemTimeInMs(-1)
    model=simGetObjectAssociatedWithScript(sim_handle_self)
    simSetScriptAttribute(sim_handle_self,sim_customizationscriptattribute_activeduringsimulation,true)
    previousPlotDlgPos,previousPlotDlgSize,previousDlgPos=utils.readSessionPersistentObjectData(model,"dlgPosAndSize")
    createOrRemovePlotIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_firstaftersimulation) then
    createOrRemovePlotIfNeeded(false)
    enableMouseInteractions(true)
end

if (sim_call_type==sim_customizationscriptcall_lastbeforesimulation) then
    removeDlg()
    createOrRemovePlotIfNeeded(true)
    prepareCurves()
    clearCurves()
    enableMouseInteractions(false)
end

if (sim_call_type==sim_customizationscriptcall_simulationpausefirst) then
    enableMouseInteractions(true)
end

if (sim_call_type==sim_customizationscriptcall_simulationpauselast) then
    enableMouseInteractions(false)
end

if (sim_call_type==sim_customizationscriptcall_simulationsensing) then
    updateCurves()
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

if (sim_call_type==sim_customizationscriptcall_lastbeforeinstanceswitch) then
    removeDlg()
    removePlot()
end

if (sim_call_type==sim_customizationscriptcall_firstafterinstanceswitch) then
    createOrRemovePlotIfNeeded()
end

if (sim_call_type==sim_customizationscriptcall_cleanup) then
    removePlot()
    removeDlg()
    utils.writeSessionPersistentObjectData(model,"dlgPosAndSize",previousPlotDlgPos,previousPlotDlgSize,previousDlgPos)
end