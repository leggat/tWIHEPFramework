#A short script that takes the nominal templates from a desired file and prints their integral

import sys, math

from ROOT import *

from array import array

import genericPlottingMacro

if "--help" in sys.argv or "-h" in sys.argv:
    print "usage: useTemplatesForYieldTable.py <directoryPostfix> [options]"
    print " ele - run electron channel (can also be achieved with prefix option)"
    print " --prefix <prefix> - add a prefix to the directory (useful for electron or barrel/endcap)"
    print " --compareYields <secondDir> - print out the results for a second directory and make comparison SFs"
    print " --useMVAOutput - use the mva output instead of a particular plot"
    print " --useBarrelAndEndcap - do barrel and endcap versions"

secondDir = False
if "--compareYields" in sys.argv:
    ind = sys.argv.index("--compareYields")
    secondDir = sys.argv[ind+1]

inDir = sys.argv[1]

lepton = "mu"

eleString = ""

if "ele" in sys.argv:
    lepton = "ele"

if lepton == "ele": eleString = "Ele"

samples  = ["tW","zPlusJets","VV","qcd","ttbar","singleTop","wPlusJets","data_obs"]

inFileName = "12_HistogrammingMuon_MuN.root"
inFileName = "12_HistogrammingElectron_EleN.root"
inFileName = "15_HistogrammingMET_SumEt.root"
#inFileName = "SumEt.root"

#inFileName = "15_HistogrammingMET_SumEt.root"
#inFileName = "16_HistogrammingMtW_mTW.root"

plotDirPrefix = ""

if "--prefix" in sys.argv:
    ind = sys.argv.index("--prefix")
    plotDirPrefix = sys.argv[ind+1]

useMVAOutput = False
if "--useMVAOutput" in sys.argv: useMVAOutput = True

tableDict = {
"3j1t":"Signal Region (3j1t)",
"3j2t":"\\ttbar Region (3j2t)", 
"2j1t":"\\wPlusJets/QCD Region (2j1t)",
"4j1t":"Signal ISR region (4j1t)",
"4j2t":"\\ttbar ISR region (4j2t)"
}

def useMvaTemplates(region):
    inFile = TFile("{0}/mvaDists_{1}{3}_{2}_bin1000.root".format(inDir,region,lepton,plotDirPrefix),"READ")
    hists = {}
    for sample in samples:
        hists[sample] = inFile.Get(sample)
        hists[sample].SetDirectory(0)
    return hists

def usePlotFile(region,inDir):
    inFile = TFile("plots/{3}{4}{2}{0}/{1}".format(inDir,inFileName,region,eleString,plotDirPrefix),"READ")
    canvy = 0
    for prim in inFile.GetListOfKeys(): canvy = prim.ReadObj()

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
    return hists
        

def makeTable(region,inDir):
    hists = {}
    if useMVAOutput: hists = useMvaTemplates(region)
    else: hists = usePlotFile(region,inDir)

    if not hists: return(0,0)

    yields = {}

    uncerts = {}

    nEntries = {}

    total = 0.
    totalUncert = 0.

    for sample in samples:
        hist = hists[sample]
        yields[sample] = 0.
        uncerts[sample] = array('d',[0.])
        yields[sample] = hist.IntegralAndError(1,hist.GetXaxis().GetNbins()+1,uncerts[sample])
        nEntries[sample] = hist.GetEntries()
#        print sample,hist.Integral(),uncerts[sample][0]
        if not "data_obs" in sample: 
            total+=yields[sample]
            totalUncert += uncerts[sample][0]

#    print "Total MC: {0} +- {1}".format(total,totalUncert)
    print tableDict[region],
    for sample in samples:
        if "data" in sample: continue
#        print " & {0:.1f}$\\pm${1:.1f} ({2})".format(yields[sample],uncerts[sample][0],nEntries[sample]),
        print " & {0:.1f}$\\pm${1:.1f} ({2:.1f}\\%)".format(yields[sample],uncerts[sample][0],100*(yields[sample]/total)),

    print " & {0:.1f}$\\pm${2:.1f} & {1}$\\pm${3:.1f} \\\\".format(total,yields["data_obs"],totalUncert,math.sqrt(yields["data_obs"]))
    return (yields,uncerts)

def turnMapsIntoPlots(yields,uncerts,regions):
    samples = yields["3j1t"].keys()
    histMap = {}
    for sample in samples:
        histMap[sample] = TH1F(sample+"RegionPlot",sample+"RegionPlot",len(regions),0,len(regions))
    for i in range(1,len(regions)+1):
        for sample in samples:
            histMap[sample].SetBinContent(i,yields[regions[i-1]][sample])
#            print region,sample,uncerts[region][sample]
            histMap[sample].SetBinError(i,uncerts[regions[i-1]][sample][0])

    return histMap

