/******************************************************************************
 * CutDiMuon.hpp                                                               *
 *                                                                            *
 * Cuts on dimuon pair properties                                             *
 * Initially this will be on dR and combined mass window, others may follow   *
 * Desired muon collection (tight, veto etc) is passed in the constructor     *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutDiMuon class                                 *
 *    CutDiMuon()                     -- Parameterized Constructor            *
 *    ~CutDiMuon()                    -- Destructor                           *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutDiMuon"                   *
 *                                                                            *
 * Private Data Members of CutDiMuon class                                    *
 *    myTH1F* _hDiMuonDRBefore;    -- Hist mu pair delta r before cut         *
 *    myTH1F* _hDiMuonDRAfter;     -- Hist mu pair delta r after cut          *
 *    myTH1F* _hDiMuonMassBefore;    -- Hist mu pair mass before cut          *
 *    myTH1F* _hDiMuonMassAfter;     -- Hist mu pair mass after cut           *
 *                                                                            *
 *    Int_t _DiMuonDRMin;            -- Minimum delta r between mu pair       *
 *    Int_t _DiMuonDRMax;            -- Maximum delta r between mu pair       *
 *    Int_t _DiMuonMassMin;          -- Minimum mu pair mass                  *
 *    Int_t _DiMuonMassMax;          -- Maximum mu pair mass                  *
 *                                                                            *
 * History                                                                    *
 *      5 Nov 2021 - Created by D. Leggat                                     *
 *****************************************************************************/

#ifndef CutDiMuon_h
#define CutDiMuon_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class CutDiMuon : public HistoCut 
{

public:

  // Parameterized Constructor
  CutDiMuon(EventContainer *EventContainerObj, TString muonType);
  
  // Destructor
  ~CutDiMuon();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "CutDiMuon"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  myTH1F* _hDiMuonDRBefore;    // Hist mu pair delta r before cut
  myTH1F* _hDiMuonDRAfter;     // Hist mu pair delta r after cut 
  myTH1F* _hDiMuonMassBefore;    // Hist mu pair mass before cut 
  myTH1F* _hDiMuonMassAfter;     // Hist mu pair mass after cut  

  // Cut parameters
  Float_t _DiMuonDRMin;            // Minimum delta r between mu pair
  Float_t _DiMuonDRMax;            // Maximum delta r between mu pair
  Float_t _DiMuonMassMin;          // Minimum mu pair mass           
  Float_t _DiMuonMassMax;          // Maximum mu pair mass           

  // Muon Type (veto, tight, unIsolated, etc)
  TString muonType;

};


#endif







