#Script will plot all of the items from a given selection of samples and files

import sys,os,threading

from ROOT import *

import subprocess

from genericPlottingClass import GenericPlottingClass

#allow command line argument implementation
from optparse import OptionParser

from analysisComponents import AnalysisComponents

parser = OptionParser(usage="usage: %prog [options] \nrun with --help to get list of options")
parser.add_option("-m","--mcOnly","--noData",dest="mcOnly",default=False,action="store_true",help="Make plots without data only")
parser.add_option("-o","--outDir",dest="outDir",default="plots",type="string",help="The output directory")
parser.add_option("-l","--noLatex",dest="noLatex",default=False,action="store_true",help="If you don't want to make a latex file containing all of the plots")
parser.add_option("-q","--quiet",dest="quiet",default=False,action="store_true",help="Suppress various working messages")
parser.add_option("-c","--combine2016",dest="combine2016",default=False,action="store_true",help="Combine 2016 and 2016APV into 2016Total")
parser.add_option("-y","--year",dest="year",default="",type="string",help="If we want to run over a particular year")
parser.add_option("--lepton",dest="lepton",default="",type="string",help="If we want to run over a particular lepton")
parser.add_option("-s","--systs",dest="systs",default=False,action="store_true",help="Include jer/jes/unclustered uncertainties where possible")
parser.add_option("-r","--rebin",dest="rebin",default=-1,type="int",help="If you want a specific number of bins in your histogram")
parser.add_option("--debugSysts",dest="debugSysts",default=False,action="store_true",help="Run the debug systematics methods")
parser.add_option("--doExactPlotsName",dest="doExactPlotsName",default=False,action="store_true",help="Use the internal exact plot name list to only produce certain plots")
parser.add_option("--doIncludeNames",dest="doIncludeNames",default=False,action="store_true",help="Match the list of include plots to the plot names to run")
parser.add_option("--symSysts",dest="symSysts",default=False,action="store_true",help="Set the uncertainties to be symmetric")
parser.add_option("--ignoreJER",dest="ignoreJER",default=False,action="store_true",help="Ignore JER uncertainties. This is for the issues seen in 2017 there")
parser.add_option("--savePDFs",dest="savePDFs",default=False,action="store_true",help="Save the output histograms in PDF format as well as png")
parser.add_option("--saveCFiles",dest="saveCFiles",default=False,action="store_true",help="Save the output histograms in .C format")

(options, args) = parser.parse_args()

#Add a string if you want to ignore plots with that name. Useful to include numbers in the framework to skip out whole sections.
ignorePlots = ["00_","01_","02_","05_","06_","07_","08_","Jet4","Jet5","Jet6","JES","JER","CutEff"] #Ignore the weight variables
ignorePlots = ["00_","01_","02_","03_","04_""05_","06_","07_","Jet4","Jet5","Jet6","Jet7","JES","JER","_jer","_jes","_unclust","CutEff"] #Ignore the weight variables
#ignorePlots = ["00_","01_","02_","03_",_] #Ignore the weight variables

includePlots = ["/MET"]
includePlots = ["/MET","/PuppiMET","ZQt","/RawMET","MetMuDiMuonMass","MetEleDiElectronMass"]
#includePlots = ["/RawMET"]

exactPlots = ["12_HistogrammingMETPaper/PuppiMET"]#,"12_HistogrammingMETPaper/MET_jerUp","12_HistogrammingMETPaper/MET_jerDown","12_HistogrammingMETPaper/MET_jesDown","12_HistogrammingMETPaper/MET_jesUp","12_HistogrammingMETPaper/MET_unclusteredUp","12_HistogrammingMETPaper/MET_unclusteredDown","12_HistogrammingMETPaper/PuppiMET","12_HistogrammingMETPaper/PuppiMET_jerUp","12_HistogrammingMETPaper/PuppiMET_jerDown","12_HistogrammingMETPaper/PuppiMET_jesDown","12_HistogrammingMETPaper/PuppiMET_jesUp","12_HistogrammingMETPaper/PuppiMET_unclusteredUp","12_HistogrammingMETPaper/PuppiMET_unclusteredDown"]
#exactPlots = []
#exactPlots = ["12_HistogrammingMETPaper/MET","12_HistogrammingMETPaper/METPhi","12_HistogrammingMETPaper/METPhi_uncorr"]

