from ROOT import *

import sys,os,math,weightProcesses,analysisComponents,subprocess,json

components = analysisComponents.AnalysisComponents()

jetShiftNames = components.jetShiftDict
translateNames = True

if "--help" in sys.argv or "-h" in sys.argv or "--options" in sys.argv or "-o" in sys.argv:
    print "Assemble BDT discriminants into one root file per region/channel for the fit"
    print "Usage: python createDatacardRootFile.py <inDir> <outDir> [options]"
    print "   Options:"
    print "      ele                       Do electron channel"
    print "      --usetWmcanlo             Use the mcanlo tW sample (for validation)"
    print "      --extendRegions           Include 0 tag regions"
    print "      --onlySigReg              Only run on 3j1t"
    print "      --noSplitBarrelEndcap     Don't run on both endcap and barrel. Running like this will be the default going forward"
    print "      --jsonWeights <jsonFile>  Use json weights for the weights"
    print "      --ignoreEndcap            Ignore the endcap in the hadd process"
    print "      --preRebin <nBins>        Do a rebinning here with nBins"
    print "      --jetPt2jReg              Use jet1pt instead of BDT in 2j1t region"
    sys.exit(1)

usetWmcanlo = False
if "--usetWmcanlo" in sys.argv: usetWmcanlo = True

histoGramPerSample = {"tW_top":"tW","tW_antitop":"tW","sChan":"singleTop","tChan":"singleTop","zz":"VV","zPlusJetsLowMass":"zPlusJets","zPlusJetsHighMass":"zPlusJets","wz":"VV","ww":"VV","wPlusJets":"wPlusJets","ttbar":"ttbar","qcd700_1000":"qcdMC","qcd500_700":"qcdMC","qcd300_500":"qcdMC","qcd200_300":"qcdMC","qcd2000_inf":"qcdMC","qcd1500_2000":"qcdMC","qcd100_200":"qcdMC","qcd1000_1500":"qcdMC","wPlusJetsMCatNLO":"wPlusJets","tChan_top":"singleTop","tChan_antitop":"singleTop","ttbarBU":"ttbar","tW_top_nfh":"tW","tW_antitop_nfh":"tW","wPlusJetsMadgraph":"wPlusJets","wPlus0Jets":"wPlusJets","wPlus1Jets":"wPlusJets","wPlus2Jets":"wPlusJets","tW_mcanlo":"tW"}

samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","wPlusJetsMCatNLO","ttbar","ttbarBU"]
#samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","wPlusJetsMadgraph","ttbar"]
samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","ttbar","ttbarBU","wPlus0Jets","wPlus1Jets","wPlus2Jets","qcd100_200","qcd200_300","qcd300_500","qcd500_700","qcd700_1000","qcd1000_1500","qcd1500_2000"]
if usetWmcanlo: samples = ["tW_mcanlo","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","ttbar","ttbarBU","wPlus0Jets","wPlus1Jets","wPlus2Jets","qcd100_200","qcd200_300","qcd300_500","qcd500_700","qcd700_1000","qcd1000_1500","qcd1500_2000"]

#samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","ttbar","ttbarBU","wPlus2Jets"]
#samples = ["tW_top_nfh"]

samplesDataMu = ["SingMuB","SingMuC","SingMuD","SingMuE","SingMuF","SingMuG","SingMuH"]
samplesDataEle = ["SingEleB","SingEleC","SingEleD","SingEleE","SingEleF","SingEleG","SingEleH"]
samplesData = samplesDataMu

colourPerSample = {"tW_top":kGreen+2,"tW_antitop":kGreen+2,"tW_top_nfh":kGreen+2,"tW_antitop_nfh":kGreen+2,"tChan":kYellow,"zPlusJetsLowMass":kBlue,"zPlusJetsHighMass":kBlue,"wz":kPink,"ww":kPink,"zz":kPink,"wPlusJets":kTeal,"ttbar":kRed,"qcd700_1000":kGray,"qcd500_700":kGray,"qcd300_500":kGray,"qcd200_300":kGray,"qcd2000_inf":kGray,"qcd1500_2000":kGray,"qcd100_200":kGray,"qcd1000_1500":kGray,"sChan":kOrange,"VV":kPink,"qcd":kGray,"tW":kGreen+2,"zPlusJets":kBlue,"singleTop":kYellow}

ignoreSystPlots = ["PS","herwig","amcatnlo"]

legendOrder = ["tW","wPlusJets","ttbar","qcd","VV","zPlusJets","singleTop"]

plotLeptonSampleName = "Mu"

makeCutAndCountTemplates = False #This will only ever run once anyway
def setMakeCutAndCount(bool):
    global makeCutAndCountTemplates
    makeCutAndCountTemplates = False

includeDataInStack = False

makeStatBins = False

includeJetVariations = True
nJetVariations = 54

setNegToZero = False

doDiffMVAName = False
if "--jetPt2jReg" in sys.argv: doDiffMVAName = True

gROOT.SetBatch()

from setTDRStyle import setTDRStyle

setTDRStyle()

latex = TLatex()
latex.SetNDC()
latex.SetTextAlign(31)

gStyle.SetTitleYOffset(0.8)
gStyle.SetOptTitle(0)

gStyle.SetPalette(1)
gStyle.SetCanvasBorderMode(0)
gStyle.SetCanvasColor(kWhite)
gStyle.SetCanvasDefH(600)
gStyle.SetCanvasDefW(600)
gStyle.SetLabelFont(18,"")

cmsTextFont = 61
extraTextFont = 52

ratioMax = 1.1
ratioMin = 0.9

latex2 = TLatex();
latex2.SetNDC();
latex2.SetTextSize(0.04);
latex2.SetTextAlign(31);



cmsText = "CMS"
extraText = "Preliminary"

makeSystComps = False #Make a whole bunch of comparison plots if true
makeStackPlots = True #Make some stack plots of the output
reduceBinsToFilled = False #Reduce the histograms to only their filled bins. This might possibly influence the fit, but my tests do not indicate that it does.

mvaNameDef = "MVA_ttbar_"
mvaPostfix = ""

