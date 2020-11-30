/******************************************************************************
 * CutBarrelEndcapLepton.hpp                                                           *
 *                                                                            *
 * Cuts on Missing Et                                                         *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of CutBarrelEndcapLepton class                              *
 *    CutBarrelEndcapLepton()                    -- Parameterized Constructor          *
 *    ~CutBarrelEndcapLepton()                   -- Destructor                         *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Apply cuts and fill histograms     *
 *    GetCutName()                      -- Returns "CutBarrelEndcapLepton"             *
 *                                                                            *
 * Private Data Members of CutBarrelEndcapLepton                                       *
 *    myTH1F* _hMissingEtBefore         -- Hist of MissingEt before cuts      *
 *    myTH1F* _hMissingEtAfter          -- Hist of MissingEt after cuts       *
 *    Int_t _missingEtMin;              -- Minimum Missing Et                 *
 *    Int_t _missingEtMin;              -- Minimum Missing Et                 *
 *                                                                            *
 * History                                                                    *
 *      15 Dec 2006 - Created by P. Ryan                                      *
 *****************************************************************************/

#ifndef CutBarrelEndcapLepton_h
#define CutBarrelEndcapLepton_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <vector>
#include <sstream>

class CutBarrelEndcapLepton : public HistoCut 
{

public:

  // Parameterized Constructor
  CutBarrelEndcapLepton(EventContainer *EventContainerObj, Int_t barrelEndcap, Bool_t unisolated);
  
  // Destructor
  ~CutBarrelEndcapLepton();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "CutBarrelEndcapLepton"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  myTH1F* _hLepEtaBefore;    // Histogram Missing Et
  myTH1F* _hLepEtaAfter;     // Histogram Missing Et

  // Cut parameters
  Int_t _detRegSelect;           // Which region we're selecting: -1 = all, 1 = barrel, 0 = endcap
  Bool_t _unisolated;               // Whether we are using unisolated leptons

};


#endif







