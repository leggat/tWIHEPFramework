#A script thta will shift the data by a few times the tW dataset so that we can do the fit for a bias scan

samples  = ["tW","zPlusJets","VV","qcd","ttbar","singleTop","wPlusJets","data_obs"]

from ROOT import *
import sys,os

def makeOneIncrement(canvy,scale,outFileName):
    
    dataHist,stack = 0,0
    for i in canvy.GetListOfPrimitives():
        if i.ClassName() == "THStack": stack = i
        if i.ClassName() == "TH1F": dataHist = i

    hists = {}
    hists["data_obs"] = dataHist
    for hist in stack.GetHists():
        for sample in samples:
            if sample in hist.GetName():
                hists[sample] = hist

    for i in range(1,dataHist.GetXaxis().GetNbins()+1):
        dataHist.SetBinContent(i,dataHist.GetBinContent(i)+(scale-1)*hists["tW"].GetBinContent(i))

    gPad.Update()
#    canvy.cd()
#    stack.Draw()
#    dataHist.Draw("same")
#    print "write?"
    canvy.SaveAs(outFileName+".png")    
    canvy.SaveAs(outFileName+".jpg")    
    canvy.SaveAs(outFileName+".pdf")
    
    canvy.SaveAs(outFileName+".root")

def makeBiasScan(inFileName):
    inFile = TFile("{0}".format(inFileName),"READ")
    #Get the canvas
    canvy = 0
    for prim in inFile.GetListOfKeys(): canvy = prim.ReadObj()

#    canvy.SaveAs("test.root")
#    canvy.Print("test.png")
#    canvy.SaveAs("test.jpg")
#    canvy.SaveAs("test.pdf")
#
#    canvyOne = canvy.Clone()
    scaleMap = {"0p0":0.,"0p3":0.3,"0p5":0.5,"0p7":0.7,"1p3":1.3,"1p5":1.5,"1p7":1.7,"2p0":2.0}
    for scaleName in scaleMap.keys():
        canvyOne = canvy.Clone()
        makeOneIncrement(canvyOne,scaleMap[scaleName],"{0}_{1}".format(inFileName.split(".")[0],scaleName))

    #get the histograms

                
    


if __name__ == "__main__":
#    inFileName = sys.argv[1]
    regions = ["3j1t","3j2t","2j1t","4j1t"]
    regions = ["2j0t","3j0t","4j0t"]
    postfixName = "iteration0"
    histogramName = "16_HistogrammingMtW_mTW.root"
    for region in regions:
        print region,
        for barEnd in ["Barrel","Endcap"]:
            print barEnd
            makeBiasScan("plots/{0}{1}{2}/{3}".format(barEnd,region,postfixName,histogramName))