inDir = sys.argv[1]
outDir = sys.argv[2]
doSystDir = True
systDir = inDir+"Systs/"
isEle = False
channeltr = "mu"
leptonType = "Mu"
leptonStringLabel = "#mu"
if "ele" in sys.argv:
    samplesData = samplesDataEle
    plotLeptonSampleName = "Ele"
    channeltr = "ele"
    leptonStringLabel = "e"
    isEle = True

#if len(sys.argv) > 3: systDir = sys.argv[3]

weights = weightProcesses.ReweightObject(False,isEle)
if "--jsonWeights" in sys.argv:
    ind = sys.argv.index("--jsonWeights")
    weights.overrideWeightsFromJSON(sys.argv[ind+1])

if (not os.path.isdir(outDir)):
    os.makedirs(outDir)
if (makeSystComps or makeStackPlots) and not os.path.isdir(outDir+"plots/"): os.makedirs(outDir+"plots/")

nominal = {}

systs = [
"LSF","PU","bTag","PDF","Trig","mistag"
]

nStatsBins = 0
systHists = {}

totalYieldsCount = {}

minMap = {"2j1t":-0.5}

def findMaxAndMinBins(nominalHists,systHist,nBins=0,region="3j1t"):
    maxBin = 0
    minBin = 1000
    returnNominals = {}
    returnSysts = {}
    for key in nominalHists.keys():
        if nominalHists[key].FindFirstBinAbove() < minBin: minBin = nominalHists[key].FindFirstBinAbove()
        if nominalHists[key].FindLastBinAbove() > maxBin: maxBin = nominalHists[key].FindLastBinAbove()
        if key == "data" or key == "qcd":continue
        for key2 in systHist[key].keys():
            if systHist[key][key2].FindFirstBinAbove() < minBin: minBin = systHist[key][key2].FindFirstBinAbove()
            if systHist[key][key2].FindLastBinAbove() > maxBin: maxBin = systHist[key][key2].FindLastBinAbove()
    xLow = nominalHists[key].GetXaxis().GetBinLowEdge(minBin)
    xHigh = nominalHists[key].GetXaxis().GetBinUpEdge(maxBin)
    expandLevel = 25.
    xLow = -0.8
    if "2j1t" in region: xLow = -0.5 #fudge this a litlte because I don't know why it isn't working properly.
    xHigh = 0.78
    if nBins == 0: nBins = maxBin-minBin+1
    for key in nominalHists.keys():
        returnNominals[key] = TH1F(nominalHists[key].GetName(),nominalHists[key].GetTitle(),nBins,xLow,xHigh)
        i = 1
        errorSqr = [0.]*(nBins)
        for j in range(minBin,maxBin+1):
            returnNominals[key].Fill(nominalHists[key].GetXaxis().GetBinCenter(j),nominalHists[key].GetBinContent(j))
            newBin = returnNominals[key].GetXaxis().FindBin(nominalHists[key].GetXaxis().GetBinCenter(j))
#            print newBin
            errorSqr[newBin-1] += nominalHists[key].GetBinError(j) * nominalHists[key].GetBinError(j)
#            returnNominals[key].SetBinContent(i,nominalHists[key].GetBinContent(j))
#            returnNominals[key].SetBinError(i,nominalHists[key].GetBinError(j))
#            i+=1
        for j in range(len(errorSqr)): 
            returnNominals[key].SetBinError(j+1,math.sqrt(errorSqr[j]))
        if key == "data" or key == "qcd":continue
        returnSysts[key] = {}
        for key2 in systHist[key].keys():
            returnSysts[key][key2] = TH1F(systHist[key][key2].GetName(),systHist[key][key2].GetTitle(),nBins,xLow,xHigh)
            i = 1
            errorSqr = [0.]*(nBins)
            for j in range(minBin,maxBin+1):
                returnSysts[key][key2].Fill(systHist[key][key2].GetXaxis().GetBinCenter(j),systHist[key][key2].GetBinContent(j))
                newBin = returnSysts[key][key2].GetXaxis().FindBin(systHist[key][key2].GetXaxis().GetBinCenter(j))
                errorSqr[newBin-1] += nominalHists[key].GetBinError(j) * nominalHists[key].GetBinError(j)
            for j in range(len(errorSqr)):
                returnSysts[key][key2].SetBinError(j+1,math.sqrt(errorSqr[j]))
#                returnSysts[key][key2].SetBinContent(i,systHist[key][key2].GetBinContent(j))
#                returnSysts[key][key2].SetBinError(i,systHist[key][key2].GetBinError(j))
#                i+=1
    print "Hists have been rescaled to fit range {0}-{1}".format(xLow,xHigh)
    return (returnNominals,returnSysts)

def setAllNegBinsToZero(hist):
    for i in range(1,hist.GetXaxis().GetNbins()+1):
        if hist.GetBinContent(i) < 0.: hist.SetBinContent(i,0.)

