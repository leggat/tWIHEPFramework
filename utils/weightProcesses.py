import json

#A utility package so that I don't have to go through updating the normalisation factors in each file that requires it
class ReweightObject:
    "Contains the data we want to reweight etc"
#    perMCPerRegion = {}
    def __init__(self,prefit,electron,reweightRatio = 0.):
        self.isPrefit = prefit
        self.perMCPerRegion = {}
        self.perRegionOverall = {}
        if prefit:
#            self.perRegionOverall = {"2j0t":0.874787665808,"3j0t":0.910137808664,"4j0t":0.936179117514}
            self.perMCPerRegion["ttbar"] = {"3j1t":1.,"2j1t":1.,"3j2t":1.,"4j1t":1.,"4j2t":1.,"all":1.}
            self.perMCPerRegion["wPlusJets"] = {"3j1t":2.98,"2j1t":2.98,"3j2t":2.98,"4j1t":2.98,"4j2t":2.98,"all":1.}
            self.perMCPerRegion["qcd"] = {"3j1t":1.,"2j1t":1.,"3j2t":1.,"4j1t":1.,"4j2t":1.,"all":1.}
#            self.perMCPerRegion["qcd"] = {"3j1t":0.08,"2j1t":0.08,"3j2t":0.08,"4j1t":0.08,"4j2t":0.08}
        else:
            if electron:

                #met fit
                self.perMCPerRegion["wPlusJets"] = {"3j1t":2.98 * 0.4009, "2j1t":2.98 * 0.8744, "3j2t":2.98 * 0.1620, "4j1t":2.98 * 0.1000, "4j2t":2.98 * 0.1000, "all":2.98 * 0.8398,
                                                    "2j0t":2.98 * 0.8343, "3j0t":2.98 * 0.8938 , "4j0t":2.98 * 0.8631,
                                                    "3j1tBarrel":2.98 * 0.5303, "2j1tBarrel":2.98 * 0.9825, "3j2tBarrel":2.98 * 0.5000, "4j1tBarrel":2.98 * 0.5000, "4j2tBarrel":2.98 * 1.5000,
                                                    #"3j1tEndcap":2.98 * 1.2354, "2j1tEndcap":2.98 * 1.1621, "3j2tEndcap":2.98 * 1.5000, "4j1tEndcap":2.98 * 1.4310, "4j2tEndcap":2.98 * 1.2194,
                                                    #noNorm met fit
                                                    "3j1tEndcap":2.98 * 0.5238, "2j1tEndcap":2.98 * 1.4335, "3j2tEndcap":2.98 * 0.7948, "4j1tEndcap":2.98 * 0.5000, "4j2tEndcap":2.98 * 0.5000,
                                 #                   "3j1tEndcap":2.98 * 0.5000, "2j1tEndcap":2.98 * 0.6213, "3j2tEndcap":2.98 * 0.5000, "4j1tEndcap":2.98 * 0.5000, "4j2tEndcap":2.98 * 0.5000,
                                                    #"3j1tEndcap":2.98 * 0.9760, "2j1tEndcap":2.98 * 0.5326, "3j2tEndcap":2.98 * 0.6692, "4j1tEndcap":2.98 * 1.0956, "4j2tEndcap":2.98 * 0.5000,
#                                                    "3j1tEndcap":2.98 * 1.5, "2j1tEndcap":2.98 * 0.6213, "3j2tEndcap":2.98 * 0.5000, "4j1tEndcap":2.98 * 0.5000, "4j2tEndcap":2.98 * 0.5000,

}
                self.perMCPerRegion["qcd"] = { "3j1t":3.8045, "2j1t":3.1248, "3j2t":4.2055, "4j1t":2.5204, "4j2t":1.6544, "all":4.1173,
                                               "2j0t":4.1554, "3j0t":4.0849 * (588290.575285/534727.117323), "4j0t":3.9319 * (123982.757078/106309.879236),
                                               "3j1tBarrel":3.4071, "2j1tBarrel":2.4009, "3j2tBarrel":4.0965, "4j1tBarrel":0.8029, "4j2tBarrel":0.0000,
                                               #no norm met fit
                                               "3j1tEndcap":1.4388, "2j1tEndcap":0.9339, "3j2tEndcap":1.7288, "4j1tEndcap":2.1025, "4j2tEndcap":2.9220,
#                                               "3j1tEndcap":1.0246, "2j1tEndcap":1.0172, "3j2tEndcap":1.2085, "4j1tEndcap":0.9502, "4j2tEndcap":1.2719,
                                               #"3j1tEndcap":2.9083, "2j1tEndcap":4.1523, "3j2tEndcap":2.4802, "4j1tEndcap":1.8451, "4j2tEndcap":0.9721,
                                               #"3j1tEndcap":3.7603, "2j1tEndcap":3.9382, "3j2tEndcap":3.2819, "4j1tEndcap":3.0239, "4j2tEndcap":1.5286,
                                               #"3j1tEndcap":2.05, "2j1tEndcap":3.9382, "3j2tEndcap":3.2819, "4j1tEndcap":3.0239, "4j2tEndcap":1.5286,


                                               }


            else: #muon channel
                #march doover mtw fit
                self.perMCPerRegion["wPlusJets"] = {"3j1t":2.98 * 0.8534, "2j1t":2.98 * 0.9164, "3j2t":2.98 * 1.9161, "4j1t":2.98 * 0.4310, "4j2t":2.98 * 1.9891, "all":2.98 * 0.8092,
                                                    "2j0t":2.98 * 0.9311 * (5378451.2214/6418566.72377), "3j0t":2.98 * 1.0774 * (1022695.75782/1294871.85892), "4j0t":2.98 * 1.1629 * (185035.749706/237631.401553),
                                                    "3j1tBarrel":2.98 * 1.0164, "2j1tBarrel":2.98 * 1.0803, "3j2tBarrel":2.98 * 1.5000, "4j1tBarrel":2.98 * 0.5000, "4j2tBarrel":2.98 * 1.5000,
                                                    "3j1tEndcap":2.98 * 1.1252, "2j1tEndcap":2.98 * 1.0884, "3j2tEndcap":2.98 * 1.5000, "4j1tEndcap":2.98 * 0.8928, "4j2tEndcap":2.98 * 1.5000,

                                                    }
                self.perMCPerRegion["qcd"] = {"3j1t":0.5928, "2j1t":0.6905, "3j2t":0.7108, "4j1t":0.4850, "4j2t":0.3872, "all":0.7374,
                                              "2j0t":0.7812 * (1417628.92359/1228625.76236), "3j0t":0.6662 * (242547.159966/218722.733143), "4j0t":0.5727 * (41200.830614/37935.9971849),
                                              "3j1tBarrel":0.4835, "2j1tBarrel":0.5452, "3j2tBarrel":0.8978, "4j1tBarrel":0.3220, "4j2tBarrel":0.5761,
                                              "3j1tEndcap":0.8086, "2j1tEndcap":0.9684, "3j2tEndcap":1.1021, "4j1tEndcap":0.6848, "4j2tEndcap":0.7739,

                                              }

                #muon met fit