customWeightsAllYears={
"2017":{
 "muon":{
  "data":0.77,
  "ww":2.294,
  "zz":1.000,
  "dy":1.296,
  "ttbar_2l":1.000,
  "wz":1.072,
   },
 "electron":{
  "data":0.7,
  "ww":2.294,
  "zz":1.000,
  "dy":1.296,
  "ttbar_2l":1.000,
  "wz":1.072,
   },
 },
"2016":{
 "muon":{
  "data":0.94,
  "ww":2.329,
  "zz":1.085,
  "dy":1.007,
  "ttbar_2l":1.000,
  "wz":2.252,
   },
 "electron":{
  "data":0.87,
  "ww":2.329,
  "zz":1.085,
  "dy":1.007,
  "ttbar_2l":1.000,
  "wz":2.252,
   },
 },
"2018":{
 "muon":{
  "data":0.921,
  "ww":1.323,
  "zz":1.000,
  "dy":1.485,
  "ttbar_2l":1.381,
  "wz":1.423,
   },
 "electron":{
  "data":0.956,
  "ww":1.323,
  "zz":1.000,
  "dy":1.485,
  "ttbar_2l":1.381,
  "wz":1.423,
   },
 },
"2016APV":{
 "muon":{
  "data":0.7,
  "ww":1.296,
  "zz":1.024,
  "dy":1.000,
  "ttbar_2l":1.000,
  "wz":1.060,
   },
 "electron":{
  "data":0.84,
  "ww":1.296,
  "zz":1.024,
  "dy":1.000,
  "ttbar_2l":1.000,
  "wz":1.060,
   },
},
"2016Total":{
 "muon":{
  "data":0.8,
  "ww":1.,
  "zz":1.085,
  "dy":1.007,
  "ttbar_2l":1.000,
  "wz":1.,
   },
 "electron":{
  "data":0.8,
  "ww":1.1,
  "zz":1.085,
  "dy":1.007,
  "ttbar_2l":1.000,
  "wz":1.,
   },
 },

}

customWeightsAllYearsChanged={
"2017":{
 "muon":{
  "data":0.73,
  "ww":2.294,
  "zz":1.000,
  "dy":1.296,
  "ttbar_2l":1.000,
  "wz":1.072,
   },
 "electron":{
  "data":0.68,
  "ww":2.294,
  "zz":1.000,
  "dy":1.296,
  "ttbar_2l":1.000,
  "wz":1.072,
   },
 },
"2016":{
 "muon":{
  "data":0.95,
  "ww":2.329,
  "zz":1.085,
  "dy":1.007,
  "ttbar_2l":1.000,
  "wz":2.252,
   },
 "electron":{
  "data":0.86,
  "ww":2.329,
  "zz":1.085,
  "dy":1.007,
  "ttbar_2l":1.000,
  "wz":2.252,
   },
 },
"2016Total":{
 "muon":{
  "data":0.8,
  "ww":1.,
  "zz":1.085,
  "dy":1.007,
  "ttbar_2l":1.000,
  "wz":1.,
   },
 "electron":{
  "data":0.8,
  "ww":1.1,
  "zz":1.085,
  "dy":1.007,
  "ttbar_2l":1.000,
  "wz":1.,
   },
 },
"2018":{
 "muon":{
  "data":0.921,
  "ww":1.323,
  "zz":1.000,
  "dy":1.485,
  "ttbar_2l":1.381,
  "wz":1.423,
   },
 "electron":{
  "data":0.956,
  "ww":1.323,
  "zz":1.000,
  "dy":1.485,
  "ttbar_2l":1.381,
  "wz":1.423,
   },
 },
"2016APV":{
 "muon":{
  "data":0.70,
  "ww":1.296,
  "zz":1.024,
  "dy":1.000,
  "ttbar_2l":1.000,
  "wz":1.060,
   },
 "electron":{
  "data":0.8,
  "ww":1.296,
  "zz":1.024,
  "dy":1.000,
  "ttbar_2l":1.000,
  "wz":1.060,
   },
 },

}
customWeightsAllYearsPrevious={
"2017":{
 "muon":{
  "data":0.825,
  "ww":2.294,
  "zz":1.000,
  "dy":1.136,
  "ttbar_2l":1.000,
  "wz":1.072,
   },
 "electron":{
  "data":0.877,
  "ww":2.294,
  "zz":1.000,
  "dy":1.436,
  "ttbar_2l":1.000,
  "wz":1.072,
   },
 },
"2016":{
 "muon":{
  "data":0.47,
  "ww":2.329,
  "zz":1.085,
  "dy":50.85,
  "ttbar_2l":1.000,
  "wz":2.252,
   },
 "electron":{
  "data":0.45,
  "ww":2.329,
  "zz":1.085,
  "dy":0.85,
  "ttbar_2l":1.000,
  "wz":2.252,
   },
 },
"2018":{
 "muon":{
  "data":0.921,
  "ww":1.323,
  "zz":1.000,
  "dy":1.485,
  "ttbar_2l":1.381,
  "wz":1.423,
   },
 "electron":{
  "data":0.956,
  "ww":1.323,
  "zz":1.000,
  "dy":1.485,
  "ttbar_2l":1.381,
  "wz":1.423,
   },
 },
"2016APV":{
 "muon":{
  "data":0.670,
  "ww":1.296,
  "zz":1.024,
  "dy":0.95,
  "ttbar_2l":1.000,
  "wz":1.060,
   },
 "electron":{
  "data":0.857,
  "ww":1.296,
  "zz":1.024,
  "dy":1.000,
  "ttbar_2l":1.000,
  "wz":1.060,
   },
 },
"2016Total":{
 "muon":{
  "data":0.85,
  "ww":1.0,
  "zz":1.024,
  "dy":0.85,
  "ttbar_2l":1.000,
  "wz":1.060,
   },
 "electron":{
  "data":0.857,
  "ww":1.0,
  "zz":1.024,
  "dy":0.9,
  "ttbar_2l":1.000,
  "wz":1.060,
   },
 },

}

