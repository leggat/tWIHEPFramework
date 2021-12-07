 /******************************************************************************
 * CutDiElectron.hpp                                                               *
 *                                                                            *
 * Cuts on dielectron pair properties                                             *
 * Initially this will be on dR and combined mass window, others may follow   *
 * Desired electron collection (tight, veto etc) is passed in the constructor     *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutDiElectron class                                 *
 *    CutDiElectron()                     -- Parameterized Constructor            *
 *    ~CutDiElectron()                    -- Destructor                           *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutDiElectron"                   *
 *                                                                            *
 * Private Data Members of CutDiElectron class                                    *
 *    myTH1F* _hDiElectronDRBefore;    -- Hist mu pair delta r before cut         *
 *    myTH1F* _hDiElectronDRAfter;     -- Hist mu pair delta r after cut          *
 *    myTH1F* _hDiElectronMassBefore;    -- Hist mu pair mass before cut          *
 *    myTH1F* _hDiElectronMassAfter;     -- Hist mu pair mass after cut           *
 *                                                                            *
 *    Int_t _DiElectronDRMin;            -- Minimum delta r between mu pair       *
 *    Int_t _DiElectronDRMax;            -- Maximum delta r between mu pair       *
 *    Int_t _DiElectronMassMin;          -- Minimum mu pair mass                  *
 *    Int_t _DiElectronMassMax;          -- Maximum mu pair mass                  *
 *                                                                            *
 * History                                                                    *
 *      5 Nov 2021 - Created by D. Leggat                                     *
 *****************************************************************************/

#ifndef CutDiElectron_h
#define CutDiElectron_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class CutDiElectron : public HistoCut 
{

public:

  // Parameterized Constructor
  CutDiElectron(EventContainer *EventContainerObj, TString electronType);
  
  // Destructor
  ~CutDiElectron();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "CutDiElectron"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  myTH1F* _hDiElectronDRBefore;    // Hist mu pair delta r before cut
  myTH1F* _hDiElectronDRAfter;     // Hist mu pair delta r after cut 
  myTH1F* _hDiElectronMassBefore;    // Hist mu pair mass before cut 
  myTH1F* _hDiElectronMassAfter;     // Hist mu pair mass after cut  
  myTH1F* _hDiElectronPtBefore;    // Hist mu pair pt before cut 
  myTH1F* _hDiElectronPtAfter;     // Hist mu pair pt after cut  

  // Cut parameters
  Float_t _DiElectronDRMin;            // Minimum delta r between mu pair
  Float_t _DiElectronDRMax;            // Maximum delta r between mu pair
  Float_t _DiElectronMassMin;          // Minimum mu pair mass           
  Float_t _DiElectronMassMax;          // Maximum mu pair mass           

  // Electron Type (veto, tight, unIsolated, etc)
  TString electronType;

};


#endif







