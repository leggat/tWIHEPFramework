#The point of this script is to assemble the plots created in the skimmer program into stack plots and comparison plots (i.e. where they're all scaled to 1.)

from ROOT import *

from setTDRStyle import setTDRStyle
import sys, os

from array import array

import weightProcesses

inDir = sys.argv[1]
outDir = sys.argv[2]

barEndStr = ""
if "Barrel" in inDir: barEndStr = "Barrel"
if "Endcap" in inDir: barEndStr = "Endcap"

region = inDir[-5:-1]
noTagRegion = region[:2]+"0t"
noTagDir = inDir.split(region)[0]+noTagRegion+"/"
invMCDir = "tWInv"+inDir.split("tW")[1]
noTagData = noTagDir.split("tW")[0] + "tWData" + noTagDir.split("tW")[1]

onlyDoBDTVars = False

doData = False
dataFolder = ""

useQCD = False
qcdFolder = ""

makeCertainPlots = False
if "--makeCertainPlots" in sys.argv: makeCertainPlots = True

forceDataMax = False
if "--forceDataMax" in sys.argv: forceDataMax = True

isPrefit = False
if "--isPrefit" in sys.argv: isPrefit = True

use0TagRegion = False
#Use the 0 tag region as the QCD template
if "--useZeroTag" in sys.argv: use0TagRegion = True


#Subtract other MC sources from QCD background
subtractTemplatesFromQCD = True
if "--noSubtractTemplates" in sys.argv: subtractTemplatesFromQCD = False
normalisedSubtractionTemplates = True
if "--noNormaliseSubtract" in sys.argv: normalisedSubtractionTemplates = False


normaliseMCToData = False

#Make the weighting object
isEle = "Ele" in inDir
leptonInLabel = "#mu"
if isEle:
    leptonInLabel = "e"

weights = weightProcesses.ReweightObject(isPrefit,isEle)
qcdNoTagRegWeights = weightProcesses.ReweightObject(False,isEle) # Always use the post-fit for the DD background
#No weighting

if "--readJSONWeights" in sys.argv:
    ind = sys.argv.index("--readJSONWeights")
    weights.overrideWeightsFromJSON(sys.argv[ind+1])

onlyPlotsForFit = False
if "--plotsForFit" in sys.argv:
    onlyPlotsForFit = True

nBinsForPlots = 20.
nBinsForPlots = 40.

masterMCScale = 12554./27217. #runs B-D scale
masterMCScale = 1.
#masterMCScale = 0.1
#masterMCScale = 27217./35900.

ratioMin = 0.7
ratioMax = 1.3

if len(sys.argv) > 3: 
    if not sys.argv[3] == "--isPrefit":
        doData = True
        dataFolder = sys.argv[3]

if len(sys.argv) > 4:
    if not sys.argv[4] == "--isPrefit": 
        useQCD = True
        qcdFolder = sys.argv[4]

#Do a load of set up for the plots here

if not useQCD: subtractTemplatesFromQCD = False

def subtractMCTemplates(qcdTemplate,inFiles,plotName,samples):
    sumMCTemplate = qcdTemplate.Clone("totalMC")
    totalTotal = qcdTemplate.Clone("totalMC")
    sumMCTemplate.Reset()
    totalTotal.Reset()
    for sample in samples:
        if not inFiles[sample+"inv"].Get(plotName):continue
        tempHist = inFiles[sample+"inv"].Get(plotName)
        scaleFactor = weights.getDatasetWeight(sample,region+barEndStr)
        totalTotal.Add(tempHist,scaleFactor)
        if "qcd" in sample:continue
        sumMCTemplate.Add(tempHist,scaleFactor)
    if normalisedSubtractionTemplates and totalTotal.Integral() > 0:
        sumMCTemplate.Scale(qcdTemplate.Integral()/totalTotal.Integral())
    qcdTemplate.Add(sumMCTemplate,-1)
    for i in range(1,qcdTemplate.GetXaxis().GetNbins()+1):                  
        if qcdTemplate.GetBinContent(i) < 0: qcdTemplate.SetBinContent(i,0)
    return qcdTemplate
    

def getEleQCDTemplate(inFiles,plotName,samples):
    #We want to grab the data plot for this and then subtract the total MC from that region
    zeroTagRegions = {"3j1t":"3j0t","3j2t":"3j0t","2j1t":"2j0t","4j1t":"4j0t","4j2t":"4j0t"}
    qcdTemplate = inFiles["qcdData0tag"].Get(plotName).Clone(plotName+"qcd")
    qcdTemplate.SetDirectory(0)
    #Now we make a sum hist of all the MCs
    totalBkg = qcdTemplate.Clone("totBkg")
    totalBkg.Reset()
    for sample in samples:
        if "qcd" in sample:continue
        scaleFactor = qcdNoTagRegWeights.getDatasetWeight(histoGramPerSample[sample],zeroTagRegions[region])
#        print sample,totalBkg.Integral(),
        totalBkg.Add(inFiles[sample+"0tag"].Get(plotName),scaleFactor)
