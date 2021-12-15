from ROOT import *

gROOT.SetBatch()

import os

from array import array

from analysisComponents import AnalysisComponents

def readTwoDistributions(inHistOne,inHistTwo,binEdges,profileX,profileEachBin=False,upperLimit=-1):

    x = array('d',[])
    y = array('d',[])
    exl = array('d',[])
    exh = array('d',[])
    eyl = array('d',[])
    eyh = array('d',[])

#    if upperLimit > 0: nDivisions = upperLimit

    for i in range(1,len(binEdges)):
        lowerBin = inHistOne.GetYaxis().FindBin(binEdges[i-1])
        upperBin = inHistOne.GetYaxis().FindBin(binEdges[i])
        profHist1 = inHistOne.ProfileX("profile1",lowerBin,upperBin)
        
        

        u_ll_mean = profHist1.GetMean()
        
        profHist2 = inHistTwo.ProfileX("profile2",lowerBin,upperBin)
        z_qt_mean = profHist2.GetMean()

        binCenter = (binEdges[i-1]+binEdges[i])/2.
        binWidth = binEdges[i]-binEdges[i-1]

        x.append(binCenter)
        exl.append(binWidth/2.)
        exh.append(binWidth/2.)
        if z_qt_mean == 0:
            print "Below 0"
            y.append(0.0)
            continue
        y.append(-u_ll_mean/z_qt_mean)
        eyl.append(0.001)
        eyh.append(0.001)

        print "{0:<3} {1:.1f} {2:.1f} {3:.3f}".format(i,z_qt_mean,u_ll_mean,-u_ll_mean/z_qt_mean)

    response = TGraphAsymmErrors(len(binEdges)-1,x,y,exl,exh,eyl,eyh)

    return response


def readOneDistribution(inHist,binEdges,profileX):
    

    x = array('d',[])
    y = array('d',[])
    exl = array('d',[])
    exh = array('d',[])
    eyl = array('d',[])
    eyh = array('d',[])

    for i in range(1,len(binEdges)):
        lowerBin = inHist.GetYaxis().FindBin(binEdges[i-1])
        upperBin = inHist.GetYaxis().FindBin(binEdges[i])  

        profHist = inHist.ProfileX("profile",lowerBin,upperBin)
        u_ll_mean = profHist.GetMean()
        z_qt_mean = (binEdges[i-1]+binEdges[i])/2.
        binWidth = binEdges[i]-binEdges[i-1]
        x.append(z_qt_mean)
        exl.append(binWidth/2.)
        exh.append(binWidth/2.)
        eyl.append(0.001)
        eyh.append(0.001)
        y.append(-u_ll_mean/z_qt_mean)
        print "{0:<3} {1:.1f} {2:.1f} {3:.3f}".format(i,z_qt_mean,u_ll_mean,-u_ll_mean/z_qt_mean)
    
    response = TGraphAsymmErrors(len(binEdges)-1,x,y,exl,exh,eyl,eyh)

    return response


def plotResponse(response,name,outDir,xAxis="Z q_{T}"):
    
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
        response[key].GetXaxis().SetTitle(xAxis)
        response[key].GetYaxis().SetTitle("-<u_par>/<Z q_{T}>")
        mg.Add(response[key])
        leggy.AddEntry(response[key],key)

    mg.SetMaximum(1.2)
    mg.Draw("AC*")
    mg.GetXaxis().SetTitle(xAxis)
    mg.GetYaxis().SetTitle("-<u_par>/<Z q_{T}>")
    mg.SetMinimum(0.3)

    leggy.Draw()

    canvy.SaveAs("{1}{0}.root".format(name,outDir))
    canvy.SaveAs("{1}{0}.png".format(name,outDir))
    canvy.SaveAs("{1}{0}.pdf".format(name,outDir))


def calculateResponse(inDir,samples,outDir):

    print inDir

    inFiles = {}

    for sample in samples:
        if os.path.exists("{0}/{1}/hists/merged{1}.root".format(inDir,sample)):
            inFiles[sample] = TFile("{0}/{1}/hists/merged{1}.root".format(inDir,sample),"READ")
        else:
            print "could not find ","{0}/{1}/hists/merged{1}.root".format(inDir,sample)
    
    mets = ["MET","PuppiMET","RawMET"]
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
        
        binEdges = []
        maxValue = inHist.GetYaxis().GetBinUpEdge(inHist.GetYaxis().GetNbins()+1)
        for i in range(nDivisions+1): binEdges.append(i*(float(maxValue)/nDivisions))
        binEdges = [0,10,20,30,40,50,60,80,110,150,200,300,500]
#[f for f in range(0,inHist.GetYaxis().GetNbins()+inHist.GetYaxis().GetNbins()/nDivisions,inHist.GetYaxis().GetNbins()/nDivisions)]
        print binEdges

        response[met] = readOneDistribution(inHist,binEdges,True)


    plotResponse(response,inDir+"zQt",outDir)

    response = {}
    for met in mets:
        inHist1 = ""
        inHist2 = ""
        nDivisions = 80
        binEdges = [f for f in range(61)]
        print binEdges
        for sample in samples:
            if inHist1 == "":
                inHist1 = inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_par_nVtx".format(met))
                inHist2 = inFiles[sample].Get("12_HistogrammingMETPaper/ZQt_nVertex")
            else:
                inHist1.Add(inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_par_nVtx".format(met)))
                inHist2.Add(inFiles[sample].Get("12_HistogrammingMETPaper/ZQt_nVertex"))
        response[met] = readTwoDistributions(inHist1,inHist2,binEdges,True,profileEachBin=True,upperLimit=60)

    plotResponse(response,inDir+"_nVtx",outDir,"nVtx")
    

if __name__ == "__main__":
    
    leptons = ["muon","electron"]
    years = ["2018","2017","2016","2016APV","2016Total"]
    dataMC = ["","Data"]

    #reduced sets
 #   leptons = ["muon"]
 #   years = ["2018"]
    dataMC = [""]

    baseDirName = "met"

    outDir = "responseResPlots/"

    if not os.path.exists(outDir): os.mkdir(outDir)
        

    for year in years:
        for lepton in leptons:
            for dmc in dataMC:
                comps = AnalysisComponents("met",lepton=lepton,era=year)
                
                samples = comps.samples if dmc == "" else comps.dataSamples
                
                directory = "{0}{1}{2}{3}".format(baseDirName,year[2:],"Ele" if lepton == "electron" else "",dmc)
            
                calculateResponse(directory,samples,outDir)
            
