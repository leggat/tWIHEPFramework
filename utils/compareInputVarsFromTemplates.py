#A script that will let me compare the nominal BDT input distributions with those from another file for a specific sample and systematic.


from ROOT import *

import sys,os,analysisComponents

from setTDRStyle import setTDRStyle
setTDRStyle()

comp = analysisComponents.AnalysisComponents()

xAxisLabels = comp.xAxisLabels

varsToCheck = ["M_DeltaRlightjets",
"M_hadronicWmass",
"M_DeltaRBJetLepton",
"M_DeltaRWlvJet2",
"M_Mass_Jet1Jet3",
#"M_nJet2040",
#"M_Pt_AllJets2040",
"M_Pt_Lepton",
"M_E_Jet2Jet3",
"M_Pt_AllJetsLeptonMET"]

#make an output directory:
outDir = "plots/comparePlots/"
if not os.path.exists(outDir): os.makedirs(outDir)

gROOT.SetBatch()

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

ratioMax = 1.05
ratioMin = 0.95

latex2 = TLatex();
latex2.SetNDC();
latex2.SetTextSize(0.04);
latex2.SetTextAlign(31);

cwd = os.getcwd()
leptonInLabel = "#mu"
if "elect" in cwd: leptonInLabel = "e"

text2 = TLatex(0.45,0.98, "{0} channel".format(leptonInLabel))
text2.SetNDC()
text2.SetTextAlign(13)
text2.SetX(0.18)
text2.SetY(0.92)
text2.SetTextFont(42)
text2.SetTextSize(0.0610687)


includeBackup = True

cmsText = "CMS"
extraText = "Preliminary"

rebin = 1

def makeSystHist(nominalHist,upHist,downHist,canvasName,region="_3j1t"):
    
#    nominalHist.Rebin(nominalHist.GetXaxis().GetNbins()/rebin)
#    upHist.Rebin(upHist.GetXaxis().GetNbins()/rebin)
#    downHist.Rebin(downHist.GetXaxis().GetNbins()/rebin)
    canvy = TCanvas(canvasName+region,canvasName+region,1000,800)
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

    latex2.DrawLatex(0.95, 0.95, canvasName+region);

    text2.Draw()

    ratioCanvy = TPad("mva_ratio","mva_ratio",0.0,0.0,1.0,1.0)
    ratioCanvy.SetTopMargin(0.7)
    ratioCanvy.SetFillColor(0)
    ratioCanvy.SetFillStyle(0)
    ratioCanvy.SetGridy(1)
    ratioCanvy.Draw()
    ratioCanvy.cd(0)
    SetOwnership(ratioCanvy,False)

    upHistRatio = upHist.Clone()
    upHistRatio.Divide(nominalHist)
#    upHistRatio.SetMaximum(ratioMax)
#    upHistRatio.SetMinimum(ratioMin)
    upHistRatio.Draw("hist same")
    downHistRatio = downHist.Clone()
    downHistRatio.Divide(nominalHist)
    maximum = upHistRatio.GetMaximum()
    if downHistRatio.GetMaximum() > maximum: maximum = downHistRatio.GetMaximum()
    diff = maximum-1
#    upHistRatio.SetMaximum(1+(diff * 1.1))
#    upHistRatio.SetMinimum(1-(diff*1.1))
    upHistRatio.SetMaximum(1.1)
    upHistRatio.SetMinimum(0.9)

    #    print maximum,2-(maximum*1.1),upHistRatio.GetMaximum(),upHistRatio.GetMinimum()
    for key in xAxisLabels.keys():
        if key in canvasName:     upHistRatio.GetXaxis().SetTitle(xAxisLabels[key])
    downHistRatio.Draw("hist same")

    canvy.SaveAs(outDir+canvasName+".png")


def processOneRegion(inFiles,upFile,downFile,varsOfInterest,sampleName,systSampleName,region):
    for var in varsOfInterest:
        print "{0}_{1}".format(var,sampleName), "{0}_{1}_{2}up".format(var,sampleName,systSampleName)
        nomHist = 0
        for i in range(len(inFiles)):  
            if i == 0:
                nomHist  = inFiles[i].Get("{0}_{1}".format(var,sampleName))
            else:
                nomHist.Add(inFiles[i].Get("{0}_{1}BU".format(var,sampleName)))
        upHist   = upFile.Get("{0}_{1}_{2}up".format(var,sampleName,systSampleName))
        downHist = downFile.Get("{0}_{1}_{2}down".format(var,sampleName,systSampleName))
        
        makeSystHist(nomHist,upHist,downHist,"{2}_{0}_{1}_{3}".format(sampleName,systSampleName,var,region),"")

if __name__ == "__main__":
    regions = ["3j1tBarrel","3j1tEndcap","2j1tBarrel","2j1tEndcap","4j1tBarrel","4j1tEndcap"]
    directoryPrefix = "bdtReading/2020-11-23"
    sampleName = "ttbar"
    systSampleName = "tune"
    for region in regions:
        nomFileName = "{1}/{0}/output_{2}.root".format(region,directoryPrefix,sampleName)
        upFileName = "{1}/Systs{0}/output_{2}_{3}up.root".format(region,directoryPrefix,sampleName,systSampleName) 
        downFileName = "{1}/Systs{0}/output_{2}_{3}down.root".format(region,directoryPrefix,sampleName,systSampleName) 

        ttbarFiles = []
        ttbarFiles.append(TFile(nomFileName,"READ"))
        if includeBackup:
            nomFileName2 = "{1}/{0}/output_{2}BU.root".format(region,directoryPrefix,sampleName)
            ttbarFiles.append(TFile(nomFileName2,"READ"))
        ttbarUpFile = TFile(upFileName,"READ")
        ttbarDownFile = TFile(downFileName,"READ")

        processOneRegion(ttbarFiles,ttbarUpFile,ttbarDownFile,varsToCheck,sampleName,systSampleName,region)

    doLatex =     False
    channelMap = {"REPLACEMUON":"Muon channel","REPLACEELECTRON":"Electron channel"}
    if doLatex:
        latexFile = open(outDir+"latexFile.tex","w")
        latexFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")
        for var in varsToCheck:
            for barEnd in ["Barrel","Endcap"]:
                latexFile.write("\\frame{{\n\\frametitle{{{0} - {1}}}\n".format("+".join(var.split("_")),barEnd))
                for tempDir in ["REPLACEMUON","REPLACEELECTRON"]:
                    latexFile.write("{0}\n".format(channelMap[tempDir]))
                    for region in ["3j1t","2j1t","4j1t"]:
                        latexFile.write("\\includegraphics[width=0.3\\textwidth]{{{0}/{1}_{2}_{3}_{4}{5}.png}}\n".format(tempDir,var,sampleName,systSampleName,region,barEnd))
                    latexFile.write("\\\\\n")
                latexFile.write("\n}\n")
        latexFile.write("\\end{document}")
        latexFile.close()

                    
    