#        print scaleFactor,inFiles[sample+"0tag"].Get(plotName).Integral(),totalBkg.Integral()
#    print qcdTemplate.Integral(),
    totalBkg.Sumw2()
    qcdTemplate.Sumw2()
    qcdTemplate.Add(totalBkg,-1)
    for i in range(1,qcdTemplate.GetXaxis().GetNbins()+1):
        if qcdTemplate.GetBinContent(i) == 0: qcdTemplate.SetBinContent(i,0) #Negative bins mess up everything.
#    print qcdTemplate.Integral()
    return qcdTemplate

def getErrorPlot(totalMC,systUp,systDown,isRatio = False):
    x = array('d',[])
    y = array('d',[])
    exl = array('d',[])
    exh = array('d',[])
    eyl = array('d',[])
    eyh = array('d',[])
    xAxis = totalMC.GetXaxis()
    for i in range(1,xAxis.GetNbins()+1):
        x.append(xAxis.GetBinCenter(i))
        if not isRatio: y.append(totalMC.GetBinContent(i))
        else: y.append(1.)
        exl.append(xAxis.GetBinCenter(i) - xAxis.GetBinLowEdge(i))
        exh.append(xAxis.GetBinLowEdge(i)+xAxis.GetBinWidth(i)-xAxis.GetBinCenter(i))
        if not isRatio:
            eyl.append(systDown[i-1])
            eyh.append(systUp[i-1])
        else:
            if totalMC.GetBinContent(i) == 0:
                eyl.append(0.)
                eyh.append(0.)
                continue
            eyl.append(systDown[i-1]/totalMC.GetBinContent(i))
            eyh.append(systUp[i-1]/totalMC.GetBinContent(i))
    errors = TGraphAsymmErrors(xAxis.GetNbins(),x,y,exl,exh,eyl,eyh)
    return errors

gROOT.SetBatch()

gStyle.SetOptTitle(0)

gStyle.SetPalette(1)
gStyle.SetCanvasBorderMode(0)
gStyle.SetCanvasColor(kWhite)
gStyle.SetCanvasDefH(600)
gStyle.SetCanvasDefW(600)
gStyle.SetLabelFont(18,"")

setTDRStyle()

gStyle.SetTitleYOffset(0.8)

cmsTextFont = 61
extraTextFont = 52

#New labels here                                                                                                                                                        
latex = TLatex()
latex.SetNDC()
latex.SetTextAlign(31)

cmsLabelText = "CMS"
extraText = "Preliminary"

latex2 = TLatex();
latex2.SetNDC();
latex2.SetTextSize(0.04);
latex2.SetTextAlign(31);

#text2 = TLatex(0.45,0.98, channelLabelMap[channel]+postFixLabel2)
text2 = TLatex(0.45,0.98, "{1} channel, {0}".format(region,leptonInLabel))
text2.SetNDC()
text2.SetTextAlign(13)
text2.SetX(0.18)
text2.SetY(0.92)
text2.SetTextFont(42)
text2.SetTextSize(0.0610687)


#samples = ["tW_top","tW_antitop","tChan","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","wPlusJets","ttbar","qcd700_1000","qcd500_700","qcd300_500","qcd200_300","qcd2000_inf","qcd1500_2000","qcd100_200","qcd1000_1500"]
samples = ["tW_top","tW_antitop","tChan","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","wPlusJetsMCatNLO","ttbar","qcd700_1000","qcd500_700","qcd300_500","qcd200_300","qcd2000_inf","qcd1500_2000","qcd100_200","qcd1000_1500"]
samples = ["tW_top","tW_antitop","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","wPlusJetsMCatNLO","ttbar","qcd700_1000","qcd500_700","qcd300_500","qcd200_300","qcd2000_inf","qcd1500_2000","qcd100_200","qcd1000_1500"]
samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","wPlusJetsMCatNLO","ttbar","qcd700_1000","qcd500_700","qcd300_500","qcd200_300","qcd2000_inf","qcd1500_2000","qcd100_200","qcd1000_1500","wPlusJetsMadgraph"]
samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","ttbar","qcd700_1000","qcd500_700","qcd300_500","qcd200_300","qcd2000_inf","qcd1500_2000","qcd100_200","qcd1000_1500","wPlusJetsMCatNLO","ttbarBU"]
samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","ttbar","qcd700_1000","qcd500_700","qcd300_500","qcd200_300","qcd2000_inf","qcd1500_2000","qcd100_200","qcd1000_1500","ttbarBU","wPlus0Jets","wPlus1Jets","wPlus2Jets"]
#samples = ["tW_top","tW_antitop","tChan","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","wPlusJetsMCatNLO","ttbar","qcd700_1000","qcd500_700","qcd300_500","qcd200_300","qcd2000_inf","qcd1500_2000","qcd100_200","qcd1000_1500"]
#samples = ["tW_top","tW_antitop","tChan","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","wPlusJets","ttbar","qcd700_1000","qcd500_700","qcd300_500","qcd200_300","qcd2000_inf","qcd1500_2000","qcd100_200"]
#samples = ["tW_top","tW_antitop","tChan","zz","wz","ww","ttbar","qcd700_1000","qcd500_700","qcd300_500","qcd200_300","qcd2000_inf","qcd1500_2000","qcd100_200","qcd1000_1500"]
histsGlobal = ["singleTop","VV","ttbar","wPlusJets","zPlusJets","qcd","tW"]
#hists = ["tW","singleTop","VV","ttbar","qcd"]

