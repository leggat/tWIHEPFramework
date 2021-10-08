/******************************************************************************
 * HistogrammingEventList.cpp                                                 *
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
 *      16 Mar 2011 - Created by Huaqiao ZHANG @ CERN                         *
 *****************************************************************************/

#ifndef HistogrammingHTXS_h
#define HistogrammingHTXS_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"
#include <fstream>

class HistogrammingHTXS : public HistoCut 
{

public:

  // Parameterized Constructor
  HistogrammingHTXS(EventContainer *obj);
  
  // Destructor
  ~HistogrammingHTXS();

  // Get the name describing the cut
  inline std::string GetCutName() { return "HistogrammingHTXS"; };

  // Book Histograms
  void BookHistogram();

  ofstream evtListFile;

  // Fill Histograms
  Bool_t Apply();
  
private:
  // Histograms declarations 
  myTH1F* _hNone;     // Histogram of nothing

};


#endif







