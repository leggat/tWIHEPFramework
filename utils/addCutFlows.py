#A script that will add together the cut flows from the different datasets

import sys,os,math,weightProcesses

samplesMC=[
#"qcd1000_1500",
#"qcd100_200",
#"qcd1500_2000",
#"qcd2000_inf",
#"qcd200_300",
#"qcd300_500",
#"qcd500_700",
#"qcd700_1000",
"sChan",
"tChan_top",
"tChan_antitop",
"ttbar",
"ttbarBU",
"tW_top_nfh",
"tW_antitop_nfh",
#"wPlusJets",
"ww",
"wz",
"zz",
"zPlusJetsLowMass",
"zPlusJetsHighMass",
#"wPlusJetsMadgraph",
"wPlus0Jets",
"wPlus1Jets",
"wPlus2Jets"
]

histoGramPerSample = {"tW_top":"tW","tW_antitop":"tW","sChan":"singleTop","tChan":"singleTop","zz":"VV","zPlusJetsLowMass":"zPlusJets","zPlusJetsHighMass":"zPlusJets","wz":"VV","ww":"VV","wPlusJets":"wPlusJets","ttbar":"ttbar","ttbarBU":"ttbar","qcd700_1000":"qcd","qcd500_700":"qcd","qcd300_500":"qcd","qcd200_300":"qcd","qcd2000_inf":"qcd","qcd1500_2000":"qcd","qcd100_200":"qcd","qcd1000_1500":"qcd","wPlusJetsMCatNLO":"wPlusJets","singleMuon":"Data","tChan_top":"singleTop","tChan_antitop":"singleTop","tW_top_nfh":"tW","tW_antitop_nfh":"tW","wPlusJetsMadgraph":"wPlusJets","wPlus0Jets":"wPlusJets","wPlus1Jets":"wPlusJets","wPlus2Jets":"wPlusJets"}

#samplesMC = ["tW_top","tW_antitop","ttbar","singleMuon"]
#samplesMC = ["tW_top"]

perMCSFs = {}
perMCSFs["ttbar"] = 2.
perMCSFs["qcd"] = 5099897.0/2604652.19727

masterScale =  1.

sampleDataMu = ['SingMuB','SingMuC','SingMuD','SingMuE','SingMuF','SingMuG',"SingMuH"]
sampleDataEle = ['SingEleB','SingEleC','SingEleD','SingEleE','SingEleF','SingEleG',"SingEleH"]
sampleData = sampleDataMu

emptyFile = {}

inDirMC = sys.argv[1]
inDirData = ""
if len(sys.argv) > 2:
    inDirData = sys.argv[2]

region = inDirMC[-5:-1]
isEle = False
if "Ele" in inDirData:
    isEle = True

weights = weightProcesses.ReweightObject(False,isEle)



if "Ele" in inDirMC:
    sampleData = sampleDataEle
    perMCSFs["qcd"] *= 0.575639172238
    perMCSFs["ttbar"] *= 0.905594867235
    perMCSFs["wPlusJets"] = 3.08934118354

else:
    perMCSFs["qcd"] = 5227921/8899239.0 #This will be the muon channel once I've re-run all of the files.
    perMCSFs["qcd"] *= 1.10329115653
    perMCSFs["ttbar"] = 1.
    perMCSFs["wPlusJets"] = 3.48537322128
    perMCSFs["qcd"] = 0.703806774077
    perMCSFs["wPlusJets"] = 3.50893764226

perMCSFs["ttbar"] = 1.     
perMCSFs["qcd"] = 1.
perMCSFs["wPlusJets"] = 1.



cutYield = {}
cutOrder = []

cutYield["total"] = {}

logFilesWithNothing = {}

