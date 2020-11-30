from ROOT import *

import sys,os,subprocess,math
from threading import Thread
from multiprocessing.pool import ThreadPool

#thisDir = os.getcwd()
#print os.path.join(thisDir.split("tmva")[0],"utils")
#sys.path.append(os.path.join(thisDir.split("tmva")[0],"utils"))

import weightProcesses

gROOT.SetBatch()

isElectron = False

weights = weightProcesses.ReweightObject(kTRUE,isElectron)

isOnlyWJets = True

#Rfers to dedicated jer/jes runs. Set to false to use built in jet shift mechanics.
useJESJER = False
nJetVariations = 54

# Command booleans to change what the program does. Probably easier to change these than always alter the main?
doSFExtraction = True
runFits = True
runExtraction = False
calculateError = False

systematics = [""]
systematics = ["","_bTag_up","_bTag_down","_mistag_up","_mistag_down","_PDF_up","_PDF_down","_LSF_up","_LSF_down","_Trig_up","_Trig_down","_PU_up","_PU_down"]
if not useJESJER:
    for i in range(nJetVariations):
        systematics.append("_JetShifts_{0}".format(i))

distOfInterest = "MVA_ttbar_bin30"
distOfInterest = "mTW"

#samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","ttbar","wPlusJetsMCatNLO","ttbarBU","SingMuB","SingMuC","SingMuD","SingMuE","SingMuF","SingMuG","SingMuH","wPlus0Jets","wPlus1Jets","wPlus2Jets"]
samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","ttbar","ttbarBU","wPlus0Jets","wPlus1Jets","wPlus2Jets"]
dataSample = ["SingMuB","SingMuC","SingMuD","SingMuE","SingMuF","SingMuG","SingMuH"]
if isElectron: dataSample = ["SingEleB","SingEleC","SingEleD","SingEleE","SingEleF","SingEleG","SingEleH"]
samples += dataSample

colourPerSample = {"tW_top":kGreen+2,"tW_antitop":kGreen+2,"tW_top_nfh":kGreen+2,"tW_antitop_nfh":kGreen+2,"tChan":kYellow,"zPlusJetsLowMass":kBlue,"zPlusJetsHighMass":kBlue,"wz":kPink,"ww":kPink,"zz":kPink,"wPlusJets":kTeal,"ttbar":kRed,"qcd700_1000":kGray,"qcd500_700":kGray,"qcd300_500":kGray,"qcd200_300":kGray,"qcd2000_inf":kGray,"qcd1500_2000":kGray,"qcd100_200":kGray,"qcd1000_1500":kGray,"sChan":kOrange,"VV":kPink,"qcd":kGray,"tW":kGreen+2,"zPlusJets":kBlue,"singleTop":kYellow}

histoGramPerSample = {"tW_top":"tW","tW_antitop":"tW","sChan":"singleTop","tChan":"singleTop","zz":"VV","zPlusJetsLowMass":"zPlusJets","zPlusJetsHighMass":"zPlusJets","wz":"VV","ww":"VV","wPlusJets":"wPlusJets","ttbar":"ttbar","qcd700_1000":"qcd","qcd500_700":"qcd","qcd300_500":"qcd","qcd200_300":"qcd","qcd2000_inf":"qcd","qcd1500_2000":"qcd","qcd100_200":"qcd","qcd1000_1500":"qcd","wPlusJetsMCatNLO":"wPlusJets","tChan_top":"singleTop","tChan_antitop":"singleTop","ttbarBU":"ttbar","tW_top_nfh":"tW","tW_antitop_nfh":"tW","wPlusJetsMadgraph":"wPlusJets","SingEleB":"data","SingEleC":"data","SingEleD":"data","SingEleE":"data","SingEleF":"data","SingEleG":"data","SingEleH":"data","SingMuB":"data","SingMuC":"data","SingMuD":"data","SingMuE":"data","SingMuF":"data","SingMuG":"data","SingMuH":"data","wPlus0Jets":"wPlusJets","wPlus1Jets":"wPlusJets","wPlus2Jets":"wPlusJets"}

def getOneRegionPlots(inDir,savePostfix,outDir):
    alldists = {}
    region = savePostfix
