/******************************************************************************
 * CutMuonTrigger.hpp                                                               *
 *                                                                            *
 * Cuts on muon Number (can cut on unIsolated, tight, or veto Muons)          *
 * Must pass unisolated, tight, or veto to constructor                        *
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

#ifndef CutMuonTrigger_h
#define CutMuonTrigger_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class CutMuonTrigger : public HistoCut 
{

public:

  // Parameterized Constructor
  CutMuonTrigger(EventContainer *EventContainerObj, TString muonType);
  
  // Destructor
  ~CutMuonTrigger();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "CutMuonTrigger"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  //  myTH1F* _hMuonNumberBefore;   // Histogram Muon Number before the cut
  // myTH1F* _hMuonNumberAfter;    // Histogram Muon Number of jets after the cut

  // Cut parameters - There aren't really any parameters for this cut.
  //Int_t _MuonNumberMin;         // Minimum Muon Number
  //Int_t _MuonNumberMax;         // Maximum Muon Number

  // Muon Type (veto, tight, unIsolated)
  TString muonType;

};


#endif







