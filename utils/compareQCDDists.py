#A script that will get the shape missing in the plots and compare it the DD QCD estimate

from ROOT import *

import sys

gROOT.SetBatch()

inFile = TFile(sys.argv[1],"READ")
outFile = sys.argv[2]

dataHist = 0
mcStack = 0

for primy in inFile.GetListOfKeys():
    canvy = primy.ReadObj()
    for prim in canvy.GetListOfPrimitives():
        if "TH1F" in prim.ClassName():
            dataHist = prim
        if "THStack" in prim.ClassName():
            mcStack = prim

qcdPlot = 0
otherMCTotal = dataHist.Clone("totalMC")
otherMCTotal.Reset()
missingData = dataHist.Clone("leftoverData")

for hist in mcStack.GetHists():
#    print hist.GetName()
    if "qcd" in hist.GetName():
#        print hist.GetName()
        qcdPlot = hist.Clone("dd qcd")
    else:
        otherMCTotal.Add(hist)

missingData.Add(otherMCTotal,-1)

qcdPlot.Scale(1./qcdPlot.Integral())
missingData.Scale(1./missingData.Integral())

#print missingData.Integral()

canvy = TCanvas("compPlot","compPlot",1000,800)
canvy.cd()
qcdPlot.SetLineColor(kBlue)
qcdPlot.SetLineWidth(2)
qcdPlot.SetFillColor(0)
missingData.SetLineColor(kRed)
missingData.SetLineWidth(2)
missingData.SetFillColor(0)
leggy = TLegend(0.8,0.6,0.95,0.9)
leggy.SetFillStyle(1001)
leggy.SetBorderSize(1)
leggy.SetFillColor(0)
leggy.SetLineColor(0)
leggy.SetShadowColor(0)
leggy.SetFillColor(kWhite)
leggy.AddEntry(qcdPlot,"DD QCD","f")
leggy.AddEntry(qcdPlot,"Diff Data/MC","f")

missingData.Draw("h")
qcdPlot.Draw("same h")

leggy.Draw("same")

canvy.SaveAs(outFile+".root")
canvy.SaveAs(outFile+".png")
