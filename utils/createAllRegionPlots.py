import subprocess,os,sys 

if "--help" in sys.argv or "-h" in sys.argv:
    print "Creates plots for all regions\nUsage: createAllRegionPlots.py [options]"
    print "ele to run on electron channel"
    print "--invIsoReg to run on the inverted isolation region"
    print "--qcdMC to use QCD MC instead of data-driven QCD"
    print "--useZeroTag to use the zero tag region for QCD estimation"
    print "-p <dir> postfix you want to plot directories to have"
    print "--isPrefit Use thig flat to indicate that we shouldn't use fitted event weights" 
    print "--noLatexComp Don't do the latex compilation of this code. Useful for if debugging plots"
    print "--includeZeroTag - Include zero tag regions"
    print "--splitBarrelEndcap do the plots separately for barrel/endcap"
    print "--barrel - only do barrel plots"
    print "--endcap - only do endcap plots"
    print "--region <reg> - run on a specific region"
    print "--noNorm - don't do any normalisation of the inverted qcd templates"
    print "--noSub - don't do the subtraction of other inverted templates"
    print "--jsonWeights <jsonWeights> - take dataset weights from a json file"
    print "--useIterJson - use the auto-generated iterative weights"
    print "--plotsForFit - only make the met plots"
    print "--forceDataMax make the plots take the data maximum"
    print "--makeCertainPlots - make some plots instead of making all plots"

    sys.exit(1)

barEndMap = {"Barrel":"barrel","Endcap":"endcap"}

postfix = ""


useAutoJson = False
if "--useIterJson" in sys.argv: useAutoJson = True 

barEnd = [""]
if "--splitBarrelEndcap" in sys.argv:
    barEnd = ["Barrel","Endcap"]

noLatexComp = False
if "--noLatexComp" in sys.argv: noLatexComp = True

doElectron = False
if "ele" in sys.argv:
    doElectron = True

makeQCDRegionPlots = False
if "--invIsoReg" in sys.argv:
    postfix+="Inv"
    makeQCDRegionPlots = True

useQCDMCinPlots = False
if "--qcdMC" in sys.argv:
    postfix+="NoDD"
    useQCDMCinPlots = True

useZeroTag = False

isPrefit = False
prefitPost = ""
if "--isPrefit" in sys.argv:
    isPrefit = True
    prefitPost += " --isPrefit"

if "--forceDataMax" in sys.argv:
    prefitPost += " --forceDataMax"

if "--makeCertainPlots" in sys.argv:
    prefitPost += " --makeCertainPlots"

if "--plotsForFit" in sys.argv:
    prefitPost += " --plotsForFit"

if "--jsonWeights" in sys.argv:
    ind = sys.argv.index("--jsonWeights")
    prefitPost += " --readJSONWeights {0}".format(sys.argv[ind+1])

if "--useZeroTag" in sys.argv:
    prefitPost += " --useZeroTag"

if "--noNorm" in sys.argv:
    prefitPost += " --noNormaliseSubtract"
if "--noSub" in sys.argv: prefitPost += " --noSubtractTemplates"

if "-p" in sys.argv:
    ind = sys.argv.index("-p")
    postfix += sys.argv[ind+1]

if "--barrel" in sys.argv:
    barEnd = ["Barrel"]

if "--endcap" in sys.argv:
    barEnd = ["Endcap"]

#dirMap = {"":"signal","2j1t":"wJets","3j2t":"ttbar","4j1t":"signal2","4j2t":"ttbar2"}
dirMap = ["3j1t","3j2t","2j1t","4j1t","4j2t","all"]
dirMap = ["3j1t","3j2t","2j1t","4j1t","4j2t"]
if "--jetAngFit" in sys.argv: dirMap = ["3j1t","3j2t","4j1t","4j2t"]
if "--region" in sys.argv:
    dirMap = []
    ind = sys.argv.index("--region")
    dirMap.append(sys.argv[ind+1])
#dirMap = ["4j1t","4j2t"]
#dirMap = ["3j1t"]
#dirMap = ["3j1t","3j2t","2j1t","4j1t","4j2t","2j0t","3j0t","4j0t","2j","3j","4j"]
#dirMap = ["2j","3j","4j"]
if "--includeZeroTag" in sys.argv: 
    dirMap += ["2j0t","3j0t","4j0t"]
