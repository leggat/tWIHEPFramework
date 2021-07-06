from ROOT import *

gROOT.SetBatch()

import sys

if __name__ == "__main__":
    regions = ["3j1t","2j1t","4j1t"]
    regions = ["3j1t"]
    barEnd = ["Barrel","Endcap"]
    samples = ["wPlus0Jets","wPlus1Jets","wPlus2Jets","zPlusJetsLowMass","zPlusJetsHighMass"]
    inDir = "bdtReading/2021-03-02/"


    for barE in barEnd:
        print barE
        for region in regions:
            print region
            for sample in samples:
                print sample
                inFile = TFile("{0}{1}{2}/output_{3}.root".format(inDir,region,barE,sample),"READ")
                
                hist = inFile.Get("MVA_ttbar_bin1000_{0}".format(sample))
                print hist.Integral(),hist.GetEntries()
#                for i in range(1,hist.GetXaxis().GetNbins()+1):
#                    print i, hist.GetBinContent(i),hist.GetBinError(i)

    mvaDir = "mvaDir/2021-03-03/"
    for region in regions:
        print region
        inFile2 = TFile("{1}mvaDists_{0}_mu_bin1000.root".format(region,mvaDir))
        wJetComb = inFile2.Get("wPlusJets")
        print wJetComb.Integral(),wJetComb.GetEntries()
#        for i in range(1,wJetComb.GetXaxis().GetNbins()+1):
#            if wJetComb.GetBinContent(i) == 0.: continue
#            print "{0}: {1:.1f} {2:.2f}".format(i,wJetComb.GetBinContent(i),wJetComb.GetBinError(i))
