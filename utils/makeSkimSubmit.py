import sys
import os
import glob
import string
import subprocess
#####
##   Parameters to be specified by the user
#####
#analysis and task
analysis = "tW"
taskname = "EvtSel"
frameworkDir = "/publicfs/cms/user/duncanleg/tW13TeV/framework/"
executable = "bin/Wt/Wt_channelSkimHists.x"
#executable = "Wt_nVertOnly.x"
configFile = "config/overall/SingleTop.Wt.LP.mm1+j.muonMSSmeardown.config"
invPostfix = ""
mcPostfix = " -MCatNLO -mc -bTagReshape -lepSFs -PileUpWgt"
triggerName = "Muon "
nJets = 3
nbJets = 1
fileListSearchTerm = "nominal"
fileListDirectory = "inputLists/"
useIterFitbTag = False
makeSkims = False
samplesMC76=[
"qcd1000_1500",
"qcd100_200",
"qcd1500_2000",
"qcd2000_inf",
"qcd200_300",
"qcd300_500",
"qcd500_700",
"qcd700_1000",
"sChan",
"tChan",
"ttbar",
"tW_top",
"tW_antitop",
"wPlusJets",
"ww",
"wz",
"zz",
"zPlusJetsLowMass",
"zPlusJetsHighMass",
"wPlusJetsMCatNLO"
]
samplesMC=[
"qcd1000_1500",
"qcd100_200",
"qcd1500_2000",
"qcd2000_inf",
"qcd200_300",
"qcd300_500",
"qcd500_700",
"qcd700_1000",
"sChan",
"tChan_top",
"tChan_antitop",
"ttbar",
"ttbarBU",
#"tW_top",
#"tW_antitop",
"ww",
"wz",
"zz",
"zPlusJetsLowMass",
"zPlusJetsHighMass",
"wPlusJetsMCatNLO",
"wPlusJetsMadgraph",
"tW_top_nfh",
"tW_antitop_nfh",
"tW_mcanlo",
"wPlus0Jets",
"wPlus1Jets",
"wPlus2Jets"
]
samplesData2015=[
"singleMuon"
]
samplesData=[
"SingMuB",
"SingMuC",
"SingMuD",
"SingMuE",
"SingMuF",
"SingMuG",
"SingMuH"
]

samplesDataElectron=[
"SingEleB",
"SingEleC",
"SingEleD",
"SingEleE",
"SingEleF",
"SingEleG",
"SingEleH"
]
samplesSyst = [
"tW_antitop_DS",
"tW_antitop_isrup",
"tW_antitop_isrdown",
"tW_antitop_fsrup",
"tW_antitop_fsrdown",
"tW_antitop_herwig",
"tW_antitop_MEup",
"tW_antitop_MEdown",
"tW_antitop_PSup",
"tW_antitop_PSdown",
"tW_top_DS",
"tW_top_isrup",
"tW_top_isrdown",
"tW_top_fsrup",
"tW_top_fsrdown",
"tW_top_herwig",
"tW_top_MEup",
"tW_top_MEdown",
"tW_top_PSup",
"tW_top_PSdown",
"ttbar_isrup",
"ttbar_isrdown",
"ttbar_fsrup",
"ttbar_fsrdown",
"ttbar_tuneup",
"ttbar_tunedown",
"ttbar_herwig",
"ttbar_mcatnlo",
"ttbar_hdampup",
"ttbar_hdampdown"
]
jesTestSamples = ["JESTest"]
bTagSamples = ["tW_top_nfh",
"tW_antitop_nfh",
"ttbar",
"ttbarBU"
]
tW_mcanloSamples = ["tW_mcanlo"]
#systSamples = ["ttbar_hdampdown"]
#mcSamples = []
#samplesData = []
sample = samplesMC
fileListSearchTerm = ""
if "inv" in sys.argv:
	invPostfix = " -InvertIsolation"
	analysis += "Inv"
	fileListSearchTerm = "Inv"
if "wJetsReg" in sys.argv:
	nJets = 3
	nbJets = 0
	analysis += "3j0t"
if "ttbarReg" in sys.argv:
	nJets = 3
	nbJets = 2
	analysis += "3j2t"
if "wJets2" in sys.argv:
	nJets = 2
	nbJets = 1
	analysis += "2j1t"
if "ttbar2" in sys.argv:
	nJets = 4
	nbJets = 2
	analysis += "4j2t"
