/******************************************************************************
 * CutMissingEt.cpp                                                       *
 *                                                                            *
 * Cuts on the requirement of a good primary vertex                           *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutMissingEt class                              *
 *    CutMissingEt()                     -- Parameterized Constructor         *
 *    ~CutMissingEt()                    -- Destructor                        *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutMissingEt"                *
 *                                                                            *
 * Private Data Members of CutMissingEt class                                 *
 *    myTH1F* _hMissingEtBefore;    -- Hist of PV before cut        *
 *    myTH1F* _hMissingEtAfter;     -- Hist of PV after cut         *
 *                                                                            *
 * History                                                                    *
 *      5th July 2016 - Created by Duncan Leggat                              *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Other/CutMissingEt.hpp"
#include <iostream>

using namespace std;

/******************************************************************************
 * CutMissingEt::CutMissingEt(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutMissingEt::CutMissingEt(EventContainer *EventContainerObj)
{
  // Set Event Container
  SetEventContainer(EventContainerObj);
} // CutMissingEt


/******************************************************************************
 * CutMissingEt::~CutMissingEt()                                              *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutMissingEt::~CutMissingEt()
{
  
}//~CutMissingEt

/******************************************************************************
 * void CutMissingEt::BookHistogram()                                         *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutMissingEt::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // Histogram Before Cut
  std::ostringstream histNameBeforeStream;
  histNameBeforeStream << "MissingEtBefore";
  TString histNameBefore = histNameBeforeStream.str().c_str();

  std::ostringstream histTitleBeforeStream;
  histTitleBeforeStream << "MissingEt Before";
  TString histTitleBefore = histTitleBeforeStream.str().c_str();

  // Histogram After Cut
  std::ostringstream histNameAfterStream;
  histNameAfterStream << "MissingEtAfter";
  TString histNameAfter = histNameAfterStream.str().c_str();

  std::ostringstream histTitleAfterStream;
  histTitleAfterStream << "MissingEt After";
  TString histTitleAfter = histTitleAfterStream.str().c_str();

  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // Histogram before cut
  _hMissingEtBefore =  DeclareTH1F(histNameBefore.Data(), histTitleBefore.Data(), 100, 0.0, 200.);
  _hMissingEtBefore -> SetXAxisTitle("Missing Et");
  _hMissingEtBefore -> SetYAxisTitle("Events");

  // Histogram after cut
  _hMissingEtAfter=  DeclareTH1F(histNameAfter.Data(), histTitleAfter.Data(), 100, 0.0, 200.);
  _hMissingEtAfter-> SetXAxisTitle("Missing Et");
  _hMissingEtAfter-> SetYAxisTitle("Events");


  //First, get the config file
  EventContainer *EventContainerObj = GetEventContainer();
  TEnv * config = EventContainerObj->GetConfig();

  //Then set the cuts here.
  _missingEtMin = config -> GetValue("Cut.Event.MissingEt.Min",0.0);
  _missingEtMax = config -> GetValue("Cut.Event.MissingEt.Max",999.0);

  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************
  ostringstream cutFlowTitleStream;
  ostringstream cutFlowNameStream;
  TString cutFlowTitle;
  TString cutFlowName;

  // Min cut
  cutFlowTitleStream.str("");
  cutFlowTitleStream << "MET > " << _missingEtMin;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << "MET.Min";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  // Max cut
    cutFlowTitleStream.str("");
  cutFlowTitleStream << "MET < " << _missingEtMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << "MET.Max";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  //Min + Max
  cutFlowTitleStream.str("");
  cutFlowTitleStream << _missingEtMin << " < MET < " << _missingEtMax;
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream.str("");
  cutFlowNameStream << "MET.All";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());


  // ***********************************************
  // Get the PV IDs from the config file. Perhaps these should be included as an object
  // ***********************************************



}//BookHistograms()

/******************************************************************************
 * Bool_t CutMissingEt::Apply()                                                *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the selected trigger                                                *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutMissingEt::Apply()
{

  EventContainer *EventContainerObj = GetEventContainer();

  Bool_t passesMETMinCut = kTRUE;
  Bool_t passesMETMaxCut = kTRUE;

  Float_t met = EventContainerObj->missingEt_xy;
  //Float_t met = EventContainerObj->missingEt;

  _hMissingEtBefore->Fill(met);

  ostringstream cutFlowNameMinStream;
  ostringstream cutFlowNameMaxStream;
  ostringstream cutFlowNameAllStream;
  
  TString cutFlowNameMin;
  TString cutFlowNameMax;
  TString cutFlowNameAll;

  cutFlowNameMinStream << "MET.Min";
  cutFlowNameMin = cutFlowNameMinStream.str().c_str();

  cutFlowNameMaxStream << "MET.Max";
  cutFlowNameMax = cutFlowNameMaxStream.str().c_str();

  cutFlowNameAllStream << "MET.All";
  cutFlowNameAll = cutFlowNameAllStream.str().c_str();


  if (met > _missingEtMin){
    GetCutFlowTable()->PassCut(cutFlowNameMin.Data());
  }
  else {
    GetCutFlowTable()->FailCut(cutFlowNameMin.Data());
    passesMETMinCut = kFALSE;
  }
  if (met < _missingEtMax){
    GetCutFlowTable()->PassCut(cutFlowNameMax.Data());
  }
  else {
    GetCutFlowTable()->FailCut(cutFlowNameMax.Data());
    passesMETMaxCut = kFALSE;
  }
  if (passesMETMaxCut && passesMETMinCut){
    GetCutFlowTable()->PassCut(cutFlowNameAll);
    _hMissingEtAfter->Fill(met);
  }
  else{
    GetCutFlowTable()->FailCut(cutFlowNameAll);
  }

  return passesMETMaxCut && passesMETMinCut;

} //Apply












