#define mvaTool_cxx
#include "mvaTool.h"

mvaTool::mvaTool(Int_t channel, Bool_t useIterFit, Bool_t isEle){
  
  _useIterFit = useIterFit;

  _isEle = isEle;

  _channel = channel;

  //old setup
  _nTrees = 400;
  _depth = 2;
  _nCuts = 22;

  _nTrees = 300;
  _depth=4;
  _nCuts=13;
  
  _emptyVarValue = -999.;
  
  TString bdtType = "grad";

  /*  if (_isEle){
    _nCuts = 16;
    _depth=3;
    }*/

  //So that it's easier to not split barrel and endcap if I change my mind about it
  _splitBarrelEndcap = true;

  if (_splitBarrelEndcap){
    regionNames = {"3j1tBarrel","3j2tBarrel","2j1tBarrel","4j1tBarrel","4j2tBarrel","2j0tBarrel","3j0tBarrel","4j0tBarrel",
		   "3j1tEndcap","3j2tEndcap","2j1tEndcap","4j1tEndcap","4j2tEndcap","2j0tEndcap","3j0tEndcap","4j0tEndcap"};
  }
  else {
    regionNames = {"3j1t","3j2t","2j1t","4j1t","4j2t","2j0t","3j0t","4j0t"};
  }
  //regionNames = {"3j1t"};

  for (unsigned int reg = 0; reg < regionNames.size(); reg++){
    if (false){
      //    if (reg == 1){
      if (_isEle) _bdtName.push_back("grad_nTrees200_depth2_nCuts7_channel1");
      else _bdtName.push_back("grad_nTrees200_depth2_nCuts11_channel1");
    }
    //    else if (reg == 2){
    else if (false){
      if (_isEle) _bdtName.push_back("grad_nTrees400_depth2_nCuts19_channel2");
      else _bdtName.push_back("grad_nTrees400_depth2_nCuts19_channel2");
    }
    else _bdtName.push_back(bdtType+"_nTrees"+std::to_string(_nTrees)+"_depth"+std::to_string(_depth)+"_nCuts"+std::to_string(_nCuts));

  }
  //Start by initialising the list of variables we will base the MVA on
  initialiseVarList();
  baseName = "";
}

void mvaTool::initialiseVarList(){

  for (unsigned int reg = 0; reg < regionNames.size(); reg++){
    varList.push_back({});
    //    if (reg == 1){
    if (false){
      varList[reg].push_back("M_Mass_Jet1Jet3");
      varList[reg].push_back("M_HT");
      varList[reg].push_back("M_DeltaRBJetLepton");
      varList[reg].push_back("M_DeltaRBJethadronicW");
      varList[reg].push_back("M_DeltaRWlvJet2");
      varList[reg].push_back("M_Mass_AllJetsMET");
      varList[reg].push_back("M_DeltaPhiJet1Jet2");
      if (!_isEle){
	varList[reg].push_back("M_Pt_Lepton");
      }
      varList[reg].push_back("M_Pt_Jet1LeptonMET");
      varList[reg].push_back("M_Pt_BJet1_4000");
    }
    else if (false){
      //    else if (reg == 2){
      //    varList.push_back("M_nJet3040e24");
      varList[reg].push_back("M_Mass_AllJets");
      varList[reg].push_back("M_Pt_BJet1_4000");
      varList[reg].push_back("M_DeltaRJet1Jet2");
      //varList[reg].push_back("M_DeltaRWlvJet2");
      if (!_isEle){
	varList[reg].push_back("M_Pt_Lepton");
      }
      varList[reg].push_back("M_hadronicWmass");
      varList[reg].push_back("M_DeltaRBJetLepton");
      varList[reg].push_back("M_nJet2040");
    }
    else {
      varList[reg].push_back("M_hadronicWmass");
      varList[reg].push_back("M_DeltaRBJetLepton");
      varList[reg].push_back("M_DeltaRWlvJet2");
      varList[reg].push_back("M_Mass_Jet1Jet3");
      //      varList[reg].push_back("M_nJet2040"); //This variable is poorly modelled now
      varList[reg].push_back("M_DeltaRlightjets");
      /*      if (_isEle){ //Adding all of these into a training for consistency between the two BDTs
	varList[reg].push_back("M_Pt_AllJetsLeptonMET");
      }  
      else {
	varList[reg].push_back("M_Pt_Lepton");
      }*/
      varList[reg].push_back("M_Pt_AllJetsLeptonMET");
      varList[reg].push_back("M_Pt_Lepton");
      varList[reg].push_back("M_E_Jet2Jet3");
    }
  }

  for (auto reg: varList){
    for (auto var: reg){
      _varsInBDTs[var] = 0;
      _varsInBDT_JESShifts[var] = {};
    }
  }
  //Because this screws some things up if it isn't included
  //  _varsInBDTs["M_Pt_Lepton"] = 0;

  /*
    varList.push_back("lightJet1CSV");
    varList.push_back("M_Mass_AllJetsMET");
    //    
    varList.push_back("M_DeltaPhiJet1Jet2");
    varList.push_back("M_DeltaRJet1Jet2");
    varList.push_back("M_E_AllJets");
    varList.push_back("M_Mass_AllJetsMET");
    varList.push_back("M_Pt_Jet1LeptonMET");
    varList.push_back("M_Pt_Jet1Jet2Lepton");
    varList.push_back("M_Pt_LeptonJet1");
    varList.push_back("M_Pt_Jet1_2030");
 
    varList.push_back("M_E_Jet1MET");
    // 
    varList.push_back("M_Pt_Jet1_2040");

    
    varList.push_back("M_hadronicWEta");
    varList.push_back("M_DeltaRBJethadronicW");
    varList.push_back("M_Pt_sys");
    varList.push_back("M_Pt_AllJets2040");
    varList.push_back("M_Pt_BJet1_4000");
    varList.push_back("M_hadronicWEta");
    varList.push_back("M_DeltaRBJethadronicW");
    varList.push_back("M_Pt_sys");

    
    varList.push_back("M_DeltaRBJetLepton");
    varList.push_back("M_DeltaRWlvJet2");

    }*/

  /*
  //og10 variables
  varList.push_back("M_hadronicWmass");
  varList.push_back("M_topMass2_lep");
  varList.push_back("M_Pt_AllJetsLeptonMET");
  varList.push_back("M_Mass_Jet1Jet2Jet3LeptonMET");
  varList.push_back("M_Pt_Lepton");
  varList.push_back("M_DeltaRlightjets");
  varList.push_back("M_DeltaRLeptonJet1");
  varList.push_back("M_DeltaRBJetLepton");
  varList.push_back("lightJet1CSV");
  varList.push_back("M_nJet2040");
  */

  //Variables definitely in both channels

  //  varList.push_back("M_Pt_AllJets2040");
  //varList.push_back("M_Mass_Jet1Jet2");
  //  varList.push_back("M_topMass2_lep");

  
  //maybe use?

    //    varList.push_back("M_HT");
    
    //  varList.push_back("M_DeltaRLeptonJet1");
    
    //varList.push_back("M_Mass_Jet1Jet2Jet3LeptonMET");
    
    //    varList.push_back("M_TMass_Jet1Jet2Jet3"); 
    //    varList.push_back("lightJet1CSV");


    std::cout << varList[0].size() << std::endl;

    //Ones I want to test real quick in 2019 for electron channel
    //    varList.push_back("M_cosThetaStar");
    //    varList.push_back("M_Mass_AllJetsMET");
    //varList.push_back("M_cosThetaStar_hadOnly");
    //    varList.push_back("M_DeltaPhiJet1Jet2");
    //    varList.push_back("M_DeltaRJet1Jet2");
    //varList.push_back("M_E_AllJets");
    //    varList.push_back("M_E_Jet1Jet2Jet3MET");    
    //    varList.push_back("M_HT");
    //    varList.push_back("M_Mass_AllJets");
    //varList.push_back("M_Mass_AllJetsMET");
    
    //    varList.push_back("M_Pt_Jet1LeptonMET");
    //    varList.push_back("M_Pt_Jet1Jet2Lepton");
    //    varList.push_back("M_Pt_LeptonJet1");
    //    varList.push_back("M_Pt_Jet1_2030");
    
    //varList.push_back("M_nJet3040e24");
    //    varList.push_back("M_E_Jet1MET");
    //varList.push_back("M_Jet1Jet2Jet3_Centrality");
    //  varList.push_back("M_Pt_Jet1_2040");

    
    //varList.push_back("M_hadronicWEta");
    //varList.push_back("M_DeltaRBJethadronicW");
    //varList.push_back("M_Pt_sys");
    //varList.push_back("M_cosThetaStar");
    //varList.push_back("M_cosThetaStar_lepOnly");
    //varList.push_back("lightJet2CSV");
    //varList.push_back("M_Pt_AllJets2040");
    //varList.push_back("M_Pt_BJet1_4000");
//varList.push_back("M_hadronicWEta");
    //varList.push_back("M_DeltaRBJethadronicW");
    //varList.push_back("M_Pt_sys");


    

  //The list of samples. this is used by the reading, not the training.
  samplelist.push_back("tW_top_nfh");
  
  //At some point this should be filled out with the names of the systematics so that we can read those too
  systlist.push_back("");
  systlist.push_back("_PU_up");
  systlist.push_back("_PU_down");
  systlist.push_back("_LSF_up");
  systlist.push_back("_LSF_down");
  systlist.push_back("_Trig_up");
  systlist.push_back("_Trig_down");
  systlist.push_back("_bTag_up");
  systlist.push_back("_bTag_down");
  systlist.push_back("_mistag_up");
  systlist.push_back("_mistag_down");
  systlist.push_back("_PDF_up");
  systlist.push_back("_PDF_down");

  //    printVarList();


}



