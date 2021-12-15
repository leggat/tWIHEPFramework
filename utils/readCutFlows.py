import os

from analysisComponentsNoRoot import AnalysisComponents

nEvents={"2017":{
"doubleMu_2017b":14501767,
"doubleMu_2017c":49636525,
"doubleMu_2017d":23075733,
"doubleMu_2017e":51531477,
"doubleMu_2017f":79756560,
"datamuon":218502062,
"dy_ext":100865609,
"ttbar_2l":106724000,
"dy":203729540,
"ww":15634000,
"wz":7889000,
"zz":2706000,
"doubleEG_2017b":58088760,
"doubleEG_2017c":65181125,
"doubleEG_2017d":25911432,
"doubleEG_2017e":56241190,
"doubleEG_2017f":74265012,
"dataelectron":229687519
},
"2016":{
"dy":104072004,
"ttbar_2l":43546000,
"ww":15821000,
"wz":7584000,
"zz":1151000,
"doubleMu_2016f":2429162,
"doubleMu_2016g":45235604,
"doubleMu_2016h":48912812,
"doubleEG_2016f":4360689,
"doubleEG_2016g":78797031,
"doubleEG_2016h":85388673,
"dataelectron":168546393,
"datamuon":96577578
},
"2016APV":{
"dy":95170542,
"ttbar_2l":37505000,
"ww":15859000,
"wz":7934000,
"zz":1282000,
"doubleMu_2016b_v1":4199947,
"doubleMu_2016b_v2":82535526,
"doubleMu_2016c":27934629,
"doubleMu_2016d":33861745,
"doubleMu_2016e":28246946,
"doubleMu_2016f_hipm":17900759,
"datamuon":194679552,
"doubleEG_2016b_v1":5686987,
"doubleEG_2016b_v2":143073268,
"doubleEG_2016c":47677856,
"doubleEG_2016d":53324960,
"doubleEG_2016e":49877710,
"doubleEG_2016f_hipm":30216940,
"dataelectron":347758480
},
"2018":{
"doubleMu_2018a":75491789,
"doubleMu_2018b":35057758,
"doubleMu_2018c":34565869,
"doubleMu_2018d":168172656,
"datamuon":313288072,
"eGamma_2018a":339013231,
"eGamma_2018b":153792795,
"eGamma_2018c":147827904,
"eGamma_2018d":752524583,
"dataelectron":1393158513,
"ttbar_2l":145020000,
"ww":14938000,
"wz":7883000,
"zz":3907000,
"dy":197649078,
"dy_ext":101415750

}
}

def readOutOneDirectory(dirName,samples,reRunFile=""):
    print "reading ",dirName

    totalCount = {}

    for sample in samples:
        totalCount[sample] = 0
        logFiles = [f for f in os.listdir("{0}/{1}/logs/".format(inDir,sample)) if f.endswith(".log")]
        print sample, "{0} logs".format(len(logFiles)),
        for inLog in logFiles:
            foundLog = False
            log = file("{0}/{1}/logs/{2}".format(inDir,sample,inLog),"r")
            for lin in log:
                if "| Pass JSON filter" in lin:
#                    print lin.split("|")
                    totalCount[sample] += int(lin.split("|")[2])
                    foundLog = True
            if not foundLog: 
                print "Didn't find anything in {0}".format(inLog)
                print "hep_sub {0}/{1}/{2}/scripts/{3}.sh -o {0}/{1}/{2}/logs/{3}.log -e {0}/{1}/{2}/logs/{3}.error".format(os.getcwd(),dirName,sample,inLog.split(".")[0])
                if reRunFile: print >>reRunFile, "hep_sub {0}/{1}/{2}/scripts/{3}.sh -o {0}/{1}/{2}/logs/{3}.log -e {0}/{1}/{2}/logs/{3}.error".format(os.getcwd(),dirName,sample,inLog.split(".")[0])

        print totalCount[sample]

    return totalCount


def writeAsMap(totalCounts):
    
    for year in totalCounts.keys():
        print "\"{0}\":{{".format(year)
        for lepton in totalCounts[year].keys():
            print " \"{0}\":{{".format(lepton)
            for sample in totalCounts[year][lepton].keys():
                if "data" in sample: 
                    print "  \"{0}\":{1:.3f},".format("data",float(totalCounts[year][lepton][sample])/float(nEvents[year][sample]))
                else:
                    print "  \"{0}\":{1:.3f},".format(sample,float(nEvents[year][sample])/float(totalCounts[year][lepton][sample]))
            print "   },"
        print " },"


if __name__ == "__main__":
    
    years = ["2018","2017","2016","2016APV"]
#    years = ["2016","2016APV"]
    leptons = ["muon","electron"]

    totalCounts = {}

    reRunFile = open("rerunFiles.sh","w")

    for year in years:
        totalCounts[year] = {}
        for lepton in leptons:
            totalCounts[year][lepton] = {}
            for dmc in ["","d"]:
                inDir = "met{0}{1}{2}".format(year[2:],"Ele" if lepton == "electron" else "","" if dmc == "" else "Data")
                print inDir
                if not os.path.exists(inDir): continue
        
                comp = AnalysisComponents("met",era=year,lepton=lepton)
                
                samples =  comp.samples
                if dmc: samples = comp.dataSamples

                tmpCounts = readOutOneDirectory(inDir,samples,reRunFile)
                
                if dmc == "":
                    totalCounts[year][lepton] = tmpCounts
                else:
                    totalData = 0
                    for key in tmpCounts.keys():
                        totalData+= tmpCounts[key]
                    totalCounts[year][lepton]["data"+lepton] = totalData
#
    writeAsMap(totalCounts)

    reRunFile.close()
