/******************************************************************************
 * MatchTriggerMuon.hpp                                                               *
 *                                                                            *
 * Cuts on muon Number (can cut on unIsolated, tight, or veto Muons)          *
 * Must pass unisolated, tight, or veto to constructor                        *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of MatchTriggerMuon class                                  *
 *    MatchTriggerMuon()                     -- Parameterized Constructor             *
 *    ~MatchTriggerMuon()                    -- Destructor                            *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "MatchTriggerMuon"                    *
 *                                                                            *
 * Private Data Members of MatchTriggerMuon class                                     *
 *    myTH1F* _hMuonNumberBefore;    -- Hist mu Number before cut             *
 *    myTH1F* _hMuonNumberAfter;     -- Hist mu Number of jets after cut      *
 *                                                                            *
 *    Int_t _MuonNumberMin;          -- Minimum Muon Number                   *
 *    Int_t _MuonNumberMax;          -- Maximum Muon Number                   *
 *                                                                            *
 * History                                                                    *
 *      15 Jan 2007 - Created by P. Ryan                                      *
 *****************************************************************************/

#ifndef MatchTriggerMuon_h
#define MatchTriggerMuon_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class MatchTriggerMuon : public HistoCut 
{

public:

  // Parameterized Constructor
  MatchTriggerMuon(EventContainer *EventContainerObj, TString muonType);
  
  // Destructor
  ~MatchTriggerMuon();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "MatchTriggerMuon"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  myTH1F* _hMuonTrigObjN;   // Number of trigger objects with muon id
  myTH1F* _hMuonTrigDeltaR;    // Closest delta  r between muon trig and muon
  myTH1F* _hMuonTrigDeltaPt;    // delta pt between chosen trigger object and muon

  // Muon Type (veto, tight, unIsolated)
  TString muonType;

};


#endif







