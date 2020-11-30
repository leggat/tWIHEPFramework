/******************************************************************************
 * CutMtW.cpp                                                       *
 *                                                                            *
 * Cuts on the requirement of a good primary vertex                           *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutMtW class                              *
 *    CutMtW()                     -- Parameterized Constructor         *
 *    ~CutMtW()                    -- Destructor                        *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutMtW"                *
 *                                                                            *
 * Private Data Members of CutMtW class                                 *
 *    myTH1F* _hMtWBefore;    -- Hist of PV before cut        *
 *    myTH1F* _hMtWAfter;     -- Hist of PV after cut         *
 *                                                                            *
 * History                                                                    *
 *      5th July 2016 - Created by Duncan Leggat                              *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Other/CutMtW.hpp"
#include <iostream>

using namespace std;

/******************************************************************************
 * CutMtW::CutMtW(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutMtW::CutMtW(EventContainer *EventContainerObj, bool unisolated)
{
  // Set Event Container
  _unisolated = unisolated;
  SetEventContainer(EventContainerObj);
} // CutMtW


/******************************************************************************
 * CutMtW::~CutMtW()                                              *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutMtW::~CutMtW()
{
  
}//~CutMtW

/******************************************************************************
 * void CutMtW::BookHistogram()                                         *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutMtW::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // Histogram Before Cut
  std::ostringstream histNameBeforeStream;
  histNameBeforeStream << "MtWBefore";
  TString histNameBefore = histNameBeforeStream.str().c_str();

  std::ostringstream histTitleBeforeStream;
  histTitleBeforeStream << "MtW Before";
  TString histTitleBefore = histTitleBeforeStream.str().c_str();

  // Histogram After Cut
  std::ostringstream histNameAfterStream;
  histNameAfterStream << "MtWAfter";
  TString histNameAfter = histNameAfterStream.str().c_str();

  std::ostringstream histTitleAfterStream;
  histTitleAfterStream << "MtW After";
  TString histTitleAfter = histTitleAfterStream.str().c_str();

  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // Histogram before cut
  _hMtWBefore =  DeclareTH1F(histNameBefore.Data(), histTitleBefore.Data(), 100, 0.0, 200.);
  _hMtWBefore -> SetXAxisTitle("MtW");
  _hMtWBefore -> SetYAxisTitle("Events");

  // Histogram after cut
  _hMtWAfter=  DeclareTH1F(histNameAfter.Data(), histTitleAfter.Data(), 100, 0.0, 200.);
  _hMtWAfter-> SetXAxisTitle("MtW");
  _hMtWAfter-> SetYAxisTitle("Events");


  // ***********************************************
  // Get the PV IDs from the config file. Perhaps these should be included as an object
  // ***********************************************

  //First, get the config file
  EventContainer *EventContainerObj = GetEventContainer();
  TEnv * config = EventContainerObj->GetConfig();

  //Then set the cuts here.
  _mtWMin = config -> GetValue("Cut.Event.MtW.Min",0.0);
  _mtWMax = config -> GetValue("Cut.Event.MtW.Max",999.0);

  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************
  ostringstream cutFlowTitleStream;
  ostringstream cutFlowNameStream;
  TString cutFlowTitle;
  TString cutFlowName;

  // Min cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << "MtW > " << _mtWMin;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << "MtW.Min";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Max cut
    cutFlowTitleStream.str("");
  cutFlowTitleStream << "MtW < " << _mtWMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << "MtW.Max";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  //Min + Max
  cutFlowTitleStream.str("");
  cutFlowTitleStream << _mtWMin << " < MtW < " << _mtWMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << "MtW.All";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());


}//BookHistograms()

/******************************************************************************
 * Bool_t CutMtW::Apply()                                               *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the selected trigger                                                *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutMtW::Apply()
{

  EventContainer *EventContainerObj = GetEventContainer();

  Bool_t passesMtWMinCut = kTRUE;
  Bool_t passesMtWMaxCut = kTRUE;

  //Float_t met = EventContainerObj->mtW;

  TLorentzVector lepton;

  if (!_unisolated){
    if (EventContainerObj->tightMuons.size() > 0){
      lepton = EventContainerObj->tightMuons[0];
    }
    else {
      lepton = EventContainerObj->tightElectrons[0];
    }
  }
  else {
    if (EventContainerObj->unIsolatedMuons.size() > 0){
      lepton = EventContainerObj->unIsolatedMuons[0];
    }
    else {
      lepton = EventContainerObj->unIsolatedElectrons[0];
    }
  }
  
  Float_t met = std::sqrt(2*EventContainerObj->missingEt*lepton.Pt()*(1-cos(EventContainerObj->missingPhi - lepton.Phi())));

  _hMtWBefore->Fill(met);

  ostringstream cutFlowNameMinStream;
  ostringstream cutFlowNameMaxStream;
  ostringstream cutFlowNameAllStream;
  
  TString cutFlowNameMin;
  TString cutFlowNameMax;
  TString cutFlowNameAll;

  cutFlowNameMinStream << "MtW.Min";
  cutFlowNameMin = cutFlowNameMinStream.str().c_str();

  cutFlowNameMaxStream << "MtW.Max";
  cutFlowNameMax = cutFlowNameMaxStream.str().c_str();

  cutFlowNameAllStream << "MtW.All";
  cutFlowNameAll = cutFlowNameAllStream.str().c_str();


  if (met > _mtWMin){
    GetCutFlowTable()->PassCut(cutFlowNameMin.Data());
  }
  else {
    GetCutFlowTable()->FailCut(cutFlowNameMin.Data());
    passesMtWMinCut = kFALSE;
  }
  if (met < _mtWMax){
    GetCutFlowTable()->PassCut(cutFlowNameMax.Data());
  }
  else {
    GetCutFlowTable()->FailCut(cutFlowNameMax.Data());
    passesMtWMaxCut = kFALSE;
  }
  if (passesMtWMaxCut && passesMtWMinCut){
    GetCutFlowTable()->PassCut(cutFlowNameAll);
    _hMtWAfter->Fill(met);
  }
  else{
    GetCutFlowTable()->FailCut(cutFlowNameAll);
  }

  return passesMtWMaxCut && passesMtWMinCut;

} //Apply












