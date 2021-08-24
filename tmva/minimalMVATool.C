#define minimalMVATool_cxx
#include "minimalMVATool.h"

minimalMVATool::minimalMVATool(){

  //Start by initialising the list of variables we will base the MVA on
  initialiseVarList();
  //A few universal variables
  _treeName = "TNT/BOOM"; //The name of the TTree to read from the input
  _bdtName = "myBDT"; //Name of BDT - name something useful
  
  
}

void minimalMVATool::initialiseVarList(){

  //Put in the name of your variables here. Some examples are provided
  varList.push_back("M_Mass_Jet1Jet3");
  varList.push_back("M_HT");
  varList.push_back("M_DeltaRBJetLepton");
  varList.push_back("M_DeltaRBJethadronicW");
  varList.push_back("M_DeltaRWlvJet2");

  //This automatically sets up the proxy variables used in the training/reading
  for (auto var: varList){
    _varsInBDTs[var] = 0;
  }

}

//Train the BDT
void minimalMVATool::doTraining(TString signalFile, TString bkgFile, TString sampleName){
  
  //This needs to be done first
  TMVA::Tools::Instance();

  //Make the output of the training
  TString outfileName("training/Output.root");
  //TString outfileName("training/trainingFornote.root");
  TFile* outFile = TFile::Open(outfileName, "RECREATE");

  //Create the factory
  TMVA::Factory *factory = new TMVA::Factory("tWLepJet_training",outFile,"!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification");

  //Create a data loader
  TMVA::DataLoader loader{"loader"};
  
  //Name of the tree in your signal/background files
  TString treeName = "TNT/BOOM";

  //Make TChains for signal and background
  TChain signal(treeName);
  TChain bkg(treeName);

  //Set up the signalFile
  signal.Add(signalFile);
  
  //Set up the background sample. Add more files to this for multiple backgrounds
  bkg.Add(bkgFile);
  
  //Load the trees into the factory
  loader.AddSignalTree    (&signal,    1.);
  loader.AddBackgroundTree(&bkg,       1.);
  
  //Set up the input variables
  for (unsigned int i=0; i < varList.size(); i++) loader.AddVariable(varList.Data(),'F');

  //Name of the event weight branch in your trees
  TString eventWeightBranchName = "EventWeight";

  //And put it into the factory
  loader.SetSignalWeightExpression(eventWeightBranchName);
  loader.SetBackgroundWeightExpression(eventWeightBranchName);

  //If you have any cuts to apply to the input, load them here as strings
  TCut mycutsSig = "";
  TCut mycutsBkg = "";

  //Prepares the training and test trees
  loader.PrepareTrainingAndTestTree( mycutsSig, mycutsBkg,"nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

  //Book the mva methods you want to train here. This can be many types, refer to TMVA manual for information.
  //Note that many methods can be booked with different configurations
  factory->BookMethod(&loader,TMVA::Types::kBDT,_bdtName);

  // Train MVAs using the set of training events                  
  factory->TrainAllMethods();

  // ---- Evaluate all MVAs using the set of test events          
  factory->TestAllMethods();

  // ----- Evaluate and compare performance of all configured MVAs
  factory->EvaluateAllMethods();

  outFile->Close();

  delete factory;

}

void minimalMVATool::doReading(TString inFile, TString outFile){
  
  //Create instance of TMVA
  TMVA::Tools::Instance();

  //A set of proxy variables to read out the mva information
  std::vector<float> proxyvars;

  //MVA reader
  reader = new TMVA::Reader( "!Color:!Silent" );

  //Add each variable from the varList
  for (unsigned int i=0; i<varList.size(); i++){
      std::cout << "[Variable loop] Adding variable: " << varList[reg][i].Data() << std::endl;
      proxyvars.push_back(0);
      //      std::cout << "adding to reader" << std::endl;
      reader->AddVariable (varList[i].Data(), &(proxyvars[i]) );
  }

  //Reads in the bdtWeights
  reader->BookMVA(_bdtName, "loader/weights/"+_bdtName + ".weights.xml" );

  //Carry out the reading for a single MC sample
  processMCSample(inFile,outFile,proxyvars,sampleName);
  
  std::cout << "Finished processing " << sampleName << std::endl;
  
}

//Do the reading for a single mc sample
void minimalMVATool::processMCSample(TString inFile, TString outFile, std::vector<float>  proxyvars, TString sampleName){

  //Set up the tree to read
  TChain* theTree = new TChain(_treeName);
  
  //Add input file to the tree
  theTree->Add(inFile);

  //Make the output file
  TFile *theoutputfile = new TFile( outFile, "RECREATE");
  
  //create histograms of the variables and the bdt output
  createHists(sampleName);
  
  loopInSample(theTree,sampleName,proxyvars);
  //makeStatVariationHists(sampleName,theoutputfiles); //We do this in processMCSample so that we have the output file to save the stat variations into.
  
  saveHists(theoutputfile);
    
  outputfile->Close();
  delete outputfile;
}

//Loop over the events in the desired sample
void minimalMVATool::loopInSample(TTree* theTree, TString sampleName, std::vector<std::vector<float> > proxyvars, bool isData, bool doMVA){

  //This makes sure the tree is initialised and finds the correct vectors. This hasnm't always been here, and for a long time it worked without it. Why has this changed now? I HAVE NO IDEA
  theTree->GetEntry(0);

  //assign all the BDT variables for each region
  for (auto vars: _varsInBDTs){
    theTree->SetBranchAddress( vars.first.Data(), &(_varsInBDTs[vars.first]));
  }
   
  theweight=0.;
  theTree->SetBranchAddress( "EventWeight", &theweight );

  //Debug vars
  int eventNumber = 0;
  theTree->SetBranchAddress ( "EVENT_event", &eventNumber);

  std::cout << "[loopInSample] Finished assigning variables" << std::endl;

  if (theTree ==0) cout << "No TTree found for " << sampleName << "!" << std::endl;

  //Loop over the events
  int loopMax = theTree->GetEntries();
  
  //debug mode
  //loopMax = 2000;

  for (int i = 0; i < loopMax; i++){

    if (i%500 == 0){
      printf ("Processing event %i\r", i);
      fflush(stdout);
    }
    theTree->GetEntry(i);

    //Get the variables for the MVA from the tree
    for (unsigned int ivar=0; ivar<varList.size(); ivar++) {
      proxyvars[ivar] = _varsInBDTs[varList[ivar]];
    }

    //Read the MVA value for the event
    mvaValue = reader->EvaluateMVA(proxyvars,_bdtName);

    //Fill the output histograms
    fillHists(sampleName,proxyvars,mvaValue,theweight);
    
  }
}

void minimalMVATool::createHists(TString sampleNameBase){

  TString sampleName = sampleNameBase;
  
  TH1F* histovect;
  TH1F* bdtVect;

  //Make a histogram per variable
  for (unsigned int i = 0; i<varList.size(); i++){
    int nbins = 1;
    double xmin = -1000;
    double xmax = 1000;
    //customise depending on the variable. Please add your own, or not. I'm not your boss (probably?)
    if (varList[i] ==  "M_DeltaRBJetLepton") {nbins = 20; xmin = 0; xmax = 6.;};
    if (varList[i] ==  "M_DeltaRlightjets") {nbins = 20; xmin = 0; xmax = 6.;};
    if (varList[i] == "M_topMass2_lep") {nbins = 20; xmin = 0.; xmax = 600.;};
    if (varList[i] == "M_Pt_AllJets2040") {nbins = 20; xmin = 0; xmax = 160;};
    if (varList[i] == "M_Pt_Lepton") {nbins = 20; xmin = 0.; xmax = 300.;};
    if (varList[i] == "M_Pt_AllJetsLeptonMET") {nbins = 20; xmin = 0.; xmax = 300.;};
    if (varList[i] == "M_E_Jet1MET") {nbins = 20; xmin = 0.; xmax = 300.;};
    if (varList[i] == "M_Jet1Jet2Jet3_Centrality") {nbins = 20; xmin = 0.; xmax = 1.;};
    if (varList[i] == "M_DeltaRLeptonJet1") {nbins = 20; xmin = 0.; xmax = 6.;};
    if (varList[i] == "M_nJet3040e24") {nbins = 10; xmin = 0.; xmax = 10.;};
    if (varList[i] == "M_nJet2040") {nbins = 10; xmin = 0.; xmax = 10.;};
    if (varList[i] == "M_Mass_Jet1Jet2Jet3LeptonMET") {nbins = 20; xmin = 0.; xmax = 1500.;};
    if (varList[i] == "M_Pt_BJet1_4000") {nbins = 20; xmin = 40.; xmax = 200.;};
    if (varList[i] == "M_Pt_Jet1_2040") {nbins = 20; xmin = 20.; xmax = 40.;};
    if (varList[i] == "M_hadronicWmass") {nbins = 20; xmin = 0.; xmax = 500.;};
    if (varList[i] == "M_hadronicWEta") {nbins = 20; xmin = -6.; xmax = 6.;};
    if (varList[i] == "M_DeltaRBJethadronicW") {nbins = 20; xmin = 0.; xmax = 6.;};
    if (varList[i] == "M_Pt_sys") {nbins = 20; xmin = 0.; xmax = 300.;};
    if (varList[i] == "M_HT") {nbins = 20; xmin = 0.; xmax = 900.;};
    if (varList[i] == "M_TMass_Jet1Jet2Jet3") {nbins = 20; xmin = 0.; xmax = 1400.;};
    if (varList[i] == "M_cosThetaStar") {nbins = 20; xmin = -1.; xmax = 1.;};
    if (varList[i] == "M_cosThetaStar_lepOnly") {nbins = 20; xmin = -1.; xmax = 1.;};
    if (varList[i] == "lightJet1CSV") {nbins = 20; xmin = 0.; xmax = 1.;};
    if (varList[i] == "lightJet2CSV") {nbins = 20; xmin = 0.; xmax = 1.;};
    if (varList[i] == "M_DeltaPhiJet1Jet2") {nbins = 20; xmin = 0.; xmax = 3.3;};
    if (varList[i] == "M_DeltaRJet1Jet2") {nbins = 20; xmin = 0.; xmax = 6.;};
    if (varList[i] == "M_DeltaRWlvJet2") {nbins = 20; xmin = 0.; xmax = 6.;};
    if (varList[i] == "M_E_AllJets") {nbins = 20; xmin = 0.; xmax = 700.;};
    if (varList[i] == "M_E_Jet1Jet2Jet3MET") {nbins = 20; xmin = 0.; xmax = 1000.;};
    if (varList[i] == "M_E_Jet2Jet3") {nbins = 20; xmin = 0.; xmax = 400.;};
    if (varList[i] == "M_HT") {nbins = 20; xmin = 0.; xmax = 600.;};
    if (varList[i] == "M_Mass_AllJets") {nbins = 20; xmin = 0.; xmax = 1000.;};
    if (varList[i] == "M_Mass_AllJetsMET") {nbins = 20; xmin = 0.; xmax = 1000.;};
    if (varList[i] == "M_Mass_Jet1Jet2") {nbins = 20; xmin = 0.; xmax = 700.;};
    if (varList[i] == "M_Mass_Jet1Jet3") {nbins = 20; xmin = 0.; xmax = 600.;};
    if (varList[i] == "M_Pt_AllJets2040") {nbins = 20; xmin = 0.; xmax = 100.;};
    if (varList[i] == "M_Pt_Jet1LeptonMET") {nbins = 20; xmin = 0.; xmax = 200.;};
    if (varList[i] == "M_Pt_Jet1Jet2Lepton") {nbins = 20; xmin = 0.; xmax = 300.;};
    if (varList[i] == "M_Pt_LeptonJet1") {nbins = 20; xmin = 0.; xmax = 300.;};
    if (varList[i] == "M_Pt_Jet1_2030") {nbins = 20; xmin = 20.; xmax = 30.;};

    //Here we make the actual plot
    TH1F* histo = new TH1F((varList[i] + "_" + sampleName).Data(), (varList[i] + "_" + sampleName).Data(),nbins,xmin,xmax);
    histo->Sumw2();
    histovect.push_back(histo);
  }

  int nBins = 1000;
  TH1F* histo = new TH1F(("MVA_"+_bdtName_+"_"+sampleName).Data(),("MVA_"+_bdtName_+"_"+sampleName).Data(),nBins,-1.,1.);
  histo->Sumw2();
  bdtVect.push_back(histo);

  theHistoMap[sampleNameBase] = histovect;
  bdtHistoMap[sampleNameBase] = bdtVect;
}

//fill the histograms
void minimalMVATool::fillHists(TString sampleName, std::vector<float> treevars, double mvaValue, double theweight){

  std::vector<std::vector<TH1F*> > histovect = theHistoMap[sampleName];
  std::vector<std::vector<TH1F*> > bdtVector = bdtHistoMap[sampleName];

  for (unsigned int i=0; i < varList.size(); i++) histovect[i]->Fill(treevars[i],theweight);
  for (unsigned int j = 0; j < bdtVector.size(); j++) bdtVector[j]->Fill(mvaValue,theweight);
}

//save the histograms
void minimalMVATool::saveHists(std::vector<TFile*> outFile){
  for (auto histoMapElement: theHistoMap){
    for (auto hist: (histoMapElement.second)) {
      tmpName = hist->GetName();
      hist->SetTitle(tmpName);
      hist->Write(tmpName);
    }
  }
  for (auto histoMapElement: bdtHistoMap){
    for (auto hist: (histoMapElement.second)) {
      tmpName = hist->GetName()
      hist->Write(tmpName);
      hist->SetTitle(tmpName);
    }
  }
}

