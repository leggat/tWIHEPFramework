#BA storage element for all the universal things in the analysis, i.e. MCs, regions, all that stuff.

#from ROOT import *
#import ROOT

class AnalysisComponents:
    extendedRegions = []
    samples = []
    histoGramPerSample = {}
    histoColours = {}
    histoGramOrder = []
    histNameInLegend = {}
    jetShiftDict = {}
    def __init__(self):
        self.regions = ["3j1t","3j2t","2j1t","4j1t","4j2t"]
        self.extendedRegions = ["3j1t","3j2t","2j1t","4j1t","4j2t","2j0t","3j0t","4j0t"]
        self.sample = ["qcd1000_1500","qcd100_200","qcd1500_2000","qcd2000_inf","qcd200_300","qcd300_500","qcd500_700","qcd700_1000","sChan","tChan_top","tChan_antitop","ttbar","ttbarBU","ww","wz","zz","zPlusJetsLowMass","zPlusJetsHighMass","tW_top_nfh","tW_antitop_nfh","wPlus0Jets","wPlus1Jets","wPlus2Jets"]
        self.histoGramPerSample = {"tW_top":"tW","tW_antitop":"tW","sChan":"singleTop","tChan":"singleTop","zz":"VV","zPlusJetsLowMass":"zPlusJets","zPlusJetsHighMass":"zPlusJets","wz":"VV","ww":"VV","wPlusJets":"wPlusJets","ttbar":"ttbar","qcd700_1000":"qcd","qcd500_700":"qcd","qcd300_500":"qcd","qcd200_300":"qcd","qcd2000_inf":"qcd","qcd1500_2000":"qcd","qcd100_200":"qcd","qcd1000_1500":"qcd","wPlusJetsMCatNLO":"wPlusJets","tChan_top":"singleTop","tChan_antitop":"singleTop","ttbarBU":"ttbar","tW_top_nfh":"tW","tW_antitop_nfh":"tW","wPlusJetsMadgraph":"wPlusJets","wPlus0Jets":"wPlusJets","wPlus1Jets":"wPlusJets","wPlus2Jets":"wPlusJets","SingMuB":"muData","SingMuC":"muData","SingMuD":"muData","SingMuE":"muData","SingMuF":"muData","SingMuG":"muData", "SingMuH":"muData","SingEleB":"eleData","SingEleC":"eleData","SingEleD":"eleData","SingEleE":"eleData","SingEleF":"eleData","SingEleG":"eleData", "SingEleH":"eleData","tW_mcanlo":"tW"}
        self.histoGramOrder = ["singleTop","VV","ttbar","wPlusJets","zPlusJets","qcd","tW"]
        self.histNameInLegend = {
            "singleTop":"Single top","VV":"VV","ttbar":"t#bar{t}","wPlusJets":"W+Jets","zPlusJets":"Z+jets","qcd":"QCD","tW":"tW"
            }
        self.jetShiftDict = {"jetShift0" : "JES_AbsoluteStat","jetShift1" : "JES_AbsoluteScale","jetShift2" : "JES_AbsoluteMPFBias","jetShift3" : "JES_Fragmentation","jetShift4" : "JES_SinglePionECAL","jetShift5" : "JES_SinglePionHCAL","jetShift6" : "JES_FlavorQCD","jetShift7" : "JES_TimePtEta","jetShift8" : "JES_RelativeJEREC1","jetShift9" : "JES_RelativeJEREC2","jetShift10" : "JES_RelativeJERHF","jetShift11" : "JES_RelativePtBB","jetShift12" : "JES_RelativePtEC1","jetShift13" : "JES_RelativePtEC2","jetShift14" : "JES_RelativePtHF","jetShift15" : "JES_RelativeBal","jetShift16" : "JES_RelativeFSR","jetShift17" : "JES_RelativeStatEC","jetShift18" : "JES_RelativeStatHF","jetShift19" : "JES_PileUpPtRef","jetShift20" :  "JES_PileUpPtBB","jetShift21" : "JES_PileUpPtEC1","jetShift22" : "JES_PileUpPtEC2","jetShift23" : "JES_PileUpPtHF","jetShift24" : "JER_stat","jetShift25" : "JER_syst","jetShift26" : "ptmiss_uncert"
                             }
        
