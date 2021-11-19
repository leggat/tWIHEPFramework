/******************************************************************************
 * MatchTriggerMuon.hpp                                                               *
 *                                                                            *
 * Cuts on muon Number (can cut on All, UnIsolated, Tight, or Veto Muons)     *
 * Must pass All, UnIsolated, Tight, or Veto to constructor                   *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of MatchTriggerMuon class                                  *
 *    MatchTriggerMuon()                     -- Parameterized Constructor             *
 *    ~MatchTriggerMuon()                    -- Destructor                            *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "MatchTriggerMuon"                    *
 *                                                                            *
 * Private Data Members of MatchTriggerMuon class                                     *
 *    myTH1F* _hMuonNumberBefore;    -- Hist mu Number before cut             *
 *    myTH1F* _hMuonNumberAfter;     -- Hist mu Number of jets after cut      *
 *                                                                            *
 *    Int_t _MuonNumberMin;          -- Minimum Muon Number                   *
 *    Int_t _MuonNumberMax;          -- Maximum Muon Number                   *
 *                                                                            *
 * History                                                                    *
 *      15 Jan 2007 - Created by P. Ryan                                      *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Muon/MatchTriggerMuon.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * MatchTriggerMuon::MatchTriggerMuon(EventContainer *EventContainerObj, TString muonType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
MatchTriggerMuon::MatchTriggerMuon(EventContainer *EventContainerObj, TString muonTypePassed)
{
  // Check muonType parameter
  if (!EventContainerObj->IsValidCollection("Muon",muonTypePassed)){
    std::cout << "ERROR " << "<MatchTriggerMuon::MatchTriggerMuon()> " 
	      << "Incorrect muon type " << muonTypePassed << " passed. Exiting..." << std::endl;
    exit(8);
  } //if
  muonType = muonTypePassed;

  // Set Event Container
  SetEventContainer(EventContainerObj);
} // MatchTriggerMuon


/******************************************************************************
 * MatchTriggerMuon::~MatchTriggerMuon()                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
MatchTriggerMuon::~MatchTriggerMuon()
{
  
}//~MatchTriggerMuon

/******************************************************************************
 * void MatchTriggerMuon::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void MatchTriggerMuon::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // Histogram Before Cut
  std::ostringstream histNameBeforeStream;
  histNameBeforeStream << muonType << "TrigObjDeltaR";
  TString histNameBefore = histNameBeforeStream.str().c_str();

  std::ostringstream histTitleBeforeStream;
  histTitleBeforeStream << muonType << " muon trig obj delta R ";
  TString histTitleBefore = histTitleBeforeStream.str().c_str();

  // Histogram After Cut
  std::ostringstream histNameAfterStream;
  histNameAfterStream << muonType << "TrigObjDelta";
  TString histNameAfter = histNameAfterStream.str().c_str();

  std::ostringstream histTitleAfterStream;
  histTitleAfterStream << muonType << " muon trig obj delta Pt ";
  TString histTitleAfter = histTitleAfterStream.str().c_str();

  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // Number of trigger objects with muon ID
  _hMuonTrigObjN =  DeclareTH1F("MuTrigObjs", "Number of muon trigger objects" , 10, 0.0, 10.0);
  _hMuonTrigObjN -> SetXAxisTitle("N_{#mu trigger}");
  _hMuonTrigObjN -> SetYAxisTitle("Events");

  // Delta R between muon and trigger muon
  _hMuonTrigDeltaR =  DeclareTH1F(histNameBefore.Data(), histTitleBefore.Data(), 100, 0.0, 7.);
  _hMuonTrigDeltaR -> SetXAxisTitle("#Delta(R)_{#mu,trig}");
  _hMuonTrigDeltaR -> SetYAxisTitle("Events");

  // Delta Pt between muon and trigger muon
  _hMuonTrigDeltaPt =  DeclareTH1F(histNameAfter.Data(), histTitleAfter.Data(), 100, 0.0, 100.);
  _hMuonTrigDeltaPt -> SetXAxisTitle("#Delta(Pt)_{#mu,trig}");
  _hMuonTrigDeltaPt -> SetYAxisTitle("Events");
  
}//BookHistograms()

/******************************************************************************
 * Bool_t MatchTriggerMuon::Apply()                                                *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the number of electrons and the p_T of these electrons              *
 * Apply separate p_T cuts to the first and second (don't worry about others) *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t MatchTriggerMuon::Apply()
{

  // Get Event Container                                   
  EventContainer *EventContainerObj = GetEventContainer();

  // Get muon collection
  std::vector<Muon> muons = EventContainerObj->GetMuonCollection(muonType);

  // GetTriggerObjs
  std::vector<TriggerObj> trigObjs = EventContainerObj->triggerObjects;

  //Find a muon tirgger objects
  Float_t largestPt = 0.;
  Int_t nMuonTrigObjs = 0;
  Int_t trigMuonIndex = -1;

  Int_t indexCounter = 0;

  for (auto trigObj: trigObjs){
    indexCounter++;
    if (! (trigObj.Getid() == 13)) continue;
    nMuonTrigObjs++;
    if (trigObj.Pt() > largestPt){
      largestPt = trigObj.Pt();
      trigMuonIndex = indexCounter - 1;
    }
  }

  //There are no trigger objects. That's okay, move on?
  if (trigMuonIndex < 0) return kTRUE;

  _hMuonTrigObjN->FillWithoutWeight(nMuonTrigObjs);

  TriggerObj trigMu = trigObjs[trigMuonIndex];

  

  Float_t smallestDeltaR = 999.;

  Int_t muonIndex = 0;
  Int_t chosenMuon = -99;

  for (auto muon: muons){
    muonIndex++;
    if (muon.DeltaR(trigMu) > smallestDeltaR) continue;
      smallestDeltaR = muon.DeltaR(trigMu);
    chosenMuon = muonIndex-1;
  }

  

  if (chosenMuon > -1){
    EventContainerObj->SetObjectIsTrigger("Muon",muonType,chosenMuon);
    _hMuonTrigDeltaR->FillWithoutWeight(muons[chosenMuon].DeltaR(trigMu));
    _hMuonTrigDeltaPt->FillWithoutWeight(fabs(muons[chosenMuon].Pt() - trigMu.Pt()));
  }

  return kTRUE;
 
} //Apply












