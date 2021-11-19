from ROOT import *

import sys




def compareDists(histOne,histTwo):

    for n in range(1,histOne.GetXaxis().GetNbins()+1):
        ratio = 1.0
        if histTwo.GetBinContent(n) > 0.: ratio =  histOne.GetBinContent(n)/histTwo.GetBinContent(n)
        print n, histOne.GetBinContent(n), histTwo.GetBinContent(n), ratio

if __name__ == "__main__":

    histOne = ""
    histTwo = ""

    #do the comparison from one internal file
    if True:
        inFile = TFile(sys.argv[1],"READ")
        
        inPlotOne = "01_HistogrammingNPvtx/nTrueInteractions"
        histOne = inFile.Get(inPlotOne)
        inPlotTwo = "01_HistogrammingNPvtx/nTrueInteractions_pre"
        histTwo = inFile.Get(inPlotTwo)

    #compare the pv input distributions
    if False:
        inF1 = TFile("config/weights/data_pileup_2018_true.root","READ")
        histOne = inF1.Get("pileup")
        inF2 = TFile("config/weights/pv_dy_2018_mc_trueInt.root","READ")
        histTwo = inF2.Get("pileup")
        

    histOne.Scale(1./histOne.Integral())
    histTwo.Scale(1./histTwo.Integral())
    compareDists(histOne,histTwo)