void mvaTool::doBothTraining(TString inDir){
  doTraining(inDir,true);
  doTraining(inDir,false);
}

void mvaTool::doTraining(TString inDir, bool isttbar = true){
  
  //This needs to be done first
  TMVA::Tools::Instance();

  //Create the output file that will store the training results
  std::string postfix1 = "_ttbar";
  if (!isttbar) postfix1 = "_wJets";
  //  TString outfileName("training/tWLepJets_training_BDT"+postfix1+std::to_string(_nTrees)+"_channel"+std::to_string(_channel)+".root");
  TString outfileName("training/tWLepJets_training_BDT"+postfix1+std::to_string(_nTrees)+".root");
  //TString outfileName("training/trainingFornote.root");
  TFile* outFile = TFile::Open(outfileName, "RECREATE");

  //Create the factory
  TMVA::Factory *factory = new TMVA::Factory("tWLepJet_training",outFile,"!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification");

  //Create a data loader
  TMVA::DataLoader loader{"loader"};
  
  //Make TChains for signal and background
  TChain signal("TNT/BOOM");
  TChain bkg_ttbar("TNT/BOOM");
  TChain bkg_wJets("TNT/BOOM");

    //From the inDir, add all skim files to the trees
  for (unsigned int i = 0; i < 30; i++){
    if (!_isEle){
    signal.Add(inDir+"tW_top_nfh/skims/tW_top_nfh_tW_"+std::to_string(i)+"Skim.root");
    signal.Add(inDir+"tW_antitop_nfh/skims/tW_antitop_nfh_tW_"+std::to_string(i)+"Skim.root");
    }
    else{
    signal.Add(inDir+"tW_top_nfh/skims/tW_top_nfh_tWEle_"+std::to_string(i)+"Skim.root");
    signal.Add(inDir+"tW_antitop_nfh/skims/tW_antitop_nfh_tWEle_"+std::to_string(i)+"Skim.root");
    }
  }
  for (unsigned int i = 0; i < 100; i++){
    if (!_isEle){
    bkg_ttbar.Add(inDir+"ttbar/skims/ttbar_tW_"+std::to_string(i)+"Skim.root");
    bkg_ttbar.Add(inDir+"ttbarBU/skims/ttbarBU_tW_"+std::to_string(i)+"Skim.root");
    }
    else {
    bkg_ttbar.Add(inDir+"ttbar/skims/ttbar_tWEle_"+std::to_string(i)+"Skim.root");
    bkg_ttbar.Add(inDir+"ttbarBU/skims/ttbarBU_tWEle_"+std::to_string(i)+"Skim.root");
    }
  }
  //  bkg_ttbar.Add(inDir+"ttbarBU/skims/ttbarBU*Skim.root");
  //bkg_wJets.Add(inDir+"wPlusJetsMCatNLO/skims/wPlusJetsMCatNLO*Skim.root");
  bkg_wJets.Add(inDir+"wPlus0Jets/skims/wPlus0Jets*Skim.root");
  bkg_wJets.Add(inDir+"wPlus1Jets/skims/wPlus1Jets*Skim.root");
  bkg_wJets.Add(inDir+"wPlus2Jets/skims/wPlus2Jets*Skim.root");
  
  
  //Grab the input files now
  //  TFile * sigFile = TFile::Open("../condorStuff/skims20161129_bdtVariables/tW/tW_top/skims/tW_top0Skim.root"); //Not the final one, of course. Will probably need to sum the skims
  //  TFile * bkgFile = TFile::Open("../condorStuff/skims20161129_bdtVariables/tW/ttbar/skims/ttbar0Skim.root");

  //signal.Add("tW/tW_antitop_nfh/skims/tW_antitop_nfh0Skim.root");
  //bkg_ttbar.Add("tW/ttbar/skims/ttbar0Skim.root");

  //Get the trees
  //TTree *signal = (TTree*)sigFile->Get("TNT/BOOM");
  //TTree *bkg_ttbar = (TTree*)bkgFile->Get("TNT/BOOM");

  //Add the trees to the factory
  loader.AddSignalTree    (&signal,    1.);
  if (isttbar && !(_channel == 2)){
    loader.AddBackgroundTree(&bkg_ttbar,1.);
  }
  else {
    loader.AddBackgroundTree(&bkg_wJets,1.);
  }
  
  for (unsigned int i=0; i < varList[_channel].size(); i++) loader.AddVariable(varList[_channel][i].Data(),'F');

  loader.SetSignalWeightExpression("EventWeight");
  loader.SetBackgroundWeightExpression("EventWeight");

  TCut mycutsSig = "M_channel == 0 && (M_nBJet3040 + M_nBJet4000) == 1 ";
  TCut mycutsBkg = "M_channel == 0 && (M_nBJet3040 + M_nBJet4000) == 1 ";

  if (_channel == 1){
    mycutsSig = "M_channel == 1";
    mycutsBkg = "M_channel == 1";
  }
  if (_channel == 2){
    mycutsSig = "M_channel == 2";
    mycutsBkg = "M_channel == 2";
  }

  loader.PrepareTrainingAndTestTree( mycutsSig, mycutsBkg,"nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

  TString postfix = "_ttbar";
  if (!isttbar) postfix = "_wJets";

  //  factory->BookMethod(&loader,TMVA::Types::kBDT, "BDT_200_10", "!H:!V:NTrees=200:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=10:PruneMethod=NoPruning" );
  //  factory->BookMethod(&loader,TMVA::Types::kBDT, "BDT_200_5", "!H:!V:NTrees=200:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=5:PruneMethod=NoPruning" );
  //  factory->BookMethod(&loader,TMVA::Types::kBDT, "BDT_Grad_400_10", "!H:!V:NTrees=400:MaxDepth=3:BoostType=Grad:SeparationType=GiniIndex:nCuts=10:PruneMethod=NoPruning" );
  //factory->BookMethod(&loader,TMVA::Types::kBDT, "BDT_Grad_400_5", "!H:!V:NTrees=400:MaxDepth=3:BoostType=Grad:SeparationType=GiniIndex:nCuts=5:PruneMethod=NoPruning" );
  //factory->BookMethod(&loader,TMVA::Types::kBDT, "BDT_Grad_200_10", "!H:!V:NTrees=200:MaxDepth=3:BoostType=Grad:SeparationType=GiniIndex:nCuts=10:PruneMethod=NoPruning" );
  //factory->BookMethod(&loader,TMVA::Types::kBDT, "BDT_Grad_200_5", "!H:!V:NTrees=200:MaxDepth=3:BoostType=Grad:SeparationType=GiniIndex:nCuts=5:PruneMethod=NoPruning" );
  //factory->BookMethod(&loader,TMVA::Types::kBDT, "BDT_400_10", "!H:!V:NTrees=400:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=10:PruneMethod=NoPruning" );
  //  for (int nTrees=400;nTrees<601;nTrees+=100){
  //    for (int depth=2;depth<5;depth+=1){
  //  for (_depth=2;_depth<5;_depth+=1){
  //  for (_nCuts=4;_nCuts<25;_nCuts+=3){
  //  for (int nCuts=22;nCuts<23;nCuts+=4){
	/*  for (int nTrees=100;nTrees<201;nTrees+=100){ 
    for (int depth=1;depth<2;depth+=1){
    for (int nCuts=4;nCuts<5;nCuts+=2){*/
	//	factory->BookMethod(&loader,TMVA::Types::kBDT, "paramScan_nTrees"+std::to_string(_nTrees)+"_depth"+std::to_string(depth)+"_nCuts"+std::to_string(nCuts),"!H:!V:NTrees="+std::to_string(_nTrees)+":MaxDepth="+std::to_string(depth)+":BoostType=AdaBoost:SeparationType=GiniIndex:nCuts="+std::to_string(nCuts)+":PruneMethod=NoPruning:Shrinkage=0.2" );
	//	factory->BookMethod(&loader,TMVA::Types::kBDT, "grad_nTrees"+std::to_string(_nTrees)+"_depth"+std::to_string(depth)+"_nCuts"+std::to_string(nCuts),"!H:!V:NTrees="+std::to_string(_nTrees)+":MaxDepth="+std::to_string(depth)+":BoostType=AdaBoost:SeparationType=GiniIndex:nCuts="+std::to_string(nCuts)+":PruneMethod=NoPruning" );
	//      factory->BookMethod(&loader,TMVA::Types::kBDT, "grad_nTrees"+std::to_string(_nTrees)+"_depth"+std::to_string(_depth)+"_nCuts"+std::to_string(_nCuts)+"_channel"+std::to_string(_channel),"!H:!V:NTrees="+std::to_string(_nTrees)+":MaxDepth="+std::to_string(_depth)+":BoostType=Grad:SeparationType=GiniIndex:nCuts="+std::to_string(_nCuts)+":PruneMethod=NoPruning" );
  //	factory->BookMethod(&loader,TMVA::Types::kBDT, "grad_nTrees"+std::to_string(_nTrees)+"_depth"+std::to_string(_depth)+"_nCuts"+std::to_string(_nCuts),"!H:!V:NTrees="+std::to_string(_nTrees)+":MaxDepth="+std::to_string(_depth)+":BoostType=Grad:SeparationType=GiniIndex:nCuts="+std::to_string(_nCuts)+":PruneMethod=NoPruning" );
	factory->BookMethod(&loader,TMVA::Types::kBDT, "grad_nTrees"+std::to_string(_nTrees)+"_depth"+std::to_string(_depth)+"_nCuts"+std::to_string(_nCuts),"!H:!V:NTrees="+std::to_string(_nTrees)+":MaxDepth="+std::to_string(_depth)+":BoostType=Grad:SeparationType=GiniIndex:nCuts="+std::to_string(_nCuts)+":PruneMethod=NoPruning" );
      // }
      //}
	//	factory->BookMethod(&loader,TMVA::Types::kBDT, "baggin_nTrees"+std::to_string(_nTrees)+"_depth"+std::to_string(depth)+"_nCuts"+std::to_string(nCuts),"!H:!V:NTrees="+std::to_string(_nTrees)+":MaxDepth="+std::to_string(depth)+":BoostType=Bagging:SeparationType=GiniIndex:nCuts="+std::to_string(nCuts)+":PruneMethod=NoPruning" );
	//      }
	// }
	//}
  

  //  factory->BookMethod(&loader,TMVA::Types::kBDT, "No_ptJets2040_BDT_Grad_1000_20_0.1"+postfix, "!H:!V:NTrees=1000:MaxDepth=3:BoostType=Grad:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning:Shrinkage=0.1" );
  //  factory->BookMethod(&loader,TMVA::Types::kBDT, "BDT_Grad"+postfix, "!H:!V:NTrees=10:MaxDepth=2:BoostType=Grad:SeparationType=GiniIndex:PruneMethod=NoPruning:Shrinkage=0.1" );
  

  // Train MVAs using the set of training events                  
  factory->TrainAllMethods();

  // ---- Evaluate all MVAs using the set of test events          
  factory->TestAllMethods();

  // ----- Evaluate and compare performance of all configured MVAs
  factory->EvaluateAllMethods();

  outFile->Close();

  delete factory;

}

void mvaTool::doReading(TString sampleName, TString inDir, TString outDir, bool isData, int minFile, int maxFile){
  
  std::cout << "Entering reading routine" << std::endl;
  TMVA::Tools::Instance();

  //Bare with me, this needs to be done for each channel individually because we want to be able to run different MVAs per channel now.
  std::vector<std::vector<float > > proxyvars;

  for (unsigned int reg = 0; reg < regionNames.size(); reg++){
    std::cout << "Making BDT for " << regionNames[reg] << " region" << std::endl;
    //    reader.push_back(new TMVA::Reader( "!Color:!Silent" ));
    reader.push_back(new TMVA::Reader( "!Color:!Silent" ));
    unsigned int varListRegSize = varList[reg].size();
    proxyvars.push_back({});
    for (unsigned int i=0; i<varListRegSize; i++){
      std::cout << "[Variable loop] Adding variable: " << varList[reg][i].Data() << std::endl;
      proxyvars[reg].push_back(0);
      //      std::cout << "adding to reader" << std::endl;
      reader[reg]->AddVariable (varList[reg][i].Data(), &(proxyvars[reg][i]) );
    }
    reader[reg]->BookMVA(_bdtName[reg], baseName+"loader/weights/tWLepJet_training_"+_bdtName[reg] + ".weights.xml" );
  }

  //  reader->BookMVA( "BDT_ttbar", baseName+"loader/weights/tWLepJet_training_No_ptJets2040_BDT_Grad_1000_20_0.1_ttbar.weights.xml" );
  //.  else reader->BookMVA( _bdtName, baseName+"loader/weights/tWLepJet_training_ada_nTrees400_depth2_nCuts22.weights.xml" );

  //  reader->BookMVA( "BDT_wJets", baseName+"loader/weights/tWLepJet_training_BDT_Grad_1000_20_0.1_wJets.weights.xml" );

  std::cout << "Finished reading BDT training" << std::endl;

  std::cout << "Processing sample: " << sampleName << std::endl;
  
  processMCSample(sampleName,inDir,outDir,proxyvars,isData, true, minFile, maxFile);
  
  std::cout << "Finished processing " << sampleName << std::endl;
  
}

void mvaTool::doReadingNoMVA(TString sampleName, TString inDir, TString outDir, bool isData){
  
  std::cout << "Entering reading routine" << std::endl;

  unsigned int varsize = varList[0].size();
  std::vector<float *> treevars[varsize];
  //  std::vector<float> treevarsJetShifts[varsize];
  std::vector<std::vector<std::vector<float> * > > treevarsJetShifts;
  std::vector<float *> proxyvars[varsize];

  std::cout << "Entering variable adding" << std::endl;
  //for (unsigned int i=0; i<varsize;i++){
  // proxyvars[i] = 0;
  //  std::cout << "[Variable loop] Adding variable: " << varList[i].Data() << std::endl;
  //}


  std::cout << "Finished reading BDT training" << std::endl;

  std::cout << "Processing sample: " << sampleName << std::endl;
  
  //  processMCSample(sampleName,inDir,outDir,proxyvars, treevars, treevarsJetShifts, isData, false);
  
  std::cout << "Finished processing " << sampleName << std::endl;
  
}


//Do the thing
void mvaTool::processMCSample(TString sampleName, TString inDir, TString outDir, std::vector<std::vector<float > > proxyvars, bool isData, bool doMVA, int minFile, int maxFile){

  TChain* theTree = new TChain("TNT/BOOM");
  
  std::cout << minFile << " " << maxFile;

  TString dirWithTrees;
  if (minFile < 0 && maxFile < 0){
    theTree->Add(inDir+sampleName+"/skims/"+sampleName+"*Skim.root");
    std::cout << "[loopInSample] Added "<< inDir+sampleName+"/skims/"+sampleName+"*Skim.root" << " to TChain" << std::endl;  
  }
  else {
    for (int i = minFile; i <= maxFile; i++){
      theTree->Add(inDir+sampleName+"/skims/"+sampleName+"_"+inDir(0,inDir.Length()-1)+"_"+std::to_string(i)+"Skim.root");
      std::cout << "[loopInSample] Added "<< inDir+sampleName+"/skims/"+sampleName+"_"+inDir(0,inDir.Length()-1)+"_"+std::to_string(i)+"Skim.root" << " to TChain" << std::endl;  
    }
  }
//  TFile *input = new TFile( (inDir+sampleName+"/hists/merged"+sampleName+".root").Data(), "READ");
  std::vector<TFile*> theoutputfiles;
  string outNumber = "";
  if (minFile >= 0 && maxFile >= 0)
    outNumber = std::to_string((int)(minFile/(maxFile+1-minFile)));
  for (auto const regionName : regionNames){
    std::cout << "Createing file " << outDir+regionName+"/output_"+sampleName+outNumber+".root" <<std::endl;
    TFile *theoutputfile = new TFile( (outDir+regionName+"/output_"+sampleName+outNumber+".root").Data(), "RECREATE");
    theoutputfiles.push_back(theoutputfile);
  }

  std::cout << "Files have been create" << std::endl;
  //  TFile *thetreefile = new TFile( (outDir+"bdtTree_"+sampleName+".root").Data(), "RECREATE");
  //theTreeOutputFileMap[sampleName] = thetreefile;
  
  //------------------------
  // Systematic loop
  //------------------------
  // WIP. This will include the additional files at some point
  

  for (unsigned int j=0; j < systlist.size(); j++){
    if (isData && systlist[j] != "") continue;
    createHists(sampleName+systlist[j]);
  }

  loopInSample(theTree,sampleName,proxyvars,isData,doMVA);
  //makeStatVariationHists(sampleName,theoutputfiles); //We do this in processMCSample so that we have the output file to save the stat variations into.
  
  saveHists(theoutputfiles);
    
  for (auto const outputfile: theoutputfiles){
    outputfile->Close();
    delete outputfile;
  }
}

//Loop over the events in the desired sample
void mvaTool::loopInSample(TTree* theTree, TString sampleName, std::vector<std::vector<float> > proxyvars, bool isData, bool doMVA){

  //This makes sure the tree is initialised and finds the correct vectors. This hasnm't always been here, and for a long time it worked without it. Why has this changed now? I HAVE NO IDEA
  theTree->GetEntry(0);

  //assign all the BDT variables for each region
  for (auto vars: _varsInBDTs){
    std::cout << vars.first << " " <<  vars.second << std::endl;
    theTree->SetBranchAddress( vars.first.Data(), &(_varsInBDTs[vars.first]));
    std::cout << vars.first << " " <<  vars.second << " " << _varsInBDTs[vars.first] <<  std::endl;
    theTree->SetBranchAddress( (vars.first+"_JESShifts").Data(), &(_varsInBDT_JESShifts[vars.first]));
    std::cout << vars.first+"_JESShifts" << " " << _varsInBDT_JESShifts[vars.first] << std::endl;
  }
   
  theweight=0.;
  theTree->SetBranchAddress( "EventWeight", &theweight );

  //Debug vars
  int eventNumber = 0;
  theTree->SetBranchAddress ( "EVENT_event", &eventNumber);

  //Get the systematic weights here. We will then fill hists separately as a result of this.
  float puWeight=0., puWeightUp = 0., puWeightDown = 0.;
  float lepSFWeight=0., lepSFWeightUp = 0., lepSFWeightDown = 0.;
  float trigSFWeight=0., trigSFWeightUp = 0., trigSFWeightDown = 0.;
  float bWeight=0., bWeighthfs1Up = 0., bWeighthfs1Down = 0., bWeighthfs2Up = 0., bWeighthfs2Down = 0., bWeightcferr1Up = 0., bWeightcferr1Down = 0., bWeightcferr2Up = 0., bWeightcferr2Down = 0., bWeightjerUp = 0., bWeightjerDown = 0., bWeightlfUp = 0., bWeightlfDown = 0., bWeightUp = 0., bWeightDown = 0.;
  float mistagWeight=0., mistagWeighthfs1Up = 0., mistagWeighthfs1Down = 0., mistagWeighthfs2Up = 0., mistagWeighthfs2Down = 0., mistagWeightcferr1Up = 0., mistagWeightcferr1Down = 0., mistagWeightcferr2Up = 0., mistagWeightcferr2Down = 0., mistagWeightjerUp = 0., mistagWeightjerDown = 0., mistagWeightlfUp = 0., mistagWeightlfDown = 0., mistagWeightDown = 0., mistagWeightUp = 0.;
  double pdfUp = 0., pdfDown = 0.;

  float jet1Pt = 0.;

  //grab the jet pts
  std::vector<double> jetPts;
  for (int it = 0; it  < 10; it++){
    jetPts.push_back(0.);
  }
  std::vector<double> * jetPtsPtr = &jetPts;
  theTree->SetBranchAddress("Jet_pt",&jetPtsPtr);
  theTree->SetBranchAddress("M_Pt_Jet1",&jet1Pt);

  //jes shifts
  //std::vector<double> jesAbsSts;
  //for (unsigned int jesInd = 0; jesInd < 10; jesInd++){
  //  jesAbsSts.push_back(0.);
  //}
  //std::vector<double> * jesAbsVec = &jesAbsSts;
  std::vector<std::vector<double> > jesPtr;
  std::vector<std::vector<double> * > pointerVec;
  for (unsigned int jesInd = 0; jesInd < 60; jesInd++){
    std::vector<double> tempJES;
    for (unsigned int jetInd = 0; jetInd < 10; jetInd++){
      tempJES.push_back(1.);
    }
    //    std::vector<double> * tempJESPtr = &tempJES;
    jesPtr.push_back(tempJES);
    //    std::cout << pointerVec[jesInd] << " ";
  }
  std::cout << std::endl;
  for (unsigned int jesInd = 0; jesInd < 60; jesInd++){ 
    pointerVec.push_back(&jesPtr[jesInd]);
    //std::cout <<pointerVec[jesInd] << " " << & jesPtr[jesInd] << " "; }
  }
    //  std::vector<double> *tempVec = & jesPtr[0];
  //  std::cout << std::endl << tempVec << " " <<pointerVec[0] << " " << &jesPtr[0] << std::endl;
  //theTree->SetBranchAddress("Jet_JesSF_AbsoluteStat_down",&pointerVec[0]);
  //  tempVec = & jesPtr[1];
  if (!isData){
  theTree->SetBranchAddress("Jet_JesSF_AbsoluteStat_up",&(pointerVec[0]));
  theTree->SetBranchAddress("Jet_JesSF_AbsoluteStat_down",&(pointerVec[1]));
  theTree->SetBranchAddress("Jet_JesSF_AbsoluteScale_up",&(pointerVec[2]));
  theTree->SetBranchAddress("Jet_JesSF_AbsoluteScale_down",&(pointerVec[3]));
  theTree->SetBranchAddress("Jet_JesSF_AbsoluteMPFBias_up",&(pointerVec[4]));
  theTree->SetBranchAddress("Jet_JesSF_AbsoluteMPFBias_down",&(pointerVec[5]));
  theTree->SetBranchAddress("Jet_JesSF_Fragmentation_up",&(pointerVec[6]));
  theTree->SetBranchAddress("Jet_JesSF_Fragmentation_down",&(pointerVec[7]));
  theTree->SetBranchAddress("Jet_JesSF_SinglePionECAL_up",&(pointerVec[8]));
  theTree->SetBranchAddress("Jet_JesSF_SinglePionECAL_down",&(pointerVec[9]));
  theTree->SetBranchAddress("Jet_JesSF_SinglePionHCAL_up",&(pointerVec[10]));
  theTree->SetBranchAddress("Jet_JesSF_SinglePionHCAL_down",&(pointerVec[11]));
  theTree->SetBranchAddress("Jet_JesSF_FlavorQCD_up",&(pointerVec[12]));
  theTree->SetBranchAddress("Jet_JesSF_FlavorQCD_down",&(pointerVec[13]));
  theTree->SetBranchAddress("Jet_JesSF_TimePtEta_up",&(pointerVec[14]));
  theTree->SetBranchAddress("Jet_JesSF_TimePtEta_down",&(pointerVec[15]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeJEREC1_up",&(pointerVec[16]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeJEREC1_down",&(pointerVec[17]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeJEREC2_up",&(pointerVec[18]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeJEREC2_down",&(pointerVec[19]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeJERHF_up",&(pointerVec[20]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeJERHF_down",&(pointerVec[21]));
  theTree->SetBranchAddress("Jet_JesSF_RelativePtBB_up",&(pointerVec[22]));
  theTree->SetBranchAddress("Jet_JesSF_RelativePtBB_down",&(pointerVec[23]));
  theTree->SetBranchAddress("Jet_JesSF_RelativePtEC1_up",&(pointerVec[24]));
  theTree->SetBranchAddress("Jet_JesSF_RelativePtEC1_down",&(pointerVec[25]));
  theTree->SetBranchAddress("Jet_JesSF_RelativePtEC2_up",&(pointerVec[26]));
  theTree->SetBranchAddress("Jet_JesSF_RelativePtEC2_down",&(pointerVec[27]));
  theTree->SetBranchAddress("Jet_JesSF_RelativePtHF_up",&(pointerVec[28]));
  theTree->SetBranchAddress("Jet_JesSF_RelativePtHF_down",&(pointerVec[29]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeBal_up",&(pointerVec[30]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeBal_down",&(pointerVec[31]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeFSR_up",&(pointerVec[32]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeFSR_down",&(pointerVec[33]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeStatEC_up",&(pointerVec[34]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeStatEC_down",&(pointerVec[35]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeStatHF_up",&(pointerVec[36]));
  theTree->SetBranchAddress("Jet_JesSF_RelativeStatHF_down",&(pointerVec[37]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtRef_up",&(pointerVec[38]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtRef_down",&(pointerVec[39]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtBB_up",&(pointerVec[40]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtBB_down",&(pointerVec[41]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtEC1_up",&(pointerVec[42]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtEC1_down",&(pointerVec[43]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtEC2_up",&(pointerVec[44]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtEC2_down",&(pointerVec[45]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtHF_up",&(pointerVec[46]));
  theTree->SetBranchAddress("Jet_JesSF_PileUpPtHF_down",&(pointerVec[47]));
  theTree->SetBranchAddress("Jet_JerSFup",&pointerVec[48]);
  theTree->SetBranchAddress("Jet_JerSFdown",&pointerVec[49]);
  theTree->SetBranchAddress("Jet_JerSFup_syst",&pointerVec[50]);
  theTree->SetBranchAddress("Jet_JerSFdown_syst",&pointerVec[51]);
  theTree->SetBranchAddress("Jet_JesSF", &pointerVec[58]);
  theTree->SetBranchAddress("Jet_JerSF", &pointerVec[59]);
  
  //  theTree->SetBranchAddress("Jet_JesSF_AbsoluteStat_up",&jesAbsVec);

  }

  int nbJets3040 = 0., nbJets4000 = 0.;
  int nJets3040 = 0.,nJets4000 = 0.;

  double met = -100.,metPhi = 100.;
  float lepPt = -100., lepPhi = 100., lepEta = 100.;

  float theChannel = -1;

  std::vector<float> jesChannelsVec;
  std::vector<float> jet1PtVec;
  for (unsigned int jesInd = 0; jesInd < 100; jesInd++){
    jesChannelsVec.push_back(0.);
    jet1PtVec.push_back(0.);
  }

  std::vector<float>*  jesChannels = &jesChannelsVec;
  std::vector<float>* jet1PtJES = & jet1PtVec;

  std::cout << "[loopInSample] Finished assigning variables" << std::endl;

  //  
  ptrdiff_t pos1  = std::distance(varList[0].begin(), std::find(varList[0].begin(),varList[0].end(),"M_Pt_Lepton")); 
  unsigned int pos = pos1; //This line prevents an annoying warning. Woo hack.
  //if (pos > varList[reg].size()) theTree->SetBranchAddress( "M_Pt_Lepton",&lepPt);
  //  theTree->SetBranchAddress( "M_Pt_Lepton",&lepPt);
  theTree->SetBranchAddress( "M_Phi_Lepton",&lepPhi);
  theTree->SetBranchAddress( "M_Eta_Lepton",&lepEta);
  theTree->SetBranchAddress( "Met_type1PFxy_pt",&met);
  theTree->SetBranchAddress( "Met_type1PFxy_phi",&metPhi);
  
  theTree->SetBranchAddress("M_channel_JESShifts",&jesChannels);

  theTree->SetBranchAddress("M_Pt_Jet1_JESShifts",&jet1PtJES);

  theTree->SetBranchAddress("M_Pt_Lepton",&_varsInBDTs["M_Pt_Lepton"]);

  std::cout << "Var addresses"  << lepPhi << " " << _varsInBDTs["M_Pt_Lepton"] << " " << _varsInBDT_JESShifts["M_Pt_Lepton"] << " " << met << std::endl;

  if (doMVA){
    theTree->SetBranchAddress( "M_channel",&theChannel);
    theTree->SetBranchAddress( "M_nBJet3040",&nbJets3040);
    theTree->SetBranchAddress( "M_nBJet4000",&nbJets4000);
    theTree->SetBranchAddress( "M_nJet3040e24",&nJets3040);
    theTree->SetBranchAddress( "M_nJet4000e24",&nJets4000);
    
  }

  if (!isData){
    theTree->SetBranchAddress( "puWeight", &puWeight );
    theTree->SetBranchAddress( "puWeight_SysUp", &puWeightUp );
    theTree->SetBranchAddress( "puWeight_SysDown", &puWeightDown );
  
    std::cout << "[loopInSample] Finished assigning pileup weights" << std::endl;

    theTree->SetBranchAddress( "lepSF", &lepSFWeight );
    theTree->SetBranchAddress( "lepSF_SysUp", &lepSFWeightUp );
    theTree->SetBranchAddress( "lepSF_SysDown", &lepSFWeightDown );

    theTree->SetBranchAddress( "trigSF", &trigSFWeight );
    theTree->SetBranchAddress( "trigSF_SysUp", &trigSFWeightUp );
    theTree->SetBranchAddress( "trigSF_SysDown", &trigSFWeightDown );
    
    if (_useIterFit){

      theTree->SetBranchAddress( "bWeight_central", &bWeight );
      theTree->SetBranchAddress( "bWeight_up_hfstats1", &bWeighthfs1Up );
      theTree->SetBranchAddress( "bWeight_down_hfstats1", &bWeighthfs1Down );
      theTree->SetBranchAddress( "bWeight_up_hfstats2", &bWeighthfs2Up );
      theTree->SetBranchAddress( "bWeight_down_hfstats2", &bWeighthfs2Down );
      theTree->SetBranchAddress( "bWeight_up_cferr1", &bWeightcferr1Up );
      theTree->SetBranchAddress( "bWeight_down_cferr1", &bWeightcferr1Down );
      theTree->SetBranchAddress( "bWeight_up_cferr2", &bWeightcferr2Up );
      theTree->SetBranchAddress( "bWeight_down_cferr2", &bWeightcferr2Down );
      theTree->SetBranchAddress( "bWeight_up_jes", &bWeightjerUp );
      theTree->SetBranchAddress( "bWeight_down_jes", &bWeightjerDown );
      theTree->SetBranchAddress( "bWeight_up_lf", &bWeightlfUp );
      theTree->SetBranchAddress( "bWeight_down_lf", &bWeightlfDown );

      theTree->SetBranchAddress( "misTagWeight_central", &mistagWeight );
      theTree->SetBranchAddress( "misTagWeight_up_hfstats1", &mistagWeighthfs1Up );
      theTree->SetBranchAddress( "misTagWeight_down_hfstats1", &mistagWeighthfs1Down );
      theTree->SetBranchAddress( "misTagWeight_up_hfstats2", &mistagWeighthfs2Up );
      theTree->SetBranchAddress( "misTagWeight_down_hfstats2", &mistagWeighthfs2Down );
      theTree->SetBranchAddress( "misTagWeight_up_cferr1", &mistagWeightcferr1Up );
      theTree->SetBranchAddress( "misTagWeight_down_cferr1", &mistagWeightcferr1Down );
      theTree->SetBranchAddress( "misTagWeight_up_cferr2", &mistagWeightcferr2Up );
      theTree->SetBranchAddress( "misTagWeight_down_cferr2", &mistagWeightcferr2Down );
      theTree->SetBranchAddress( "misTagWeight_up_jes", &mistagWeightjerUp );
      theTree->SetBranchAddress( "misTagWeight_down_jes", &mistagWeightjerDown );
      theTree->SetBranchAddress( "misTagWeight_up_lf", &mistagWeightlfUp );
      theTree->SetBranchAddress( "misTagWeight_down_lf", &mistagWeightlfDown );
    }
    else {
      theTree->SetBranchAddress( "bWeight_central", &bWeight );
      theTree->SetBranchAddress( "bWeight_up", &bWeightUp );
      theTree->SetBranchAddress( "bWeight_down", &bWeightDown );

      theTree->SetBranchAddress( "misTagWeight_central", &mistagWeight );
      theTree->SetBranchAddress( "misTagWeight_up", &mistagWeightUp );
      theTree->SetBranchAddress( "misTagWeight_down", &mistagWeightDown );

    }
    
    theTree->SetBranchAddress( "EVENT_PDFtthbbWeightUp",&pdfUp );
    theTree->SetBranchAddress( "EVENT_PDFtthbbWeightDown",&pdfDown );

    std::cout << "[loopInSample] Finished assigning lepton weights" << std::endl;
  }
  
  //  std::vector<float> bTagSysts;
  //  setbTagVars(theTree);
  
  if (theTree ==0) cout << "No TTree found for " << sampleName << "!" << std::endl;
  
  std::tuple<float,float> bSysts (std::make_pair(1.,1.));
  std::tuple<float,float> mistagSysts (std::make_pair(1.,1.));

  //Loop over the events
  std::cout << theTree->GetEntries() << " events in loop for sample " << sampleName << std::endl; 
  int loopMax = theTree->GetEntries();
  
  //debug mode
  //loopMax = 2000;

  for (int i = 0; i < loopMax; i++){
  //  for (int i = 0; i < 50; i++){ //Temporary 'just do this a few times' loop

    //if (i > 20000) break;
    if (i%500 == 0){
      //if (i%1 == 0){
      printf ("Processing event %i\r", i);
      fflush(stdout);
      //      std::cout << jesPtr[0].at(0);
      //  std::cout << " " << jesAbsSts[0] << std::endl;
    }
    theTree->GetEntry(i);
    //    if (! (pos > varList[reg].size())) lepPt = treevars[reg][pos];
    //    std::cout << _varsInBDTs["M_Pt_Lepton"] <<  " " << (_varsInBDTs["M_Pt_Lepton"]) << std::endl;
    lepPt = (_varsInBDTs["M_Pt_Lepton"]);

    //make these before 
    /*    for (unsigned int jesInd = 0; i <  treevarsJetS1hifts.size(); i++){
      std::cout << "jet shifts length " <<i << " " << treevarsJetShifts[i]->size() << std::endl;
      }*/
    if (i == 0) { //This is the first event in the tree and we nede to generate the histograms. This apparently has to go here to avoid causing a massive crash.
      for (unsigned int jesInd = 0; jesInd < _varsInBDT_JESShifts.begin()->second->size(); jesInd++){
	createHists(sampleName+"_JetShifts_"+std::to_string(jesInd));
      }
    }

    //    if (theChannel != _channel) continue;

    bool runEvent = false;
    
    int nJets = nJets3040+nJets4000;
    int nbJets = nbJets3040+nbJets4000;

    //    float jet1Pt = jetPts[0] * jesPtr[59].at(0);

    bool isBarrel = true;
    if (_isEle){
      if (fabs(lepEta) > 1.479) isBarrel = false;
    }
    else {
      if (fabs(lepEta) > 1.2) isBarrel = false;
    }

    //Recalculate the channel
    int tempChannel = -1;
    if (nJets == 2 && nbJets == 1) tempChannel = 2;     
    else if (nJets == 3 && nbJets == 1) tempChannel = 0;
    else if (nJets == 4 && nbJets == 1) tempChannel = 3;
    else if (nJets == 3 && nbJets == 2) tempChannel = 1;
    else if (nJets == 4 && nbJets == 2) tempChannel = 4;
    else if (nJets == 2 && nbJets == 0) tempChannel = 5;
    else if (nJets == 3 && nbJets == 0) tempChannel = 6;
    else if (nJets == 4 && nbJets == 0) tempChannel = 7;

    //This is where we will split barrel and endcap if we want to
    if (_splitBarrelEndcap && !isBarrel) tempChannel+=8;

    //    theChannel = tempChannel;


    if (theChannel >= 0 && theChannel <= 7) runEvent = true;
    if (_splitBarrelEndcap && !isBarrel)  theChannel+=8;
    for (unsigned int chanInd = 0; chanInd < _varsInBDT_JESShifts.begin()->second->size(); chanInd++){
      if (jesChannels->at(chanInd) >= 0 && jesChannels->at(chanInd) <= 7) runEvent = true;
    }
    if (!runEvent){
      continue;
    }

    //    std::cout << theChannel << " " << tempChannel << " " << jesChannels->at(0) << " " << jesChannels->at(1) << std::endl;
      
    //    if (tempChannel != theChannel) std::cout << theChannel << " " << tempChannel << " " << jesChannels->at(0) << " " << jesChannels->at(1) << std::endl;

    /*    if (theChannel == 1){
      std::cout << eventNumber << " weight: " << theweight << std::endl;
      }*/


    //if (theChannel == 0){ // A backup because I messed up the channel flag in the first reprocessing.
      //      std::cout << "Zero! njets are: " << nbJets4000 << " " << nbJets3040 <<std::endl;
    //  if ((nbJets4000 + nbJets3040) != 1) continue;
    //}

    for (unsigned int reg = 0; reg < regionNames.size(); reg++){
      for (unsigned int ivar=0; ivar<varList[reg].size(); ivar++) {
	proxyvars[reg][ivar] = _varsInBDTs[varList[reg][ivar]];
	//Here we will change the value of default parameters if they are very negative.
	if (proxyvars[reg][ivar] < -100.){
	  //	  std::cout << reg << " " << ivar << " " <<  proxyvars[reg][ivar];
	  proxyvars[reg][ivar] = _emptyVarValue;
	  //std::cout << " after: " << proxyvars[reg][ivar] << std::endl;
	}
	//	if (_varsInBDTs[varList[reg][ivar]] == 0) std::cout << reg << " " << ivar << std::endl;
      }
    }
    //    std::cout << std::endl;
    //    mvawJetsValue = reader->EvaluateMVA("BDT_wJets");
    mvawJetsValue = 0.;

    //This is a test thing to check whether the mva value changes
    if (false){
      float defValues [13] = {-999.,-500.,-100.,-50.,0.,1.,2.,4.,5.,7.,10.,50.,500.};
      for (int i = 0 ; i < 11; i++){
	  for (unsigned int ivar=0; ivar<varList[theChannel].size(); ivar++) {
	    if (proxyvars[theChannel][ivar] == defValues[i]) proxyvars[theChannel][ivar] = defValues[i+1];
	  
	}
	std::cout << defValues[i+1] << " " << reader[theChannel]->EvaluateMVA(proxyvars[theChannel],_bdtName[theChannel]) << std::endl;
      }
    }

    float mtw = std::sqrt(2*met*lepPt*(1-cos(metPhi-lepPhi)));

    float ogMVA = 0.;

    if (theChannel > -1  && theChannel < regionNames.size()){

      if (theChannel == 2 || theChannel == 10){ //Reset some variables if we are in the 2j case
	float defVal = 999.;
	if (!_isEle){
	  proxyvars[theChannel][5] = 100.;	  
	  proxyvars[theChannel][6] = 0.;
	}
	proxyvars[theChannel][3] = defVal;
	proxyvars[theChannel][4] = defVal;
	proxyvars[theChannel][7] = defVal;


      }
      if (theChannel == 0 || theChannel == 8){
	proxyvars[theChannel][4] = -999.;
	proxyvars[theChannel][5] = -999.;
      }

      if (doMVA) {
	//std::cout << theChannel << " " << reader[theChannel] << std::endl;
	mvaValue = reader[theChannel]->EvaluateMVA(proxyvars[theChannel],_bdtName[theChannel]);
	ogMVA = mvaValue;
	//if (theChannel == 0 || theChannel == 2 || theChannel == 1) std::cout << std::endl << "Channel! " << theChannel << " " << mvaValue;
      }
      else {
	mvaValue = 0.;
	theChannel = 0;
      }

      fillHists(sampleName,proxyvars[theChannel],mvaValue,mvawJetsValue,theweight,met,mtw,jet1Pt,theChannel);
     
      //Now fill the weight-based systematic histograms
      if (!isData){
	fillHists(sampleName+"_PU_up",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * (puWeightUp/puWeight),met,mtw,jet1Pt,theChannel);
	fillHists(sampleName+"_PU_down",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * (puWeightDown/puWeight),met,mtw,jet1Pt,theChannel);
	if (lepSFWeight > 0.){
	  fillHists(sampleName+"_LSF_up",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * (lepSFWeightUp/lepSFWeight),met,mtw,jet1Pt,theChannel);
	  fillHists(sampleName+"_LSF_down",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * (lepSFWeightDown/lepSFWeight),met,mtw,jet1Pt,theChannel);
	}
	fillHists(sampleName+"_Trig_up",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * (trigSFWeightUp/trigSFWeight),met,mtw,jet1Pt,theChannel);
	fillHists(sampleName+"_Trig_down",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * (trigSFWeightDown/trigSFWeight),met,mtw,jet1Pt,theChannel);
	if (_useIterFit){
	  bSysts = calculatebTagSyst(bWeight,{bWeighthfs1Up,bWeighthfs1Down,bWeighthfs2Up,bWeighthfs2Down,bWeightcferr1Up,bWeightcferr1Down,bWeightcferr2Up,bWeightcferr2Down,bWeightjerUp,bWeightjerDown,bWeightlfUp,bWeightlfDown});
	  //      std::cout << "btag systs: " << std::get<0>(bSysts) << " " << std::get<1>(bSysts) << std::endl;
	  fillHists(sampleName+"_bTag_up",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * std::get<0>(bSysts),met,mtw,jet1Pt,theChannel);
	  fillHists(sampleName+"_bTag_down",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * std::get<1>(bSysts),met,mtw,jet1Pt,theChannel);
	  mistagSysts = calculatebTagSyst(mistagWeight,{mistagWeighthfs1Up,mistagWeighthfs1Down,mistagWeighthfs2Up,mistagWeighthfs2Down,mistagWeightcferr1Up,mistagWeightcferr1Down,mistagWeightcferr2Up,mistagWeightcferr2Down,mistagWeightjerUp,mistagWeightjerDown,mistagWeightlfUp,mistagWeightlfDown});
	  //      std::cout << "mistag systs: " << std::get<0>(mistagSysts) << " " << std::get<1>(mistagSysts) << std::endl;
	  fillHists(sampleName+"_mistag_up",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * std::get<0>(mistagSysts),met,mtw,jet1Pt,theChannel);
	  fillHists(sampleName+"_mistag_down",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * std::get<1>(mistagSysts),met,mtw,jet1Pt,theChannel);
	}
	else{
	  fillHists(sampleName+"_bTag_up",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * (bWeightUp/bWeight),met,mtw,jet1Pt,theChannel);
	  fillHists(sampleName+"_bTag_down",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * (bWeightDown/bWeight),met,mtw,jet1Pt,theChannel);
	  fillHists(sampleName+"_mistag_up",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * ( mistagWeightUp/mistagWeight),met,mtw,jet1Pt,theChannel);
	  fillHists(sampleName+"_mistag_down",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight * (mistagWeightDown/mistagWeight),met,mtw,jet1Pt,theChannel);
	}
	fillHists(sampleName+"_PDF_up",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight*pdfUp,met,mtw,jet1Pt,theChannel);
	fillHists(sampleName+"_PDF_down",proxyvars[theChannel],mvaValue,mvawJetsValue,theweight*pdfDown,met,mtw,jet1Pt,theChannel);
      }
    }
	
	
	//Here we will need to do each of the JES uncertainties. This code is gonna be gross, I'm sure. Actually it doesn't look too bad?
	//      for (unsigned int jesInd = 0; jesInd < treevarsJetShifts[0].size(); jesInd++){
	//	std::cout << jesInd << std::endl;
	//}
    //    continue;
    if (!isData){
      //      std::cout << "Starts the jes l;oop" << std::endl;
      //print out the channel for debugging
      for (unsigned int jesInd = 0; jesInd < _varsInBDT_JESShifts.begin()->second->size(); jesInd++){
	if (jesChannels->at(jesInd) < 0 || jesChannels->at(jesInd) > 7 || jesChannels->at(jesInd) > reader.size()) continue;
	int jesChannel = jesChannels->at(jesInd);
	if (_splitBarrelEndcap && !isBarrel) jesChannel += 8;
	//	if (theChannel != jesChannel) { std::cout << std::endl << theChannel << " " << jesChannel << " ";}
	for (unsigned int varInd = 0; varInd < varList[jesChannel].size(); varInd ++){
	    proxyvars[jesChannel][varInd] = _varsInBDT_JESShifts[varList[jesChannel][varInd]]->at(jesInd);
	    if (proxyvars[jesChannel][varInd] < -995) proxyvars[jesChannel][varInd] = _varsInBDTs[varList[jesChannel][varInd]];
	    if (proxyvars[jesChannel][varInd] < -100.) proxyvars[jesChannel][varInd] = _emptyVarValue;
	    //	    if (isnan(proxyvars[jesChannel][varInd])) std::cout << jesInd << " " << varInd << std::endl;
	    //treevarsJetShifts[jesChannel][varInd]->at(jesInd);
	}
	if (jesChannel == 2 || jesChannel == 10){ //Reset some variables if we are in the 2j case
	  
	  //proxyvars[jesChannel][6] = -999.;                                     	
	  float defVal = 999.;
	  if (!_isEle){
	    proxyvars[jesChannel][5] = 100.;	  
	    proxyvars[jesChannel][6] = 0.;
	  }
	  proxyvars[jesChannel][3] = defVal;
	  proxyvars[jesChannel][4] = defVal;
	  proxyvars[jesChannel][7] = defVal;
                 
	  //	  proxyvars[jesChannel][7] = 999.;                                                      
	  //proxyvars[jesChannel][3] = 999.;                                                      
	  //proxyvars[jesChannel][4] = 999.;                                                                                                                             proxyvars[jesChannel][5] = 999.;                  
	  
	}                                                                                        

	if (jesChannel == 0 || jesChannel == 8){
	  proxyvars[jesChannel][4] = -999.; 
	  proxyvars[jesChannel][5] = -999.; 
	}

	if (doMVA)  {
	  mvaValue = reader[jesChannel]->EvaluateMVA(proxyvars[jesChannel],_bdtName[jesChannel]);
	  /*	  if (jesInd == 23 && fabs(ogMVA - mvaValue) > 0.001){ 
	    std::cout << jesInd << " " << theChannel << " " << jesChannel << " " << ogMVA << " " << mvaValue << std::endl;
	    for (unsigned int varInd = 0; varInd < varList[jesChannel].size(); varInd ++){
	      std::cout << varList[jesChannel][varInd] << ": " << _varsInBDTs[varList[jesChannel][varInd]] << " " << proxyvars[jesChannel][varInd] << " 23jet: " << _varsInBDT_JESShifts[varList[jesChannel][varInd]]->at(13) << " " << _varsInBDT_JESShifts[varList[jesChannel][varInd]]->at(23) << std::endl;
	      for (unsigned int jesInd2 = 0; jesInd2 < jesChannels->size(); jesInd2++){
		std::cout << " "  << jesInd2 << " " << _varsInBDT_JESShifts[varList[jesChannel][varInd]]->at(jesInd2);
	      }
	      std::cout << std::endl;

	      }
	      }*/
	  //std::cout << " " << mvaValue;
	}
	else {                                                  
	  mvaValue = 0.;                                        
	  theChannel = 0;                                       
	}
	//	float jet1PtJES = jet1Pt * (jesPtr[jesInd].at(0)/jesPtr[58].at(0));
	//if (jesInd > 47) jet1PtJES = jet1Pt*(jesPtr[jesInd].at(0)/jesPtr[59].at(0));
	float jesShiftedJet1Pt = jet1PtJES->at(jesInd);
	fillHists(sampleName+"_JetShifts_"+std::to_string(jesInd),proxyvars[jesChannel],mvaValue,mvawJetsValue,theweight,met,mtw,jesShiftedJet1Pt,jesChannel);
      }
    }
  }
}

void mvaTool::createHists(TString sampleNameBase){

  TString sampleName = sampleNameBase;
  
  for (unsigned int region = 0; region < regionNames.size(); region++){

    TString sampleName = sampleNameBase + regionNames[region];

    std::vector<TH1F*> histovect;
    std::vector<TH1F*> bdtVect;

    //Make a histogram per variable
    std::vector<TString> varNames = varList[region];
    for (unsigned int i = 0; i<varNames.size(); i++){
      int nbins = 1;
      double xmin = -1000;
      double xmax = 1000;
      if (varNames[i] ==  "M_DeltaRBJetLepton") {nbins = 20; xmin = 0; xmax = 6.;};
      if (varNames[i] ==  "M_DeltaRlightjets") {nbins = 20; xmin = 0; xmax = 6.;};
      if (varNames[i] == "M_topMass2_lep") {nbins = 20; xmin = 0.; xmax = 600.;};
      if (varNames[i] == "M_Pt_AllJets2040") {nbins = 20; xmin = 0; xmax = 160;};
      if (varNames[i] == "M_Pt_Lepton") {nbins = 20; xmin = 0.; xmax = 300.;};
      if (varNames[i] == "M_Pt_AllJetsLeptonMET") {nbins = 20; xmin = 0.; xmax = 300.;};
      if (varNames[i] == "M_E_Jet1MET") {nbins = 20; xmin = 0.; xmax = 300.;};
      if (varNames[i] == "M_Jet1Jet2Jet3_Centrality") {nbins = 20; xmin = 0.; xmax = 1.;};
      if (varNames[i] == "M_DeltaRLeptonJet1") {nbins = 20; xmin = 0.; xmax = 6.;};
      if (varNames[i] == "M_nJet3040e24") {nbins = 10; xmin = 0.; xmax = 10.;};
      if (varNames[i] == "M_nJet2040") {nbins = 10; xmin = 0.; xmax = 10.;};
      if (varNames[i] == "M_Mass_Jet1Jet2Jet3LeptonMET") {nbins = 20; xmin = 0.; xmax = 1500.;};
      if (varNames[i] == "M_Pt_BJet1_4000") {nbins = 20; xmin = 40.; xmax = 200.;};
      if (varNames[i] == "M_Pt_Jet1_2040") {nbins = 20; xmin = 20.; xmax = 40.;};
      if (varNames[i] == "M_hadronicWmass") {nbins = 20; xmin = 0.; xmax = 500.;};
      if (varNames[i] == "M_hadronicWEta") {nbins = 20; xmin = -6.; xmax = 6.;};
      if (varNames[i] == "M_DeltaRBJethadronicW") {nbins = 20; xmin = 0.; xmax = 6.;};
      if (varNames[i] == "M_Pt_sys") {nbins = 20; xmin = 0.; xmax = 300.;};
      if (varNames[i] == "M_HT") {nbins = 20; xmin = 0.; xmax = 900.;};
      if (varNames[i] == "M_TMass_Jet1Jet2Jet3") {nbins = 20; xmin = 0.; xmax = 1400.;};
      if (varNames[i] == "M_cosThetaStar") {nbins = 20; xmin = -1.; xmax = 1.;};
      if (varNames[i] == "M_cosThetaStar_lepOnly") {nbins = 20; xmin = -1.; xmax = 1.;};
      if (varNames[i] == "lightJet1CSV") {nbins = 20; xmin = 0.; xmax = 1.;};
      if (varNames[i] == "lightJet2CSV") {nbins = 20; xmin = 0.; xmax = 1.;};
      if (varNames[i] == "M_DeltaPhiJet1Jet2") {nbins = 20; xmin = 0.; xmax = 3.3;};
      if (varNames[i] == "M_DeltaRJet1Jet2") {nbins = 20; xmin = 0.; xmax = 6.;};
      if (varNames[i] == "M_DeltaRWlvJet2") {nbins = 20; xmin = 0.; xmax = 6.;};
      if (varNames[i] == "M_E_AllJets") {nbins = 20; xmin = 0.; xmax = 700.;};
      if (varNames[i] == "M_E_Jet1Jet2Jet3MET") {nbins = 20; xmin = 0.; xmax = 1000.;};
      if (varNames[i] == "M_E_Jet2Jet3") {nbins = 20; xmin = 0.; xmax = 400.;};
      if (varNames[i] == "M_HT") {nbins = 20; xmin = 0.; xmax = 600.;};
      if (varNames[i] == "M_Mass_AllJets") {nbins = 20; xmin = 0.; xmax = 1000.;};
      if (varNames[i] == "M_Mass_AllJetsMET") {nbins = 20; xmin = 0.; xmax = 1000.;};
      if (varNames[i] == "M_Mass_Jet1Jet2") {nbins = 20; xmin = 0.; xmax = 700.;};
      if (varNames[i] == "M_Mass_Jet1Jet3") {nbins = 20; xmin = 0.; xmax = 600.;};
      if (varNames[i] == "M_Pt_AllJets2040") {nbins = 20; xmin = 0.; xmax = 100.;};
      if (varNames[i] == "M_Pt_Jet1LeptonMET") {nbins = 20; xmin = 0.; xmax = 200.;};
      if (varNames[i] == "M_Pt_Jet1Jet2Lepton") {nbins = 20; xmin = 0.; xmax = 300.;};
      if (varNames[i] == "M_Pt_LeptonJet1") {nbins = 20; xmin = 0.; xmax = 300.;};
      if (varNames[i] == "M_Pt_Jet1_2030") {nbins = 20; xmin = 20.; xmax = 30.;};


      TH1F* histo = new TH1F((varNames[i] + "_" + sampleName).Data(), (varNames[i] + "_" + sampleName).Data(),nbins,xmin,xmax);
      histo->Sumw2();
      histovect.push_back(histo);
    }
    //Add in some plots for met and mtw for control/fitting
    TH1F* histomtw = new TH1F(("mTW_" + sampleName).Data(), ("mTW_" + sampleName).Data(),20,0.,200.);
    histomtw->Sumw2();
    histovect.push_back(histomtw);
    TH1F* histomet = new TH1F(("met_" + sampleName).Data(), ("met_" + sampleName).Data(),20,0.,200.);
    histomet->Sumw2();
    histovect.push_back(histomet);
    //jet pt which I'm gonna use for 2j1t region    
    TH1F* histjet1pt = new TH1F(("jet1Pt_" + sampleName).Data(), ("jet1Pt_" + sampleName).Data(), 10,30.,250.);
    histjet1pt->Sumw2();
    histovect.push_back(histjet1pt);
  

    /*    for (int nBins = 10; nBins <= 100; nBins+=10){
      TH1F* histo = new TH1F(("MVA_ttbar_bin"+std::to_string(nBins)+"_"+sampleName).Data(),("MVA_ttbar_bin"+std::to_string(nBins)+"_"+sampleName).Data(),nBins,-0.8,0.8);
      histo->Sumw2();
      bdtVect.push_back(histo);
      }*/
    int nBins = 1000;
    TH1F* histo = new TH1F(("MVA_ttbar_bin"+std::to_string(nBins)+"_"+sampleName).Data(),("MVA_ttbar_bin"+std::to_string(nBins)+"_"+sampleName).Data(),nBins,-0.8,0.8);
    histo->Sumw2();
    bdtVect.push_back(histo);
    TH1F* histo2 = new TH1F(("MVA_wJets_"+sampleName).Data(),("MVA_wJets_"+sampleName).Data(),20,-0.3,0.3);
    histo2->Sumw2();
    histovect.push_back(histo2);

    TH2F* bdt2D = new TH2F(("MVA_2D_"+sampleName).Data(),("MVA_2D_"+sampleName).Data(),20,-0.8,0.8,20,-0.3,0.3);
    bdt2D->Sumw2();
    the2DHistoMap[sampleNameBase].push_back(bdt2D);

    theHistoMap[sampleNameBase].push_back(histovect);
    bdtHistoMap[sampleNameBase].push_back(bdtVect);
  }
  //  std::cout << "hist maps are this long: " << the2DHistoMap[sampleNameBase].size() << " " << theHistoMap[sampleNameBase].size() << " " << bdtHistoMap[sampleNameBase].size() << std::endl;
}

void mvaTool::fillHists(TString sampleName, std::vector<float> treevars, double mvaValue, double mvawJetsValue, double theweight, float met, float mtw, float jet1pt, int theChannel){

  std::vector<std::vector<TH1F*> > histovect = theHistoMap[sampleName];
  std::vector<std::vector<TH1F*> > bdtVector = bdtHistoMap[sampleName];

  for (unsigned int i=0; i < varList[theChannel].size(); i++) histovect[theChannel][i]->Fill(treevars[i],theweight);
  for (unsigned int j = 0; j < bdtVector[theChannel].size(); j++) bdtVector[theChannel][j]->Fill(mvaValue,theweight);
   
  histovect[theChannel][histovect[theChannel].size() - 4]->Fill(mtw,theweight);
  histovect[theChannel][histovect[theChannel].size() - 3]->Fill(met,theweight);
  histovect[theChannel][histovect[theChannel].size() - 2]->Fill(jet1pt,theweight);

  if (histovect[theChannel].size() > 1){
    //    histovect[histovect.size()-2]->Fill(mvaValue,theweight);
     histovect[theChannel][histovect[theChannel].size()-1]->Fill(mvawJetsValue,theweight);
  }
  the2DHistoMap[sampleName][theChannel]->Fill(mvaValue,mvawJetsValue,theweight);

}

void mvaTool::saveHists(std::vector<TFile*> outFile){
  for (unsigned int i = 0; i < regionNames.size(); i++){
    outFile[i]->cd();
    string tmpName;
    for (auto histoMapElement: theHistoMap){
      for (auto hist: (histoMapElement.second)[i]) {
	tmpName = hist->GetName();
	int pos = tmpName.find(regionNames[i]);
	hist->SetTitle(tmpName.substr(0,pos).c_str());
	hist->Write(tmpName.substr(0,pos).c_str());
      }
    }
    for (auto histoMapElement: bdtHistoMap){
      for (auto hist: (histoMapElement.second)[i]) {
	tmpName = hist->GetName();
	int pos = tmpName.find(regionNames[i]);
	hist->Write(tmpName.substr(0,pos).c_str());
	hist->SetTitle(tmpName.substr(0,pos).c_str());
      }
    }
    
    //    for (auto hist2D: the2DHistoMap){
    // (hist2D.second)[i]->Write();
    //}
  }
}

void mvaTool::setbTagVars(TChain * theTree){
  std::cout << "[setbTagVars] Entered setbTagVars" << std::endl;
  //  std::vector<TString> bTagSystNames = {"cferr1","cferr2","hfstats1","hfstats2","jes","lf"};
  //  std::vector<TString> bTagSystNames = {"cferr1","cferr2","hfstats2"};
  std::vector<TString> bTagSystNames = {"jes","hfstats1","lf","hfstats2","cferr1"};
  std::vector<TString> upDown = {"up","down"};
  bTagSysts.push_back(0.);
  std::cout << "[setbTagVars] Assigned central variable " << std::endl;
  theTree->SetBranchAddress( "bWeight_central", &(bTagSysts[0]) );
  std::cout << "[setbTagVars] Assigned central variable to tree " << std::endl;
  int i = 1;
  for (auto bName: bTagSystNames){
    std::cout << "[setbTageVars] Booking " << bName << " variable: " << std::endl;
    for (auto ud: upDown){
      bTagSysts.push_back(0.);
      std::cout << ("     [setbTageVars] bWeight_"+ud+"_"+bName).Data() << std::endl;
      theTree->SetBranchAddress( ("bWeight_"+ud+"_"+bName).Data(), &(bTagSysts[i]));
      i++;
    }
  }
}

std::tuple<float,float> mvaTool::calculatebTagSyst(float bCentral, std::vector<float> bWeightsSyst){
  float systUp = 0., systDown = 0.;
  for (unsigned int i = 1; i < bWeightsSyst.size(); i++){
      if (i%2 == 0){//Down systs
	systDown += pow((bWeightsSyst[i]/bCentral)-1,2);
	//	std::cout << (bWeightsSyst[i]/bCentral)-1 << ",";
      }
      else{
	systUp += pow((bWeightsSyst[i]/bCentral)-1,2);
	//std::cout << (bWeightsSyst[i]/bCentral)-1 << ",";
      }
    }
  //  std::cout << std::endl;
  return std::make_tuple(1+sqrt(systUp),1-sqrt(systDown));
}

/*std::tuple<float,float> mvaTool::calculateMistagSyst(float bCentral, std::vector<float> bWeightsSyst){
  float systUp = 0., systDown = 0.;
  for (unsigned int i = 1; i < bWeightsSyst.size(); i++){
      if (i%2 == 0){//Down systs
	systDown += pow((bWeightsSyst[i]/bCentral)-1,2);
      }
      else{
	systUp += pow((bWeightsSyst[i]/bCentral)-1,2);
      }
    }
  return std::make_tuple(1+sqrt(systUp),1-sqrt(systDown));
  }*/
  
void mvaTool::makeStatVariationHists(TString sampleName, std::vector<TFile *> outputFile){
  for (unsigned int i = 0; i < regionNames.size(); i++){
    TH1F * theDefaultMVA = theHistoMap[sampleName][i].back(); //We only need to do this to nominal.
    outputFile[i]->cd();
    std::ostringstream histNameStream;
    std::string histName;             
    
    for (int i = 1; i <= theDefaultMVA->GetXaxis()->GetNbins();i++){
      histNameStream.str("");
      histNameStream << "MVA_" << sampleName << "_statbin"<<i<<"_up";
      histName = histNameStream.str();
      TH1F* tempMVAUp = (TH1F*)theDefaultMVA->Clone(histName.c_str());
      histNameStream.str("");
      histNameStream << "MVA_" << sampleName << "_statbin"<<i<<"_down";
      histName = histNameStream.str();
      TH1F* tempMVADown = (TH1F*)theDefaultMVA->Clone(histName.c_str());
      float binToChange = theDefaultMVA->GetBinContent(i);
      //This is the bit I'm not sure of, but can edit later with any luck.
      float uncert = sqrt(binToChange);
      tempMVAUp->SetBinContent(i,binToChange+uncert);
      tempMVADown->SetBinContent(i,binToChange-uncert);
      tempMVAUp->Write();
      tempMVADown->Write();
    }
  }
}

void mvaTool::printVarList(){

  for (unsigned int reg = 0; reg < regionNames.size(); reg++){
    std::cout << "Var list for region " << regionNames[reg] << " is " << varList[reg].size() << " variables long" << std::endl;
    for (unsigned int i = 0; i < varList.size() ; i++){
      std::cout << varList[reg][i] << std::endl;
    }
  }
  std::cout << "Sys list is " << systlist.size() << " variables long" << std::endl;
  for (unsigned int i = 0; i < systlist.size() ; i++){
    std::cout << systlist[i] << std::endl;
  }
  std::cout << _useIterFit << " " << _channel  << " " << std::endl;
  for (unsigned int i = 0; i < regionNames.size() ; i++){
    std::cout << regionNames[i] << std::endl;
  }
}

