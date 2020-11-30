#Run the fit and iterate a few times to see if the fit changes drastically

import sys, os, json, subprocess,time,shutil, analysisComponents, copy

nIterations = 2

if "--help" in sys.argv or "-h" in sys.argv:
    print "Run the fit iteratively to see how the SFs evolve"
    print " --barrelEndcap - Split barrel endcap"
    print " --barrel - only run on barrel"
    print " --endcap - only run on endcap"
    print " ele -- runs the electron channel"
    print " --metFit makes the fit be on met instead of mtw"
    print " --noNorm - don't normalise the inverted qcd templates"
    print " --noSub - don't subtract the inverted templates"
    print " --skipFits - to debug the SF evolution code"
    print " --onlyFitPlots - do all plots"
    print " --nIterations <nIts> - number of iterations to run (default {0})".format(nIterations)
    print " --customPostfix <postfix> - add something to the iterated dirName"
    print " --noPlotCombine - don't run the combine plot command if barrelEndcap option is selected"
    print " --useZeroTag - use zero tag region when making plots"
    print " --fitZeroTag - Run the process on the zero tyag region also"

components = analysisComponents.AnalysisComponents()

barEndL = [""]

directoryPostfix = ""
if "--customPostfix" in sys.argv:
    ind = sys.argv.index("--customPostfix")
    directoryPostfix = sys.argv[ind+1]

fitPostFix = ""
plotsPostfix = ""
barendPost = ""
combinePlots = False
if "--onlyFitPlots" in sys.argv: plotsPostfix += " --plotsForFit"
if "--barrelEndcap" in sys.argv: 
    fitPostFix += " --barrelEndcap"
    plotsPostfix += " --splitBarrelEndcap"
    barEndL = ["Barrel","Endcap"]
    barendPost = "_both"
    if not "--noPlotCombine" in sys.argv: combinePlots = True
if "--barrel" in sys.argv: 
    fitPostFix += " --barrel"
    plotsPostfix += " --barrel"
    barEndL = ["Barrel"]
    barendPost = "_barrel"
if "--endcap" in sys.argv: 
    fitPostFix += " --endcap"
    plotsPostfix += " --endcap"
    barEndL = ["Endcap"]
    barendPost = "_endcap"
combineEle = ""
if "ele" in sys.argv: 
    fitPostFix += " ele"
    plotsPostfix += " ele"
    combineEle += " --electron"
if "--metFit" in sys.argv: fitPostFix += " --metFit"
if "--noNorm" in sys.argv: plotsPostfix += " --noNorm"
if "--noSub" in sys.argv: plotsPostfix += " --noSub"
if "--useZeroTag" in sys.argv: plotsPostfix += " --useZeroTag"
if "--fitZeroTag" in sys.argv:
    plotsPostfix += " --includeZeroTag"
    fitPostFix += " --zeroTag"
if "--nIterations" in sys.argv:
    ind = sys.argv.index("--nIterations")
    nIterations = int(sys.argv[ind+1])
lep = "mu"
if "ele" in sys.argv: lep = "ele"
          
#Start fresh with prefit plots

if not os.path.exists("logs/"): 
    os.mkdir("logs/")
    os.mkdir("logs/iterFit/")

#Check whether the prefit has been run

def runPrefit():    

    print "Running prefit..."
    
    subprocess.call("python /publicfs/cms/user/duncanleg/tW13TeV/framework/utils/createAllRegionPlots.py --noLatexComp -p iteration0 {0} --isPrefit > logs/iterFit/prefit.log 2> logs/iterFit/prefit.error".format(plotsPostfix),shell =True)
    

def readCoeffs(jsonFileName):
    data = ""
    with open(jsonFileName,"read") as read_file:
        data = json.load(read_file)                       
    return data


def iterateJSONFile(previousJSON,thisJSON):
    initWeights = readCoeffs(previousJSON)
    iteratedWeights = readCoeffs(thisJSON)
    
    outWeights = copy.deepcopy(initWeights)

#    print initWeights,iteratedWeights

    for process in initWeights.keys():
        if not process in iteratedWeights.keys(): continue
        for barEnd in ["Barrel","Endcap"]:
            if not barEnd in iteratedWeights[process].keys(): continue
            for region in initWeights[process][barEnd].keys():
                if region in iteratedWeights[process][barEnd].keys(): outWeights[process][barEnd][region] = float(initWeights[process][barEnd][region]) * float(iteratedWeights[process][barEnd][region])

    return outWeights
#    for process in 
    
def makeInitSFs():
    print "Making init SFs" 
    initSFs = {}
    for process in components.histoGramOrder:
        initSFs[process] = {}
        for barEnd in barEndL:
            initSFs[process][barEnd] = {}            
            for region in components.extendedRegions:
                nitSF = 1.
                if process == "wPlusJets": nitSF = 2.98
                initSFs[process][barEnd][region] = nitSF
    with open("logs/iterFit/initSFs_{0}{1}.json".format(lep,barendPost),"w") as outfile:
        json.dump(initSFs,outfile)
                

