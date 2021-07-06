import sys,os

samples=[
"wPlus0Jets",
"wPlus1Jets",
"wPlus2Jets",
"zPlusJetsHighMass",
"ttbar",
"ttbarBU"
]



if __name__ == "__main__":

    totalCount = {}
    totalWeight = {}

    for sample in samples:
        totalCount[sample] = 0.
        totalWeight[sample] = 0.
        logFiles = [f for f in os.listdir("tW/{0}/logs/".format(sample)) if f.endswith(".log")]
        for inLog in logFiles:
            log = file("tW/{0}/logs/{1}".format(sample,inLog),"r")
            for line in log:
                if "                 Jet.Number.All |" in line:
                    totalCount[sample]+=int(line.split("|")[4])
                    totalWeight[sample]+=float(line.split("|")[5])

        print sample,totalCount[sample],totalWeight[sample]
