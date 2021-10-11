/******************************************************************************
 * HistogrammingGenPart.hpp                                                  *
 *                                                                            *
 * Books and fills histograms                                                 *
 * Must pass a paramter describing the type of muon you want to plot          *
 *   Choices are All, Veto, Tight                                             *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of AnalysisMain class                              *
 *    HistogrammingGenPart()           -- Parameterized Constructor          *
 *    ~HistogrammingGenPart()          -- Destructor                         *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Fill histograms only (No Cuts)     *
 *    GetCutName()                      -- Returns "HistogrammingGenPart"    *
 *                                                                            *
 * Private Data Members of this class                                         *
 *  - lots of histograms of electron properties                               *
 *                                                                            *
 * History                                                                    *
 *      14 Nov 2006 - Created by R. Schwienhorst for ATLAS                    *
 *      18 Jan 2007 - P. Ryan - modified for all/tight/veto/                  *
 *****************************************************************************/

#ifndef HistogrammingGenPart_h
#define HistogrammingGenPart_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"



class HistogrammingGenPart : public HistoCut 
{

public:

  // Parameterized Constructor
  HistogrammingGenPart(EventContainer *obj);
  
  // Destructor
  ~HistogrammingGenPart();

  // Get the name describing the cut
  inline std::string GetCutName() { return "HistogrammingGenPart"; };

  // Book Histograms
  void BookHistogram();

  // Fill Histograms
  Bool_t Apply();
  
private:
  // Histograms declarations 
  myTH1F* _hNObj;            // Histogram of number of electrons
  
  myTH1F* _hPtHiggs;          // Electron 1 PT

  myTH1F* _hEtaHiggs;         // Electron 1 Eta
  myTH1F* _hPhiHiggs;         // Electron 1 Phi

  myTH1F *_hStatusHiggs;          // Status of all electrons 

  Int_t _nTimesRun;
  Float_t _integral;


};


#endif







