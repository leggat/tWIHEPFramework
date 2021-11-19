#A utility class that will put the plotting macros in one place.
from ROOT import *

from array import array

import math, os

from setTDRStyle import setTDRStyle

from analysisComponents import AnalysisComponents

import weightProcesses

weights = ""

customWeights = ""

comp = AnalysisComponents("met")

#Global variables
latex = TLatex()
cmsText = "CMS"
extraText = "Preliminary"
latex2 = TLatex()
text2 = TLatex()
latex2String = "35.9 fb^{-1} (13TeV)"
text2String = "3j1t channel, #mu channel"
regionStr = "3j1t"

cmsTextFont = 61
extraTextFont = 52

nBinsPerPlots = 0

outDir = ""
ratioMin = 0.7
ratioMax = 1.3


def setRebinNumber(nBins):
    global nBinsPerPlots
    nBinsPerPlots = nBins

def setWeights(isPrefit,isElectron):
    global weights
    weights = weightProcesses.ReweightObject(isPrefit,isElectron)

def getErrorPlot(totalMC,systUp,systDown,isRatio = False):
    ###
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

def setRegionLepton(region="3j1t",lepton="#mu"):
    global text2String
    text2String = "{0} region, {1} channel".format(region,lepton)
    global regionStr
    regionStr = region

def changeLabels(latex2=False,text2=False):
    global latex2String
    if latex2: latex2String = latex2
    global text2String
    if text2: text2String = text2

def setupEnv(isMuon = True):
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

    global latex 
    global cmsText  #In case we want to change this bit of text for some reason
    global extraTex
    global latex2
    global text2

    latex.SetNDC()
    latex.SetTextAlign(31)
    
    latex2.SetNDC();
    latex2.SetTextSize(0.04);
    latex2.SetTextAlign(31);

    text2.SetNDC()
    text2.SetTextAlign(13)
    text2.SetX(0.18)
    text2.SetY(0.92)
    text2.SetTextFont(42)
    text2.SetTextSize(0.0610687)

    
def printCMSText():
    print cmsText,extraText

def setOutDir(ourDirName):
    global outDir
    outDir = ourDirName
    if not os.path.exists(ourDirName): os.makedirs(ourDirName)

def makeLegend():
    leggy = TLegend(0.8,0.6,0.95,0.9)
    leggy.SetFillStyle(1001)
    leggy.SetBorderSize(1)
    leggy.SetFillColor(0)
    leggy.SetLineColor(0)
    leggy.SetShadowColor(0)
    leggy.SetFillColor(kWhite)
    return leggy

def makeRatioCanvas(name):
    ratioCanvy = TPad(name+"_ratio",name+"_ratio",0.0,0.0,1.0,1.0)
    ratioCanvy.SetTopMargin(0.7)
    ratioCanvy.SetFillColor(0)
    ratioCanvy.SetFillStyle(0)
    ratioCanvy.SetGridy(1)
    SetOwnership(ratioCanvy,False)
    return ratioCanvy

def setRatioHistProps(sumHistoData):
    sumHistoData.GetYaxis().SetTitle("Data/MC")
    sumHistoData.GetYaxis().SetTitleOffset(1.3)

    sumHistoData.SetMinimum(ratioMin)
    sumHistoData.SetMaximum(ratioMax)
    sumHistoData.GetXaxis().SetTitleOffset(1.)
    sumHistoData.GetXaxis().SetLabelSize(0.04)
    sumHistoData.GetXaxis().SetTitleSize(0.045)
    sumHistoData.GetYaxis().SetNdivisions(6)
    sumHistoData.GetYaxis().SetTitleSize(0.03)
    sumHistoData.GetYaxis().SetLabelSize(0.03)
    return sumHistoData

