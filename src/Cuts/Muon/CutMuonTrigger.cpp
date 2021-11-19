/******************************************************************************
 * CutMuonTrigger.hpp                                                               *
 *                                                                            *
 * Cuts on muon Number (can cut on All, UnIsolated, Tight, or Veto Muons)     *
 * Must pass All, UnIsolated, Tight, or Veto to constructor                   *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutMuonTrigger class                                  *
 *    CutMuonTrigger()                     -- Parameterized Constructor             *
 *    ~CutMuonTrigger()                    -- Destructor                            *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutMuonTrigger"                    *
 *                                                                            *
 * Private Data Members of CutMuonTrigger class                                     *
 *    myTH1F* _hMuonNumberBefore;    -- Hist mu Number before cut             *
 *    myTH1F* _hMuonNumberAfter;     -- Hist mu Number of jets after cut      *
 *                                                                            *
 *    Int_t _MuonNumberMin;          -- Minimum Muon Number                   *
 *    Int_t _MuonNumberMax;          -- Maximum Muon Number                   *
 *                                                                            *
 * History                                                                    *
 *      15 Jan 2007 - Created by P. Ryan                                      *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Muon/CutMuonTrigger.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * CutMuonTrigger::CutMuonTrigger(EventContainer *EventContainerObj, TString muonType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutMuonTrigger::CutMuonTrigger(EventContainer *EventContainerObj, TString muonTypePassed)
{
  // Check muonType parameter
  if( muonTypePassed.CompareTo("All") && muonTypePassed.CompareTo("UnIsolated") && muonTypePassed.CompareTo("Isolated") && 
      muonTypePassed.CompareTo("Tight") && muonTypePassed.CompareTo("PtEtaCut") && muonTypePassed.CompareTo("Veto") &&
      muonTypePassed.CompareTo("MetMu") && muonTypePassed.CompareTo("TriggerMatch") ){
    std::cout << "ERROR " << "<CutMuonTrigger::CutMuonTrigger()> " 
	      << "Must pass All, Tight, PtEtaCut, Veto, Isolated, MetMu, TriggerMatch or UnIsolated to constructor" << std::endl;
    exit(8);
  } //if
  muonType = muonTypePassed;

  // Set Event Container
  SetEventContainer(EventContainerObj);
} // CutMuonTrigger


/******************************************************************************
 * CutMuonTrigger::~CutMuonTrigger()                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutMuonTrigger::~CutMuonTrigger()
{
  
}//~CutMuonTrigger

/******************************************************************************
 * void CutMuonTrigger::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutMuonTrigger::BookHistogram(){
  
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
  cutFlowTitleStream << muonType.Data() << " Trigger match pass";
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream << muonType.Data() << "Muon.TriggerMatch";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());
  
}//BookHistograms()

/******************************************************************************
 * Bool_t CutMuonTrigger::Apply()                                                *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the number of electrons and the p_T of these electrons              *
 * Apply separate p_T cuts to the first and second (don't worry about others) *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutMuonTrigger::Apply()
{

  // ***********************************************
  // Get Muons 
  // ***********************************************
  
  // Get Event Container
  EventContainer *EventContainerObj = GetEventContainer();

  // Get variables from EventContainerObj - Depens on muon Type                                                                                                                                                                                                                                                            
  std::vector<Muon> muons = EventContainerObj->GetMuonCollection(muonType);

  Bool_t passesTriggerPt = kFALSE;

  //Check that one muon is trigger matched
  
  for (auto muon: muons){
    if (muon.isTriggerMatchedMu() && muon.Pt() > 26) passesTriggerPt = kTRUE;
  }

  //Basic method of pt selection
  /*Float_t minMuPt = 0.;

  //There is scope here for a better version of this...
  if (EventContainerObj->triggerBits["HLT_IsoMu24"] ) minMuPt = 26.;
  else if (EventContainerObj->triggerBits["HLT_IsoMu27"] ) minMuPt = 29.;

  //There is scope here for better trigger matching
  for (auto muon : muons) if (muon.Pt() > minMuPt) passesTriggerPt = kTRUE;
  */

  ostringstream cutFlowNameStream;
  cutFlowNameStream << muonType.Data() << "Muon.TriggerMatch";


  if (passesTriggerPt) GetCutFlowTable() -> PassCut(cutFlowNameStream.str().c_str());
  else GetCutFlowTable() -> FailCut(cutFlowNameStream.str().c_str());

  return passesTriggerPt;
 
} //Apply












