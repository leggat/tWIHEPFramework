#A script that loops through the plots that are available for the barrel and endcap separately and unifies them

from ROOT import *

import sys,os,analysisComponents,genericPlottingMacro,subprocess

if "--help" in sys.argv or "-h" in sys.argv:
    print "Script to combine barrel and endcap plots in some given directories"
    print " --inPostfix <inPostfix> - the postfix of the directory we're going to combine the plots from"
    print " --noLatex - don't make the latex file"
    print " --electron - do electron channel"
    print " --includeZeroTag - include zero tag regions in plots"
    sys.exit(0)

elePrefix = ""
if "--electron" in sys.argv: elePrefix = "Ele"

components = analysisComponents.AnalysisComponents()
postfix = "iteration1"
if "--inPostfix" in sys.argv:
    ind = sys.argv.index("--inPostfix")
    postfix =  sys.argv[ind+1]

    
def combinePlots(plot1Path,plot2Path,outPath):
    inFile1 = TFile(plot1Path,"READ")
    inFile2 = TFile(plot2Path,"READ")

#    print plot1Path,plot2Path
    canvy1,canvy2 = "",""
    for i in inFile1.GetListOfKeys():
        canvy1 = i.ReadObj()
    for i in inFile2.GetListOfKeys():
        canvy2 = i.ReadObj()

    mcStack = ""
    data = ""
    xAxisLabel = ""
    #Get stuff from canvy1
    for key in canvy1.GetListOfPrimitives():
        if key.ClassName() == "TH1F": 
            data = key.Clone()
         #   print data.Integral(),key.Integral()
            data.SetDirectory(0)
            xAxisLabel = data.GetXaxis().GetTitle()
        if key.ClassName() == "THStack": 
            mcStack = key.Clone()
            for hist in mcStack.GetHists():
                hist.SetDirectory(0)
    
#    print data.Integral()

    #Add stuff from canvy 2 to canvy 1
    for key in canvy2.GetListOfPrimitives():
        #deal with data
        if key.ClassName() == "TH1F":
#            print key.GetName()
            data.Add(key)
#            print key.Integral()
        if key.ClassName() == "THStack":
            for hist in key.GetHists():
                for hist2 in mcStack.GetHists():
                    if hist.GetName() == hist2.GetName():
                        hist2.Add(hist)

#    print data.Integral()
    #Turn the st6ack and data into a map
    histMap = {}
    histName = data.GetName().split("data")[0]
    histMap["data_obs"] = data
    for hist in mcStack.GetHists():
        histMap[hist.GetName().split(histName)[-1]] = hist

    genericPlottingMacro.makeASingleStackPlot(histMap,histName,xAxisLabel=xAxisLabel)
    return histName
    

def doOneRegion(region,latexFile):
    print "Doing {0} region".format(region)
#    if not os.path.exists("plots/{0}{1}{2}/".format("Combined",region,postfix)): os.mkdir("plots/{0}{1}{2}/".format("Combined",region,postfix))
    genericPlottingMacro.setOutDir("plots/{3}{0}{1}{2}/".format("Combined",region,postfix,elePrefix))
    prePlots = {}
    for i in ["Barrel","Endcap"]:
        prePlots[i] = [f for f in os.listdir("plots/{3}{0}{1}{2}/".format(i,region,postfix,elePrefix)) if f.endswith(".root") and "_log" not in f and "comp.root" not in f and "Comp.root" not in f]
    for key in prePlots["Barrel"]:
        if key not in prePlots["Endcap"]:
            print "{0} not in endcap directory?!?".format(key)
            continue
        histName = combinePlots("plots/{3}Barrel{0}{1}/{2}".format(region,postfix,key,elePrefix),"plots/{3}Endcap{0}{1}/{2}".format(region,postfix,key,elePrefix),"plots/{4}{0}{1}{2}/{3}".format("Combined",region,postfix,key.split(".root")[0],elePrefix))
        if latexFile:
            latexFile.write("\\frame{{\n\\frametitle{{{0}}}\n".format("-".join(histName.split("_"))))
            latexFile.write("\\includegraphics[width=0.3\\textwidth]{{../{3}Barrel{0}{1}/{2}.png}}\n".format(region,postfix,key.split(".root")[0],elePrefix))
            latexFile.write("\\includegraphics[width=0.3\\textwidth]{{../{3}Endcap{0}{1}/{2}.png}}\n".format(region,postfix,key.split(".root")[0],elePrefix))
            latexFile.write("\\includegraphics[width=0.3\\textwidth]{{../{3}Combined{0}{1}/{2}.png}}\n".format(region,postfix,histName,elePrefix))
            latexFile.write("}\n")
    

if __name__ == "__main__":
    #set up plotting environment
    genericPlottingMacro.setupEnv()
    latexFile = ""
    if not "--noLatex" in sys.argv: 
        if not os.path.exists("plots/latexDir/"): os.mkdir("plots/latexDir/")
        latexFile = open("plots/latexDir/combinedPlots{0}.tex".format(postfix),"w")
        latexFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")
    regions = components.regions
    if "--includeZeroTag" in sys.argv: regions = components.extendedRegions
    for region in regions:
        genericPlottingMacro.setRegionLepton(region=region)
        doOneRegion(region,latexFile)
    if latexFile:
        latexFile.write("\\end{document}")
        latexFile.close()
        os.chdir("plots/latexDir/")
        subprocess.call("pdflatex combinedPlots{0}.tex".format(postfix),shell=True)
        os.chdir("../../")
