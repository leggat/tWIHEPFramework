/******************************************************************************
 * MatchTriggerElectron.hpp                                                               *
 *                                                                            *
 * Cuts on electron Number (can cut on unIsolated, tight, or veto Electrons)          *
 * Must pass unisolated, tight, or veto to constructor                        *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of MatchTriggerElectron class                                  *
 *    MatchTriggerElectron()                     -- Parameterized Constructor             *
 *    ~MatchTriggerElectron()                    -- Destructor                            *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "MatchTriggerElectron"                    *
 *                                                                            *
 * Private Data Members of MatchTriggerElectron class                                     *
 *    myTH1F* _hElectronNumberBefore;    -- Hist mu Number before cut             *
 *    myTH1F* _hElectronNumberAfter;     -- Hist mu Number of jets after cut      *
 *                                                                            *
 *    Int_t _ElectronNumberMin;          -- Minimum Electron Number                   *
 *    Int_t _ElectronNumberMax;          -- Maximum Electron Number                   *
 *                                                                            *
 * History                                                                    *
 *      15 Jan 2007 - Created by P. Ryan                                      *
 *****************************************************************************/

#ifndef MatchTriggerElectron_h
#define MatchTriggerElectron_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class MatchTriggerElectron : public HistoCut 
{

public:

  // Parameterized Constructor
  MatchTriggerElectron(EventContainer *EventContainerObj, TString electronType);
  
  // Destructor
  ~MatchTriggerElectron();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "MatchTriggerElectron"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  myTH1F* _hElectronTrigObjN;   // Number of trigger objects with electron id
  myTH1F* _hElectronTrigDeltaR;    // Closest delta  r between electron trig and electron
  myTH1F* _hElectronTrigDeltaPt;    // delta pt between chosen trigger object and electron

  // Electron Type (veto, tight, unIsolated)
  TString electronType;

};


#endif