histNameInLegend = {
"singleTop":"Single top","VV":"VV","ttbar":"t#bar{t}","wPlusJets":"W+Jets","zPlusJets":"Z+jets","qcd":"QCD","tW":"tW"
}

histoGramPerSample = {"tW_top":"tW","tW_antitop":"tW","sChan":"singleTop","tChan":"singleTop","zz":"VV","zPlusJetsLowMass":"zPlusJets","zPlusJetsHighMass":"zPlusJets","wz":"VV","ww":"VV","wPlusJets":"wPlusJets","ttbar":"ttbar","qcd700_1000":"qcd","qcd500_700":"qcd","qcd300_500":"qcd","qcd200_300":"qcd","qcd2000_inf":"qcd","qcd1500_2000":"qcd","qcd100_200":"qcd","qcd1000_1500":"qcd","wPlusJetsMCatNLO":"wPlusJets","tChan_top":"singleTop","tChan_antitop":"singleTop","ttbarBU":"ttbar","tW_top_nfh":"tW","tW_antitop_nfh":"tW","wPlusJetsMadgraph":"wPlusJets","wPlus0Jets":"wPlusJets","wPlus1Jets":"wPlusJets","wPlus2Jets":"wPlusJets"}
colourPerSample = {"tW_top":kGreen+2,"tW_antitop":kGreen+2,"tW_top_nfh":kGreen+2,"tW_antitop_nfh":kGreen+2,"tChan":kYellow,"zPlusJetsLowMass":kBlue,"zPlusJetsHighMass":kBlue,"wz":kPink,"ww":kPink,"zz":kPink,"wPlusJets":kTeal,"ttbar":kRed,"qcd700_1000":kGray,"qcd500_700":kGray,"qcd300_500":kGray,"qcd200_300":kGray,"qcd2000_inf":kGray,"qcd1500_2000":kGray,"qcd100_200":kGray,"qcd1000_1500":kGray,"sChan":kOrange,"VV":kPink,"qcd":kGray,"tW":kGreen+2,"zPlusJets":kBlue,"singleTop":kYellow}

reducedHists = ["tW","ttbar","wPlusJets"]
reducedHists2 = ["zPlusJets","qcd"]
#reducedHists = ["tW","ttbar"]

xAxisLabels = {
"BDTVars_lightJet1CSV":"CSV_{light jet 1}",
"BDTVars_M_DeltaRBJetLepton":"#DeltaR_{b jet,lepton}",
"BDTVars_M_DeltaRLeptonJet1":"#DeltaR_{lepton,jet_{1}}",
"BDTVars_M_DeltaRlightjets":"#DeltaR_{light jets}",
"BDTVars_M_hadronicWmass":"m_{W_{hadronic}}",
"BDTVars_M_Mass_Jet1Jet2Jet3LeptonMET":"m_{j_{1,2,3},lepton,MET}",
"BDTVars_M_nJet2040":"N_{Jets:20<p_{T}<40}",
"BDTVars_M_Pt_AllJetsLeptonMET":"p_{T}_{j_{all},lepton,MET}",
"BDTVars_M_Pt_Lepton":"p_{T,lepton}",
"BDTVars_M_topMass2_lep":"m_{top_{leptonic}}",
"M_DeltaPhiJet1Jet2":"#Delta#Phi_{j_{1,2}}",
"M_DeltaRJet1Jet2":"#DeltaR_{j_{1,2}}",
"M_DeltaRWlvJet2":"#DeltaR_{W_{lv}j_{2}}",
"M_E_AllJets":"E_{j_{all}}",
"M_E_Jet1Jet2Jet3MET":"E_{j_{1,2,3}MET}",
"M_E_Jet2Jet3":"E_{j_{2,3}}",
"M_HT":"H_{T}",
"M_Mass_AllJets":"m_{j_{all}}",
"M_Mass_AllJetsMET":"m_{j_{all}MET}",
"M_Mass_Jet1Jet2":"m_{j_{1,2}}",
"M_Mass_Jet1Jet3":"m_{j_{1,3}}",
"M_Pt_AllJets2040":"p_{T,j_{20-40}}",
"M_Pt_Jet1LeptonMET":"p_{T,j_{1}lMET}",
"M_Pt_Jet1Jet2Lepton":"p_{T,j_{1,2}l}",
"M_Pt_LeptonJet1":"p_{T,l,j_{1}}",
"M_Pt_Jet1_2030":"p_{T,j_{1,20-30}}",
#"HistogrammingMtW_mTW":"m_{T,W}",
#"HistogrammingMET_MET":"E_{T}^{miss}",
#"HistogrammingMET_METPhi":"#Phi_{E_{T}^{miss}}",
#"HistogrammingMET_MEX":"E_{X}^{miss}",
#"HistogrammingMET_MEY":"E_{Y}^{miss}",
}