def makeAllSystHists(nominal,systHists,region,savePost=""):
    latexFile = open(outDir+"plots/{0}latexFile{1}.tex".format(region,savePost),"w")
    secondLatexFile = open(outDir+"plots/{0}latexFile{1}NotBeamer.tex".format(region,savePost),"w")
    latexFileSystsOnOne = open(outDir+"plots/{0}latexSystComp{1}.tex".format(region,savePost),"w")
    latexFileSystsOnOne.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")
    latexFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")
    for sample in nominal.keys():
        if sample == "data" or sample == "qcd" or sample == "qcdMC": continue
        for syst in systHists[sample].keys():
            doPlot = True
            for ignore in ignoreSystPlots:
                if ignore in syst: doPlot = False
            if not doPlot: continue
            if "Down" in syst or "stat" in syst or "down" in syst: continue
            downSystName = syst.split("Up")[0]+"Down"
            shortenedName = syst.split("Up")[0]
            if "up" in syst: downSystName = syst.split("up")[0]+"down"
            makeSystHist(nominal[sample],systHists[sample][syst],systHists[sample][downSystName],region+sample+shortenedName+savePost)
            latexFile.write("\\frame{{\n\\frametitle{{{0}-{1}-{2}}}\n".format(region,sample,shortenedName))
            latexFile.write("\\includegraphics[width=0.9\\textwidth]{"+region+sample+shortenedName+savePost+".png}")
            secondLatexFile.write("\\includegraphics[width=0.5\\textwidth]{"+region+sample+shortenedName+savePost+".png}\n")
            latexFile.write("\n}\n")
        #Now make one for the statistical uncertainty
        statHists = getStatUpDownHists(nominal[sample])
        makeSystHist(nominal[sample],statHists[0],statHists[1],region+sample+"stats"+savePost)
        latexFile.write("\\frame{{\n\\frametitle{{{0}-{1}-{2}}}\n".format(region,sample,"Stats"))
        latexFile.write("\\includegraphics[width=0.9\\textwidth]{"+region+sample+"stats"+savePost+".png}")
        secondLatexFile.write("\\includegraphics[width=0.5\\textwidth]{"+region+sample+"stats"+savePost+".png}\n")
        latexFile.write("\n}\n")

    latexFile.write("\\frame{{\n\\frametitle{{MVA {0}}}\n".format(region))
    latexFile.write("\\includegraphics[width=0.9\\textwidth]{{mva{0}{1}.png}}".format(region,savePost))
    latexFile.write("\n}\n")                                                                       

    #make a latex file with comparison plots for each sample on one slide for each syst
    for syst in systHists[nominal.keys()[0]]:
        if "Down" in syst or "stat" in syst or "down" in syst: continue
        downSystName = syst.split("Up")[0]+"Down"
        shortenedName = syst.split("Up")[0]
        latexFileSystsOnOne.write("\\frame{{\n\\frametitle{{{0}-{1}}}\n".format(region,shortenedName))
        nSamp = 0
        for sample in nominal.keys():
            if sample == "data" or sample == "qcd": continue
            if syst not in systHists[sample].keys(): continue
            for ignore in ignoreSystPlots:
                if ignore in syst: continue
            if "herwig" in syst or "PS" in syst: continue
            latexFileSystsOnOne.write("\\includegraphics[width=0.3\\textwidth]{"+region+sample+shortenedName+savePost+".png}\n")
            nSamp += 1
            if nSamp % 3 == 0: latexFileSystsOnOne.write("\\\\\n")
        latexFileSystsOnOne.write("\n}\n")


    latexFile.write("\\end{document}")
    latexFileSystsOnOne.write("\\end{document}")

def getStatUpDownHists(nominal):
    #get histograms with the bins altered up and down by the bin error
     statsUp = nominal.Clone(nominal.GetName()+"StatsUp")
     statsDown = nominal.Clone(nominal.GetName()+"StatsDown")
     for i in range(1,nominal.GetXaxis().GetNbins()+1):
         statsUp.SetBinContent(i,nominal.GetBinContent(i)+nominal.GetBinError(i))
         statsDown.SetBinContent(i,nominal.GetBinContent(i)-nominal.GetBinError(i))
     return statsUp,statsDown

def makeSystHist(nominalHist,upHist,downHist,canvasName):
#    canvasName = upHist.GetName().split("Up")[0]
    canvy = TCanvas(canvasName,canvasName,1000,800)
    canvy.cd()
    canvy.SetBottomMargin(0.3)
    nominalHist.SetLineColor(kBlack)
    histMax = 0.
    if upHist.GetMaximum() > histMax: histMax = upHist.GetMaximum() 
    if downHist.GetMaximum() > histMax: histMax = downHist.GetMaximum()
    nominalHist.SetMaximum(histMax*1.2)
    nominalHist.Draw("hist")
    upHist.SetLineColor(kRed)
    upHist.Draw("hist same")
    downHist.SetLineColor(kBlue)
    downHist.Draw("hist same")

    latex.SetTextSize(0.04)
    latex.SetTextFont(cmsTextFont)
    latex.DrawLatex(0.23, 0.95, cmsText )
    
    latex.SetTextFont(extraTextFont)
    latex.SetTextSize(0.04*0.76)
    latex.DrawLatex(0.35, 0.95 , extraText )
    
    latex2.DrawLatex(0.95, 0.95, canvasName);
    
    ratioCanvy = TPad("mva_ratio","mva_ratio",0.0,0.0,1.0,1.0)
    ratioCanvy.SetTopMargin(0.7)
    ratioCanvy.SetFillColor(0)
    ratioCanvy.SetFillStyle(0)
    ratioCanvy.SetGridy(1)
    ratioCanvy.Draw()
    ratioCanvy.cd(0)
    SetOwnership(ratioCanvy,False)
    

    
#    text2 = TLatex(0.45,0.98, "#mu channel " + canvasName)
#    text2.SetNDC()
#    text2.SetTextAlign(13)
#    text2.SetX(0.18)
#    text2.SetY(0.92)
#    text2.SetTextFont(42)
#    text2.SetTextSize(0.0610687)
#

    upHistRatio = upHist.Clone()
    upHistRatio.Divide(nominalHist)
    upHistRatio.SetMaximum(ratioMax)
    upHistRatio.SetMinimum(ratioMin)
    upHistRatio.Draw("hist same")
    downHistRatio = downHist.Clone()
    downHistRatio.Divide(nominalHist)
    downHistRatio.GetXaxis().SetTitle("BDT Discriminant")
    downHistRatio.Draw("hist same")

    canvy.SaveAs(outDir+"plots/"+canvasName+".png")

