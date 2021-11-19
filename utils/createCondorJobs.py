import sys
import os
import glob
import string
import subprocess
from analysisComponentsNoRoot import AnalysisComponents

comp = AnalysisComponents(analysis="met")


#####
##   Parameters to be specified by the user
#####
#analysis and task
analysis = "met"
taskname = "EvtSel"
frameworkDir = "/publicfs/cms/user/duncanleg/metPaper/tWIHEPFramework/"
executable = "bin/metPaper/metPlots.x"
#executable = "Wt_nVertOnly.x"
configFile = "config/overall/metPaper.config"
#configFile = "config/overall/lhcNomMinBias.config"
invPostfix = ""
mcPostfix = " -mc -lepSFs -PileUpWgt"
#mcPostfix = " -mc -PileUpWgt"
fileListDirectory = "config/files/metFiles/"
fileListSearchTerm = ""
#fileListDirectory = "config/files/abbrevLists20180216/"
useIterFitbTag = False
makeSkims = True
doHists = True
samplesMC = comp.sample
samplesData = comp.dataSamples

#debugging
onlyOne = False

#default to MC
sample = samplesMC

if "data" in sys.argv:
	mcPostfix = ""
	analysis += "Data"
	sample = samplesData
if "systs" in sys.argv:
	analysis += "Systs"
	sample = samplesSyst
	#fileListDirectory = "config/files/systSamples/"
	fileListSearchTerm = "Systs"
if "skims" in sys.argv:
	makeSkims = True 
if "electron" in sys.argv:
	configFile = configFile.split("overall/")[0] + "overall/electron" + configFile.split("overall/")[-1]
	triggerName = "Electron "
#	if not "data" in sys.argv:
	analysis += "Ele"
	fileListSearchTerm += "Ele"
if "jesTest" in sys.argv:
	sample = jesTestSamples
	analysis += "JESTest"
if "tWNLO" in sys.argv:
	sample = tWmcAtNLOSamples
	fileListSearchTerm = "Systs"
if "bTag" in sys.argv:
	sample = bTagSamples
	analysis += "bTag"
	executable = "bin/Wt/Wt_efficiencies.x"
fileListPath = frameworkDir+fileListDirectory
if "fileListDir" in sys.argv:
	fileListPath = sys.argv[sys.argv.index("fileListDir") + 1]
if "bdtSkim" in sys.argv:
	print "Is bdt skim"
	executable = "bin/Wt/Wt_bdtSkim.x"
	doHists = False
	makeSkims = True
#executable = "Wt_generic.x"
#for the queue

workpath    = os.getcwd()+"/"+analysis +"/"
jobDir      = workpath+"/"+"Jobs"
smallerJobs = True
AnalyzerDir = workpath+"/"+"Analyzer"
task        = analysis+"_"+taskname
rootplizer  = "Rootplizer_"+task+".cc"
headplizer  = "Rootplizer_"+task+".h"
#Directory of input files


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

def prepareCshJob(sample,shFile,frameworkDir,workpath,samplePost="",fileListPost="",inListPostFix=""):
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
        print >> subFile, "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"+frameworkDir+"/lib/"
	#print >> subFile, "cp ${jobDir}/getAhist.C ."
#	print >> subFile, frameworkDir+"bin/Wt/Wt_generic.x -config "+frameworkDir+"SingleTop.Wt.LP.mm1+j.muonMSSmeardown.config -inlist "+frameworkDir+"config/files/"+fileListDirectory+sample+samplePost+".list -hfile "+workpath+"/"+sample+"/hists/"+sample+samplePost+"hists.root -skimfile "+workpath+"/"+sample+"/skims/"+sample+samplePost+"Skim.root -mc -BkgdTreeName DiElectronPreTagTree  -UseTotalEvtFromFile -MCatNLO -mc -SelectTrigger Muon -PileUpWgt -BWgt"
	skimString = ""
	histString = ""
	if makeSkims: skimString = " -skimfile "+workpath+"/"+sample+"/skims/"+sample+samplePost+"Skim.root "
	if doHists: histString = " -hfile "+workpath+"/"+sample+"/hists/"+sample+samplePost+"hists.root "
	print >> subFile, frameworkDir+executable+" -config "+frameworkDir+configFile+" -inlist "+fileListPath+sample+inListPostFix+".list -UseTotalEvtFromFile " + invPostfix + mcPostfix + skimString + histString #+ " -nJets {0} -nbJets {1}".format(nJets,nbJets) we don't do this anymore.
        #print >> subFile, "root -b -q -l "+rootplizer+"'(\""+input+"\",\""+output+"\")'"
	subprocess.call("chmod 777 "+shFile, shell=True)

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
		
#		prepareSubmitJob(submitFileName, shFileName, logFileName, errorFileName)
		prepareCshJob(sampleName,shFileName,frameworkDir,workpath)
		
		submitPath = sampleName + "/scripts/" + sampleName + ".submit"
		
		#print >> allJobFile, "condor_submit "+ submitPath + " -group cms -name job@schedd01.ihep.ac.cn"
		print >> allJobFile, "hep_sub "+ shFileName + " -o "+logFileName+ " -e "+errorFileName

	else:
                print fileListPath
		inputFiles = [f for f in os.listdir(fileListPath) if sampleName in f and f.split(sampleName)[-1][:1].isdigit()]
#		if isBDTSkim: inputFiles  = [f for f in os.listdir(fileListPath) if sampleName in f and f.split(sampleName)[-1][:1].isdigit() and fileListSearchTerm in f]
		for j in range(len(inputFiles)):
			submissionName = sampleName + "_" + analysis + "_" + str(j)
			subPostfix = "_" + analysis + "_" + str(j)
			fileListPostFix = str(j)
#			submitFileName = workpath + sampleName + "/scripts/" + sampleName + str(j) + ".submit"
			shFileName = workpath + sampleName + "/scripts/" + submissionName + ".sh"
			logFileName = workpath + sampleName + "/logs/" + submissionName + ".log"
			errorFileName = workpath + sampleName + "/logs/" + submissionName + ".error"
			
#			prepareSubmitJob(submitFileName, shFileName, logFileName, errorFileName)
#			if isBDTSkim: prepareCshJob(sampleName,shFileName,frameworkDir,workpath,subPostfix,fileListSearchTerm,fileListPostFix)
			prepareCshJob(sampleName,shFileName,frameworkDir,workpath,subPostfix,fileListSearchTerm,fileListPostFix)

			submitPath = sampleName + "/scripts/" + submissionName + ".submit"
			
#			print >> allJobFile, "hep_sub "+ submitPath + " -name job@schedd01.ihep.ac.cn"
			print >> allJobFile, "hep_sub "+ shFileName + " -o "+logFileName+ " -e "+errorFileName
                        
                        if onlyOne: break
#			print >> allJobFile, "condor_submit "+ submitPath + " -group cms -name job@schedd01.ihep.ac.cn"

	print >> MergeFile, "hadd -f "+analysis+"/"+sampleName + "/hists/merged"+sampleName+".root  "+analysis+"/"+sampleName + "/hists/"+sampleName+"*hists.root"

print >> allJobFile, "cd -"
print "Finished",analysis
