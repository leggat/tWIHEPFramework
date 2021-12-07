/******************************************************************************
 * CutDiElectron.hpp                                                               *
 *                                                                            *
 * Cuts on dielectron pair properties                                             *
 * Initially this will be on dR and combined mass window, others may follow   *
 * Desired electron collection (tight, veto etc) is passed in the constructor     *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutDiElectron class                                 *
 *    CutDiElectron()                     -- Parameterized Constructor            *
 *    ~CutDiElectron()                    -- Destructor                           *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutDiElectron"                   *
 *                                                                            *
 * Private Data Members of CutDiElectron class                                    *
 *    myTH1F* _hDiElectronDRBefore;    -- Hist ele pair delta r before cut         *
 *    myTH1F* _hDiElectronDRAfter;     -- Hist ele pair delta r after cut          *
 *    myTH1F* _hDiElectronMassBefore;    -- Hist ele pair mass before cut          *
 *    myTH1F* _hDiElectronMassAfter;     -- Hist ele pair mass after cut           *
 *                                                                            *
 *    Int_t _DiElectronDRMin;            -- Minielem delta r between ele pair       *
 *    Int_t _DiElectronDRMax;            -- Maxielem delta r between ele pair       *
 *    Int_t _DiElectronMassMin;          -- Minielem ele pair mass                  *
 *    Int_t _DiElectronMassMax;          -- Maxielem ele pair mass                  *
 *                                                                            *
 * History                                                                    *
 *      5 Nov 2021 - Created by D. Leggat                                     *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Electron/CutDiElectron.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * CutDiElectron::CutDiElectron(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutDiElectron::CutDiElectron(EventContainer *EventContainerObj, TString electronTypePassed)
{
  // Check electronType parameter
  EventContainerObj->IsValidCollection("Electron",electronTypePassed);
  electronType = electronTypePassed;

  // Set Event Container
  SetEventContainer(EventContainerObj);
} // CutDiElectron


/******************************************************************************
 * CutDiElectron::~CutDiElectron()                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutDiElectron::~CutDiElectron()
{
  
}//~CutDiElectron

/******************************************************************************
 * void CutDiElectron::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutDiElectron::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // DR Before Cut
  std::ostringstream histNameBeforeStreamDR;
  histNameBeforeStreamDR << electronType << "DiElectronDRBefore";
  TString histNameBeforeDR = histNameBeforeStreamDR.str().c_str();

  std::ostringstream histTitleBeforeStreamDR;
  histTitleBeforeStreamDR << electronType << "Electron pair dR before cut";
  TString histTitleBeforeDR = histTitleBeforeStreamDR.str().c_str();

  // DR After Cut
  std::ostringstream histNameAfterStreamDR;
  histNameAfterStreamDR << electronType << "DiElectronDRAfter";
  TString histNameAfterDR = histNameAfterStreamDR.str().c_str();

  std::ostringstream histTitleAfterStreamDR;
  histTitleAfterStreamDR << electronType << "Electron pair dR after cut";
  TString histTitleAfterDR = histTitleAfterStreamDR.str().c_str();

  // Mass Before Cut
  std::ostringstream histNameBeforeStreamMass;
  histNameBeforeStreamMass << electronType << "DiElectronMassBefore";
  TString histNameBeforeMass = histNameBeforeStreamMass.str().c_str();

  std::ostringstream histTitleBeforeStreamMass;
  histTitleBeforeStreamMass << electronType << "Electron pair mass before cut";
  TString histTitleBeforeMass = histTitleBeforeStreamMass.str().c_str();

  // Mass After Cut
  std::ostringstream histNameAfterStreamMass;
  histNameAfterStreamMass << electronType << "DiElectronMassAfter";
  TString histNameAfterMass = histNameAfterStreamMass.str().c_str();

  std::ostringstream histTitleAfterStreamMass;
  histTitleAfterStreamMass << electronType << "Electron pair mass after cut";
  TString histTitleAfterMass = histTitleAfterStreamMass.str().c_str();

  // Pt before cut
  std::ostringstream histNameBeforeStreamPt;                          
  histNameBeforeStreamPt << electronType << "DiElectronPtBefore";           
  TString histNameBeforePt = histNameBeforeStreamPt.str().c_str();  
                                                                      
  std::ostringstream histTitleBeforeStreamPt;                         
  histTitleBeforeStreamPt << electronType << "Electron pair pt before cut"; 
  TString histTitleBeforePt = histTitleBeforeStreamPt.str().c_str();
                                                                      
  // Pt After Cut                                                     
  std::ostringstream histNameAfterStreamPt;                           
  histNameAfterStreamPt << electronType << "DiElectronPtAfter";             
  TString histNameAfterPt = histNameAfterStreamPt.str().c_str();    
                                                                      
  std::ostringstream histTitleAfterStreamPt;                          
  histTitleAfterStreamPt << electronType << "Electron pair pt after cut";   
  TString histTitleAfterPt = histTitleAfterStreamPt.str().c_str();  


  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // dR Histogram before cut
  _hDiElectronDRBefore =  DeclareTH1F(histNameBeforeDR.Data(), histTitleBeforeDR.Data(), 100, 0.0, 10.0);
  _hDiElectronDRBefore -> SetXAxisTitle("#Delta (R)_{#ele#ele}");
  _hDiElectronDRBefore -> SetYAxisTitle("Events");

  // dR Histogram after cut
  _hDiElectronDRAfter =  DeclareTH1F(histNameAfterDR.Data(), histTitleAfterDR.Data(), 100, 0.0, 10);
  _hDiElectronDRAfter -> SetXAxisTitle("#Delta (R)_{#ele#ele}");
  _hDiElectronDRAfter -> SetYAxisTitle("Events");


  // mass Histogram before cut
  _hDiElectronMassBefore =  DeclareTH1F(histNameBeforeMass.Data(), histTitleBeforeMass.Data(), 100, 0.0, 150.0);
  _hDiElectronMassBefore -> SetXAxisTitle("m_{#ele#ele}");
  _hDiElectronMassBefore -> SetYAxisTitle("Events");

  // mass Histogram before cut
  _hDiElectronPtBefore =  DeclareTH1F(histNameBeforePt.Data(), histTitleBeforePt.Data(), 100, 0.0, 150.0);
  _hDiElectronPtBefore -> SetXAxisTitle("Pt_{#ele#ele}");
  _hDiElectronPtBefore -> SetYAxisTitle("Events");

  // mass Histogram after cut
  _hDiElectronPtAfter =  DeclareTH1F(histNameAfterPt.Data(), histTitleAfterPt.Data(), 100, 0.0, 150);
  _hDiElectronPtAfter -> SetXAxisTitle("Pt_{#ele#ele}");
  _hDiElectronPtAfter -> SetYAxisTitle("Events");

  // ***********************************************
  // Get cuts from configuration file
  // ***********************************************  

  // Get configuration file
  EventContainer *EventContainerObj = GetEventContainer();
  TEnv *config = EventContainerObj -> GetConfig();

  // Make name in configuration file depending upon electron type (all, veto, tight, unIsolated)
  std::ostringstream configDRMinStream;
  configDRMinStream << "Cut.DiElectron." << electronType.Data() << ".DR.Min";
  TString configMinDR = configDRMinStream.str().c_str();

  std::ostringstream configDRMaxStream;
  configDRMaxStream << "Cut.DiElectron." << electronType.Data() << ".DR.Max";
  TString configMaxDR = configDRMaxStream.str().c_str();

  // Make name in configuration file depending upon electron type (all, veto, tight, unIsolated)
  std::ostringstream configMassMinStream;
  configMassMinStream << "Cut.DiElectron." << electronType.Data() << ".Mass.Min";
  TString configMinMass = configMassMinStream.str().c_str();

  std::ostringstream configMassMaxStream;
  configMassMaxStream << "Cut.DiElectron." << electronType.Data() << ".Mass.Max";
  TString configMaxMass = configMassMaxStream.str().c_str();

  // Use configuration to set min and max number of jets to cut on
  _DiElectronDRMin   = config -> GetValue(configMinDR.Data(),   999.);
  _DiElectronDRMax   = config -> GetValue(configMaxDR.Data(),   999.);
  _DiElectronMassMin = config -> GetValue(configMinMass.Data(), 999.);
  _DiElectronMassMax = config -> GetValue(configMaxMass.Data(), 999.);


  //Defining the after histograms after the cuts so that we can use that information in the binning
  // mass Histogram after cut
  _hDiElectronMassAfter =  DeclareTH1F(histNameAfterMass.Data(), histTitleAfterMass.Data(), 100, float(_DiElectronMassMin), float(_DiElectronMassMax));
  _hDiElectronMassAfter -> SetXAxisTitle("m_{#ele#ele}");
  _hDiElectronMassAfter -> SetYAxisTitle("Events");



  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************
  ostringstream cutFlowTitleStream;
  ostringstream cutFlowNameStream;
  TString cutFlowTitle;
  TString cutFlowName;

  // Min cut
  cutFlowTitleStream << electronType.Data() << " DiElectron : " << "dR >= " << _DiElectronDRMin;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream << electronType.Data() << "DiElectron.DR.Min";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Max cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << electronType.Data() << " DiElectron : " << "dR <= " << _DiElectronDRMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();
  
  cutFlowNameStream.str("");
  cutFlowNameStream << electronType.Data() << "DiElectron.DR.Max";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Min + Max cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << electronType.Data() << " DiElectron : " << _DiElectronDRMin << " <= dR <= " << _DiElectronDRMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << electronType.Data() << "DiElectron.DR.All";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName, cutFlowTitle);

  // Min cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << electronType.Data() << " DiElectron : " << "mass >= " << _DiElectronMassMin;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << electronType.Data() << "DiElectron.Mass.Min";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Max cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << electronType.Data() << " DiElectron : " << "mass <= " << _DiElectronMassMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();
  
  cutFlowNameStream.str("");
  cutFlowNameStream << electronType.Data() << "DiElectron.Mass.Max";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Min + Max cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << electronType.Data() << " DiElectron : " << _DiElectronMassMin << " <= mass <= " << _DiElectronMassMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << electronType.Data() << "DiElectron.Mass.All";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName, cutFlowTitle);

  // Min + Max mass and dr
  cutFlowTitleStream.str("");
  cutFlowTitleStream << electronType.Data() << " DiElectron : " << _DiElectronMassMin << " <= mass <= " << _DiElectronMassMax << " and " << _DiElectronDRMin << " <= dR <= " << _DiElectronDRMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << electronType.Data() << "DiElectron.All.All";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName, cutFlowTitle);

  
}//BookHistograms()

/******************************************************************************
 * Bool_t CutDiElectron::Apply()                                                *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the number of electrons and the p_T of these electrons              *
 * Apply separate p_T cuts to the first and second (don't worry about others) *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutDiElectron::Apply()
{

  // ***********************************************
  // Get Number of Electrons and fill histograms
  // ***********************************************
  
  // Initialize cut parameters
  Float_t DiElectronDR  = 0.;       
  Float_t DiElectronMass = 0.;

  // Extra parameters to plot only
  Float_t DiElectronPt = 0.;

  // Flags 
  Bool_t ElectronDRMinPass = kTRUE;   // Event passes min pT electron cuts
  Bool_t ElectronDRMaxPass = kTRUE;   // Event passes max pT electron cuts
  Bool_t ElectronMassMinPass = kTRUE;   // Event passes min pT electron cuts
  Bool_t ElectronMassMaxPass = kTRUE;   // Event passes max pT electron cuts
  
  // Get Event Container
  EventContainer *EventContainerObj = GetEventContainer();

  // Get variables from EventContainerObj - Depens on electron Type
  std::vector<Electron> electrons = EventContainerObj->GetElectronCollection(electronType);

  if (electrons.size() > 1) { //This ideally wouldn't be possible, but need the check to avoid potential segfaults from misuse. Will fail the cut if there aren't 2 electrons
    //    std::cout << "Gets into the electron vcalculation loop" << std::endl;
    DiElectronDR = electrons[0].DeltaR(electrons[1]);
    Electron tmpElectron = electrons[0];
    tmpElectron+= electrons[1];
    DiElectronMass = (tmpElectron).M();
    DiElectronPt = tmpElectron.Pt();
  }

  // Fill the histograms before the cuts
  _hDiElectronDRBefore     -> Fill(DiElectronDR);
  _hDiElectronMassBefore   -> Fill(DiElectronMass);
  _hDiElectronPtBefore     -> Fill(DiElectronPt);

  // Evaluate cuts
  ElectronDRMinPass = DiElectronDR > _DiElectronDRMin;
  ElectronDRMaxPass = DiElectronDR < _DiElectronDRMax;
  ElectronMassMinPass = DiElectronMass > _DiElectronMassMin;
  ElectronMassMaxPass = DiElectronMass < _DiElectronMassMax;


  // ***********************************************   
  // Add to the cut flow tables
  // ***********************************************   

  ostringstream cutFlowNameSt;
  TString cutFlowName;

  // DR cuts

  cutFlowNameSt.str("");
  cutFlowNameSt << electronType.Data() << "DiElectron.DR.Min";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (ElectronDRMinPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  cutFlowNameSt.str("");
  cutFlowNameSt << electronType.Data() << "DiElectron.DR.Max";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (ElectronDRMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  cutFlowNameSt.str("");
  cutFlowNameSt << electronType.Data() << "DiElectron.DR.All";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (ElectronDRMinPass && ElectronDRMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  //Mass cuts

  cutFlowNameSt.str("");
  cutFlowNameSt << electronType.Data() << "DiElectron.Mass.Min";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (ElectronMassMinPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  cutFlowNameSt.str("");
  cutFlowNameSt << electronType.Data() << "DiElectron.Mass.Max";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (ElectronMassMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  cutFlowNameSt.str("");
  cutFlowNameSt << electronType.Data() << "DiElectron.Mass.All";
  cutFlowName = cutFlowNameSt.str().c_str();

  if (ElectronMassMinPass && ElectronMassMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }


  //Both cuts
  cutFlowNameSt.str("");                                
  cutFlowNameSt << electronType.Data() << "DiElectron.All.All";
  cutFlowName = cutFlowNameSt.str().c_str();            
  if (ElectronMassMinPass && ElectronMassMaxPass && ElectronDRMinPass && ElectronDRMaxPass){
    GetCutFlowTable() -> PassCut(cutFlowName.Data());
    _hDiElectronMassAfter -> Fill(DiElectronMass);
    _hDiElectronDRAfter   -> Fill(DiElectronDR);
    _hDiElectronPtAfter   -> Fill(DiElectronPt);
  }
  else{
    GetCutFlowTable() -> FailCut(cutFlowName.Data());
  }

  return (ElectronMassMinPass && ElectronMassMaxPass && ElectronDRMinPass && ElectronDRMaxPass);
 
} //Apply