#    if savePostfix == "3j1t": region = ""

    for sample in samples:
        if not histoGramPerSample[sample] in alldists.keys():
            alldists[histoGramPerSample[sample]] = {}
        inFile = TFile(inDir+"{1}/output_{0}.root".format(sample,region),"READ")
        for syst in systematics:
            if "Sing" in sample and not syst =="": continue
            if not syst in alldists[histoGramPerSample[sample]].keys():
                print "{0}_{1}{2} new".format(distOfInterest,sample,syst)
                alldists[histoGramPerSample[sample]][syst] = inFile.Get("{0}_{1}{2}".format(distOfInterest,sample,syst)).Clone("{0}_{1}{2}".format(distOfInterest,histoGramPerSample[sample],syst))
                alldists[histoGramPerSample[sample]][syst].SetDirectory(0)
                
                print alldists[histoGramPerSample[sample]][syst],alldists[histoGramPerSample[sample]][syst].Integral()
            else:
                print "{0}_{1}{2} add".format(distOfInterest,sample,syst)
                alldists[histoGramPerSample[sample]][syst].Add(inFile.Get("{0}_{1}{2}".format(distOfInterest,sample,syst)))
                print alldists[histoGramPerSample[sample]][syst],alldists[histoGramPerSample[sample]][syst].Integral()

        
                
    #Get QCD templates
    for sample in dataSample:
        inFile = TFile(inDir+"QCD{1}/output_{0}.root".format(sample,region),"READ")
        if not "qcd" in alldists.keys():
            alldists["qcd"] = {}
            alldists["qcd"][""] = inFile.Get("{0}_{1}".format(distOfInterest,sample)).Clone("{0}_{1}".format(distOfInterest,"qcd"))
            alldists["qcd"][""].SetDirectory(0)
            print "QCD hist has {0} events".format(alldists["qcd"][""].Integral())
        else:
            alldists["qcd"][""].Add(inFile.Get("{0}_{1}".format(distOfInterest,sample)))
            print "QCD hist has {0} events".format(alldists["qcd"][""].Integral())

    if useJESJER:
        #JES/JER templates now
        for jetSyst in ["JESUp","JESDown","JERUp","JERDown"]:
            for sample in samples:
                if "Sing" in sample: continue    
                systInFile = TFile("{0}{2}{1}/output_{3}.root".format(inDir,region,jetSyst,sample))
                if not jetSyst in alldists[histoGramPerSample[sample]].keys():
                    alldists[histoGramPerSample[sample]][jetSyst] = systInFile.Get("{0}_{1}".format(distOfInterest,sample)).Clone()
                    alldists[histoGramPerSample[sample]][jetSyst].SetDirectory(0)
                else:
                    alldists[histoGramPerSample[sample]][jetSyst].Add(systInFile.Get("{0}_{1}".format(distOfInterest,sample)))
            
            
    systSamples = [f for f in os.listdir(inDir+"/Systs"+region+"/")]
    systSampleList = []
    for systFile in systSamples:
        inSystFile = TFile(inDir+"Systs{1}/{0}".format(systFile,region),"READ")
        systName = systFile.split("_")[-1].split(".root")[0]
        sampleName = systFile.split("output_")[1].split(systName)[0][:-1]
        print systFile, systName, sampleName
        if not systName in systSampleList: systSampleList.append(systName)
        if not sampleName in histoGramPerSample.keys():
            print "Sample: ",systFile," does not fit the naming convention. Fix this!"
            continue
        if not systName in alldists[histoGramPerSample[sampleName]].keys():
            print "{0}_{1}_{2}".format(distOfInterest,sampleName,systName)
            alldists[histoGramPerSample[sampleName]][systName] = inSystFile.Get("{0}_{1}_{2}".format(distOfInterest,sampleName,systName)).Clone()
            alldists[histoGramPerSample[sampleName]][systName].SetDirectory(0)
        else:
            alldists[histoGramPerSample[sampleName]][systName].Add(inSystFile.Get("{0}_{1}_{2}".format(distOfInterest,sampleName,systName)))

    for sample in alldists.keys():
        for systName in systSampleList:
            if not systName in alldists[sample].keys():
                alldists[sample][systName] = alldists[sample][""].Clone()
        
    #Scale the templates based on pre-fit numbers
    for process in alldists.keys():
        scaleFactor = weights.getDatasetWeight(process,savePostfix)
        for syst in alldists[process].keys():
            alldists[process][syst].Scale(scaleFactor)

    outFile = TFile(outDir+"/combinedOutput{0}.root".format(savePostfix),"RECREATE")
    outFile.cd()
    for sys in alldists["tW"].keys():
        print sys
        tempStack = THStack(distOfInterest+sys,distOfInterest+sys)
        dataHist = 0
        for key in alldists.keys():
            if key == "data":
                dataHist = alldists[key][""]
                continue
            if key == "qcd":
                alldists[key][""].SetLineColor(colourPerSample[key])
                alldists[key][""].SetLineWidth(1)
                tempStack.Add(alldists[key][""])
                continue
            alldists[key][sys].SetLineColor(colourPerSample[key])
            alldists[key][sys].SetLineWidth(1)
            tempStack.Add(alldists[key][sys])
            
            
        canvy = TCanvas(distOfInterest+sys,distOfInterest+sys,1000,800)
        canvy.cd()
        tempStack.Draw("hist")
        dataHist.Draw("same")
        canvy.Write()
        canvy.SaveAs("{2}/{0}{1}{3}.png".format(distOfInterest,sys,outDir,savePostfix))
        canvy.SaveAs("{2}/{0}{1}{3}.root".format(distOfInterest,sys,outDir,savePostfix))
        del canvy
    outFile.Write()
    print alldists.keys(),alldists[alldists.keys()[0]].keys()
    for key in alldists.keys():
        print key,alldists[key][""].Integral()
        for key2 in alldists[key].keys():
            del alldists[key][key2]
    print alldists.keys(),alldists[alldists.keys()[0]].keys()

