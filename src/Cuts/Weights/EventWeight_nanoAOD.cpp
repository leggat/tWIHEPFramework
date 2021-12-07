/******************************************************************************
 * EventWeight_nanoAOD.hpp                                                            *
 *                                                                            *
 * Weight events in the histograms                                            *
 *                                                                            *
 * This class is derived from the BaseCut Class and simply                    *
 * cuts on if the event has reconstructed objects                             *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of AnalysisMain class                              *
 *    EventWeight_nanoAOD()                     -- Parameterized Constructor          *
 *    ~EventWeight_nanoAOD()                    -- Destructor                         *
 *    BookHistogram()                   -- Book histograms of weights         *
 *    Apply()                           -- Apply weight and fill histograms   *
 *    GetCutName()                      -- Returns "EventWeight_nanoAOD"              *
 *                                                                            *
 * Private Data Members of AnalysisMain class                                 *
 *    myTH1F* _hTreeWeight              -- Histogram of Input Tree Weights    *
 *    myTH1F* _hGlobalWeight            -- Histogram of Global Weights        *
 *    myTH1F* _hOutputWeight            -- Histogram of Output Weights        *
 *                                                                            *
 * History                                                                    *
 *     28 Apr 2005 - Created by R. Schwienhorst at D0                         *
 *      7 Nov 2006 - Modified by R. Schwienhorst for ATLAS                    *
 *      9 Nov 2006 - Modified by P. Ryan - cleanup and reorganized            *
 *     11 Sep 2007 - RS: Add MCatNLO weight                                   *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Weights/EventWeight_nanoAOD.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <tuple>

using namespace std;


/******************************************************************************
 * EventWeight_nanoAOD::EventWeight_nanoAOD(EventContainer *obj)                              *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Particle class                                                     *
 * Output: None                                                               *
 ******************************************************************************/
EventWeight_nanoAOD::EventWeight_nanoAOD(EventContainer *EventContainerObj,Double_t TotalMCatNLOEvents,const std::string& MCtype, Bool_t pileup, Bool_t bWeight, Bool_t useLeptonSFs, Bool_t usebTagReshape, Bool_t doIterFitbTag, Bool_t verbose, TString muonsToUse, TString electronsToUse ):
  _useLeptonSFs(useLeptonSFs),
  _usebTagReshape(usebTagReshape),
  _verbose(verbose),
  _doIterFitbTag(doIterFitbTag),
  _muonsForLepWeight(muonsToUse),
  _electronsForLepWeight(electronsToUse)
{

  TEnv* conf = EventContainerObj -> GetConfig();

  //Find the number of events in the file
  _totalMCatNLOEvents = 0;
  SetEventContainer(EventContainerObj);
  if(MCtype.find("UseTotalEvtFromFile")<=MCtype.size()){
    Int_t sNumber = EventContainerObj -> GetSourceNumber();
    stringstream strNumber;
    strNumber<<"Events.Source."<<sNumber;
    Double_t totMCnloEvt = conf -> GetValue(strNumber.str().c_str(), -1.);

    SetTotalMCatNLOEvents(totMCnloEvt);
  } else {
    SetTotalMCatNLOEvents(TotalMCatNLOEvents);
  }

  //Setting up pileup reweighting
  setPileUpSyst(false);
  if(pileup){
    setPileUpWgt(true);
    TFile* dataPVFile = TFile::Open(conf -> GetValue("Include.dataPVFile","null"),"READ");
    std::cout << "Getting data pileup info from " << conf -> GetValue("Include.dataPVFile","null") << std::endl;
    _dataPV = (TH1F*)dataPVFile->Get("pileup");
    _dataPV->SetDirectory(0);
    _dataPV->Scale(1./_dataPV->Integral());
    dataPVFile->Close();
    delete dataPVFile;
    
    TFile* mcPVFile = TFile::Open(conf -> GetValue("Include.mcPVFile","null"),"READ");    
    std::cout << "Getting MC pileup info from " << conf -> GetValue("Include.mcPVFile","null") << std::endl;
    _mcPV = (TH1F*)mcPVFile->Get("pileup");
    _mcPV->SetDirectory(0);
    _mcPV->Scale(1./_mcPV->Integral());
    mcPVFile->Close();
    delete mcPVFile;

    //Load in the pileup distributions with the min bias x-section altered for systematic studies
    string minBiasFileName = conf -> GetValue("Include.minBiasUp","null");
    if (minBiasFileName != "null"){
      TFile* minBiasUpFile = TFile::Open(minBiasFileName.c_str(),"READ");
      setPileUpSyst(true);
      _minBiasUpPV = (TH1F*)minBiasUpFile->Get("pileup");
      _minBiasUpPV->SetDirectory(0);
      _minBiasUpPV->Scale(1./_minBiasUpPV->Integral());
      minBiasUpFile->Close();
      delete minBiasUpFile;
    }

    string minBiasDownFileName = conf -> GetValue("Include.minBiasDown","null");
    if (minBiasDownFileName != "null"){    
      TFile* minBiasDownFile = TFile::Open(minBiasDownFileName.c_str(),"READ");
      _minBiasDownPV = (TH1F*)minBiasDownFile->Get("pileup");
      _minBiasDownPV->SetDirectory(0);
      _minBiasDownPV->Scale(1./_minBiasDownPV->Integral());
      minBiasDownFile->Close();
      delete minBiasDownFile;
    }

  }
  else setPileUpWgt(false);

  string bTagEfficFileName = conf -> GetValue("Include.bTagEfficFile","null");
  if (bTagEfficFileName != "null"){
    setEfficbTag();
    std::cout << "Doing efficiency based b-tags!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << isEfficbTag() << std::endl;
    TFile* bTagEfficFile = TFile::Open(bTagEfficFileName.c_str(),"READ");
    _bFlavEffic = (TH1F*)bTagEfficFile->Get("bFlavEfficiency");
    _bFlavEffic->SetDirectory(0);
    _cFlavEffic = (TH1F*)bTagEfficFile->Get("cFlavEfficiency");
    _cFlavEffic->SetDirectory(0);
    _lightFlavEffic = (TH1F*)bTagEfficFile->Get("lightFlavEfficiency");
    _lightFlavEffic->SetDirectory(0);
    bTagEfficFile->Close();
    delete bTagEfficFile;
  }
  else setEfficbTag(kFALSE);
  

  if (bWeight) setbWeight(true);
  else setbWeight(false);

  ostringstream strFileData;
  ostringstream strFileMC;
  ostringstream strHistMC;
  TString rootfileData;
  TString rootfileMC;
  TString histMC;
  //strFileData<<"Weight.Pileup.Data.File"; 
  //strFileMC<<"Weight.Pileup.MC.File";
  //strHistMC<<"Weight.Pileup.MC.Hist";

  //rootfileData = conf -> GetValue(strFileData.str().c_str(), "null.root");
  //rootfileMC = conf -> GetValue(strFileMC.str().c_str(), "null.root");
  
  histMC = conf -> GetValue(strHistMC.str().c_str(), "null");
  
  //PileupReweighting = new Root::TPileupReweighting("pileuprw");
  //int isGood = PileupReweighting->initialize(rootfileData, "avgintperbx", rootfileMC, histMC);
  //if ( isGood == 0 ) cout<< "EventContainer:: Initialization of TPileupReweighting successful"<<endl;
  //else  cout<<"EventContainer:: Initialization of TPileupReweighting NOT successful!"<<endl;

  //Default list of b tagging systematics. This could possibly become customisable, but it probably doesn't need to be.
  if (_doIterFitbTag) _bTagSystNames = {"central","down_jes","up_lf","down_lf","up_hfstats1","down_hfstats1","up_hfstats2","down_hfstats2","up_cferr1","down_cferr1","up_cferr2","down_cferr2","up_jes"};
  else _bTagSystNames = {"central","up","down"};
  
  
} // EventWeight_nanoAOD

