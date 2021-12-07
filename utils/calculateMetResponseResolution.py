from ROOT import *

gROOT.SetBatch()

import os

from array import array

from analysisComponents import AnalysisComponents

def readOneDistribution(inHist,nDivisions,profileX):
    
    nBinsTotal = inHist.GetYaxis().GetNbins()
    binsPerDiv = nBinsTotal/nDivisions

    x = array('d',[])
    y = array('d',[])


    for i in range(nDivisions):
        lowerBin = i*binsPerDiv
        upperBin = (i+1)*binsPerDiv - 1
        profHist = inHist.ProfileX("profile",lowerBin,upperBin)
        u_ll_mean = profHist.GetMean()
        z_qt_mean = (inHist.GetYaxis().GetBinLowEdge(lowerBin) + inHist.GetYaxis().GetBinUpEdge(upperBin))/2.
        x.append(z_qt_mean)
        y.append(-u_ll_mean/z_qt_mean)
        print "{0:<3} {1:.1f} {2:.1f} {3:.3f}".format(i,z_qt_mean,u_ll_mean,-u_ll_mean/z_qt_mean)
    
    response = TGraph(nDivisions,x,y)

    return response


def plotResponse(response,name):
    
    canvy = TCanvas(name,name,1000,800)
    canvy.cd()

    leggy = TLegend(0.75,0.3,0.87,0.5)
    leggy.SetFillStyle(1001)
    leggy.SetBorderSize(1)
    leggy.SetFillColor(0)
    leggy.SetLineColor(0)
    leggy.SetShadowColor(0)
    leggy.SetFillColor(kWhite)

    mg = TMultiGraph(name,name)

    first = True
    markerStyle = 21
    markerColor = 3
    for key in response.keys():
        response[key].SetNameTitle(key,key)

        response[key].SetMaximum(1.2)
        response[key].SetMinimum(0.3)
        response[key].SetMarkerStyle(markerStyle)
        markerStyle+=1
        response[key].SetLineColor(markerColor)
        markerColor+=1
#        response[key].Draw("AC* {0}".format("same" if not first else ""))
        response[key].GetXaxis().SetTitle("Z q_{T}")
        response[key].GetYaxis().SetTitle("-<u_par>/<Z q_{T}>")
        mg.Add(response[key])
        leggy.AddEntry(response[key],key)

    mg.SetMaximum(1.2)
    mg.Draw("AC*")
    mg.GetXaxis().SetTitle("Z q_{T}")
    mg.GetYaxis().SetTitle("-<u_par>/<Z q_{T}>")
    mg.SetMinimum(0.3)

    leggy.Draw()

    canvy.SaveAs("{0}.root".format(name))
    canvy.SaveAs("{0}.png".format(name))
    canvy.SaveAs("{0}.pdf".format(name))


def calculateResponse(inDir,samples):

    print inDir

    inFiles = {}

    for sample in samples:
        if os.path.exists("{0}/{1}/hists/merged{1}.root".format(inDir,sample)):
            inFiles[sample] = TFile("{0}/{1}/hists/merged{1}.root".format(inDir,sample),"READ")
        else:
            print "could not find ","{0}/{1}/hists/merged{1}.root".format(inDir,sample)
    
    mets = ["MET","PuppiMET"]
#    mets = ["MET"]

    response = {}
    for met in mets:
        inHist = ""
        #First do wrt Zqt
        for sample in samples:
            
            if inHist == "": 
                inHist = inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_par_qt".format(met))
            else: 
                inHist.Add(inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_par_qt".format(met)))
                         
        nDivisions = 10
        
        response[met] = readOneDistribution(inHist,nDivisions,True)
  
    plotResponse(response,inDir)

if __name__ == "__main__":
    
    leptons = ["muon","electron"]
    years = ["2018","2017"]
    dataMC = ["","Data"]

    #reduced sets
 #   leptons = ["muon"]
 #   years = ["2018"]
    dataMC = [""]

    baseDirName = "met"

    for year in years:
        for lepton in leptons:
            for dmc in dataMC:
                comps = AnalysisComponents("met",lepton=lepton,era=year)
                
                samples = comps.samples if dmc == "" else comps.dataSamples
                
                directory = "{0}{1}{2}{3}".format(baseDirName,year[-2:],"Ele" if lepton == "electron" else "",dmc)
            
                calculateResponse(directory,samples)
            
