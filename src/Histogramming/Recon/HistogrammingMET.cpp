/******************************************************************************
 * HistogrammingMET.cpp                                                       *
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
 *    myTH1F* _hMET                   -- Hist of MET                          *
 *                                                                            *
 * History                                                                    *
 *      14 Nov 2006 - Created by R. Schwienhorst for ATLAS                    *
 *      20 Nov 2006 - Modified by Bernard Pope                                *
 *      21 Mar 2007 - RS: Fill from event container, add sumET, mex, mey      *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingMET.hpp"
#include <iostream>

using namespace std;

/******************************************************************************
 * HistogrammingMET::HistogrammingMET(EventContainer *obj)                     *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Particle class                                                     *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingMET::HistogrammingMET(EventContainer *obj, bool unisolated)
{
  SetEventContainer(obj);
  _unisolated = unisolated;
  _nTimesRun = 0;
  _integral = 0.;
} //HistogrammingMET()

/******************************************************************************
 * HistogrammingMET::~HistogrammingMET()                                  *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingMET::~HistogrammingMET()
{
  
} //HistogrammingMET

/******************************************************************************
 * void HistogrammingMET::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void HistogrammingMET::BookHistogram(){
  
  Int_t metBins = 200;
  Float_t metMin = 0.;
  Float_t metMax = 150.;

  Int_t metXYBins = 200.;
  Float_t metXYMin = -100.;
  Float_t metXYMax = 100.;
  
  Int_t phiBins = 100;
  Float_t phiMin = -TMath::Pi();
  Float_t phiMax = TMath::Pi();

  Int_t sumEtBins = 200;
  Float_t sumEtMin = 0.;
  Float_t sumEtMax = 2000.;

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
 

  //cout<<"end of HistogrammingMET::BookHistogram"<<endl;

} //BookHistogram

/******************************************************************************
 * Bool_t HistogrammingMET::Apply()                                         *
 *                                                                            *
 * Fill histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t HistogrammingMET::Apply()
{
  //cout<<"Begin of HistogrammingMET::Apply()"<<endl;

  EventContainer *evc = GetEventContainer();
  
  // Fill the histogram before the cut
  //cout<<"missing et is "<<MissingEt<<endl;


  // Fill Histograms
  //  if((evc->tightElectrons.size() >0)||(evc->tightMuons.size() >0)){
  _hMET    -> Fill(evc->missingEt);
  _hMEX    -> Fill(evc->missingEx);
  _hMEY    -> Fill(evc->missingEy);
  _hMETPhi -> Fill(evc->missingPhi);

  _hSumEt  -> Fill(evc->sumETs["met"]);


  //If we are doing the larger number of METs, fill the histograms here.
  if (evc->DoManyMETs()){
    TLorentzVector tempMet;
    
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
  //cout<<"End of HistogrammingMET::Apply()"<<endl;
  //  std::cout << "Integral SumEt: " << _hSumEt->Integral() << " n times run: " << _nTimesRun << " integral: " << _integral << std::endl;
  return kTRUE;  
  
} //Apply












