/******************************************************************************
 * HistogrammingJet.cpp                                                       *
 *                                                                            *
 * Books and fills histograms                                                 *
 * Used for events passing cuts applied in other classes                      *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of AnalysisMain class                              *
 *    HistogrammingJet()              -- Parameterized Constructor            *
 *    ~HistogrammingJet()             -- Destructor                           *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Fill histograms only (No Cuts)     *
 *    GetCutName()                      -- Returns "HistogrammingJet"         *
 *                                                                            *
 * Private Data Members of this class                                         *
 *  - lots of histograms of jet properties                                    *
 *                                                                            *
 * History                                                                    *
 *      14 Nov 2006 - Created by R. Schwienhorst for ATLAS                    *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingJet.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * HistogrammingJet::HistogrammingJet(EventContainer *obj)                    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event container                                                    *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingJet::HistogrammingJet(EventContainer *obj)
{
  SetEventContainer(obj);
} //HistogrammingJet()

/******************************************************************************
 * HistogrammingJet::~HistogrammingJet()                                      *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
HistogrammingJet::~HistogrammingJet()
{

} //HistogrammingJet

/******************************************************************************
 * void HistogrammingJet::BookHistogram()                                     *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void HistogrammingJet::BookHistogram(){
  
  // Histogram of Number of Objects
  _hNObj = DeclareTH1F("JetN","Number of Jets",10,0.,10.);
  _hNObj->SetXAxisTitle("N^{Jet}");
  _hNObj->SetYAxisTitle("Events");

  //Number of Tagged Jets
  _hNBJets = DeclareTH1F("BJetN","Number of b-jets", 10,0.,10.);
  _hNBJets->SetXAxisTitle("N^{b-Jet}");
  _hNBJets->SetYAxisTitle("Events");

  // Jet 1 Pt
  _hPtObj1 = DeclareTH1F("Jet1Pt","Jet 1 p_{T}",100,0.,400.);
  _hPtObj1->SetXAxisTitle("p_{T}^{Jet 1} [GeV]");
  _hPtObj1->SetYAxisTitle("Events");
  // Jet 1 Uncorr Pt
  _hUncorrPtObj1 = DeclareTH1F("Jet1UncorrPt","Jet 1 uncorrected p_{T}",100,0.,400.);
  _hUncorrPtObj1->SetXAxisTitle("p_{T}^{Jet 1, uncorrected} [GeV]");
  _hUncorrPtObj1->SetYAxisTitle("Events");
  // Jet 1 Eta
  _hEtaObj1 = DeclareTH1F("Jet1Eta","Jet 1 Pseudorapidity",50,-5.,5.);
  _hEtaObj1->SetXAxisTitle("#eta^{Jet 1}");
  _hEtaObj1->SetYAxisTitle("Events");
  // Jet 1 Phi
  _hPhiObj1 = DeclareTH1F("Jet1Phi","Jet 1 phi",50,-1.*TMath::Pi(),TMath::Pi());
  _hPhiObj1->SetXAxisTitle("#phi^{Jet 1}");
  _hPhiObj1->SetYAxisTitle("Events");
  // Jet 1 Tag
  _hTagObj1 = DeclareTH1F("Jet1Tag","Jet 1 tag",50,-1.,1.);
  _hTagObj1->SetXAxisTitle("Tag^{Jet 1}");
  _hTagObj1->SetYAxisTitle("Events");
  // Jet 1 tag vs flavour
  _hFlavVTag1 = DeclareTH2F("Jet1FlavVTag","Jet 1 flavour vs tag",3,0,3,50,-1.,1.);
  _hFlavVTag1->SetXAxisTitle("Flavour_{Jet_{1}}");
  _hFlavVTag1->SetYAxisTitle("Tag_{Jet_{1}}");
  // Jet 1 eta vs pt
  _hPtEtaObj1 = DeclareTH2F("Jet1PtEta","Jet 1 p_{T} vs #eta",100,0.,400.,50,-5.,5.);
  _hPtEtaObj1->SetXAxisTitle("p_{T}^{Jet 1} [GeV]");
  _hPtEtaObj1->SetYAxisTitle("#eta_{Jet 1}");
  // Jet 1 JER                                                
  _hjerObj1 = DeclareTH1F("Jet1JER","Jet 1 JER",100,0.8,1.5);
  _hjerObj1->SetXAxisTitle("JER^{Jet 1}");
  _hjerObj1->SetYAxisTitle("Events");
  // Jet 1 JES                                                
  _hjesObj1 = DeclareTH1F("Jet1JES","Jet 1 JES",100,0.8,1.5);
  _hjesObj1->SetXAxisTitle("JES^{Jet 1}");
  _hjesObj1->SetYAxisTitle("Events");
  // Jet 1 Pt no JER
  _hPtNoJERObj1 = DeclareTH1F("Jet1PtNoJER","Jet 1 p_{T} without JER",100,0.,400.);
  _hPtNoJERObj1->SetXAxisTitle("p_{T}^{Jet 1, no JER} [GeV]");
  _hPtNoJERObj1->SetYAxisTitle("Events");
  // Jet 1 Pt no JES
  _hPtNoJESObj1 = DeclareTH1F("Jet1PtNoJES","Jet 1 p_{T} without JES",100,0.,400.);
  _hPtNoJESObj1->SetXAxisTitle("p_{T}^{Jet 1, no JES} [GeV]");
  _hPtNoJESObj1->SetYAxisTitle("Events");
  // Jet 1 Pt no Smear
  _hPtNoSmearObj1 = DeclareTH1F("Jet1PtNoSmear","Jet 1 p_{T} without Smear",100,0.,400.);
  _hPtNoSmearObj1->SetXAxisTitle("p_{T}^{Jet 1, no Smear} [GeV]");
  _hPtNoSmearObj1->SetYAxisTitle("Events");
  // Jet 1 Pt no JES
  _hPtNoJERJESObj1 = DeclareTH1F("Jet1PtNoJERJES","Jet 1 p_{T} without JES",100,0.,400.);
  _hPtNoJERJESObj1->SetXAxisTitle("p_{T}^{Jet 1, no JES or JER} [GeV]");
  _hPtNoJERJESObj1->SetYAxisTitle("Events");
  // Jet 1 Pt with JES
  _hPtWithJESObj1 = DeclareTH1F("Jet1PtWithJES","Jet 1 p_{T} with JES",100,0.,400.);
  _hPtWithJESObj1->SetXAxisTitle("p_{T}^{Jet 1, with JES} [GeV]");
  _hPtWithJESObj1->SetYAxisTitle("Events");
  // Jet 1 Pt with JER
  _hPtWithJERObj1 = DeclareTH1F("Jet1PtWithJER","Jet 1 p_{T} with JER",100,0.,400.);
  _hPtWithJERObj1->SetXAxisTitle("p_{T}^{Jet 1, with JER} [GeV]");
  _hPtWithJERObj1->SetYAxisTitle("Events");
 // Jet 1 Pt with JERJES
  _hPtWithJERJESObj1 = DeclareTH1F("Jet1PtWithJERJES","Jet 1 p_{T} with JERJES",100,0.,400.);
  _hPtWithJERJESObj1->SetXAxisTitle("p_{T}^{Jet 1, with JERJES} [GeV]");
  _hPtWithJERJESObj1->SetYAxisTitle("Events");


  // Jet 2 Pt
  _hPtObj2 = DeclareTH1F("Jet2Pt","Jet 2 p_{T}",100,0.,400.);
  _hPtObj2->SetXAxisTitle("p_{T}^{Jet 2} [GeV]");
  _hPtObj2->SetYAxisTitle("Events");
  // Jet 2 Uncorr Pt
  _hUncorrPtObj2 = DeclareTH1F("Jet2UncorrPt","Jet 2 uncorrected p_{T}",100,0.,400.);
  _hUncorrPtObj2->SetXAxisTitle("p_{T}^{Jet 2, uncorrected} [GeV]");
  _hUncorrPtObj2->SetYAxisTitle("Events");
  // Jet 2 Eta
  _hEtaObj2 = DeclareTH1F("Jet2Eta","Jet 2 Pseudorapidity",50,-5.,5.);
  _hEtaObj2->SetXAxisTitle("#eta^{Jet 2} ");
  _hEtaObj2->SetYAxisTitle("Events");
  // Jet 2 Phi
  _hPhiObj2 = DeclareTH1F("Jet2Phi","Jet 2 phi",50,-1.*TMath::Pi(),TMath::Pi());
  _hPhiObj2->SetXAxisTitle("#phi^{Jet 2}");
  _hPhiObj2->SetYAxisTitle("Events");
  // Jet 2 Tag
  _hTagObj2 = DeclareTH1F("Jet2Tag","Jet 2 tag",50,-1.,1.);
  _hTagObj2->SetXAxisTitle("Tag^{Jet 2}");
  _hTagObj2->SetYAxisTitle("Events");
  // Jet 2 tag vs flavour
  _hFlavVTag2 = DeclareTH2F("Jet2FlavVTag","Jet 2 flavour vs tag",3,0,3,50,-1.,1.);
  _hFlavVTag2->SetXAxisTitle("Flavour_{Jet_{2}}");
  _hFlavVTag2->SetYAxisTitle("Tag_{Jet_{2}}");
  // Jet 2 eta vs pt
  _hPtEtaObj2 = DeclareTH2F("Jet2PtEta","Jet 2 p_{T} vs #eta",100,0.,400.,50,-5.,5.);
  _hPtEtaObj2->SetXAxisTitle("p_{T}^{Jet 2} [GeV]");
  _hPtEtaObj2->SetYAxisTitle("#eta_{Jet 2}");
  // Jet 2 JER                                                                           
  _hjerObj2 = DeclareTH1F("Jet2JER","Jet 2 JER",100,0.8,1.5);
  _hjerObj2->SetXAxisTitle("JER^{Jet 2}");
  _hjerObj2->SetYAxisTitle("Events");
  // Jet 2 JES                                                                           
  _hjesObj2 = DeclareTH1F("Jet2JES","Jet 2 JES",100,0.8,1.5);
  _hjesObj2->SetXAxisTitle("JES^{Jet 2}");
  _hjesObj2->SetYAxisTitle("Events");
  // Jet 2 Pt no JER                                                                     
  _hPtNoJERObj2 = DeclareTH1F("Jet2PtNoJER","Jet 2 p_{T} without JER",100,0.,400.);
  _hPtNoJERObj2->SetXAxisTitle("p_{T}^{Jet 2, no JER} [GeV]");
  _hPtNoJERObj2->SetYAxisTitle("Events");
  // Jet 2 Pt no JES                                                                     
  _hPtNoJESObj2 = DeclareTH1F("Jet2PtNoJES","Jet 2 p_{T} without JES",100,0.,400.);
  _hPtNoJESObj2->SetXAxisTitle("p_{T}^{Jet 2, no JES} [GeV]");
  _hPtNoJESObj2->SetYAxisTitle("Events");
  // Jet 2 Pt no Smear                                                                     
  _hPtNoSmearObj2 = DeclareTH1F("Jet2PtNoSmear","Jet 2 p_{T} without Smear",100,0.,400.);
  _hPtNoSmearObj2->SetXAxisTitle("p_{T}^{Jet 2, no Smear} [GeV]");
  _hPtNoSmearObj2->SetYAxisTitle("Events");
  // Jet 2 Pt no JES                                                                     
  _hPtNoJERJESObj2 = DeclareTH1F("Jet2PtNoJERJES","Jet 2 p_{T} without JES",100,0.,400.);
  _hPtNoJERJESObj2->SetXAxisTitle("p_{T}^{Jet 2, no JES or JER} [GeV]");
  _hPtNoJERJESObj2->SetYAxisTitle("Events");
  // Jet 2 Pt with JES
  _hPtWithJESObj2 = DeclareTH1F("Jet2PtWithJES","Jet 2 p_{T} with JES",100,0.,400.);
  _hPtWithJESObj2->SetXAxisTitle("p_{T}^{Jet 2, with JES} [GeV]");
  _hPtWithJESObj2->SetYAxisTitle("Events");
  // Jet 2 Pt with JER
  _hPtWithJERObj2 = DeclareTH1F("Jet2PtWithJER","Jet 2 p_{T} with JER",100,0.,400.);
  _hPtWithJERObj2->SetXAxisTitle("p_{T}^{Jet 2, with JER} [GeV]");
  _hPtWithJERObj2->SetYAxisTitle("Events");
  // Jet 2 Pt with JERJES
  _hPtWithJERJESObj2 = DeclareTH1F("Jet2PtWithJERJES","Jet 2 p_{T} with JERJES",100,0.,400.);
  _hPtWithJERJESObj2->SetXAxisTitle("p_{T}^{Jet 2, with JERJES} [GeV]");
  _hPtWithJERJESObj2->SetYAxisTitle("Events");

  // Jet 3 Pt
  _hPtObj3 = DeclareTH1F("Jet3Pt","Jet 3 p_{T}",100,0.,400.);
  _hPtObj3->SetXAxisTitle("p_{T}^{Jet 3} [GeV]");
  _hPtObj3->SetYAxisTitle("Events");
  // Jet 3 Uncorr Pt
  _hUncorrPtObj3 = DeclareTH1F("Jet3UncorrPt","Jet 3 uncorrected p_{T}",100,0.,400.);
  _hUncorrPtObj3->SetXAxisTitle("p_{T}^{Jet 3, uncorrected} [GeV]");
  _hUncorrPtObj3->SetYAxisTitle("Events");
  // Jet 3 Eta
  _hEtaObj3 = DeclareTH1F("Jet3Eta","Jet 3 Pseudorapidity",50,-5.,5.);
  _hEtaObj3->SetXAxisTitle("#eta^{Jet 3}");
  _hEtaObj3->SetYAxisTitle("Events");
  // Jet 3 Phi
  _hPhiObj3 = DeclareTH1F("Jet3Phi","Jet 3 phi",50,-1.*TMath::Pi(),TMath::Pi());
  _hPhiObj3->SetXAxisTitle("#phi^{Jet 3}");
  _hPhiObj3->SetYAxisTitle("Events");
  // Jet 3 Tag
  _hTagObj3 = DeclareTH1F("Jet3Tag","Jet 3 tag",50,-1.,1.);
  _hTagObj3->SetXAxisTitle("Tag^{Jet 3}");
  _hTagObj3->SetYAxisTitle("Events");
  // Jet 3 tag vs flavour
  _hFlavVTag3 = DeclareTH2F("Jet3FlavVTag","Jet 3 flavour vs tag",3,0,3,50,-1.,1.);
  _hFlavVTag3->SetXAxisTitle("Flavour_{Jet_{3}}");
  _hFlavVTag3->SetYAxisTitle("Tag_{Jet_{3}}");
  // Jet 3 eta vs pt
  _hPtEtaObj3 = DeclareTH2F("Jet3PtEta","Jet 3 p_{T} vs #eta",100,0.,400.,50,-5.,5.);
  _hPtEtaObj3->SetXAxisTitle("p_{T}^{Jet 3} [GeV]");
  _hPtEtaObj3->SetYAxisTitle("#eta_{Jet 3}");
  // Jet 3 JER                                                                           
  _hjerObj3 = DeclareTH1F("Jet3JER","Jet 3 JER",100,0.8,1.5);
  _hjerObj3->SetXAxisTitle("JER^{Jet 3}");
  _hjerObj3->SetYAxisTitle("Events");
  // Jet 3 JES                                                                           
  _hjesObj3 = DeclareTH1F("Jet3JES","Jet 3 JES",100,0.8,1.5);
  _hjesObj3->SetXAxisTitle("JES^{Jet 3}");
  _hjesObj3->SetYAxisTitle("Events");
  // Jet 3 Pt no JER                                                                     
  _hPtNoJERObj3 = DeclareTH1F("Jet3PtNoJER","Jet 3 p_{T} without JER",100,0.,400.);
  _hPtNoJERObj3->SetXAxisTitle("p_{T}^{Jet 3, no JER} [GeV]");
  _hPtNoJERObj3->SetYAxisTitle("Events");
  // Jet 3 Pt no JES                                                                     
  _hPtNoJESObj3 = DeclareTH1F("Jet3PtNoJES","Jet 3 p_{T} without JES",100,0.,400.);
  _hPtNoJESObj3->SetXAxisTitle("p_{T}^{Jet 3, no JES} [GeV]");
  _hPtNoJESObj3->SetYAxisTitle("Events");
  // Jet 3 Pt no Smear                                                                     
  _hPtNoSmearObj3 = DeclareTH1F("Jet3PtNoSmear","Jet 3 p_{T} without Smear",100,0.,400.);
  _hPtNoSmearObj3->SetXAxisTitle("p_{T}^{Jet 3, no Smear} [GeV]");
  _hPtNoSmearObj3->SetYAxisTitle("Events");
  // Jet 3 Pt no JES                                                                     
  _hPtNoJERJESObj3 = DeclareTH1F("Jet3PtNoJERJES","Jet 3 p_{T} without JES",100,0.,400.);
  _hPtNoJERJESObj3->SetXAxisTitle("p_{T}^{Jet 3, no JES or JER} [GeV]");
  _hPtNoJERJESObj3->SetYAxisTitle("Events");
  // Jet 3 Pt with JES
  _hPtWithJESObj3 = DeclareTH1F("Jet3PtWithJES","Jet 3 p_{T} with JES",100,0.,400.);
  _hPtWithJESObj3->SetXAxisTitle("p_{T}^{Jet 3, with JES} [GeV]");
  _hPtWithJESObj3->SetYAxisTitle("Events");
  // Jet 3 Pt with JER
  _hPtWithJERObj3 = DeclareTH1F("Jet3PtWithJER","Jet 3 p_{T} with JER",100,0.,400.);
  _hPtWithJERObj3->SetXAxisTitle("p_{T}^{Jet 3, with JER} [GeV]");
  _hPtWithJERObj3->SetYAxisTitle("Events");
  // Jet 3 Pt with JERJES
  _hPtWithJERJESObj3 = DeclareTH1F("Jet3PtWithJERJES","Jet 3 p_{T} with JERJES",100,0.,400.);
  _hPtWithJERJESObj3->SetXAxisTitle("p_{T}^{Jet 3, with JERJES} [GeV]");
  _hPtWithJERJESObj3->SetYAxisTitle("Events");

  // Jet 4 Pt
  _hPtObj4 = DeclareTH1F("Jet4Pt","Jet 4 p_{T}",100,0.,400.);
  _hPtObj4->SetXAxisTitle("p_{T}^{Jet 4} [GeV]");
  _hPtObj4->SetYAxisTitle("Events");
  // Jet 4 Eta
  _hEtaObj4 = DeclareTH1F("Jet4Eta","Jet 4 Pseudorapidity",50,-5.,5.);
  _hEtaObj4->SetXAxisTitle("#eta^{Jet 4}");
  _hEtaObj4->SetYAxisTitle("Events");
  // Jet 4 Phi
  _hPhiObj4 = DeclareTH1F("Jet4Phi","Jet 4 phi",50,-1.*TMath::Pi(),TMath::Pi());
  _hPhiObj4->SetXAxisTitle("#phi^{Jet 4}");
  _hPhiObj4->SetYAxisTitle("Events");
  // Jet 4 Tag
  _hTagObj4 = DeclareTH1F("Jet4Tag","Jet 4 tag",50,-1.,1.);
  _hTagObj4->SetXAxisTitle("Tag^{Jet 4}");
  _hTagObj4->SetYAxisTitle("Events");

  // Jet 5 Pt
  _hPtObj5 = DeclareTH1F("Jet5Pt","Jet 5 p_{T}",100,0.,400.);
  _hPtObj5->SetXAxisTitle("p_{T}^{Jet 5} [GeV]");
  _hPtObj5->SetYAxisTitle("Events");
  // Jet 5 Eta
  _hEtaObj5 = DeclareTH1F("Jet5Eta","Jet 5 Pseudorapidity",50,-5.,5.);
  _hEtaObj5->SetXAxisTitle("#eta^{Jet 5}");
  _hEtaObj5->SetYAxisTitle("Events");
  // Jet 5 Phi
  _hPhiObj5 = DeclareTH1F("Jet5Phi","Jet 5 phi",50,-1.*TMath::Pi(),TMath::Pi());
  _hPhiObj5->SetXAxisTitle("#phi^{Jet 5}");
  _hPhiObj5->SetYAxisTitle("Events");
  // Jet 5 Tag
  _hTagObj5 = DeclareTH1F("Jet5Tag","Jet 5 tag",50,-1.,1.);
  _hTagObj5->SetXAxisTitle("Tag^{Jet 5}");
  _hTagObj5->SetYAxisTitle("Events");

  // Jet 6 Pt
  _hPtObj6 = DeclareTH1F("Jet6Pt","Jet 6 p_{T}",100,0.,400.);
  _hPtObj6->SetXAxisTitle("p_{T}^{Jet 6} [GeV]");
  _hPtObj6->SetYAxisTitle("Events");
  // Jet 6 Eta
  _hEtaObj6 = DeclareTH1F("Jet6Eta","Jet 6 Pseudorapidity",50,-5.,5.);
  _hEtaObj6->SetXAxisTitle("#eta^{Jet 6}");
  _hEtaObj6->SetYAxisTitle("Events");
  // Jet 6 Phi
  _hPhiObj6 = DeclareTH1F("Jet6Phi","Jet 6 phi",50,-1.*TMath::Pi(),TMath::Pi());
  _hPhiObj6->SetXAxisTitle("#phi^{Jet 6}");
  _hPhiObj6->SetYAxisTitle("Events");
  // Jet 6 Tag
  _hTagObj6 = DeclareTH1F("Jet6Tag","Jet 6 tag",50,-1.,1.);
  _hTagObj6->SetXAxisTitle("Tag^{Jet 6}");
  _hTagObj6->SetYAxisTitle("Events");

  // Jet 7 Pt
  _hPtObj7 = DeclareTH1F("Jet7Pt","Jet 7 p_{T}",100,0.,400.);
  _hPtObj7->SetXAxisTitle("p_{T}^{Jet 7} [GeV]");
  _hPtObj7->SetYAxisTitle("Events");
  // Jet 7 Eta
  _hEtaObj7 = DeclareTH1F("Jet7Eta","Jet 7 Pseudorapidity",50,-5.,5.);
  _hEtaObj7->SetXAxisTitle("#eta^{Jet 7}");
  _hEtaObj7->SetYAxisTitle("Events");
  // Jet 7 Phi
  _hPhiObj7 = DeclareTH1F("Jet7Phi","Jet 7 phi",50,-1.*TMath::Pi(),TMath::Pi());
  _hPhiObj7->SetXAxisTitle("#phi^{Jet 7}");
  _hPhiObj7->SetYAxisTitle("Events");
  // Jet 7 Tag
  _hTagObj7 = DeclareTH1F("Jet7Tag","Jet 7 tag",50,-1.,1.);
  _hTagObj7->SetXAxisTitle("Tag^{Jet 7}");
  _hTagObj7->SetYAxisTitle("Events");

  // any Jet Pt
  _hPt = DeclareTH1F("JetPt","All Jet p_{T}",100,0.,400.);
  _hPt->SetXAxisTitle("p_{T}^{Jet} [GeV]");
  _hPt->SetYAxisTitle("Events");
  // any Jet Eta
  _hEta = DeclareTH1F("JetEta","All Jet Pseudorapidity",50,-5.,5.);
  _hEta->SetXAxisTitle("#eta^{Jet}");
  _hEta->SetYAxisTitle("Events");
  // any Jet Phi
  _hPhi = DeclareTH1F("JetPhi","All Jet phi",50,-1.*TMath::Pi(),TMath::Pi());
  _hPhi->SetXAxisTitle("#phi^{Jet}");
  _hPhi->SetYAxisTitle("Events");
  // any Jet Charge
  _hCharge = DeclareTH1F("JetCharge","All jet charge",3,-1.5,1.5);
  _hCharge->SetXAxisTitle("Charge^{jet}");
  _hCharge->SetYAxisTitle("Events");
  // any Jet E over P
  _hEtEM0 = DeclareTH1F("JetEtEM0","All Jet E_{T} in EM layer 0",100,0.,100.);
  _hEtEM0->SetXAxisTitle("E_{T}^{jet} [GeV]");
  _hEtEM0->SetYAxisTitle("Events");
  // any Jet E over P
  _hTruthLabel = DeclareTH1F("JetTruthLabel","All Jet Truth ID",100,-1050.,100.);
  _hTruthLabel->SetXAxisTitle("Truth Label");
  _hTruthLabel->SetYAxisTitle("Events");
  // any Jet tag vs flavour
  _hFlavVTag = DeclareTH2F("JetFlavVTag","All Jet flavour vs tag",3,0,3,50,-1.,1.);
  _hFlavVTag->SetXAxisTitle("Flavour_{Jet}");
  _hFlavVTag->SetYAxisTitle("Tag_{Jet}");  


  //cout<<"end of HistogrammingJet::BookHistogram"<<endl;

} //BookHistogram

/******************************************************************************
 * Bool_t HistogrammingJet::Apply()                                         *
 *                                                                            *
 * Fill histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t HistogrammingJet::Apply()
{
  //cout<<"Begin of HistogrammingJet::Apply()"<<endl;
  // Get Event Tree
  EventContainer *evc = GetEventContainer();

  // Fill Histograms
  _hNObj -> Fill(evc->jets.size());

  int jetFlavour;

  // Jet 1
  if(evc->jets.size()>0) {
    _hPtObj1  -> Fill(evc->jets[0].Pt());
    _hUncorrPtObj1  -> Fill(evc->jets[0].uncorrPt());
    _hEtaObj1 -> Fill(evc->jets[0].Eta());
    _hPhiObj1 -> Fill(evc->jets[0].Phi());
    _hTagObj1 -> Fill(evc->jets[0].bDiscriminator());
    jetFlavour = fabs(evc->jets[0].GethadronFlavour());
    if (jetFlavour == 4) jetFlavour = 1;
    if (jetFlavour == 5) jetFlavour = 2;
    _hFlavVTag1->Fill(jetFlavour,evc->jets[0].bDiscriminator());
    _hPtEtaObj1->Fill(evc->jets[0].Pt(),evc->jets[0].Eta());
    _hjerObj1->Fill(evc->jets[0].GetjerSF());
    _hjesObj1->Fill(evc->jets[0].GetjesSF());
    _hPtWithJESObj1->Fill(evc->jets[0].Pt() * evc->jets[0].GetjesSF());
    _hPtWithJERObj1->Fill(evc->jets[0].Pt() * evc->jets[0].GetjerSF());
    _hPtWithJERJESObj1->Fill(evc->jets[0].Pt() * evc->jets[0].GetjerSF() * evc->jets[0].GetjesSF());
    if (!evc->jets[0].GetjerSF() == 0.) _hPtNoJERObj1->Fill(evc->jets[0].Pt()/evc->jets[0].GetjerSF());
    if (!evc->jets[0].GetjesSF() == 0.) _hPtNoJESObj1->Fill(evc->jets[0].Pt()/evc->jets[0].GetjesSF());
    if (!evc->jets[0].GetjerSF() == 0. && !evc->jets[0].GetjesSF() == 0.) _hPtNoJERJESObj1->Fill(evc->jets[0].Pt()/(evc->jets[0].GetjerSF()*evc->jets[0].GetjesSF()));
    if (!evc->jets[0].GetsmearFactor() == 0.) _hPtNoSmearObj1->Fill(evc->jets[0].Pt()/evc->jets[0].GetsmearFactor());
    //jer/jes things
    
  } //if

  // Jet 2
  if(evc->jets.size()>1) {
    _hPtObj2  -> Fill(evc->jets[1].Pt());
    _hUncorrPtObj2  -> Fill(evc->jets[1].uncorrPt());
    _hEtaObj2 -> Fill(evc->jets[1].Eta());
    _hPhiObj2 -> Fill(evc->jets[1].Phi());
    _hTagObj2 -> Fill(evc->jets[1].bDiscriminator());
    jetFlavour = fabs(evc->jets[1].GethadronFlavour());
    if (jetFlavour == 4) jetFlavour = 1;
    if (jetFlavour == 5) jetFlavour = 2;
    _hFlavVTag2->Fill(jetFlavour,evc->jets[1].bDiscriminator());
    _hPtEtaObj2->Fill(evc->jets[1].Pt(),evc->jets[1].Eta());
    _hjerObj2->Fill(evc->jets[1].GetjerSF());
    _hjesObj2->Fill(evc->jets[1].GetjesSF());
    _hPtWithJESObj2->Fill(evc->jets[1].Pt() * evc->jets[1].GetjesSF());
    _hPtWithJERObj2->Fill(evc->jets[1].Pt() * evc->jets[1].GetjerSF());
    _hPtWithJERJESObj2->Fill(evc->jets[1].Pt() * evc->jets[1].GetjerSF() * evc->jets[1].GetjesSF());
    if (!evc->jets[1].GetjerSF() == 0.) _hPtNoJERObj2->Fill(evc->jets[1].Pt()/evc->jets[1].GetjerSF());
    if (!evc->jets[1].GetjesSF() == 0.) _hPtNoJESObj2->Fill(evc->jets[1].Pt()/evc->jets[1].GetjesSF());
    if (!evc->jets[1].GetjerSF() == 0. && !evc->jets[1].GetjesSF() == 0.) _hPtNoJERJESObj2->Fill(evc->jets[1].Pt()/(evc->jets[1].GetjerSF()*evc->jets[1].GetjesSF()));
    if (!evc->jets[1].GetsmearFactor() == 0.) _hPtNoSmearObj2->Fill(evc->jets[1].Pt()/evc->jets[1].GetsmearFactor());

  } //if
  
  // Jet 3
  if(evc->jets.size()>2) {
    _hPtObj3  -> Fill(evc->jets[2].Pt());
    _hUncorrPtObj3  -> Fill(evc->jets[2].uncorrPt());
    _hEtaObj3 -> Fill(evc->jets[2].Eta());
    _hPhiObj3 -> Fill(evc->jets[2].Phi());
    _hTagObj3 -> Fill(evc->jets[2].bDiscriminator());
    jetFlavour = fabs(evc->jets[2].GethadronFlavour());
    if (jetFlavour == 4) jetFlavour = 1;
    if (jetFlavour == 5) jetFlavour = 2;
    _hFlavVTag3->Fill(jetFlavour,evc->jets[2].bDiscriminator());
    _hPtEtaObj3->Fill(evc->jets[2].Pt(),evc->jets[2].Eta());
    _hjerObj3->Fill(evc->jets[2].GetjerSF());
    _hjesObj3->Fill(evc->jets[2].GetjesSF());
    _hPtWithJESObj3->Fill(evc->jets[2].Pt() * evc->jets[2].GetjesSF());
    _hPtWithJERObj3->Fill(evc->jets[2].Pt() * evc->jets[2].GetjerSF());
    _hPtWithJERJESObj3->Fill(evc->jets[2].Pt() * evc->jets[2].GetjerSF()) * evc->jets[2].GetjesSF();
    if (!evc->jets[2].GetjerSF() == 0.) _hPtNoJERObj3->Fill(evc->jets[2].Pt()/evc->jets[2].GetjerSF());
    if (!evc->jets[2].GetjesSF() == 0.) _hPtNoJESObj3->Fill(evc->jets[2].Pt()/evc->jets[2].GetjesSF());
    if (!evc->jets[2].GetjerSF() == 0. && !evc->jets[2].GetjesSF() == 0.) _hPtNoJERJESObj3->Fill(evc->jets[2].Pt()/(evc->jets[2].GetjerSF()*evc->jets[2].GetjesSF()));
    if (!evc->jets[2].GetsmearFactor() == 0.) _hPtNoSmearObj3->Fill(evc->jets[2].Pt()/evc->jets[2].GetsmearFactor());

  } //if

  // Jet 4
  if(evc->jets.size()>3) {
    _hPtObj4  -> Fill(evc->jets[3].Pt());
    _hEtaObj4 -> Fill(evc->jets[3].Eta());
    _hPhiObj4 -> Fill(evc->jets[3].Phi());
    _hTagObj4 -> Fill(evc->jets[3].bDiscriminator());
  } //if

  // Jet 5
  if(evc->jets.size()>4) {
    _hPtObj5  -> Fill(evc->jets[4].Pt());
    _hEtaObj5 -> Fill(evc->jets[4].Eta());
    _hPhiObj5 -> Fill(evc->jets[4].Phi());
    _hTagObj5 -> Fill(evc->jets[4].bDiscriminator());
  } //if

  // Jet 6
  if(evc->jets.size()>5) {
    _hPtObj6  -> Fill(evc->jets[5].Pt());
    _hEtaObj6 -> Fill(evc->jets[5].Eta());
    _hPhiObj6 -> Fill(evc->jets[5].Phi());
    _hTagObj6 -> Fill(evc->jets[5].bDiscriminator());
  } //if

  // Jet 7
  if(evc->jets.size()>6) {
    _hPtObj7  -> Fill(evc->jets[6].Pt());
    _hEtaObj7 -> Fill(evc->jets[6].Eta());
    _hPhiObj7 -> Fill(evc->jets[6].Phi());
    _hTagObj7 -> Fill(evc->jets[6].bDiscriminator());
  } //if

  // Loop over all jets and histogram their properties
  int nBJets = 0;
  for(int io = 0; io < evc -> jets.size(); io++) {
    _hPt     -> Fill(evc -> jets[io].Pt());
    _hEta    -> Fill(evc -> jets[io].Eta());
    _hPhi    -> Fill(evc -> jets[io].Phi());
    _hCharge -> Fill(evc -> jets[io].GetCharge());
    jetFlavour = fabs(evc->jets[io].GethadronFlavour());
    if (jetFlavour == 4) jetFlavour = 1;
    if (jetFlavour == 5) jetFlavour = 2;
    _hFlavVTag->Fill(jetFlavour,evc->jets[io].bDiscriminator());
    if (evc->jets[io].IsTagged()) nBJets++;
    //_hEtEM0  -> Fill(evc -> jets[io].GetEtEM0());
    //_hTruthLabel  -> Fill(evc -> jets[io].GetPdgId());
  } //for 
  
  _hNBJets->Fill(nBJets);

  //cout<<"End of HistogrammingJet::Apply()"<<endl;
  return kTRUE;  
  
} //Apply