def compareHists(name,compareHists,normaliseHists=False,includeRatio=True):

    leggy  = makeLegend()
    canvy = TCanvas(name,name,1000,800)
    
    if includeRatio: canvy.SetBottomMargin(0.3)
    canvy.cd()

    if normaliseHists:
        for hist in compareHists:
            if hist.Integral() > 0: hist.Scale(1./hist.Integral())

    histMax = compareHists[0].GetMaximum()
    colours = [kRed,kBlue,kGreen]
    for hist in range(len(compareHists)):
        if compareHists[hist].GetMaximum() > histMax: histMax = compareHists[hist].GetMaximum()
        compareHists[hist].SetFillColor(0)
        compareHists[hist].SetLineWidth(3)
        compareHists[hist].SetLineColor(colours[hist])
        leggy.AddEntry(compareHists[hist],compareHists[hist].GetName())
    compareHists[0].SetMaximum(histMax * 1.2)
    
    for hits in compareHists:
        hits.Draw("hist same")

    leggy.Draw("same")

    latex.SetTextSize(0.04)
    latex.SetTextFont(cmsTextFont)
    latex.DrawLatex(0.23, 0.95, cmsText )
    
    latex.SetTextFont(extraTextFont)
    latex.SetTextSize(0.04*0.76)
    latex.DrawLatex(0.35, 0.95 , extraText )
    
    latex2.DrawLatex(0.95, 0.95, latex2String)

    text2.DrawLatex(0.18,0.92, text2String)
    
    if includeRatio:
        ratioCanvy = makeRatioCanvas(name)
        ratioCanvy.Draw()
        ratioCanvy.cd(0)
        for i in range(1,len(compareHists)):
            tmpHist = compareHists[i].Clone(compareHists[i].GetName()+"ratio")
            tmpHist.Divide(compareHists[0])
            tmpHist = setRatioHistProps(tmpHist)
            tmpHist.Draw("hist same")

    canvy.SaveAs(outDir+name+".png")
    canvy.SaveAs(outDir+name+".root")