/******************************************************************************
 * EventWeight_nanoAOD::~EventWeight_nanoAOD()                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
EventWeight_nanoAOD::~EventWeight_nanoAOD()
{
 //if(isPileUpWgt()) {
 // delete PileupReweighting; 
 //}
} //~EventWeight_nanoAOD

/******************************************************************************
 * void EventWeight_nanoAOD::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void EventWeight_nanoAOD::BookHistogram()
{

  //cout<<"EventWeight_nanoAOD::BookHistogram, debug level "<<obj->GetDebugLevel()<<endl;

  // Histogram of Input Tree Weights
  _hTreeWeight   =  DeclareTH1F("TreeWeight","Event Weight from the input tree",100,-10.,10.);
  _hTreeWeight   -> SetXAxisTitle("tree weight");

  // Histogram of Global Weights
  _hGlobalWeight =  DeclareTH1F("GlobalWeight","Global Event Weight",100,-10.,10.);
  _hGlobalWeight -> SetXAxisTitle("Global Weight");
  _hGlobalWeight -> SetYAxisTitle("Events");

  // Histogram of MC@NLO Weight. This is one of the global weights
  _hMCatNLOWeight =  DeclareTH1F("MCatNLOWeight","Event Weight for MCatNLO",100,-10.,10.);
  _hMCatNLOWeight -> SetXAxisTitle("MCatNLO Weight");
  _hMCatNLOWeight -> SetYAxisTitle("Events");

  // Histogram of pile up Weight(vtx). This is one of the global weights
  _hPileUpWeight =  DeclareTH1F("PileUpWgtWeight","Event Weight for PileUpWgt",100,-10.,10.);
  _hPileUpWeight -> SetXAxisTitle("PileUpWgt Weight");
  _hPileUpWeight -> SetYAxisTitle("Events");

  // Histogram of pile up Weight(vtx). This is one of the global weights
  _hbWeight =  DeclareTH1F("bWeight","Event Weight for b tagging",100,0.,10.);
  _hbWeight -> SetXAxisTitle("b Weight");
  _hbWeight -> SetYAxisTitle("Events");

  // Histogram of lepton weight
  _hLeptonSFWeight =  DeclareTH1F("leptonSFWeight","Event Weight for lepton SFs",100,0.,2.);
  _hLeptonSFWeight -> SetXAxisTitle("Lep SF");
  _hLeptonSFWeight -> SetYAxisTitle("Events");

  // Histogram of trigger weight
  _hTriggerSFWeight =  DeclareTH1F("triggerSFWeight","Event Weight for trigger SF",100,0.,2.);
  _hTriggerSFWeight -> SetXAxisTitle("Trigger SF");
  _hTriggerSFWeight -> SetYAxisTitle("Events");

  //Create one histogtam per b-tag systematic (and central value)
  for (auto const bTagSystName: _bTagSystNames){
    // Histogram of bTag shape weight
    _hbTagReshape[bTagSystName] =  DeclareTH1F("bTagReshape_"+bTagSystName,"bTag reshaping "+bTagSystName,100,-5.0,5.);
    _hbTagReshape[bTagSystName] -> SetXAxisTitle("bTag Reshape " + bTagSystName);

    _hbTagReshape[bTagSystName] -> SetYAxisTitle("Events");
    _hMisTagReshape[bTagSystName] =  DeclareTH1F("misTagReshape_"+bTagSystName,"mistag reshaping "+bTagSystName,100,-5.0,5.);
    _hMisTagReshape[bTagSystName] -> SetXAxisTitle("misTag Reshape " + bTagSystName);
    _hMisTagReshape[bTagSystName] -> SetYAxisTitle("Events");
  }

  //Create the histograms to show the gen weight of the sample.
  _hGenWeight = DeclareTH1F("genWeight","Generator level event weight",10,-2.,2.);
  _hGenWeight -> SetXAxisTitle("Gen Weight");
  _hGenWeight -> SetYAxisTitle("Events");

  // Histogram of Output Weights
  _hOutputWeight =  DeclareTH1F("OutputWeight","Output Event Weight",100,-10.,10.);
  _hOutputWeight -> SetXAxisTitle("Output Weight");
  _hOutputWeight -> SetXAxisTitle("Events");
  
  // Get configuration
  EventContainer *EventContainerObj =  GetEventContainer();
  TEnv* conf = EventContainerObj -> GetConfig();

  // Use configuration parameters to set weights
  TString sName = EventContainerObj -> GetSourceName();
  Int_t sNumber = EventContainerObj -> GetSourceNumber();
  stringstream strNumber;
  strNumber<<"Weight.Source."<<sNumber;
  //string wq = "Weight.Source."+strNumber.str();
  //wq += strNumber.str();
  //  cout << EventContainerObj->GetEventTree()->GetEntries() << endl;
  Double_t xsecstuff = conf -> GetValue(strNumber.str().c_str(), 0.0);

  //Get the lumi of the data from the config file
  Double_t lumi = conf -> GetValue("Weight.Lumi",100);
  
  //Now multiply the xsec by the lumi
  xsecstuff *= lumi;

  //NOTE: 0.0 is a default value.  Will assume if xsecstuff is 0.0 that there is no cross-section available and the global weight will be set to 1.0.

  if(EventContainerObj -> GetDebugLevel() > 1) {
    cout << "<EventWeight_nanoAOD::BookHistogram>          |     Source Name:   " << std::setw(12) << sName   << "|" << endl;
    cout << "<EventWeight_nanoAOD::BookHistogram>          |     Source Num:    " << std::setw(12) << sNumber << "|" << endl;
    cout << "<EventWeight_nanoAOD::BookHistogram>          |     xsec*BR*lum: " << std::setw(12) << xsecstuff<< "|" << endl;

    //wgt is set in BookHistogram to the value in the weight config file.  This value should be the x-sec*BR*lum
   //The total MC events count (including MCatNLO) is divided by in this program NOT EXTERNALLY
    Double_t gwgt = xsecstuff;
    if(_totalMCatNLOEvents != 0){
      gwgt = gwgt/_totalMCatNLOEvents;
    }else{
      cout<<"_totalMCatNLOEvents is zero- check EventWeight_nanoAOD, executable, and AnalysisMain"<<endl;
    }
  // For now set output event weight equal to global event weight, including MCatNLO if needed
    if(gwgt == 0){
      gwgt = 1.0;
    }
    EventContainerObj -> SetGlobalEventWeight(gwgt);

    cout << "<EventWeight_nanoAOD::BookHistogram>          |     Counts + MCatNLO: " << std::setw(12) <<_totalMCatNLOEvents<< "|" << endl;
    cout << "<EventWeight_nanoAOD::BookHistogram>          |     Global Weight: " << std::setw(12) << EventContainerObj -> GetGlobalEventWeight()<< "|" << endl;
    //	 << sName << " ("<<sNumber<<") is " << EventContainerObj -> GetGlobalEventWeight_nanoAOD() << endl;
  } //if

  //Set up the lepton efficiency SF histograms
  if (_useLeptonSFs) setLeptonHistograms(conf->GetValue("Include.MuonIDSFsFile","null"),conf->GetValue("LeptonID.MuonIDSFHistName","null"),conf->GetValue("Include.MuonISOSFsFile","null"),conf->GetValue("LeptonID.MuonIsoSFHistName","null"),conf->GetValue("Include.MuonTrigSFsFile","null"),conf->GetValue("LeptonID.MuonTrigSHHistName","null"),conf->GetValue("Include.MuonTKSFsFile","null"),conf->GetValue("Include.EleRecoFileName","null"),conf->GetValue("LeptonID.EleRecoHistName","null"),conf->GetValue("Include.EleIDFileName","null"),conf->GetValue("LeptonID.EleIDHistName","null"),conf->GetValue("Include.EleTrigFileName","null"),conf->GetValue("LeptonID.EleTrigHistName","null"));

  if (_usebTagReshape){
    _bTagCalib = BTagCalibration(conf->GetValue("BTaggerAlgo","CSVv2"),conf->GetValue("Include.BTagCSVFile","null"));
    if (_doIterFitbTag){
      _bTagCalibReader = BTagCalibrationReader(BTagEntry::OP_RESHAPING, "central",_bTagSystNames);
      _bTagCalibReader.load(_bTagCalib, BTagEntry::FLAV_UDSG,"iterativefit");
      _bTagCalibReader.load(_bTagCalib, BTagEntry::FLAV_C,"iterativefit");
      _bTagCalibReader.load(_bTagCalib, BTagEntry::FLAV_B,"iterativefit");
    }
    else {
      _bTagCalibReader = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central",_bTagSystNames);
      _bTagCalibReader.load(_bTagCalib, BTagEntry::FLAV_UDSG,"incl");
      _bTagCalibReader.load(_bTagCalib, BTagEntry::FLAV_C,"comb");
      _bTagCalibReader.load(_bTagCalib, BTagEntry::FLAV_B,"comb");
    }
  }

} //BookHistogram()

/******************************************************************************
 * Bool_t EventWeight_nanoAOD::Apply()                                                *
 *                                                                            *
 * Apply weights and fill histograms                                          *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t EventWeight_nanoAOD::Apply()
{
  EventContainer *EventContainerObj = GetEventContainer();

  //nanoAODTree* tree = EventContainerObj->GetEventTree();

  Double_t wgt = EventContainerObj -> GetGlobalEventWeight();

  //This reweight variable might be used in later reweightings perhaps.
  EventContainerObj->SeteventReweight(1.); //Reset the reweighting variable

  float genWeight(1.0);
  if (EventContainerObj->GetGenEventWeight() < 0.) genWeight = -1.;

  wgt *= genWeight;

  // multiply by PileUpWgt weight if desired.
  float pileupEventWeight(-1.0);
  float pileupMinBiasUpWeight(-1.0);
  float pileupMinBiasDownWeight(-1.0);
  float mu = 1; //(float)EventContainerObj -> GetEventTree() -> lbn;
  //eventInfo->event_ID()->lumi_block();
     
  //Do the pileup weight here, but commented out for now
     
  //only apply pileup weight if specified
  if(isPileUpWgt()) {
    //Uncomment to use true interactions
    Int_t binOfInterest = _mcPV->GetXaxis()->FindBin(EventContainerObj->trueInteractions);
    //Uncomment to use number of reconstructed vertices
    //Int_t binOfInterest = _mcPV->GetXaxis()->FindBin(EventContainerObj->pv_npvs);
    if (_mcPV->GetBinContent(binOfInterest) > 0){
      pileupEventWeight = _dataPV->GetBinContent(binOfInterest) / _mcPV->GetBinContent(binOfInterest);
      if (isPileupSysts()){
	pileupMinBiasUpWeight = _minBiasUpPV->GetBinContent(binOfInterest) / _mcPV->GetBinContent(binOfInterest);
	pileupMinBiasDownWeight = _minBiasDownPV->GetBinContent(binOfInterest) / _mcPV->GetBinContent(binOfInterest);
	if (_verbose) { // Debugging checks - shouldn't be written out by default!
	  Float_t pileupEventWeightDebug = _dataPV->GetBinContent(EventContainerObj->trueInteractions+1) / _mcPV->GetBinContent(EventContainerObj->trueInteractions+1);
	  Float_t pileupMinBiasUpWeightDebug = _minBiasUpPV->GetBinContent(EventContainerObj->trueInteractions+1) / _mcPV->GetBinContent(EventContainerObj->trueInteractions+1);
	  Float_t pileupMinBiasDownWeightDebug = _minBiasDownPV->GetBinContent(EventContainerObj->trueInteractions+1) / _mcPV->GetBinContent(EventContainerObj->trueInteractions+1);
	  std::cout << EventContainerObj->trueInteractions << " " << binOfInterest << " " << pileupEventWeight << " up: " << pileupMinBiasUpWeight << " down: " << pileupMinBiasDownWeight << std::endl;
	  std::cout << EventContainerObj->trueInteractions << " " << binOfInterest << " nominal: " << pileupEventWeight << " up: " << pileupMinBiasUpWeight << " down: " << pileupMinBiasDownWeight <<  " old: " << pileupEventWeightDebug << " up: " << pileupMinBiasUpWeightDebug << " down: " << pileupMinBiasDownWeightDebug << std::endl;
	}
      }
    }
    else {
      pileupEventWeight = 1.;
      pileupMinBiasUpWeight = 1.;
      pileupMinBiasDownWeight = 1.;
    }
    wgt *= pileupEventWeight;
  }
  
 //This is where lepton reweights are done, but agtain not implemented yet for nanoAOD

 float lepSFWeight(1.0), lepSFWeightUp(1.0), lepSFWeightDown(1.0);
 float trigSFWeight(1.0), trigSFWeightUp(1.0), trigSFWeightDown(1.0);

 if(_useLeptonSFs){
   std::tie(lepSFWeight,lepSFWeightUp,lepSFWeightDown,trigSFWeight,trigSFWeightUp,trigSFWeightDown) = getLeptonWeight(EventContainerObj);
   wgt *= lepSFWeight;
   wgt *= trigSFWeight;
 }
  
 
 std::map<std::string,float> bTagReshape;
 std::map<std::string,float> misTagReshape;

 /*
 if (_usebTagReshape){
   if (!isEfficbTag()) {
     for (auto const bSystName: _bTagSystNames) std::tie(bTagReshape[bSystName],misTagReshape[bSystName]) = getBTagReshape(EventContainerObj,bSystName);
   }
   else {
     for (auto const bSystName: _bTagSystNames) std::tie(bTagReshape[bSystName],misTagReshape[bSystName]) = getEfficBTagReshape(EventContainerObj,bSystName);
   }
   wgt *= bTagReshape["central"];
   wgt *= misTagReshape["central"];
 }
 /*
 //PDF weights
 

 // if(isPileUpWgt()) {
    //This version is based on primary vertex number and no longer used
    /**
    if(EventContainerObj->DoFastSim()){
      mnwgt = 1;
    }else{
      int n = EventContainerObj -> nPvtx;
      if(n>9) n=9; // Number of vertex >9 use the weight of nPvtx = 9  
      mnwgt = vecPileUpWgt.at(n);
    }
    wgt *= mnwgt;
    _hPileUpWgtWeight->FillWithoutWeight(mnwgt);
    **/
 // 
 // cout << wgt << endl;
  EventContainerObj -> SetOutputEventWeight(wgt);
  EventContainerObj -> SetEventPileupWeight(pileupEventWeight);
  //Save systematic pileup weights
  EventContainerObj -> SetEventPileupMinBiasUpWeight(pileupMinBiasUpWeight);
  EventContainerObj -> SetEventPileupMinBiasDownWeight(pileupMinBiasDownWeight);
  EventContainerObj -> SetEventbWeight(1.); // to be set later(bEventWeight);
  EventContainerObj -> SetEventLepSFWeight(lepSFWeight);
  EventContainerObj -> SetEventTrigSFWeight(trigSFWeight);
  EventContainerObj -> SetGenWeight(genWeight);

  for (auto const bSystName: _bTagSystNames) {
    EventContainerObj -> SetEventbTagReshape(bTagReshape[bSystName],bSystName);
    EventContainerObj -> SetEventMisTagReshape(misTagReshape[bSystName],bSystName);
  }

  EventContainerObj -> SetEventLepSFWeightUp(lepSFWeightUp);
  EventContainerObj -> SetEventLepSFWeightDown(lepSFWeightDown);

  EventContainerObj -> SetEventTrigSFWeightUp(trigSFWeightUp);
  EventContainerObj -> SetEventTrigSFWeightDown(trigSFWeightDown);


  //Fill histograms
  _hTreeWeight     -> FillWithoutWeight(EventContainerObj -> GetTreeEventWeight());
  _hGlobalWeight   -> FillWithoutWeight(EventContainerObj -> GetGlobalEventWeight());
  _hOutputWeight   -> FillWithoutWeight(EventContainerObj -> GetEventWeight());
  _hPileUpWeight   -> FillWithoutWeight(EventContainerObj -> GetEventPileupWeight());
  _hbWeight	   -> FillWithoutWeight(EventContainerObj -> GetEventbWeight());
  _hLeptonSFWeight -> FillWithoutWeight(EventContainerObj -> GetEventLepSFWeight());
  _hTriggerSFWeight -> FillWithoutWeight(EventContainerObj -> GetEventTrigSFWeight());
  _hGenWeight	   -> FillWithoutWeight(EventContainerObj -> GetGenWeight());
  for (auto const bSystName: _bTagSystNames) {
    _hbTagReshape[bSystName] -> FillWithoutWeight(EventContainerObj -> GetEventbTagReshape(bSystName));
    _hMisTagReshape[bSystName] -> FillWithoutWeight(EventContainerObj -> GetEventMisTagReshape(bSystName));
  }

  //a bunch of debugging info
  if (false){
    std::cout << "---------" << std::endl;
    std::cout << "Event number: "   << EventContainerObj -> GetEventNumber() << std::endl;
    std::cout << "Event weight: "   << EventContainerObj -> GetEventWeight() << std::endl;
    std::cout << "Global weight: "   << EventContainerObj -> GetGlobalEventWeight() << std::endl;
    std::cout << "Pileup weight: "  << EventContainerObj -> GetEventPileupWeight() << std::endl;
    std::cout << "Lepton weight: "  << EventContainerObj -> GetEventLepSFWeight() << std::endl;
    std::cout << "Trigger weight: " << EventContainerObj -> GetEventTrigSFWeight() << std::endl;
    
  }

  return kTRUE;
  
} //Apply

