/******************************************************************************
 * HistogrammingMETPaper.hpp                                                       *
 *                                                                            *
 * Books and fills histograms                                                 *
 * Used for events passing cuts applied in other classes                      *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of AnalysisMain class                              *
 *    HistogrammingMETPaper()              -- Parameterized Constructor            *
 *    ~HistogrammingMETPaper()             -- Destructor                           *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Fill histograms only (No Cuts)     *
 *    GetCutName()                      -- Returns "HistogrammingMETPaper"         *
 *                                                                            *
 * Private Data Members of this class                                         *
 *  - histograms of MET                                                       *
 *                                                                            *
 * History                                                                    *
 *      14 Nov 2006 - Created by R. Schwienhorst for ATLAS                    *
 *      20 Nov 2006 - Modified by Bernard Pope                                *
 *      21 Mar 2007 - RS: Fill from event container, add sumET, mex, mey      *
 *****************************************************************************/

#ifndef HistogrammingMETPaper_h
#define HistogrammingMETPaper_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"


class HistogrammingMETPaper : public HistoCut 
{

public:

  // Parameterized Constructor
  HistogrammingMETPaper(EventContainer *obj, TString muonType);
  
  // Destructor
  ~HistogrammingMETPaper();

  // Get the name describing the cut
  inline std::string GetCutName() { return "HistogrammingMETPaper"; };

  // Book Histograms
  void BookHistogram();

  // Fill Histograms
  Bool_t Apply();
  
private:
  
  myTH1F* _hZQt; // qt of the reconstructed z boson
  myTH1F* _hZEta; 
  myTH1F* _hZPhi;

  // Histograms declarations 
  myTH1F* _hMET;  // Histogram of MET
  myTH1F* _hMEX;  // Histogram of MEX
  myTH1F* _hMEY;  // Histogram of MEY
  myTH1F* _hMETPhi;  // Histogram of MET phi
  myTH1F* _hMET_u_tot; // u_tot for MET
  myTH1F* _hMET_u_perp; // u_perp for MET
  myTH1F* _hMET_u_par; // u_par for MET
  myTH1F* _hMET_u_parPqt; // u_par minus qt for MET

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
  myTH1F* _hPuppiMET_u_tot; // u_tot for MET
  myTH1F* _hPuppiMET_u_perp; // u_perp for MET
  myTH1F* _hPuppiMET_u_par; // u_par for MET
  myTH1F* _hPuppiMET_u_parPqt; // u_par minus qt for MET

  // RawMET
  myTH1F* _hRawMET;  // Histogram of MET
  myTH1F* _hRawMEX;  // Histogram of MEX
  myTH1F* _hRawMEY;  // Histogram of MEY
  myTH1F* _hRawMETPhi;  // Histogram of MET phi
  myTH1F* _hRawSumEt;  // Histogram of MET phi
  myTH1F* _hRawMET_u_tot; // u_tot for MET
  myTH1F* _hRawMET_u_perp; // u_perp for MET
  myTH1F* _hRawMET_u_par; // u_par for MET  
  myTH1F* _hRawMET_u_parPqt; // u_par minus qt for MET

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

  TString _muonType;

};


#endif







