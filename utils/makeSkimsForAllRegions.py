#Make the job folders for all of the regions we want to study for the tW lepton+jets

import subprocess,sys,os

baseDir = "/publicfs/cms/user/duncanleg/tW13TeV/framework/"

isElectron = False

elePost = ""
leptonList = [""]

if "ele" in sys.argv: isElectron = True
if isElectron: 
    elePost = " electron"
    leptonList = [" electron"]

for i in sys.argv:
    if "-makeLists" in i:
        makeListInDir = i.split("=")[-1]
        outDir = "inputLists/"
        subprocess.call("python {0}utils/createAbbrevLists.py {1} {2} {3}".format(baseDir,makeListInDir,outDir,elePost),shell=True)

#sys.exit(1)
makeSubmitFile = "utils/makeSkimSubmit.py"

postfix = ""

if "prefireVeto" in sys.argv:
    print "Adding in prefire veto"
    postfix += " prefireVeto"

if "qcdReject" in sys.argv:
    print "Adding in QCD reject cuts"
    postfix += " qcdReject"

if "qcdEnriched" in sys.argv:
    print "Adding in MET/mTW cuts"
    postfix += " qcdEnriched"

if "bdtSkim" in sys.argv:
    print "Doing a thing"
    makeSubmitFile = "utils/makeHEPSubmit.py"
    postfix += " skims bdtSkim fileListDir " + os.getcwd() + "/inputLists/"

if not os.path.exists("inputLists/"):
    print "No input list directory! Please re-run this command with argument -makeLists=<inputDirectory> !"
    exit(1)

#doBarrel = [""]
doBarrel = [" barrel"," endcap"]
if "splitBarrelEndcap" in sys.argv:
    doBarrel = [" barrel"," endcap"]

if "noSplitBarEnd" in sys.argv:
    doBarrel = [""] 

if "barrel" in sys.argv:
    doBarrel = [" barrel"]

if "endcap" in sys.argv:
    doBarrel = [" endcap"]


for i in [""," data"]:
#for i in [" data"]:
#    continue
#for i in [""]:
    for k in [""," inv"]:
#    for k in [""]:
#    for k in [" inv"]:
#        if k == " inv" and i == "":continue
        #    for k in [" inv"]:
        for j in [""]:
#            for l in [""," electron"]:
#            for l in [""]:
            for l in leptonList:
                for m in [""]:
#                    for n in [""," systs"]:
#                    for n in [" systs"]:
                    for n in [""]:
                        if n == " systs" and not m == "": continue
#                    for n in [" systs"]:
                        if i == " data" and not (m == "" and n == ""): continue
                        for o in doBarrel:
                            print makeSubmitFile+i+k+j+l+m+n+o+elePost+postfix
                            subprocess.call( "python "+baseDir+makeSubmitFile+i+k+j+l+m+n+o+elePost+postfix,shell=True)

#for i in ["jesUp","jesDown","jerUp","jerDown"]:
#    for j in [" electron"]:
#        subprocess.call( "python "+baseDir+"utils/makeHEPSubmit.py skims "+i+j,shell=True)

#for j in ["", " ttbarReg"," wJets2", " ttbar2"," sig2"]:
#    subprocess.call("python " +baseDir+"utils/makeHEPSubmit.py skims systs"+j,shell=True)