def makeStackPlot(nominal,systHists,region,savePost = ""):
    stack = THStack("mva_{0}".format(region),"mva_{0}".format(region))
    canvy = TCanvas("MVA_{0}".format(region),"MVA_{0}".format(region),1000,800)
    leggy = TLegend(0.8,0.6,0.95,0.9)
    leggy.SetFillStyle(1001)
    leggy.SetBorderSize(1)
    leggy.SetFillColor(0)
    leggy.SetLineColor(0)
    leggy.SetShadowColor(0)
    leggy.SetFillColor(kWhite)
    
    canvy.cd()
    if includeDataInStack: canvy.SetBottomMargin(0.3)
    dataHist = 0
    sumHist = nominal[nominal.keys()[0]].Clone()
    sumHist.Reset()
    for i in nominal.keys():
        if i == "qcdMC": continue
        if i == "data":
            dataHist = nominal["data"]
            dataHist.SetMarkerStyle(20)
            dataHist.SetMarkerSize(1.2)
            dataHist.SetMarkerColor(kBlack)
            continue
        nominal[i].SetFillColor(colourPerSample[i])
        nominal[i].SetLineColor(kBlack)
        nominal[i].SetLineWidth(1)
#        stack.Add(nominal[i])
        sumHist.Add(nominal[i])
        #Do systematic estimation here when I get aorund to it)

    if "data" in nominal.keys():
        leggy.AddEntry(nominal['data'],"Data","p")
    for entry in legendOrder:
        if entry not in nominal.keys(): continue
        leggy.AddEntry(nominal[entry],entry,"f")
        nominal[entry].GetXaxis().SetTitle("BDT discriminant")
        nominal[entry].GetYaxis().SetTitle("Events")
    legendOrder.reverse()
    for entry in legendOrder:
        if entry not in nominal.keys(): continue
        stack.Add(nominal[entry])


    legendOrder.reverse()
    maxi = stack.GetMaximum()
    if dataHist.GetMaximum() > stack.GetMaximum(): maxi = dataHist.GetMaximum()
    stack.SetMaximum(maxi * 1.2)
    stack.Draw("hist")
    if stack.GetXaxis(): stack.GetXaxis().SetTitle("BDT discriminant") 
    if stack.GetYaxis(): 
        stack.GetYaxis().SetTitle("Events")
        stack.GetYaxis().SetTitleOffset(1.4)
        stack.GetYaxis().SetLabelSize(0.04) 
        stack.GetYaxis().SetTitleSize(0.05) 
        stack.GetXaxis().SetLabelSize(0.04) 
        stack.GetXaxis().SetTitleSize(0.05) 
        
        
    if includeDataInStack: dataHist.Draw("e1x0 same")
    leggy.Draw()

    latex.SetTextSize(0.04)
    latex.SetTextFont(cmsTextFont)
    latex.DrawLatex(0.23, 0.95, cmsText )

    latex.SetTextFont(extraTextFont)
    latex.SetTextSize(0.04*0.76)
    latex.DrawLatex(0.35, 0.95 , extraText )

    latex2.DrawLatex(0.95, 0.95, "35.9 fb^{-1} (13TeV)");

    text2 = TLatex(0.45,0.98, "{1} channel, {0}".format(region,leptonStringLabel))
    text2.SetNDC()
    text2.SetTextAlign(13)
    text2.SetX(0.18)
    text2.SetY(0.92)
    text2.SetTextFont(42)
    text2.SetTextSize(0.0610687)

    text2.Draw()


    if includeDataInStack:
        ratioCanvy = TPad("mva_ratio","mva_ratio",0.0,0.0,1.0,1.0)
        ratioCanvy.SetTopMargin(0.7)
        ratioCanvy.SetFillColor(0)
        ratioCanvy.SetFillStyle(0)
        ratioCanvy.SetGridy(1)
        ratioCanvy.Draw()
        ratioCanvy.cd(0)
        SetOwnership(ratioCanvy,False)

        sumHistoData = dataHist.Clone(dataHist.GetName()+"_ratio")
        sumHistoData.Sumw2()
        sumHistoData.Divide(sumHist)

        sumHistoData.GetYaxis().SetTitle("Data/MC")
        sumHistoData.GetYaxis().SetTitleOffset(1.3)
        ratioCanvy.cd()
        SetOwnership(sumHistoData,False)
        sumHistoData.SetMinimum(0.8)
        sumHistoData.SetMaximum(1.2)
        sumHistoData.GetXaxis().SetTitleOffset(1.2)
        sumHistoData.GetXaxis().SetLabelSize(0.04)
        sumHistoData.GetYaxis().SetNdivisions(6)
        sumHistoData.GetYaxis().SetTitleSize(0.03)
        sumHistoData.Draw("E1X0")

    canvy.SaveAs(outDir+"plots/mva{0}{1}.png".format(region,savePost))
    canvy.SaveAs(outDir+"plots/mva{0}{1}.root".format(region,savePost))


def getDownHist(upHist,nominalHist):
    downHist = nominalHist.Clone(upHist.GetName().split("Up")[0]+"Down")
    downHist.SetDirectory(0)
    for i in range(1,nominalHist.GetXaxis().GetNbins()+1):
        diffBin = nominalHist.GetBinContent(i) - upHist.GetBinContent(i)
        downHist.SetBinContent(i,nominalHist.GetBinContent(i) + diffBin)
        if downHist.GetBinContent(i) < 0.: downHist.SetBinContent(i,0.)
    return downHist

def makeStatBinVariations(hist, binNumber, region):

    variationHistUp = hist.Clone("{2}_{2}_{3}{1}_statbin{0}Up".format(binNumber,region,hist.GetName(),plotLeptonSampleName))
    variationHistDown = hist.Clone("{2}_{2}_{3}{1}_statbin{0}Down".format(binNumber,region,hist.GetName(),plotLeptonSampleName))
    binWeight = hist.GetBinContent(binNumber)
    shift = 0

    shift = hist.GetBinError(binNumber)
    variationHistUp.SetBinContent(binNumber,binWeight+shift)
    variationHistDown.SetBinContent(binNumber,binWeight-shift)
    return (variationHistUp,variationHistDown)


def bookCCTemplatePlots(nomsCC,systCC,nBins,samples):
    for sample in (legendOrder+['data']):
        nomsCC[sample] = TH1F(sample,sample,nBins,0,nBins)
        nomsCC[sample].SetDirectory(0)
        systCC[sample] = {}

