#A utility class that will put the plotting macros in one place.
from ROOT import *

from array import array

import math, os

from setTDRStyle import setTDRStyle

from analysisComponents import AnalysisComponents

import weightProcesses

class GenericPlottingClass:

    weights = ""

    customWeights = ""

    savePDFs = False
    saveCFiles = False
    
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

    symmetriseTotalUncert = False

    outDir = ""
    ratioMin = 0.7
    ratioMax = 1.3
    def __init__(self):
        
        self.cmsText = "CMS"
                 
    def setSaveCFiles(self,save):
        self.saveCFiles = save

    def setSavePDF(self,save):
        self.savePDFs = save

    def setSymmetriseTotalUncert(self,symUncert):
        self.symmetriseTotalUncert = symUncert

    def setRatioWidth(self,width):
        self.ratioMin = 1-width
        self.ratioMax = 1+width

    def setRebinNumber(self,nBins):
        self.nBinsPerPlots = nBins

    def setWeights(self,isPrefit,isElectron):
        self.weights = weightProcesses.ReweightObject(isPrefit,isElectron)

    def makeTotalUncert(self,totalMC,systMap):
        errorUp = [0]*totalMC.GetXaxis().GetNbins()
        errorDown = [0]*totalMC.GetXaxis().GetNbins()

        for i in range(1,totalMC.GetXaxis().GetNbins()+1):
            if totalMC.GetBinContent(i) == 0.: continue
            for syst in systMap.keys():
                if "Up" in syst:
                    errorUp[i-1] += ((systMap[syst].GetBinContent(i)/totalMC.GetBinContent(i))-1) *  ((systMap[syst].GetBinContent(i)/totalMC.GetBinContent(i))-1)
                else:
                    errorDown[i-1] += ((systMap[syst].GetBinContent(i)/totalMC.GetBinContent(i))-1) *  ((systMap[syst].GetBinContent(i)/totalMC.GetBinContent(i))-1)
            errorUp[i-1] = (sqrt(errorUp[i-1])) * totalMC.GetBinContent(i)
            errorDown[i-1] = (sqrt(errorDown[i-1])) * totalMC.GetBinContent(i)
            #symmetrise if we want to
            if self.symmetriseTotalUncert:
                if abs(errorUp[i-1]) > abs(errorDown[i-1]):
                    errorDown[i-1] = errorUp[i-1]
                else:
                    errorUp[i-1] = errorDown[i-1] 
        return errorUp,errorDown

    def makeSystList(self,totalMC,systematic):
        errorList = []
        for i in range(1,totalMC.GetXaxis().GetNbins()+1):
            errorList.append(abs(totalMC.GetBinContent(i) - systematic.GetBinContent(i)))
        return errorList

    def getErrorPlot(self,totalMC,systUp,systDown,isRatio = False):
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

    def setRegionLepton(self,region="3j1t",lepton="#mu"):
        self.text2String = "{0} region, {1} channel".format(region,lepton)
        self.regionStr = region

    def changeLabels(self,latex2=False,text2=False):
        if latex2: self.latex2String = latex2
        if text2: self.text2String = text2

    def setupEnv(self,isMuon = True):
        gROOT.SetBatch()

        gStyle.SetOptTitle(0)

        gStyle.SetPalette(1)
        gStyle.SetCanvasBorderMode(0)
        gStyle.SetCanvasColor(kWhite)
        gStyle.SetCanvasDefH(600)
        gStyle.SetCanvasDefW(600)
        gStyle.SetLabelFont(18,"")

        setTDRStyle()

        gStyle.SetHatchesLineWidth(3)
#        gStyle.SetHatchesSpacing(1)


        gStyle.SetTitleYOffset(0.8)