//Used to set up the efficiency histograms for the first time
/****************************************************************************** 
 * Bool_t EventWeight_nanoAOD::setLeptonHistograms()                                  * 
 *                                                                            * 
 * Sets up the histograms that will be used for lepton SF reweighting         * 
 *                                                                            * 
 * Input:  Names of files and histograms that are relevant to the calculation * 
 * Output: none                                                               * 
 ******************************************************************************/
void EventWeight_nanoAOD::setLeptonHistograms(TString muonIDFileName, TString muonIDHistName, TString muonIsoFileName, TString muonIsoHistName, TString muonTrigFileName, TString muonTrigHistName, TString muonTkFileName, TString eleRecoFileName, TString eleRecoHistName, TString eleIDFileName, TString eleIDHistName, TString eleTrigFileName, TString eleTrigHistName){

  if (muonIsoFileName == "null" || muonIDFileName == "null"){
    std::cout << "You want lepton SFs included in the weight but you haven't specified files for this! Fix your config!" << std::endl;
  }
  
  TFile* muonIDFile = TFile::Open(muonIDFileName,"READ");
  if (!muonIDFile) std::cout << "Muon ID file not found!" << std::endl;
  _muonIDSF = (TH2F*)muonIDFile->Get(muonIDHistName)->Clone();
  _idEtaIsX = muonIDHistName.Contains("abseta_pt");
  _muonIDSF->SetDirectory(0);
  muonIDFile->Close();

  TFile* muonIsoFile = TFile::Open(muonIsoFileName,"READ");
  if (!muonIsoFile) std::cout << "Muon iso file not found!" << std::endl;
  _muonIsoSF = (TH2F*)muonIsoFile->Get(muonIsoHistName)->Clone();
  _isoEtaIsX = muonIsoHistName.Contains("abseta_pt");
  _muonIsoSF->SetDirectory(0);
  muonIsoFile->Close();

  TFile* muonTrigFile = TFile::Open(muonTrigFileName,"READ");
  if (!muonTrigFile) std::cout << "Muon trig file not found!" << std::endl;
  _muonTrigSF = (TH2F*)muonTrigFile->Get(muonTrigHistName)->Clone();
  _trigEtaIsX = muonTrigHistName.Contains("abseta_pt");
  _muonTrigSF->SetDirectory(0);
  muonTrigFile->Close();

  TFile* muonTkFile = TFile::Open(muonTkFileName,"READ");
  if (!muonTkFile) {
    std::cout << "Muon tracker file not found!" << std::endl;
    _doMuonTk = kFALSE;
  }
  else{
    _muonTkSF = (TGraphAsymmErrors*)muonTkFile->Get("ratio_eff_aeta_dr030e030_corr")->Clone();
    _doMuonTk = kTRUE;
    //  _muonTkSF->SetDirectory(0);
    muonTkFile->Close();
  }

  TFile* eleRecoFile = TFile::Open(eleRecoFileName,"READ");
  if (!eleRecoFile) std::cout << "Electron reco SF file not found!" << std::endl;
  _eleRecoSF = (TH2F*)eleRecoFile->Get(eleRecoHistName)->Clone();
  _eleRecoSF->SetDirectory(0);
  eleRecoFile->Close();

  TFile* eleIDFile = TFile::Open(eleIDFileName,"READ");
  if (!eleIDFile) std::cout << "Electron ID SF file not found!" << std::endl;
  _eleIDSF = (TH2F*)eleIDFile->Get(eleIDHistName)->Clone();
  _eleIDSF->SetDirectory(0);
  eleIDFile->Close();

  TFile* eleTrigFile = TFile::Open(eleTrigFileName,"READ");
  if (!eleTrigFile) {
    std::cout << "Electron Trig SF file not found!" << std::endl;
    _doEleTrig = kFALSE;
  }
  else{
    _eleTrigSF = (TH2F*)eleTrigFile->Get(eleTrigHistName)->Clone();
    _eleTrigSF->SetDirectory(0);
    _doEleTrig = kTRUE;
    eleTrigFile->Close();
  }

  delete muonIsoFile,muonIDFile,muonTrigFile,muonTkFile,eleRecoFile,eleIDFile,eleTrigFile;

}

