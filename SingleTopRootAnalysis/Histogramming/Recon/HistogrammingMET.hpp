/******************************************************************************
 * HistogrammingMET.hpp                                                       *
 *                                                                            *
 * Books and fills histograms                                                 *
 * Used for events passing cuts applied in other classes                      *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of AnalysisMain class                              *
 *    HistogrammingMET()              -- Parameterized Constructor            *
 *    ~HistogrammingMET()             -- Destructor                           *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Fill histograms only (No Cuts)     *
 *    GetCutName()                      -- Returns "HistogrammingMET"         *
 *                                                                            *
 * Private Data Members of this class                                         *
 *  - histograms of MET                                                       *
 *                                                                            *
 * History                                                                    *
 *      14 Nov 2006 - Created by R. Schwienhorst for ATLAS                    *
 *      20 Nov 2006 - Modified by Bernard Pope                                *
 *      21 Mar 2007 - RS: Fill from event container, add sumET, mex, mey      *
 *****************************************************************************/

#ifndef HistogrammingMET_h
#define HistogrammingMET_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"


class HistogrammingMET : public HistoCut 
{

public:

  // Parameterized Constructor
  HistogrammingMET(EventContainer *obj, bool unisolated = false);
  
  // Destructor
  ~HistogrammingMET();

  // Get the name describing the cut
  inline std::string GetCutName() { return "HistogrammingMET"; };

  // Book Histograms
  void BookHistogram();

  // Fill Histograms
  Bool_t Apply();
  
private:
  // Histograms declarations 
  myTH1F* _hMET;  // Histogram of MET
  myTH1F* _hMEX;  // Histogram of MEX
  myTH1F* _hMEY;  // Histogram of MEY
  myTH1F* _hMETPhi;  // Histogram of MET phi

  // CaloMET
  myTH1F* _hCaloMET;  // Histogram of MET
  myTH1F* _hCaloMET_et;  // Histogram of MET
  myTH1F* _hCaloMEX;  // Histogram of MEX
  myTH1F* _hCaloMEY;  // Histogram of MEY
  myTH1F* _hCaloMETPhi;  // Histogram of MET phi
  myTH1F* _hCaloSumEt;  // Histogram of MET phi

  // ChsMET
  myTH1F* _hChsMET;  // Histogram of MET
  myTH1F* _hChsMEX;  // Histogram of MEX
  myTH1F* _hChsMEY;  // Histogram of MEY
  myTH1F* _hChsMETPhi;  // Histogram of MET phi
  myTH1F* _hChsSumEt;  // Histogram of MET phi

  // PuppiMET
  myTH1F* _hPuppiMET;  // Histogram of MET
  myTH1F* _hPuppiMEX;  // Histogram of MEX
  myTH1F* _hPuppiMEY;  // Histogram of MEY
  myTH1F* _hPuppiMETPhi;  // Histogram of MET phi
  myTH1F* _hPuppiSumEt;  // Histogram of MET phi

  // RawMET
  myTH1F* _hRawMET;  // Histogram of MET
  myTH1F* _hRawMEX;  // Histogram of MEX
  myTH1F* _hRawMEY;  // Histogram of MEY
  myTH1F* _hRawMETPhi;  // Histogram of MET phi
  myTH1F* _hRawSumEt;  // Histogram of MET phi

  // RawPuppiMET
  myTH1F* _hRawPuppiMET;  // Histogram of MET
  myTH1F* _hRawPuppiMEX;  // Histogram of MEX
  myTH1F* _hRawPuppiMEY;  // Histogram of MEY
  myTH1F* _hRawPuppiMETPhi;  // Histogram of MET phi
  myTH1F* _hRawPuppiSumEt;  // Histogram of MET phi

  //Corrected met distributions
  myTH1F* _hMET_xy; // Histogram of xy corrected MET
  myTH1F* _hMEX_xy; // Histogram of xy corrected MEX
  myTH1F* _hMEY_xy; // Histogram of xy corrected MEY
  myTH1F* _hMETPhi_xy; // Histogram of xy corrected MET phi

  //barrel/endcap
  myTH1F* _hMET_xy_barrel; // Histogram of xy corrected MET phi  
  myTH1F* _hMET_xy_endcap; // Histogram of xy corrected MET phi

  myTH1F* _hSumEt;  // Histogram of SumEt

  Int_t _nTimesRun;
  Float_t _integral;

  bool _unisolated;

};


#endif