def runSingleIteration(currentJSON,iterNumber):

    print "Iteration {0}".format(iterNumber)

    print "Running plots"
    print "python /publicfs/cms/user/duncanleg/tW13TeV/framework/utils/createAllRegionPlots.py --noLatexComp --jsonWeights {0} -p {3}iteration{1} {2} > logs/\
iterFit/plots{1}{3}.log 2> logs/iterFit/plots{1}{3}.error".format(currentJSON,iterNumber,plotsPostfix,directoryPostfix)
        #make the plots now with the new weights
    subprocess.call("python /publicfs/cms/user/duncanleg/tW13TeV/framework/utils/createAllRegionPlots.py --noLatexComp --jsonWeights {0} -p {3}iteration{1} {2} > logs/iterFit/plots{1}{3}.log 2> logs/iterFit/plots{1}{3}.error".format(currentJSON,iterNumber,plotsPostfix,directoryPostfix),shell=True)

    timeStr = time.strftime("%Y%m%d-%H%M%S",time.gmtime())
    jsonFileName = "weights_{0}_fit{1}_{2}{3}_{4}.json".format(timeStr,iterNumber,lep,barendPost,directoryPostfix)
    
    jsonPath = "/publicfs/cms/user/duncanleg/tW13TeV/framework/config/weights/jsonWeights/{0}".format(jsonFileName)

    print "Running fit!"
    print "python /publicfs/cms/user/duncanleg/tW13TeV/framework/utils/bkgFit/allRegionFits.py {0} --inDirPost {3}iteration{2} --outJSONFile {1} > logs/iterFit/fit{2}{3}.log 2>logs/iterFit/fit{2}{3}.error".format(fitPostFix,jsonPath,iterNumber,directoryPostfix)
    subprocess.call("python /publicfs/cms/user/duncanleg/tW13TeV/framework/utils/bkgFit/allRegionFits.py {0} --inDirPost {3}iteration{2} --outJSONFile {1} > logs/iterFit/fit{2}{3}.log 2>logs/iterFit/fit{2}{3}.error".format(fitPostFix,jsonPath,iterNumber,directoryPostfix),shell=True)

    newJSONData = iterateJSONFile(currentJSON,jsonPath)
    outJsonName = "logs/iterFit/iter{0}SFs_{1}{2}{3}.json".format(iterNumber,lep,barendPost,directoryPostfix)
    with open(outJsonName,"w") as outFile:
        json.dump(newJSONData,outFile)
    return outJsonName


def runIterations():

    currentJson = "logs/iterFit/initSFs_{0}{1}.json".format(lep,barendPost)
    jsonNames = []
    
    for i in range(nIterations):
        jsonNames.append(runSingleIteration(currentJson,i))
        currentJson = jsonNames[-1]
        #unique weight names
        #run the fit
        
        #copy json file to iterFit for debug purposes
#        shutil.copyfile("/publicfs/cms/user/duncanleg/tW13TeV/framework/config/weights/jsonWeights/{0}".format(jsonFileName),"logs/iterFit/fit{0}.json".format(i))

    return jsonNames


def printEvolvingSFs(jsonNames):
    perMCEvo = []
    regions = []
    processes = []
    barrelEndcap = []

    for jsonNameInd in range(len(jsonNames)):
        perMCEvo.append({})
        data = readCoeffs(jsonNames[jsonNameInd])
        for key in data:
            if key not in processes: processes.append(key)
            print key
            perMCEvo[jsonNameInd][key] = {}
            for key1 in data[key]:
                if key1 not in barrelEndcap: barrelEndcap.append(key1)
                for key2 in data[key][key1]:
                    if key2 not in regions: regions.append(key2)
                    print jsonNameInd,key,key1,key2,data[key][key1][key2]
                    perMCEvo[jsonNameInd][key]["{0}{1}".format(key2,key1)] = float(data[key][key1][key2])

    for process in processes:
        print process
        for barEnd in barrelEndcap:
            print barEnd
            for region in regions:
                print region,
                for i in range(len(jsonNames)):
                    print "{0:.2f} ".format(float(perMCEvo[i][process]["{0}{1}".format(region,barEnd)])),
                print


if __name__ == "__main__":
    jsonNames = []
    #if not os.path.exists("logs/iterFit/initSFs_{0}.json".format(lep)): 
    makeInitSFs()
    if not "--skipFits" in sys.argv:
#        if not os.path.exists("logs/iterFit/prefit.log"): runPrefit()
        print "Preparing to run {0} times".format(nIterations)
        jsonNames = runIterations()
        
    else:
        for i in range(nIterations):
            jsonNames.append("logs/iterFit/fit{0}.json".format(i))

    if combinePlots:
        subprocess.call("python ../../framework/utils/combineBarrelEndcapPlots.py --inPostfix {0}iteration{1} {2}".format(directoryPostfix,nIterations-1,combineEle),shell=True)
            
    print "Finished iterative fit, printing iterated SFs:"
    printEvolvingSFs(jsonNames)

    
