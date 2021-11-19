#Script will plot all of the items from a given selection of samples and files

import sys,os

from ROOT import *

import genericPlottingMacro

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
ignorePlots = ["00_","JES","JER"] #Ignore the weight variables
#ignorePlots = ["00_","01_","02_","03_",_] #Ignore the weight variables

#Main loop
if __name__ == "__main__":

    #make output directory
    if not os.path.exists(options.outDir):
        os.makedirs(options.outDir)
    
    #Get the analysis components from the central store
    comps = AnalysisComponents("met")
    
    #collect together the MC files
    inFiles = {}
    for sample in comps.sample:
        if os.path.exists("{0}/{1}/hists/merged{1}.root".format(comps.dirName,sample)):
            inFiles[sample] = TFile("{0}/{1}/hists/merged{1}.root".format(comps.dirName,sample),"READ")
        else:
            print "Could not find {0}/{1}/hists/merged{1}.root\nExiting".format(comps.dirName,sample)
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
        for dataSample in comps.dataSamples:
            if os.path.exists("{0}Data/{1}/hists/merged{1}.root".format(comps.dirName,dataSample)):                
                dataFiles[dataSample] = TFile("{0}Data/{1}/hists/merged{1}.root".format(comps.dirName,dataSample),"READ")
            else:                                                                                          
                print "Could not find {0}Data/{1}/hists/merged{1}.root\nExiting".format(comps.dirName,dataSample)  
                sys.exit(0)                                                                                
    

    #Set up plots environment
    genericPlottingMacro.setupEnv()
    genericPlottingMacro.setOutDir(options.outDir)
    genericPlottingMacro.changeLabels(text2="Dimu channel")

    #define custom weights here
    customWeights = {"dy":0.735}
#    customWeights = {"dy":1.4}
    genericPlottingMacro.customWeightObj(customWeights)

    #Set up a latex file to store all the plots in
    latexFile = ""
    if not options.noLatex:
        latexFile = open(options.outDir+"combinedLatexFile.tex","w")
        latexFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")

    #Loop through the histograms
    for plot in plotPaths:
        doPlot = True
        #If we are ignoring the plot, ignore it
        for ignore in ignorePlots:
            if ignore in plot:
                doPlot = False
                break
        if not doPlot: continue

        if not options.quiet: print "Doing plot {0}".format(plot)

        #Make a name that saves without slashes
        saveName = "_".join(plot.split("/"))
        writtenName = "\\_".join(saveName.split("_"))
        
        #The map of histograms to pass to the plotter
        histMap = {}
        
        #loop through simulated samples
        for sample in comps.samples:
            histMap[sample] = inFiles[sample].Get(plot)

        if not options.mcOnly:
            for dataSample in comps.dataSamples:
                if not "data_obs" in histMap.keys():
                    histMap["data_obs"] = dataFiles[dataSample].Get(plot)
                else:
                    histMap["data_obs"].Add(dataFiles[dataSample].Get(plot))

        #make the plot
        genericPlottingMacro.makeASingleStackPlot(histMap,saveName,doData=(not options.mcOnly),xAxisLabel=plot)

        #add the plot to the latex file
        latexFile.write("\\frame{\n\\frametitle{"+writtenName+"}\n")
        latexFile.write("\\includegraphics[width=0.9\\textwidth]{"+saveName+".png}")
        latexFile.write("}\n")
        
    latexFile.write("\\end{document}")
