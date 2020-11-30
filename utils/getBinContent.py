#A script that will add all bins from 

from ROOT import *

import sys, os

inDir = sys.argv[1]
plotName = sys.argv[2]
binNumber = sys.argv[3]

directoriesToLoop = []

if "Data" in inDir:
    if "Ele" in inDir:
        directoriesToLoop.append("singleEle")
    else:
        directoriesToLoop.append("singleMu")
else:
    directoriesToLoop = [f for f in os.listdir(inDir)]

print directoriesToLoop

totalContent = 0.
totalWithoutQCD = 0.

for directory in directoriesToLoop:
    print directory,
    tempRootFile = TFile("{0}/{1}/hists/merged{1}.root".format(inDir,directory),"READ")

    hist = tempRootFile.Get(plotName)
    print " {0} ({1}), Integral: {2}".format(hist.GetBinContent(int(binNumber)),hist.GetBinError(int(binNumber)),hist.Integral())
    totalContent += hist.GetBinContent(int(binNumber))
    if not "qcd" in directory: totalWithoutQCD += hist.GetBinContent(int(binNumber))

print "Total :",totalContent
print "Total (no QCD): ", totalWithoutQCD