def makeSumTable(region,yields1,uncerts1,yields2,uncerts2):
    print tableDict[region],
    total = 0.
    totalUncert = 0.
    for sample in samples:
        if "data" in sample: continue
        total += yields1[sample]+yields2[sample]
        totalUncert += uncerts1[sample][0] + uncerts2[sample][0]
    for sample in samples:
        if "data" in sample: continue
#        print " & {0:.1f}$\\pm${1:.1f} ({2})".format(yields[sample],uncerts[sample][0],nEntries[sample]),                                                                                                                                                                                                                    
        print " & {0:.1f}$\\pm${1:.1f} ({2:.1f}\\%)".format(yields1[sample]+yields2[sample],uncerts1[sample][0]+uncerts2[sample][0],100*((yields1[sample]+yields2[sample])/total)),

    print " & {0:.1f}$\\pm${2:.1f} & {1}$\\pm${3:.1f} \\\\".format(total,yields1["data_obs"]+yields2["data_obs"],totalUncert,math.sqrt(yields1["data_obs"]+yields2["data_obs"]))


def initTable():
    print "Region ",
    for sample in samples:
        if "data" in sample: 
            print " & Total MC & Data \\\\",
            continue
        print " & {0}".format(sample),
    print "\n\\hline"


if __name__ == "__main__":

    genericPlottingMacro.setupEnv()

    regions =  ["3j1t","2j1t","3j2t","4j1t","4j2t"]
    regions =  ["3j1t","2j1t","4j1t"]
    newRegions=[]

    if "--useBarrelAndEndcap" in sys.argv:
        for region in regions:
#            newRegions.append(region)
            newRegions.append("Barrel"+region)
            tableDict["Barrel"+region]=tableDict[region]+" barrel"
            newRegions.append("Endcap"+region)
            tableDict["Endcap"+region]=tableDict[region]+" endcap"
        regions = newRegions    
    genericPlottingMacro.setOutDir("regionPlot/")
    labelLepton = "#mu"
    if lepton=="ele": labelLepton="e"
    genericPlottingMacro.changeLabels(text2=labelLepton+" channel")

    print inDir, regions

    yields = {}
    uncerts = {}

    inDirs = [inDir]
    if secondDir: inDirs.append(secondDir)
    for inputDir in inDirs:
        print inputDir
        initTable()

        yields[inputDir] = {}
        uncerts[inputDir] = {}
        for region in regions:
            yields[inputDir][region],uncerts[inputDir][region] = makeTable(region,inputDir)
            if "Endcap" in region:
                makeSumTable(region.split("Endcap")[1],yields[inputDir][region],uncerts[inputDir][region],yields[inputDir]["Barrel"+region.split("Endcap")[1]],uncerts[inputDir]["Barrel"+region.split("Endcap")[1]])

    

        print "\\hline"
        histMap = turnMapsIntoPlots(yields[inputDir],uncerts[inputDir],regions)
    
        genericPlottingMacro.makeASingleStackPlot(histMap,lepton+"_regionPlot_"+inputDir,xAxisLabel="Region",xAxisBinLabels=regions)

    if secondDir:
        initTable()
        for region in regions:
            print "{0}".format(region), 
            for sample in samples:
                print "{0:.2f}".format((yields[secondDir][region][sample]/yields[inDir][region][sample])),
            print "\\hline"
        print "<tr> <td> <\\td> <td>W+Jets<\\td> <td>{0:.2f}<\\td> <td>{1:.2f}<\\td> <td>{2:.2f}<\\td> <\\tr>".format(yields[secondDir]["3j1t"]["wPlusJets"]/yields[inDir]["3j1t"]["wPlusJets"],yields[secondDir]["2j1t"]["wPlusJets"]/yields[inDir]["2j1t"]["wPlusJets"],yields[secondDir]["4j1t"]["wPlusJets"]/yields[inDir]["4j1t"]["wPlusJets"])
        print "<tr> <td> <\\td> <td>QCD<\\td> <td>{0:.2f}<\\td> <td>{1:.2f}<\\td> <td>{2:.2f}<\\td> <\\tr>".format(yields[secondDir]["3j1t"]["qcd"]/yields[inDir]["3j1t"]["qcd"],yields[secondDir]["2j1t"]["qcd"]/yields[inDir]["2j1t"]["qcd"],yields[secondDir]["4j1t"]["qcd"]/yields[inDir]["4j1t"]["qcd"])
#        print "QCD   : 3j1t: {0:.2f} 2j1t: {1:.2f} 4j2t: {2:.2f}".format(yields[secondDir]["3j1t"]["qcd"]/yields[inDir]["3j1t"]["qcd"],yields[secondDir]["2j1t"]["qcd"]/yields[inDir]["2j1t"]["qcd"],yields[secondDir]["4j1t"]["qcd"]/yields[inDir]["4j1t"]["qcd"])