def fillCutAndCountPlots(nominal,systHists,nomCC,systCC,fillBin):
    for sample in nominal.keys():
        if sample == "qcdMC": continue
        nomCC[sample].SetBinContent(fillBin+1,nominal[sample].Integral())
        if sample == "data": continue
        for systName in systHists[sample].keys():
            #if we haven't made the syst hist yet, make it now.
            if systName not in systCC[sample].keys():
                systCC[sample][systName] = nomCC[sample].Clone("{0}_{1}".format(sample,systName))
                systCC[sample][systName].SetDirectory(0)
                systCC[sample][systName].Reset()
            systCC[sample][systName].SetBinContent(fillBin+1,systHists[sample][systName].Integral())

def makeDatacard(mvaNameOrig,regions,savePostfix="",ignoreEndcap=False,nBins=0):

    cutAndCountNom = {}
    cutAndCountSysts = {}
    if makeCutAndCountTemplates: bookCCTemplatePlots(cutAndCountNom,cutAndCountSysts,len(regions),samples)

    for regionIt in range(len(regions)):
        region = regions[regionIt]
        saveName = "3j1t"
        if not region == "": saveName = region
        mvaName = mvaNameDef + mvaNameOrig
        if "2j1t" in region and doDiffMVAName: mvaName = "jet1Pt_"
    
        
        outFile = TFile(outDir+"mvaDists_{0}_{1}{2}.root".format(saveName,channeltr,savePostfix),"RECREATE")

        totalYieldsCount[region] = {}
        nominal = {}
        invNominal = {}
        systHists = {}
        if doSystDir: systDir = inDir + "Systs" + region + "/"
        for sample in samples:
            #get input file
            inFile = TFile(inDir+region+"/output_"+sample+".root","READ")
            inFileInv = False
            if os.path.exists(inDir+"QCD"+region+"/output_"+sample+".root"):
                inFileInv = TFile(inDir+"QCD"+region+"/output_"+sample+".root","READ")
            print sample
            #get nominal plot"
            if histoGramPerSample[sample] in nominal.keys():
                
                nominal[histoGramPerSample[sample]].Add(inFile.Get(mvaName+sample))
                if inFileInv: invNominal[histoGramPerSample[sample]].Add(inFileInv.Get(mvaName+sample))
                for sys in systs:
                    #print mvaName+sample+"_"+sys+"_up"
                    systHists[histoGramPerSample[sample]][sys+"Up"].Add(inFile.Get(mvaName+sample+"_"+sys+"_up"))
                    systHists[histoGramPerSample[sample]][sys+"Down"].Add(inFile.Get(mvaName+sample+"_"+sys+"_down"))
                if includeJetVariations: #If we're doing jet variations, we'll add them here.
                    for jetShiftSyst in range(int(nJetVariations/2.)):
                        if translateNames: jetShiftName = jetShiftNames["jetShift{0}".format(jetShiftSyst)]
                        else: jetShiftName = "jetShift{0}".format(jetShiftSyst)
                        systHists[histoGramPerSample[sample]]["{0}Up".format(jetShiftName)].Add(inFile.Get(mvaName+sample+"_JetShifts_{0}".format(jetShiftSyst*2)))
                        systHists[histoGramPerSample[sample]]["{0}Down".format(jetShiftName)].Add(inFile.Get(mvaName+sample+"_JetShifts_{0}".format((jetShiftSyst*2)+1)))
                        
                    
            else:
                print sample,histoGramPerSample[sample],mvaName
                nominal[histoGramPerSample[sample]] = inFile.Get(mvaName+sample).Clone(histoGramPerSample[sample])
                nominal[histoGramPerSample[sample]].Sumw2()
                nominal[histoGramPerSample[sample]].SetDirectory(0)
                if inFileInv:
                    invNominal[histoGramPerSample[sample]] = inFileInv.Get(mvaName+sample).Clone(histoGramPerSample[sample])
                    invNominal[histoGramPerSample[sample]].Sumw2()
                    invNominal[histoGramPerSample[sample]].SetDirectory(0)
    #            if nStatsBins == 0:
    #                nStatsBins = nominal[histoGramPerSample[sample]].GetXaxis().GetNbins()
    #                print "nStatsBins: ",nStatsBins
    #                for i in range(1,nStatsBins+1):
    #                    systs.append("statbin"+str(i))
                systHists[histoGramPerSample[sample]] = {}
                for sys in systs:
                    #print sys, mvaName+sample+"_"+sys+"_up"
                    systNameForClone = histoGramPerSample[sample]+"_"+sys
                    if "statbin" in sys: systNameForClone = histoGramPerSample[sample]+"_"+histoGramPerSample[sample]+"_"+plotLeptonSampleName+"_"+sys
                    systHists[histoGramPerSample[sample]][sys+"Up"] = inFile.Get(mvaName+sample+"_"+sys+"_up").Clone(systNameForClone+"Up")
                    systHists[histoGramPerSample[sample]][sys+"Up"].SetDirectory(0)
                    systHists[histoGramPerSample[sample]][sys+"Down"] = inFile.Get(mvaName+sample+"_"+sys+"_down").Clone(systNameForClone+"Down")
                    systHists[histoGramPerSample[sample]][sys+"Down"].SetDirectory(0)
                    systHists[histoGramPerSample[sample]][sys+"Up"].Sumw2()
                    systHists[histoGramPerSample[sample]][sys+"Down"].Sumw2()
                if includeJetVariations:
                    for jetShiftSyst in range(int(nJetVariations/2.)):
                        if translateNames: jetShiftName = jetShiftNames["jetShift{0}".format(jetShiftSyst)]
                        else: jetShiftName = "jetShift{0}".format(jetShiftSyst)
                        systNameForClone = histoGramPerSample[sample]+"_{0}".format(jetShiftName)
                        print jetShiftName, mvaName,sample
                        systHists[histoGramPerSample[sample]]["{0}Up".format(jetShiftName)] = inFile.Get(mvaName+sample+"_JetShifts_{0}".format(jetShiftSyst*2)).Clone(systNameForClone+"Up")
                        systHists[histoGramPerSample[sample]]["{0}Up".format(jetShiftName)].SetTitle(systNameForClone+"Up")
                        systHists[histoGramPerSample[sample]]["{0}Up".format(jetShiftName)].SetDirectory(0)
                        systHists[histoGramPerSample[sample]]["{0}Up".format(jetShiftName)].Sumw2()
                        systHists[histoGramPerSample[sample]]["{0}Down".format(jetShiftName)] = inFile.Get(mvaName+sample+"_JetShifts_{0}".format((jetShiftSyst*2)+1)).Clone(systNameForClone+"Down")
                        systHists[histoGramPerSample[sample]]["{0}Down".format(jetShiftName)].SetTitle(systNameForClone+"Down")
                        systHists[histoGramPerSample[sample]]["{0}Down".format(jetShiftName)].SetDirectory(0)
                        systHists[histoGramPerSample[sample]]["{0}Down".format(jetShiftName)].Sumw2()
                        

        if "wPlusJets" in invNominal.keys(): invNominal["wPlusJets"].Scale(2.98)
        
        if not includeJetVariations: #Only need to do this if we haven't included those things in the root files directly
            dirSysts = ["JES","JER"]
            upDown = ["Up","Down"]
            if doSystDir:
                for ud in upDown:
                    for syst in dirSysts:
                        dirName = syst+ud
                        print "Processing systematic in {0}".format(dirName)
                        for sample in samples:
                            inFile = TFile(inDir+dirName+region+"/output_"+sample+".root","READ")
                            print "Processing sample {0}".format(sample)
                            if dirName not in systHists[histoGramPerSample[sample]]:
                                systHists[histoGramPerSample[sample]][dirName] = inFile.Get(mvaName+sample).Clone(histoGramPerSample[sample]+"_"+syst+ud)
                                systHists[histoGramPerSample[sample]][dirName].SetDirectory(0)
                            else:
                                systHists[histoGramPerSample[sample]][dirName].Add(inFile.Get(mvaName+sample).Clone(histoGramPerSample[sample]+"_"+syst+ud))
                                
        for sample in samplesData:
            print "Dataset: {0}".format(sample)
            inFile = TFile(inDir+region+"/output_"+sample+".root","READ")
            inFileQCD = TFile(inDir+"QCD{0}/output_".format(region)+sample+".root","READ")
            if "data" not in nominal.keys():
                nominal["data"] = inFile.Get(mvaName+sample).Clone("data_obs")
                nominal["data"].SetDirectory(0)
                nominal["qcd"] = inFileQCD.Get(mvaName+sample).Clone("qcd")
                nominal["qcd"].SetDirectory(0)
                print "QCD template has {0}".format(nominal["qcd"].Integral())
            else:
                nominal["data"].Add(inFile.Get(mvaName+sample))
                nominal["qcd"].Add(inFileQCD.Get(mvaName+sample))
                print "QCD template has {0}".format(nominal["qcd"].Integral())

        #Subtract non QCD templates from DD background
        print "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
        totalMC = nominal["qcd"].Clone("totalMC")
        totalMC.Reset()
        totalTotal = totalMC.Clone("totalTotal")
        for key in invNominal.keys():
            totalTotal.Add(invNominal[key])
            if "qcdMC" in key: continue
            totalMC.Add(invNominal[key])
        if totalTotal.Integral() > 0: totalMC.Scale(nominal["qcd"].Integral()/totalTotal.Integral())
