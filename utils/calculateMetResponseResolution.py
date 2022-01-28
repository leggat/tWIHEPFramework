from ROOT import *

gROOT.SetBatch()

import os

from array import array

from analysisComponents import AnalysisComponents

import genericPlottingMacro

def readTwoDistributions(inHistOne,inHistTwo,uPerpHist,binEdges,profileX,profileEachBin=False,upperLimit=-1):

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

        profuPerp = uPerpHist.ProfileX("prof_perp",lowerBin,upperBin)

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


def readOneDistribution(inHist,zqtHist,uPerpHist,uParHist,binEdges,profileX,zqtIs1D=True):
    

    x = array('d',[])
    y = array('d',[])
    exl = array('d',[])
    exh = array('d',[])
    eyl = array('d',[])
    eyh = array('d',[])

    yRes_par = array('d',[])
    yRes_perp = array('d',[])

    yRes_par_raw = array('d',[])
    yRes_perp_raw = array('d',[])

    for i in range(1,len(binEdges)):
        lowerBin = inHist.GetYaxis().FindBin(binEdges[i-1])
        upperBin = inHist.GetYaxis().FindBin(binEdges[i]) -1

        profHist = inHist.ProfileX("profile",lowerBin,upperBin)
        u_ll_mean = profHist.GetMean()
        u_ll_rms = profHist.GetRMS()
        z_qt_mean = 0.
        if zqtIs1D:
            total_zqt = 0.

            for j in range(lowerBin,upperBin+1):
                total_zqt += zqtHist.GetBinContent(j)*zqtHist.GetXaxis().GetBinCenter(j)
            z_qt_mean = total_zqt/zqtHist.Integral(lowerBin,upperBin)
        else:
            prof1Hist = zqtHist.ProfileX("profile_2",lowerBin,upperBin)
            z_qt_mean = prof1Hist.GetRMS()
        binWidth = binEdges[i]-binEdges[i-1]
        binCenter = (binEdges[i]+binEdges[i-1])/2.
        x.append(binCenter)
        exl.append(binWidth/2.)
        exh.append(binWidth/2.)
        eyl.append(0.001)
        eyh.append(0.001)
        yValue = 0.
        if not z_qt_mean == 0.: 
            yValue = -u_ll_mean/z_qt_mean
        y.append(yValue)

        #profile the u plots
        profile_perp = uPerpHist.ProfileX("prof_perp",lowerBin,upperBin)
        profile_par = uParHist.ProfileX("prof_par",lowerBin,upperBin)
        
        #and now calculate the resolution why not?
        par_res_raw = profile_par.GetRMS()
        par_res = par_res_raw
        perp_res_raw = profile_perp.GetRMS()
        perp_res = perp_res_raw
        if not yValue == 0.:
            par_res /= yValue
            perp_res /= yValue
        yRes_par.append(par_res)
        yRes_par_raw.append(par_res_raw)
        yRes_perp.append(perp_res)
        yRes_perp_raw.append(perp_res_raw)
        

        print "{0:<3} Bins: {4}-{5} zQT: {1:.1f} u_ll: {2:.1f} response: {3:.3f} ".format(i,z_qt_mean,u_ll_mean,yValue,lowerBin,upperBin)
    
    response = TGraphAsymmErrors(len(binEdges)-1,x,y,exl,exh,eyl,eyh)
    resolution_par = TGraphAsymmErrors(len(binEdges)-1,x,yRes_par,exl,exh,eyl,eyh)
    resolution_par_raw = TGraphAsymmErrors(len(binEdges)-1,x,yRes_par_raw,exl,exh,eyl,eyh)
    resolution_perp = TGraphAsymmErrors(len(binEdges)-1,x,yRes_perp,exl,exh,eyl,eyh)
    resolution_perp_raw = TGraphAsymmErrors(len(binEdges)-1,x,yRes_perp_raw,exl,exh,eyl,eyh)

    return response,resolution_par,resolution_perp,resolution_par_raw,resolution_perp_raw


def plotResponse(response,name,outDir,xAxis="Z q_{T}",yAxis="-<u_par>/<Z q_{T}>",maximum=1.2,minimum=0.3):
    
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

        response[key].SetMaximum(maximum)
        response[key].SetMinimum(minimum)
        response[key].SetMarkerStyle(markerStyle)
        markerStyle+=1
        response[key].SetLineColor(markerColor)
#        response[key].SetMarkerColor(markerColor)
        markerColor+=1
