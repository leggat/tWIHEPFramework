from ROOT import *

gROOT.SetBatch()

import sys

inFile = TFile(sys.argv[1],"READ")

hist = inFile.Get("MVA_ttbar_bin1000_tW_top_nfh")

canvas = TCanvas("canvas","canvas",1000,1000)

hist.Rebin(5)
hist.Draw()
hist.GetXaxis().SetRangeUser(-0.55,0.6)
canvas.SaveAs("quickDump.png")