#        print nominal["qcd"].Integral(), key, invNominal[key].Integral()
        nominal["qcd"].Add(totalMC,-1)
        #No negative bins, please.
        for i in range(1,nominal["qcd"].GetXaxis().GetNbins()+1):
            if nominal["qcd"].GetBinContent(i) < 0: nominal["qcd"].SetBinContent(i,0)
        totalYieldsCount[region]["qcd"] = invNominal[key].Integral()

        #Here we should grab the systematic samples if we're doing that.        
        sysDirNamesList = []
        sysNamesToGetDownHist = []
        if doSystDir:
            for fileName in os.listdir(systDir):
                sample = fileName.split("_")[1]
                sample2 = fileName.split("output_")[1].split(".root")[0]
                if (sample2[-1]).isdigit():continue
                syst = fileName.split("_")[-1].split(".")[0]
                inFile = TFile(systDir+fileName,"READ")
                if syst in systHists[sample].keys():
                    print "Adding 1 {0},{1},{2}".format( sample, syst, sample2), systHists[sample][syst].Integral(),
                    systHists[sample][syst].Add(inFile.Get(mvaName+sample2))
                    print systHists[sample][syst].Integral()
                elif syst+"Up" in systHists[sample].keys():
                    print "Adding 2 {0},{1},{2}".format( sample, syst, sample2),
                    print systHists[sample][syst+"Up"].Integral(),
                    systHists[sample][syst+"Up"].Add(inFile.Get(mvaName+sample2))
                    print sample2,systHists[sample][syst+"Up"].Integral()
        #            systHists[sample][syst+"Down"].Add(getDownHist(inFile.Get(mvaName+sample2),nominal[sample]))
                else:
                    print "Making new {0},{1},{2}".format( sample, syst, sample2)
                    cloneName = syst
                    sysDirNamesList.append(syst)
                    if "up" in syst:
                        cloneName = syst.split("up")[0]+"Up"
                    elif "down" in syst:
                        cloneName = syst.split("down")[0]+"Down"
                    else:
                        systHists[sample][syst+"Up"] = inFile.Get(mvaName+sample2).Clone(sample+"_"+syst+"Up")
                        systHists[sample][syst+"Up"].SetDirectory(0)
                        sysNamesToGetDownHist.append((sample,syst))
        #                systHists[sample][syst+"Down"] = getDownHist(systHists[sample][syst+"Up"],nominal[sample])
                        continue
                    systHists[sample][syst] = inFile.Get(mvaName+sample2).Clone(sample+"_"+cloneName)
                    systHists[sample][syst].SetDirectory(0)

        #Here we will rebin if we want to.
        if nBins > 0:
            (nominal,systHists) = findMaxAndMinBins(nominal,systHists,nBins,region)
        for pair in sysNamesToGetDownHist:

            print "EnMaking down hist Entry: ",pair[0],pair[1]
            #A hack to normalise herwig sample correctly
