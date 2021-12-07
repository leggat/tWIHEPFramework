/******************************************************************************
 * CutElectronTrigger.hpp                                                               *
 *                                                                            *
 * Cuts on electron Number (can cut on unIsolated, tight, or veto Electrons)          *
 * Must pass unisolated, tight, or veto to constructor                        *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutElectronTrigger class                                  *
 *    CutElectronTrigger()                     -- Parameterized Constructor             *
 *    ~CutElectronTrigger()                    -- Destructor                            *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutElectronTrigger"                    *
 *                                                                            *
 * Private Data Members of CutElectronTrigger class                                     *
 *    myTH1F* _hElectronNumberBefore;    -- Hist mu Number before cut             *
 *    myTH1F* _hElectronNumberAfter;     -- Hist mu Number of jets after cut      *
 *                                                                            *
 *    Int_t _ElectronNumberMin;          -- Minimum Electron Number                   *
 *    Int_t _ElectronNumberMax;          -- Maximum Electron Number                   *
 *                                                                            *
 * History                                                                    *
 *      15 Jan 2007 - Created by P. Ryan                                      *
 *****************************************************************************/

#ifndef CutElectronTrigger_h
#define CutElectronTrigger_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class CutElectronTrigger : public HistoCut 
{

public:

  // Parameterized Constructor
  CutElectronTrigger(EventContainer *EventContainerObj, TString electronType);
  
  // Destructor
  ~CutElectronTrigger();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "CutElectronTrigger"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  //  myTH1F* _hElectronNumberBefore;   // Histogram Electron Number before the cut
  // myTH1F* _hElectronNumberAfter;    // Histogram Electron Number of jets after the cut

  // Cut parameters - There aren't really any parameters for this cut.
  //Int_t _ElectronNumberMin;         // Minimum Electron Number
  //Int_t _ElectronNumberMax;         // Maximum Electron Number

  // Electron Type (veto, tight, unIsolated)
  TString electronType;

};


#endif