inFiles = {}

plotNames = []

for sample in samples:
    inFiles[sample] = TFile(inDir+sample+"/hists/merged"+sample+".root","READ")
    
if doData: 
    if os.path.exists(dataFolder+"/singleMu/hists/mergedsingleMu.root"): inFiles['data'] = TFile(dataFolder+"/singleMu/hists/mergedsingleMu.root","READ")
    else: inFiles['data'] = TFile(dataFolder+"/singleEle/hists/mergedsingleEle.root","READ") 

if useQCD: 
    if os.path.exists(qcdFolder+"/singleMu/hists/mergedsingleMu.root"): inFiles['qcdData'] = TFile(qcdFolder+"/singleMu/hists/mergedsingleMu.root","READ")
    else: inFiles['qcdData'] = TFile(qcdFolder+"/singleEle/hists/mergedsingleEle.root","READ") 

if subtractTemplatesFromQCD:
    for sample in samples:
        if os.path.exists(invMCDir+sample+"/hists/merged"+sample+".root"):
            inFiles[sample+"inv"] = TFile(invMCDir+sample+"/hists/merged"+sample+".root","READ")
        else:
            print "No inverse MC files found"
            subtractTemplatesFromQCD = False
            break

if use0TagRegion:
    print "Sorts out the infile"
    for sample in samples:
        inFiles[sample + "0tag"] = TFile(noTagDir+sample+"/hists/merged"+sample+".root","READ")
    if os.path.exists(noTagData+"/singleMu/hists/mergedsingleMu.root"): inFiles['qcdData0tag'] = TFile(noTagData+"/singleMu/hists/mergedsingleMu.root","READ")
    else: inFiles['qcdData0tag'] = TFile(noTagData+"/singleEle/hists/mergedsingleEle.root","READ") 
    

plotPaths= []

#print inFiles.keys()

for obj in inFiles["tW_top_nfh"].GetListOfKeys():
    tempThing = inFiles["tW_top_nfh"].Get(obj.GetName())
    print obj.GetName()
    if not tempThing.ClassName().find("TH1") == -1 : plotPaths.append(tempThing.GetName())
    if not tempThing.ClassName().find("Directory") == -1:
        for k2 in tempThing.GetListOfKeys():
            temp2 = tempThing.Get(k2.GetName())
            if not temp2.ClassName().find("TH1") == -1:
                plotPaths.append(tempThing.GetName() + "/"+ temp2.GetName())
                
print
#print plotPaths

#ignorePlots = ["00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23"]
ignorePlots = ["01","02","03","04","05","10","23"]
ignorePlots = ["01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","24","26"]
#ignorePlots = ["01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","20","23"]
ignorePlots = ["00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","19","20","23","24","25","26"]
ignorePlots = ["00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","19","20","25","26"]
ignorePlots = ["00","03","04","06","08","09","16","18","19"]
ignorePlots = ["01","02","05","07","10","11","12","13","14","15","17","20"]
ignorePlots = ["01_","02_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","19_","20_"]
#ignorePlots = ["00_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","15_","16_","17_","18_","19_","20_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","15_","16_","18_","19_","20_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","15_","16_","18_","19_","20_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","13_","18_","19_","20_","21_"]
#ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","13_","14_","18_","19_","20_","21_","22_"]
#ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","12_","13_","17_","18_","19_","20_","21_","22_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","10_","11_","15_","16_","17_","18_","19_","20_","21_","22_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","10_","11_","15_","16_","17_","18_","19_","20_","21_","22_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","11_","12_","13_","18_","20_","21_","22_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","11_","12_","13_","15_","18_","19_","20_","21_","22_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","13_","15_","19_","20_","21_","22_","23_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","13_","15_","18_","22_","23_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","15_","16_","17_","18_","19_","20_","21_","22_","23_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","13_","15_","18_","22_","23_"]
#ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","15_","16_","17_","18_","19_","20_","21_","22_","23_","24_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","15_","16_","17_","18_","19_","20_","22_","23_","24_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","15_","19_","20_","21_","22_","23_","24_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","14_","19_","20_","21_","22_","23_","24_"]
ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","19_","20_","21_","22_","23_","24_"]
if makeCertainPlots: ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","15_","16_","17_","18_","19_","21_","22_","23_","24_"]
#ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","15_","16_","17_","18_","19_","21_","22_","23_","24_"]
#if onlyPlotsForFit: ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","17_","18_","19_","20_","21_","22_","23_","24_"]
if onlyPlotsForFit: ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","12_","13_","14_","15_","17_","18_","19_","20_","21_","22_","23_","24_"]

ignorePlots += ["_Jet5","_Jet6","_Jet7"]
ignorePlots += ["CutEff"]

makeCertainPlotsList = ["2040","2030","3040","4000"]
makeCertainPlotsList = ["hadronicWmass","Mass_Jet1Jet3","DeltaRlightjets","DeltaRWlvJet2","Pt_Lepton","E_Jet2Jet3","DeltaRBJetLepton","Pt_AllJetsLeptonMET","nJet2040"]

plotsNotToLatex = []