#            if pair[0] == "ttbar" and pair[1] == "herwig":
#                systHists[pair[0]][pair[1]+"Up"].Scale(0.5)
            systHists[pair[0]][pair[1]+"Down"] = getDownHist(systHists[pair[0]][pair[1]+"Up"],nominal[pair[0]])

#        halveSize = ["isr","fsr","tune","hdamp"]
        halveSize = []

        for key in nominal.keys():
            if setNegToZero: setAllNegBinsToZero(nominal[key])
            totalYieldsCount[region][key] = nominal[key].Integral()

            nominal[key].Sumw2()
            
            #Make the stat variation histograms
            if "data" in key or "qcd" in key: continue
            if makeStatBins:
                for i in range(1,nominal[key].GetXaxis().GetNbins()+1):
                    systHists[key]["statbin"+str(i)+"up"],systHists[key]["statbin"+str(i)+"down"] = makeStatBinVariations(nominal[key],i,region)

        #Here make a loop to find out the highest and lowest filled bins so get rid of zero occupancy bins?
    #    for key in nominal.keys():
    #global reduceBinsToFilled
        if False: (nominal,systHists) = findMaxAndMinBins(nominal,systHists)
        outFile.cd()
        print sysDirNamesList
        for key in nominal.keys():
            if "qcdMC" in key: continue
            scaleFactor = weights.getDatasetWeight(key,region)
#            scaleFactor = 1.
            if not scaleFactor == 1: print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   Non intergral SF in region {2}!!!: {0}: {1:.2f}".format(key,scaleFactor,region)
            nominal[key].Scale(scaleFactor)
            totalYieldsCount[region][key] *= scaleFactor
#            if key in perMCSFs.keys():
#                nominal[key].Scale(perMCSFs[key])
#                totalYieldsCount[region][key] *= perMCSFs[key]
            nominal[key].Write()
            print key, nominal[key].Integral()
            if key == "data" or key == "qcd": continue
            for key2 in systHists[key].keys():
                systHists[key][key2].Scale(scaleFactor)
#                if key in perMCSFs.keys():
#                    for systName in halveSize:
#                        if key == "ttbar" and systName in key2: 
    #                        print "rescaling: ",key,key2
#                            systHists[key][key2].Scale(0.5)
#                    systHists[key][key2].Scale(perMCSFs[key])
                if setNegToZero: setAllNegBinsToZero(systHists[key][key2])
                systHists[key][key2].Write()

        if makeSystComps: 
            makeAllSystHists(nominal,systHists,region,savePostfix)
        if makeStackPlots:
            makeStackPlot(nominal,systHists,region,savePostfix)

        if makeCutAndCountTemplates:
            fillCutAndCountPlots(nominal,systHists,cutAndCountNom,cutAndCountSysts,regionIt)

    #Here we're going to make a quick table for each of the systematic uncertainties impact on the tW rate
        if not region == "3j1t": continue
        systUpAllSamples = {}
        systDownAllSamples = {}        
        allSysts = ["JES","JER","Colour rec"]
        for sample in nominal.keys():
            if "data" in sample or "qcd" in sample : continue 
            totalJESUp = 0.
            totalJESDown = 0.
            totalJERUp = 0.
            totalJERDown = 0.
            colourUp = 0.
            colourDown = 0.
            systUp = {}
            systDown = {}
            nomInt = nominal[sample].Integral()
            for key in systHists[sample].keys():
                if "gluonMoveerdOn" in key: continue
                fraction = (1-(systHists[sample][key].Integral()/nomInt))
                if "statbin" in key: continue
                if "JES" in key:
                    if "Up" in key: 
                        totalJESUp += fraction * fraction
                    elif "Down" in key:
                        totalJESDown += fraction * fraction
                    continue
                if "erdO" in key or "gluonMove" in key:
                    if "Up" in key: 
                        colourUp += fraction * fraction
                    elif "Down" in key:
                        colourDown += fraction * fraction
                    continue
                if "JER" in key:
                    if "Up" in key: 
                        totalJERUp += fraction * fraction
                    elif "Down" in key:
                        totalJERDown += fraction * fraction
                    continue
                if "Up" in key or "up" in key:
                    systName = key[:-2]
                    if not systName  in allSysts: allSysts.append(systName)
                    systUp[systName] = fraction
                elif "Down" in key or "down" in key:
                    systName = key[:-4]
                    if not systName  in allSysts: allSysts.append(systName)
                    systDown[systName] = fraction
            systUpAllSamples[sample] = systUp
            systDownAllSamples[sample] = systDown
            systUpAllSamples[sample]["JES"] = math.sqrt(totalJESUp)
            systDownAllSamples[sample]["JES"] = -math.sqrt(totalJESDown)
            systUpAllSamples[sample]["JER"] = math.sqrt(totalJERUp)
            systDownAllSamples[sample]["JER"] = -math.sqrt(totalJERDown)
            systUpAllSamples[sample]["Colour rec"] = math.sqrt(colourUp)
            systDownAllSamples[sample]["Colour rec"] = -math.sqrt(colourDown)
#        print systUp.keys(), systDown.keys()
        print "\\hline"
        for sample in systUpAllSamples.keys():
            print "& {0}".format(sample),
        print "\\\\\n\\hline"
        for syst in allSysts:
            print "{0}".format(syst),
            for sample in systUpAllSamples.keys():
                systUpPrint,systDownPrint = 0.0,0.0
                if syst in systUpAllSamples[sample].keys():
                    systUpPrint = 100*systUpAllSamples[sample][syst]
                    systDownPrint = 100*systDownAllSamples[sample][syst]
                if not ( systUpPrint == 0.0 and systDownPrint == 0.0):
                    print "\t& {1:.1f}\\%/{2:.1f}\\%".format(syst,systUpPrint,systDownPrint),
                else:
                    print "\t& -/-",
            print "\\\\"
        print "\\hline"
