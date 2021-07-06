import sys,os

def readLogs(inDir):
    
    files = [f for f in os.listdir(inDir) if "log" in f and not "#" in f]
    
    totalEvents = 0

    counter = 0

    for inFile in files:
        counter +=1
        sys.stdout.write("{0}/{1} \t - {2}\r".format(counter,len (files),totalEvents))
        sys.stdout.flush()

        logFile =  open(inDir+inFile,"r")
        for line in logFile:
            if "TOTAL EVENT COUNT IN TNT" in line:
                totalEvents += int(line.split(" = ")[1])
    
    print inDir, totalEvents

if __name__ == "__main__":
    
    inDirs = ["tWSysts/ttbar_hdampup/logs/","tWSysts/ttbar_hdampdown/logs/","tW/ttbar/logs/","tW/ttbarBU/logs/"]
    
    for direc in inDirs:
        readLogs(direc)
