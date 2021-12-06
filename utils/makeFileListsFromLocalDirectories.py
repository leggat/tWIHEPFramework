#A short script that takes input from the user to create a set of file lists to run on condor. the inputs are:
# Number of files per job - how many files to put in each file list
# Name of dataset
# Total number of files in dataset
# Dataset's designated number according to the configuration file in configs

import sys,os,math

#datasets lists the names of the datasets to be processed in this script
datasetsAllYears = {"2018":[
"dy",
"ttbar_2l",
"ww",
"wz",
"zz",
"doubleMu_2018a",
"doubleMu_2018b",
"doubleMu_2018c",
"doubleMu_2018d",
"eGamma_2018a",
"eGamma_2018b",
"eGamma_2018c",
"eGamma_2018d"
],
"2017":[
"dy",
"ttbar_2l",
"ww",
"wz",
"zz",
"doubleMu_2017b",
"doubleMu_2017c",
"doubleMu_2017d",
"doubleMu_2017e",
"doubleMu_2017f",
"doubleEG_2017b",
"doubleEG_2017c",
"doubleEG_2017d",
"doubleEG_2017e",
"doubleEG_2017f",
]
}

#datasetID is a map of MC information ID number for each sample name
datasetIDAllYears = {"2018":{
"dy":600000,
"ttbar_2l":600001,
"ww":600002,
"wz":600003,
"zz":600004,
"doubleMu_2018a":400000,
"doubleMu_2018b":400001,
"doubleMu_2018c":400002,
"doubleMu_2018d":400003,
"eGamma_2018a":400004,
"eGamma_2018b":400005,
"eGamma_2018c":400006,
"eGamma_2018d":400007
},
"2017":
{
"dy":600100,
"ttbar_2l":600101,
"ww":600102,
"wz":600103,
"zz":600104,
"doubleMu_2017b":400100,
"doubleMu_2017c":400101,
"doubleMu_2017d":400102,
"doubleMu_2017e":400103,
"doubleMu_2017f":400104,
"doubleEG_2017b":400105,
"doubleEG_2017c":400106,
"doubleEG_2017d":400107,
"doubleEG_2017e":400108,
"doubleEG_2017f":400109
}

}

#A map of a list of directories containing files to be associated by the sample name
datasetDirsAllYears = {"2018":{
"doubleMu_2018a":["/publicfs/cms/data/TopQuark/nanoAOD/2018/data/doubleMu_2018a/"],
"doubleMu_2018b":["/publicfs/cms/data/TopQuark/nanoAOD/2018/data/doubleMu_2018b/"],
"doubleMu_2018c":["/publicfs/cms/data/TopQuark/nanoAOD/2018/data/doubleMu_2018c/"],
"doubleMu_2018d":["/publicfs/cms/data/TopQuark/nanoAOD/2018/data/doubleMu_2018d/"],
"eGamma_2018a":["/publicfs/cms/data/TopQuark/nanoAOD/2018/data/eGamma_2018a/"],
"eGamma_2018b":["/publicfs/cms/data/TopQuark/nanoAOD/2018/data/eGamma_2018b/"],
"eGamma_2018c":["/publicfs/cms/data/TopQuark/nanoAOD/2018/data/eGamma_2018c/"],
"eGamma_2018d":["/publicfs/cms/data/TopQuark/nanoAOD/2018/data/eGamma_2018d/"],
"dy":["/publicfs/cms/data/TopQuark/nanoAOD/2018/mc/dy/","/publicfs/cms/data/TopQuark/nanoAOD/2018/mc/dy_ext/"],
"ww":["/publicfs/cms/data/TopQuark/nanoAOD/2018/mc/ww/"],
"wz":["/publicfs/cms/data/TopQuark/nanoAOD/2018/mc/wz/"],
"zz":["/publicfs/cms/data/TopQuark/nanoAOD/2018/mc/zz/"],
"ttbar_2l":["/publicfs/cms/data/TopQuark/nanoAOD/2018/mc/ttbar_2l/"]
},

"2017":{
"dy":["/publicfs/cms/data/TopQuark/nanoAOD/2017/mc/dy/","/publicfs/cms/data/TopQuark/nanoAOD/2017/mc/dy_ext/"],
"ww":["/publicfs/cms/data/TopQuark/nanoAOD/2017/mc/ww/"],
"wz":["/publicfs/cms/data/TopQuark/nanoAOD/2017/mc/wz/"],
"zz":["/publicfs/cms/data/TopQuark/nanoAOD/2017/mc/zz/"],
"ttbar_2l":["/publicfs/cms/data/TopQuark/nanoAOD/2017/mc/ttbar_2l/"],
"doubleMu_2017b":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleMu_2017b/"],
"doubleMu_2017c":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleMu_2017c/"],
"doubleMu_2017d":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleMu_2017d/"],
"doubleMu_2017e":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleMu_2017e/"],
"doubleMu_2017f":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleMu_2017f/"],
"doubleEG_2017b":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleEG_2017b/"],
"doubleEG_2017c":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleEG_2017c/"],
"doubleEG_2017d":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleEG_2017d/"],
"doubleEG_2017e":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleEG_2017e/"],
"doubleEG_2017f":["/publicfs/cms/data/TopQuark/nanoAOD/2017/data/doubleEG_2017f/"],

}

#"tW_antitop":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_antitop_NFHext1updatedEleJets/180508_154320/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_antitop_NFHext2updatedEleJets/180517_145622/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_antitop_NFHupdatedEleJets/180508_154202/0000/"],
#"tW_top":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_top_NFHext1updatedEleJets/180508_153925/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_top_NFHext2updatedEleJets/180508_154046/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJets/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_top_NFHupdatedEleJets/180508_153756/0000/"]
}

#The number of files to be put into each file list. A smaller number means each condor job will take less time, at the expense of more required jobs
namesPerFile = 1

for year in ["2018","2017"]:
    datasets = datasetsAllYears[year]
    datasetID = datasetIDAllYears[year]
    datasetDirs = datasetDirsAllYears[year]

    #the directory to write the lists to
    outputDirectory = "config/files/ul{0}/".format(year)
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
