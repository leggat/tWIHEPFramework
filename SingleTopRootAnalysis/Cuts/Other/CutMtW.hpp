/******************************************************************************
 * CutMtW.hpp                                                           *
 *                                                                            *
 * Cuts on Missing Et                                                         *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of CutMtW class                              *
 *    CutMtW()                    -- Parameterized Constructor          *
 *    ~CutMtW()                   -- Destructor                         *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Apply cuts and fill histograms     *
 *    GetCutName()                      -- Returns "CutMtW"             *
 *                                                                            *
 * Private Data Members of CutMtW                                       *
 *    myTH1F* _hMissingEtBefore         -- Hist of MissingEt before cuts      *
 *    myTH1F* _hMissingEtAfter          -- Hist of MissingEt after cuts       *
 *    Int_t _missingEtMin;              -- Minimum Missing Et                 *
 *    Int_t _missingEtMin;              -- Minimum Missing Et                 *
 *                                                                            *
 * History                                                                    *
 *      15 Dec 2006 - Created by P. Ryan                                      *
 *****************************************************************************/

#ifndef CutMtW_h
#define CutMtW_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <vector>
#include <sstream>

class CutMtW : public HistoCut 
{

public:

  // Parameterized Constructor
  CutMtW(EventContainer *EventContainerObj, bool unisolated);
  
  // Destructor
  ~CutMtW();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "CutMtW"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  myTH1F* _hMtWBefore;    // Histogram Missing Et
  myTH1F* _hMtWAfter;     // Histogram Missing Et

  // Cut parameters
  Double_t _mtWMin;           // Minimum Missing Et
  Double_t _mtWMax;           // Maximum Missing Et

  bool _unisolated;

};


#endif







