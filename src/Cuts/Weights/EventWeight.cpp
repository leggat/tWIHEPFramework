/******************************************************************************
 * EventWeight.hpp                                                            *
 *                                                                            *
 * Weight events in the histograms                                            *
 *                                                                            *
 * This class is derived from the BaseCut Class and simply                    *
 * cuts on if the event has reconstructed objects                             *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of AnalysisMain class                              *
 *    EventWeight()                     -- Parameterized Constructor          *
 *    ~EventWeight()                    -- Destructor                         *
 *    BookHistogram()                   -- Book histograms of weights         *
 *    Apply()                           -- Apply weight and fill histograms   *
 *    GetCutName()                      -- Returns "EventWeight"              *
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

#include "SingleTopRootAnalysis/Cuts/Weights/EventWeight.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <tuple>

using namespace std;


/******************************************************************************
 * EventWeight::EventWeight(EventContainer *obj)                              *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Particle class                                                     *
 * Output: None                                                               *
 ******************************************************************************/
EventWeight::EventWeight(EventContainer *EventContainerObj,Double_t TotalMCatNLOEvents,const std::string& MCtype, Bool_t pileup, Bool_t bWeight, Bool_t useLeptonSFs, Bool_t usebTagReshape):
  _useLeptonSFs(useLeptonSFs),
  _usebTagReshape(usebTagReshape)
{
  //pileup is NOT applied by default.  Instead it is applied by the user, and stored in the tree for later application

  TEnv* conf = EventContainerObj -> GetConfig();
  /**
  if(MCtype.find("PileUpWgt")<=MCtype.size()){
    cout<<"EventWeight: This version of pileup weights is based on primary vertex number and no longer used!!!"<<endl;
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV0", 1.));
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV1", 1.));
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV2", 1.));
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV3", 1.));
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV4", 1.));
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV5", 1.));
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV6", 1.));
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV7", 1.));
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV8", 1.));
    vecPileUpWgt.push_back(conf->GetValue("Weight.Source.nPvtx.NPV9", 1.));
  }
  **/
  // Set Particle object
  _totalMCatNLOEvents = 0;

 if(MCtype.find("MCatNLO")<=MCtype.size()){
    setMCatNLO(true);
  }else{
    setMCatNLO(false);
  }
 /**
 if(MCtype.find("PileUpWgt")<=MCtype.size()){
    setPileUpWgt(true);
  }else{
    setPileUpWgt(false);
  }
 **/
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
  setPileUpSyst(false);
  if(pileup){
    setPileUpWgt(true);
    TFile* dataPVFile = TFile::Open(conf -> GetValue("Include.dataPVFile","null"),"READ");
    _dataPV = (TH1F*)dataPVFile->Get("pileup");
    _dataPV->SetDirectory(0);
    _dataPV->Scale(1./_dataPV->Integral());
    dataPVFile->Close();
    delete dataPVFile;
    
    TFile* mcPVFile = TFile::Open(conf -> GetValue("Include.mcPVFile","null"),"READ");
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
  _bTagSystNames = {"central","down_jes","up_lf","down_lf","up_hfstats1","down_hfstats1","up_hfstats2","down_hfstats2","up_cferr1","down_cferr1","up_cferr2","down_cferr2","up_jes"};
  
  
} // EventWeight

/******************************************************************************
 * EventWeight::~EventWeight()                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
EventWeight::~EventWeight()
{
 //if(isPileUpWgt()) {
 // delete PileupReweighting; 
 //}
} //~EventWeight

/******************************************************************************
 * void EventWeight::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void EventWeight::BookHistogram()
{

  //cout<<"EventWeight::BookHistogram, debug level "<<obj->GetDebugLevel()<<endl;

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
  _hPileUpWeight =  DeclareTH1F("PileUpWgtWeight","Event Weight for PileUpWgt",100,0.,10.);
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

  //Create one histogtam per b-tag systematic (and central value)
  for (auto const bTagSystName: _bTagSystNames){
    // Histogram of bTag shape weight
    _hbTagReshape[bTagSystName] =  DeclareTH1F("bTagReshape_"+bTagSystName,"bTag reshaping "+bTagSystName,100,-5.0,5.);
    _hbTagReshape[bTagSystName] -> SetXAxisTitle("bTag Reshape " + bTagSystName);
    _hbTagReshape[bTagSystName] -> SetYAxisTitle("Events");
  }
    
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
    cout << "<EventWeight::BookHistogram>          |     Source Name:   " << std::setw(12) << sName   << "|" << endl;
    cout << "<EventWeight::BookHistogram>          |     Source Num:    " << std::setw(12) << sNumber << "|" << endl;
    cout << "<EventWeight::BookHistogram>          |     xsec*BR*lum: " << std::setw(12) << xsecstuff<< "|" << endl;

    //wgt is set in BookHistogram to the value in the weight config file.  This value should be the x-sec*BR*lum
   //The total MC events count (including MCatNLO) is divided by in this program NOT EXTERNALLY
    Double_t gwgt = xsecstuff;
    if(_totalMCatNLOEvents != 0){
      gwgt = gwgt/_totalMCatNLOEvents;
    }else{
      cout<<"_totalMCatNLOEvents is zero- check EventWeight, executable, and AnalysisMain"<<endl;
    }
  // For now set output event weight equal to global event weight, including MCatNLO if needed
    if(gwgt == 0){
      gwgt = 1.0;
    }
    EventContainerObj -> SetGlobalEventWeight(gwgt);

    cout << "<EventWeight::BookHistogram>          |     Counts + MCatNLO: " << std::setw(12) <<_totalMCatNLOEvents<< "|" << endl;
    cout << "<EventWeight::BookHistogram>          |     Global Weight: " << std::setw(12) << EventContainerObj -> GetGlobalEventWeight()<< "|" << endl;
    //	 << sName << " ("<<sNumber<<") is " << EventContainerObj -> GetGlobalEventWeight() << endl;
  } //if

  //Set up the lepton efficiency SF histograms
  if (_useLeptonSFs) setLeptonHistograms(conf->GetValue("Include.MuonIDSFsFile","null"),conf->GetValue("LeptonID.MuonIDSFHistName","null"),conf->GetValue("Include.MuonISOSFsFile","null"),conf->GetValue("LeptonID.MuonIsoSFHistName","null"));

  if (_usebTagReshape){
    _bTagCalib = BTagCalibration(conf->GetValue("BTaggerAlgo","CSVv2"),conf->GetValue("Include.BTagCSVFile","null"));
    _bTagCalibReader = BTagCalibrationReader(BTagEntry::OP_RESHAPING, "central",_bTagSystNames);
    _bTagCalibReader.load(_bTagCalib, BTagEntry::FLAV_B,"iterativefit");
    _bTagCalibReader.load(_bTagCalib, BTagEntry::FLAV_C,"iterativefit");
    _bTagCalibReader.load(_bTagCalib, BTagEntry::FLAV_UDSG,"iterativefit");
  }

} //BookHistogram()

/******************************************************************************
 * Bool_t EventWeight::Apply()                                                *
 *                                                                            *
 * Apply weights and fill histograms                                          *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t EventWeight::Apply()
{
     EventContainer *EventContainerObj = GetEventContainer();
   // Set output event weight- no difference

     EventTree* tree = EventContainerObj->GetEventTree();

  Double_t wgt = EventContainerObj -> GetGlobalEventWeight();
 Double_t mnwgt = 1;
  //
  // multiply by MCatNLO weight if desired.
 //Note that if files like data are flagged as MCatNLO, strange weights might result (negative yields...)
  if(isMCatNLO()) {
    //  cout<<"we found MCatNLO"<<endl;
    if(EventContainerObj->DoFastSim()){
      mnwgt =EventContainerObj -> GetFastSimTree() ->eventWeightMCatNLO;
    }else{
      mnwgt = 1; //EventContainerObj -> GetEventTree() -> weight;
      // For comparison
      //mnwgt = 1;
    }
    //    cout << tree->EVENT_genWeight << " " << tree->bWeight << " " << tree->PUWeight << "weight" << endl;
    //  Double_t mnwgt = EventContainerObj->GetTruthTree()->eventWeightMCatNLO;
    // cout<<"the weight is "<<mnwgt<<endl;

    // cout<<wgt<<"  "<<mnwgt<<"  "<<endl;
    wgt *= mnwgt;
    _hMCatNLOWeight->FillWithoutWeight(mnwgt);
  }

  // multiply by PileUpWgt weight if desired.
 float pileupEventWeight(-1.0);
 float pileupMinBiasUpWeight(-1.0);
 float pileupMinBiasDownWeight(-1.0);
 float mu = 1; //(float)EventContainerObj -> GetEventTree() -> lbn;
 //eventInfo->event_ID()->lumi_block();
 
 // Get the pileup weight for this event
 //int isGood = PileupReweighting->getPileupWeight( mu, pileupEventWeight );
 
 //if ( isGood == 0 )
 //  {
 //    // Printout message
 //    // cout << "Event has a mu of " << mu 
 //    //	    << " and a resulting pileup weight of " << pileupEventWeight<<endl;;
 //  }
 //else if ( isGood == -1 )
 //  {
 //    cout << "Data histogram pointer got lost"<<endl;;
 //  }
 //else if ( isGood == -2 )
 //  {
 //    cout << "MC histogram pointer got lost"<<endl;;
 //  }
 //else {
 //  cout<<"unknown error"<<endl;
 //}
 //Adjust by the total uncut event yield over total pileup weighted uncut event yield 
 //pileupEventWeight = pileupEventWeight*PileupAdjust(EventContainerObj ->eventNumber, EventContainerObj ->runNumber);

 //only apply pileup weight if specified
 if(isPileUpWgt()) {
   if (_mcPV->GetBinContent(tree->trueInteractions) > 0){
     pileupEventWeight = _dataPV->GetBinContent(tree->trueInteractions) / _mcPV->GetBinContent(tree->trueInteractions);
     if (isPileupSysts()){
       pileupMinBiasUpWeight = _minBiasUpPV->GetBinContent(tree->trueInteractions) / _mcPV->GetBinContent(tree->trueInteractions);
       pileupMinBiasDownWeight = _minBiasDownPV->GetBinContent(tree->trueInteractions) / _mcPV->GetBinContent(tree->trueInteractions);
     }
   }
   wgt *= pileupEventWeight;
 }

 float bEventWeight(1.0);
 
 if (isbWeight()){
   bEventWeight = tree->bWeight;
   wgt *= bEventWeight;
 }

 
 float lepSFWeight(1.0), lepSFWeightUp(1.0), lepSFWeightDown(1.0);

 if(_useLeptonSFs){
   std::tie(lepSFWeight,lepSFWeightUp,lepSFWeightDown) = getLeptonWeight(EventContainerObj);
   wgt *= lepSFWeight;
 }

 std::map<std::string,float> bTagReshape;

 if (_usebTagReshape){
   for (auto const bSystName: _bTagSystNames) bTagReshape[bSystName] = getBTagReshape(EventContainerObj,bSystName);
   wgt *= bTagReshape["central"];
 }

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
 // }
 // cout << wgt << endl;
  EventContainerObj -> SetOutputEventWeight(wgt);
  EventContainerObj -> SetEventPileupWeight(pileupEventWeight);
  //Save systematic pileup weights
  EventContainerObj -> SetEventPileupMinBiasUpWeight(pileupMinBiasUpWeight);
  EventContainerObj -> SetEventPileupMinBiasDownWeight(pileupMinBiasDownWeight);
  EventContainerObj -> SetEventbWeight(bEventWeight);
  EventContainerObj -> SetEventLepSFWeight(lepSFWeight);
  for (auto const bSystName: _bTagSystNames) EventContainerObj -> SetEventbTagReshape(bTagReshape[bSystName],bSystName);

  EventContainerObj -> SetEventLepSFWeightUp(lepSFWeightUp);
  EventContainerObj -> SetEventLepSFWeightDown(lepSFWeightDown);

  //Also save the systematic variations in these SFs
  //  EventContainerObj -> SetEventLepSFWeightUp(lepSFWeightUp);
  //EventContainerObj -> SetEventLepSFWeightDown(lepSFWeightDown);
  //  cout<<"weight "<<EventContainerObj -> GetOutputEventWeight()<<endl;;
  // Fill Histograms

  _hTreeWeight     -> FillWithoutWeight(EventContainerObj -> GetTreeEventWeight());
  _hGlobalWeight   -> FillWithoutWeight(EventContainerObj -> GetGlobalEventWeight());
  _hOutputWeight   -> FillWithoutWeight(EventContainerObj -> GetEventWeight());
  _hPileUpWeight   -> FillWithoutWeight(EventContainerObj -> GetEventPileupWeight());
  _hbWeight	   -> FillWithoutWeight(EventContainerObj -> GetEventbWeight());
  _hLeptonSFWeight -> FillWithoutWeight(EventContainerObj -> GetEventLepSFWeight());
  for (auto const bSystName: _bTagSystNames) _hbTagReshape[bSystName] -> FillWithoutWeight(EventContainerObj -> GetEventbTagReshape(bSystName));

  return kTRUE;
  
} //Apply

//Used to set up the efficiency histograms for the first time
/****************************************************************************** 
 * Bool_t EventWeight::setLeptonHistograms()                                  * 
 *                                                                            * 
 * Sets up the histograms that will be used for lepton SF reweighting         * 
 *                                                                            * 
 * Input:  Names of files and histograms that are relevant to the calculation * 
 * Output: none                                                               * 
 ******************************************************************************/
void EventWeight::setLeptonHistograms(TString muonIDFileName, TString muonIDHistName, TString muonIsoFileName, TString muonIsoHistName){

  if (muonIsoFileName == "null" || muonIDFileName == "null"){
    std::cout << "You want lepton SFs included in the weight but you haven't specified files for this! Fix your config!" << std::endl;
  }
  
  TFile* muonIDFile = TFile::Open(muonIDFileName,"READ");
  _muonIDSF = (TH2F*)muonIDFile->Get(muonIDHistName+"_pt_spliteta_bin1/pt_abseta_ratio");
  _muonIDSF->SetDirectory(0);
  muonIDFile->Close();

  TFile* muonIsoFile = TFile::Open(muonIsoFileName,"READ");
  _muonIsoSF = (TH2F*)muonIsoFile->Get(muonIsoHistName+"_pt_spliteta_bin1/pt_abseta_ratio")->Clone();
  _muonIsoSF->SetDirectory(0);
  muonIsoFile->Close();
  delete muonIsoFile,muonIDFile;

}

/****************************************************************************** 
 * Bool_t EventWeight::getLeptonWeight()                                      * 
 *                                                                            * 
 * Get the relevant pt and eta dependent SFs for the leptons in the event     *
 * and put them into one weight that is returned                              * 
 *                                                                            * 
 * Input:  None                                                               * 
 * Output: Double_t weight to be applied to the event weight                  * 
 ******************************************************************************/
std::tuple<Double_t,Double_t,Double_t> EventWeight::getLeptonWeight(EventContainer* EventContainerObj){

  Double_t leptonWeight = 1.0, leptonWeightUp = 1.0, leptonWeightDown = 1.0;

  for (auto const & muon : *EventContainerObj->muonsToUsePtr){
    Int_t xAxisBin = _muonIsoSF->GetXaxis()->FindBin(muon.Pt());
    if (muon.Pt() > 200.) xAxisBin = _muonIsoSF->GetXaxis()->FindBin(199.);
    Int_t yAxisBin = _muonIsoSF->GetYaxis()->FindBin(std::fabs(muon.Eta()));
    if (std::fabs(muon.Eta()) > 2.4) yAxisBin = _muonIsoSF->GetYaxis()->FindBin(2.39);
    Float_t isoSF = _muonIsoSF->GetBinContent(xAxisBin,yAxisBin);
    Float_t isoUnc = _muonIsoSF->GetBinError(xAxisBin,yAxisBin);
    Float_t idSF = _muonIDSF->GetBinContent(xAxisBin,yAxisBin);
    Float_t idUnc = _muonIDSF->GetBinError(xAxisBin,yAxisBin);
    leptonWeight *= isoSF * idSF;
    leptonWeightUp *= (isoSF + isoUnc) * (idSF + idUnc);
    leptonWeightDown *= (isoSF - isoUnc) * (idSF - idUnc);
  }

  return std::make_tuple(leptonWeight,leptonWeightUp,leptonWeightDown);
}

/******************************************************************************  
 * Double_t EventWeight::getBTagReshape()                                     *  
 *                                                                            *  
 * Get the reshaped CSV discriminant from the reshaping class                 *
 *                                                                            *  
 * Input:  EventContainer of the event                                        *  
 * Output: Double_t weight to be applied to the event weight                  *  
 ******************************************************************************/ 
Double_t EventWeight::getBTagReshape(EventContainer * EventContainerObj, std::string syst){

  Double_t bTagWeight = 1.0;
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
}

Double_t EventWeight::PileupAdjust(int eventNumber, int runnumber)
{
  float frac = 1.0;

if (runnumber == 105200)  frac = 1.02596;
else if (runnumber == 105204)  frac = 1.0053;
else if (runnumber == 105205)  frac = 1.00194;
else if (runnumber == 105500)  frac = 1.00716;
else if (runnumber == 105860)  frac = 1.00061;
else if (runnumber == 105861)  frac = 1.00109;
else if (runnumber == 105985)  frac = 1.00013;
else if (runnumber == 105986)  frac = 1.00167;
else if (runnumber == 105987)  frac = 1.00041;

else if (runnumber == 107650)  frac = 1.00056;
else if (runnumber == 107651)  frac = 0.999814;
else if (runnumber == 107652)  frac = 0.999761;
else if (runnumber == 107653)  frac = 1;
else if (runnumber == 107654)  frac = 1.00621;
else if (runnumber == 107655)  frac = 1.0055;

else if (runnumber == 107661)  frac = 1.00339;
else if (runnumber == 107662)  frac = 1.00444;
else if (runnumber == 107663)  frac = 1.00222;
else if (runnumber == 107664)  frac = 1.00731;
else if (runnumber == 107665)  frac = 1.00674;
else if (runnumber == 107670)  frac = 0.998971;
else if (runnumber == 107671)  frac = 0.998627;
else if (runnumber == 107672)  frac = 0.99429;
else if (runnumber == 107673)  frac = 0.993913;
else if (runnumber == 107674)  frac = 0.990936;
else if (runnumber == 107675)  frac = 1.02436;

else if (runnumber == 107681)  frac = 0.995526;
else if (runnumber == 107682)  frac = 0.997984;
else if (runnumber == 107683)  frac = 0.994221;
else if (runnumber == 107684)  frac = 0.994099;
else if (runnumber == 107685)  frac = 1.00607;

else if (runnumber == 107691)  frac = 1.00435;
else if (runnumber == 107692)  frac = 1.00421;
else if (runnumber == 107693)  frac = 1.00402;
else if (runnumber == 107694)  frac = 1.00054;
else if (runnumber == 107695)  frac = 1.00184;
else if (runnumber == 107700)  frac = 1.00221;
else if (runnumber == 107701)  frac = 0.99912;
else if (runnumber == 107702)  frac = 1.00062;

else if (runnumber == 107704)  frac = 0.990074;
else if (runnumber == 107705)  frac = 0.994124;
else if (runnumber == 108340)  frac = 0.995628;
else if (runnumber == 108341)  frac = 1.00081;
else if (runnumber == 108342)  frac = 1.00517;
else if (runnumber == 108343)  frac = 0.996556;
else if (runnumber == 108344)  frac = 1.00456;
else if (runnumber == 108345)  frac = 1.0076;
else if (runnumber == 108346)  frac = 1.00447;
else if (runnumber == 117255)  frac = 0.999862;
else if (runnumber == 117256)  frac = 1.00185;
else if (runnumber == 117257)  frac = 1.00131;
else if (runnumber == 117258)  frac = 1.00001;
else if (runnumber == 117259)  frac = 1.00036;
else if (runnumber == 117260)  frac = 1.00263;
else if (runnumber == 117284)  frac = 0.998673;
else if (runnumber == 117285)  frac = 1.00038;
else if (runnumber == 117286)  frac = 0.998383;
else if (runnumber == 117287)  frac = 0.997502;

else if (runnumber == 117320)  frac = 1.00079;
else if (runnumber == 117321)  frac = 1.00362;
else if (runnumber == 117322)  frac = 1.00177;

else if (runnumber == 117324)  frac = 1.0027;
else if (runnumber == 117325)  frac = 0.996011;
else if (runnumber == 117326)  frac = 1.002;
else if (runnumber == 117330)  frac = 0.996473;
else if (runnumber == 117331)  frac = 1.00042;
else if (runnumber == 117332)  frac = 1.00385;
else if (runnumber == 117333)  frac = 0.995029;

else if (runnumber == 117335)  frac = 0.987182;
else if (runnumber == 117336)  frac = 1.0021;
else if (runnumber == 117340)  frac = 0.994011;
else if (runnumber == 117341)  frac = 1.00372;
else if (runnumber == 117342)  frac = 0.998009;
else if (runnumber == 117343)  frac = 0.995256;
else if (runnumber == 117344)  frac = 1.00386;
else if (runnumber == 117345)  frac = 0.990821;
else if (runnumber == 117346)  frac = 0.997322;
else if (runnumber == 117350)  frac = 0.995102;
else if (runnumber == 117351)  frac = 1.00322;
else if (runnumber == 117352)  frac = 1.00255;

else if (runnumber == 117356)  frac = 0.998025;
else if (runnumber == 117360)  frac = 0.994038;
else if (runnumber == 117361)  frac = 1.00316;
else if (runnumber == 117362)  frac = 0.994114;
else if (runnumber == 117363)  frac = 0.997215;
else if (runnumber == 117364)  frac = 1.00182;
else if (runnumber == 117365)  frac = 0.999941;
else if (runnumber == 117370)  frac = 0.996266;

else if (runnumber == 117372)  frac = 1.003;
else if (runnumber == 117373)  frac = 0.996895;
else if (runnumber == 117374)  frac = 1.00244;
else if (runnumber == 117375)  frac = 1.00104;
else if (runnumber == 117376)  frac = 0.995137;
else if (runnumber == 117380)  frac = 0.994951;
else if (runnumber == 117381)  frac = 1.00204;
else if (runnumber == 117382)  frac = 1.00991;
else if (runnumber == 117383)  frac = 1.00025;
else if (runnumber == 117384)  frac = 1.00303;
else if (runnumber == 117385)  frac = 1.001;
else if (runnumber == 117386)  frac = 1.00197;

else if (runnumber == 117391)  frac = 1.00353;
else if (runnumber == 117392)  frac = 1.00345;
else if (runnumber == 117393)  frac = 0.993738;
else if (runnumber == 117394)  frac = 1.00359;
else if (runnumber == 117395)  frac = 1.01838;
else if (runnumber == 117396)  frac = 1.00084;

else if (runnumber == 107280)  frac = 0.994461;
else if (runnumber == 107281)  frac = 0.993678;
else if (runnumber == 107282)  frac = 0.998292;
else if (runnumber == 107283)  frac = 1.00964;
else if (runnumber == 107660)  frac = 1.00989;
else if (runnumber == 107680)  frac = 0.993555;
else if (runnumber == 107690)  frac = 1.00323;
else if (runnumber == 107703)  frac = 0.999144;
else if (runnumber == 117293)  frac = 0.998648;
else if (runnumber == 117294)  frac = 0.998781;
else if (runnumber == 117295)  frac = 0.998505;
else if (runnumber == 117296)  frac = 1.00431;
else if (runnumber == 117297)  frac = 0.99105;
else if (runnumber == 117323)  frac = 0.999915;
else if (runnumber == 117334)  frac = 1.00099;
else if (runnumber == 117353)  frac = 1.00225;
else if (runnumber == 117354)  frac = 1.00332;
else if (runnumber == 117355)  frac = 1.00345;
else if (runnumber == 117371)  frac = 1.00331;
else if (runnumber == 117390)  frac = 0.996491;

    // else cout<<"EventWeight: Pileup correction run number not listed!! fraction is 1"<<endl;
    
    return frac;
}
