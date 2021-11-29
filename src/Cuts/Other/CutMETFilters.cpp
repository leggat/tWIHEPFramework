/******************************************************************************
 * CutMETFilters.cpp                                                       *
 *                                                                            *
 * Cuts on the requirement of a good primary vertex                           *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutMETFilters class                              *
 *    CutMETFilters()                     -- Parameterized Constructor         *
 *    ~CutMETFilters()                    -- Destructor                        *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutMETFilters"                *
 *                                                                            *
 * Private Data Members of CutMETFilters class                                 *
 *    myTH1F* _hMissingEtBefore;    -- Hist of PV before cut        *
 *    myTH1F* _hMissingEtAfter;     -- Hist of PV after cut         *
 *                                                                            *
 * History                                                                    *
 *      5th July 2016 - Created by Duncan Leggat                              *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Other/CutMETFilters.hpp"
#include <iostream>

using namespace std;

/******************************************************************************
 * CutMETFilters::CutMETFilters(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutMETFilters::CutMETFilters(EventContainer *EventContainerObj)
{
  // Set Event Container
  SetEventContainer(EventContainerObj);
} // CutMETFilters


/******************************************************************************
 * CutMETFilters::~CutMETFilters()                                              *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutMETFilters::~CutMETFilters()
{
  
}//~CutMETFilters

/******************************************************************************
 * void CutMETFilters::BookHistogram()                                         *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutMETFilters::BookHistogram(){
  
  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // histogram before cut
  _hMETFilterBefore =  DeclareTH1F("METFilterBefore","MET Filters Before", 2, 0.0, 2.);
  _hMETFilterBefore -> SetXAxisTitle("MET Filter");
  _hMETFilterBefore -> SetYAxisTitle("Events");

  // Histogram after cut
  _hMETFilterAfter=  DeclareTH1F("METFilterAfter", "MET Filters After", 2, 0.0, 2.);
  _hMETFilterAfter-> SetXAxisTitle("MET Filter");
  _hMETFilterAfter-> SetYAxisTitle("Events");

  // histogram before cut
  _hMETAllFilterBefore =  DeclareTH1F("METAllFilterBefore","MET AllFilters Before", 2, 0.0, 2.);
  _hMETAllFilterBefore -> SetXAxisTitle("MET AllFilter");
  _hMETAllFilterBefore -> SetYAxisTitle("Events");

  // Histogram after cut
  _hMETAllFilterAfter=  DeclareTH1F("METAllFilterAfter", "MET AllFilters After", 2, 0.0, 2.);
  _hMETAllFilterAfter-> SetXAxisTitle("MET AllFilter");
  _hMETAllFilterAfter-> SetYAxisTitle("Events");


  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************

  GetCutFlowTable()->AddCutToFlow("MET.Filters","Pass MET filter");

}//BookHistograms()

/******************************************************************************
 * Bool_t CutMETFilters::Apply()                                                *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the selected trigger                                                *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutMETFilters::Apply()
{

  EventContainer *EventContainerObj = GetEventContainer();

  Bool_t passesMETFilter = EventContainerObj->passesMETFilters;

  _hMETFilterBefore->Fill(passesMETFilter);
  _hMETAllFilterBefore->Fill(EventContainerObj->passesAllMETFilters);

  if (passesMETFilter){
    GetCutFlowTable()->PassCut("MET.Filters");
    _hMETFilterAfter->Fill(passesMETFilter);
    _hMETAllFilterAfter->Fill(EventContainerObj->passesAllMETFilters);
  }
  else {
    GetCutFlowTable()->FailCut("MET.Filters");
  }
  
  return passesMETFilter;

} //Apply












