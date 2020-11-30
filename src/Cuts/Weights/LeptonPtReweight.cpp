/******************************************************************************
 * LeptonPtReweight.hpp                                                            *
 *                                                                            *
 * Weight events in the histograms                                            *
 *                                                                            *
 * This class is derived from the BaseCut Class and simply                    *
 * cuts on if the event has reconstructed objects                             *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of AnalysisMain class                              *
 *    LeptonPtReweight()                     -- Parameterized Constructor          *
 *    ~LeptonPtReweight()                    -- Destructor                         *
 *    BookHistogram()                   -- Book histograms of weights         *
 *    Apply()                           -- Apply weight and fill histograms   *
 *    GetCutName()                      -- Returns "LeptonPtReweight"              *
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

#include "SingleTopRootAnalysis/Cuts/Weights/LeptonPtReweight.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <tuple>

using namespace std;


/******************************************************************************
 * LeptonPtReweight::LeptonPtReweight(EventContainer *obj)                              *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Particle class                                                     *
 * Output: None                                                               *
 ******************************************************************************/
LeptonPtReweight::LeptonPtReweight(EventContainer *EventContainerObj, TString leptonType, int regionInt, int isMuon):
  _electronType(leptonType),_isElectron(!isMuon)
{

  SetEventContainer(EventContainerObj);

  TString regionName = "";
  switch (regionInt) {
  case 0: regionName = "3j1t";
    break;
  case 1: regionName=  "3j2t";
    break;
  case 2: regionName=  "2j1t";
    break;
  case 3: regionName=  "4j1t";
    break;
  case 4: regionName=  "4j2t";
    break;
  default:
    regionName = "3j1t";
    break;
  }

  TString plotName = "ratio" + regionName;
  TEnv* conf = EventContainerObj -> GetConfig();
  
  _histogramForReweighting = NULL;

  TFile* reweightingFile = TFile::Open(conf -> GetValue("Include.leptonReweightFile","null"),"READ");
  if (!reweightingFile){
    std::cout << "No lepton pt reweighting file has been added." << std::endl;
    return;
  }
  _histogramForReweighting = (TH1F*)reweightingFile->Get(plotName);
  _histogramForReweighting->SetDirectory(0);
  reweightingFile->Close();
  delete reweightingFile;

}

/******************************************************************************
 * LeptonPtReweight::~LeptonPtReweight()                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
LeptonPtReweight::~LeptonPtReweight()
{
 //if(isPileUpWgt()) {
 // delete PileupReweighting; 
 //}
} //~LeptonPtReweight

/******************************************************************************
 * void LeptonPtReweight::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void LeptonPtReweight::BookHistogram()
{

  //cout<<"LeptonPtReweight::BookHistogram, debug level "<<obj->GetDebugLevel()<<endl;
  // Histogram of Global Weights

  _hLeptonReweight =  DeclareTH1F("LeptonPtReweight","Lepton Pt Reweight",100,-5.,5.);
  _hLeptonReweight -> SetXAxisTitle("Lepton Pt Weight");
  _hLeptonReweight -> SetYAxisTitle("Events");

  _hTotalReweight =  DeclareTH1F("Total reweight","Total Reweight",100,-7.,7.);
  _hTotalReweight -> SetXAxisTitle("Lepton Pt Weight");
  _hTotalReweight -> SetYAxisTitle("Events");

  _hEventWeight =  DeclareTH1F("EventWeightPostLep","Event Event Weight Lepton Pt Reweighted",100,-10.,10.);
  _hEventWeight -> SetXAxisTitle("Event Weight");
  _hEventWeight -> SetYAxisTitle("Events");

  _hEventPreWeight =  DeclareTH1F("EventWeightPreLepReweight","Event Event Weight before Lepton Pt Reweighting",100,-10.,10.);
  _hEventPreWeight -> SetXAxisTitle("Event Weight Pre Weighting");
  _hEventPreWeight -> SetYAxisTitle("Events");
}

/******************************************************************************
 * Bool_t LeptonPtReweight::Apply()                                                *
 *                                                                            *
 * Apply weights and fill histograms                                          *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t LeptonPtReweight::Apply()
{

  if (!_histogramForReweighting) return kTRUE;

  EventContainer *evc = GetEventContainer();
  // Set output event weight- no difference
  
  Double_t wgt = evc -> GetEventWeight();
  
    //Get the leptons
  std::vector<Particle> electronVector; //It's called electron vector, but aaaaactually we will also use it for muons. Woops.. 

  if(     "All"        == _electronType) {
    if (!_isElectron)    electronVector.assign(evc -> muons.begin(),           evc -> muons.end());
    else electronVector.assign(evc -> electrons.begin(),           evc -> electrons.end());
  }
  else if("Veto"       == _electronType) {
    if (!_isElectron)    electronVector.assign(evc -> vetoMuons.begin(),       evc -> vetoMuons.end());
    else electronVector.assign(evc -> vetoElectrons.begin(),       evc -> vetoElectrons.end());
  }
  else if("PtEtaCut"   == _electronType) {
    if (!_isElectron)    electronVector.assign(evc -> ptetaMuons.begin(),      evc -> ptetaMuons.end());
    else electronVector.assign(evc -> ptetaElectrons.begin(),      evc -> ptetaElectrons.end());
  }
  else if("Tight"      == _electronType) {
    if (!_isElectron)    electronVector.assign(evc -> tightMuons.begin(),      evc -> tightMuons.end());
    else electronVector.assign(evc -> tightElectrons.begin(),      evc -> tightElectrons.end());
  }
  else if("Isolated"   == _electronType) {
    if (!_isElectron)    electronVector.assign(evc -> isolatedMuons.begin(),   evc -> isolatedMuons.end());
    else electronVector.assign(evc -> isolatedElectrons.begin(),   evc -> isolatedElectrons.end());
  }
  else if("UnIsolated" == _electronType) {
    if (!_isElectron)    electronVector.assign(evc -> unIsolatedMuons.begin(), evc -> unIsolatedMuons.end());
    else electronVector.assign(evc -> unIsolatedElectrons.begin(), evc -> unIsolatedElectrons.end());
  }
  else{
    std::cout << "ERROR " << "<HistogramminElectron::Apply()> "
	      << "electronType must be All, Tight, Isolated, UnIsolated, or Veto, PtEtaCut" << std::endl;
    exit(8);
  } //else                                                                                                               
  Float_t leptonReweight = 1.;
  
  if (electronVector.size() > 0){
    Float_t electronPt = electronVector[0].Pt();
    leptonReweight = _histogramForReweighting->GetBinContent(_histogramForReweighting->GetXaxis()->FindBin(electronPt));
    
  }

  Double_t initialReweight = 1./evc->GeteventReweight();
  _hLeptonReweight->FillWithoutWeight(leptonReweight);
  _hTotalReweight->FillWithoutWeight(initialReweight * leptonReweight);
  evc->SeteventReweight(leptonReweight * initialReweight);
  _hEventPreWeight->FillWithoutWeight(wgt);
  wgt *= leptonReweight;

  

  //  std::cout << electronVector[0].Pt() << " " << _histogramForReweighting->GetXaxis()->FindBin( electronVector[0].Pt()) << " " << leptonReweight << " " << evc->GeteventReweight() << " " << initialReweight * leptonReweight << " " << initialReweight << std::endl;

  _hEventWeight->FillWithoutWeight(wgt);

  evc -> SetOutputEventWeight(wgt);


  return kTRUE;
  
} //Apply