#        print "JES \t& {0:.1f}%/{1:.1f}%".format(100*math.sqrt(totalJESUp),100*math.sqrt(totalJESDown))


    if makeCutAndCountTemplates:
        outFileCC = TFile(outDir+"mvaDists_cc_{1}.root".format(saveName,channeltr),"RECREATE")
        outFileCC.cd()
        for sample in cutAndCountNom.keys():
            cutAndCountNom[sample].Write()
            for syst in cutAndCountSysts[sample].keys():
                cutAndCountSysts[sample][syst].Write()
        setMakeCutAndCount(False)
#        makeCutAndCountTemplates = False #We only need to run this once.
        outFile.cd()

    for region in regions:
        print region
        for key in totalYieldsCount[region].keys():
            print key, totalYieldsCount[region][key]

    for j in regions:
        print "\"{0}\":[".format(j),
        for i in ["VV", "wPlusJets", "zPlusJets", "ttbar", "singleTop", "qcd", "tW"]:
            print "{0},".format(totalYieldsCount[j][i]),
        print "],",

    #add in a line about 4j region if we have used it
    if "4j1t" in regions and "4j2t" in regions:
        print "\"4j\":[",
        for i in ["VV", "wPlusJets", "zPlusJets", "ttbar", "singleTop", "qcd", "tW"]:
            print "{0},".format(totalYieldsCount["4j1t"][i] + totalYieldsCount["4j2t"][i]),
        print "],",
        
    print ""

    for j in regions:
        for i in ["VV", "wPlusJets", "zPlusJets", "ttbar", "singleTop", "qcd", "tW"]:
            print "{0} {1}".format(j,i),

    print

    for j in regions:
        print "\"{0}\":{1},".format(j,totalYieldsCount[j]["data"]),
    if "4j1t" in regions and "4j2t" in regions: print "\"{0}\":{1},".format("4j",totalYieldsCount["4j1t"]["data"]+totalYieldsCount["4j2t"]["data"])

    jsonYields = totalYieldsCount
    #Redo these summing barrel and endcap
    if "Barrel" in regions[0] or "Endcap" in regions[0]: jsonYields = {}
    for j in regions:
        if not "Barrel" in j: continue
        reducedRegion = j.split("Barrel")[0]
        endcapReg = "{0}Endcap".format(reducedRegion)
        jsonYields[reducedRegion] = {}
        print "\"{0}\":[".format(reducedRegion),
        for i in ["VV", "wPlusJets", "zPlusJets", "ttbar", "singleTop", "qcd", "tW"]:
            jsonYields[reducedRegion][i] = totalYieldsCount[j][i] 
            if not ignoreEndcap: jsonYields[reducedRegion][i] += totalYieldsCount[endcapReg][i]
            print "{0},".format(jsonYields[reducedRegion][i]),
        print "],",
            
    for j in regions:
        if not "Barrel" in j: continue
        reducedRegion =j.split("Barrel")[0]
        endcapReg = "{0}Endcap".format(reducedRegion)
        jsonYields[reducedRegion]["data"] = totalYieldsCount[j]["data"]
        if not ignoreEndcap: jsonYields[reducedRegion]["data"]  += totalYieldsCount[endcapReg]["data"]
        print "\"{0}\":{1},".format(reducedRegion,jsonYields[reducedRegion]["data"]),

    #if "4j1t" in regions and "4j2t" in regions: print "\"{0}\":{1},".format("4j",totalYieldsCount["4j1t"]["data"]+totalYieldsCount["4j2t"]["data"])
                                    
    with open("{0}/yields_{1}.json".format(outDir,channeltr),"w") as outfile:
        json.dump(jsonYields,outfile)

def haddRegions(regions,savePostfix,ignoreEndcap=False):
    for region in regions:
        if os.path.exists("{3}/mvaDists_{0}_{1}{2}.root".format(region,channeltr,savePostfix,outDir)): os.remove("{3}/mvaDists_{0}_{1}{2}.root".format(region,channeltr,savePostfix,outDir))
        if ignoreEndcap:         subprocess.call("hadd {3}/mvaDists_{0}_{1}{2}.root {3}/mvaDists_{0}Barrel_{1}{2}.root".format(region,channeltr,savePostfix,outDir),shell=True)
        else: subprocess.call("hadd {3}/mvaDists_{0}_{1}{2}.root {3}/mvaDists_{0}*_{1}{2}.root".format(region,channeltr,savePostfix,outDir),shell=True)


if __name__ == "__main__":

    regions = components.regions
    if "--extendRegions" in sys.argv: regions = components.extendedRegions
    if "--onlySigReg" in sys.argv: 
        regions = ["4j1t","3j1t"] 
    nBins = 0
    if "--preRebin" in sys.argv:
        ind = sys.argv.index("--preRebin")
        nBins = int(sys.argv[ind+1])
    ogRegions = regions
    if not "--noSplitBarrelEndcap" in sys.argv:
        newRegions = []
        for region in regions:
            newRegions.append("{0}Barrel".format(region))
            newRegions.append("{0}Endcap".format(region))
        regions = newRegions
#    regions = ["3j1t"]
#    regions = ["3j1t","2j1t","3j2t","4j1t","4j2t"]
    bins = ["bin10_","bin20_","bin30_","bin40_","bin50_","bin80_","bin100_","bin1000_"]
#    bins = ["bin10_","bin20_","bin30_","bin40_","bin50_","bin80_"]
    bins = ["bin1000_"]
#    bins = ["bin10_"]
#    for postName in ["bin10_","bin20_","bin30_","bin40_","bin50_","bin80_","bin100_","bin1000_"]:
#    for postName in ["bin10_","bin20_","bin30_","bin40_","bin50_","bin80_"]:
#    for postName in ["bin80_","bin100_"]:
#    for postName in ["bin10_"]:
    ignoreEndcap = "--ignoreEndcap" in sys.argv            
    for postName in bins:
        print mvaNameDef+postName, "_"+postName[:-1]
        makeDatacard(postName,regions,"_"+postName[:-1],ignoreEndcap,nBins)
        if not "--noSplitBarrelEndcap" in sys.argv:
            haddRegions(ogRegions,"_"+postName[:-1],ignoreEndcap)
