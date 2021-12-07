/******************************************************************************
 * CutElectronTrigger.hpp                                                               *
 *                                                                            *
 * Cuts on electron Number (can cut on All, UnIsolated, Tight, or Veto Electrons)     *
 * Must pass All, UnIsolated, Tight, or Veto to constructor                   *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutElectronTrigger class                                  *
 *    CutElectronTrigger()                     -- Parameterized Constructor             *
 *    ~CutElectronTrigger()                    -- Destructor                            *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutElectronTrigger"                    *
 *                                                                            *
 * Private Data Members of CutElectronTrigger class                                     *
 *    myTH1F* _hElectronNumberBefore;    -- Hist ele Number before cut             *
 *    myTH1F* _hElectronNumberAfter;     -- Hist ele Number of jets after cut      *
 *                                                                            *
 *    Int_t _ElectronNumberMin;          -- Minimum Electron Number                   *
 *    Int_t _ElectronNumberMax;          -- Maximum Electron Number                   *
 *                                                                            *
 * History                                                                    *
 *      15 Jan 2007 - Created by P. Ryan                                      *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Electron/CutElectronTrigger.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * CutElectronTrigger::CutElectronTrigger(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutElectronTrigger::CutElectronTrigger(EventContainer *EventContainerObj, TString electronTypePassed)
{
  // Check electronType parameter
  EventContainerObj->IsValidCollection("Electron",electronTypePassed);
  electronType = electronTypePassed;

  // Set Event Container
  SetEventContainer(EventContainerObj);
} // CutElectronTrigger


/******************************************************************************
 * CutElectronTrigger::~CutElectronTrigger()                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutElectronTrigger::~CutElectronTrigger()
{
  
}//~CutElectronTrigger

/******************************************************************************
 * void CutElectronTrigger::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutElectronTrigger::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // No histograms

  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // No Histrograms

  // ***********************************************
  // Get cuts from configuration file
  // ***********************************************  

  // Maybe change this to be configurable somehow?
  
  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************
  ostringstream cutFlowTitleStream;
  ostringstream cutFlowNameStream;
  TString cutFlowTitle;
  TString cutFlowName;

  // If it passes the cut
  cutFlowTitleStream << electronType.Data() << " Trigger match pass";
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream << electronType.Data() << "Electron.TriggerMatch";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());
  
}//BookHistograms()

/******************************************************************************
 * Bool_t CutElectronTrigger::Apply()                                                *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the number of electrons and the p_T of these electrons              *
 * Apply separate p_T cuts to the first and second (don't worry about others) *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutElectronTrigger::Apply()
{

  // ***********************************************
  // Get Electrons 
  // ***********************************************
  
  // Get Event Container
  EventContainer *EventContainerObj = GetEventContainer();

  // Get variables from EventContainerObj - Depens on electron Type                                                                                                                                                                                                                                                            
  std::vector<Electron> electrons = EventContainerObj->GetElectronCollection(electronType);

  Bool_t passesTriggerPt = kFALSE;

  //Check that one electron is trigger matched
  
  for (auto electron: electrons){
    if (electron.isTriggerMatchedEle() && electron.Pt() > 37) passesTriggerPt = kTRUE;
  }

  //Basic method of pt selection
  /*Float_t minElePt = 0.;

  //There is scope here for a better version of this...
  if (EventContainerObj->triggerBits["HLT_IsoEle24"] ) minElePt = 26.;
  else if (EventContainerObj->triggerBits["HLT_IsoEle27"] ) minElePt = 29.;

  //There is scope here for better trigger matching
  for (auto electron : electrons) if (electron.Pt() > minElePt) passesTriggerPt = kTRUE;
  */

  ostringstream cutFlowNameStream;
  cutFlowNameStream << electronType.Data() << "Electron.TriggerMatch";


  if (passesTriggerPt) GetCutFlowTable() -> PassCut(cutFlowNameStream.str().c_str());
  else GetCutFlowTable() -> FailCut(cutFlowNameStream.str().c_str());

  return passesTriggerPt;
 
} //Apply