lumiStrings = {
"2018":"60",
"2017":"41",
"2016":"16",
"2016APV":"20",
"2016Total":"36"
}

#Let us run all directories concurrently
class myPlottingThread(threading.Thread):
    def __init__(self,inDir,outDir,samples,dataSamples,options,histoGramPerSample,customWeights,genericPlottingMacro):
        threading.Thread.__init__(self)
        self.inDir = inDir
        self.outDir = outDir
        self.samples = samples
        self.dataSamples = dataSamples
        self.options = options
        self.histoGramPerSample = histoGramPerSample
        self.customWeights = customWeights
        self.genericPlottingMacro = genericPlottingMacro
    def run(self):
        print "Running {0}".format(inDir)
        makePlotsInOneDir(self.inDir,self.outDir,self.samples,self.dataSamples,self.options,self.histoGramPerSample,self.customWeights,self.genericPlottingMacro)
        print "Done {0}".format(inDir) 

def getPlotList(inFile):
    plotPaths = []
    for obj in inFile.GetListOfKeys():
        tempThing = inFile.Get(obj.GetName())
        print obj.GetName()
        if not tempThing.ClassName().find("TH1") == -1 : plotPaths.append(tempThing.GetName())
        if not tempThing.ClassName().find("Directory") == -1:
            for k2 in tempThing.GetListOfKeys():
                temp2 = tempThing.Get(k2.GetName())
                if not temp2.ClassName().find("TH1") == -1:
                    plotPaths.append(tempThing.GetName() + "/"+ temp2.GetName())
    return plotPaths

def getInFiles(inDir,samples):
    inFiles={}
    for sample in samples:
        if os.path.exists("{0}/{1}/hists/merged{1}.root".format(inDir,sample)):
            inFiles[sample] = TFile("{0}/{1}/hists/merged{1}.root".format(inDir,sample),"READ")
        else:
            print "Could not find {0}/{1}/hists/merged{1}.root\nExiting".format(inDir,sample)
            sys.exit(0)
    return inFiles