if "sig2" in sys.argv:
	nJets = 4
	nbJets = 1
	analysis += "4j1t"
if "jesUp" in sys.argv:
	configFile = "config/overall/signal3j1tJESUp.config"
	analysis += "JESUp"
	fileListSearchTerm = "JESUp"
if "jesDown" in sys.argv:
	configFile = "config/overall/signal3j1tJESDown.config"
	analysis += "JESDown"
	fileListSearchTerm = "JESDown"
if "jerUp" in sys.argv:
	configFile = "config/overall/signal3j1tJERUp.config"
	analysis += "JERUp"
	fileListSearchTerm = "JERUp"
if "jerDown" in sys.argv:
	configFile = "config/overall/signal3j1tJERDown.config"
	analysis += "JERDown"
	fileListSearchTerm = "JERDown"
if "wJetsReg" in sys.argv and "ttbarReg" in sys.argv:
	print "Please only use one of ttbar and wJets -Reg! Exiting..."
	sys.exit()
if "data" in sys.argv:
	mcPostfix = ""
	analysis += "Data"
	sample = samplesData
#	fileListSearchTerm = "Data"
#	if "inv" in sys.argv: fileListSearchTerm = "InvData"
	if "electron" in sys.argv:
		sample = samplesDataElectron
if "systs" in sys.argv:
	analysis += "Systs"
	sample = samplesSyst
	fileListSearchTerm = "Systs"
#	fileListDirectory = "config/files/systSamples/"
if "skims" in sys.argv:
	makeSkims = True 
if "qcdEnriched" in sys.argv:
	configFile = "config/overall/qcdEnriched.config"
if "qcdReject" in sys.argv:
	configFile = "config/overall/qcdReject.config"
if "electron" in sys.argv:
	configFile = configFile.split("overall/")[0] + "overall/electron" + configFile.split("overall/")[-1]
	triggerName = "Electron "
#	if not "data" in sys.argv:
	analysis += "Ele"
	#fileListSearchTerm += "Ele"
if "jesTest" in sys.argv:
	sample = jesTestSamples
	analysis += "JESTest"
if "bTag" in sys.argv:
	sample = bTagSamples
	analysis += "bTag"
	executable = "bin/Wt/Wt_efficiencies.x"
if "tW_mcanlo" in sys.argv:
	sample = tW_mcanloSamples
if "barrel" in sys.argv:
	analysis += "Barrel"
	mcPostfix += " -detRegSelect 1"
if "endcap" in sys.argv:
	analysis += "Endcap"
	mcPostfix += " -detRegSelect 0"
if not useIterFitbTag: mcPostfix += " -noIterFitbTag"
if "prefireVeto" in sys.argv and not "data" in sys.argv: mcPostfix += " -VetoPrefire"
#We do this if we want to reweight the distributions in the inverted isolation lepton region
#if "electron" in sys.argv and "inv" in sys.argv and "data" in sys.argv:
if "inv" in sys.argv and "data" in sys.argv: #We'll do this ffor muons as well as a test
	mcPostfix += " -reweightLeptonPt"
#executable = "Wt_generic.x"
#for the queue
#fileListSearchTerm = ""
workpathBase    = os.getcwd()+"/"+analysis
workpathSuperBase = os.getcwd()+"/"
jobDir      = workpathBase+"/"+"Jobs"
smallerJobs = True
AnalyzerDir = workpathBase+"/"+"Analyzer"
task        = analysis+"_"+taskname
rootplizer  = "Rootplizer_"+task+".cc"
headplizer  = "Rootplizer_"+task+".h"
#Directory of input files

#####
##   The script itsself
#####
#cshFilePath = jobDir+"/"+"sh"
#logFilePath = jobDir+"/"+"log"
if os.path.exists(jobDir):
	os.popen('rm -fr '+jobDir)
if os.path.exists(AnalyzerDir):
        os.popen('rm -fr '+AnalyzerDir)
#os.popen('mkdir -p '+cshFilePath)
#os.popen('mkdir -p '+logFilePath)
allSubmit = 0
allMerge = 0
if os.path.exists(os.getcwd()+"/all.sh"):
	allSubmit = open(os.getcwd()+"/all.sh","a")
	allMerge = open(os.getcwd()+"/mergeAll.sh","a")
else:
	allSubmit = open(os.getcwd()+"/all.sh","w")
	allMerge = open(os.getcwd()+"/mergeAll.sh","w")
	allSubmit.write("#!/bin/bash\n")
	allMerge.write("#!/bin/bash\n")
