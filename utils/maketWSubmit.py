import sys
import os
import glob
import string
#####
##   Parameters to be specified by the user
#####
#analysis and task
analysis = "tW"
taskname = "EvtSel"
executable = "Wt_generic.x"
#executable = "Wt_nVertOnly.x"
configFile = "config/overall/SingleTop.Wt.LP.mm1+j.muonMSSmeardown.config"
invPostfix = ""
mcPostfix = " -MCatNLO -mc -bTagReshape -lepSFs -PileUpWgt"
triggerName = "Muon "
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
"tW_top",
"tW_antitop",
"ww",
"wz",
"zz",
"zPlusJetsLowMass",
"zPlusJetsHighMass",
"wPlusJetsMCatNLO",
"tW_top_nfh",
"tW_antitop_nfh"
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
"ttbar_amcatnlo",
"ttbar_hdampup",
"ttbar_hdampdown"
]

fileListDirectory = "moriond17/"
sample = samplesMC
if "inv" in sys.argv:
	invPostfix = " -InvertIsolation"
	analysis += "Inv"
if "wJetsReg" in sys.argv:
	configFile = "config/overall/wJets3j0t.config"
	analysis += "3j0t"
if "ttbarReg" in sys.argv:
	configFile = "config/overall/ttbar3j2t.config"
	analysis += "3j2t"
if "wJets2" in sys.argv:
	configFile = "config/overall/wJets2j1t.config"
	analysis += "2j1t"
if "jesUp" in sys.argv:
	configFile = "config/overall/signal3j1tJESUp.config"
	analysis += "JESUp"
if "jesDown" in sys.argv:
	configFile = "config/overall/signal3j1tJESDown.config"
	analysis += "JESDown"
if "jerUp" in sys.argv:
	configFile = "config/overall/signal3j1tJERUp.config"
	analysis += "JERUp"
if "jerDown" in sys.argv:
	configFile = "config/overall/signal3j1tJERDown.config"
	analysis += "JERDown"
if "wJetsReg" in sys.argv and "ttbarReg" in sys.argv:
	print "Please only use one of ttbar and wJets -Reg! Exiting..."
	sys.exit()
if "data" in sys.argv:
	mcPostfix = ""
	analysis += "Data"
	sample = samplesData
	if "electron" in sys.argv:
		sample = samplesDataElectron
if "systs" in sys.argv:
	analysis += "Systs"
	sample = samplesSyst
	fileListDirectory = "systSamples/"
if "skims" in sys.argv:
	makeSkims = True 
if "electron" in sys.argv:
	configFile = configFile.split("overall/")[0] + "overall/electron" + configFile.split("overall/")[-1]
	triggerName = "Electron "
	if not "data" in sys.argv:
		analysis += "Ele"
#executable = "Wt_generic.x"
#for the queue
workpath    = os.getcwd()+"/"+analysis +"/"
frameworkDir = "/publicfs/cms/user/duncanleg/tW13TeV/framework/"
jobDir      = workpath+"/"+"Jobs"
smallerJobs = True
AnalyzerDir = workpath+"/"+"Analyzer"
task        = analysis+"_"+taskname
rootplizer  = "Rootplizer_"+task+".cc"
headplizer  = "Rootplizer_"+task+".h"
#Directory of input files
nJobs = {
"qcd1000_1500":9,
"qcd100_200":74,
"qcd1500_2000":8,
"qcd2000_inf":4,
"qcd200_300":18,
"qcd300_500":20,
"qcd500_700":21,
"qcd700_1000":19,
"sChan":3,
"tChan_top":73,
"tChan_antitop":41,
#"ttbar":14,
"ttbar":99,
"ttbarBU":120,
"tW_top":2,
"tW_antitop":2,
"ww":2,
"wz":3,
"zz":2,
"tW_top_nfh":20,
"tW_antitop_nfh":19,
"zPlusJetsLowMass":24,
"zPlusJetsHighMass":48,
"wPlusJetsMCatNLO":30,
"SingMuB":176,
"SingMuC":58,
"SingMuD":98,
"SingMuE":83,
"SingMuF":61,
"SingMuG":143,
"SingMuH":147,
"SingEleB":114,
"SingEleC":38,
"SingEleD":38,
"SingEleE":83,
"SingEleF":60,
"SingEleG":143,
"SingEleH":150,
"tW_antitop_DS":9,
"tW_antitop_isrup":10,
"tW_antitop_isrdown":8,
"tW_antitop_fsrup":7,
"tW_antitop_fsrdown":11,
"tW_antitop_herwig":8,
"tW_antitop_MEup":5,
"tW_antitop_MEdown":6,
"tW_antitop_PSup":5,
"tW_antitop_PSdown":4,
"tW_top_DS":9,
"tW_top_isrup":8,
"tW_top_isrdown":11,
"tW_top_fsrup":9,
"tW_top_fsrdown":7,
"tW_top_herwig":10,
"tW_top_MEup":9,
"tW_top_MEdown":9,
"tW_top_PSup":8,
"tW_top_PSdown":10,
"ttbar_isrup":76,
"ttbar_isrdown":35,
"ttbar_fsrup":39,
"ttbar_fsrdown":42,
"ttbar_tuneup":38,
"ttbar_tunedown":40,
"ttbar_herwig":46,
"ttbar_amcatnlo":88,
"ttbar_hdampup":59,
"ttbar_hdampdown":5
}

