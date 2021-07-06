#from ROOT import *

import subprocess, sys, os, time, math

#gROOT.ProcessLine(".L /publicfs/cms/user/duncanleg/tW13TeV/tmva/mvaTool.C+")

if "--help" in sys.argv or "-h" in sys.argv:
    print "Usage: makeCondorSubmissionForMVA.py <outdir> [options]"
    print "Make condor submission jobs to run the mva reading code"
    print " --oneFile only run over one file in each dataset (for mva debugging purposes)"
    print " --onlyNominal don't include systematic variations"
    print " --numberOfFilesPerJob <n> Set there to be n files in each submitted job"
    print " --noSplitBarrelEndcap - slpit the samples into barrel and endcap during the reading"
    print " --emptyVal <float> - fill the empty distributions with a different number than the -999. default. This is to test how the BDT responds with different values set"
    print " ele - run on the electron channel"
    exit(0)


onlyRunOnOneFile = False
if "--oneFile" in sys.argv:
    onlyRunOnOneFile = True

reducedttbarSet = False

onlySignal = False

runInvMV = True #Compatability in case we don't want to the inverted MC case

useMVA = True

splitBarrelEndcap = True #Default splits the samples between barrel and endcap
if "--noSplitBarrelEndcap" in sys.argv:
    splitBarrelEndcap = False

onlyNominal = False
if "--onlyNominal" in sys.argv:
    onlyNominal = True

runMacro = "runMVAReading"
if not useMVA: runMacro = "runReadingNoMVA"

inputtWIHEPFrameworkDirectory = "tW"

emptyVal = ""
if "--emptyVal" in sys.argv:
    ind = sys.argv.index("--emptyVal")
    emptyVal = ",{0}".format(sys.argv[ind+1])

numberOfFilesPerJob = 5
if "--numberOfFilesPerJob" in sys.argv:
    ind = sys.argv.index("--numberOfFilesPerJob")
    numberOfFilesPerJob = int(sys.argv[ind+1])

outDir = sys.argv[1]

thisDir = os.getcwd()

for i in sys.argv:
    if "-overrideDir" in i:
        overrideDir = i.split("=")[-1]

print thisDir

elePostfix = ""

samplesDataMu = ["SingMuB","SingMuC","SingMuD","SingMuE","SingMuF","SingMuG","SingMuH"]
samplesDataEle = ["SingEleB","SingEleC","SingEleD","SingEleE","SingEleF","SingEleG","SingEleH"]

samplesData = samplesDataMu
if reducedttbarSet: samplesData = []

if "ele" in sys.argv:
    elePostfix = "Ele"
    samplesData = samplesDataEle

#t = mvaTool()

samples = ["tW_top_nfh","tW_antitop_nfh","tW_mcanlo","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","wPlusJetsMadgraph","wPlusJetsMCatNLO","ttbar","ttbarBU","wPlus0Jets","wPlus1Jets","wPlus2Jets","qcd100_200","qcd200_300","qcd300_500","qcd500_700","qcd700_1000","qcd1000_1500","qcd1500_2000"]
samples = ["tW_top_nfh","tW_antitop_nfh","tChan_top","tChan_antitop","sChan","zz","zPlusJetsLowMass","zPlusJetsHighMass","wz","ww","ttbar","ttbarBU","wPlus0Jets","wPlus1Jets","wPlus2Jets","qcd100_200","qcd200_300","qcd300_500","qcd500_700","qcd700_1000","qcd1000_1500","qcd1500_2000"]
if reducedttbarSet: samples = ["ttbar"]

regions = ["3j1t","3j2t","2j1t","4j1t","4j2t","2j0t","3j0t","4j0t"]
if splitBarrelEndcap:
    regions = ["3j1tBarrel","3j2tBarrel","2j1tBarrel","4j1tBarrel","4j2tBarrel","2j0tBarrel","3j0tBarrel","4j0tBarrel",
               "3j1tEndcap","3j2tEndcap","2j1tEndcap","4j1tEndcap","4j2tEndcap","2j0tEndcap","3j0tEndcap","4j0tEndcap"]
