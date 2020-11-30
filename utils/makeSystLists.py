#A short script that takes input from the user to create a set of file lists to run on condor. the inputs are:
# Number of files per job - how many files to put in each file list
# Name of dataset
# Total number of files in dataset
# Dataset's designated number according to the configuration file in configs

import sys,os

treeName = "OutTree_"
treeName = "OutTree_ttbar_"

datasets = ["tW_top","tW_antitop","ttbar","ww","wz","zz","zPlusJetsHighMass","zPlusJetsLowMass","sChan","qcd100_200","qcd200_300","qcd300_500","qcd500_700","qcd700_1000","qcd1000_1500","qcd1500_2000","qcd2000_inf","wPlusJetsMCatNLO","tChan_top","tChan_antitop","SingMuB","SingMuC","SingMuD","SingMuE","SingMuF","SingMuG"]
#datasets = ["SingMuB","SingMuC","SingMuD","SingMuE","SingMuF","SingMuG"]
#datasets = ["singleMuon"]
datasets = ["ttbar","ttbarBU"]
datasets = ["tW_top_nfh","tW_antitop_nfh","SingMuH"]
datasets = ["tW_mcanlo"]
datasetID = {"tW_top":500000,"tW_antitop":500001,"ttbar":500024,"ww":500007,"wz":500008,"zz":500009,"zPlusJetsHighMass":500004,"zPlusJetsLowMass":500005,"tChan":500002,"sChan":500010,"wPlusJets":500006,"qcd100_200":500011,"qcd200_300":500012,"qcd300_500":500013,"qcd500_700":500014,"qcd700_1000":500015,"qcd1000_1500":500016,"qcd1500_2000":500017,"qcd2000_inf":500018,"wPlusJetsMCatNLO":500019,"tChan_antitop":500020,"tChan_top":500021,"SingMuB":400000,"SingMuB_1":400007,"SingMuC":400001,"SingMuD":400002,"SingMuE":400003,"SingMuF":400004,"SingMuG":400005,"SingMuG_1":400008,"SingMuH":400006,"ttbarBU":500025,"tW_top_nfh":500026,"tW_antitop_nfh":500027,"tW_mcanlo":700034,
"tW_antitop_DS":700000,
"tW_antitop_isrup":700001,
"tW_antitop_isrdown":700002,
"tW_antitop_fsrup":700003,
"tW_antitop_fsrdown":700004,
"tW_antitop_herwig":700005,
"tW_antitop_MEup":700006,
"tW_antitop_MEdown":700007,
"tW_antitop_PSup":700008,
"tW_antitop_PSdown":700009,
"tW_top_DS":700010,
"tW_top_isrup":700011,
"tW_top_isrdown":700012,
"tW_top_fsrup":700013,
"tW_top_fsrdown":700014,
"tW_top_herwig":700015,
"tW_top_MEup":7000016,
"tW_top_MEdown":700017,
"tW_top_PSup":700018,
"tW_top_PSdown":700019,
"ttbar_isrup":700020,
"ttbar_isrdown":700021,
"ttbar_fsrup":700022,
"ttbar_fsrdown":700023,
"ttbar_tuneup":700024,
"ttbar_tunedown":700025,
"ttbar_herwig":700026,
"ttbar_amcatnlo":700027,
"ttbar_hdampup":700028,
"ttbar_hdampdown":700029
}
nFilesDataset = {"tW_top":14,"tW_antitop":17,"ttbar":983,"ww":11,"wz":30,"zz":12,"zPlusJetsHighMass":477,"zPlusJetsLowMass":237,"tChan":478,"sChan":21,"wPlusJets":422,"qcd100_200":738,"qcd200_300":179,"qcd300_500":199,"qcd500_700":210,"qcd700_1000":184,"qcd1000_1500":90,"qcd1500_2000":80,"qcd2000_inf":36,"singleMuon":1072,"wPlusJetsMCatNLO":300,"tChan_antitop":406,"tChan_top":723,"SingMuB":1756,"SingMuB_1":740,"SingMuC":580,"SingMuD":972,"SingMuE":826,"SingMuF":603,"SingMuG":1423,"SingMuG_1":320,"ttbarBU":1192}
datasetDirs = {"tW_top":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/crab_tW_topReTrig/170210_081934/0000/"],
"tW_antitop":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4/crab_tW_antitopReTrig/170210_082043/0000/"],
#"ttbar":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8-evtgen/crab_TTpowhegReTrig/170210_081831/0000/"],
"ttbar":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/crab_ttbarNewReTrig/170404_120840/0000/"],
"ttbarBU":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/crab_ttbarBackupNewReTrig/170404_121029/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/crab_ttbarBackupNewReTrig/170404_121029/0001/"],
"ww":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/WW_TuneCUETP8M1_13TeV-pythia8/crab_WWReTrig/170210_103335/0000/"],
"wz":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/WZ_TuneCUETP8M1_13TeV-pythia8/crab_WZReTrig/170210_083217/0000/"],
"zz":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ZZ_TuneCUETP8M1_13TeV-pythia8/crab_ZZReTrig/170210_083340/0000/"],
"zPlusJetsHighMass":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_ZJets_M-50ReTrig/170210_103203/0000/"],
"zPlusJetsLowMass":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_ZJets_M-10-50ReTrig/170210_082606/0000/"],
"tChan":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/crab_/0000/"],
"tChan_top":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/crab_tChanTReTrig/170210_082257/0000/"],
"tChan_antitop":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/crab_tChanTbarReTrig/170210_082143/0000/"],
"sChan":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/crab_sChanReTrig/170210_082418/0000/"],
"wPlusJets":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_WJetsaMCatNLOReTrig/170210_083000/0000/"],
"qcd100_200":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_qcd100_200ReTrig/170210_103443/0000/"],
"qcd200_300":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_qcd200_300ReTrig/170210_083759/0000/"],
"qcd300_500":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_qcd300_500ReTrig/170210_105633/0000/"],
"qcd500_700":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_qcd500_700ReTrig/170210_105746/0000/"],
"qcd700_1000":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_qcd700_1000ReTrig/170210_084005/0000/"],
"qcd1000_1500":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_qcd1000_1500ReTrig/170210_084106/0000/"],
"qcd1500_2000":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_qcd1500_2000ReTrig/170210_105902/0000/"],
"qcd2000_inf":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/crab_qcd2000_InfReTrig/170210_084346/0000/"],
"singleMuon":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_Full2202_SMu_16Dec2015S_JsonGold/160224_170442/0000/"],
"wPlusJetsMCatNLO":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/crab_WJetsaMCatNLOReTrig/170210_083000/0000/"],
"SingMuB":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuBReTrig/170210_083224/0000/","/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuBReTrig/170210_083224/0001/"],
"SingMuC":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuCReTrig/170210_083351/0000/"],
"SingMuD":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuDReTrig/170210_105844/0000/"],
"SingMuE":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuEReTrig/170210_102817/0000/"],
"SingMuF":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuFReTrig/170210_083759/0000/"],
"SingMuG":["/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuGReTrig/170210_084032/0000/","/acfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuGReTrig/170210_084032/0001/"],
"tW_top_nfh":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_top_NFHReTrig/170602_140456/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_top_NFHext1ReTrig/170602_140615/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_top_NFHext2ReTrig/170602_140730/0000/"],
"tW_antitop_nfh":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_antitop_NFHReTrig/170602_140845/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_antitop_NFHext1ReTrig/170602_141003/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/mc/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1/crab_tW_antitop_NFHext2ReTrig/170602_141119/0000/"],
"SingMuH":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuHReTrig/170210_084205/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuH2ReTrig/170425_131349/0000/","/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/moriond17/data/SingleMuon/crab_SingMuH2ReTrig/170425_131349/0001/"],
"tW_antitop_DS":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_5f_DS_NoFullyHadronicDecays_13TeV-powheg-pythia8/crab_tW_antitop_DS/170627_101651/0000/"],
"tW_top_DS":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_5f_DS_NoFullyHadronicDecays_13TeV-powheg-pythia8_TuneCUETP8M1/crab_tW_top_DS/170626_115820/0000/"],
"tW_antitop_fsrup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_5f_fsrup_NoFullyHadronicDecays_13TeV-powheg/crab_tW_antitop_fsrUp/170626_122153/0000/"],
"tW_antitop_fsrdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_fsrdown_5f_NoFullyHadronicDecays_13TeV-powheg/crab_tW_antitop_fsrDown/170626_122311/0000/"],
"tW_antitop_isrup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_5f_isrup_NoFullyHadronicDecays_13TeV-powheg/crab_tW_antitop_isrUp/170626_121912/0000/"],
"tW_antitop_isrdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_isrdown_5f_NoFullyHadronicDecays_13TeV-powheg/crab_tW_antitop_isrDown/170626_122032/0000"],
"tW_top_fsrup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_5f_fsrup_NoFullyHadronicDecays_13TeV-powheg/crab_tW_top_fsrUp/170626_120642/0000/"],
"tW_top_fsrdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_fsrdown_5f_NoFullyHadronicDecays_13TeV-powheg/crab_tW_top_fsrDown/170626_120754/0000/"],
"tW_top_isrup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_5f_isrup_NoFullyHadronicDecays_13TeV-powheg/crab_tW_top_isrUp/170626_120419/0000/"],
"tW_top_isrdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_isrdown_5f_NoFullyHadronicDecays_13TeV-powheg/crab_tW_top_isrDown/170626_120530/0000/"],
"ttbar_fsrup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TT_TuneCUETP8M2T4_13TeV-powheg-fsrup-pythia8/crab_tt_fsrup/170626_123237/0000/"],
"ttbar_fsrdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TT_TuneCUETP8M2T4_13TeV-powheg-fsrdown-pythia8/crab_tt_fsrdown/170626_123118/0000/"],
"ttbar_isrup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TT_TuneCUETP8M2T4_13TeV-powheg-isrup-pythia8/crab_tt_isrup/170627_102036/0000/"],
"ttbar_isrdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TT_TuneCUETP8M2T4_13TeV-powheg-isrdown-pythia8/crab_tt_isrdown/170626_122836/0000/"],
"tW_antitop_MEup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_5f_MEscaleup_NoFullyHadronicDecays_13TeV-powheg/crab_tW_antitop_MEUp/170626_122430/0000/"],
"tW_antitop_MEdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_MEscaledown_5f_NoFullyHadronicDecays_13TeV-powheg/crab_tW_antitop_MEDown/170626_122553/0000/"],
"tW_top_MEup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_5f_MEscaleup_NoFullyHadronicDecays_13TeV-powheg/crab_tW_top_MEUp/170626_120905/0000/"],
"tW_top_MEdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_MEscaledown_5f_NoFullyHadronicDecays_13TeV-powheg/crab_tW_top_MEDown/170626_121016/0000/"],
"tW_antitop_PSup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_5f_PSscaleup_NoFullyHadronicDecays_13TeV-powheg-pythia8/crab_tW_antitop_PSUp/170626_121632/0000/"],
"tW_antitop_PSdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_PSscaledown_5f_NoFullyHadronicDecays_13TeV-powheg-pythia8/crab_tW_antitop_PSDown/170626_121755/0000/"],
"tW_top_PSup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_5f_PSscaleup_NoFullyHadronicDecays_13TeV-powheg-pythia8/crab_tW_top_PSUp/170626_120154/0000/"],
"tW_top_PSdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_PSscaledown_5f_NoFullyHadronicDecays_13TeV-powheg-pythia8/crab_tW_top_PSDown/170626_120307/0000/"],
"ttbar_hdampup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TT_hdampUP_TuneCUETP8M2T4_13TeV-powheg-pythia8/crab_tt_hdampUp/170627_102150/0000/"],
"ttbar_hdampdown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TT_hdampDOWN_TuneCUETP8M2T4_13TeV-powheg-pythia8/crab_tt_hDampDown/170626_123356/0000/"],
"tW_antitop_herwig":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_antitop_5f_NoFullyHadronicDecays_13TeV-powheg_herwigpp/crab_tW_antitop_herwig/170627_101806/0000/"],
"tW_top_herwig":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_herwigpp/crab_tW_top_herwig/170627_101921/0000/"],
"ttbar_herwig":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TT_TuneEE5C_13TeV-powheg-herwigpp/crab_tt_herwig/170626_124038/0000/"],
"ttbar_amcatnlo":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/crab_ttbar_amcatnlo/170626_144520/0000/"],
"ttbar_tuneup":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TT_TuneCUETP8M2T4up_13TeV-powheg-pythia8/crab_tt_tuneUp/170626_123632/0000/"],
"ttbar_tunedown":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/systematicSamples/TT_TuneCUETP8M2T4down_13TeV-powheg-pythia8/crab_tt_tuneDown/170626_123757/0000/"],
"tW_mcanlo":["/publicfs/cms/data/TopQuark/cms13TeV/tWlJetSamples/mc/updatedEleJetsSysts/ST_tW_DR_lept_13TeV_MG5_aMCatNLO_pythia8/crab_tW_amcatnloupdatedEleJets/181213_142908/0000/"]

}