nJobsSum16 = {
"qcd1000_1500":12,
"qcd100_200":54,
"qcd1500_2000":9,
"qcd2000_inf":5,
"qcd200_300":14,
"qcd300_500":41,
"qcd500_700":44,
"qcd700_1000":36,
"sChan":3,
"tChan_top":33,
"tChan_antitop":152,
"ttbar":34,
"tW_top":3,
"tW_antitop":3,
"ww":3,
"wz":3,
"zz":3,
"zPlusJetsLowMass":74,
"zPlusJetsHighMass":72,
"wPlusJetsMCatNLO":24,
"SingMuB":174,
"SingMuC":58,
"SingMuD":98,
"SingMuE":83,
"SingMuF":61,
"SingMuG":132
}

nJobs76X = {
"qcd1000_1500":13,
"qcd100_200":193,
"qcd1500_2000":10,
"qcd2000_inf":6,
"qcd200_300":46,
"qcd300_500":50,
"qcd500_700":48,
"qcd700_1000":36,
"sChan":3,
"tChan":48,
"ttbar":242,
"tW_top":3,
"tW_antitop":3,
"wPlusJets":115,
"ww":3,
"wz":3,
"zz":3,
"zPlusJetsLowMass":76,
"zPlusJetsHighMass":69,
"wPlusJetsMCatNLO":60,
"singleMuon":108
}
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

def prepareCshJob(sample,shFile,frameworkDir,workpath,samplePost=""):
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
        #print >> subFile, "eval \`scramv1 runtime -sh\`"
        print >> subFile, "cd "+frameworkDir
	#print >> subFile, "cp ${jobDir}/getAhist.C ."
#	print >> subFile, frameworkDir+"bin/Wt/Wt_generic.x -config "+frameworkDir+"SingleTop.Wt.LP.mm1+j.muonMSSmeardown.config -inlist "+frameworkDir+"config/files/"+fileListDirectory+sample+samplePost+".list -hfile "+workpath+"/"+sample+"/hists/"+sample+samplePost+"hists.root -skimfile "+workpath+"/"+sample+"/skims/"+sample+samplePost+"Skim.root -mc -BkgdTreeName DiElectronPreTagTree  -UseTotalEvtFromFile -MCatNLO -mc -SelectTrigger Muon -PileUpWgt -BWgt"
	skimString = ""
	if makeSkims: skimString = " -skimfile "+workpath+"/"+sample+"/skims/"+sample+samplePost+"Skim.root "
	print >> subFile, frameworkDir+"bin/Wt/"+executable+" -config "+frameworkDir+configFile+" -inlist "+frameworkDir+"config/files/"+fileListDirectory+sample+samplePost+".list -hfile "+workpath+"/"+sample+"/hists/"+sample+samplePost+"hists.root -BkgdTreeName DiElectronPreTagTree  -UseTotalEvtFromFile -SelectTrigger " + triggerName + invPostfix + mcPostfix + skimString
        #print >> subFile, "root -b -q -l "+rootplizer+"'(\""+input+"\",\""+output+"\")'"
 
#for iroot in range(nroot):
for k in sample:
	print k
	sampleName = k
	
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
		
		prepareSubmitJob(submitFileName, shFileName, logFileName, errorFileName)
		prepareCshJob(sampleName,shFileName,frameworkDir,workpath)
		
		submitPath = sampleName + "/scripts/" + sampleName + ".submit"
		
		#print >> allJobFile, "condor_submit "+ submitPath + " -group cms -name job@schedd01.ihep.ac.cn"
		print >> allJobFile, "hep_sub "+ submitPath + " -name job@schedd01.ihep.ac.cn"

	else:
		for j in range(nJobs[sampleName]):
			submitFileName = workpath + sampleName + "/scripts/" + sampleName + str(j) + ".submit"
			shFileName = workpath + sampleName + "/scripts/" + sampleName + str(j) + ".sh"
			logFileName = workpath + sampleName + "/logs/" + sampleName + str(j) + ".log"
			errorFileName = workpath + sampleName + "/logs/" + sampleName + str(j) + ".error"
			
			prepareSubmitJob(submitFileName, shFileName, logFileName, errorFileName)
			prepareCshJob(sampleName,shFileName,frameworkDir,workpath,str(j))

			submitPath = sampleName + "/scripts/" + sampleName + str(j) + ".submit"
			
			print >> allJobFile, "hep_sub "+ submitPath + " -name job@schedd01.ihep.ac.cn"
#			print >> allJobFile, "condor_submit "+ submitPath + " -group cms -name job@schedd01.ihep.ac.cn"

	print >> MergeFile, "hadd -f "+analysis+"/"+sampleName + "/hists/merged"+sampleName+".root  "+analysis+"/"+sampleName + "/hists/"+sampleName+"*hists.root"

#print >> MergeFile, "cd",outputDirectory
#print >> MergeFile, "hadd Merged_rootplas.root",MergeSourceFile

if mcPostfix == "":
	if "electron" in sys.argv:
		print >> MergeFile, "mkdir -p "+analysis+"/singleEle/hists/"
		print >> MergeFile, "hadd -f "+analysis+"/singleEle/hists/mergedsingleMEle.root " + analysis+"/SingEle*/hists/merged*"
	else:
		print >> MergeFile, "mkdir -p "+analysis+"/singleMuon/hists/"
		print >> MergeFile, "hadd -f "+analysis+"/singleMuon/hists/mergedsingleMuon.root " + analysis+"/SingMu*/hists/merged*"

print >> allJobFile, "cd -"
print "Finished",analysis
