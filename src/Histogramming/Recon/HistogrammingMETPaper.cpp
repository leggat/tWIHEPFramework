/******************************************************************************
 * HistogrammingMETPaper.cpp                                                       *
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
 *    myTH1F* _hMET                   -- Hist of MET                          *
 *                                                                            *
 * History                                                                    *
 *      14 Nov 2006 - Created by R. Schwienhorst for ATLAS                    *
 *      20 Nov 2006 - Modified by Bernard Pope                                *
 *      21 Mar 2007 - RS: Fill from event container, add sumET, mex, mey      *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingMETPaper.hpp"
#include <iostream>

using namespace std;

/******************************************************************************
 * HistogrammingMETPaper::HistogrammingMETPaper(EventContainer *obj)                     *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Particle class                                                     *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingMETPaper::HistogrammingMETPaper(EventContainer *obj, TString muonType)
{
  SetEventContainer(obj);
  obj->IsValidCollection("Muon",muonType);
  _muonType = muonType;
  _nTimesRun = 0;
  _integral = 0.;
} //HistogrammingMETPaper()

/******************************************************************************
 * HistogrammingMETPaper::~HistogrammingMETPaper()                                  *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingMETPaper::~HistogrammingMETPaper()
{
  
} //HistogrammingMETPaper

/******************************************************************************
 * void HistogrammingMETPaper::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void HistogrammingMETPaper::BookHistogram(){
  
  Int_t ptBins = 200;
  Float_t minPt = 0.;
  Float_t maxPt  = 450.;

  Int_t u_perpBins = 200;
  Float_t min_u_perp = -200.;
  Float_t max_u_perp = 200.;

  Int_t u_parBins = 200;
  Float_t min_u_par = -200.;
  Float_t max_u_par = 200.;

  Int_t metBins = 200;
  Float_t metMin = 0.;
  Float_t metMax = 300.;

  Int_t metXYBins = 200.;
  Float_t metXYMin = -100.;
  Float_t metXYMax = 100.;
  
  Int_t phiBins = 100;
  Float_t phiMin = -TMath::Pi();
  Float_t phiMax = TMath::Pi();

  Int_t sumEtBins = 200;
  Float_t sumEtMin = 0.;
  Float_t sumEtMax = 2000.;

  Int_t significanceBins = 50;
  Float_t sigMin = 0.;
  Float_t sigMax = 50.;

  Int_t vertexBins = 80;
  Float_t vertexMin = 0.;
  Float_t vertexMax = 80.;

  //Histogramming for Z qt
  _hZQt = DeclareTH1F("ZQt","Z qt",ptBins,minPt,maxPt);
  _hZQt->SetXAxisTitle("Z_{q_T}");
  _hZQt->SetYAxisTitle("Events");

  //Histogramming for Z eta
  _hZEta = DeclareTH1F("ZEta","Z eta",200,0,6.);
  _hZEta->SetXAxisTitle("#eta_{q_T}");
  _hZEta->SetYAxisTitle("Events");

  //Histogramming for Z phi
  _hZPhi = DeclareTH1F("ZPhi","Z phi",phiBins,phiMin,phiMax);
  _hZPhi->SetXAxisTitle("#phi_{q_T}");
  _hZPhi->SetYAxisTitle("Events");

  //Histogramming for Z qt versus nVertex
  _hZQt_nVertex = DeclareTH2F("ZQt_nVertex","Z qt vs nVertex",ptBins,minPt,maxPt,vertexBins,vertexMin,vertexMax);
  _hZQt_nVertex->SetXAxisTitle("Z_{q_T}");
  _hZQt_nVertex->SetYAxisTitle("n_{vertex}");
  
  // Histogram of MET
  _hMET = DeclareTH1F("MET","Missing transverse energy",metBins,metMin,metMax);
  _hMET->SetXAxisTitle("MET [GeV]");
  _hMET->SetYAxisTitle("Events");

  // Histogram of MEX 
  _hMEX = DeclareTH1F("MEX","X component of missing transverse energy",metXYBins,metXYMin,metXYMax);
  _hMEX->SetXAxisTitle("MEX [GeV]");
  _hMEX->SetYAxisTitle("Events");

  // Histogram of MEY
  _hMEY = DeclareTH1F("MEY","Y component of missing transverse energy",metXYBins,metXYMin,metXYMax);
  _hMEY->SetXAxisTitle("MEY [GeV]");
  _hMEY->SetYAxisTitle("Events");

  // Histogram of MET phi
  _hMETPhi = DeclareTH1F("METPhi","Phi of missing transverse energy",phiBins,phiMin,phiMin);
  _hMETPhi->SetXAxisTitle("#Phi_{MET} [GeV]");
  _hMETPhi->SetYAxisTitle("Events");

  // Histogram of SUM ET
  _hSumEt = DeclareTH1F("SumEt","Summed transverse energy",sumEtBins,sumEtMin,sumEtMax);
  _hSumEt->SetXAxisTitle("SumEt [GeV]");
  _hSumEt->SetYAxisTitle("Events");

  // u_tot PF MET
  _hMET_u_tot = DeclareTH1F("MET_u_tot","u_tot of PF MET",ptBins,minPt,maxPt);
  _hMET_u_tot->SetXAxisTitle("u_{tot} [PF MET]");
  _hMET_u_tot->SetYAxisTitle("Events");

  // u_perp PF MET
  _hMET_u_perp = DeclareTH1F("MET_u_perp","u_perp of PF MET",u_perpBins,min_u_perp,max_u_perp);
  _hMET_u_perp->SetXAxisTitle("u_{perp} [PF MET]");
  _hMET_u_perp->SetYAxisTitle("Events");
  
    // u_par PF MET
  _hMET_u_par = DeclareTH1F("MET_u_par","u_par of PF MET",u_parBins,min_u_par,max_u_par);
  _hMET_u_par->SetXAxisTitle("u_{par} [PF MET]");
  _hMET_u_par->SetYAxisTitle("Events");

    // u_par PF MET + Zqt
  _hMET_u_parPqt = DeclareTH1F("MET_u_parPqt","u_par of PF MET plus Z qt",u_parBins,min_u_par,max_u_par);
  _hMET_u_parPqt->SetXAxisTitle("u_{par} + q_{T}[PF MET]");
  _hMET_u_parPqt->SetYAxisTitle("Events");

  // Histogram of MET uncorr
  _hMET_uncorr = DeclareTH1F("MET_uncorr","Missing transverse energy no xy corrections",metBins,metMin,metMax);
  _hMET_uncorr->SetXAxisTitle("MET uncorr [GeV]");
  _hMET_uncorr->SetYAxisTitle("Events");

  // Histogram of MEX  uncorr
  _hMEX_uncorr = DeclareTH1F("MEX_uncorr","X component of missing transverse energy no xy corrections",metXYBins,metXYMin,metXYMax);
  _hMEX_uncorr->SetXAxisTitle("MEX uncorr [GeV]");
  _hMEX_uncorr->SetYAxisTitle("Events");

  // Histogram of MEY uncorr
  _hMEY_uncorr = DeclareTH1F("MEY_uncorr","Y component of missing transverse energy no xy corrections",metXYBins,metXYMin,metXYMax);
  _hMEY_uncorr->SetXAxisTitle("MEY uncorr [GeV]");
  _hMEY_uncorr->SetYAxisTitle("Events");

  // Histogram of MET phi uncorr
  _hMETPhi_uncorr = DeclareTH1F("METPhi_uncorr","Phi of missing transverse energy no xy corrections",phiBins,phiMin,phiMin);
  _hMETPhi_uncorr->SetXAxisTitle("#Phi uncorr_{MET} [GeV]");
  _hMETPhi_uncorr->SetYAxisTitle("Events");


  // 2D plot of u_par vs Z qt for PF MET
  _hMET_u_par_qt = DeclareTH2F("MET_u_par_qt","u_par vs Z qt PF MET",u_parBins,min_u_par,max_u_par,ptBins,minPt,maxPt);
  _hMET_u_par_qt->SetXAxisTitle("u_{par} [PF MET][");
  _hMET_u_par_qt->SetYAxisTitle("Z q_T");

  // 2D plot of u_par vs Z qt for PF MET
  _hMET_u_perp_qt = DeclareTH2F("MET_u_perp_qt","u_perp vs Z qt PF MET",u_perpBins,min_u_perp,max_u_perp,ptBins,minPt,maxPt);
  _hMET_u_perp_qt->SetXAxisTitle("u_{perp} [PF MET][");
  _hMET_u_perp_qt->SetYAxisTitle("Z q_T");

  // 2D plot of u_par vs nVtx for PF MET
  _hMET_u_par_nVtx = DeclareTH2F("MET_u_par_nVtx","u_par vs nVtx PF MET",u_parBins,min_u_par,max_u_par,vertexBins,vertexMin,vertexMax);
  _hMET_u_par_nVtx->SetXAxisTitle("u_{par} [PF MET][");
  _hMET_u_par_nVtx->SetYAxisTitle("N_{vtx}");

  // 2D plot of u_par vs Z nVtx for PF MET
  _hMET_u_perp_nVtx = DeclareTH2F("MET_u_perp_nVtx","u_perp vs nVtx PF MET",u_perpBins,min_u_perp,max_u_perp,vertexBins,vertexMin,vertexMax);
  _hMET_u_perp_nVtx->SetXAxisTitle("u_{perp} [PF MET][");
  _hMET_u_perp_nVtx->SetYAxisTitle("N_{vtx}");

  // 2D plot of u_par + Z qt vs nVtx for PF MET
  _hMET_u_parPqt_nVtx = DeclareTH2F("MET_u_parPqt_nVtx","u_par + Z qt vs nVtx PF MET",u_parBins,min_u_par,max_u_par,vertexBins,vertexMin,vertexMax);
  _hMET_u_parPqt_nVtx->SetXAxisTitle("u_{par} + Z q_T [PF MET][");
  _hMET_u_parPqt_nVtx->SetYAxisTitle("N_{vtx}");

  //Met significance
  _hMET_significance = DeclareTH1F("MET_significance","MET significance",significanceBins,sigMin,sigMax);
  _hMET_significance->SetXAxisTitle("Significance");
  _hMET_significance-> SetYAxisTitle("Events");

  //Met significance_blowup
  _hMET_significance_blowup = DeclareTH1F("MET_significance_blowup","MET significance englarged",100,0.,10.);
  _hMET_significance_blowup->SetXAxisTitle("Significance_blowup");
  _hMET_significance_blowup-> SetYAxisTitle("Events");

  //Calo MET
  
  // Histogram of CaloMET
  _hCaloMET = DeclareTH1F("CaloMET","Missing transverse [Calo]",metBins,metMin,metMax);
  _hCaloMET->SetXAxisTitle("CaloMET [GeV]");
  _hCaloMET->SetYAxisTitle("Events");

  // Histogram of CaloMET
  _hCaloMET_et = DeclareTH1F("CaloMET_et","Missing transverse [Calo]",metBins,metMin,metMax);
  _hCaloMET_et->SetXAxisTitle("CaloMET_et [GeV]");
  _hCaloMET_et->SetYAxisTitle("Events");

  // Histogram of CaloMEX 
  _hCaloMEX = DeclareTH1F("CaloMEX","X component of missing transverse energy [Calo]",metXYBins,metXYMin,metXYMax);
  _hCaloMEX->SetXAxisTitle("CaloMEX [GeV]");
  _hCaloMEX->SetYAxisTitle("Events");

  // Histogram of CaloMEY
  _hCaloMEY = DeclareTH1F("CaloMEY","Y component of missing transverse energy [Calo]",metXYBins,metXYMin,metXYMax);
  _hCaloMEY->SetXAxisTitle("CaloMEY [GeV]");
  _hCaloMEY->SetYAxisTitle("Events");

  // Histogram of CaloMET phi
  _hCaloMETPhi = DeclareTH1F("CaloMETPhi","Phi of missing transverse energy [Calo]",phiBins,phiMin,phiMin);
  _hCaloMETPhi->SetXAxisTitle("#Phi_{CaloMET} [GeV]");
  _hCaloMETPhi->SetYAxisTitle("Events");

  // Histogram of SUM ET
  _hCaloSumEt = DeclareTH1F("CaloSumEt","Summed transverse energy [Calo]",sumEtBins,sumEtMin,sumEtMax);
  _hCaloSumEt->SetXAxisTitle("CaloSumEt [GeV]");
  _hCaloSumEt->SetYAxisTitle("Events");

  //Chs MET
  
  // Histogram of ChsMET
  _hChsMET = DeclareTH1F("ChsMET","Missing transverse [Chs]",metBins,metMin,metMax);
  _hChsMET->SetXAxisTitle("ChsMET [GeV]");
  _hChsMET->SetYAxisTitle("Events");

  // Histogram of ChsMEX 
  _hChsMEX = DeclareTH1F("ChsMEX","X component of missing transverse energy [Chs]",metXYBins,metXYMin,metXYMax);
  _hChsMEX->SetXAxisTitle("ChsMEX [GeV]");
  _hChsMEX->SetYAxisTitle("Events");

  // Histogram of ChsMEY
  _hChsMEY = DeclareTH1F("ChsMEY","Y component of missing transverse energy [Chs]",metXYBins,metXYMin,metXYMax);
  _hChsMEY->SetXAxisTitle("ChsMEY [GeV]");
  _hChsMEY->SetYAxisTitle("Events");

  // Histogram of ChsMET phi
  _hChsMETPhi = DeclareTH1F("ChsMETPhi","Phi of missing transverse energy [Chs]",phiBins,phiMin,phiMin);
  _hChsMETPhi->SetXAxisTitle("#Phi_{ChsMET} [GeV]");
  _hChsMETPhi->SetYAxisTitle("Events");

  // Histogram of SUM ET
  _hChsSumEt = DeclareTH1F("ChsSumEt","Summed transverse energy [Chs]",sumEtBins,sumEtMin,sumEtMax);
  _hChsSumEt->SetXAxisTitle("ChsSumEt [GeV]");
  _hChsSumEt->SetYAxisTitle("Events");

  //Puppi MET
  
  // Histogram of PuppiMET
  _hPuppiMET = DeclareTH1F("PuppiMET","Missing transverse [Puppi]",metBins,metMin,metMax);
  _hPuppiMET->SetXAxisTitle("PuppiMET [GeV]");
  _hPuppiMET->SetYAxisTitle("Events");

  // Histogram of PuppiMEX 
  _hPuppiMEX = DeclareTH1F("PuppiMEX","X component of missing transverse energy [Puppi]",metXYBins,metXYMin,metXYMax);
  _hPuppiMEX->SetXAxisTitle("PuppiMEX [GeV]");
  _hPuppiMEX->SetYAxisTitle("Events");

  // Histogram of PuppiMEY
  _hPuppiMEY = DeclareTH1F("PuppiMEY","Y component of missing transverse energy [Puppi]",metXYBins,metXYMin,metXYMax);
  _hPuppiMEY->SetXAxisTitle("PuppiMEY [GeV]");
  _hPuppiMEY->SetYAxisTitle("Events");

  // Histogram of PuppiMET phi
  _hPuppiMETPhi = DeclareTH1F("PuppiMETPhi","Phi of missing transverse energy [Puppi]",phiBins,phiMin,phiMin);
  _hPuppiMETPhi->SetXAxisTitle("#Phi_{PuppiMET} [GeV]");
  _hPuppiMETPhi->SetYAxisTitle("Events");

  // Histogram of SUM ET
  _hPuppiSumEt = DeclareTH1F("PuppiSumEt","Summed transverse energy [Puppi]",sumEtBins,sumEtMin,sumEtMax);
  _hPuppiSumEt->SetXAxisTitle("PuppiSumEt [GeV]");
  _hPuppiSumEt->SetYAxisTitle("Events");

  // u_tot PF PuppiMET
  _hPuppiMET_u_tot = DeclareTH1F("PuppiMET_u_tot","u_tot of PF PuppiMET",ptBins,minPt,maxPt);
  _hPuppiMET_u_tot->SetXAxisTitle("u_{tot} [PF PuppiMET]");
  _hPuppiMET_u_tot->SetYAxisTitle("Events");

  // u_perp PF PuppiMET
  _hPuppiMET_u_perp = DeclareTH1F("PuppiMET_u_perp","u_perp of PF PuppiMET",u_perpBins,min_u_perp,max_u_perp);
  _hPuppiMET_u_perp->SetXAxisTitle("u_{perp} [PF PuppiMET]");
  _hPuppiMET_u_perp->SetYAxisTitle("Events");
  
    // u_par PF PuppiMET
  _hPuppiMET_u_par = DeclareTH1F("PuppiMET_u_par","u_par of PF PuppiMET",u_parBins,min_u_par,max_u_par);
  _hPuppiMET_u_par->SetXAxisTitle("u_{par} [PF PuppiMET]");
  _hPuppiMET_u_par->SetYAxisTitle("Events");

    // u_par PF PuppiMET + Zqt
  _hPuppiMET_u_parPqt = DeclareTH1F("PuppiMET_u_parPqt","u_par of PF PuppiMET plus Z qt",u_parBins,min_u_par,max_u_par);
  _hPuppiMET_u_parPqt->SetXAxisTitle("u_{par} + q_{T}[PF PuppiMET]");
  _hPuppiMET_u_parPqt->SetYAxisTitle("Events");

  // Histogram of PuppiMET uncorr
  _hPuppiMET_uncorr = DeclareTH1F("PuppiMET_uncorr","Missing transverse energy no xy corrections",metBins,metMin,metMax);
  _hPuppiMET_uncorr->SetXAxisTitle("PuppiMET uncorr [GeV]");
  _hPuppiMET_uncorr->SetYAxisTitle("Events");

  // Histogram of PuppiMEX  uncorr
  _hPuppiMEX_uncorr = DeclareTH1F("PuppiMEX_uncorr","X component of missing transverse energy no xy corrections",metXYBins,metXYMin,metXYMax);
  _hPuppiMEX_uncorr->SetXAxisTitle("PuppiMEX uncorr [GeV]");
  _hPuppiMEX_uncorr->SetYAxisTitle("Events");

  // Histogram of PuppiMEY uncorr
  _hPuppiMEY_uncorr = DeclareTH1F("PuppiMEY_uncorr","Y component of missing transverse energy no xy corrections",metXYBins,metXYMin,metXYMax);
  _hPuppiMEY_uncorr->SetXAxisTitle("PuppiMEY uncorr [GeV]");
  _hPuppiMEY_uncorr->SetYAxisTitle("Events");

  // Histogram of PuppiMET phi uncorr
  _hPuppiMETPhi_uncorr = DeclareTH1F("PuppiMETPhi_uncorr","Phi of missing transverse energy no xy corrections",phiBins,phiMin,phiMin);
  _hPuppiMETPhi_uncorr->SetXAxisTitle("#Phi uncorr_{PuppiMET} [GeV]");
  _hPuppiMETPhi_uncorr->SetYAxisTitle("Events");

  // 2D plot of u_par vs Z qt for  PuppiMET
  _hPuppiMET_u_par_qt = DeclareTH2F("PuppiMET_u_par_qt","u_par vs Z qt  PuppiMET",u_parBins,min_u_par,max_u_par,ptBins,minPt,maxPt);
  _hPuppiMET_u_par_qt->SetXAxisTitle("u_{par} [PuppiMET][");
  _hPuppiMET_u_parPqt->SetYAxisTitle("Z q_T");

  // 2D plot of u_par vs Z qtMET for PuppiMET
  _hPuppiMET_u_perp_qt = DeclareTH2F("PuppiMET_u_perp_qt","u_perp vs Z qt PuppiMET",u_perpBins,min_u_perp,max_u_perp,ptBins,minPt,maxPt);
  _hPuppiMET_u_perp_qt->SetXAxisTitle("u_{perp} [PuppiMET][");
  _hPuppiMET_u_perp_qt->SetYAxisTitle("Z q_T");

  // 2D plot of u_par vs nVtx for PuppiMET
  _hPuppiMET_u_par_nVtx = DeclareTH2F("PuppiMET_u_par_nVtx","u_par vs nVtx PuppiMET",u_parBins,min_u_par,max_u_par,vertexBins,vertexMin,vertexMax);
  _hPuppiMET_u_par_nVtx->SetXAxisTitle("u_{par} [PuppiMET][");
  _hPuppiMET_u_par_nVtx->SetYAxisTitle("N_{vtx}");

  // 2D plot of u_par vs Z nVtx for PuppiMET
  _hPuppiMET_u_perp_nVtx = DeclareTH2F("PuppiMET_u_perp_nVtx","u_perp vs nVtx PuppiMET",u_perpBins,min_u_perp,max_u_perp,vertexBins,vertexMin,vertexMax);
  _hPuppiMET_u_perp_nVtx->SetXAxisTitle("u_{perp} [PuppiMET][");
  _hPuppiMET_u_perp_nVtx->SetYAxisTitle("N_{vtx}");

  // 2D plot of u_par + Z qt vs nVtx for PuppiMET
  _hPuppiMET_u_parPqt_nVtx = DeclareTH2F("PuppiMET_u_parPqt_nVtx","u_par + Z qt vs nVtx PuppiMET",u_parBins,min_u_par,max_u_par,vertexBins,vertexMin,vertexMax);
  _hPuppiMET_u_parPqt_nVtx->SetXAxisTitle("u_{par} + Z q_T [PuppiMET][");
  _hPuppiMET_u_parPqt_nVtx->SetYAxisTitle("N_{vtx}");

  //Raw MET
  
  // Histogram of RawMET
  _hRawMET = DeclareTH1F("RawMET","Missing transverse [Raw]",metBins,metMin,metMax);
  _hRawMET->SetXAxisTitle("RawMET [GeV]");
  _hRawMET->SetYAxisTitle("Events");

  // Histogram of RawMEX 
  _hRawMEX = DeclareTH1F("RawMEX","X component of missing transverse energy [Raw]",metXYBins,metXYMin,metXYMax);
  _hRawMEX->SetXAxisTitle("RawMEX [GeV]");
  _hRawMEX->SetYAxisTitle("Events");

  // Histogram of RawMEY
  _hRawMEY = DeclareTH1F("RawMEY","Y component of missing transverse energy [Raw]",metXYBins,metXYMin,metXYMax);
  _hRawMEY->SetXAxisTitle("RawMEY [GeV]");
  _hRawMEY->SetYAxisTitle("Events");

  // Histogram of RawMET phi
  _hRawMETPhi = DeclareTH1F("RawMETPhi","Phi of missing transverse energy [Raw]",phiBins,phiMin,phiMin);
  _hRawMETPhi->SetXAxisTitle("#Phi_{RawMET} [GeV]");
  _hRawMETPhi->SetYAxisTitle("Events");

  // Histogram of SUM ET
  _hRawSumEt = DeclareTH1F("RawSumEt","Summed transverse energy [Raw]",sumEtBins,sumEtMin,sumEtMax);
  _hRawSumEt->SetXAxisTitle("RawSumEt [GeV]");
  _hRawSumEt->SetYAxisTitle("Events");

  // u_tot PF RawMET
  _hRawMET_u_tot = DeclareTH1F("RawMET_u_tot","u_tot of PF RawMET",ptBins,minPt,maxPt);
  _hRawMET_u_tot->SetXAxisTitle("u_{tot} [PF RawMET]");
  _hRawMET_u_tot->SetYAxisTitle("Events");

  // u_perp PF RawMET
  _hRawMET_u_perp = DeclareTH1F("RawMET_u_perp","u_perp of PF RawMET",u_perpBins,min_u_perp,max_u_perp);
  _hRawMET_u_perp->SetXAxisTitle("u_{perp} [PF RawMET]");
  _hRawMET_u_perp->SetYAxisTitle("Events");
  
    // u_par PF RawMET
  _hRawMET_u_par = DeclareTH1F("RawMET_u_par","u_par of PF RawMET",u_parBins,min_u_par,max_u_par);
  _hRawMET_u_par->SetXAxisTitle("u_{par} [PF RawMET]");
  _hRawMET_u_par->SetYAxisTitle("Events");

    // u_par PF RawMET + Zqt
  _hRawMET_u_parPqt = DeclareTH1F("RawMET_u_parPqt","u_par of PF RawMET plus Z qt",u_parBins,min_u_par,max_u_par);
  _hRawMET_u_parPqt->SetXAxisTitle("u_{par} + q_{T}[PF RawMET]");
  _hRawMET_u_parPqt->SetYAxisTitle("Events");

  // 2D plot of u_par vs Z qt for  RawMET
  _hRawMET_u_par_qt = DeclareTH2F("RawMET_u_par_qt","u_par vs Z qt  RawMET",u_parBins,min_u_par,max_u_par,ptBins,minPt,maxPt);
  _hRawMET_u_par_qt->SetXAxisTitle("u_{par} [RawMET][");
  _hRawMET_u_parPqt->SetYAxisTitle("Z q_T");

  // 2D plot of u_par vs Z qtMET for RawMET
  _hRawMET_u_perp_qt = DeclareTH2F("RawMET_u_perp_qt","u_perp vs Z qt RawMET",u_perpBins,min_u_perp,max_u_perp,ptBins,minPt,maxPt);
  _hRawMET_u_perp_qt->SetXAxisTitle("u_{perp} [RawMET][");
  _hRawMET_u_perp_qt->SetYAxisTitle("Z q_T");

  // 2D plot of u_par vs nVtx for RawMET
  _hRawMET_u_par_nVtx = DeclareTH2F("RawMET_u_par_nVtx","u_par vs nVtx RawMET",u_parBins,min_u_par,max_u_par,vertexBins,vertexMin,vertexMax);
  _hRawMET_u_par_nVtx->SetXAxisTitle("u_{par} [RawMET][");
  _hRawMET_u_par_nVtx->SetYAxisTitle("N_{vtx}");

  // 2D plot of u_par vs Z nVtx for RawMET
  _hRawMET_u_perp_nVtx = DeclareTH2F("RawMET_u_perp_nVtx","u_perp vs nVtx RawMET",u_perpBins,min_u_perp,max_u_perp,vertexBins,vertexMin,vertexMax);
  _hRawMET_u_perp_nVtx->SetXAxisTitle("u_{perp} [RawMET][");
  _hRawMET_u_perp_nVtx->SetYAxisTitle("N_{vtx}");

  // 2D plot of u_par + Z qt vs nVtx for RawMET
  _hRawMET_u_parPqt_nVtx = DeclareTH2F("RawMET_u_parPqt_nVtx","u_par + Z qt vs nVtx RawMET",u_parBins,min_u_par,max_u_par,vertexBins,vertexMin,vertexMax);
  _hRawMET_u_parPqt_nVtx->SetXAxisTitle("u_{par} + Z q_T [RawMET][");
  _hRawMET_u_parPqt_nVtx->SetYAxisTitle("N_{vtx}");

  //RawPuppi MET
  
  // Histogram of RawPuppiMET
  _hRawPuppiMET = DeclareTH1F("RawPuppiMET","Missing transverse [RawPuppi]",metBins,metMin,metMax);
  _hRawPuppiMET->SetXAxisTitle("RawPuppiMET [GeV]");
  _hRawPuppiMET->SetYAxisTitle("Events");

  // Histogram of RawPuppiMEX 
  _hRawPuppiMEX = DeclareTH1F("RawPuppiMEX","X component of missing transverse energy [RawPuppi]",metXYBins,metXYMin,metXYMax);
  _hRawPuppiMEX->SetXAxisTitle("RawPuppiMEX [GeV]");
  _hRawPuppiMEX->SetYAxisTitle("Events");

  // Histogram of RawPuppiMEY
  _hRawPuppiMEY = DeclareTH1F("RawPuppiMEY","Y component of missing transverse energy [RawPuppi]",metXYBins,metXYMin,metXYMax);
  _hRawPuppiMEY->SetXAxisTitle("RawPuppiMEY [GeV]");
  _hRawPuppiMEY->SetYAxisTitle("Events");

  // Histogram of RawPuppiMET phi
  _hRawPuppiMETPhi = DeclareTH1F("RawPuppiMETPhi","Phi of missing transverse energy [RawPuppi]",phiBins,phiMin,phiMin);
  _hRawPuppiMETPhi->SetXAxisTitle("#Phi_{RawPuppiMET} [GeV]");
  _hRawPuppiMETPhi->SetYAxisTitle("Events");

  // Histogram of SUM ET
  _hRawPuppiSumEt = DeclareTH1F("RawPuppiSumEt","Summed transverse energy [RawPuppi]",sumEtBins,sumEtMin,sumEtMax);
  _hRawPuppiSumEt->SetXAxisTitle("RawPuppiSumEt [GeV]");
  _hRawPuppiSumEt->SetYAxisTitle("Events");

  //The following were for 2016 running that required correcting xy coordinates for MET.
  /*  // Histogram of MET corrected for xy -
  _hMET_xy = DeclareTH1F("MET_xy","Missing transverse energy xy corrected",200,0.,200.);
  _hMET_xy->SetXAxisTitle("MET [GeV]");
  _hMET_xy->SetYAxisTitle("Events");

  // Histogram of MEX  corrected for xy
  _hMEX_xy = DeclareTH1F("MEX_xy","X component of missing transverse energy xy corrected",400,-200.,200.);
  _hMEX_xy->SetXAxisTitle("MEX [GeV]");
  _hMEX_xy->SetYAxisTitle("Events");

  // Histogram of MEY corrected for xy
  _hMEY_xy = DeclareTH1F("MEY_xy","Y component of missing transverse energy xy corrected",400,-200.,200.);
  _hMEY_xy->SetXAxisTitle("MEY [GeV]");
  _hMEY_xy->SetYAxisTitle("Events");

  // Histogram of MET phi corrected for xy
  _hMETPhi_xy = DeclareTH1F("METPhi_xy","Phi of missing transverse energy xy corrected",50,-3.3,3.3);
  _hMETPhi_xy->SetXAxisTitle("#Phi_{MET} [GeV]");
  _hMETPhi_xy->SetYAxisTitle("Events");

  // Histogram of MET corrected for xy if the lepton is in the barrel
  _hMET_xy_barrel = DeclareTH1F("MET_xy_barrel","Missing transverse energy xy corrected (barrel lepton)",200,0.,200.);
  _hMET_xy_barrel->SetXAxisTitle("MET [GeV]");
  _hMET_xy_barrel->SetYAxisTitle("Events");

  // Histogram of MET corrected for xy if the lepton is in the endcap
  _hMET_xy_endcap = DeclareTH1F("MET_xy_endcap","Missing transverse energy xy corrected (endcap lepton)",200,0.,200.);
  _hMET_xy_endcap->SetXAxisTitle("MET [GeV]");
  _hMET_xy_endcap->SetYAxisTitle("Events");*/
 

  //cout<<"end of HistogrammingMETPaper::BookHistogram"<<endl;

} //BookHistogram