for plotName in plotPaths:
#    break
    doPlot = True
    for ignore in ignorePlots:
        if ignore in plotName:
            doPlot = False
            break
    skipIfOnlyDoingBDT = True

#    hists.reverse()
    #now we look at the make certain plots list if we're making certain plots
    if makeCertainPlots:
        doPlot = False
        for checkStr in makeCertainPlotsList:
            if checkStr in plotName:
                doPlot = True
                break
    if not doPlot: continue
    saveName = plotName
    if not plotName.find("/") == -1:
        saveName = ""
        for part in plotName.split("/"):
            saveName += part + "_"
        saveName = saveName[:-1]
        print saveName
#for plotName in inFiles["tW_top"].GetListOfKeys():
    for labelPlotName in xAxisLabels:
        if labelPlotName in saveName:
            skipIfOnlyDoingBDT = False
    print plotName
    #If we're only doing the BDT variables, we can just skip to the next one here if it isn't that.
    if onlyDoBDTVars and skipIfOnlyDoingBDT: continue

#    if not plotName.GetName().split("_")[0] in plotNames: plotNames.append( plotName.GetName().split("_")[0] )
    
    leggy = TLegend(0.8,0.6,0.95,0.9)
    leggy.SetFillStyle(1001)
    leggy.SetBorderSize(1)
    leggy.SetFillColor(0)
    leggy.SetLineColor(0)
    leggy.SetShadowColor(0)
    leggy.SetFillColor(kWhite)

    histMap = {}

    for sample in samples:
        #print sample
#        if "ttbar" in sample:
#            print sample, inFiles[sample], inFiles[sample].Get(plotName), histoGramPerSample[sample]
        if histoGramPerSample[sample] in histMap.keys():
            #           if not inFiles[sample].Get(plotName): continue
#            print sample, plotName, inFiles[sample].Get(plotName)
            if not inFiles[sample].Get(plotName): continue
            histMap[histoGramPerSample[sample]].Add(inFiles[sample].Get(plotName))
        else:
            if not inFiles[sample].Get(plotName): continue
            histMap[histoGramPerSample[sample]] = inFiles[sample].Get(plotName)
            histMap[histoGramPerSample[sample]].SetName(histMap[histoGramPerSample[sample]].GetName()+histoGramPerSample[sample])

    qcdNormalisation = histMap['qcd'].Integral()    
    if useQCD:
        if inFiles['qcdData'].Get(plotName):
            histMap['qcd'] = inFiles['qcdData'].Get(plotName)
        elif "Tight" in plotName and inFiles['qcdData'].Get(plotName.split("Tight")[0]+"UnIsolated"+plotName.split("Tight")[1]):
            print "Using alternate named data-driven QCD estimation plot"
            histMap['qcd'] = inFiles['qcdData'].Get(plotName.split("Tight")[0]+"UnIsolated"+plotName.split("Tight")[1])
        elif inFiles['qcdData'].Get(str(int(plotName.split("_")[0])+1).zfill(2)+plotName.split(plotName.split("_")[0])[-1]):
            histMap['qcd'] = inFiles['qcdData'].Get(str(int(plotName.split("_")[0])+1).zfill(2)+plotName.split(plotName.split("_")[0])[-1])
        elif "Tight" in plotName and inFiles['qcdData'].Get(str(int(plotName.split("_")[0])+1).zfill(2)+plotName.split(plotName.split("_")[0])[-1].split("Tight")[0]+"UnIsolated"+plotName.split(plotName.split("_")[0])[-1].split("Tight")[1]):
            histMap['qcd'] = inFiles['qcdData'].Get(str(int(plotName.split("_")[0])+1).zfill(2)+plotName.split(plotName.split("_")[0])[-1].split("Tight")[0]+"UnIsolated"+plotName.split(plotName.split("_")[0])[-1].split("Tight")[1])
        else:
            print "Couldn't find "+plotName+" in the data enriched QCD sample so skipping this histogram."
            plotsNotToLatex.append(saveName)
            continue
#        if histMap['qcd'].Integral() > 0.: #we now no longer do that here, as it leads to weird changes in normalisation where there shouldn't be any.
#            histMap['qcd'].Scale(qcdNormalisation/histMap['qcd'].Integral())
        #Electron channel using the 0tag region
        histMap['qcd'].SetName(histMap['qcd'].GetName() + "qcd")
        if subtractTemplatesFromQCD: 
            histMap['qcd'] = subtractMCTemplates(histMap['qcd'],inFiles,plotName,samples)
#        print histMap['qcd'].Integral()
        #If I need to do any scaling I will do it here I suppose.
#        print "Doing scaling:", histMap['qcd'].Integral(),
#        histMap['qcd'].Scale(0.5)
#        print histMap['qcd'].Integral()

    if use0TagRegion:
        histMap['qcd'] = getEleQCDTemplate(inFiles,plotName,samples)
        if histMap['qcd'].Integral() > 0:
            histMap['qcd'].Scale(qcdNormalisation/histMap['qcd'].Integral())
        
    mcstack = THStack(plotName,plotName)


