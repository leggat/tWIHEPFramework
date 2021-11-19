#A script that will sum up the total of cut flows for each directory in the indicated folder

import sys,os

inDir = sys.argv[1]

listOfDirs = os.listdir(inDir)

totalEvents = {}

for subdir in listOfDirs:
    print "{0}/{1}/logs/".format(inDir,subdir)
    scaleFactor = 1.
    if "ttbar" in subdir: scaleFactor = 0.891728577865
    logFiles = [f for f in os.listdir("{0}/{1}/logs/".format(inDir,subdir)) if "log" in f and "#" not in f]
    totalEvents[subdir] = 0.
    for logName in logFiles:
        logFile = open("{0}/{1}/logs/{2}".format(inDir,subdir,logName),"r")
        for line in logFile:
            if not len(line.split("|")) == 7: continue
            if not "Tagged.Jet.Number.All" in line: continue
            totalEvents[subdir] += float(line.split("|")[5]) * scaleFactor

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
for sample in listOfDirs:
    print sample,totalEvents[sample]
