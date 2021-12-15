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
parser.add_option("-o","--outDir",dest="outDir",default="plots/",type="string",help="The output directory")
parser.add_option("-l","--noLatex",dest="noLatex",default=False,action="store_true",help="If you don't want to make a latex file containing all of the plots")
parser.add_option("-q","--quiet",dest="quiet",default=False,action="store_true",help="Suppress various working messages")

(options, args) = parser.parse_args()

#Add a string if you want to ignore plots with that name. Useful to include numbers in the framework to skip out whole sections.
ignorePlots = ["00_","01_","02_","05_","06_","07_","08_","Jet4","Jet5","Jet6","JES","JER","CutEff"] #Ignore the weight variables
ignorePlots = ["00_","01_","02_","03_","04_""05_","06_","07_","08_","Jet4","Jet5","Jet6","Jet7","JES","JER","CutEff"] #Ignore the weight variables
#ignorePlots = ["00_","01_","02_","03_",_] #Ignore the weight variables

includePlots = ["METPhi"]
includePlots = []

customWeightsAllYears={
"2017":{
 "muon":{
  "data":0.818,
  "ww":2.294,
  "zz":1.000,
  "dy":1.296,
  "ttbar_2l":1.000,
  "wz":1.072,
   },
 "electron":{
  "data":0.877,
  "ww":2.294,
  "zz":1.000,
  "dy":1.296,
  "ttbar_2l":1.000,
  "wz":1.072,
   },
 },
"2016":{
 "muon":{
  "data":0.919,
  "ww":2.334,
  "zz":1.092,
  "dy":1.007,
  "ttbar_2l":1.000,
  "wz":2.252,
   },
 "electron":{
  "data":0.900,
  "ww":2.334,
  "zz":1.092,
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
  "data":0.752,
  "ww":1.296,
  "zz":1.024,
  "dy":1.000,
  "ttbar_2l":1.000,
  "wz":1.060,
   },
 "electron":{
  "data":0.875,
  "ww":1.296,
  "zz":1.024,
  "dy":1.000,
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

def makePlotsInOneDir(inDir,outDir,samples,dataSamples,options,histoGramPerSample,customWeights,genericPlottingMacro):

    #collect together the MC files
    inFiles = {}
    for sample in samples:
        if os.path.exists("{0}/{1}/hists/merged{1}.root".format(inDir,sample)):
            inFiles[sample] = TFile("{0}/{1}/hists/merged{1}.root".format(inDir,sample),"READ")
        else:
            print "Could not find {0}/{1}/hists/merged{1}.root\nExiting".format(inDir,sample)
            sys.exit(0)
        
    #The list of plots in the file
    plotPaths = []
    for obj in inFiles[inFiles.keys()[0]].GetListOfKeys():
        tempThing = inFiles[inFiles.keys()[0]].Get(obj.GetName())
        print obj.GetName()
        if not tempThing.ClassName().find("TH1") == -1 : plotPaths.append(tempThing.GetName())
        if not tempThing.ClassName().find("Directory") == -1:
            for k2 in tempThing.GetListOfKeys():
                temp2 = tempThing.Get(k2.GetName())
                if not temp2.ClassName().find("TH1") == -1:
                    plotPaths.append(tempThing.GetName() + "/"+ temp2.GetName())

    #get the data files in we are using data
    dataFiles = {}
    if not options.mcOnly:
        for dataSample in dataSamples:
            if os.path.exists("{0}Data/{1}/hists/merged{1}.root".format(inDir,dataSample)):                
                dataFiles[dataSample] = TFile("{0}Data/{1}/hists/merged{1}.root".format(inDir,dataSample),"READ")
            else:                                                                                          
                print "Could not find {0}Data/{1}/hists/merged{1}.root\nExiting".format(inDir,dataSample)  
                sys.exit(0)                                                                                
    


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
        for ignore in ignorePlots:
            if ignore in plot:
                doPlot = False
                break
        for includeKeyword in includePlots:
#            print includeKeyword
            if includeKeyword in plot:
                doPlot = True
                break
            else: doPlot = False
        if not doPlot: continue

        if not options.quiet: print "Doing plot {0}".format(plot)

        #Make a name that saves without slashes
        saveName = "_".join(plot.split("/"))
        writtenName = "\\_".join(saveName.split("_"))
        
        #The map of histograms to pass to the plotter
        histMap = {}
        
        #loop through simulated samples
        for sample in samples:
            tmpHist = inFiles[sample].Get(plot)
            if sample in customWeights.keys(): tmpHist.Scale(customWeights[key])
            if histoGramPerSample[sample] in histMap.keys():
                histMap[histoGramPerSample[sample]].Add(tmpHist)
            else:
                histMap[histoGramPerSample[sample]] = tmpHist

        if not options.mcOnly:
            for dataSample in dataSamples:
                if not "data_obs" in histMap.keys():
#                    if not dataFiles[dataSample].Get(plot): continue
#                    print dataSample
                    histMap["data_obs"] = dataFiles[dataSample].Get(plot)
                else:
#                    print dataFiles[dataSample].Get(plot).ClassName()
                    histMap["data_obs"].Add(dataFiles[dataSample].Get(plot))

        #make the plot
        genericPlottingMacro.makeASingleStackPlot(histMap,saveName,doData=(not options.mcOnly),xAxisLabel=histMap[samples[0]].GetXaxis().GetTitle())
        
        #make a log version
        genericPlottingMacro.makeASingleStackPlot(histMap,saveName+"_log",doData=(not options.mcOnly),isLog=True,xAxisLabel=histMap[samples[0]].GetXaxis().GetTitle())

        #add the plot to the latex file
        latexFile.write("\\frame{\n\\frametitle{"+writtenName+"}\n")
        latexFile.write("\\includegraphics[width=0.9\\textwidth]{"+saveName+".png}")
        latexFile.write("}\n")
        
    latexFile.write("\\end{document}")


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

    dirtyCombine2016()

#    exit()

    years = ["2018","2017","2016","2016APV","2016Total"]
    
#    years = ["2017","2016","2016APV","2016Total"]
#    years = ["2016Total"]
#    years = ["2017"]
    leptons =  ["muon","electron"]
    #    leptons =  ["electron"]
#    leptons = ["muon"]

    threads = []

    for year in years:
        for lepton in leptons:
            comps = AnalysisComponents("met",era=year,lepton=lepton)

            inDir = "met{0}{1}".format(year[2:],"" if lepton == "muon" else "Ele")
            
            outDir = "plots{0}/".format(inDir)
#{0}{1}".format(year[2:],"" if lepton =="muon" else "Ele")
            
            if not os.path.exists(outDir): os.mkdir(outDir)

            genericPlottingMacro = GenericPlottingClass()
            
            genericPlottingMacro.setupEnv()
            genericPlottingMacro.setRatioWidth(1.)

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
    