#    for sample in hists:
#        tempHist = inFiles[sample].Get(plotName.GetName().split("_")[0]+"_"+sample+"_"+plotName.GetName().split("_")[-1]).Clone(histoGramPerSample[sample])
##        print plotName.GetName().split("_")[0]+"_"+sample+"_"+plotName.GetName().split("_")[-1]
#        leggy.AddEntry(tempHist,sample,"f")
#        tempHist.SetFillColor(colourPerSample[sample])
#        tempHist.SetLineColor(kBlack)
#        tempHist.SetLineWidth(1)
#        mcstack.Add(tempHist)
    canvy = TCanvas(plotName,plotName,1000,800)
    #Add in a ratio plot if running with data included
    if doData:
        canvy.SetBottomMargin(0.3)

    canvy.cd()
#    SetOwnership(mcstack,False)

    dataHist = 0

    if doData:
        if inFiles['data'].Get(plotName):
            dataHist = inFiles['data'].Get(plotName)
        elif inFiles['data'].Get(str(int(plotName.split("_")[0])+1).zfill(2)+plotName.split(plotName.split("_")[0])[-1]):
            dataHist = inFiles['data'].Get(str(int(plotName.split("_")[0])+1).zfill(2)+plotName.split(plotName.split("_")[0])[-1])            
        else:
            print plotName, str(int(plotName.split("_")[0])+1).zfill(2)+plotName.split(plotName.split("_")[0])[-1]
        print "Doing data"
        dataHist.SetName(dataHist.GetName()+"data")
        dataHist.SetMarkerStyle(20)
        dataHist.SetMarkerSize(1.2)
        dataHist.SetMarkerColor(kBlack)
        if dataHist.GetXaxis().GetNbins() > int(nBinsForPlots):
            rebin = int(dataHist.GetXaxis().GetNbins() / nBinsForPlots)
            dataHist.Rebin(rebin)
#        if "MET" in plotName: dataHist.Rebin(4)
        leggy.AddEntry(dataHist,"Data","p")

#    print histMap.keys()
    hists = histsGlobal
    legendOrder = hists
#    legendOrder.reverse()
    for histName in legendOrder[::-1]:
        leggy.AddEntry(histMap[histName],histNameInLegend[histName],"f")

    for histName in hists:
        histMap[histName].SetFillColor(colourPerSample[histName])
        histMap[histName].SetLineColor(kBlack)
        histMap[histName].SetLineWidth(1)
        #This is the master rescaling - we do this to account for incorrect lumi calculations or whatever. Change the master variable at the top of the code
        histMap[histName].Scale(masterMCScale)
        #Now do per MC scaling if there's a SF in there somewhere
        scaleFactor = weights.getDatasetWeight(histName,region+barEndStr)
#        if not scaleFactor == 1.: print scaleFactor
#        print histName,scaleFactor, histMap[histName].Integral(),
        histMap[histName].Scale(scaleFactor)
#        print histMap[histName].Integral()
#        if histName in perMCSFs.keys():
#            print histName, "rescaling",perMCSFs[histName]
#            histMap[histName].Scale(perMCSFs[histName])
#            print histName, perMCSFs[histName]
#        if "2j1t" in inDir:
#            if histName in perRegMCSFs["2j1t"].keys():
#                histMap[histName].Scale(perRegMCSFs["2j1t"][histName])
#        elif "3j2t" in inDir:
#            if histName in perRegMCSFs["3j2t"].keys():
#                histMap[histName].Scale(perRegMCSFs["3j2t"][histName])
#        else:
#            if histName in perRegMCSFs[""].keys():
#                histMap[histName].Scale(perRegMCSFs[""][histName])
        # Do rebinning here. Most of these plots have too many bins.
        if histMap[histName].GetXaxis().GetNbins() > int(nBinsForPlots):
            rebin = int(histMap[histName].GetXaxis().GetNbins() / nBinsForPlots)
            histMap[histName].Rebin(rebin)
        #Now grab the systematic variations per sample

#        if "MET" in plotName: histMap[histName].Rebin(4)

#    hists.reverse()

    #Make the sum histogram for the ratio plot
    sumHistoMC = histMap[hists[0]].Clone()
    errorList = [0] * histMap[hists[0]].GetXaxis().GetNbins()
    binErrorHist = histMap[hists[0]].Clone()
    sumHistoMC.Reset()
    binErrorHist.Reset()

#    totalCount = 0.

    for histName in hists:
        mcstack.Add(histMap[histName])
        for binN in range(1,histMap[histName].GetXaxis().GetNbins()+1):
            errorList[binN -1] += histMap[histName].GetBinError(binN)
            binErrorHist.SetBinContent(binN,binErrorHist.GetBinContent(binN)+histMap[histName].GetBinContent(binN))
#        totalCount += histMap[histName].GetBinContent(4)
#        print histName, histMap[histName].GetBinContent(4)
#        print histName,histMap[histName].Integral()
        sumHistoMC.Add(histMap[histName])

    if normaliseMCToData and sumHistoMC.Integral() > 0.: #This is a thing we#re only gonna do to test the inverted region of the electron.
        for histName in hists:
            histMap[histName].Scale(dataHist.Integral()/sumHistoMC.Integral())
        