datasets = [
"tW_antitop_DS",
"tW_antitop_isrup",
"tW_antitop_isrdown",
"tW_antitop_fsrup",
"tW_antitop_fsrdown",
"tW_antitop_herwig",
"tW_antitop_MEup",
"tW_antitop_MEdown",
"tW_antitop_PSup",
"tW_antitop_PSdown",
"tW_top_DS",
"tW_top_isrup",
"tW_top_isrdown",
"tW_top_fsrup",
"tW_top_fsrdown",
"tW_top_herwig",
"tW_top_MEup",
"tW_top_MEdown",
"tW_top_PSup",
"tW_top_PSdown",
"ttbar_isrup",
"ttbar_isrdown",
"ttbar_fsrup",
"ttbar_fsrdown",
"ttbar_tuneup",
"ttbar_tunedown",
"ttbar_herwig",
"ttbar_amcatnlo",
"ttbar_hdampup",
"ttbar_hdampdown"
]

datasets = ["tW_mcanlo"]

import sys, math, mmap, subprocess

namesPerFile = 10 #float(raw_input("How many files to run over per job? "))
if namesPerFile == 0:
    print "You idiot, we can't divide by 0"
    sys.exit()
#outputDirectory = raw_input("Output directory: ")
outputDirectory = "config/files/systSamples/"