/****************************************************************************** 
 * Bool_t EventWeight_nanoAOD::getLeptonWeight()                                      * 
 *                                                                            * 
 * Get the relevant pt and eta dependent SFs for the leptons in the event     *
 * and put them into one weight that is returned                              * 
 *                                                                            * 
 * Input:  None                                                               * 
 * Output: Double_t weight to be applied to the event weight                  * 
 ******************************************************************************/
std::tuple<Double_t,Double_t,Double_t,Double_t,Double_t,Double_t> EventWeight_nanoAOD::getLeptonWeight(EventContainer* EventContainerObj){

  Double_t leptonWeight = 1.0, leptonWeightUp = 1.0, leptonWeightDown = 1.0;
  Double_t triggerWeight = 1.0, triggerWeightUp = 1.0, triggerWeightDown = 1.0;

  for (auto const & muon : EventContainerObj->GetMuonCollection(_muonsForLepWeight)){
    //Get the bin shared by iso and id SFs
    Int_t xAxisBin,yAxisBin,xAxisBinTrig,yAxisBinTrig;
    if (!_isoEtaIsX){
      xAxisBin = _muonIsoSF->GetXaxis()->FindBin(muon.Pt());
      if (muon.Pt() > 120.) xAxisBin = _muonIsoSF->GetXaxis()->FindBin(119.);
      if (muon.Pt() < _muonIsoSF->GetXaxis()->GetBinLowEdge(1)) xAxisBin = 1;
      yAxisBin = _muonIsoSF->GetYaxis()->FindBin(std::fabs(muon.Eta()));
      if (std::fabs(muon.Eta()) > 2.4) yAxisBin = _muonIsoSF->GetYaxis()->FindBin(2.39);
    }
    else{
      yAxisBin = _muonIsoSF->GetYaxis()->FindBin(muon.Pt());
      if (muon.Pt() > 120.) yAxisBin = _muonIsoSF->GetYaxis()->FindBin(119.);
      if (muon.Pt() < _muonIsoSF->GetYaxis()->GetBinLowEdge(1)) yAxisBin = 1;
      xAxisBin = _muonIsoSF->GetXaxis()->FindBin(std::fabs(muon.Eta()));
      if (std::fabs(muon.Eta()) > 2.4) xAxisBin = _muonIsoSF->GetXaxis()->FindBin(2.39);
    }
    //And now get the iso and id SFs/uncs
    Float_t isoSF = _muonIsoSF->GetBinContent(xAxisBin,yAxisBin);
    Float_t isoUnc = _muonIsoSF->GetBinError(xAxisBin,yAxisBin);
    Float_t idSF = _muonIDSF->GetBinContent(xAxisBin,yAxisBin);
    Float_t idUnc = _muonIDSF->GetBinError(xAxisBin,yAxisBin);
    
    Float_t trigSF = 1.0;
    Float_t trigUnc = 0.0;

    //Get the bin for trigger SF, but only if the muon in question is the triggering muon
    if (muon.isTriggerMatchedMu()){
      if (!_trigEtaIsX){
	xAxisBinTrig = _muonTrigSF->GetXaxis()->FindBin(muon.Pt());
	if (muon.Pt() > 500.) xAxisBinTrig = _muonTrigSF->GetXaxis()->FindBin(499.);
	if (muon.Pt() < _muonTrigSF->GetXaxis()->GetBinLowEdge(1)) xAxisBinTrig = -1;
	yAxisBinTrig = _muonTrigSF->GetYaxis()->FindBin(std::fabs(muon.Eta()));
	if (std::fabs(muon.Eta()) > 2.4) yAxisBinTrig = _muonTrigSF->GetYaxis()->FindBin(2.39);
      }
      else{
	yAxisBinTrig = _muonTrigSF->GetYaxis()->FindBin(muon.Pt());
	if (muon.Pt() > 500.) yAxisBinTrig = _muonTrigSF->GetYaxis()->FindBin(499.);
	if (muon.Pt() < _muonTrigSF->GetYaxis()->GetBinLowEdge(1)) yAxisBinTrig = -1;
	xAxisBinTrig = _muonTrigSF->GetXaxis()->FindBin(std::fabs(muon.Eta()));
	if (std::fabs(muon.Eta()) > 2.4) xAxisBinTrig = _muonTrigSF->GetXaxis()->FindBin(2.39);      
      }
      //Get the trigSF
      trigSF = _muonTrigSF->GetBinContent(xAxisBinTrig,yAxisBinTrig);
      trigUnc = _muonTrigSF->GetBinError(xAxisBinTrig,yAxisBinTrig);
      if (xAxisBinTrig < 0 || yAxisBinTrig < 0){
	trigSF = 1.;
      trigUnc = 0.;
      }
    }

    //Evaluate muon tk
    Float_t tkSF = 1.0;
    if (_doMuonTk) _muonTkSF->Eval(std::fabs(muon.Eta()));

    leptonWeight *= isoSF * idSF * tkSF;
    leptonWeightUp *= (isoSF + isoUnc) * (idSF + idUnc) * tkSF;
    leptonWeightDown *= (isoSF - isoUnc) * (idSF - idUnc) * tkSF;

    triggerWeight *= trigSF;
    triggerWeightUp *= trigSF + trigUnc;
    triggerWeightDown *= trigSF - trigUnc;

  }

  for (auto const & ele : EventContainerObj->GetElectronCollection(_electronsForLepWeight)){
    //Get which bins we're in need of for the reco SF
    Int_t xAxisBin = _eleRecoSF->GetXaxis()->FindBin(ele.scEta());
    if (ele.scEta() > 2.5) xAxisBin = _eleRecoSF->GetXaxis()->FindBin(2.49);
    if (ele.scEta() < -2.5) xAxisBin = _eleRecoSF->GetXaxis()->FindBin(-2.49);
    Int_t yAxisBin = _eleRecoSF->GetYaxis()->FindBin(ele.Pt());
    if (ele.Pt() > 500) yAxisBin = _eleRecoSF->GetYaxis()->FindBin(499.);
    //Now get the reco and id SFs
    Float_t recoSF = _eleRecoSF->GetBinContent(xAxisBin,yAxisBin);
    Float_t recoUnc = _eleRecoSF->GetBinError(xAxisBin,yAxisBin);
    //Now do ID
    xAxisBin = _eleIDSF->GetXaxis()->FindBin(ele.scEta());
    if (ele.scEta() > 2.5) xAxisBin = _eleIDSF->GetXaxis()->FindBin(2.49);
    if (ele.scEta() > -2.5) xAxisBin = _eleIDSF->GetXaxis()->FindBin(-2.49);
    yAxisBin = _eleIDSF->GetYaxis()->FindBin(ele.Pt());
    if (ele.Pt() > 500) yAxisBin = _eleIDSF->GetYaxis()->FindBin(499.);
    Float_t idSF = _eleIDSF->GetBinContent(xAxisBin,yAxisBin);
    Float_t idUnc = _eleIDSF->GetBinError(xAxisBin,yAxisBin);

    //And finally trigger
    Float_t trigSF = 1.0;
    Float_t trigUnc = 0.0;
    if (_doEleTrig){
      yAxisBin = _eleTrigSF->GetYaxis()->FindBin(ele.scEta());
      if (ele.scEta() > 2.5) yAxisBin = _eleTrigSF->GetYaxis()->FindBin(2.49);
      if (ele.scEta() < -2.5) yAxisBin = _eleTrigSF->GetYaxis()->FindBin(-2.49);
      xAxisBin = _eleTrigSF->GetXaxis()->FindBin(ele.Pt());
      if (ele.Pt() > 200) xAxisBin = _eleTrigSF->GetXaxis()->FindBin(199.);
      Float_t trigSF = _eleTrigSF->GetBinContent(xAxisBin,yAxisBin);
      Float_t trigUnc = _eleTrigSF->GetBinError(xAxisBin,yAxisBin);
    }

    triggerWeight = trigSF;              
    triggerWeightUp = trigSF + trigUnc;  
    triggerWeightDown = trigSF - trigUnc;

    leptonWeight *= recoSF * idSF;
    leptonWeightUp *= (recoSF + recoUnc) * (idSF + idUnc);
    leptonWeightDown *= (recoSF - recoUnc) * (idSF - idUnc);
    if (_verbose && leptonWeight == 0.){
      std::cout << "Reco: " << recoSF << " ID: " << idSF << " pt: " <<ele.Pt() << " scEta: " << ele.scEta()  << " x: " << xAxisBin << " y: " << yAxisBin <<std::endl;
    }

  }

  if (_verbose and leptonWeight == 0.) std::cout << "FinalWeight: " <<  leptonWeight << " up: " << leptonWeightUp << " down: " << leptonWeightDown << std::endl;

  return std::make_tuple(leptonWeight,leptonWeightUp,leptonWeightDown,triggerWeight,triggerWeightUp,triggerWeightDown);
}