#        response[key].Draw("AC* {0}".format("same" if not first else ""))
        response[key].GetXaxis().SetTitle(xAxis)
        response[key].GetYaxis().SetTitle(yAxis)
        mg.Add(response[key])
        leggy.AddEntry(response[key],key,"lep")

    mg.SetMaximum(maximum)
    mg.Draw("APC")
    mg.GetXaxis().SetTitle(xAxis)
    mg.GetYaxis().SetTitle(yAxis)
    mg.SetMinimum(minimum)

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
    res_par = {}
    res_par_raw = {}
    res_perp = {}
    res_perp_raw = {}
    for met in mets:
        inHist = ""
        uPerpHist = ""
        uParHist = ""
        zqtHist = ""
        #First do wrt Zqt
        for sample in samples:
            
            if inHist == "": 
                inHist = inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_par_qt".format(met))
                uPerpHist = inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_perp_qt".format(met))
                uParHist = inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_parPqt_qt".format(met))
                zqtHist = inFiles[sample].Get("12_HistogrammingMETPaper/ZQt")
            else: 
                inHist.Add(inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_par_qt".format(met)))
                uPerpHist.Add(inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_perp_qt".format(met)))
                uParHist.Add(inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_parPqt_qt".format(met)))
                zqtHist.Add(inFiles[sample].Get("12_HistogrammingMETPaper/ZQt"))
        nDivisions = 10
        
        binEdges = []
        maxValue = inHist.GetYaxis().GetBinUpEdge(inHist.GetYaxis().GetNbins()+1)
        for i in range(nDivisions+1): binEdges.append(i*(float(maxValue)/nDivisions))
        binEdges = [0,10,20,30,40,50,60,80,110,150,200,300,500]
#[f for f in range(0,inHist.GetYaxis().GetNbins()+inHist.GetYaxis().GetNbins()/nDivisions,inHist.GetYaxis().GetNbins()/nDivisions)]
        print binEdges

        (response[met],res_par[met],res_perp[met],res_par_raw[met],res_perp_raw[met]) = readOneDistribution(inHist,zqtHist,uPerpHist,uParHist,binEdges,True)


    genericPlottingMacro.plotGraphs(response,inDir+"zQt")
    genericPlottingMacro.plotGraphs(res_par,inDir+"_res_uPar_zQT",yAxis="#sigma (u_{par}) [GeV]",maximum=50,minimum=0,legendTitle="Response corrected")
    genericPlottingMacro.plotGraphs(res_par_raw,inDir+"_res_uPar_zQT_uncorr",yAxis="#sigma (u_{par}) [GeV] - uncorrected",maximum=50,minimum=0)
    genericPlottingMacro.plotGraphs(res_perp,inDir+"_res_uPerp_zQT",yAxis="#sigma (u_{perp}) [GeV]",maximum=50,minimum=0,legendTitle="Response corrected")
    genericPlottingMacro.plotGraphs(res_perp_raw,inDir+"_res_uPerp_zQT_uncorr",yAxis="#sigma (u_{perp}) [GeV] - uncorrected",maximum=50,minimum=0)

    response = {}
    res_par = {}
    res_par_raw = {}
    res_perp = {}
    res_perp_raw = {}

    for met in mets:
        inHist1 = ""
        inHist2 = ""
        uPerpHist = ""
        nDivisions = 80
        binEdges = [f for f in range(61)]
        print binEdges
        for sample in samples:
            if inHist1 == "":
                inHist1 = inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_par_nVtx".format(met))
                uPerpHist = inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_perp_nVtx".format(met))
                uParHist = inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_parPqt_nVtx".format(met))
                inHist2 = inFiles[sample].Get("12_HistogrammingMETPaper/ZQt_nVertex")
            else:
                inHist1.Add(inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_par_nVtx".format(met)))
                uPerpHist.Add(inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_perp_nVtx".format(met)))
                uParHist.Add(inFiles[sample].Get("12_HistogrammingMETPaper/{0}_u_parPqt_nVtx".format(met)))
                inHist2.Add(inFiles[sample].Get("12_HistogrammingMETPaper/ZQt_nVertex"))
        (response[met],res_par[met],res_perp[met],res_par_raw[met],res_perp_raw[met]) = readOneDistribution(inHist1,inHist2,uPerpHist,uParHist,binEdges,True,zqtIs1D=False)

    genericPlottingMacro.plotGraphs(response,inDir+"_nVtx","nVtx")
    genericPlottingMacro.plotGraphs(res_par,inDir+"_res_uPar_nVtx",xAxis="nVtx",yAxis="#sigma (u_{par}) [GeV]",maximum=50,minimum=0,legendTitle="Response corrected")
    genericPlottingMacro.plotGraphs(res_par_raw,inDir+"_res_uPar_nVtx_uncorr",xAxis="nVtx",yAxis="#sigma (u_{par}) [GeV] - uncorrected",maximum=50,minimum=0)
    genericPlottingMacro.plotGraphs(res_perp,inDir+"_res_uPerp_nVtx",xAxis="nVtx",yAxis="#sigma (u_{perp}) [GeV]",maximum=50,minimum=0,legendTitle="Response corrected")
    genericPlottingMacro.plotGraphs(res_perp_raw,inDir+"_res_uPerp_nVtx_uncorr",xAxis="nVtx",yAxis="#sigma (u_{perp}) [GeV] - uncorrected",maximum=50,minimum=0)
    

    for sample in samples:
        inFiles[sample].Close()

if __name__ == "__main__":
    
    leptons = ["muon","electron"]
    years = ["2018","2017","2016","2016APV","2016Total"]
    dataMC = ["","Data"]

    #reduced sets
#    leptons = ["muon"]
#    years = ["2018"]
#    dataMC = [""]

    baseDirName = "met"

    lumiStrings = {
        "2018":"60",
        "2017":"41",
        "2016":"16",
        "2016APV":"20",
        "2016Total":"36"
    }

    outDir = "responseResPlots/"

    if not os.path.exists(outDir): os.mkdir(outDir)
       
    genericPlottingMacro.setupEnv()
    genericPlottingMacro.setOutDir(outDir)    

    for year in years:
        for lepton in leptons:
            for dmc in dataMC:
                comps = AnalysisComponents("met",lepton=lepton,era=year)

                #Set up plots environment
                lepChanString = "#mu#mu"
                lumiString = "{} fb^{{-1}} (13TeV)".format(lumiStrings[year])
                
                if lepton  == "electron": lepChanString = "ee"
                genericPlottingMacro.changeLabels(latex2=lumiString,text2="{0} channel".format(lepChanString))
                
                samples = comps.samples if dmc == "" else comps.dataSamples
                
                directory = "{0}{1}{2}{3}".format(baseDirName,year[2:],"Ele" if lepton == "electron" else "",dmc)
            
                calculateResponse(directory,samples,outDir)
            
