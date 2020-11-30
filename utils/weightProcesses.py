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
                                                    "2j0t":2.98 * 0.8343, "3j0t":2.98 * 0.8938, "4j0t":2.98 * 0.8631
}
                self.perMCPerRegion["qcd"] = { "3j1t":3.8045, "2j1t":3.1248, "3j2t":4.2055, "4j1t":2.5204, "4j2t":1.6544, "all":4.1173,
                                               "2j0t":4.1554, "3j0t":4.0849, "4j0t":3.9319,
                                               }


            else: #muon channel
                #march doover mtw fit
                self.perMCPerRegion["wPlusJets"] = {"3j1t":2.98 * 0.8534, "2j1t":2.98 * 0.9164, "3j2t":2.98 * 1.9161, "4j1t":2.98 * 0.4310, "4j2t":2.98 * 1.9891, "all":2.98 * 0.8092}
                self.perMCPerRegion["qcd"] = {"3j1t":0.5928, "2j1t":0.6905, "3j2t":0.7108, "4j1t":0.4850, "4j2t":0.3872, "all":0.7374}


        self.ratioPlot = 0

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

    
    
