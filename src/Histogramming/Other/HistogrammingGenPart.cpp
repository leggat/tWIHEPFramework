/******************************************************************************
 * HistogrammingGenPart.hpp                                                  *
 *                                                                            *
 * Books and fills histograms                                                 *
 * Must pass a paramter describing the type of electron you want to plot      *
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
 *      06 Feb 2008 - P. Ryan - add 2d histograms                             *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Histogramming/Other/HistogrammingGenPart.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * HistogrammingGenPart::HistogrammingGenPart(EventContainer *obj)          *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event container                                                    *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingGenPart::HistogrammingGenPart(EventContainer *obj)
{
  SetEventContainer(obj);
} //HistogrammingGenPart()

/******************************************************************************
 * HistogrammingGenPart::~HistogrammingGenPart()                                  *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingGenPart::~HistogrammingGenPart()
{
  
} //HistogrammingGenPart

/******************************************************************************
 * void HistogrammingGenPart::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void HistogrammingGenPart::BookHistogram(){
  //cout<<"Begin of HistogrammingGenPart::Book()"<<endl;
  Int_t ptBin    = 25;
  Int_t ptBin2D  = 10;
  Double_t ptMin = 0.0;
  Double_t ptMax = 100.0;

  Int_t etaBin    = 24;
  Int_t etaBin2D  = 10;
  Double_t etaMin = -3.0;
  Double_t etaMax = 3.0;

  Int_t phiBin    = 16;
  Int_t phiBin2D  = 10;
  Double_t phiMin = -1.0 * TMath::Pi();
  Double_t phiMax = TMath::Pi();

  Int_t isoBin    = 50;
  Double_t isoMin = 0.;
  Double_t isoMax = 0.06;
  Double_t isoHiMin = 0.06;
  Double_t isoHiMax = 1.;

  ////////////////////////////////////////////////////////////////
  // Number of Objects
  ////////////////////////////////////////////////////////////////
  _hNObj = DeclareTH1F("GenN","Number of Gen Particles",100,0.,100.);
  _hNObj->SetXAxisTitle("N^{gen}");
  _hNObj->SetYAxisTitle("Events");

  ////////////////////////////////////////////////////////////////
  // Kinematic Quantities - Individual
  ////////////////////////////////////////////////////////////////
  // 1 Pt
  _hPtHiggs = DeclareTH1F("GenHiggsPt", "Gen Higgs P_{T}", ptBin, ptMin, ptMax);
  _hPtHiggs->SetXAxisTitle("P_{T} (Higgs) [GeV]");
  _hPtHiggs->SetYAxisTitle("Events");
  // 1 Eta
  _hEtaHiggs = DeclareTH1F("GenHiggsEta", "Gen Higgs Pseudorapidity", etaBin, etaMin, etaMax);
  _hEtaHiggs->SetXAxisTitle("#eta (Higgs) ");
  _hEtaHiggs->SetYAxisTitle("Events");
  // 1 Phi
  _hPhiHiggs = DeclareTH1F("GenHiggsPhi", "Gen Higgs phi", phiBin, phiMin, phiMax);
  _hPhiHiggs->SetXAxisTitle("#phi (Higgs)");
  _hPhiHiggs->SetYAxisTitle("Events");

  // any Gen Particle Charge
  _hChargeHiggs = DeclareTH1F("GenHiggsCharge","Gen Higgs charge",3,-1.5,1.5);
  _hChargeHiggs->SetXAxisTitle("Charge^{Higgs}");
  _hChargeHiggs->SetYAxisTitle("Events");

  //cout<<"Begin of HistogrammingGenPart::Book()"<<endl;
} //BookHistogram

/******************************************************************************
 * Bool_t HistogrammingGenPart::Apply()                                         *
 *                                                                            *
 * Fill histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t HistogrammingGenPart::Apply()
{
  //cout<<"Begin of HistogrammingGenPart::Apply()"<<endl;
  
  // Get Event Tree
  EventContainer *evc = GetEventContainer();
  
  // Gen Particles class depends on 
  std::vector<GenPart> genpartVector;
  genpartVector.assign(evc -> GenParts.begin(),           evc -> GenParts.end());
  
  _hNObj -> Fill(genpartVector.size());

  if(genpartVector.size()>0) {
    for(int i=0;i<genpartVector.size();i++){
      if(genpartVector[i].GetpdgId()==25){
	_hPtHiggs  -> Fill(genpartVector[i].Pt());
	_hEtaHiggs -> Fill(genpartVector[i].Eta());
	_hPhiHiggs -> Fill(genpartVector[i].Phi());
	_hChargeHiggs->Fill(genpartVector[i].GetCharge());
      }
    }
  }    

  _nTimesRun ++;
  _integral+=evc->GetEventWeight();

  //cout<<"End of HistogrammingGenPart::Apply()"<<endl;
  //  cout << "Integral electronPt: " << _hNObj->Integral() << " n times run: " << _nTimesRun << " integral: " << _integral << std::endl;

  return kTRUE;  
  
} //Apply