#    print "Total MC Count: ",totalCount, 
#    for binN in range(1,histMap[hists[0]].GetXaxis().GetNbins()):
#        sumHistoMC.SetBinError(binN,binErrorHist.GetBinContent(binN))
#        print "sumHisto: {0} error: {1}".format(sumHistoMC.GetBinContent(binN),errorList[binN-1])

    errorGraph = getErrorPlot(sumHistoMC,errorList,errorList,False)
    errorGraph.SetFillStyle(3013)
    errorGraph.SetFillColor(12)


    sumHistoMC.SetFillColor(0)
    sumHistoMC.SetLineColor(0)

    maxi = mcstack.GetMaximum() #if not doData or dataHist.GetMaximum() < mcstack.GetMaximum() else dataHist.GetMaximum()
    if doData and (dataHist.GetMaximum() > mcstack.GetMaximum() or forceDataMax): maxi = dataHist.GetMaximum()

    mcstack.Draw("hist")
    errorGraph.Draw("same e2")
    leggy.AddEntry(errorGraph,"Stat. Unc.","f")
    if doData: mcstack.GetXaxis().SetLabelSize(0.0)

    mcstack.GetXaxis().SetTitle(histMap[hists[0]].GetXaxis().GetTitle())

    mcstack.GetYaxis().SetTitleOffset(1.)
    mcstack.GetYaxis().SetLabelSize(0.03)
    mcstack.GetYaxis().SetTitleSize(0.04)
    mcstack.GetYaxis().CenterTitle()
    
    mcstack.GetXaxis().SetTitleSize(0.045)
    mcstack.GetYaxis().SetTitle("Events")
    
    skipIfOnlyDoingBDT = True

    for labelPlotName in xAxisLabels:
        if labelPlotName in saveName:
            mcstack.GetXaxis().SetTitle(xAxisLabels[labelPlotName])
            skipIfOnlyDoingBDT = False

    #If we're only doing the BDT variables, we can just skip to the next one here if it isn't that.
    if onlyDoBDTVars and skipIfOnlyDoingBDT: continue
#    hists.reverse()

    mcstack.SetMaximum(maxi*1.3)
    minim = mcstack.GetMinimum()
#    mcstack.SetMinimum(0.0001)

 
    if doData: dataHist.Draw("e x0, same")

    leggy.Draw()

    latex.SetTextSize(0.04)
    latex.SetTextFont(cmsTextFont)
    latex.DrawLatex(0.23, 0.95, cmsLabelText )
    
    latex.SetTextFont(extraTextFont)
    latex.SetTextSize(0.04*0.76)
    latex.DrawLatex(0.35, 0.95 , extraText )
    
    latex2.DrawLatex(0.95, 0.95, "35.9 fb^{-1} (13TeV)");
    
    text2.Draw()
    

    #If we're running on data, add in the ratio plot here
    ratioCanvy = 0
    sumHistoData = 0
    if doData:
        mcstack.GetXaxis().SetTitle("")
        ratioCanvy = TPad(plotName+"_ratio",plotName+"_ratio",0.0,0.0,1.0,1.0)
        ratioCanvy.SetTopMargin(0.7)
        ratioCanvy.SetFillColor(0)
        ratioCanvy.SetFillStyle(0)
        ratioCanvy.SetGridy(1)
        ratioCanvy.Draw()
        ratioCanvy.cd(0)
        SetOwnership(ratioCanvy,False)

        sumHistoData = dataHist.Clone(dataHist.GetName()+"_ratio")
        sumHistoData.Sumw2()
        sumHistoData.Divide(sumHistoMC)

        sumHistoData.GetYaxis().SetTitle("Data/MC")
        sumHistoData.GetYaxis().SetTitleOffset(1.3)
        ratioCanvy.cd()
        SetOwnership(sumHistoData,False)
        sumHistoData.SetMinimum(ratioMin)
        sumHistoData.SetMaximum(ratioMax)
        sumHistoData.GetXaxis().SetTitleOffset(1.2)
        sumHistoData.GetXaxis().SetLabelSize(0.04)
        sumHistoData.GetYaxis().SetNdivisions(6)
        sumHistoData.GetYaxis().SetTitleSize(0.03)
        sumHistoData.Draw("E1X0")
        errorPlotRatio = getErrorPlot(sumHistoMC,errorList,errorList,True)
        errorPlotRatio.SetFillStyle(3013)
        errorPlotRatio.SetFillColor(12)  
        errorPlotRatio.Draw("e2 same")
        for labelPlotName in xAxisLabels:                              
            if labelPlotName in saveName:                              
                sumHistoData.GetXaxis().SetTitle(xAxisLabels[labelPlotName])



    canvy.SaveAs(outDir+saveName+".png")
    canvy.SaveAs(outDir+saveName+".root")

#    mcstack.SetMinimum(1)
    canvy.SetLogy()

#    print "after setlogy"

#    if canvy: 
#        print "Canvy okay"
#        print canvy.GetName()
#        canvy.Write()

    canvy.SaveAs(outDir+saveName+"_log.png")
    canvy.SaveAs(outDir+saveName+"_log.root")

    del ratioCanvy
    del sumHistoData