/******************************************************************************  
 * Double_t EventWeight_nanoAOD::getBTagReshape()                                     *  
 *                                                                            *  
 * Get the reshaped CSV discriminant from the reshaping class                 *
 *                                                                            *  
 * Input:  EventContainer of the event                                        *  
 * Output: Double_t weight to be applied to the event weight                  *  
 ******************************************************************************/ 
std::tuple<Double_t,Double_t> EventWeight_nanoAOD::getBTagReshape(EventContainer * EventContainerObj, std::string syst){


  Double_t bTagWeight = 1.0;
  Double_t mistagWeight = 1.0;

  for (auto const & jet : EventContainerObj->jets){
    if (jet.GethadronFlavour() == 5){ // If jet is b-jet
      float jetSF = _bTagCalibReader.eval_auto_bounds(syst, BTagEntry::FLAV_B, jet.Eta(), jet.Pt(), jet.bDiscriminator());
      if (jetSF == 0) jetSF = _bTagCalibReader.eval_auto_bounds("central", BTagEntry::FLAV_B, jet.Eta(), jet.Pt(), jet.bDiscriminator());
      bTagWeight *= jetSF;
    }
    else if (jet.GethadronFlavour() == 4){
      float jetSF = _bTagCalibReader.eval_auto_bounds(syst, BTagEntry::FLAV_C, jet.Eta(), jet.Pt(), jet.bDiscriminator());
      if (jetSF == 0) jetSF = _bTagCalibReader.eval_auto_bounds("central", BTagEntry::FLAV_C, jet.Eta(), jet.Pt(), jet.bDiscriminator());
      mistagWeight *= jetSF;
    }
    else {
      float jetSF = _bTagCalibReader.eval_auto_bounds(syst, BTagEntry::FLAV_UDSG, jet.Eta(), jet.Pt(), jet.bDiscriminator());
      if (jetSF == 0) jetSF = _bTagCalibReader.eval_auto_bounds("central", BTagEntry::FLAV_UDSG, jet.Eta(), jet.Pt(), jet.bDiscriminator());
      mistagWeight *= jetSF;
    }
  }

  return std::make_tuple(bTagWeight,mistagWeight);
  /*
  for (auto const & jet : EventContainerObj->taggedJets){
    float jetSF = _bTagCalibReader.eval_auto_bounds(syst, BTagEntry::FLAV_B, jet.Eta(), jet.Pt(), jet.bDiscriminator());
    if (jetSF == 0) jetSF = _bTagCalibReader.eval_auto_bounds("central", BTagEntry::FLAV_B, jet.Eta(), jet.Pt(), jet.bDiscriminator());
    bTagWeight *= jetSF;
  }

  for (auto const & jet : EventContainerObj->unTaggedJets){
    float jetSF = _bTagCalibReader.eval_auto_bounds(syst, BTagEntry::FLAV_UDSG, jet.Eta(), jet.Pt(), jet.bDiscriminator());
    if (jetSF == 0) jetSF = _bTagCalibReader.eval_auto_bounds("central", BTagEntry::FLAV_UDSG, jet.Eta(), jet.Pt(), jet.bDiscriminator());
    bTagWeight *= jetSF;
  }
  return bTagWeight;
  */
}

