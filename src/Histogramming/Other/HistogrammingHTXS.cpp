/******************************************************************************
 * HistogrammingHTXS.cpp                                                 *
 *                                                                            *
 * Format write out selected Events: runNumber	EventNumber                   *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of AnalysisMain class                              *
 *    HistogrammingHTXS()              -- Parameterized Constructor      *
 *    ~HistogrammingHTXS()             -- Destructor                     *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Fill histograms only (No Cuts)     *
 *    GetCutName()                      -- Returns "HistogrammingHTXS"   *
 *                                                                            *
 * Private Data Members of this class                                         *
 * - lots of histograms of muon properties                                    *
 *                                                                            *
 * History                                                                    *
 *      16 Mar 2011 - Created by Huaqiao ZHANG                                *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Histogramming/Other/HistogrammingHTXS.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * HistogrammingHTXS::HistogrammingHTXS(EventContainer *obj)        *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event container                                                    *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingHTXS::HistogrammingHTXS(EventContainer *obj)
{
  SetEventContainer(obj);
} //HistogrammingHTXS()

/******************************************************************************
 * HistogrammingHTXS::~HistogrammingHTXS()                                    *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingHTXS::~HistogrammingHTXS()
{
  
} //HistogrammingHTXS

/******************************************************************************
 * void HistogrammingHTXS::BookHistogram()                                    *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void HistogrammingHTXS::BookHistogram(){
  
  ////////////////////////////////////////////////////////////////
  // Histogram
  ////////////////////////////////////////////////////////////////
  _hNone = DeclareTH1F("Nothing","Nothing", 400.0, 0.0, 400.0);
  _hNone->SetXAxisTitle("HTXS_Higgs_pt");
  _hNone->SetYAxisTitle("Events");

  //cout<<"end of HistogrammingHTXS::BookHistogram"<<endl;

} //BookHistogram

/******************************************************************************
 * Bool_t HistogrammingHTXS::Apply()                                     *
 *                                                                            *
 * Fill histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t HistogrammingHTXS::Apply()
{
  //cout<<"Begin of HistogrammingHTXS::Apply()"<<endl;

  // Get Event Tree
  EventContainer *evc = GetEventContainer();
  // Fill Histograms
  //cout<<"End of HistogrammingHTXS::Apply()"<<endl;
  _hNone -> Fill(evc->HTXS_Higgs_pt);
  //evtListFile << (int)(evc->runNumber) << "\t" << (int)(evc->eventNumber) << endl;
  //cout<<"End of HistogrammingHTXS::Apply()"<<endl;
  return kTRUE;  
  
} //Apply