finishCopyScripts = raw_input("File for additional copies: ")
copyScript = 0
if finishCopyScripts:
    copyScript = file(finishCopyScripts,"w")
    print copyScript.write("#!/bin/bash\n")

for dataset in datasets:
    
    fileList = []
    for datasetDir in datasetDirs[dataset]:
        fileList += [os.path.join(datasetDir,f) for f in os.listdir(datasetDir) if ".root" in f]
    nJobs = int(math.ceil(len(fileList)/namesPerFile))
    print ("Dataset: {0}, ID: {1}. Number of jobs created = {2}".format(dataset,datasetID[dataset],nJobs))
    nFile = 0
    for i in range(nJobs):
        currentFile = open(outputDirectory + dataset + str(i) + ".list","w")
        currentFile.write("Name: " + dataset)
        currentFile.write("\nNumber: " + str(datasetID[dataset]) + "_1\n")
        for j in range(namesPerFile):
            if nFile >= len(fileList): continue
            currentFile.write(fileList[nFile]+"\n")
            nFile+=1
        currentFile.close()
        
    continue

    nJobs = int(math.ceil(nFilesDataset[dataset]/namesPerFile))
    print ("Dataset: {0}, ID: {1}. Number of jobs created = {2}".format(dataset,datasetID[dataset],nJobs))
