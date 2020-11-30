import sys, os

from ROOT import *

gROOT.SetBatch()

chain = TChain("TNT/BOOM")

dirToCheck = "tW/tW_mcanlo/skims/"
dirToCheck = "tW/tW_top_nfh/skims/"

files = [f for f in os.listdir(dirToCheck) if ".root" in f]

for fileN in files:
    chain.Add(dirToCheck+fileN)

print chain.GetEntries()

chain.Draw("channel")

htemp = gPad.GetPrimitive("htemp")



print htemp.Integral()

for i in range(1,htemp.GetXaxis().GetNbins()+1):
    print "{0}:\t{1:.1f}".format(i,htemp.GetBinContent(i))

weightedRegions = htemp.Clone("weightedRegions")

weightedRegions.Reset()

for i in range(chain.GetEntries()):
    chain.GetEntry(i)
#    print chain.channel,chain.EventWeight
    weightedRegions.Fill(chain.channel,chain.EventWeight)

#globalReweight = 1.28702/18.1799
globalReweight = 1.

for i in range(1,htemp.GetXaxis().GetNbins()+1):
    print "{0}:\t{1:.1f}".format(i,globalReweight * weightedRegions.GetBinContent(i))