if not useMVA: regions = [""]
if reducedttbarSet: regions = [""]
if onlySignal: regions = [""]

#samples = ["tW_top_nfh","tW_antitop_nfh"]

processes = []
systs = ["JESUp","JESDown","JERUp","JERDown"]
systs = [] #These get done internally now
if reducedttbarSet or onlySignal: systs = []

onlySysts = False 

systDir = ""
systDir = "tWSysts"+elePostfix
#if len(sys.argv) > 2:
#    systDir = sys.argv[2]
if reducedttbarSet or onlySignal or not useMVA: systDir = ""
#systDir = ""

if (not os.path.isdir(outDir+"/submit/")):
    subprocess.call("mkdir -p "+outDir + "/submit/",shell=True)
if (not os.path.isdir(outDir+"/logs/")):
    subprocess.call("mkdir -p "+outDir + "/logs/",shell=True)

for i in range(len(regions)):

    region = regions[i]
    
    if (not os.path.isdir(outDir+region)):
        subprocess.call("mkdir -p "+outDir+region+"/",shell=True)
    if (not os.path.isdir(outDir+"Systs"+region+"/")):
        subprocess.call("mkdir -p "+outDir+"Systs"+region+"/",shell=True)
    if (not os.path.isdir(outDir+"QCD"+region+"/")):
        subprocess.call("mkdir -p "+outDir+"QCD"+region+"/",shell=True)
    for syst in systs:
        if (not os.path.isdir(outDir+syst+region+"/")):
            subprocess.call("mkdir -p "+outDir+syst+region+"/",shell=True)

def prepareCshJob(submitFileName,directory,commandToRun):
    subFile      = file(submitFileName,"w")
    print >> subFile, "#!/bin/bash"
    print >> subFile, "/bin/hostname"
    print >> subFile, "gcc -v"
    print >> subFile, "pwd"
    print >> subFile, "source /afs/ihep.ac.cn/soft/CMS/64bit/root/profile/rootenv-entry 6.08.02"
    print >> subFile, "cd {0}".format(directory)
    print >> subFile, commandToRun
    subprocess.call("chmod 777 "+submitFileName,shell=True)

def checkExistance(fileName):
    if not os.path.exists(fileName): return False
    if os.stat(fileName).st_size < 700: return False
    return True

def makeSubmissionFiles(systDir):
    
    toRun = []
    if not onlySysts:
        for sample in samples:
            if numberOfFilesPerJob > 0:
                skimFiles = os.listdir("{0}{2}/{1}/skims/".format(inputtWIHEPFrameworkDirectory,sample,elePostfix))
                nFiles = len(skimFiles)
                tempNFilesPerJob = numberOfFilesPerJob
#                print os.stat("{0}{2}/{1}/skims/{3}".format(inputtWIHEPFrameworkDirectory,sample,elePostfix,skimFiles[0])).st_size
                if os.stat("{0}{2}/{1}/skims/{3}".format(inputtWIHEPFrameworkDirectory,sample,elePostfix,skimFiles[0])).st_size > 100000000: tempNFilesPerJob = 2
                if os.stat("{0}{2}/{1}/skims/{3}".format(inputtWIHEPFrameworkDirectory,sample,elePostfix,skimFiles[0])).st_size > 300000000: tempNFilesPerJob = 1
                if onlyRunOnOneFile: nFiles = 1
                nJobs = int(math.ceil((nFiles-1)/tempNFilesPerJob) +1)
                print sample,nJobs
                for i in range(nJobs):
                    runSample = False
                    for region in regions:
                        if not checkExistance(outDir+region+"/output_"+sample+str(i)+".root"): 
