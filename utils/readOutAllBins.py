#Loop over all hists in a stack and print out all bins in a distribution

from ROOT import *

import sys,os

#

def getOnePlotIntegral(inFileName):
    if not os.path.exists(inFileName): return 1.,1.
    inFile = TFile(inFileName,"READ")
    integrals = {}

    totalIntegralMC = 0.
    dataInt = 0.

    qcdInt = 0.
    wPlusInt = 0.

    for primy in inFile.GetListOfKeys():
        canvy = primy.ReadObj()
        for prim in canvy.GetListOfPrimitives():
#            print prim.GetName(), prim.ClassName()
            if "TH1F" in prim.ClassName():
                integrals[prim.GetName()] = prim.Integral()       
                dataInt =  prim.Integral()
#                for i in range(1,prim.GetXaxis().GetNbins() + 1): 
#                    print prim.GetBinContent(i)                   

            if  "THStack" in prim.ClassName():
                for hist in prim.GetHists():
#                    print hist.GetName()
                    integrals[hist.GetName()] = hist.Integral()
                    if "qcd" in hist.GetName(): qcdInt = hist.Integral()
                    if "wPlusJ" in hist.GetName(): wPlusInt = hist.Integral()
                    totalIntegralMC += hist.Integral()
#                    for i in range(1,hist.GetXaxis().GetNbins() + 1):
#                        print hist.GetBinContent(i)

            #print prim.GetName()
#    for intName in integrals.keys():
#        print "{0}: \t{1:.1f}".format(intName, integrals[intName])

#    print "Data Integral: {0} \nMC Integral {1}\nData to MC ratio{2}".format(dataInt,totalIntegralMC,dataInt/totalIntegralMC)
    
    return qcdInt,wPlusInt

if __name__ == "__main__":
    
    prefitPostfix = "NoDDpreFit"
    postfitPostfix = "same2j1t_fitted"
    dist = "16_HistogrammingMtW_mTW.root"

    regions  = ["3j1t","3j2t","2j1t","4j1t","2j0t","3j0t","4j0t"]
    for region in regions:
        for barEnd in ["Barrel","Endcap"]:
            print region, barEnd
            preFitQCD,preFitwPlus = getOnePlotIntegral("plots/{0}{1}{2}/{3}".format(barEnd,region,prefitPostfix,dist))
            postFitQCD,postFitwPlus = getOnePlotIntegral("plots/{0}{1}{2}/{3}".format(barEnd,region,postfitPostfix,dist))
            print "QCD: {0:1f} {1:.1f} {2:.2f}".format( preFitQCD, postFitQCD, postFitQCD/preFitQCD)
            print "wPlus: {0:1f} {1:.1f} {2:.2f}".format( preFitwPlus, postFitwPlus, postFitwPlus/preFitwPlus)
