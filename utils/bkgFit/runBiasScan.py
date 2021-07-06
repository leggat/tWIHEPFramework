import subprocess, os, sys, json



if __name__ == "__main__":
    barEnd = ["Barrel","Endcap"]
    regions = ["3j1t","2j1t","3j2t","4j1t","2j0t","3j0t","4j0t"] 
    scaleMap = {"0p0":0.,"0p3":0.3,"0p5":0.5,"0p7":0.7,"1p3":1.3,"1p5":1.5,"1p7":1.7,"2p0":2.0}
    orderedList = ["0p0","0p3","0p5","0p7","1p3","1p5","1p7","2p0"]

#    barEnd = ["Barrel"]
#    regions = ["3j1t"]
#    scaleMap = {"0p0":0.,"0p3":0.3}
#    orderedList = ["0p0","0p3"]

    histName = "16_HistogrammingMtW_mTW"
    dirPost = "iteration0"
    
    doFits=  True

    if not os.path.exists("logs/biasScans"): os.makedirs("logs/biasScans")

    #first let's do the fits 
    if doFits:
        for region in regions:
            for barEndInd in barEnd:
                print "Scanning", region,barEndInd
                subprocesses = []
                for scale in scaleMap.keys():
                    subprocesses.append(subprocess.Popen("python ../../framework/utils/bkgFit/fit.py plots/{0}{1}{2}/{3}_{4}.root > logs/biasScans/fit_{0}_{1}_{4}.txt".format(barEndInd,region,dirPost,histName,scale),shell=True))
                for i in range(len(subprocesses)):
                    subprocesses[i].wait()

    #then we'll read the scans
    sfMap = {}
    sfMapq = {}
    for region in regions:                    
        sfMap[region]  = {}
        sfMapq[region]  = {}
        for barEndInd in barEnd:              
            sfMap[region][barEndInd] = {}
            sfMapq[region][barEndInd] = {}
            for scale in scaleMap.keys():     
                inFile = open("logs/biasScans/fit_{0}_{1}_{2}.txt".format(barEndInd,region,scale),"r")
                for line in inFile:
                    if "wJet SF" in line:
                        sfMap[region][barEndInd][scale] = float(line.split("= ")[1])
                    if "QCD SF" in line:
                        sfMapq[region][barEndInd][scale] = float(line.split("= ")[1])
    
    #Now we'll make a nice table
    for region in regions:
        print region
        for barEndInd in barEnd:
            print barEndInd,
            for scale in orderedList: print "\t{0}*tW".format(scaleMap[scale]),
            print "\nWJet:",
            for scale in orderedList: 
                print "\t{0:.2f}".format(sfMap[region][barEndInd][scale]),
            print "\nQCD:",
            for scale in orderedList: 
                print "\t{0:.2f}".format(sfMapq[region][barEndInd][scale]),
            print
