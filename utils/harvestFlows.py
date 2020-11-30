#makes a quick and dirty cut flow for the slides

import os,sys,analysisComponentsNoRoot

components = analysisComponentsNoRoot.AnalysisComponents()

def harvestSingleLog(logFilePath,cutFlowStages):
#    print logFilePath
    logFile = open(logFilePath,"r")
    harvestNow = False
    cutFlow = {}
    for line in logFile:
        if "<CutFlowTable> GlobalCutFlow result:" in line: harvestNow = True
        if not harvestNow: continue
        for stage in cutFlowStages:
            if not stage in line: continue
            cutFlow[stage] = float(line.split("|")[5])
            break
    return cutFlow

def getCutFlow(sampleDir,cutFlowStages):
    cutFlow = {}
    for stage in cutFlowStages: cutFlow[stage] = 0
    logFiles = [f for f in os.listdir("{0}/logs/".format(sampleDir)) if f.endswith("log")]
    for logFile in logFiles:
        tmpCutFlow = harvestSingleLog("{0}/logs/{1}".format(sampleDir,logFile),cutFlowStages)
        for stage in cutFlowStages: 
            if stage in tmpCutFlow.keys(): cutFlow[stage] += tmpCutFlow[stage]
    return cutFlow
        

def doDirectory(dirName,cutFlowStages):
    sampleDirs = [f for f in os.listdir(dirName)]
    ignoreSamples = ["wPlusJetsMCatNLO","wPlusJetsMadgraph","tW_mcanlo"]
    #init the cutflows
    cutFlows = {}
    for sampleD in sampleDirs:
        sample = components.histoGramPerSample[sampleD]
        if sample in cutFlows.keys(): continue
        cutFlows[sample] = {}
        for stage in cutFlowStages:
            cutFlows[sample][stage] = 0.
    for sampleDir in sampleDirs:
        if sampleDir in ignoreSamples:continue
        print sampleDir,components.histoGramPerSample[sampleDir]
        tmpCutFlow = getCutFlow(os.path.join(dirName,sampleDir),cutFlowStages)
        scaleFactor = 1.
        if components.histoGramPerSample[sampleDir] == "wPlusJets": scaleFactor = 2.98
        for stage in cutFlowStages: cutFlows[components.histoGramPerSample[sampleDir]][stage]+=tmpCutFlow[stage]*scaleFactor
    return cutFlows

def printCutFlowNicely(cutFlows,dataCutFlow,isEle):
    tableMapMap = {"PV":"Vertex","Trigger":"Trigger","VetoElectron.Number.All":"Single Lepton","Jet.Number.All":" 2 <= nJets <= 4"}
    tableMap = ["PV","Trigger","VetoElectron.Number.All","Jet.Number.All"]
    dataName = "muData"
    if isEle: dataName = "eleData"
    print "Step ",
    for sample in cutFlows.keys():
        print "& {0} ".format(sample),
    print " & TotalMC & Data \\\\"
    for stage in tableMap:
        stageTotal = 0.
        for sample in cutFlows.keys(): stageTotal += cutFlows[sample][stage]
        if stageTotal == 0: stageTotal = 1.
        print tableMapMap[stage],
        for sample in cutFlows.keys():
            print " & {0:.1f} ({1:.1f}\\%)".format(cutFlows[sample][stage],100*(cutFlows[sample][stage]/stageTotal)),
        print " & {0:.1f} & {1:.1f} \\\\".format(stageTotal,dataCutFlow[dataName][stage])
        

if __name__ == "__main__":
    cutFlowStages = ["PV","Trigger","TightMuon.Number.Max","VetoMuon.Number.All","TightElectron.Number.All","VetoElectron.Number.All","Jet.Number.All"]
    isEle = False
    if "--ele" in sys.argv: isEle = True
    if "--mu" in sys.argv:isEle = False
    elePost = "Ele"
    if not isEle: elePost = ""
    cutFlows = doDirectory("tW{0}".format(elePost),cutFlowStages)
    dataCutFlow = doDirectory("tWData{0}".format(elePost),cutFlowStages)
    for key1 in cutFlows.keys():
        print key1
        for key2 in cutFlows[key1].keys():
            print key2, cutFlows[key1][key2]
    printCutFlowNicely(cutFlows,dataCutFlow,isEle)
