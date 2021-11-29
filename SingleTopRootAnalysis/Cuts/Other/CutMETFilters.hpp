/******************************************************************************
 * CutMETFilters.hpp                                                           *
 *                                                                            *
 * Cuts on Missing Et                                                         *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of CutMETFilters class                              *
 *    CutMETFilters()                    -- Parameterized Constructor          *
 *    ~CutMETFilters()                   -- Destructor                         *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Apply cuts and fill histograms     *
 *    GetCutName()                      -- Returns "CutMETFilters"             *
 *                                                                            *
 * Private Data Members of CutMETFilters                                       *
 *    myTH1F* _hMissingEtBefore         -- Hist of MissingEt before cuts      *
 *    myTH1F* _hMissingEtAfter          -- Hist of MissingEt after cuts       *
 *    Int_t _missingEtMin;              -- Minimum Missing Et                 *
 *    Int_t _missingEtMin;              -- Minimum Missing Et                 *
 *                                                                            *
 * History                                                                    *
 *      15 Dec 2006 - Created by P. Ryan                                      *
 *****************************************************************************/

#ifndef CutMETFilters_h
#define CutMETFilters_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <vector>
#include <sstream>

class CutMETFilters : public HistoCut 
{

public:

  // Parameterized Constructor
  CutMETFilters(EventContainer *EventContainerObj);
  
  // Destructor
  ~CutMETFilters();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "CutMETFilters"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  myTH1F* _hMETFilterBefore;    // Histogram Missing Et
  myTH1F* _hMETFilterAfter;     // Histogram Missing Et
  myTH1F* _hMETAllFilterBefore;    // Histogram Missing Et
  myTH1F* _hMETAllFilterAfter;    // Histogram Missing Et

};


#endif