#        global latex 
#        global cmsText  #In case we want to change this bit of text for some reason
#        global extraTex
#        global latex2
#        global text2

        self.latex.SetNDC()
        self.latex.SetTextAlign(31)

        self.latex2.SetNDC();
        self.latex2.SetTextSize(0.04);
        self.latex2.SetTextAlign(31);

        self.text2.SetNDC()
        self.text2.SetTextAlign(13)
        self.text2.SetX(0.18)
        self.text2.SetY(0.92)
        self.text2.SetTextFont(42)
        self.text2.SetTextSize(0.0610687)


    def printCMSText(self):
        print cmsText,extraText

    def setOutDir(self,ourDirName):
        self.outDir = ourDirName
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

    def setRatioHistProps(self,sumHistoData):
        sumHistoData.GetYaxis().SetTitle("Data/MC")
        sumHistoData.GetYaxis().SetTitleOffset(1.3)

        sumHistoData.SetMinimum(self.ratioMin)
        sumHistoData.SetMaximum(self.ratioMax)
        sumHistoData.GetXaxis().SetTitleOffset(1.)
        sumHistoData.GetXaxis().SetLabelSize(0.04)
        sumHistoData.GetXaxis().SetTitleSize(0.045)
        sumHistoData.GetYaxis().SetNdivisions(6)
        sumHistoData.GetYaxis().SetTitleSize(0.03)
        sumHistoData.GetYaxis().SetLabelSize(0.03)
        return sumHistoData

    def compareHists(self,name,compareHists,normaliseHists=False,includeRatio=True):

        leggy  = self.makeLegend()
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

        self.latex.SetTextSize(0.04)
        self.latex.SetTextFont(self.cmsTextFont)
        self.latex.DrawLatex(0.23, 0.95, self.cmsText )

        self.latex.SetTextFont(self.extraTextFont)
        self.latex.SetTextSize(0.04*0.76)
        self.latex.DrawLatex(0.35, 0.95 , self.extraText )

        self.latex2.DrawLatex(0.95, 0.95, self.latex2String)

        self.text2.DrawLatex(0.18,0.92, self.text2String)

        if includeRatio:
            ratioCanvy = self.makeRatioCanvas(name)
            ratioCanvy.Draw()
            ratioCanvy.cd(0)
            for i in range(1,len(compareHists)):
                tmpHist = compareHists[i].Clone(compareHists[i].GetName()+"ratio")
                tmpHist.Divide(compareHists[0])
                tmpHist = self.setRatioHistProps(tmpHist)
                tmpHist.Draw("hist same")

        canvy.SaveAs(outDir+name+".png")
        canvy.SaveAs(outDir+name+".root")

    def makeASingleStackPlot(self,histMap,name,doData=True,dataHistName="data_obs",xAxisLabel="",yAxisLabel="Events",xAxisBinLabels=[],isLog=False,forceDataMax=False,systMap=""):

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
            if self.nBinsPerPlots > 0:
                if dataHist.GetXaxis().GetNbins() > int(self.nBinsPerPlots):
                    rebin = int(dataHist.GetXaxis().GetNbins() / self.nBinsPerPlots)
                    dataHist.Rebin(rebin)
            leggy.AddEntry(dataHist,"Data","p")

        makeClone = True
        sumHistoMC = 0
        errorList = 0
        for sample in self.comp.histoGramOrder[::-1]:
            if sample not in histMap.keys(): continue
            leggy.AddEntry(histMap[sample],self.comp.histNameInLegend[sample],"f")
            if makeClone: 
                sumHistoMC = histMap[sample].Clone("Total MC")
                sumHistoMC.Reset()
                if self.nBinsPerPlots > 0:
                    rebin = int(sumHistoMC.GetXaxis().GetNbins() / self.nBinsPerPlots)
                    sumHistoMC.Rebin(rebin)
                errorList = [0] * sumHistoMC.GetXaxis().GetNbins()
                makeClone = False

        for sample in self.comp.histoGramOrder:
            if sample not in histMap.keys(): continue
            histMap[sample].SetFillColor(self.comp.histoColours[sample])
            histMap[sample].SetLineColor(kBlack)
            histMap[sample].SetLineWidth(1)
            if self.weights:
                scaleFactor = histMap[sample].getDatasetWeight(name,regionStr)
                histMap[sample].Scale(scaleFactor)

            if self.customWeights and sample in self.customWeights.keys():
                scaleFactor = self.customWeights[sample]
                histMap[sample].Scale(scaleFactor)

            if self.nBinsPerPlots > 0:
                rebin = int(histMap[sample].GetXaxis().GetNbins() / self.nBinsPerPlots)
                histMap[sample].Rebin(rebin)


            for binN in range(histMap[sample].GetXaxis().GetNbins()):
                errorList[binN] += histMap[sample].GetBinError(binN+1)

    
            mcStack.Add(histMap[sample])
            sumHistoMC.Add(histMap[sample])
            
            

        maxi = mcStack.GetMaximum()

        if doData and (dataHist.GetMaximum() > maxi or forceDataMax): maxi = dataHist.GetMaximum()

        mcStack.SetMaximum(1.3 * maxi)

        #Make the stat error graph
        errorGraph = self.getErrorPlot(sumHistoMC,errorList,errorList,False)
        errorGraph.SetFillStyle(3354)
        errorGraph.SetFillColor(36)

        #If we have been given a custom syst map, add them in here
        additionalSysts = {}
        additionalErrorLists = {}
        if systMap:

            newSystMap = {}
            for syst in systMap:
                newSystMap[syst] = systMap[syst].Clone()
                if self.nBinsPerPlots > 0:
                    rebin = int(newSystMap[syst].GetXaxis().GetNbins() / self.nBinsPerPlots )
                    newSystMap[syst].Rebin(rebin)
            additionalErrorLists["JER/JES/Unc.Up"],additionalErrorLists["JER/JES/Unc.Down"] = self.makeTotalUncert(sumHistoMC,newSystMap)
            additionalSysts["JER/JES/Unc."] = self.getErrorPlot(sumHistoMC,additionalErrorLists["JER/JES/Unc.Up"],additionalErrorLists["JER/JES/Unc.Down"],False)
            syst = "JER/JES/Unc."
            additionalSysts[syst].SetFillStyle(3345)
            additionalSysts[syst].SetFillColor(46)
        if False:
            systs = [f[:-2] for f in systMap.keys() if "Up" in f]
            systFill = 3008
            systColour = 7
            for syst in systs:
                sysUp = systMap[syst+"Up"]
                sysDown = systMap[syst+"Down"]
                if self.nBinsPerPlots > 0:
                    rebin = int(sysUp.GetXaxis().GetNbins() / self.nBinsPerPlots)
                    sysUp.Rebin(rebin)
                    sysDown.Rebin(rebin)

                additionalErrorLists[syst+"Up"] = self.makeSystList(sumHistoMC,systMap[syst+"Up"])
                additionalErrorLists[syst+"Down"] = self.makeSystList(sumHistoMC,systMap[syst+"Down"])
                print syst
                for i in range(len(additionalErrorLists[syst+"Up"])):
                    print i, additionalErrorLists[syst+"Up"][i], additionalErrorLists[syst+"Down"][i]
                additionalSysts[syst] = self.getErrorPlot(sumHistoMC,additionalErrorLists[syst+"Up"],additionalErrorLists[syst+"Down"],False)
                additionalSysts[syst].SetFillStyle(systFill)
                systFill+=1
                additionalSysts[syst].SetFillColor(systColour)
                systColour+=1



        #Draw commands
        mcStack.Draw("hist")
        errorGraph.Draw("same e2")
        for syst in additionalSysts.keys():
