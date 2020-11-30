/******************************************************************************
 * LeptonPtReweight.hpp                                                            *
 *                                                                            *
 * Weight events in the histograms                                            *
 *                                                                            *
 * This class is derived from the BaseCut Class and simply                    *
 * cuts on if the event has reconstructed objects                             *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of AnalysisMain class                              *
 *    LeptonPtReweight()                     -- Parameterized Constructor          *
 *    ~LeptonPtReweight()                    -- Destructor                         *
 *    BookHistogram()                   -- Book histograms of weights         *
 *    Apply()                           -- Apply weight and fill histograms   *
 *    GetCutName()                      -- Returns "LeptonPtReweight"              *
 *                                                                            *
 * Private Data Members of AnalysisMain class                                 *
 *    myTH1F* _hTreeWeight              -- Histogram of Input Tree Weights    *
 *    myTH1F* _hEventWeight            -- Histogram of Event Weights        *
 *    myTH1F* _hOutputWeight            -- Histogram of Output Weights        *
 *                                                                            *
 * History                                                                    *
 *     28 Apr 2005 - Created by R. Schwienhorst at D0                         *
 *      7 Nov 2006 - Modified by R. Schwienhorst for ATLAS                    *
 *      9 Nov 2006 - Modified by P. Ryan - cleanup and reorganized            *
 *****************************************************************************/

#ifndef LeptonPtReweight_h
#define LeptonPtReweight_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/BTagEntry.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/BTagCalibration.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/BTagCalibrationReader.hpp"
//#include "externaltools/pileup/PileupReweighting/TPileupReweighting.h"
#include <TGraphAsymmErrors.h>


class LeptonPtReweight : public HistoCut
{

public:

  // Parameterized Constructor
  LeptonPtReweight(EventContainer *obj, TString electronType,int region, int isMuon);
  
  // Default Destructor
  ~LeptonPtReweight();

  // Book Histograms
  void BookHistogram(void );
  
  // Apply weight and fill histograms
  Bool_t Apply();


  // methods for weighting for MC generatd with MCatNLO
  inline std::string GetCutName() { return "LeptonPtReweight"; };

private:
  // Histograms
  myTH1F* _hLeptonReweight;   // Histogram of input tree weights
  myTH1F* _hTotalReweight;  // Histogram of the reweight
  myTH1F* _hEventWeight; // Histogram of global weights
  myTH1F* _hEventPreWeight; // Histogram of global weights before reweighting

  TH1F* _histogramForReweighting; //The histogram to use for the reweighting
  
  TString _electronType;

  int _isElectron; //We can also apply this to muons if we want.

 //Root::TPileupReweighting* PileupReweighting;

};


#endif







