/******************************************************************************
 * CutTriggerSelection.cpp                                                    *
 *                                                                            *
 * Cuts on the trigger required per channel                                   *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutTriggerSelection class                              *
 *    CutTriggerSelection()                     -- Parameterized Constructor         *
 *    ~CutTriggerSelection()                    -- Destructor                        *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutTriggerSelection"                *
 *                                                                            *
 * Private Data Members of CutTriggerSelection class                                 *
 *    myTH1F* _hTriggerBitBefore;    -- Hist of trigger bit before cut        *
 *    myTH1F* _hTriggerBitAfter;     -- Hist of trigger bit after cut         *
 *                                                                            *
 * History                                                                    *
 *      4th July 2016 - Created by Duncan Leggat                              *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Other/CutTriggerSelection.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * CutTriggerSelection::CutTriggerSelection(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 *         whichTrigger says which channel to cut on
 *         0 - Electron, 1 - Muon
 * Output: None                                                               *
 ******************************************************************************/
CutTriggerSelection::CutTriggerSelection(EventContainer *EventContainerObj)
{
  // Set Event Container
  SetEventContainer(EventContainerObj);
} // CutTriggerSelection


/******************************************************************************
 * CutTriggerSelection::~CutTriggerSelection()                                              *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutTriggerSelection::~CutTriggerSelection()
{
  
}//~CutTriggerSelection

/******************************************************************************
 * void CutTriggerSelection::BookHistogram()                                         *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutTriggerSelection::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // Histogram Before Cut
  std::ostringstream histNameBeforeStream;
  histNameBeforeStream  << "TriggerSelectionBefore";
  TString histNameBefore = histNameBeforeStream.str().c_str();

  std::ostringstream histTitleBeforeStream;
  histTitleBeforeStream  << " Channel Trigger Before Cut";
  TString histTitleBefore = histTitleBeforeStream.str().c_str();

  // Histogram After Cut
  std::ostringstream histNameAfterStream;
  histNameAfterStream  << "TriggerSelectionAfter";
  TString histNameAfter = histNameAfterStream.str().c_str();

  std::ostringstream histTitleAfterStream;
  histTitleAfterStream  << " Channel Trigger After Cut";
  TString histTitleAfter = histTitleAfterStream.str().c_str();

  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // Histogram before cut
  _hTriggerSelectionBefore =  DeclareTH1F(histNameBefore.Data(), histTitleBefore.Data(), 2, 0.0, 2.0);
  _hTriggerSelectionBefore -> SetXAxisTitle("TriggerBit");
  _hTriggerSelectionBefore -> SetYAxisTitle("Events");

  // Histogram after cut
  _hTriggerSelectionAfter =  DeclareTH1F(histNameAfter.Data(), histTitleAfter.Data(), 2, 0.0, 2.0);
  _hTriggerSelectionAfter -> SetXAxisTitle("TriggerBit");
  _hTriggerSelectionAfter -> SetYAxisTitle("Events");

  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************
  ostringstream cutFlowTitleStream;
  ostringstream cutFlowNameStream;
  TString cutFlowTitle;
  TString cutFlowName;

  // Min cut
  cutFlowTitleStream <<  " Trigger";
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream  << "Trigger";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  //Get the trigger path from the config
  // Get configuration file                               
  EventContainer *EventContainerObj = GetEventContainer();
  TEnv *config = EventContainerObj -> GetConfig();        

  //We can specify comma separated lists of triggers to run as ORs and NOTs
  TString orTriggers = config -> GetValue("Trigger.Name.Or", "");
  TString notTriggers = config -> GetValue("Trigger.Name.Not", "");

  TObjArray *orTrigs = orTriggers.Tokenize(",");
  for (Int_t i = 0; i < orTrigs->GetEntries(); i++){
    // Double check that the given path is in the HLT information giveb
    TString tmpName = ((TObjString *)(orTrigs->At(i)))->String();
    if ( EventContainerObj->triggerBits.count( tmpName ) ){
      _triggerOrs.push_back(tmpName);
    }
    else {
      std::cout << "<CutTriggerSelection::BookHistogram>  | Trigger path " << tmpName << " not found in tree but included in config - ignoring this trigger name, but please check!" << std::endl;
    }
  }

  TObjArray *notTrigs = notTriggers.Tokenize(",");
  for (Int_t i = 0; i < notTrigs->GetEntries(); i++){
    TString tmpName = ((TObjString *)(notTrigs->At(i)))->String();
    if ( EventContainerObj->triggerBits.count( tmpName ) ){
      _triggerNots.push_back(tmpName);
    }
    else {
      std::cout << "<CutTriggerSelection::BookHistogram>  | Trigger path " << tmpName << " not found in tree but included in config - ignoring this trigger name, but please check!" << std::endl;
    }
  }

}//BookHistograms()

/******************************************************************************
 * Bool_t CutTriggerSelection::Apply()                                               *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the selected trigger                                                *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutTriggerSelection::Apply()
{

  Bool_t passesTrigger = kFALSE;  //Event passes the trigger selection

  EventContainer *EventContainerObj = GetEventContainer();

  // Apply the trigger or and nots
  for (auto trigName : _triggerOrs  ) passesTrigger = passesTrigger || EventContainerObj->triggerBits[trigName];
  for (auto trigName : _triggerNots ) passesTrigger = passesTrigger && !(EventContainerObj->triggerBits[trigName]);

  int histBinToFill = 0;
  if (passesTrigger) histBinToFill = 1;
  // Fill the histograms before the cuts
  _hTriggerSelectionBefore -> Fill(histBinToFill);
 
  // ***********************************************
  // Fill cut flow table
  // ***********************************************
  
  // Names for Cut Flow Table
  ostringstream cutFlowNameStream;
  
  TString cutFlowName;
  
  cutFlowNameStream  << "Trigger";
  cutFlowName = cutFlowNameStream.str().c_str();
  
  if (passesTrigger){
    _hTriggerSelectionAfter -> Fill(histBinToFill);
    GetCutFlowTable()->PassCut(cutFlowName.Data());
    return kTRUE;
  }
  else{
    GetCutFlowTable()->FailCut(cutFlowName.Data());
    return kFALSE;
  }

} //Apply












