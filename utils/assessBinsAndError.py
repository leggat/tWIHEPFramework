#A script that will loop over the nominal BDT responses and calculate how many events are in each bin and the error on the bins.


from ROOT import *
import sys,numpy

inFile = TFile(sys.argv[1],"READ")

samples = ["tW","wPlusJets","ttbar","qcd","VV","zPlusJets","singleTop"]

binCount = numpy.zeros(30)
binErrors = numpy.zeros(30)
binCentre = numpy.zeros(30)

for sample in samples:
    print sample
    hist = inFile.Get(sample)
    for i in range(1,hist.GetXaxis().GetNbins()+1):
        binCount[i-1] += hist.GetBinContent(i)
        binErrors[i-1] += hist.GetBinError(i)
        binCentre[i-1] = hist.GetBinCenter(i)
        print hist.GetBinContent(i),hist.GetBinError(i)
    
print "All bins:"
for i in range(30):
    print "Bin: {0}\t({4:.2f})\t{1:2f}\t+-{2:.2f} ({3:.2f}%)".format(i+1,binCount[i],binErrors[i],100*(binErrors[i]/binCount[i]),binCentre[i])