def doFit(outDir,region,logDir,runFit):
    files = [f for f in os.listdir(outDir) if region in f and ".root" in f and not "combined" in f]
#    files = ["mTW4j2t.root","mTW_bTag_down4j2t.root","mTW_bTag_up4j2t.root","mTW_mistag_down4j2t.root","mTW_mistag_up4j2t.root"]
    print files
#    return
    wJetSFsup = {}
    wJetSFsdown = {}
    wJetsSF = 0
    qcdSFsup = {}
    qcdSFsdown = {}
    qcdSF = 0
    lineToSearchFor = "wJet"
    if not isOnlyWJets: lineToSearchFor = "MC"
    for fileName in files:
        print fileName
        fitFileToUse = "fit.py"
        if not isOnlyWJets: fitFileToUse = "allBkgFit.py"
        if runFit: subprocess.call("python ../../framework/utils/bkgFit/{5} {2}/{0} {4} > {3}/{1}.txt".format(fileName,fileName.split(".root")[0],outDir,logDir,region,fitFileToUse),shell=True)
        fitLog = open("{1}/{0}.txt".format(fileName.split(".root")[0],logDir),"r")
        for line in fitLog.readlines():
            if not ("{0} SF".format(lineToSearchFor) in line or "QCD SF" in line): continue
            if lineToSearchFor in line: 
                if "up" in fileName or "Up" in fileName: wJetSFsup[fileName.split("mTW")[-1].split(".root")[0]] = (float(line.split(" = ")[1][:-1]))
                elif "down" in fileName or "Down" in fileName: wJetSFsdown[fileName.split("mTW")[-1].split(".root")[0]] = (float(line.split(" = ")[1][:-1]))
                elif "mTW{0}".format(region) in fileName: wJetsSF = float(line.split(" = ")[1][:-1])
            if "QCD" in line: 
                if "up" in fileName or "Up" in fileName:  qcdSFsup[fileName.split("mTW")[-1].split(".root")[0]] = (float(line.split(" = ")[1][:-1]))
                elif "down" in fileName or "Down" in fileName:  qcdSFsdown[fileName.split("mTW")[-1].split(".root")[0]] = (float(line.split(" = ")[1][:-1]))
                elif "mTW{0}".format(region) in fileName: qcdSF = float(line.split(" = ")[1][:-1])