def readDiffInBin(inFiles,plot,samples,binToRead):
    
    systNameMap = ["jer","jes","unclustered"]

    print "Bin number: {0}".format(binToRead/4)
    print "\\begin{tabular}{| c |l|l|l|l|l|l|l|}"
    print "Sample  &Nom& JERUp& JERDown& JESUp& JESDown& UncUp& UncDown\\\\"

    for sample in samples:
        sampleName = "\\_".join(sample.split("_"))
        print sampleName,
        nomBin =  inFiles[sample].Get(plot).GetBinContent(binToRead)
        print "& {0:.1f}".format(nomBin),
        if nomBin == 0.: nomBin = 1.
        for systName in systNameMap:
            for upDown in ["Up","Down"]:
                systAmount = inFiles[sample].Get("{0}_{1}{2}".format(plot,systName,upDown)).GetBinContent(binToRead)
                print "& {0:.1f} ({1:.0f}\\%)".format(systAmount,100*systAmount/nomBin),
        print "\\\\"
    print "\\end{tabular}"

def makePlotsInOneDir(inDir,outDir,samples,dataSamples,options,histoGramPerSample,customWeights,genericPlottingMacro):

    #some debug variables
    debugBinNumber = 100
    #collect together the MC files
    inFiles = getInFiles(inDir,samples)
        
    #The list of plots in the file
    plotPaths = getPlotList(inFiles[samples[0]])    

    #get the data files in we are using data
    dataFiles = {}
    if not options.mcOnly: dataFiles = getInFiles("{0}Data".format(inDir),dataSamples)

    #define custom weights here

    #    customWeights = {"dy":1.4}
    if "data" in customWeights.keys():
        for key in customWeights.keys():
            if key == "data": continue
            customWeights[key]*=customWeights["data"]

    #Set up a latex file to store all the plots in
    latexFile = ""
    if not options.noLatex:
        latexFile = open(outDir+"combinedLatexFile.tex","w")
        latexFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")

    #Loop through the histograms
    for plot in plotPaths:
        doPlot = True
        #If we are ignoring the plot, ignore it
        if options.doIncludeNames:
            for includeKeyword in includePlots:
                #            print includeKeyword
                if includeKeyword in plot:
                    doPlot = True
                    break
                else: doPlot = False

        for ignore in ignorePlots:
            if ignore in plot:
                doPlot = False
                break
        if options.doExactPlotsName:
            for exactPlotName in exactPlots:
                if plot == exactPlotName:
                    doPlot = True
                    break
                else:
                    doPlot = False
        if not doPlot: continue

        if not options.quiet: print "Doing plot {0}".format(plot)

        #Make a name that saves without slashes
        saveName = "_".join(plot.split("/"))
        writtenName = "\\_".join(saveName.split("_"))
        
        #The map of histograms to pass to the plotter
        histMap = {}
        
        #loop through simulated samples
        for sample in samples:
            tmpHist = inFiles[sample].Get(plot).Clone()
            if sample in customWeights.keys(): 
                tmpHist.Scale(customWeights[sample])
            if histoGramPerSample[sample] in histMap.keys():
                histMap[histoGramPerSample[sample]].Add(tmpHist)
            else:
                histMap[histoGramPerSample[sample]] = tmpHist

        
        if not options.mcOnly:
            for dataSample in dataSamples:
                if not "data_obs" in histMap.keys():
#                    if not dataFiles[dataSample].Get(plot): continue
#                    print dataSample
                    histMap["data_obs"] = dataFiles[dataSample].Get(plot).Clone()
                else:
#                    print dataFiles[dataSample].Get(plot).ClassName()
                    histMap["data_obs"].Add(dataFiles[dataSample].Get(plot).Clone())

        #make the plot
        genericPlottingMacro.makeASingleStackPlot(histMap,saveName,doData=(not options.mcOnly),xAxisLabel=histMap[samples[0]].GetXaxis().GetTitle())
        
        #make a log version
        genericPlottingMacro.makeASingleStackPlot(histMap,saveName+"_log",doData=(not options.mcOnly),isLog=True,xAxisLabel=histMap[samples[0]].GetXaxis().GetTitle())

        #if we're making plots with additional systematics, we want to do that here.
        if options.systs:
            if not inFiles[samples[0]].Get(plot+"_jerUp"): continue
            print "Has jer variations"
            systMap = {}
            systNameMap = {"jer":"JER","jes":"JES","unclustered":"Unclus."}
#            systNameMap = {"jer":"JER","unclustered":"Unclus."}
            systNameMap = {"jes":"JES","unclustered":"Unclus."}