def makeASingleStackPlot(histMap,name,doData=True,dataHistName="data_obs",xAxisLabel="",yAxisLabel="Events",xAxisBinLabels=[],isLog=False,forceDataMax=False):

    leggy = TLegend(0.8,0.6,0.95,0.9)
    leggy.SetFillStyle(1001)
    leggy.SetBorderSize(1)
    leggy.SetFillColor(0)
    leggy.SetLineColor(0)
    leggy.SetShadowColor(0)
    leggy.SetFillColor(kWhite)
    
    mcStack = THStack(name,name)

    canvy = TCanvas(name,name,1000,800)

    dataHist = 0
    
    if doData: canvy.SetBottomMargin(0.3)
    
    canvy.cd()

    if doData:
        dataHist = histMap[dataHistName]
        dataHist.SetMarkerStyle(20)
        dataHist.SetMarkerSize(1.2)
        dataHist.SetMarkerColor(kBlack)
        if nBinsPerPlots > 0:
            if dataHist.GetXaxis().GetNbins() > int(nBinsForPlots):
                rebin = int(dataHist.GetXaxis().GetNbins() / nBinsForPlots)
                dataHist.Rebin(rebin)
        leggy.AddEntry(dataHist,"Data","p")

    makeClone = True
    sumHistoMC = 0
    errorList = 0
    for sample in comp.histoGramOrder[::-1]:
        if sample not in histMap.keys(): continue
        leggy.AddEntry(histMap[sample],comp.histNameInLegend[sample],"f")
        if makeClone: 
            sumHistoMC = histMap[sample].Clone("Total MC")
            sumHistoMC.Reset()
            errorList = [0] * sumHistoMC.GetXaxis().GetNbins()
            makeClone = False
        
    for sample in comp.histoGramOrder:
        if sample not in histMap.keys(): continue
        histMap[sample].SetFillColor(comp.histoColours[sample])
        histMap[sample].SetLineColor(kBlack)
        histMap[sample].SetLineWidth(1)
        if weights:
            scaleFactor = histMap[sample].getDatasetWeight(name,regionStr)
            histMap[sample].Scale(scaleFactor)
        
        if customWeights and sample in customWeights.keys():
            scaleFactor = customWeights[sample]
            histMap[sample].Scale(scaleFactor)

        if nBinsPerPlots > 0:
            rebin = int(histMap[sample].GetXaxis().GetNbins() / nBinsForPlots)
            histMap[sample].Rebin(rebin)

        
        for binN in range(histMap[sample].GetXaxis().GetNbins()):
            errorList[binN] += histMap[sample].GetBinError(binN+1)

        mcStack.Add(histMap[sample])
        sumHistoMC.Add(histMap[sample])
    
    maxi = mcStack.GetMaximum()

    if doData and (dataHist.GetMaximum() > maxi or forceDataMax): maxi = dataHist.GetMaximum()

    mcStack.SetMaximum(1.3 * maxi)

    #Make the stat error graph
    errorGraph = getErrorPlot(sumHistoMC,errorList,errorList,False)
    errorGraph.SetFillStyle(3013)
    errorGraph.SetFillColor(12)

    #Draw commands
    mcStack.Draw("hist")
    errorGraph.Draw("same e2")
    leggy.AddEntry(errorGraph,"Stat. unc.","f")
    if doData: 
        mcStack.GetXaxis().SetLabelSize(0.0)    
        dataHist.Draw("e x0, same")

    mcStack.GetXaxis().SetTitle(sumHistoMC.GetXaxis().GetTitle())
    
    mcStack.GetYaxis().SetTitleOffset(1.)
    mcStack.GetYaxis().SetLabelSize(0.03)
    mcStack.GetYaxis().SetTitleSize(0.04)
    mcStack.GetYaxis().CenterTitle()
    
    mcStack.GetXaxis().SetTitleSize(0.045)
    mcStack.GetYaxis().SetTitle(yAxisLabel)
    if xAxisLabel: mcStack.GetXaxis().SetTitle(xAxisLabel)
    
    for i in range(len(xAxisBinLabels)):
        mcStack.GetXaxis().SetBinLabel(i+1,xAxisBinLabels[i])
        if doData: dataHist.GetXaxis().SetBinLabel(i+1,xAxisBinLabels[i])
    
    #Now write the labels on the plot
    leggy.Draw()

    latex.SetTextSize(0.04)
    latex.SetTextFont(cmsTextFont)
    latex.DrawLatex(0.23, 0.95, cmsText )

    latex.SetTextFont(extraTextFont)
    latex.SetTextSize(0.04*0.76)
    latex.DrawLatex(0.35, 0.95 , extraText )

    latex2.DrawLatex(0.95, 0.95, latex2String)

    text2.DrawLatex(0.18,0.92, text2String)

    ratioCanvy = 0
    sumHistoData = 0
    
    if doData:
        mcStack.GetXaxis().SetTitle("")
        ratioCanvy = TPad(name+"_ratio",name+"_ratio",0.0,0.0,1.0,1.0)
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
        sumHistoData.GetXaxis().SetTitleOffset(1.)
        sumHistoData.GetXaxis().SetLabelSize(0.04)
        sumHistoData.GetXaxis().SetTitleSize(0.045)
        sumHistoData.GetYaxis().SetNdivisions(6)
        sumHistoData.GetYaxis().SetTitleSize(0.03)
        sumHistoData.GetYaxis().SetLabelSize(0.03)
        sumHistoData.Draw("E1X0")
        errorPlotRatio = getErrorPlot(sumHistoMC,errorList,errorList,True)
        errorPlotRatio.SetFillStyle(3013)
        errorPlotRatio.SetFillColor(12)
        errorPlotRatio.Draw("e2 same")
        sumHistoData.GetXaxis().SetTitle(xAxisLabel)
    
    if isLog: canvy.SetLogy()

    canvy.SaveAs(outDir+name+".png")
    canvy.SaveAs(outDir+name+".root")

def customWeightObj(weights):
    global customWeights
    customWeights = weights

def initLatexFile():
    return
