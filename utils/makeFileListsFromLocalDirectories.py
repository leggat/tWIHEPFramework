#A short script that takes input from the user to create a set of file lists to run on condor. the inputs are:
# Number of files per job - how many files to put in each file list
# Name of dataset
# Total number of files in dataset
# Dataset's designated number according to the configuration file in configs

import sys,os,math

#datasets lists the names of the datasets to be processed in this script
datasets = [
"dy",
"doubleMu_2018a",
"doubleMu_2018b",
"doubleMu_2018c",
"doubleMu_2018d",
]

#datasetID is a map of MC information ID number for each sample name
datasetID = {
"dy":600000,
"doubleMu_2018a":400000,
"doubleMu_2018b":400001,
"doubleMu_2018c":400002,
"doubleMu_2018d":400003,

}

#A map of a list of directories containing files to be associated by the sample name
datasetDirs = {
"doubleMu_2018a":["/publicfs/cms/data/TopQuark/nanoAOD/data/UL2018A/"],
"doubleMu_2018b":["/publicfs/cms/data/TopQuark/nanoAOD/data/UL2018B/"],
"doubleMu_2018c":["/publicfs/cms/data/TopQuark/nanoAOD/data/UL2018C/"],
"doubleMu_2018d":["/publicfs/cms/data/TopQuark/nanoAOD/data/UL2018D/"],
"dy":["/publicfs/cms/data/TopQuark/nanoAOD/mc/dy_ll/"]
#"tW_antitop":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_antitop_NFHext1updatedEleJets/180508_154320/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_antitop_NFHext2updatedEleJets/180517_145622/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_antitop_NFHupdatedEleJets/180508_154202/0000/"],
#"tW_top":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_top_NFHext1updatedEleJets/180508_153925/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_top_NFHext2updatedEleJets/180508_154046/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_top_NFHupdatedEleJets/180508_153756/0000/"]
}

#The number of files to be put into each file list. A smaller number means each condor job will take less time, at the expense of more required jobs
namesPerFile = 1

#the directory to write the lists to
outputDirectory = "config/files/metFiles/"
if (not os.path.exists(outputDirectory)):
    os.makedirs(outputDirectory)

#Make the files for each sample
for dataset in datasets:
    
    fileList = []
    for datasetDir in datasetDirs[dataset]:
        fileList += [os.path.join(datasetDir,f) for f in os.listdir(datasetDir) if ".root" in f]
    nJobs = int(math.ceil(len(fileList)/namesPerFile))
    print ("Dataset: {0}, ID: {1}. Number of jobs created = {2}".format(dataset,datasetID[dataset],nJobs))
    nFile = 0
    for i in range(nJobs):
        currentFile = open(outputDirectory + dataset + str(i) + ".list","w")
        currentFile.write("Name: " + dataset)
        currentFile.write("\nNumber: " + str(datasetID[dataset]) + "_1\n")
        for j in range(namesPerFile):
            if nFile >= len(fileList): continue
            currentFile.write(fileList[nFile]+"\n")
            nFile+=1
        currentFile.close()
        
    continue

  
print "Thank you for using the create jobs program. Have a nice day"
