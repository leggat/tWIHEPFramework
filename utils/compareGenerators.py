#Script to 

from ROOT import *

import os, math

from setTDRStyle import setTDRStyle

gROOT.SetBatch()

inDir1 = "/publicfs/cms/user/duncanleg/tW13TeV/condorStuff/hists20190228_electron/tWEle"
inDir1 = "/publicfs/cms/user/duncanleg/tW13TeV/condorStuff/hists20190228_muon/tW"
dir1Post = "/tW_mcanlo/hists/mergedtW_mcanlo.root"
inDir2 = "/publicfs/cms/user/duncanleg/tW13TeV/condorStuff/hists20190228_electron/tWEle"
inDir2 = "/publicfs/cms/user/duncanleg/tW13TeV/condorStuff/hists20190228_muon/tW"
dir2Post1 = "/tW_top_nfh/hists/mergedtW_top_nfh.root"
dir2Post2 = "/tW_antitop_nfh/hists/mergedtW_antitop_nfh.root"

outDir = "plots/genComp"

regions = ["3j1t"]
regions = ["all"]

cmsText = "CMS"
extraText = "Preliminary"

setTDRStyle()

leptonInLabel = "#mu"
if "ele" in inDir1: leptonInLabel = "e"

text2 = TLatex(0.45,0.98, "{0} channel, all regions".format(leptonInLabel))
text2.SetNDC()
text2.SetTextAlign(13)
text2.SetX(0.18)
text2.SetY(0.92)
text2.SetTextFont(42)
text2.SetTextSize(0.0610687)

ignorePlots = ["00_","01_","02_","03_","04_","05_","06_","07_","08_","09_","10_","11_","13_","15_","20_","21_","22_","23_"]

nBins = 40

def getPlotPaths(inFile):

    plotPaths = []

    for obj in inFile.GetListOfKeys():
        tempThing = inFile.Get(obj.GetName())
        print obj.GetName()
        if not tempThing.ClassName().find("TH1") == -1 : plotPaths.append(tempThing.GetName())
        if not tempThing.ClassName().find("Directory") == -1:
            for k2 in tempThing.GetListOfKeys():
                temp2 = tempThing.Get(k2.GetName())
                if not temp2.ClassName().find("TH1") == -1:
                    plotPaths.append(tempThing.GetName() + "/"+ temp2.GetName())

    return plotPaths

def makeCompPlots(plotName,tW_mcanlo,tW_top,tW_antitop,outDir,region,latexFile = ""):

    #Work out what we'll call the plot
    saveName = plotName
    titleName = plotName
    if not plotName.find("/") == -1:
        saveName = ""
        titleName = ""
        for part in plotName.split("/"):
            saveName += part + "_"
        for part in saveName.split("_"):
            titleName += part
        saveName = saveName[:-1]
        titleName = titleName[:-1]
    
    mcnloHist = tW_mcanlo.Get(plotName)
    tWHist = tW_top.Get(plotName)
    tWHist.Add(tW_antitop.Get(plotName))

    leggy = TLegend(0.8,0.6,0.95,0.9)
    leggy.SetFillStyle(1001)
    leggy.SetBorderSize(1)
    leggy.SetFillColor(0)
    leggy.SetLineColor(0)
    leggy.SetShadowColor(0)
    leggy.SetFillColor(kWhite)
    saveHistName = ""

    canvy = TCanvas(plotName,plotName,1000,800)
    canvy.cd()

    mcnloHist.SetLineColor(kRed)
    mcnloHist.SetLineWidth(3)
    mcnloHist.SetFillColor(0)
    if mcnloHist.GetXaxis().GetNbins() > nBins:
        mcnloHist.Rebin(5)
#        mcnloHist.Rebin(int(math.ceil(float(nBins)/float(mcnloHist.GetXaxis().GetNbins()))))
    if mcnloHist.Integral() > 0:
        mcnloHist.Scale(1./mcnloHist.Integral())
    leggy.AddEntry(mcnloHist,"tW_mcanlo","f")
    tWHist.SetLineColor(kBlue)
    tWHist.SetLineWidth(3)
    tWHist.SetFillColor(0)
    if tWHist.GetXaxis().GetNbins() > nBins:
        tWHist.Rebin(5)
#        tWHist.Rebin(int(math.ceil(float(nBins)/float(tWHist.GetXaxis().GetNbins()))))
    if tWHist.Integral() > 0.:
        tWHist.Scale(1./tWHist.Integral())
    maxi = mcnloHist.GetMaximum()
    if tWHist.GetMaximum() > maxi: maxi = tWHist.GetMaximum()
    mcnloHist.SetMaximum(maxi*1.2)
    leggy.AddEntry(tWHist,"tW","f")
    mcnloHist.Draw("same h")
    tWHist.Draw("same h")
    
    leggy.Draw("same")
    text2.Draw()
    canvy.SaveAs("{0}/{1}{2}.png".format(outDir,saveName,region))
    canvy.SaveAs("{0}/{1}{2}.root".format(outDir,saveName,region))
    
    if latexFile:
        latexFile.write("\\frame{\n\\frametitle{"+titleName+region+"}\n\\includegraphics[width=0.9\\textwidth]{"+saveName+region+".png}\n}\n")

if __name__ == "__main__":
    
    if not os.path.isdir(outDir):
        os.mkdir(outDir)


    latexFile = open(outDir+"/wJetRegionComps.tex","w")
    latexFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")

    for region in regions:
        tW_mcatnlo = TFile(inDir1+region+dir1Post,"READ")
        tW_top = TFile(inDir2+region+dir2Post1,"READ")
        tW_antitop = TFile(inDir2+region+dir2Post2,"READ")
        
        plotPaths = getPlotPaths(tW_mcatnlo)
        
        #Loop over the plots in the file
        for plotName in plotPaths:
            #don't make the plot if we're ignoring it
            doPlot = True
            for ignore in ignorePlots:
                if ignore in plotName: 
                    doPlot = False
                    break
            if not doPlot: continue
            
            #Now make the comparison plot
            makeCompPlots(plotName,tW_mcatnlo,tW_top,tW_antitop,outDir,region,latexFile)
    
    latexFile.write("\\end{document}")