#    if os.path.exists("config/files/full80X/"+dataset+".list"): subprocess.call("rm config/files/full80X/"+dataset+".list",shell=True)
#    for dirName in datasetDirs[dataset]:
#        subprocess.call("\"ls\" "+dirName + "* >> config/files/full80X/"+dataset+".list",shell=True)
#    bigDatasetFile = open("config/files/full80X/"+dataset+".list")
#    s = mmap.mmap(bigDatasetFile.fileno(), 0, access=mmap.ACCESS_READ)
    for i in range(nJobs):
        currentFile = open(outputDirectory + dataset + str(i) + ".list","w")
        currentFile.write("Name: " + dataset)
        currentFile.write("\nNumber: " + str(datasetID[dataset]) + "_1\n")
        maxFiles = (i+1)*namesPerFile+1
        if nFilesDataset[dataset]+1 < maxFiles: maxFiles = nFilesDataset[dataset]+1
        for j in range(i*namesPerFile+1,maxFiles):
            line = treeName + str(j) + ".root"
            fileLocation = ""
            fileSize = 0.
            for dataDir in datasetDirs[dataset]:
                if os.path.exists(dataDir+treeName+str(j)+".root"):
                    statinfo = os.stat(dataDir+treeName+str(j)+".root")
                    if statinfo.st_size > fileSize:
                        fileLocation = dataDir+treeName+str(j)+".root"
                        fileSize = statinfo.st_size
            if not fileLocation == "" and fileSize > 0:
                currentFile.write(fileLocation+"\n")
            elif copyScript: 
                fileNameForCopy = dataDir.split("tWlJetSamples/moriond17/")[-1]
                if "mc/" in fileNameForCopy: fileNameForCopy = "mcMoriond17/" + fileNameForCopy.split("mc/")[-1]
                else: fileNameForCopy = "reRecoData/"+fileNameForCopy.split("data/")[-1]
                copyScript.write("srmcp --debug srm://srm.ihep.ac.cn/pnfs/ihep.ac.cn/data/cms/store/user/leggat/tWSamples/"+ fileNameForCopy + treeName+str(j) +".root file:///" + dataDir+"\n")
                print "Couldn't find file " + str(j)
