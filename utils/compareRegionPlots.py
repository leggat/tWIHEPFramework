from ROOT import *

import sys,os, analysisComponents

gROOT.SetBatch()

def getAllRegionPlots(fileName,plotName,dirPostfix="",dirPrefix="",template="wPlusJets"):
    components = analysisComponents.AnalysisComponents()
    returnHists = {}
    for region in components.regions:
        inFile = TFile("plots/{3}{0}{1}/{2}.root".format(region,dirPostfix,fileName,dirPrefix),"READ")
        for item in inFile.GetListOfKeys():
            canvy = item.ReadObj()
            print canvy.GetName()
            print canvy.ClassName()
            for item2 in canvy.GetListOfPrimitives():
                if item2.ClassName() == "THStack":
                    print "Stack:",item2.GetName()
                    for hist in item2.GetHists():
                        print hist.GetName()
                        if template in hist.GetName(): 
                            returnHists[region] = hist
    return returnHists
    

def produceComparisonPlot(hists,outDir,histName,latexFile = ""):
    compColors = {"3j1t":kGreen+2,"2j1t":kYellow,"3j2t":kBlue,"4j1t":kGray,"4j2t":kPink}
    leggy = TLegend(0.8,0.6,0.95,0.9)
    leggy.SetFillStyle(1001)
    leggy.SetBorderSize(1)
    leggy.SetFillColor(0)
    leggy.SetLineColor(0)
    leggy.SetShadowColor(0)
    leggy.SetFillColor(kWhite)
    saveHistName = ""
    if latexFile:
#        latexFile.write("\\frame{\n\\frametitle{comparison}")
        nLine = 0
    comparisonCanvas = TCanvas(histName+"comp",histName+"comp",1000,800)
    for region in hists.keys():
        hist = hists[region]
        saveHistName = hist.GetName()
        canvy = TCanvas(hist.GetName()+region,hist.GetName()+region,1000,800)
        canvy.cd()
        hist.SetLineColor(kBlack)
        hist.SetFillColor(kWhite)
        hist.Draw()
        canvy.SaveAs("{0}/{1}.png".format(outDir,histName+region))
        canvy.SaveAs("{0}/{1}.root".format(outDir,histName+region))
        if latexFile:
            latexFile.write("\\frame{\n\\frametitle{"+region+"}\n\\includegraphics[width=0.9\\textwidth]{"+histName+region+".png}\n}\n")
#            if nLine % 2 == 0:
#                latexFile.write("\\\\\n")
#            nLine+=1
        comparisonCanvas.cd()
        if region in compColors.keys():
            hist.SetLineColor(compColors[region])
            hist.SetLineWidth(2)
            hist.SetFillColor(0)
            hist.Scale(1./hist.Integral())
            leggy.AddEntry(hist,region,"f")
            hist.Draw("same")
    comparisonCanvas.cd()
    leggy.Draw("same")
    comparisonCanvas.SaveAs("{0}/{1}comp.png".format(outDir,histName))
    comparisonCanvas.SaveAs("{0}/{1}comp.root".format(outDir,histName))
        
    if latexFile:
        latexFile.write("\\frame{\n\\frametitle{"+" comparison}\n\\includegraphics[width=0.9\\textwidth]{"+histName+"comp.png}\n}\n")


#def initialiseLatexFile =

if __name__ == "__main__":
    dirPostfix = ""
    dirPostfix = "NoDDfittedWith0Tag"
    fileName = "22_Variables_JESBDTVars_M_DeltaRLeptonJet1_nom"
    fileName = "15_HistogrammingMtW_mTW"
#    fileName = "10_HistogrammingElectron_Ele1Pt"
    plotName = "22_Variables_JESBDTVars/M_DeltaRLeptonJet1_nom"
    plotName = "15_HistogrammingMtW_MtW"
#    plotName = "10_HistogrammingElectron_Ele1Pt"
    dirPrefix = "Ele"
    histsToCompare = getAllRegionPlots(fileName,plotName,dirPostfix,dirPrefix,"qcd")
    outDir = "wJetComps"
    if not os.path.exists(outDir):
        os.makedirs(outDir)
    latexFile = open(outDir+"/wJetRegionComps.tex","w")
    latexFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")

    produceComparisonPlot(histsToCompare,outDir,plotName,latexFile)
    
    latexFile.write("\\end{document}")