#                self.perMCPerRegion["qcd"] = 
#                "3j1t":0.6915, "2j1t":0.6143, "3j2t":1.1365, "4j1t":0.6392, "4j2t":1.0281, "2j0t":0.6717, "3j0t":0.7404, "4j0t":0.8422

        self.ratioPlot = 0

    def overrideWeightsFromJSON(self,inJSONFile):
        self.perMCPerRegion = {}
        data = ""
        with open(inJSONFile,"read") as read_file:
            data = json.load(read_file)
        for key in data:                                                                          
            self.perMCPerRegion[key] = {}                                                              
            for key1 in data[key]:                                                                
                for key2 in data[key][key1]:                                                      
                    self.perMCPerRegion[key]["{0}{1}".format(key2,key1)] = float(data[key][key1][key2])
                    #    with open("outTest.json","w") as outFile:                                                
#        json.dump(jsonData,outFile)                                                          

            

    def setRatioPlot(ratioPlotsName,region):
        inFile = TFile(ratioPlotsName,"READ")
        self.ratioPlot = inFile.Get("ratio{0}".format(region))
        self.ratioPlot.SetDirectory(0)
        inFile.Close()

    def getReweight(leptonPt):
        binNumber = self.ratioPlot.GetXaxis().GetBin(leptonPt)
        return self.ratioPlot.GetBinContent(binNumber)

    def getDatasetWeight(self,process,region):
        weight = 1.
        if region in self.perRegionOverall.keys(): weight = self.perRegionOverall[region]
        if not process in self.perMCPerRegion.keys() : return weight
        if not region in self.perMCPerRegion[process].keys() :
            if process == "wPlusJets": return 2.98 * weight
            else: return 1.
        return self.perMCPerRegion[process][region] * weight

    
    