#            additionalSysts[syst].Draw("same e2")
            leggy.AddEntry(additionalSysts[syst],syst,"f")
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

        self.latex.SetTextSize(0.04)
        self.latex.SetTextFont(self.cmsTextFont)
        self.latex.DrawLatex(0.23, 0.95, self.cmsText )

        self.latex.SetTextFont(self.extraTextFont)
        self.latex.SetTextSize(0.04*0.76)
        self.latex.DrawLatex(0.35, 0.95 , self.extraText )

        self.latex2.DrawLatex(0.95, 0.95, self.latex2String)

        self.text2.DrawLatex(0.18,0.92, self.text2String)

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
            sumHistoData.SetMinimum(self.ratioMin)
            sumHistoData.SetMaximum(self.ratioMax)
            sumHistoData.GetXaxis().SetTitleOffset(1.)
            sumHistoData.GetXaxis().SetLabelSize(0.04)
            sumHistoData.GetXaxis().SetTitleSize(0.045)
            sumHistoData.GetYaxis().SetNdivisions(6)
            sumHistoData.GetYaxis().SetTitleSize(0.03)
            sumHistoData.GetYaxis().SetLabelSize(0.03)
            sumHistoData.Draw("E1X0")
            errorPlotRatio = self.getErrorPlot(sumHistoMC,errorList,errorList,True)
            errorPlotRatio.SetFillStyle(3354)
            errorPlotRatio.SetFillColor(36)
            
            systFill = 3345
            systColour = 46
            for syst in additionalSysts.keys():
                if syst == "Unclus." or syst == "JES": continue
                errorPlotRatioSyst = self.getErrorPlot(sumHistoMC,additionalErrorLists[syst+"Up"],additionalErrorLists[syst+"Down"],True)
                errorPlotRatioSyst.SetFillStyle(systFill)
                systFill+=1
                errorPlotRatioSyst.SetFillColor(systColour)
                systColour+=1
                errorPlotRatioSyst.Draw("e2 same")
            errorPlotRatio.Draw("e2 same")
            sumHistoData.Draw("E1X0 same")
            sumHistoData.GetXaxis().SetTitle(xAxisLabel)
        if isLog: canvy.SetLogy()
    

        canvy.SaveAs(self.outDir+name+".png")
        if self.savePDFs: canvy.SaveAs(self.outDir+name+".pdf")
        if self.saveCFiles: canvy.SaveAs(self.outDir+name+".C")
        canvy.SaveAs(self.outDir+name+".root")

    def customWeightObj(weights):
        self.customWeights = weights

    def initLatexFile():
        return