/******************************************************************************  
 * Double_t EventWeight_nanoAOD::getEfficBTagReshape()                                *  
 *                                                                            *  
 * Get the reshaped CSV discriminant from the reshaping class using           *
 * new efficiency based calculation.                                          *
 *                                                                            *  
 * Input:  EventContainer of the event                                        *  
 * Output: Double_t weight to be applied to the event weight                  *  
 ******************************************************************************/ 
std::tuple<Double_t,Double_t> EventWeight_nanoAOD::getEfficBTagReshape(EventContainer * EventContainerObj, std::string syst){

  
  //if (syst == "central") std::cout << "Starting event b-tag measurement..." << std::endl;

  Double_t bTagWeight = 1.0;
  Double_t mistagWeight = 1.0;

  Double_t mcNoTag = 1.;
  Double_t dataNoTag = 1.;

  float jetSF = 1.;
  float jetEffic = 1.;

  //  if (syst == "central") std::cout << syst << " j/t/u: " << EventContainerObj->jets.size() << " " << EventContainerObj->taggedJets.size() << " " << EventContainerObj->unTaggedJets.size() << std::endl;

  for (auto const & jet : EventContainerObj->jets){
    
    jetSF = getJetSF(jet,syst);
    jetEffic = getJetEffic(jet);

    if (jet.IsTagged()){
      bTagWeight *= jetSF;
    } else {
      mcNoTag *= 1 - jetEffic;
      dataNoTag *= 1 - (jetEffic*jetSF);
    }
    //    if (syst == "central") std::cout << "Jet pt: " << jet.Pt() << " flavour: " << jet.GethadronFlavour() << " tag: " << jet.IsTagged() << " " << jet.bDiscriminator() << " eta: " << jet.Eta() << " " << jetSF << " " << jetEffic << " current b/mis-tag: " << bTagWeight << " " << (dataNoTag/mcNoTag)  << std::endl;
  }

  //  if (syst == "central") std::cout <<  " After: " << bTagWeight << " " << dataNoTag/mcNoTag << std::endl;
  
  return std::make_tuple(bTagWeight,dataNoTag/mcNoTag);

}