allSubmit.write("bash "+analysis+"Submit.sh\n")
allMerge.write("bash "+analysis+"merge.sh\n")
allSubmit.close()
allMerge.close()

allJobFileName = analysis+"Submit.sh"
allJobFile      = file(allJobFileName,"w")
print >> allJobFile, "#!/bin/bash"
print >> allJobFile, "cd ",analysis

MergeFileName = analysis+"merge.sh"
MergeFile      = file(MergeFileName,"w")
MergeSourceFile = " "
def prepareSubmitJob(submitFileName,cshFileName, outPutFileName, errorFileName):
	cshFile      = file(submitFileName,"w")
	print >> cshFile, "Universe     = vanilla"
	print >> cshFile, "getenv       = true"
	print >> cshFile, "Executable   = ",cshFileName
	print >> cshFile, "Output       = ",outPutFileName
	print >> cshFile, "Error        = ",errorFileName
	print >> cshFile, "Queue"

def prepareCshJob(sample,shFile,frameworkDir,workpath,regionVariable,samplePost="",listPostfix=""):
        subFile      = file(shFile,"w")
	print >> subFile, "#!/bin/bash"
	print >> subFile, "/bin/hostname"
	print >> subFile, "gcc -v"
	print >> subFile, "pwd"
	#print >> subFile, "cd /publicfs/cms/data/TopQuark/cms13TeV/software/root/bin/"
	#print >> subFile, "source thisroot.csh"
	#print >> subFile, "cd /publicfs/cms/user/libh/CMSSW_5_3_9/src/ttH_13Tev"
	#print >> subFile, "setenv SCRAM_ARCH slc5_amd64_gcc462"
	#print >> subFile, "source /cvmfs/cms.cern.ch/cmsset_default.csh"
	print >> subFile, "source  /afs/ihep.ac.cn/soft/CMS/64bit/root/profile/rootenv-entry 5.34.18"
        print >> subFile, "source /cvmfs/sft.cern.ch/lcg/views/LCG_93/x86_64-slc6-gcc62-opt/setup.sh"
        #print >> subFile, "eval \`scramv1 runtime -sh\`"
        print >> subFile, "cd "+frameworkDir
	#print >> subFile, "cp ${jobDir}/getAhist.C ."
#	print >> subFile, frameworkDir+"bin/Wt/Wt_generic.x -config "+frameworkDir+"SingleTop.Wt.LP.mm1+j.muonMSSmeardown.config -inlist "+frameworkDir+"config/files/"+fileListDirectory+sample+samplePost+".list -hfile "+workpath+"/"+sample+"/hists/"+sample+samplePost+"hists.root -skimfile "+workpath+"/"+sample+"/skims/"+sample+samplePost+"Skim.root -mc -BkgdTreeName DiElectronPreTagTree  -UseTotalEvtFromFile -MCatNLO -mc -SelectTrigger Muon -PileUpWgt -BWgt"
	skimString = ""
	if makeSkims: skimString = " -skimfile "+workpath+"/"+sample+"/skims/"+sample+samplePost+"Skim.root "
	print >> subFile, frameworkDir+executable+" -config "+frameworkDir+configFile+" -inlist "+workpathSuperBase+fileListDirectory+sample+listPostfix+".list -hfile "+workpath+"/"+sample+"/hists/"+sample+samplePost+"hists.root -BkgdTreeName DiElectronPreTagTree  -UseTotalEvtFromFile -SelectTrigger " + triggerName + " -regionSelect " + regionVariable + invPostfix + mcPostfix + skimString #+ " -nJets {0} -nbJets {1}".format(nJets,nbJets) we don't do this anymore.
        #print >> subFile, "root -b -q -l "+rootplizer+"'(\""+input+"\",\""+output+"\")'"
	subprocess.call("chmod 777 "+shFile, shell=True)

#for iroot in range(nroot):
regions = ["3j1t","3j2t","2j1t","4j1t","4j2t","2j0t","3j0t","4j0t","2j","3j","4j","all"]
#regions = ["","","","","","","","","2j","3j","4j"]
#regions = ["","","","","","","","","","","","all"]

