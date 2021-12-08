import sys
import os
import glob
import string
import subprocess
from analysisComponentsNoRoot import AnalysisComponents

from optparse import OptionParser

def prepareCshJob(sample,shFile,frameworkDir,workpath,samplePost,fileListPath,inListPostFix,executable,configFile,doHists,makeSkims,additionalString):
        subFile      = file(shFile,"w")
	print >> subFile, "#!/bin/bash"
	print >> subFile, "/bin/hostname"
	print >> subFile, "gcc -v"
	print >> subFile, "source  /afs/ihep.ac.cn/soft/CMS/64bit/root/profile/rootenv-entry 5.34.18"
        print >> subFile, "source /cvmfs/sft.cern.ch/lcg/views/LCG_93/x86_64-slc6-gcc62-opt/setup.sh"
        print >> subFile, "cd "+frameworkDir
        print >> subFile, "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"+frameworkDir+"/lib/"

	skimString = ""
	histString = ""
	if makeSkims: skimString = " -skimfile "+workpath+"/"+sample+"/skims/"+sample+samplePost+"Skim.root "
	if doHists: histString = " -hfile "+workpath+"/"+sample+"/hists/"+sample+samplePost+"hists.root "
	print >> subFile, frameworkDir+executable+" -config "+frameworkDir+configFile+" -inlist "+fileListPath+sample+inListPostFix+".list -UseTotalEvtFromFile " + skimString + histString + additionalString

	subprocess.call("chmod 777 "+shFile, shell=True)

#for iroot in range(nroot):
def makeSubmitInOneDir(analysis,workpath,samples,fileListPath,frameworkDir,executable,configFile,doHists,makeSkims,additionalString):
        
        allJobFileName = analysis+"Submit.sh"
        allJobFile      = file(allJobFileName,"w")
        print >> allJobFile, "#!/bin/bash"
        print >> allJobFile, "cd ",analysis
        
        MergeFileName = analysis+"merge.sh"
        MergeFile      = file(MergeFileName,"w")
        MergeSourceFile = " "


        for sampleName in samples:
               
                print sampleName
                               
                os.popen('mkdir -p '+workpath + sampleName)
                os.popen('mkdir -p '+workpath + sampleName + "/scripts")
                os.popen('mkdir -p '+workpath + sampleName + "/hists")
                os.popen('mkdir -p '+workpath + sampleName + "/skims")
                os.popen('mkdir -p '+workpath + sampleName + "/logs")
                
                print fileListPath
                inputFiles = [f for f in os.listdir(fileListPath) if sampleName in f and f.split(sampleName)[-1][:1].isdigit()]
                
                for j in range(len(inputFiles)):
                        submissionName = sampleName + "_" + analysis + "_" + str(j)
                        subPostfix = "_" + analysis + "_" + str(j)
                        fileListPostFix = str(j)
                        
                        shFileName = workpath + sampleName + "/scripts/" + submissionName + ".sh"
                        logFileName = workpath + sampleName + "/logs/" + submissionName + ".log"
                        errorFileName = workpath + sampleName + "/logs/" + submissionName + ".error"
                        
                        prepareCshJob(sampleName,shFileName,frameworkDir,workpath,subPostfix,fileListPath,fileListPostFix,executable,configFile,doHists,makeSkims,additionalString)
                        
                        submitPath = sampleName + "/scripts/" + submissionName + ".submit"

                        print >> allJobFile, "hep_sub "+ shFileName + " -o "+logFileName+ " -e "+errorFileName


                print >> MergeFile, "hadd -f "+analysis+"/"+sampleName + "/hists/merged"+sampleName+".root  "+analysis+"/"+sampleName + "/hists/"+sampleName+"*hists.root"
                
        print "Finished",analysis


def addToOverallJob(analysis):
        allSubmit = 0
        allMerge = 0
        if not os.path.exists(os.getcwd()+"/all.sh"):
                allSubmit = open(os.getcwd()+"/all.sh","w")
                allMerge = open(os.getcwd()+"/mergeAll.sh","w")
                allSubmit.write("#!/bin/bash\n")
                allMerge.write("#!/bin/bash\n")
                allSubmit.close()
                allMerge.close()
        allSubmit = open(os.getcwd()+"/all.sh","a+")
        allMerge = open(os.getcwd()+"/mergeAll.sh","a+")
        addAnalysis = True
        for line in allSubmit:
                if "bash "+analysis+"S" in line: addAnalysis = False

        if addAnalysis:
                allSubmit.write("bash "+analysis+"Submit.sh\n")
                allMerge.write("bash "+analysis+"merge.sh\n")
        allSubmit.close()
        allMerge.close()

if __name__ == "__main__":

        parser = OptionParser(usage="usage: %prog [options] \nrun with --help to get list of options")
        parser.add_option("-b","--baseDirName",dest="baseDirName",default="met",type="string",help="The base name of the directory structure")
        parser.add_option("-c","--compName",dest="compName",default="met",type="string",help="The name to pass to analysisComponents")
        parser.add_option("--lepton",dest="lepton",default="muon",type="string",help="Lepton to use")
        parser.add_option("-y","--year",dest="year",default="2018",type="string",help="Which year to run on")
        parser.add_option("--hists",dest="doHists",default=False,action="store_true",help="Make histogram output")
        parser.add_option("-s","--skims",dest="makeSkims",default=False,action="store_true",help="Make skim output")
        parser.add_option("-x","--executable",dest="executable",default="bin/metPaper/metPlots.x",type="string",help="The executable to use")
        parser.add_option("-f","--frameworkDir",dest="frameworkDir",default="/publicfs/cms/user/duncanleg/metPaper/tWIHEPFramework/",type="string",help="Address of the framework to use")
        parser.add_option("--fileLists",dest="fileListDirectory",default="/publicfs/cms/user/duncanleg/metPaper/tWIHEPFramework/config/files/ul2018/",type="string",help="The directory containing the file lists")
        parser.add_option("-p","--noPileup",dest="noPileup",default=False,action="store_true",help="Flag to not run pileup")
        parser.add_option("-l","--lepSFs",dest="noLepSFs",default=False,action="store_true",help="Flag to not run lepton SFs")
        parser.add_option("--configFile",dest="configFile",default="config/overall/met_2018_dimu.config",help="The configuration file to run on")
        parser.add_option("-d","--data",dest="isData",default=False,action="store_true",help="Flag for if running on data")

        (options, args) = parser.parse_args()

        comp = AnalysisComponents(options.compName,lepton=options.lepton,era=options.year)

        analysis = options.baseDirName + options.year[2:]

        if options.lepton == "electron":
                analysis+="Ele"

        samples = comp.samples
        additionalString = ""

        if options.isData: 
                samples = comp.dataSamples
                analysis+="Data"
        else:
                additionalString += " -mc"
                if not options.noPileup: additionalString += " -PileUpWgt"
                if not options.noLepSFs: additionalString += " -lepSFs"
                

        workpath = os.getcwd()+"/"+analysis +"/"
        
        fileListDir = "/publicfs/cms/user/duncanleg/metPaper/tWIHEPFramework/config/files/ul{0}/".format(options.year)

        makeSubmitInOneDir(analysis,workpath,samples,fileListDir,options.frameworkDir,options.executable,options.configFile,options.doHists,options.makeSkims,additionalString)

        addToOverallJob(analysis)
