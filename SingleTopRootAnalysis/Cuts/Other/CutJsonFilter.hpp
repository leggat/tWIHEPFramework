/******************************************************************************
 * CutJsonFilter.hpp                                                           *
 *                                                                            *
 * Cuts on Missing Et                                                         *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of CutJsonFilter class                              *
 *    CutJsonFilter()                    -- Parameterized Constructor          *
 *    ~CutJsonFilter()                   -- Destructor                         *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Apply cuts and fill histograms     *
 *    GetCutName()                      -- Returns "CutJsonFilter"             *
 *                                                                            *
 * Private Data Members of CutJsonFilter                                       *
 *    myTH1F* _hMissingEtBefore         -- Hist of MissingEt before cuts      *
 *    myTH1F* _hMissingEtAfter          -- Hist of MissingEt after cuts       *
 *    Int_t _missingEtMin;              -- Minimum Missing Et                 *
 *    Int_t _missingEtMin;              -- Minimum Missing Et                 *
 *                                                                            *
 * History                                                                    *
 *      15 Dec 2006 - Created by P. Ryan                                      *
 *****************************************************************************/

#ifndef CutJsonFilter_h
#define CutJsonFilter_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <vector>
#include <sstream>

#include <fstream>


class CutJsonFilter : public HistoCut 
{

public:

  // Parameterized Constructor
  CutJsonFilter(EventContainer *EventContainerObj);
  
  // Destructor
  ~CutJsonFilter();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "CutJsonFilter"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

// Histograms
std::map<Int_t,std::vector<Int_t> > _goodLumis;
//  Json::Value _goodLumis;

};


#endif