#            systNameMap = {"unclustered":"Unclus."}
            for systName in systNameMap:
                for upDown in ["Up","Down"]:
                    for sample in samples:
                        systNameInMap = "{0}{1}".format(systNameMap[systName],upDown)
                        tmpHist = inFiles[sample].Get("{0}_{1}{2}".format(plot,systName,upDown)).Clone()
                        if options.ignoreJER and systName == "jer": tmpHist = inFiles[sample].Get(plot)
                        if sample in customWeights.keys(): tmpHist.Scale(customWeights[sample])
                        if systNameInMap in systMap.keys():
                            systMap[systNameInMap].Add(tmpHist)
                        else:
                            systMap[systNameInMap] = tmpHist
            genericPlottingMacro.makeASingleStackPlot(histMap,saveName+"_systs",doData=(not options.mcOnly),xAxisLabel=histMap[samples[0]].GetXaxis().GetTitle(),systMap=systMap)
            genericPlottingMacro.makeASingleStackPlot(histMap,saveName+"_systs_log",doData=(not options.mcOnly),isLog=True,xAxisLabel=histMap[samples[0]].GetXaxis().GetTitle(),systMap=systMap)

        #add the plot to the latex file
        latexFile.write("\\frame{\n\\frametitle{"+writtenName+"}\n")
        latexFile.write("\\includegraphics[width=0.9\\textwidth]{"+saveName+".png}")
        latexFile.write("}\n")

        if options.debugSysts: 
            for binNume in range(20,200,40):
                readDiffInBin(inFiles,plot,samples,binNume)
        
    latexFile.write("\\end{document}")



def lessDirtyCombine2016():

    for lepton in ["electron","muon"]:
        elePostfix = "" if lepton == "muon" else "Ele"

        pathBase = "met16Total{0}".format(elePostfix)

        if not os.path.exists(pathBase):
            for dmc in ["","Data"]:
                os.mkdir("{0}{1}".format(pathBase,dmc))
            
        comps1 = AnalysisComponents("met",era="2016",lepton=lepton)
        comps2 = AnalysisComponents("met",era="2016APV",lepton=lepton)

        for sample in comps1.samples:
            if not os.path.exists("{0}/{1}".format(pathBase,sample)):
                os.makedirs("{0}/{1}/hists/".format(pathBase,sample))
            inFile1 = TFile("met16{2}/{1}/hists/merged{1}.root".format(pathBase,sample,elePostfix),"READ")
            inFile2 = TFile("met16APV{2}/{1}/hists/merged{1}.root".format(pathBase,sample,elePostfix),"READ")
            outFile = TFile("{0}/{1}/hists/merged{1}.root".format(pathBase,sample),"RECREATE")
            outFile.cd()
#            plotPaths = getPlotList(inFile1)

            for obj in inFile1.GetListOfKeys():
                tempThing = inFile1.Get(obj.GetName())
                if not tempThing.ClassName().find("Directory") == -1:
                    tmpDir = TDirectoryFile(obj.GetName(),obj.GetName())
                    tmpDir.cd()
                    for k2 in tempThing.GetListOfKeys():
                        temp2 = tempThing.Get(k2.GetName())                             
                        #                        if not temp2.ClassName().find("TH1") == -1:                     
                        plot = tempThing.GetName() + "/"+ temp2.GetName()
                        



                        print "Add {0}".format(plot)
                        tmp1Hist = inFile1.Get(plot)
                        if sample in customWeightsAllYears["2016"].keys(): tmp1Hist.Scale(customWeightsAllYears["2016"][sample])
                        if "data" in customWeightsAllYears["2016"].keys(): tmp1Hist.Scale(customWeightsAllYears["2016"]["data"])
                        tmp2Hist = inFile2.Get(plot)
                        if sample in customWeightsAllYears["2016APV"].keys(): tmp2Hist.Scale(customWeightsAllYears["2016APV"][sample])
                        if "data" in customWeightsAllYears["2016APV"].keys(): tmp2Hist.Scale(customWeightsAllYears["2016APV"]["data"])
                        tmp1Hist.Add(tmp2Hist)
                        tmp1Hist.Write()
                    outFile.cd()
                    tmpDir.Write()
        for comp in [comps1,comps2]:
            for dataSample in comp.dataSamples:
                if not os.path.exists("{0}Data/{1}".format(pathBase,dataSample)): os.makedirs("{0}Data/{1}/hists/".format(pathBase,dataSample))
                subprocess.call("cp met{0}{2}Data/{1}/hists/merged{1}.root {3}Data/{1}/hists/merged{1}.root".format(comp.year[2:],dataSample,elePostfix,pathBase),shell=True)
        