#                            print outDir+region+"/output_"+sample+str(i)+".root"
                            runSample = True
                    if runSample: toRun.append("root -b -q \"../../framework/tmva/{0}.C(\\\"{1}\\\",\\\"{2}{3}/\\\",\\\"{4}\\\",{5},{6},false{7});\"".format(runMacro,sample,inputtWIHEPFrameworkDirectory,elePostfix,outDir,i*tempNFilesPerJob,(i+1)*tempNFilesPerJob-1,emptyVal))
                    if not runInvMV: continue # compatability with old directories
                    if onlyNominal: continue
                    runInvSample = False
                    for region in regions:
                        if not checkExistance(outDir+"QCD"+region+"/output_"+sample+str(i)+".root"): runInvSample = True
                    if runInvSample: toRun.append("root -b -q \"../../framework/tmva/{0}.C(\\\"{1}\\\",\\\"{2}Inv{3}/\\\",\\\"{4}QCD\\\",{5},{6},true{7});\"".format(runMacro,sample,inputtWIHEPFrameworkDirectory,elePostfix,outDir,i*tempNFilesPerJob,(i+1)*tempNFilesPerJob-1,emptyVal))
                continue
                #    continue
#            runSample = False
#            post = ""
#            for region in regions:
#                if not os.path.exists(outDir+region+"/output_"+sample+".root") or os.stat(outDir+region+"/output_"+sample+".root").st_size < 350: runSample = True
#            if runSample: toRun.append("root -b -q \"../../framework/tmva/"+runMacro+".C(\\\""+sample+"\\\",\\\"{0}".format(inputtWIHEPFrameworkDirectory)+elePostfix+"/\\\",\\\""+outDir+"\\\");\"")
#            for syst in systs:
#                runSysts = False
#                for region in regions:
#                    if not os.path.exists(outDir+syst+region+"/output_"+sample+".root") or os.stat(outDir+syst+region+"/output_"+sample+".root").st_size < 350: runSysts = True
#                if runSysts: toRun.append("root -b -q \"../../framework/tmva/"+runMacro+".C(\\\""+sample+"\\\",\\\"{0}".format(inputtWIHEPFrameworkDirectory)+syst+elePostfix+"/\\\",\\\""+outDir+syst+"\\\");\"")

        for sample in samplesData:
            if numberOfFilesPerJob > 0:
                nFiles = len(os.listdir("{0}Data{2}/{1}/skims/".format(inputtWIHEPFrameworkDirectory,sample,elePostfix)))
                if onlyRunOnOneFile: nFiles = 1
                nJobs = int(math.ceil((nFiles-1)/numberOfFilesPerJob) +1)
                print sample,nJobs
                for i in range(nJobs):
                    runSample = False
                    for region in regions:
                        if not checkExistance(outDir+region+"/output_"+sample+str(i)+".root"): 
                            runSample = True
                    if runSample: toRun.append("root -b -q \"../../framework/tmva/{0}.C(\\\"{1}\\\",\\\"{2}Data{3}/\\\",\\\"{4}\\\",{5},{6},true{7});\"".format(runMacro,sample,inputtWIHEPFrameworkDirectory,elePostfix,outDir,i*numberOfFilesPerJob,(i+1)*numberOfFilesPerJob-1,emptyVal))
                    runSample = False
                    if onlyNominal: continue
                    for region in regions:
                        if not checkExistance(outDir+"QCD"+region+"/output_"+sample+str(i)+".root"): runSample = True
                    if runSample: toRun.append("root -b -q \"../../framework/tmva/{0}.C(\\\"{1}\\\",\\\"{2}InvData{3}/\\\",\\\"{4}QCD\\\",{5},{6},true{7});\"".format(runMacro,sample,inputtWIHEPFrameworkDirectory,elePostfix,outDir,i*numberOfFilesPerJob,(i+1)*numberOfFilesPerJob-1,emptyVal))
                                        
                    

