/******************************************************************************
 * VetoPrefireJets.hpp                                                                *
 *                                                                            *
 * Cuts on number of jets in an event                                         *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of CutNJet class                                   *
 *    VetoPrefireJets()                         -- Parameterized Constructor          *
 *    ~VetoPrefireJets()                        -- Destructor                         *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Apply cuts and fill histograms     *
 *    GetCutName()                      -- Returns "VetoPrefireJets"                  *
 *                                                                            *
 * Private Data Members of CutNJet class                                      *
 *    myTH1F* _hJetNumberBefore         -- Hist of Num of jets before cuts    *
 *    myTH1F* _hJetNumberAfter          -- Hist of Num of jets before cuts    *
 *                                                                            *
 * History                                                                    *
 *       7 Nov 2006 - Created by R. Schwienhorst for ATLAS                    *
 *      10 Nov 2006 - Modified by P. Ryan - cleanup and reorganized           *
 *      17 Nov 2006 - RS: add interface to cutFlowTable.                      *
 *      11 Jan 2006 - Modified by P. Ryan - cleanup and reorganized           *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Jet/VetoPrefireJets.hpp"
#include <iostream>
#include <sstream>

using namespace std;


/******************************************************************************
 * VetoPrefireJets::VetoPrefireJets(EventContainer *EventContainerObj)                      *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
VetoPrefireJets::VetoPrefireJets(EventContainer *EventContainerObj)
{
  SetEventContainer(EventContainerObj);
} // VetoPrefireJets


/******************************************************************************
 * VetoPrefireJets::~VetoPrefireJets()                                                      *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
VetoPrefireJets::~VetoPrefireJets()
{
  
}//~VetoPrefireJets

/******************************************************************************
 * void VetoPrefireJets::BookHistogram()                                             *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void VetoPrefireJets::BookHistogram(){
  
  // add the two cuts to the cut flow table
  ostringstream titleStr;
  titleStr <<"Jet : Prefire Veto";
  TString tTitle(titleStr.str().c_str());
  GetCutFlowTable()->AddCutToFlow("Jet.PrefireVeto",tTitle);

  
}//BookHistograms()

/******************************************************************************
 * Bool_t VetoPrefireJets::Apply()                                                   *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t VetoPrefireJets::Apply()
{
  //cout<<"Begin of VetoPrefireJets::Apply()"<<endl;

  // Get Event Tree
  EventContainer *evObj = GetEventContainer();

  Bool_t passesPrefireCut = kTRUE;
  
  for (auto jet: evObj->jets){
    if (jet.IsPrefireVeto()) {
      //      std::cout << jet.Pt() << " " << jet.Eta() << " " << jet.IsPrefireVeto() << std::endl;
      passesPrefireCut = kFALSE;
      break;
    }
  }

  if (passesPrefireCut) GetCutFlowTable()->PassCut("Jet.PrefireVeto");
  else GetCutFlowTable()->FailCut("Jet.PrefireVeto");

  return passesPrefireCut;

} //Apply












