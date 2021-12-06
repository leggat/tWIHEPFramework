import subprocess

years = ["2018","2017"]
#years = ["2018"]                                                                                                                                                                                                                                                                                                             
lepton = ["", " -e"]
#lepton = [""]                                                                                                                                                                                                                                                                                                                
data = [""," -d"]

for dataMC in data:
    print "Data" if dataMC == " -d" else "MC"
    for year in years:
        print year
        for lep in lepton:
            print "ele" if lep==" -e" else "mu"
            config = "config/overall/met_{0}_di{1}.config".format(year,"ele" if lep==" -e" else "mu")
            executable = "bin/metPaper/metPlots{0}.x".format("" if lep == "" else "_ele")
            print "python createCondorJobs.py --hists {0} -y {1} {2} --configFile {3} -x {4}{5}".format(dataMC,year,lep,config,executable," -p -l" if not dataMC == "" else "")
            subprocess.call("python ../../tWIHEPFramework/utils/createCondorJobs.py --hists {0} -y {1} {2} --configFile {3} -x {4}{5}".format(dataMC,year,lep,config,executable," -p -l" if not dataMC == "" else ""),shell=True)



#python createCondorJobs.py -h                                                                                                                                                                                                 