def dirtyCombine2016():

    for lepton in ["electron","muon"]:

        elePostfix = "" if lepton == "muon" else "Ele"

        pathBase = "met16Total{0}".format(elePostfix)

        if not os.path.exists(pathBase):
            for dmc in ["","Data"]:
                os.mkdir("{0}{1}".format(pathBase,dmc))
            
        comps1 = AnalysisComponents("met",era="2016",lepton=lepton)
        comps2 = AnalysisComponents("met",era="2016APV",lepton=lepton)
        
        for sample in comps1.samples:
            if not os.path.exists("{0}/{1}".format(pathBase,sample)):
                os.makedirs("{0}/{1}/hists/".format(pathBase,sample))
            subprocess.call("hadd -f {0}/{1}/hists/merged{1}.root met16{2}/{1}/hists/merged{1}.root met16APV{2}/{1}/hists/merged{1}.root".format(pathBase,sample,elePostfix),shell=True)
        for comp in [comps1,comps2]:
            for dataSample in comp.dataSamples:
                if not os.path.exists("{0}Data/{1}".format(pathBase,dataSample)): os.makedirs("{0}Data/{1}/hists/".format(pathBase,dataSample))
                subprocess.call("cp met{0}{2}Data/{1}/hists/merged{1}.root {3}Data/{1}/hists/merged{1}.root".format(comp.year[2:],dataSample,elePostfix,pathBase),shell=True)

#Main loop
if __name__ == "__main__":

    if options.combine2016: lessDirtyCombine2016()

#    exit()

    years = ["2018","2017","2016","2016APV","2016Total"]
    
#    years = ["2016","2016APV","2016Total"]
#    years = ["2016Total"]
#    years = ["2017"]
    leptons =  ["muon","electron"]
    #    leptons =  ["electron"]
#    leptons = ["muon"]

    threads = []

    if not options.year == "": years = [options.year]
    if not options.lepton == "": leptons = [options.lepton]

    for year in years:
        for lepton in leptons:
            comps = AnalysisComponents("met",era=year,lepton=lepton)

            inDir = "met{0}{1}".format(year[2:],"" if lepton == "muon" else "Ele")
            
            outDir = "{1}{0}/".format(inDir,options.outDir)
#{0}{1}".format(year[2:],"" if lepton =="muon" else "Ele")
            
            if not os.path.exists(outDir): os.mkdir(outDir)

            genericPlottingMacro = GenericPlottingClass()
            
            genericPlottingMacro.setupEnv()
            genericPlottingMacro.setRatioWidth(1.)
            genericPlottingMacro.setSymmetriseTotalUncert(options.symSysts)
            genericPlottingMacro.setSavePDF(options.savePDFs)
            genericPlottingMacro.setSaveCFiles(options.saveCFiles)

            if options.rebin > 0: genericPlottingMacro.setRebinNumber(options.rebin)

            genericPlottingMacro.setOutDir(outDir)

            #Set up plots environment
            lepChanString = "#mu#mu"
            lumiString = "{} fb^{{-1}} (13TeV)".format(lumiStrings[year])
                
            if lepton  == "electron": lepChanString = "ee"
            genericPlottingMacro.changeLabels(latex2=lumiString,text2="{0} channel".format(lepChanString))

            customWeights = {}
            if year in customWeightsAllYears.keys(): customWeights = customWeightsAllYears[year][lepton]

            if not os.path.exists(inDir):
                print "Skipping {0}".format(inDir)
                continue

            print "processing {0}".format(inDir)


            makePlotsInOneDir(inDir,outDir,comps.sample,comps.dataSamples,options,comps.histoGramPerSample,customWeights,genericPlottingMacro)
#            threading.Thread(target=makePlotsInOneDir,args=[inDir,outDir,comps.sample,comps.dataSamples,options,comps.histoGramPerSample,customWeights,genericPlottingMacro]).start()

#            thread = myPlottingThread(inDir,outDir,comps.sample,comps.dataSamples,options,comps.histoGramPerSample,customWeights,genericPlottingMacro)
#            thread.start()

#            threads.append(thread)
            
#    for t in threads:
#        t.join()
#    print "Finished all"
    
