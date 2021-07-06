import subprocess, os, sys, json

subprocesses = []

inDirPotsFix = "prefit"
inDirPotsFix = ""

if "--help" in sys.argv or "-h" in sys.argv:
    print "Run the fit over the normal regions - by default it runs on mtw"
    print "--inDirPost <dirPost> changes the psotfix of the directory used to do the fit. Default: <{0}>".format(inDirPotsFix)
    print "--metFit makes the fit be on met instead of mtw"
    print "--jetAngFit - make fit on a jet angular dist"
    print "--customHist <histName> - use a non default histogram name"
    print "--zeroTag - Run over the zero tag regions instead"
    print "--barrelEndcap - split the fit between the barrel and endcap"
    print "--barrel - only run on the barrel"
    print "--endcap - only ru8n on the endcap"
    print "--outJSONFile <outFile> dump the weights into a json file"
    print "--fixWJets - fix wJets in the fit"
    print "ele runs the electron channel"

    sys.exit(1)
 
fitPost = ""
if "--fixWJets" in sys.argv: fitPost+=" --fixWJets"

outJSONFile = ""
if "--outJSONFile" in sys.argv:
    ind = sys.argv.index("--outJSONFile")
    outJSONFile = sys.argv[ind+1]

barEnd = [""]
if "--barrelEndcap" in sys.argv: barEnd = ["Barrel","Endcap"]
if "--barrel" in sys.argv: barEnd = ["Barrel"]
if "--endcap" in sys.argv: barEnd = ["Endcap"]

directoryName = "plots/"
if "ele" in sys.argv: directoryName = "plots/Ele"

if "--inDirPost" in sys.argv:
    ind = sys.argv.index("--inDirPost")
    inDirPotsFix = sys.argv[ind+1]

runOnZeroTag = False
if "--zeroTag" in sys.argv:
    runOnZeroTag = True

histName = "16_HistogrammingMtW_mTW.root"
if "--metFit" in sys.argv: histName = "15_HistogrammingMET_MET_xy.root"
if "--jetAngFit" in sys.argv: histName = "17_HistogrammingJetAngular_bJet1DelR.root"
if "--customHist" in sys.argv:
    ind = sys.argv.index("--customHist")
    histName = sys.argv[ind+1]
#histName = "19_HistogrammingJet_Jet1Pt.root"
#histName = "17_Variables_BDTVars_M_DeltaPhiLeptonJet2.root"
outDir = "fitOutputs"

if not os.path.exists(outDir):
    os.mkdir(outDir)

regions = ["3j1t","2j1t","3j2t","4j1t","4j2t" ]
regions = ["3j1t","2j1t","3j2t","4j1t" ]
if runOnZeroTag: regions += ["2j0t","3j0t","4j0t"]

if True:
    for region in regions:
        for barEn in barEnd:
            subprocesses.append(subprocess.Popen("python ../../framework/utils/bkgFit/fit.py {0}{5}{1}{4}/{2} {1} {6} > {3}/fitOutput{5}{1}.txt".format(directoryName,region,histName,outDir,inDirPotsFix,barEn,fitPost),shell=True))

for i in range(len(subprocesses)):
    subprocesses[i].wait()
              
wCoeefs = {}
qcdCoeefs = {}

for barEn in barEnd:    
    wCoeefs[barEn] = {}
    qcdCoeefs[barEn] = {}
    for region in regions:
        outFile = open("{0}/fitOutput{2}{1}.txt".format(outDir,region,barEn))
        for line in outFile:
            if "wJet SF" in line:
                wCoeefs[barEn][region] = line.split(" = ")[1][:-1]
            if "QCD SF" in line:
                qcdCoeefs[barEn][region] = line.split(" = ")[1][:-1]
        print "{0} WJet SF: {1:.3f} QCD SF: {2:.3f}".format(region+barEn, float(wCoeefs[barEn][region]),float(qcdCoeefs[barEn][region]))

print "wJets"
for barEn in barEnd:
    for region in regions:
        print "\"{0}\":2.98 * {1:.4f},".format(region+barEn,float(wCoeefs[barEn][region])),
    print

print "\nQCD"
for barEn in barEnd:
    for region in regions:
        print "\"{0}\":{1:.4f},".format(region+barEn,float(qcdCoeefs[barEn][region])),
    print 

coeffs = {"wPlusJets":wCoeefs,"qcd":qcdCoeefs}
print coeffs
    

if outJSONFile:
#    json_dump = json.dumps(coeffs)
    with open(outJSONFile, 'w') as outfile:
        json.dump(coeffs, outfile)
