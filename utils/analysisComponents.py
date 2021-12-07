#BA storage element for all the universal things in the analysis, i.e. MCs, regions, all that stuff.

from ROOT import *
#import ROOT

class AnalysisComponents:
    dirName = "" # The name of the analysis in the condor directories
    extendedRegions = []
    samples = []
    histoGramPerSample = {}
    histoColours = {}
    histoGramOrder = []
    histNameInLegend = {}
    jetShiftDict = {}
    dataSamples = []
    def __init__(self,analysis="tW",era="2018",lepton="muon"):
        #defaults for if we are doing the tW
        if analysis=="tW":
            self.dirName = "tW"
            self.regions = ["3j1t","3j2t","2j1t","4j1t","4j2t"]
            self.extendedRegions = ["3j1t","3j2t","2j1t","4j1t","4j2t","2j0t","3j0t","4j0t"]
            self.sample = ["qcd1000_1500","qcd100_200","qcd1500_2000","qcd2000_inf","qcd200_300","qcd300_500","qcd500_700","qcd700_1000","sChan","tChan_top","tChan_antitop","ttbar","ttbarBU","ww","wz","zz","zPlusJetsLowMass","zPlusJetsHighMass","tW_top_nfh","tW_antitop_nfh","wPlus0Jets","wPlus1Jets","wPlus2Jets"]
            self.histoGramPerSample = {"tW_top":"tW","tW_antitop":"tW","sChan":"singleTop","tChan":"singleTop","zz":"VV","zPlusJetsLowMass":"zPlusJets","zPlusJetsHighMass":"zPlusJets","wz":"VV","ww":"VV","wPlusJets":"wPlusJets","ttbar":"ttbar","qcd700_1000":"qcd","qcd500_700":"qcd","qcd300_500":"qcd","qcd200_300":"qcd","qcd2000_inf":"qcd","qcd1500_2000":"qcd","qcd100_200":"qcd","qcd1000_1500":"qcd","wPlusJetsMCatNLO":"wPlusJets","tChan_top":"singleTop","tChan_antitop":"singleTop","ttbarBU":"ttbar","tW_top_nfh":"tW","tW_antitop_nfh":"tW","wPlusJetsMadgraph":"wPlusJets","wPlus0Jets":"wPlusJets","wPlus1Jets":"wPlusJets","wPlus2Jets":"wPlusJets"}
            self.histoColours = {"tW_top":ROOT.kGreen+2,"tW_antitop":ROOT.kGreen+2,"tW_top_nfh":ROOT.kGreen+2,"tW_antitop_nfh":ROOT.kGreen+2,"tChan":ROOT.kYellow,"zPlusJetsLowMass":ROOT.kBlue,"zPlusJetsHighMass":ROOT.kBlue,"wz":ROOT.kPink,"ww":ROOT.kPink,"zz":ROOT.kPink,"wPlusJets":ROOT.kTeal,"ttbar":ROOT.kRed,"qcd700_1000":ROOT.kGray,"qcd500_700":ROOT.kGray,"qcd300_500":ROOT.kGray,"qcd200_300":ROOT.kGray,"qcd2000_inf":ROOT.kGray,"qcd1500_2000":ROOT.kGray,"qcd100_200":ROOT.kGray,"qcd1000_1500":ROOT.kGray,"sChan":ROOT.kOrange,"VV":ROOT.kPink,"qcd":ROOT.kGray,"tW":ROOT.kGreen+2,"zPlusJets":ROOT.kBlue,"singleTop":ROOT.kYellow}
            self.histoGramOrder = ["singleTop","VV","ttbar","wPlusJets","zPlusJets","qcd","tW"]
            self.histNameInLegend = {
                "singleTop":"Single top","VV":"VV","ttbar":"t#bar{t}","wPlusJets":"W+Jets","zPlusJets":"Z+jets","qcd":"QCD","tW":"tW"
            }
            self.jetShiftDict = {"jetShift0" : "JES_AbsoluteStat","jetShift1" : "JES_AbsoluteScale","jetShift2" : "JES_AbsoluteMPFBias","jetShift3" : "JES_Fragmentation","jetShift4" : "JES_SinglePionECAL","jetShift5" : "JES_SinglePionHCAL","jetShift6" : "JES_FlavorQCD","jetShift7" : "JES_TimePtEta","jetShift8" : "JES_RelativeJEREC1","jetShift9" : "JES_RelativeJEREC2","jetShift10" : "JES_RelativeJERHF","jetShift11" : "JES_RelativePtBB","jetShift12" : "JES_RelativePtEC1","jetShift13" : "JES_RelativePtEC2","jetShift14" : "JES_RelativePtHF","jetShift15" : "JES_RelativeBal","jetShift16" : "JES_RelativeFSR","jetShift17" : "JES_RelativeStatEC","jetShift18" : "JES_RelativeStatHF","jetShift19" : "JES_PileUpPtRef","jetShift20" :  "JES_PileUpPtBB","jetShift21" : "JES_PileUpPtEC1","jetShift22" : "JES_PileUpPtEC2","jetShift23" : "JES_PileUpPtHF","jetShift24" : "JER_stat","jetShift25" : "JER_syst","jetShift26" : "ptmiss_uncert"
                             }
        if analysis=="met":
            self.dirName = "met"
            self.sample = ["dy","ttbar_2l","ww","wz","zz"]
            self.samples = self.sample
            self.histoColours = {"dy":ROOT.kBlue,"ttbar":ROOT.kRed,"VV":"kYellow"}
            self.histoGramPerSample = {"dy":"dy","ttbar_2l":"ttbar","ww":"VV","wz":"VV","zz":"VV"}
            self.histoGramOrder = ["dy","VV","ttbar"]
            self.histNameInLegend = {"dy":"DY","ttbar":"t#bar{t}","VV":"VV"}
            if era == "2018":
                if lepton == "muon":
                    self.dataSamples = ["doubleMu_2018a",
                                        "doubleMu_2018b",
                                        "doubleMu_2018c",
                                        "doubleMu_2018d"
                                    ]
                if lepton == "electron":
                    self.dataSamples = ["eGamma_2018a",
                                        "eGamma_2018b",
                                        "eGamma_2018c",
                                        "eGamma_2018d"
                                    ]
            if era == "2017":
                if lepton == "muon":
                    self.dataSamples = ["doubleMu_2017b",
                                        "doubleMu_2017c",
                                        "doubleMu_2017d",
                                        "doubleMu_2017e",
                                        "doubleMu_2017f"
                                    ]
                if lepton == "electron":
                    self.dataSamples = ["doubleEG_2017b",
                                        "doubleEG_2017c",
                                        "doubleEG_2017d",
                                        "doubleEG_2017e",
                                        "doubleEG_2017f"
                                        ]


                
        #Generic ones
        self.xAxisLabels = {
            "lightJet1CSV":"CSV_{light jet 1}",
            "M_DeltaRBJetLepton":"#DeltaR_{b jet,lepton}",
            "M_DeltaRLeptonJet1":"#DeltaR_{lepton,jet_{1}}",
            "M_DeltaRlightjets":"#DeltaR_{light jets}",
            "M_hadronicWmass":"m_{W_{hadronic}}",
            "M_Mass_Jet1Jet2Jet3LeptonMET":"m_{j_{1,2,3},lepton,MET}",
            "M_nJet2040":"N_{Jets:20<p_{T}<40}",
            "M_Pt_AllJetsLeptonMET":"p_{T}_{j_{all},lepton,MET}",
            "M_Pt_Lepton":"p_{T,lepton}",
            "M_topMass2_lep":"m_{top_{leptonic}}",
            "M_DeltaPhiJet1Jet2":"#Delta#Phi_{j_{1,2}}",
            "M_DeltaRJet1Jet2":"#DeltaR_{j_{1,2}}",
            "M_DeltaRWlvJet2":"#DeltaR_{W_{lv}j_{2}}",
            "M_E_AllJets":"E_{j_{all}}",
            "M_E_Jet1Jet2Jet3MET":"E_{j_{1,2,3}MET}",
            "M_E_Jet2Jet3":"E_{j_{2,3}}",
            "M_HT":"H_{T}",
            "M_Mass_AllJets":"m_{j_{all}}",
            "M_Mass_AllJetsMET":"m_{j_{all}MET}",
            "M_Mass_Jet1Jet2":"m_{j_{1,2}}",
            "M_Mass_Jet1Jet3":"m_{j_{1,3}}",
            "M_Pt_AllJets2040":"p_{T,j_{20-40}}",
            "M_Pt_Jet1LeptonMET":"p_{T,j_{1}lMET}",
            "M_Pt_Jet1Jet2Lepton":"p_{T,j_{1,2}l}",
            "M_Pt_LeptonJet1":"p_{T,l,j_{1}}",
            "M_Pt_Jet1_2030":"p_{T,j_{1,20-30}}",
        }

        