#    postfix += "ZeroTag"
#dirMap = ["all"]

zeroTagDirs = {"3j1t":"3j0t","3j2t":"3j0t","2j1t":"2j0t","4j1t":"4j0t","4j2t":"4j0t","all":"all"}

elePostfix = ""
jsonNameEle = "mu"
if doElectron: 
    elePostfix = "Ele"
    jsonNameEle = "ele"

subprocesses = []

if True:
    for di in dirMap:
        for barDir in barEnd:
            i = elePostfix+barDir+di
            #if makeQCDRegionPlots: subprocess.call("mkdir -p plots/{0}Inv{1}/".format(i,postfix),shell=True)
            #elif useQCDMCinPlots: subprocess.call("mkdir -p plots/{0}NoDD{1}/".format(i,postfix),shell=True)
            #elif useZeroTag: subprocess.call("mkdir -p plots/{0}ZeroTag{1}/".format(i,postfix),shell=True)
            subprocess.call("mkdir -p plots/{0}{1}/".format(i,postfix),shell=True)

        #    subprocesses.append(subprocess.Popen("python ../../framework/utils/createPlots.py tW{0}/ plots/{1}{2}/ tW{0}Data/ tWInv{0}Data/".format(i,dirMap[i],postfix),shell=True))
            jsonPostfix = ""
            if useAutoJson: jsonPostfix = " --readJSONWeights logs/iterFit/iter1SFs_{0}_{1}.json".format(jsonNameEle,barEndMap[barDir])
            if makeQCDRegionPlots: subprocesses.append(subprocess.Popen("python ../../framework/utils/createPlots.py tWInv{0}/ plots/{0}{1}/ tWInvData{0}/ {2}".format(i,postfix,prefitPost+jsonPostfix),shell=True))
            elif useQCDMCinPlots: subprocesses.append(subprocess.Popen("python ../../framework/utils/createPlots.py tW{0}/ plots/{0}{1}/ tWData{0}/ {2}".format(i,postfix,prefitPost+jsonPostfix),shell=True))
            elif useZeroTag: subprocesses.append(subprocess.Popen("python ../../framework/utils/createPlots.py tW{0}/ plots/{0}{1}/ tWData{0}/ tWInvData{2}/ {3}".format(i,postfix,zeroTagDirs[i],prefitPost+jsonPostfix),shell=True))
            else: subprocesses.append(subprocess.Popen("python ../../framework/utils/createPlots.py tW{0}/ plots/{0}{1}/ tWData{0}/ tWInvData{0}/ {2}".format(i,postfix,prefitPost+jsonPostfix),shell=True))
    #    subprocess.call("cd plots/{0}/".format(dirMap[i]),shell=True)
    #    subprocess.call("pdflatex combinedLatexFile.tex",shell=True)
    #    subprocess.call("cd ../../",shell=True)

for i in subprocesses:
    i.wait()

if noLatexComp: sys.exit(1)

for di in dirMap:
    for barDir in barEnd:
        i = elePostfix+barDir+di
#        if makeQCDRegionPlots: os.chdir("plots/{0}Inv{1}/".format(i,postfix))
#        elif useQCDMCinPlots: os.chdir("plots/{0}NoDD{1}/".format(i,postfix))
#        elif useZeroTag: os.chdir("plots/{0}ZeroTag{1}/".format(i,postfix))
        os.chdir("plots/{0}{1}/".format(i,postfix))
        subprocesses.append(subprocess.Popen("pdflatex combinedLatexFile.tex",shell=True))
        subprocesses.append(subprocess.Popen("pdflatex wJetQCDcomps.tex",shell=True))
        os.chdir("../../")

for i in subprocesses:
    i.wait()

if "--splitBarrelEndcap" in sys.argv:
    if doElectron: postfix += " --electron"
    if "--includeZeroTag" in sys.argv: postfix += " --includeZeroTag"
    subprocess.call("python ../../framework/utils/combineBarrelEndcapPlots.py --inPostfix {0}".format(postfix),shell=True)