#            if s.find(line) != -1:
#                currentFile.write(datasetDirs[dataset] + line + "")
#                print line
        

while False:
    datasetName = raw_input("Dataset name: ")
    if datasetName == "end" or datasetName == "stop": break
    datasetNum = raw_input("Corresponds to the number: ")
    totalDatasetFiles = float(raw_input("Total number of files in this dataset: "))
    nJobs = int(math.ceil(totalDatasetFiles/namesPerFile))
    print "That means we will make " + str(nJobs) + " jobs"
    remoteDir = raw_input("Remote directory: ")
    bigDatasetFile = open("config/files/full/"+datasetName+".list")
    s = mmap.mmap(bigDatasetFile.fileno(), 0, access=mmap.ACCESS_READ)
    for i in range(nJobs):
        currentFile = open(outputDirectory + datasetName + str(i) + ".list","w")
        currentFile.write("Name: "+datasetName)
        currentFile.write("\nNumber: " + datasetNum + "_1\n")
        maxFiles = (i+1)*namesPerFile+1
        if totalDatasetFiles+1 < maxFiles: maxFiles = totalDatasetFiles+1
        for j in range(i*namesPerFile+1,maxFiles):
            line = treeName + str(j) + ".root"
            if s.find(line) != -1:
                currentFile.write(remoteDir + line + "\n")
#                print line
#            currentFile.write(remoteDir + "
        
        
    
print "Thank you for using the create jobs program. Have a nice day"