/******************************************************************************
 * Bool_t HistogrammingMETPaper::Apply()                                         *
 *                                                                            *
 * Fill histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t HistogrammingMETPaper::Apply()
{
  //cout<<"Begin of HistogrammingMETPaper::Apply()"<<endl;

  EventContainer *evc = GetEventContainer();
  
  // Fill the histogram before the cut
  //cout<<"missing et is "<<MissingEt<<endl;

  //Get the muon collection
  std::vector<Muon> muons = evc->GetMuonCollection(_muonType); //should make this metmu

  Int_t nVertex = evc->pv_npvsGood;

    // Fill Histograms
  //  if((evc->tightElectrons.size() >0)||(evc->tightMuons.size() >0)){
  _hMET    -> Fill(evc->missingEt);
  _hMEX    -> Fill(evc->missingEx);
  _hMEY    -> Fill(evc->missingEy);
  _hMETPhi -> Fill(evc->missingPhi);

  _hSumEt  -> Fill(evc->sumETs["met"]);

  _hMET_significance -> Fill(evc->met_significance);
  _hMET_significance_blowup -> Fill(evc->met_significance);

  //If we are doing the larger number of METs, fill the histograms here.
  if (evc->DoManyMETs()){
    TLorentzVector tempMet;

    //Met without xy corrections
    tempMet = evc->missingEtVecs["met_uncorr"];
    _hMET_uncorr->Fill(tempMet.Pt());    
    _hMEX_uncorr->Fill(tempMet.Px());    
    _hMEY_uncorr->Fill(tempMet.Py());    
    _hMETPhi_uncorr->Fill(tempMet.Phi());
    
    //Calo met
    tempMet = evc->missingEtVecs["calo"];
    _hCaloMET->Fill(tempMet.Pt());
    _hCaloMET_et->Fill(tempMet.Et());
    _hCaloMEX->Fill(tempMet.Px());
    _hCaloMEY->Fill(tempMet.Py());
    _hCaloMETPhi->Fill(tempMet.Phi());
    _hCaloSumEt->Fill(evc->sumETs["calo"]);

    //Chs met
    tempMet = evc->missingEtVecs["chs"];
    _hChsMET->Fill(tempMet.Pt());
    _hChsMEX->Fill(tempMet.Px());
    _hChsMEY->Fill(tempMet.Py());
    _hChsMETPhi->Fill(tempMet.Phi());
    _hChsSumEt->Fill(evc->sumETs["chs"]);

    //Puppi met
    tempMet = evc->missingEtVecs["puppi"];
    _hPuppiMET->Fill(tempMet.Pt());
    _hPuppiMEX->Fill(tempMet.Px());
    _hPuppiMEY->Fill(tempMet.Py());
    _hPuppiMETPhi->Fill(tempMet.Phi());
    _hPuppiSumEt->Fill(evc->sumETs["puppi"]);

    //Puppi met no xy corrections
    tempMet = evc->missingEtVecs["puppi_uncorr"];
    _hPuppiMET_uncorr->Fill(tempMet.Pt());
    _hPuppiMEX_uncorr->Fill(tempMet.Px());
    _hPuppiMEY_uncorr->Fill(tempMet.Py());
    _hPuppiMETPhi_uncorr->Fill(tempMet.Phi());

    //Raw met
    tempMet = evc->missingEtVecs["raw"];
    _hRawMET->Fill(tempMet.Pt());
    _hRawMEX->Fill(tempMet.Px());
    _hRawMEY->Fill(tempMet.Py());
    _hRawMETPhi->Fill(tempMet.Phi());
    _hRawSumEt->Fill(evc->sumETs["raw"]);

    //Raw puppi met
    tempMet = evc->missingEtVecs["rawPuppi"];
    _hRawPuppiMET->Fill(tempMet.Pt());
    _hRawPuppiMEX->Fill(tempMet.Px());
    _hRawPuppiMEY->Fill(tempMet.Py());
    _hRawPuppiMETPhi->Fill(tempMet.Phi());
    _hRawPuppiSumEt->Fill(evc->sumETs["rawPuppi"]);

    }

  _nTimesRun++;                    
  _integral+=evc->GetEventWeight();

  //The rest of the plots will rely on a muon collection with at least 2 muons
  if (muons.size() < 2) return kFALSE; //Shouldn't be called if muon is too small
  
  Particle zCand = muons[0];
  zCand += muons[1];

  _hZQt  -> Fill(zCand.Pt());
  _hZEta -> Fill(zCand.Eta());
  _hZPhi -> Fill(zCand.Phi());

  _hZQt_nVertex -> Fill(zCand.Pt(),nVertex);

  //Make the Z candidate in the transverse plane
  TVector3 zCand_t;  
  zCand_t.SetPtEtaPhi(zCand.Pt(),0.,zCand.Phi());

  //3 vector version of met
  TVector3 met;
  met.SetPtEtaPhi(evc->missingEtVecs["met"].Pt(),0.,evc->missingEtVecs["met"].Phi());
  
  //Define the recoil u vector
  TVector3 recoil;

  recoil = -(zCand_t + met);
  recoil.RotateZ(-zCand_t.Phi());

  //Calculate u components
  Float_t u_par = recoil.X();
  Float_t u_perp = recoil.Y();
  
  //Keeping these for later debugging
  /*  Float_t u_par = recoil.Dot(zCand_t)/zCand_t.Pt();
  Float_t u_perp = recoil.Pt() * sin(recoil.Angle(zCand_t));

  TVector3 zCandTest = zCand_t;
  TVector3 recoilTest = recoil;
  recoilTest.RotateZ(-zCandTest.Phi());
  zCandTest.RotateZ(-zCandTest.Phi());*/

  _hMET_u_tot->Fill(recoil.Pt());
  _hMET_u_par->Fill(u_par);
  _hMET_u_perp->Fill(u_perp);
  _hMET_u_parPqt->Fill(u_par+zCand_t.Pt());

  _hMET_u_par_qt->Fill(u_par,zCand_t.Pt());
  _hMET_u_perp_qt->Fill(u_perp,zCand_t.Pt());
  _hMET_u_par_nVtx->Fill(u_par,nVertex);
  _hMET_u_perp_nVtx->Fill(u_perp,nVertex);
  _hMET_u_parPqt_nVtx->Fill(u_par+zCand_t.Pt(),nVertex);

  if (evc->DoManyMETs()){
    met.SetPtEtaPhi(evc->missingEtVecs["puppi"].Pt(),0.,evc->missingEtVecs["puppi"].Phi());
    
    recoil = -(zCand_t + met);
    recoil.RotateZ(-zCand_t.Phi());
    u_par = recoil.X();
    u_perp = recoil.Y();
    
    _hPuppiMET_u_tot->Fill(recoil.Pt());
    _hPuppiMET_u_par->Fill(u_par);
    _hPuppiMET_u_perp->Fill(u_perp);
    _hPuppiMET_u_parPqt->Fill(u_par+zCand_t.Pt());
    
    //2D plots
    _hPuppiMET_u_par_qt->Fill(u_par,zCand_t.Pt());
    _hPuppiMET_u_perp_qt->Fill(u_perp,zCand_t.Pt());
    _hPuppiMET_u_par_nVtx->Fill(u_par,nVertex);
    _hPuppiMET_u_perp_nVtx->Fill(u_perp,nVertex);
    _hPuppiMET_u_parPqt_nVtx->Fill(u_par+zCand_t.Pt(),nVertex);

    met.SetPtEtaPhi(evc->missingEtVecs["raw"].Pt(),0.,evc->missingEtVecs["raw"].Phi());
    
    recoil = -(zCand_t + met);
    recoil.RotateZ(-zCand_t.Phi());
    u_par = recoil.X();
    u_perp = recoil.Y();
    
    _hRawMET_u_tot->Fill(recoil.Pt());
    _hRawMET_u_par->Fill(u_par);
    _hRawMET_u_perp->Fill(u_perp);
    _hRawMET_u_parPqt->Fill(u_par+zCand_t.Pt());

    //2D plots
    _hRawMET_u_par_qt->Fill(u_par,zCand_t.Pt());
    _hRawMET_u_perp_qt->Fill(u_perp,zCand_t.Pt());
    _hRawMET_u_par_nVtx->Fill(u_par,nVertex);
    _hRawMET_u_perp_nVtx->Fill(u_perp,nVertex);
    _hRawMET_u_parPqt_nVtx->Fill(u_par+zCand_t.Pt(),nVertex);
    
  }

  //  std::cout << met.X() << "," << met.Y() << " " << zCand_t.X() << "," << zCand_t.Y() << " " << recoil.X() << "," << recoil.Y() << " " << recoil.Pt() << " " << u_par << " " << u_perp << " mag: " << sqrt(u_par*u_par + u_perp*u_perp) << " zTest: " << zCandTest.X() << "," << zCandTest.Y() << " recoil: " << recoilTest.X() <<"," << recoilTest.Y() <<  std::endl;
  //  std::cout << met.X() << "," << met.Y() << " " << zCand_t.X() << "," << zCand_t.Y() << " " << recoil.X() << "," << recoil.Y() << " " << recoil.Pt() << " " << u_par << " " << u_perp << " mag: " << sqrt(u_par*u_par + u_perp*u_perp) << std::endl;

  //cout<<"End of HistogrammingMETPaper::Apply()"<<endl;
  //  std::cout << "Integral SumEt: " << _hSumEt->Integral() << " n times run: " << _nTimesRun << " integral: " << _integral << std::endl;
  return kTRUE;  
  
} //Apply












