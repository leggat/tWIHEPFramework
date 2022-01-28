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
  HistogrammingMETPaper(EventContainer *obj, TString leptonName, TString muonType);
  
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

  myTH2F* _hZQt_nVertex; //Zqt as a function of nVertex

  // Histograms declarations 
  myTH1F* _hMET;  // Histogram of MET
  myTH1F* _hMEX;  // Histogram of MEX
  myTH1F* _hMEY;  // Histogram of MEY
  myTH1F* _hMETPhi;  // Histogram of MET phi
  myTH1F* _hMET_u_tot; // u_tot for MET
  myTH1F* _hMET_u_perp; // u_perp for MET
  myTH1F* _hMET_u_par; // u_par for MET
  myTH1F* _hMET_u_parPqt; // u_par minus qt for MET

  //If the met hasn't been smeared with jet resolution
  myTH1F* _hMET_unsmeared;
  myTH1F* _hMETPhi_unsmeared;

  //Systematic histograms - can't think of a more clever way to easily do this...
  myTH1F* _hMET_jerDown;  // Histogram of MET
  myTH1F* _hMEX_jerDown;  // Histogram of MEX
  myTH1F* _hMEY_jerDown;  // Histogram of MEY
  myTH1F* _hMETPhi_jerDown;  // Histogram of MET phi
  myTH1F* _hMET_u_tot_jerDown; // u_tot for MET
  myTH1F* _hMET_u_perp_jerDown; // u_perp for MET
  myTH1F* _hMET_u_par_jerDown; // u_par for MET
  myTH1F* _hMET_u_parPqt_jerDown; // u_par minus qt for MET

  myTH1F* _hMET_jerUp;  // Histogram of MET
  myTH1F* _hMEX_jerUp;  // Histogram of MEX
  myTH1F* _hMEY_jerUp;  // Histogram of MEY
  myTH1F* _hMETPhi_jerUp;  // Histogram of MET phi
  myTH1F* _hMET_u_tot_jerUp; // u_tot for MET
  myTH1F* _hMET_u_perp_jerUp; // u_perp for MET
  myTH1F* _hMET_u_par_jerUp; // u_par for MET
  myTH1F* _hMET_u_parPqt_jerUp; // u_par minus qt for MET

  myTH1F* _hMET_jesDown;  // Histogram of MET
  myTH1F* _hMEX_jesDown;  // Histogram of MEX
  myTH1F* _hMEY_jesDown;  // Histogram of MEY
  myTH1F* _hMETPhi_jesDown;  // Histogram of MET phi
  myTH1F* _hMET_u_tot_jesDown; // u_tot for MET
  myTH1F* _hMET_u_perp_jesDown; // u_perp for MET
  myTH1F* _hMET_u_par_jesDown; // u_par for MET
  myTH1F* _hMET_u_parPqt_jesDown; // u_par minus qt for MET

  myTH1F* _hMET_jesUp;  // Histogram of MET
  myTH1F* _hMEX_jesUp;  // Histogram of MEX
  myTH1F* _hMEY_jesUp;  // Histogram of MEY
  myTH1F* _hMETPhi_jesUp;  // Histogram of MET phi
  myTH1F* _hMET_u_tot_jesUp; // u_tot for MET
  myTH1F* _hMET_u_perp_jesUp; // u_perp for MET
  myTH1F* _hMET_u_par_jesUp; // u_par for MET
  myTH1F* _hMET_u_parPqt_jesUp; // u_par minus qt for MET

  myTH1F* _hMET_unclusteredDown;  // Histogram of MET
  myTH1F* _hMEX_unclusteredDown;  // Histogram of MEX
  myTH1F* _hMEY_unclusteredDown;  // Histogram of MEY
  myTH1F* _hMETPhi_unclusteredDown;  // Histogram of MET phi
  myTH1F* _hMET_u_tot_unclusteredDown; // u_tot for MET
  myTH1F* _hMET_u_perp_unclusteredDown; // u_perp for MET
  myTH1F* _hMET_u_par_unclusteredDown; // u_par for MET
  myTH1F* _hMET_u_parPqt_unclusteredDown; // u_par minus qt for MET

  myTH1F* _hMET_unclusteredUp;  // Histogram of MET
  myTH1F* _hMEX_unclusteredUp;  // Histogram of MEX
  myTH1F* _hMEY_unclusteredUp;  // Histogram of MEY
  myTH1F* _hMETPhi_unclusteredUp;  // Histogram of MET phi
  myTH1F* _hMET_u_tot_unclusteredUp; // u_tot for MET
  myTH1F* _hMET_u_perp_unclusteredUp; // u_perp for MET
  myTH1F* _hMET_u_par_unclusteredUp; // u_par for MET
  myTH1F* _hMET_u_parPqt_unclusteredUp; // u_par minus qt for MET

  //no xy corrections applied
  myTH1F* _hMET_uncorr;  // Histogram of MET
  myTH1F* _hMEX_uncorr;  // Histogram of MEX
  myTH1F* _hMEY_uncorr;  // Histogram of MEY
  myTH1F* _hMETPhi_uncorr;  // Histogram of MET phi

  myTH2F* _hMET_u_par_qt; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hMET_u_perp_qt; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hMET_u_parPqt_qt; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hMET_u_par_nVtx; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hMET_u_perp_nVtx; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hMET_u_parPqt_nVtx; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  //2D histograms for systematic estimation
  myTH2F* _hMET_u_par_qt_jerUp; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hMET_u_perp_qt_jerUp; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hMET_u_parPqt_qt_jerUp; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hMET_u_par_nVtx_jerUp; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hMET_u_perp_nVtx_jerUp; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hMET_u_parPqt_nVtx_jerUp; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hMET_u_par_qt_jerDown; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hMET_u_perp_qt_jerDown; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hMET_u_parPqt_qt_jerDown; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hMET_u_par_nVtx_jerDown; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hMET_u_perp_nVtx_jerDown; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hMET_u_parPqt_nVtx_jerDown; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hMET_u_par_qt_jesUp; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hMET_u_perp_qt_jesUp; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hMET_u_parPqt_qt_jesUp; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hMET_u_par_nVtx_jesUp; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hMET_u_perp_nVtx_jesUp; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hMET_u_parPqt_nVtx_jesUp; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hMET_u_par_qt_jesDown; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hMET_u_perp_qt_jesDown; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hMET_u_parPqt_qt_jesDown; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hMET_u_par_nVtx_jesDown; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hMET_u_perp_nVtx_jesDown; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hMET_u_parPqt_nVtx_jesDown; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hMET_u_par_qt_unclusteredUp; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hMET_u_perp_qt_unclusteredUp; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hMET_u_parPqt_qt_unclusteredUp; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hMET_u_par_nVtx_unclusteredUp; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hMET_u_perp_nVtx_unclusteredUp; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hMET_u_parPqt_nVtx_unclusteredUp; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hMET_u_par_qt_unclusteredDown; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hMET_u_perp_qt_unclusteredDown; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hMET_u_parPqt_qt_unclusteredDown; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hMET_u_par_nVtx_unclusteredDown; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hMET_u_perp_nVtx_unclusteredDown; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hMET_u_parPqt_nVtx_unclusteredDown; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH1F* _hMET_significance; //met significance
  myTH1F* _hMET_significance_blowup; //met significance

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

  myTH1F* _hPuppiMET_jerDown;  // Histogram of PuppiMET
  myTH1F* _hPuppiMEX_jerDown;  // Histogram of PuppiMEX
  myTH1F* _hPuppiMEY_jerDown;  // Histogram of PuppiMEY
  myTH1F* _hPuppiMETPhi_jerDown;  // Histogram of PuppiMET phi
  myTH1F* _hPuppiMET_u_tot_jerDown; // u_tot for PuppiMET
  myTH1F* _hPuppiMET_u_perp_jerDown; // u_perp for PuppiMET
  myTH1F* _hPuppiMET_u_par_jerDown; // u_par for PuppiMET
  myTH1F* _hPuppiMET_u_parPqt_jerDown; // u_par minus qt for PuppiMET

  myTH1F* _hPuppiMET_jerUp;  // Histogram of PuppiMET
  myTH1F* _hPuppiMEX_jerUp;  // Histogram of PuppiMEX
  myTH1F* _hPuppiMEY_jerUp;  // Histogram of PuppiMEY
  myTH1F* _hPuppiMETPhi_jerUp;  // Histogram of PuppiMET phi
  myTH1F* _hPuppiMET_u_tot_jerUp; // u_tot for PuppiMET
  myTH1F* _hPuppiMET_u_perp_jerUp; // u_perp for PuppiMET
  myTH1F* _hPuppiMET_u_par_jerUp; // u_par for PuppiMET
  myTH1F* _hPuppiMET_u_parPqt_jerUp; // u_par minus qt for PuppiMET

  myTH1F* _hPuppiMET_jesDown;  // Histogram of PuppiMET
  myTH1F* _hPuppiMEX_jesDown;  // Histogram of PuppiMEX
  myTH1F* _hPuppiMEY_jesDown;  // Histogram of PuppiMEY
  myTH1F* _hPuppiMETPhi_jesDown;  // Histogram of PuppiMET phi
  myTH1F* _hPuppiMET_u_tot_jesDown; // u_tot for PuppiMET
  myTH1F* _hPuppiMET_u_perp_jesDown; // u_perp for PuppiMET
  myTH1F* _hPuppiMET_u_par_jesDown; // u_par for PuppiMET
  myTH1F* _hPuppiMET_u_parPqt_jesDown; // u_par minus qt for PuppiMET

  myTH1F* _hPuppiMET_jesUp;  // Histogram of PuppiMET
  myTH1F* _hPuppiMEX_jesUp;  // Histogram of PuppiMEX
  myTH1F* _hPuppiMEY_jesUp;  // Histogram of PuppiMEY
  myTH1F* _hPuppiMETPhi_jesUp;  // Histogram of PuppiMET phi
  myTH1F* _hPuppiMET_u_tot_jesUp; // u_tot for PuppiMET
  myTH1F* _hPuppiMET_u_perp_jesUp; // u_perp for PuppiMET
  myTH1F* _hPuppiMET_u_par_jesUp; // u_par for PuppiMET
  myTH1F* _hPuppiMET_u_parPqt_jesUp; // u_par minus qt for PuppiMET

  myTH1F* _hPuppiMET_unclusteredDown;  // Histogram of PuppiMET
  myTH1F* _hPuppiMEX_unclusteredDown;  // Histogram of PuppiMEX
  myTH1F* _hPuppiMEY_unclusteredDown;  // Histogram of PuppiMEY
  myTH1F* _hPuppiMETPhi_unclusteredDown;  // Histogram of PuppiMET phi
  myTH1F* _hPuppiMET_u_tot_unclusteredDown; // u_tot for PuppiMET
  myTH1F* _hPuppiMET_u_perp_unclusteredDown; // u_perp for PuppiMET
  myTH1F* _hPuppiMET_u_par_unclusteredDown; // u_par for PuppiMET
  myTH1F* _hPuppiMET_u_parPqt_unclusteredDown; // u_par minus qt for PuppiMET

  myTH1F* _hPuppiMET_unclusteredUp;  // Histogram of PuppiMET
  myTH1F* _hPuppiMEX_unclusteredUp;  // Histogram of PuppiMEX
  myTH1F* _hPuppiMEY_unclusteredUp;  // Histogram of PuppiMEY
  myTH1F* _hPuppiMETPhi_unclusteredUp;  // Histogram of PuppiMET phi
  myTH1F* _hPuppiMET_u_tot_unclusteredUp; // u_tot for PuppiMET
  myTH1F* _hPuppiMET_u_perp_unclusteredUp; // u_perp for PuppiMET
  myTH1F* _hPuppiMET_u_par_unclusteredUp; // u_par for PuppiMET
  myTH1F* _hPuppiMET_u_parPqt_unclusteredUp; // u_par minus qt for PuppiMET

  //no xy corrections applied
  myTH1F* _hPuppiMET_uncorr;  // Histogram of MET
  myTH1F* _hPuppiMEX_uncorr;  // Histogram of MEX
  myTH1F* _hPuppiMEY_uncorr;  // Histogram of MEY
  myTH1F* _hPuppiMETPhi_uncorr;  // Histogram of MET phi

  myTH2F* _hPuppiMET_u_par_qt; // 2D plot of u_par vs Z qt for Puppi MET
  myTH2F* _hPuppiMET_u_perp_qt; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_qt; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hPuppiMET_u_par_nVtx; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_perp_nVtx; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_nVtx; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  //puppi 2d systematics
  myTH2F* _hPuppiMET_u_par_qt_jerUp; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hPuppiMET_u_perp_qt_jerUp; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_qt_jerUp; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hPuppiMET_u_par_nVtx_jerUp; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_perp_nVtx_jerUp; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_nVtx_jerUp; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hPuppiMET_u_par_qt_jerDown; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hPuppiMET_u_perp_qt_jerDown; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_qt_jerDown; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hPuppiMET_u_par_nVtx_jerDown; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_perp_nVtx_jerDown; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_nVtx_jerDown; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hPuppiMET_u_par_qt_jesUp; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hPuppiMET_u_perp_qt_jesUp; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_qt_jesUp; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hPuppiMET_u_par_nVtx_jesUp; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_perp_nVtx_jesUp; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_nVtx_jesUp; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hPuppiMET_u_par_qt_jesDown; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hPuppiMET_u_perp_qt_jesDown; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_qt_jesDown; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hPuppiMET_u_par_nVtx_jesDown; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_perp_nVtx_jesDown; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_nVtx_jesDown; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hPuppiMET_u_par_qt_unclusteredUp; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hPuppiMET_u_perp_qt_unclusteredUp; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_qt_unclusteredUp; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hPuppiMET_u_par_nVtx_unclusteredUp; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_perp_nVtx_unclusteredUp; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_nVtx_unclusteredUp; // 2D plot of u_par + Z qt s nVtx for Puppi MET

  myTH2F* _hPuppiMET_u_par_qt_unclusteredDown; // 2D plot of u_par vs Z qt for PF MET
  myTH2F* _hPuppiMET_u_perp_qt_unclusteredDown; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_qt_unclusteredDown; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hPuppiMET_u_par_nVtx_unclusteredDown; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_perp_nVtx_unclusteredDown; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hPuppiMET_u_parPqt_nVtx_unclusteredDown; // 2D plot of u_par + Z qt s nVtx for Puppi MET

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
  
  myTH2F* _hRawMET_u_par_qt; // 2D plot of u_par vs Z qt for Raw MET
  myTH2F* _hRawMET_u_perp_qt; // 2D plot of u_perp vs Z qt for Puppi MET
  myTH2F* _hRawMET_u_parPqt_qt; // 2D plot of u_par + Z qt s qt for Puppi MET
  myTH2F* _hRawMET_u_par_nVtx; // 2D plot of u_par vs nVtx for Puppi MET
  myTH2F* _hRawMET_u_perp_nVtx; // 2D plot of u_perp vs nVtx for Puppi MET
  myTH2F* _hRawMET_u_parPqt_nVtx; // 2D plot of u_par + Z qt s nVtx for Puppi MET

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
  TString _leptonName;

};


#endif