for sampleName in samplesMC:
    logFilesWithNothing[sampleName] = 0
    print sampleName
    inDir = inDirMC
    sample = histoGramPerSample[sampleName]
    scaleFactor = 1. * masterScale
    scaleFactor *= weights.getDatasetWeight(histoGramPerSample[sampleName],region)
    if sample == "Data":
        if inDirData == "":
            continue
        inDir= inDirData
    if not sample in cutYield.keys():
        cutYield[sample] = {}
    logFiles = [f for f in os.listdir(inDir+sampleName+"/logs/") if "log" in f and "#" not in f]
    for logFileName in logFiles:
        hasInfo = False
        logFile = open(inDir+sampleName+"/logs/"+logFileName,'r')
        for line in logFile:
            if not len(line.split("|")) == 7: continue
            cut = line.split("|")[1]
            if "Min" in cut or "Max" in cut: continue
            hasInfo = True
            if not cut in cutOrder:
                cutOrder.append(cut)
            if cut == "                                ":
                if not cut in cutYield['total'].keys():
                    cutYield['total'][cut] = "Total"
                cutYield[sample][cut] = sample
                continue
            if "PV" in cut:
                nEvt = float(line.split("|")[5])
                if nEvt == 0:
                    if sampleName not in emptyFile.keys():
                        emptyFile[sampleName] = []
                    emptyFile[sampleName].append(inDir+sampleName+"/logs/"+logFileName)
            if cut in cutYield[sample].keys():
                cutYield[sample][cut] += scaleFactor * float(line.split("|")[5])
            else:
                cutYield[sample][cut] = scaleFactor * float(line.split("|")[5])
            if sample == "Data": continue
            if cut in cutYield['total'].keys():
                cutYield['total'][cut] += scaleFactor * float(line.split("|")[5])
            else:
                cutYield['total'][cut] = scaleFactor * float(line.split("|")[5])
        if not hasInfo: logFilesWithNothing[sampleName] += 1

cutYield['totalMinusQCD'] = {}
cutYield['sOverb'] = {}
for cut in cutYield['total'].keys():
    if cut == "                                ":
        cutYield['totalMinusQCD'][cut] = "Total (no QCD)"
        cutYield['sOverb'][cut] = "S/sqrt(S+B)"
    else:
        cutYield['totalMinusQCD'][cut] = cutYield['total'][cut]
        cutYield['sOverb'][cut] = cutYield['tW'][cut]/math.sqrt(cutYield['total'][cut])

cutYield['Data'] = {}
cutYield['qcd'] = {}

for sampleName in sampleData:
    if inDirData == "": continue
    logFiles = [f for f in os.listdir(inDirData+sampleName+"/logs/") if "log" in f and "#" not in f]
    for logFileName in logFiles:
        logFile = open(inDirData+sampleName+"/logs/"+logFileName,'r')
        for line in logFile:
            if not len(line.split("|")) == 7: continue
            cut = line.split("|")[1]
            if "Min" in cut or "Max" in cut: continue
            if not cut in cutOrder:
                cutOrder.append(cut)
            if cut == "                                ":
                cutYield['Data'][cut] = "Data"
                continue
            if "PV" in cut:
                nEvt = float(line.split("|")[5])
                if nEvt == 0:                            
                    if not sampleName in emptyFile.keys():
                        emptyFile[sampleName] = []
                    emptyFile[sampleName].append(inDirData+sampleName+"/logs/"+logFileName)
            if cut in cutYield["Data"].keys():
                cutYield["Data"][cut] += float(line.split("|")[5])
            else:
                cutYield["Data"][cut] = float(line.split("|")[5])
                
                
    print sampleName

inDirQCD = "tWInv"+inDirData.split("tW")[-1]
for sampleName in sampleData:
    print sampleName
    if inDirData == "": continue
    logFiles = [f for f in os.listdir(inDirQCD+sampleName+"/logs/") if "log" in f and "#" not in f]
    scaleFactor = weights.getDatasetWeight("qcd",region)
    for logFileName in logFiles:
        logFile = open(inDirQCD+sampleName+"/logs/"+logFileName,'r')
        for line in logFile:
            if not len(line.split("|")) == 7: continue
            cut = line.split("|")[1]
            
            if "UnIsolated" in cut:
                cut = cut.split("UnIsolated")[0]+"     Tight"+cut.split("UnIsolated")[1]
            if "Min" in cut or "Max" in cut: continue
            if not cut in cutOrder:
                cutOrder.append(cut)
            if cut == "                                ":
                cutYield['qcd'][cut] = "QCD"
                continue
            if "PV" in cut:
                nEvt = float(line.split("|")[5])
                if nEvt == 0:                            
                    if not sampleName in emptyFile.keys():
                        emptyFile[sampleName] = []
                    emptyFile[sampleName].append(inDirQCD+sampleName+"/logs/"+logFileName)
            if cut in cutYield["qcd"].keys():
                cutYield["qcd"][cut] += scaleFactor * float(line.split("|")[5])
            else:
                cutYield["qcd"][cut] = scaleFactor * float(line.split("|")[5])
            cutYield['total'][cut] += scaleFactor * float(line.split("|")[5])

