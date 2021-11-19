from ROOT import *

import sys

inFileName = sys.argv[1]

inFile = TFile(inFileName,"READ")

plotPaths = []

for obj in inFile.GetListOfKeys():
    tempThing = inFile.Get(obj.GetName())
    if not tempThing.ClassName().find("TH1") == -1 : plotPaths.append(tempThing.GetName())
    if not tempThing.ClassName().find("Directory") == -1:
        for k2 in tempThing.GetListOfKeys():
            temp2 = tempThing.Get(k2.GetName())
            if not temp2.ClassName().find("TH1") == -1:
                plotPaths.append(tempThing.GetName() + "/"+ temp2.GetName())

plotsToConsider = ["01_HistogrammingNPvtx/nTrueInteractions_pre"]
for plotName in plotPaths:
    print plotName
    considerPlot = False
    for name in plotsToConsider:
        if name in plotName: 
            considerPlot = True
            break

    if not considerPlot: continue
    
    histToSave = inFile.Get(plotName)

    histClone = histToSave.Clone("pileup")
    histClone.Scale(1./histClone.Integral())

    histClone.SaveAs("pvDist.root")
