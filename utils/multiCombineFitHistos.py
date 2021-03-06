#A macro to combine two plots onto a single canvas for fitting purposes.

from ROOT import *

isEle = "Ele"
#isEle = ""

import sys , os
gROOT.SetBatch()

def combineHists(inFileN1,inFileN2,outFileN):
    inFile2 = TFile(inFileN2,"READ")
    inFile1 = TFile(inFileN1,"READ")

    canvy1, canvy2 = 0,0

    for prim in inFile1.GetListOfKeys():
        canvy1 = prim.ReadObj()

    for prim in inFile2.GetListOfKeys():
        canvy2 = prim.ReadObj()

    stack1,stack2 = 0,0
    dataH1,dataH2 = 0,0

    for i in canvy1.GetListOfPrimitives():
        if "TH1" in i.ClassName(): dataH1 = i
        if i.ClassName() == "THStack":
            stack1 = i

    for i in canvy2.GetListOfPrimitives():
        if "TH1" in i.ClassName(): dataH2 = i
        if i.ClassName() == "THStack":
            stack2 = i

    nBins1 = dataH1.GetXaxis().GetNbins() 
    nBins2 = dataH2.GetXaxis().GetNbins() 

    combDataHist = TH1F("datacomb","datacomb",nBins1 + nBins2,0,nBins1 + nBins2)
    for i in range(1,nBins1+1):
        combDataHist.SetBinContent(i,dataH1.GetBinContent(i))
    for i in range(1,nBins2+1):                                                    
        combDataHist.SetBinContent(i+nBins1,dataH2.GetBinContent(i))

    print "Data",combDataHist.Integral()
    sampleNames = ["tW","singleTop","VV","ttbar","wPlusJets","zPlusJets","qcd"]
    combinedHists = {}

    newStack = THStack("combinedStack","combinedStack")

    for sample in sampleNames:
        print sample
        mergeHist1,mergeHist2 = 0,0
        for hist in stack1.GetHists():
            if sample in hist.GetName():
                mergeHist1 = hist
        for hist in stack2.GetHists():
            if sample in hist.GetName():
                mergeHist2 = hist
        combinedHists[sample] = TH1F(sample+"comb",sample+"comb",nBins1 + nBins2,0,nBins1 + nBins2)
        for i in range(1,nBins1+1):
            combinedHists[sample].SetBinContent(i,mergeHist1.GetBinContent(i))
        for i in range(1,nBins2+1):
            combinedHists[sample].SetBinContent(i+nBins1,mergeHist2.GetBinContent(i))
        combinedHists[sample].SetFillColor(mergeHist1.GetFillColor())
        newStack.Add(combinedHists[sample])
        print sample,combinedHists[sample].Integral(), mergeHist1.Integral(), mergeHist2.Integral()


    newCanvas = TCanvas("combinedHist","combinedHist",1000,800)
    newCanvas.cd()

    newStack.Draw("hist")
    combDataHist.Draw("e x0, same")

    newCanvas.SaveAs(outFileN+".root")
    newCanvas.SaveAs(outFileN+".png")

if __name__ == "__main__":
    inDirPostfix = "newReweighting"
#    plot1 = "08_HistogrammingElectron_ElePt"
    plot2 = "15_HistogrammingMtW_mTW"
    plot1 = "14_HistogrammingMET_MET_xy"
    outDir = "plots/fitPlots/"
    if not os.path.isdir(outDir): os.mkdir(outDir)
    regions = ["3j1t","2j1t","3j2t","4j1t","4j2t"]
#    regions = ["3j0t","2j0t","4j0t"]
    for region in regions:
        print region
        combineHists("plots/{0}{1}{2}/{3}.root".format(isEle,region,inDirPostfix,plot1),"plots/{0}{1}{2}/{3}.root".format(isEle,region,inDirPostfix,plot2),"{1}outPlot{0}".format(region,outDir))
        