Double_t EventWeight_nanoAOD::getJetEffic(Jet jet){
  TH1F* efficiencyPlot;
  switch (abs(jet.GethadronFlavour())){
  case 5:
    efficiencyPlot = _bFlavEffic;
    break;
  case 4:
    efficiencyPlot = _cFlavEffic;
    break;
  default:
    efficiencyPlot = _lightFlavEffic;
  }

  int binx = efficiencyPlot->GetXaxis()->FindBin(jet.Pt());
  int biny = efficiencyPlot->GetYaxis()->FindBin(fabs(jet.Eta()));

  return efficiencyPlot->GetBinContent(binx,biny);
}

Double_t EventWeight_nanoAOD::getJetSF(Jet jet, std::string syst){
  BTagEntry::JetFlavor jetFlavour = BTagEntry::FLAV_UDSG;

  if (fabs(jet.GethadronFlavour() == 5)) jetFlavour = BTagEntry::FLAV_B;
  else if (fabs(jet.GethadronFlavour() == 4)) jetFlavour = BTagEntry::FLAV_C;

  float jetSF = _bTagCalibReader.eval_auto_bounds(syst, jetFlavour, jet.Eta(), jet.Pt(), jet.bDiscriminator());
  if (jetSF == 0) jetSF = _bTagCalibReader.eval_auto_bounds("central", jetFlavour, jet.Eta(), jet.Pt(), jet.bDiscriminator());

  

  return jetSF;
}