#    del mcstack 
#    del canvy
#    del ratioCanvy
#    del sumHistoMC
#    del sumHistoData
#    del dataHist

    compCanvy = TCanvas(plotName+"comp",plotName+"comp",1000,800)
    compCanvy.cd()


#    hists.reverse()
    histMax = 0.
    for histName in hists:
        histMap[histName].SetFillColor(0)
        histMap[histName].SetLineWidth(2)
        histMap[histName].SetLineColor(colourPerSample[histName])
        if histMap[histName].Integral() > 0.:
            histMap[histName].Scale(1./histMap[histName].Integral())
        histMap[histName].Draw("hist same")
        if histMap[histName].GetMaximum() > histMax: histMax = histMap[histName].GetMaximum()

    previousMax = histMap["tW"].GetMaximum()
    histMap["tW"].SetMaximum(histMax * 1.3)
    leggy.Draw()

    compCanvy.SaveAs(outDir+saveName+"comp.png")
    compCanvy.SaveAs(outDir+saveName+"comp.root")

    reducedCanvy =  TCanvas(plotName+"reducedComp",plotName+"reducedComp",1000,800)
    reducedCanvy.cd()

    reducedLeggy = TLegend(0.7,0.7,0.94,0.94)
    histMap["tW"].SetMaximum(previousMax)
    histMax = 0.
    for hist in reducedHists:
        histMap[hist].Draw("hist same")
        if histMap[hist].GetMaximum() > histMax: histMax = histMap[hist].GetMaximum()
        reducedLeggy.AddEntry(histMap[hist],hist,'f')

    histMap["tW"].SetMaximum(histMax * 1.3)
    reducedLeggy.Draw()

    reducedCanvy.SaveAs(outDir+saveName+"reducedComp.png")
    reducedCanvy.SaveAs(outDir+saveName+"reducedComp.root")

    wJqcdCanvy =  TCanvas(plotName+"wJqcdComp",plotName+"wJqcdComp",1000,800)
    wJqcdCanvy.cd()

    wJqcdLeggy = TLegend(0.7,0.7,0.94,0.94)
    histMap["tW"].SetMaximum(previousMax)
    histMax = 0.
    for hist in reducedHists2:
        histMap[hist].Draw("hist same")
        if histMap[hist].GetMaximum() > histMax: histMax = histMap[hist].GetMaximum()
        wJqcdLeggy.AddEntry(histMap[hist],hist,'f')

    histMap["tW"].SetMaximum(histMax * 1.3)
    wJqcdLeggy.Draw()

    wJqcdCanvy.SaveAs(outDir+saveName+"wJqcdComp.png")
    wJqcdCanvy.SaveAs(outDir+saveName+"wJqcdComp.root")

    
    
stages = ["lepSel","jetSel","bTag","fullSel"]

latexFile = open(outDir+"combinedLatexFile.tex","w")
latexFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")

wJetQCDOutFile = open(outDir+"wJetQCDcomps.tex","w")
wJetQCDOutFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")

#print plotPaths

for plot in plotPaths:
    doPlot = True
    for ignore in ignorePlots:
        if ignore in plot:
            doPlot = False
            break

    if makeCertainPlots:                 
        doPlot = False                   
#        print makeCertainPlotsList
        for checkStr in makeCertainPlotsList:
            if checkStr in plot:     
#                print checkStr,plotName
                doPlot = True            
                break                    

    if not doPlot: continue

    if not  plot.find(" ") == -1: continue
    saveName = plot
    writtenName = ""
    if not plot.find("/") == -1:
        saveName = ""
        for part in plot.split("/"):
            saveName += part + "_"
        saveName = saveName[:-1]

    if saveName in plotsNotToLatex: continue
    skipIfOnlyDoingBDT = True

    for labelPlotName in xAxisLabels:
        if labelPlotName in saveName:
            skipIfOnlyDoingBDT = False

    #If we're only doing the BDT variables, we can just skip to the next one here if it isn't that.
    if onlyDoBDTVars and skipIfOnlyDoingBDT: continue
    for part in saveName.split("_"):
        writtenName += part + "\\_"
        writtenName = writtenName[:-2]

    latexFile.write("\\frame{\n\\frametitle{"+writtenName+"}\n")
    wJetQCDOutFile.write("\\frame{\n\\frametitle{"+writtenName+"}\n")

    nPlots = 0
    for log in ["","_log","comp","reducedComp"]:
        post = log
        if log == "_log": post = " log"
        latexFile.write("\\includegraphics[width=0.45\\textwidth]{"+saveName+log+".png}")
        if  nPlots == 1: latexFile.write(" \\\\")
        latexFile.write("\n")
        nPlots+=1
    latexFile.write("}\n")
    wJetQCDOutFile.write("\\includegraphics[width=0.9\\textwidth]{"+saveName+"wJqcdComp.png}\n}\n")


latexFile.write("\\end{document}")
wJetQCDOutFile.write("\\end{document}")
#latexFile.Close()

if len(plotsNotToLatex): print "Ignored: ", plotsNotToLatex



