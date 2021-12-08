nEvents={"2017":{
"doubleMu_2017b":14501767,
"doubleMu_2017c":49636525,
"doubleMu_2017d":23075733,
"doubleMu_2017e":51531477,
"doubleMu_2017f":79756560,
"dy_ext":100865609,
"ttbar_2l":106724000,
"dy":102863931,
"ww":15634000,
"wz":7889000,
"zz":2706000,
"doubleEG_2017b":58088760,
"doubleEG_2017c":65181125,
"doubleEG_2017d":25911432,
"doubleEG_2017e":56241190,
"doubleEG_2017f":74265012
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
"doubleEG_2016b_v1":5686987,
"doubleEG_2016b_v2":143073268,
"doubleEG_2016c":47677856,
"doubleEG_2016d":53324960,
"doubleEG_2016e":49877710,
"doubleEG_2016f_hipm":30216940,
},
"2018":{
"doubleMu_2018a",75491789
"doubleMu_2018b":35057758,
"doubleMu_2018c":34565869,
"doubleMu_2018d":168172656,
"eGamma_2018a":339013231,
"eGamma_2018b":153792795,
"eGamma_2018c":147827904,
"eGamma_2018d":752524583,
"ttbar_2l":145020000,
"ww":14938000,
"wz":7883000,
"zz":3907000,
"dy":96233328,
"dy_ext":101415750

}
}

def readOutOneDirectory(dirName,samples):
    print "reading ",dirName

    for sample in samples:
        


if __name__ == "__main__":
    
    years = ["2018","2017","2016","2016APV"]
    leptons = ["muon","electron"]

    for year in years:
        for lepton in leptons:
            for dmc in ["","d"]:
                inDir = "met{0}{1}{2}".format(year,"Ele" if lepton == "electron" else "","" if dmc == "" else "Data")
                if not os.path.exists(inDir): continue
                
                comp = AnalysisComponents("met",era=year,lepton=lepton)
                
                samples =  comp.samples
                if dmc: samples = comp.dataSamples

                readOutOneDirectory(inDir,samples)
