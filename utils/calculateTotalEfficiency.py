import sys,os


def runOneSample(inSample):

    inputLogs = [f for f in os.listdir("tWEle/{0}/logs/".format(inSample)) if f.endswith(".log")]
    
    totalIn = 0.
    totalOut = 0.

    for inputLog in inputLogs:
        logFile = open("tWEle/{0}/logs/{1}".format(inSample,inputLog),"r")
        for line in logFile:
            if "CutListProcessor::PrintCutDetails> Total Efficienc" in line:
                totalIn += float(line.split("otal Efficiency: ")[1].split("/")[0])
                totalOut += float(line.split("/")[1].split(" = ")[0])
    print inSample
    print "In: {0:.1f} Out: {1:.1f} Effic: {2:.5f}".format(totalIn,totalOut,totalIn/totalOut)
                    

if __name__ == "__main__":

    inSamples = ["tW_top_nfh",
                 "tW_antitop_nfh"]
    for inSample in inSamples: runOneSample(inSample)