#    subprocess.call("python ../../framework/utils/bkgFit/fit.py fitPlots/{0} > fitLogs/{1}.txt".format("mTW2j1t.root","mTW2j1t"),shell=True)
    return [wJetsSF,wJetSFsup,wJetSFsdown,qcdSF,qcdSFsup,qcdSFsdown]

    print wJetsSF
    print wJetSFsup
    print wJetSFsdown
    print qcdSF
    print qcdSFsup
    print qcdSFsdown
    

#    print outputList

def calculateError(wJetsSF,wJetSFsup,wJetSFsdown,qcdSF,qcdSFsup,qcdSFsdown,region):

    wJetsErrorUp = 0
    wJetsErrorDown = 0
    qcdErrorUp = 0
    qcdErrorDown = 0
    
    for key in wJetSFsup.keys():
        item = wJetSFsup[key]
        wJetsErrorUp += (item-wJetsSF) * (item-wJetsSF)
#    for item in wJetSFsdown:
    for key in wJetSFsdown.keys():
        item = wJetSFsdown[key]
        wJetsErrorDown += (item-wJetsSF) * (item-wJetsSF)
#    for item in qcdSFsup:
    for key in qcdSFsup.keys():
        item = qcdSFsup[key]
        qcdErrorUp += (item-qcdSF) * (item-qcdSF)
#    for item in qcdSFsdown:
    for key in qcdSFsdown.keys():
        item = qcdSFsdown[key]
        qcdErrorDown += (item-qcdSF) * (item-qcdSF)
    
    print "{10} region\nW+jets SF: {0} + {1} ({6}%) - {2} ({7}%)\nQCD SF:    {3} + {4} ({8}%) - {5} ({9}%)".format(wJetsSF,sqrt(wJetsErrorUp),sqrt(wJetsErrorDown),qcdSF,sqrt(qcdErrorUp),sqrt(qcdErrorDown),sqrt(wJetsErrorUp)/wJetsSF,sqrt(wJetsErrorDown)/wJetsSF,sqrt(qcdErrorUp)/qcdSF,sqrt(qcdErrorDown)/qcdSF,region)
    
if __name__ == "__main__":
    inDir = sys.argv[1]
    outDir = sys.argv[2]
    if (not os.path.isdir(outDir)):
        subprocess.call("mkdir -p {0}/".format(outDir),shell=True)

    logDir = sys.argv[3]
    if (not os.path.isdir(logDir)):
        subprocess.call("mkdir -p {0}/".format(logDir),shell=True)
    
    
    regionalOutputs = {}

    regionList = ["3j1t","2j1t","3j2t","4j1t","4j2t"]
#    regionList = ["4j2t"]
#    regionList = ["3j1t"]
#    regionList = ["3j1t","3j2t","4j1t","4j2t"]

    if runExtraction:
        print "Beginning extraction"
        threads = {}
        for region in regionList:
            #do it multithreaded because why not?
            getOneRegionPlots(inDir,region,outDir)
#            threads[region] = Thread(target = getOneRegionPlots, args = (inDir,region,outDir,) )
#            threads[region].start()
#            threads[region].join()
#        for key in threads.keys():
#            threads[key].join()

    if doSFExtraction: 
        print "Beginning fits"
        pool = ThreadPool(processes=len(regionList))
#        threads = {}
        asyncResults = {}
        for region in regionList:
            asyncResults[region] = pool.apply_async(doFit, (outDir,region,logDir,runFits)  )
#            regionalOutputs[region] = [0,0,0,0,0,0]
#            threads[region] = Thread(target = doFit, args = (outDir,region,regionalOutputs[region],logDir,) )
#            threads[region].start()
        for region in regionList:
            regionalOutputs[region] = asyncResults[region].get()

#        for key in threads.keys():
#            threads[key].join()
#            print regionalOutputs[key]

    print "Calculating error"
    for region in regionalOutputs.keys():
        print region, regionalOutputs[region]
        if calculateError: calculateError(regionalOutputs[region][0],regionalOutputs[region][1],regionalOutputs[region][2],regionalOutputs[region][3],regionalOutputs[region][4],regionalOutputs[region][5],region)