#            #    continue
#            runData = False
#            for region in regions:
#                if not os.path.exists(outDir+region+"/output_"+sample+".root") or os.stat(outDir+region+"/output_"+sample+".root").st_size < 350: runData = True
##            if runData: toRun.append("root -b -q \"../../framework/tmva/"+runMacro+".C(\\\""+sample+"\\\",\\\"{0}Data".format(inputtWIHEPFrameworkDirectory)+elePostfix+"/\\\",\\\""+outDir+"\\\");\"")
 #           runQCD = False
 #           for region in regions:
 #               if not os.path.exists(outDir+"QCD"+region+"/output_"+sample+".root") or os.stat(outDir+"QCD"+region+"/output_"+sample+".root") < 350: runQCD = True
 #           if runQCD: toRun.append("root -b -q \"../../framework/tmva/"+runMacro+".C(\\\""+sample+"\\\",\\\"{0}InvData".format(inputtWIHEPFrameworkDirectory)+elePostfix+"/\\\",\\\""+outDir+"QCD\\\");\"")

    #And do the theory uncertainties if they exist
    if systDir and not onlyNominal:
        for sample in os.listdir(systDir):
            systDir = "{2}{0}Systs{1}".format("",elePostfix,inputtWIHEPFrameworkDirectory)
            if numberOfFilesPerJob > 0:
                nFiles = len(os.listdir("{0}/{1}/skims/".format(systDir,sample)))
                if onlyRunOnOneFile: nFiles = 1
                nJobs = int(math.ceil((nFiles-1)/numberOfFilesPerJob) +1)
                print sample,nJobs
                for i in range(nJobs):
                    runSyst = False
                    for region in regions:
                        if not checkExistance(outDir+"Systs"+region+"/output_"+sample+str(i)+".root"): runSyst = True
                    if runSyst: toRun.append("root -b -q \"../../framework/tmva/"+runMacro+".C(\\\""+sample+"\\\",\\\""+systDir+"/\\\",\\\""+outDir+"Systs\\\",{0},{1},true{2});\"".format(i*numberOfFilesPerJob,(i+1)*numberOfFilesPerJob-1,emptyVal))

    nFinished = 0
    nRunning = 0

    #Here we will make condor submission jobs instead of just running it locally because that just won't work anymore.
    submitAll = file(outDir+"/submit/submitAll.sh","w")
    print >> submitAll, "#!/bin/bash"
    dir_path = os.path.dirname(os.path.realpath(__file__))

    for runNum in range(len(toRun)):
        prepareCshJob(outDir+"/submit/submissionJob{0}.sh".format(runNum),thisDir,toRun[runNum])
        print >> submitAll, "hep_sub {0}/submit/submissionJob{1}.sh -o {0}/logs/job{1}.out -e {0}/logs/job{1}.error".format(outDir,runNum)
                      
    print "Made {0} submission files".format(len(toRun))

def makeHaddFile(haddfile):
    for region in regions:
        for sample in samples:
            print >> haddfile, "rm {0}{1}/output_{2}.root".format(outDir,region,sample)
            print >> haddfile, "hadd {0}{1}/output_{2}.root {0}{1}/output_{2}*.root".format(outDir,region,sample)
            print >> haddfile, "rm {0}QCD{1}/output_{2}.root".format(outDir,region,sample)
            print >> haddfile, "hadd {0}QCD{1}/output_{2}.root {0}QCD{1}/output_{2}*.root".format(outDir,region,sample)
        for sample in samplesData:
            print >> haddfile, "rm {0}{1}/output_{2}.root".format(outDir,region,sample)
            print >> haddfile, "hadd {0}{1}/output_{2}.root {0}{1}/output_{2}*.root".format(outDir,region,sample)
            print >> haddfile, "rm {0}QCD{1}/output_{2}.root".format(outDir,region,sample)
            print >> haddfile, "hadd {0}QCD{1}/output_{2}.root {0}QCD{1}/output_{2}*.root".format(outDir,region,sample)
        systDir = "{2}{0}Systs{1}/".format("",elePostfix,inputtWIHEPFrameworkDirectory)
        for sample in os.listdir(systDir):
            print >> haddfile, "rm {0}Systs{1}/output_{2}.root".format(outDir,region,sample)
            print >> haddfile, "hadd {0}Systs{1}/output_{2}.root {0}Systs{1}/output_{2}*.root".format(outDir,region,sample)
    haddfile.close()

if __name__ == "__main__":
    haddFile = ""
    if numberOfFilesPerJob > 0:
        haddFile = file(outDir+"/submit/haddAll.sh","w")
        print >> haddFile, "#!/bin/bash"
        makeHaddFile(haddFile)
    makeSubmissionFiles(systDir)
    print "Made submission files"
    
