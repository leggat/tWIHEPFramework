/******************************************************************************
 * CutDiMuon.hpp                                                               *
 *                                                                            *
 * Cuts on dimuon pair properties                                             *
 * Initially this will be on dR and combined mass window, others may follow   *
 * Desired muon collection (tight, veto etc) is passed in the constructor     *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutDiMuon class                                 *
 *    CutDiMuon()                     -- Parameterized Constructor            *
 *    ~CutDiMuon()                    -- Destructor                           *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutDiMuon"                   *
 *                                                                            *
 * Private Data Members of CutDiMuon class                                    *
 *    myTH1F* _hDiMuonDRBefore;    -- Hist mu pair delta r before cut         *
 *    myTH1F* _hDiMuonDRAfter;     -- Hist mu pair delta r after cut          *
 *    myTH1F* _hDiMuonMassBefore;    -- Hist mu pair mass before cut          *
 *    myTH1F* _hDiMuonMassAfter;     -- Hist mu pair mass after cut           *
 *                                                                            *
 *    Int_t _DiMuonDRMin;            -- Minimum delta r between mu pair       *
 *    Int_t _DiMuonDRMax;            -- Maximum delta r between mu pair       *
 *    Int_t _DiMuonMassMin;          -- Minimum mu pair mass                  *
 *    Int_t _DiMuonMassMax;          -- Maximum mu pair mass                  *
 *                                                                            *
 * History                                                                    *
 *      5 Nov 2021 - Created by D. Leggat                                     *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Muon/CutDiMuon.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * CutDiMuon::CutDiMuon(EventContainer *EventContainerObj, TString muonType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutDiMuon::CutDiMuon(EventContainer *EventContainerObj, TString muonTypePassed)
{
  // Check muonType parameter
  if( muonTypePassed.CompareTo("All") && muonTypePassed.CompareTo("UnIsolated") && muonTypePassed.CompareTo("Isolated") && 
      muonTypePassed.CompareTo("Tight") && muonTypePassed.CompareTo("PtEtaCut") && muonTypePassed.CompareTo("Veto") &&
      muonTypePassed.CompareTo("MetMu") && muonTypePassed.CompareTo("TriggerMatch") ){
    std::cout << "ERROR " << "<CutDiMuon::CutDiMuon()> " 
	      << "Must pass All, Tight, PtEtaCut, Veto, Isolated, MetMu, TriggerMatch or UnIsolated to constructor" << std::endl;
    exit(8);
  } //if
  muonType = muonTypePassed;

  // Set Event Container
  SetEventContainer(EventContainerObj);
} // CutDiMuon


/******************************************************************************
 * CutDiMuon::~CutDiMuon()                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutDiMuon::~CutDiMuon()
{
  
}//~CutDiMuon

/******************************************************************************
 * void CutDiMuon::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutDiMuon::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // DR Before Cut
  std::ostringstream histNameBeforeStreamDR;
  histNameBeforeStreamDR << muonType << "DiMuonDRBefore";
  TString histNameBeforeDR = histNameBeforeStreamDR.str().c_str();

  std::ostringstream histTitleBeforeStreamDR;
  histTitleBeforeStreamDR << muonType << "Muon pair dR before cut";
  TString histTitleBeforeDR = histTitleBeforeStreamDR.str().c_str();

  // DR After Cut
  std::ostringstream histNameAfterStreamDR;
  histNameAfterStreamDR << muonType << "DiMuonDRAfter";
  TString histNameAfterDR = histNameAfterStreamDR.str().c_str();

  std::ostringstream histTitleAfterStreamDR;
  histTitleAfterStreamDR << muonType << "Muon pair dR after cut";
  TString histTitleAfterDR = histTitleAfterStreamDR.str().c_str();

  // Mass Before Cut
  std::ostringstream histNameBeforeStreamMass;
  histNameBeforeStreamMass << muonType << "DiMuonMassBefore";
  TString histNameBeforeMass = histNameBeforeStreamMass.str().c_str();

  std::ostringstream histTitleBeforeStreamMass;
  histTitleBeforeStreamMass << muonType << "Muon pair mass before cut";
  TString histTitleBeforeMass = histTitleBeforeStreamMass.str().c_str();

  // Mass After Cut
  std::ostringstream histNameAfterStreamMass;
  histNameAfterStreamMass << muonType << "DiMuonMassAfter";
  TString histNameAfterMass = histNameAfterStreamMass.str().c_str();

  std::ostringstream histTitleAfterStreamMass;
  histTitleAfterStreamMass << muonType << "Muon pair mass after cut";
  TString histTitleAfterMass = histTitleAfterStreamMass.str().c_str();

  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // dR Histogram before cut
  _hDiMuonDRBefore =  DeclareTH1F(histNameBeforeDR.Data(), histTitleBeforeDR.Data(), 100, 0.0, 10.0);
  _hDiMuonDRBefore -> SetXAxisTitle("#Delta (R)_{#mu#mu}");
  _hDiMuonDRBefore -> SetYAxisTitle("Events");

  // dR Histogram after cut
  _hDiMuonDRAfter =  DeclareTH1F(histNameAfterDR.Data(), histTitleAfterDR.Data(), 100, 0.0, 10);
  _hDiMuonDRAfter -> SetXAxisTitle("#Delta (R)_{#mu#mu}");
  _hDiMuonDRAfter -> SetYAxisTitle("Events");


  // mass Histogram before cut
  _hDiMuonMassBefore =  DeclareTH1F(histNameBeforeMass.Data(), histTitleBeforeMass.Data(), 100, 0.0, 150.0);
  _hDiMuonMassBefore -> SetXAxisTitle("m_{#mu#mu}");
  _hDiMuonMassBefore -> SetYAxisTitle("Events");

  // mass Histogram after cut
  _hDiMuonMassAfter =  DeclareTH1F(histNameAfterMass.Data(), histTitleAfterMass.Data(), 100, 0.0, 150);
  _hDiMuonMassAfter -> SetXAxisTitle("m_{#mu#mu}");
  _hDiMuonMassAfter -> SetYAxisTitle("Events");

  // ***********************************************
  // Get cuts from configuration file
  // ***********************************************  

  // Get configuration file
  EventContainer *EventContainerObj = GetEventContainer();
  TEnv *config = EventContainerObj -> GetConfig();

  // Make name in configuration file depending upon muon type (all, veto, tight, unIsolated)
  std::ostringstream configDRMinStream;
  configDRMinStream << "Cut.DiMuon." << muonType.Data() << ".DR.Min";
  TString configMinDR = configDRMinStream.str().c_str();

  std::ostringstream configDRMaxStream;
  configDRMaxStream << "Cut.DiMuon." << muonType.Data() << ".DR.Max";
  TString configMaxDR = configDRMaxStream.str().c_str();

  // Make name in configuration file depending upon muon type (all, veto, tight, unIsolated)
  std::ostringstream configMassMinStream;
  configMassMinStream << "Cut.DiMuon." << muonType.Data() << ".Mass.Min";
  TString configMinMass = configMassMinStream.str().c_str();

  std::ostringstream configMassMaxStream;
  configMassMaxStream << "Cut.DiMuon." << muonType.Data() << ".Mass.Max";
  TString configMaxMass = configMassMaxStream.str().c_str();

  // Use configuration to set min and max number of jets to cut on
  _DiMuonDRMin   = config -> GetValue(configMinDR.Data(),   999.);
  _DiMuonDRMax   = config -> GetValue(configMaxDR.Data(),   999.);
  _DiMuonMassMin = config -> GetValue(configMinMass.Data(), 999.);
  _DiMuonMassMax = config -> GetValue(configMaxMass.Data(), 999.);

  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************
  ostringstream cutFlowTitleStream;
  ostringstream cutFlowNameStream;
  TString cutFlowTitle;
  TString cutFlowName;

  // Min cut
  cutFlowTitleStream << muonType.Data() << " DiMuon : " << "dR >= " << _DiMuonDRMin;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream << muonType.Data() << "DiMuon.DR.Min";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Max cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << muonType.Data() << " DiMuon : " << "dR <= " << _DiMuonDRMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();
  
  cutFlowNameStream.str("");
  cutFlowNameStream << muonType.Data() << "DiMuon.DR.Max";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Min + Max cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << muonType.Data() << " DiMuon : " << _DiMuonDRMin << " <= dR <= " << _DiMuonDRMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << muonType.Data() << "DiMuon.DR.All";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName, cutFlowTitle);

  // Min cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << muonType.Data() << " DiMuon : " << "mass >= " << _DiMuonMassMin;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << muonType.Data() << "DiMuon.Mass.Min";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Max cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << muonType.Data() << " DiMuon : " << "mass <= " << _DiMuonMassMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();
  
  cutFlowNameStream.str("");
  cutFlowNameStream << muonType.Data() << "DiMuon.Mass.Max";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Min + Max cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << muonType.Data() << " DiMuon : " << _DiMuonMassMin << " <= mass <= " << _DiMuonMassMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << muonType.Data() << "DiMuon.Mass.All";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName, cutFlowTitle);

  // Min + Max mass and dr
  cutFlowTitleStream.str("");
  cutFlowTitleStream << muonType.Data() << " DiMuon : " << _DiMuonMassMin << " <= mass <= " << _DiMuonMassMax << " and " << _DiMuonDRMin << " <= dR <= " << _DiMuonDRMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << muonType.Data() << "DiMuon.All.All";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName, cutFlowTitle);

  
}//BookHistograms()

/******************************************************************************
 * Bool_t CutDiMuon::Apply()                                                *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the number of electrons and the p_T of these electrons              *
 * Apply separate p_T cuts to the first and second (don't worry about others) *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutDiMuon::Apply()
{

  // ***********************************************
  // Get Number of Muons and fill histograms
  // ***********************************************
  
  // Initialize cut parameters
  Float_t DiMuonDR  = 0.;       
  Float_t DiMuonMass = 0.;

  // Flags 
  Bool_t MuonDRMinPass = kTRUE;   // Event passes min pT muon cuts
  Bool_t MuonDRMaxPass = kTRUE;   // Event passes max pT muon cuts
  Bool_t MuonMassMinPass = kTRUE;   // Event passes min pT muon cuts
  Bool_t MuonMassMaxPass = kTRUE;   // Event passes max pT muon cuts
  
  // Get Event Container
  EventContainer *EventContainerObj = GetEventContainer();

  // Get variables from EventContainerObj - Depens on muon Type
  std::vector<Muon> muons = EventContainerObj->GetMuonCollection(muonType);

  if (muons.size() > 1) { //This ideally wouldn't be possible, but need the check to avoid potential segfaults from misuse. Will fail the cut if there aren't 2 muons
    //    std::cout << "Gets into the muon vcalculation loop" << std::endl;
    DiMuonDR = muons[0].DeltaR(muons[1]);
    Muon tmpMuon = muons[0];
    tmpMuon+= muons[1];
    DiMuonMass = (tmpMuon).M();
  }

  // Fill the histograms before the cuts
  _hDiMuonDRBefore    -> Fill(DiMuonDR);
  _hDiMuonDRAfter     -> Fill(DiMuonMass);

  // Evaluate cuts
  MuonDRMinPass = DiMuonDR > _DiMuonDRMin;
  MuonDRMaxPass = DiMuonDR < _DiMuonDRMax;
  MuonMassMinPass = DiMuonMass > _DiMuonMassMin;
  MuonMassMaxPass = DiMuonMass < _DiMuonMassMax;


  // ***********************************************   
  // Add to the cut flow tables
  // ***********************************************   

  ostringstream cutFlowNameSt;
  TString cutFlowName;

  // DR cuts

  cutFlowNameSt.str("");
  cutFlowNameSt << muonType.Data() << "DiMuon.DR.Min";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (MuonDRMinPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  cutFlowNameSt.str("");
  cutFlowNameSt << muonType.Data() << "DiMuon.DR.Max";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (MuonDRMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  cutFlowNameSt.str("");
  cutFlowNameSt << muonType.Data() << "DiMuon.DR.All";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (MuonDRMinPass && MuonDRMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  //Mass cuts

  cutFlowNameSt.str("");
  cutFlowNameSt << muonType.Data() << "DiMuon.Mass.Min";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (MuonMassMinPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  cutFlowNameSt.str("");
  cutFlowNameSt << muonType.Data() << "DiMuon.Mass.Max";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (MuonMassMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  cutFlowNameSt.str("");
  cutFlowNameSt << muonType.Data() << "DiMuon.Mass.All";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (MuonMassMinPass && MuonMassMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }


  //Both cuts
  cutFlowNameSt.str("");                                
  cutFlowNameSt << muonType.Data() << "DiMuon.All.All";
  cutFlowName = cutFlowNameSt.str().c_str();            
  if (MuonMassMinPass && MuonMassMaxPass && MuonDRMinPass && MuonDRMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
    _hDiMuonMassAfter -> Fill(DiMuonMass);
    _hDiMuonDRAfter   -> Fill(DiMuonDR);
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  return (MuonMassMinPass && MuonMassMaxPass && MuonDRMinPass && MuonDRMaxPass);
 
} //Apply