tableDict = {"                             PV ":"Primary Vertex      ",
             "                    MuonTrigger ":"Trigger             ",
             "                ElectronTrigger ":"Trigger             ",
             "       TightElectron.Number.All ":"1 Electron          ",
             "       UnIsolatedMuon.Number.All ":"1 Muon              ",
             "  UnIsolatedElectron.Number.All ":"1 Electron          ",
             "           TightMuon.Number.All ":"1 muon              ",
             "            VetoMuon.Number.All ":"Loose muon veto     ",
             "        VetoElectron.Number.All ":"Loose electron veto ",
             "                 Jet.Number.All ":"3 jets              ",
             "          Tagged.Jet.Number.All ":"1 tag               ",
             "                                ":"                    ",
             "                     ChannelCut ":"Channel cut         "
}

for cut in cutOrder:
    print tableDict[cut],
    for sample in cutYield.keys():
        if sample == "Data" or sample == "total" or sample == "totalMinusQCD" or sample == "sOverb": continue
        if isinstance(cutYield[sample][cut],basestring): print " \t& {0}".format(cutYield[sample][cut]),
        else: print " \t& {0:.1f}".format(cutYield[sample][cut]),
#    if inDirData == "": 
    if isinstance(cutYield["total"][cut],basestring):  print " \t& {0} \t& {1} \t& {2} \t& {3}".format(cutYield["total"][cut],cutYield['totalMinusQCD'][cut],"" if "Data" not in cutYield.keys() else cutYield["Data"][cut],cutYield['sOverb'][cut])
    else: print " \t& {0:.1f} \t& {1:.1f} \t& {2:.1f} \t& {3:.1f}".format(cutYield["total"][cut],cutYield['totalMinusQCD'][cut],"" if "Data" not in cutYield.keys() else cutYield["Data"][cut],cutYield['sOverb'][cut])
#        print " \t&",cutYield["total"][cut]," \t&",cutYield['totalMinusQCD'][cut]," \t&"," \t&",cutYield['sOverb'][cut],
#    else: print " \t&",cutYield["total"][cut]," \t&",cutYield['totalMinusQCD'][cut]," \t&",cutYield["Data"][cut]," \t&",cutYield['sOverb'][cut], 
    print "\t\\\\"

print "empty log files"

resubmitFile = file("resubmit.sh","w")
print >> resubmitFile, "#!/bin/bash"
print >> resubmitFile, "cd ", inDirMC

for sample in samplesMC:
    if not sample in emptyFile.keys(): continue
    print sample
    print "missing files: ", len(emptyFile[sample])
    for i in emptyFile[sample]:
        print >> resubmitFile, "condor_submit",sample+"/scripts/"+i.split("/")[-1].split(".")[0]+".submit -group cms -name job@schedd01.ihep.ac.cn"

resubmitFile.close()

resubmitDataFile = file("resubmitData.sh","w")
print >> resubmitDataFile, "#!/bin/bash"
print >> resubmitDataFile, "cd ", inDirData

for sample in sampleData:
    if not sample in emptyFile.keys(): continue
    print sample
    print "missing files: ", len(emptyFile[sample])
    for i in emptyFile[sample]:
        print >> resubmitDataFile, "condor_submit",sample+"/scripts/"+i.split("/")[-1].split(".")[0]+".submit -group cms -name job@schedd01.ihep.ac.cn"

resubmitDataFile.close()

for sample in logFilesWithNothing.keys():
    if logFilesWithNothing[sample] > 0: print sample, "has ",logFilesWithNothing[sample],"empty logs"
