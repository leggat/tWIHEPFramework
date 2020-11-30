#Make the job folders for all of the regions we want to study for the tW lepton+jets

import subprocess, sys

baseDir = "/publicfs/cms/user/duncanleg/tW13TeV/framework/"

if "-h" in sys.argv or "--help" in sys.argv:
    print "ele makes it electron channel"
    print "--onlyMCNom only runs MC nominal"
    print "--onlyData only runs on data"
    print "--onlyInvData only runs inverted data selection"
    print "--onlyOneFile - only make one submission file per dataset"
    sys.exit(0)

postfix = ""
if "--onlyOneFile" in sys.argv: postfix += " onlyOne"
mcData = [""," data"]
nomInv = [""," inv"]
systsList = [""," systs"]
if "--onlyMCNom" in sys.argv:
    mcData = [""]
    nomInv = [""]
    systsList = [""]
    
if "--onlyData" in sys.argv:
    mcData = [" data"]
    systsList = [""]

if "--onlyInvData" in sys.argv:
    mcData = [" data"]
    nomInv = [" inv"]

lepton = [""]
if "ele" in sys.argv:
    lepton = [" electron"]

for i in mcData:
    for k in nomInv:
        for l in lepton:
            for n in systsList:
                if n == " systs" and not k == "": continue
                if i == " data" and not n == "": continue
                print "python "+baseDir+"utils/makeHEPSubmit.py skims"+i+k+l+n+postfix
                subprocess.call( "python "+baseDir+"utils/makeHEPSubmit.py skims"+i+k+l+n+postfix,shell=True)


