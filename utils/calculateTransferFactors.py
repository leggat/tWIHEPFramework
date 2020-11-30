from ROOT import *

import sys,os

regions = ["2j1t","3j1t","4j1t","3j2t"]

qcdMC2j = 1.
qcdDD2j = 1.

barEndMap = {"barrel":"Barrel","endcap":"Endcap"}

def getQCDHist(inFile):
    qcdHist = 0
    wJetHist = 0
    for primy in inFile.GetListOfKeys():
        canvy = primy.ReadObj()
        for prim in canvy.GetListOfPrimitives():
            if "THStack" in prim.ClassName():
                for hist in prim.GetHists():
                    if "qcd" in hist.GetName(): qcdHist = hist
                    if "wPlus" in hist.GetName(): wJetHist = hist
    return qcdHist,wJetHist

qcdMCMap,wJetMCMap = {},{}
qcdDDMap,wJetDDMap = {},{}
qcdWeightMap,wJetWeightMap = {},{}
qcdSameMap,wJetSameMap = {},{}

weighted = "forTransferWeighted"
weighted = "transferFactors"

for det in barEndMap.keys():
    print det
    for region in regions:
        print region
        inFileMC = TFile("plots/{0}{1}NoDDforTransferFactor/16_HistogrammingMtW_mTW.root".format(barEndMap[det],region,det),"r")
        qcdMC,wJetMC = getQCDHist(inFileMC)
        qcdMCMap[region+det],wJetMCMap[region+det] = qcdMC.Integral(),wJetMC.Integral()

        inFileDD = TFile("plots/{0}{1}forTransferFactor/16_HistogrammingMtW_mTW.root".format(barEndMap[det],region,det),"r")
        qcdDD,wJetDD = getQCDHist(inFileDD)
        qcdDDMap[region+det],wJetDDMap[region+det] = qcdDD.Integral(),wJetDD.Integral()
        
        inFileWeighted = TFile("plots/{0}{1}{2}/16_HistogrammingMtW_mTW.root".format(barEndMap[det],region,weighted,det),"r")
        qcdWeight,wJetWeight = getQCDHist(inFileWeighted)
        qcdWeightMap[region+det],wJetWeightMap[region+det] = qcdWeight.Integral(),wJetWeight.Integral()

        inFileSameWeight =  TFile("plots/{0}{1}allSameWeight/16_HistogrammingMtW_mTW.root".format(barEndMap[det],region,det),"r")
        qcdSame,wJetSame = getQCDHist(inFileSameWeight)
        qcdSameMap[region+det],wJetSameMap[region+det] = qcdSame.Integral(),wJetSame.Integral()

        print "QCD\nMC: {0:.1f} DD: {1:.1f} Fin: {5:.1f} Same: {6:.1f} \nWJ\nMC: {2:.1f} DD: {3:.1f} Fin: {4:.1f} Same: {7:.1f}".format(qcdMC.Integral(),qcdDD.Integral(),wJetMC.Integral(),wJetDD.Integral(),wJetWeight.Integral(),qcdWeight.Integral(),qcdSame.Integral(),wJetSame.Integral())
        
        print "QCD mc/dd:", qcdMC.Integral()/qcdDD.Integral()

        if "2j1t" in region:
            qcdMC2j = qcdMC.Integral()
            qcdDD2j = qcdDD.Integral()
        else:
            print "QCD transfer factors:\nMC: {0:.2f} DD: {1:.2f}".format(qcdMC.Integral()/qcdMC2j,qcdDD.Integral()/qcdDD2j)

print "Region & MC temp & DD & Fitted \\\\\nQCD"
for region in regions:
    for det in barEndMap.keys():
        print "{0} {6} & {1:.1f} & {2:.1f} ({3:.1f}\%) & {4:.1f} ({5:.1f}\%) & {7:.2f} & {8:.1f} ({9:.1f}\%) & {10:.2f} & {11:.6f}\\\\".format(region,qcdMCMap[region+det],qcdDDMap[region+det],100*qcdDDMap[region+det]/qcdMCMap[region+det],qcdWeightMap[region+det],100*qcdWeightMap[region+det]/qcdMCMap[region+det],det,qcdDDMap[region+det]/qcdWeightMap[region+det],qcdSameMap[region+det],100*qcdSameMap[region+det]/qcdMCMap[region+det],qcdDDMap[region+det]/qcdSameMap[region+det],(qcdSameMap["2j1t"+det]/qcdMCMap["2j1t"+det])/(qcdSameMap[region+det]/qcdMCMap[region+det]))
    
print "wJet"
for region in regions:
    for det in barEndMap.keys():
        print "{0} {6} & {1:.1f} & {2:.1f} ({3:.1f}\%) & {4:.1f} ({5:.1f}\%) & {7:.2f} & {8:.1f} ({9:.1f}\%) \\\\".format(region,wJetMCMap[region+det],wJetDDMap[region+det],100*wJetDDMap[region+det]/wJetMCMap[region+det],wJetWeightMap[region+det],100*wJetWeightMap[region+det]/wJetMCMap[region+det],det,0.,wJetSameMap[region+det],100*wJetSameMap[region+det]/wJetMCMap[region+det])