regions = ["3j1t","3j2t","2j1t","4j1t","4j2t","","","","","","","all"]
regions = ["3j1t","3j2t","2j1t","4j1t","4j2t","2j0t","3j0t","4j0t","","","","all"]
regions = ["3j1t","3j2t","2j1t","4j1t","4j2t"]
regions = ["3j1t","3j2t","2j1t","4j1t","","2j0t","3j0t","4j0t"]
#regions = ["","","","","","2j0t","3j0t","4j0t","","","",""]
#regions = ["","","","4j1t","4j2t"]
for regionVar in range(len(regions)):
	if regions[regionVar] == "": continue
	print regions[regionVar]
	for k in sample:
		print k
		sampleName = k
		workpath = workpathBase+regions[regionVar]+"/"
	   #First, let's get rid of any 
		
		os.popen('mkdir -p '+workpath + sampleName)
		os.popen('mkdir -p '+workpath + sampleName + "/scripts")
		os.popen('mkdir -p '+workpath + sampleName + "/hists")
		os.popen('mkdir -p '+workpath + sampleName + "/skims")
		os.popen('mkdir -p '+workpath + sampleName + "/logs")

		if not smallerJobs:

			submitFileName = workpath + sampleName + "/scripts/" + sampleName + ".submit"
			shFileName = workpath + sampleName + "/scripts/" + sampleName +  ".sh"
			logFileName = workpath + sampleName + "/logs/" + sampleName + ".log"
			errorFileName = workpath + sampleName + "/logs/" + sampleName + ".error"

   #		prepareSubmitJob(submitFileName, shFileName, logFileName, errorFileName)
			prepareCshJob(sampleName,shFileName,frameworkDir,workpath,regionVar)

			submitPath = sampleName + "/scripts/" + sampleName + ".submit"

		   #print >> allJobFile, "condor_submit "+ submitPath + " -group cms -name job@schedd01.ihep.ac.cn"
			print >> allJobFile, "hep_sub "+ shFileName + " -o "+logFileName+ " -e "+errorFileName

		else:
			inputFiles  = [f for f in os.listdir(workpathSuperBase+fileListDirectory) if sampleName in f and f.split(sampleName)[-1][:1].isdigit() and fileListSearchTerm in f]
			for j in range(len(inputFiles)):
				if not os.path.exists(workpathSuperBase+fileListDirectory+sampleName+str(j)+fileListSearchTerm+".list"): continue
			   #			submitFileName = workpath + sampleName + "/scripts/" + sampleName + str(j) + ".submit"
				submissionName = sampleName + fileListSearchTerm + "_" + regions[regionVar] + "_" + analysis + str(j)
				subPost = fileListSearchTerm + "_" + regions[regionVar] + "_" + analysis + str(j)
				shFileName = workpath + sampleName + "/scripts/" + submissionName + ".sh"
				logFileName = workpath + sampleName + "/logs/" + submissionName + ".log"
				errorFileName = workpath + sampleName + "/logs/" + submissionName + ".error"

   #			prepareSubmitJob(submitFileName, shFileName, logFileName, errorFileName)
				prepareCshJob(sampleName,shFileName,frameworkDir,workpath,str(regionVar),subPost,str(j)+fileListSearchTerm)

				submitPath = sampleName + "/scripts/" + submissionName + ".submit"

   #			print >> allJobFile, "hep_sub "+ submitPath + " -name job@schedd01.ihep.ac.cn"
				print >> allJobFile, "hep_sub "+ shFileName + " -o "+logFileName+ " -e "+errorFileName
#			print >> allJobFile, "condor_submit "+ submitPath + " -group cms -name job@schedd01.ihep.ac.cn"

		print >> MergeFile, "hadd -f "+analysis+regions[regionVar]+"/"+sampleName + "/hists/merged"+sampleName+".root  "+analysis+regions[regionVar]+"/"+sampleName + "/hists/"+sampleName+"*hists.root"
	 
#print >> MergeFile, "cd",outputDirectory
#print >> MergeFile, "hadd Merged_rootplas.root",MergeSourceFile

	if "data" in sys.argv:
		lepton = "Mu"
		if "electron" in sys.argv: lepton = "Ele"
		print >> MergeFile, "mkdir -p "+analysis+regions[regionVar]+"/single{0}/hists/".format(lepton)
		print >> MergeFile, "hadd -f "+analysis+regions[regionVar]+"/single{0}/hists/mergedsingle{0}.root ".format(lepton) + analysis+regions[regionVar]+"/Sing{0}*/hists/merged*".format(lepton)

print >> allJobFile, "cd -"
print "Finished",analysis
