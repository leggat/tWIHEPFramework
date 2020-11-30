/******************************************************************************
 * CutBarrelEndcapLepton.cpp                                                       *
 *                                                                            *
 * Cuts on the requirement of a good primary vertex                           *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutBarrelEndcapLepton class                              *
 *    CutBarrelEndcapLepton()                     -- Parameterized Constructor         *
 *    ~CutBarrelEndcapLepton()                    -- Destructor                        *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutBarrelEndcapLepton"                *
 *                                                                            *
 * Private Data Members of CutBarrelEndcapLepton class                                 *
 *    myTH1F* _hMissingEtBefore;    -- Hist of PV before cut        *
 *    myTH1F* _hMissingEtAfter;     -- Hist of PV after cut         *
 *                                                                            *
 * History                                                                    *
 *      2nd June 2020 - Created by Duncan Leggat                              *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Other/CutBarrelEndcapLepton.hpp"
#include <iostream>

using namespace std;

/******************************************************************************
 * CutBarrelEndcapLepton::CutBarrelEndcapLepton(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutBarrelEndcapLepton::CutBarrelEndcapLepton(EventContainer *EventContainerObj, Int_t barrelEndcap, bool unisolated)
{
  // Set Event Container
  SetEventContainer(EventContainerObj);
  _unisolated = unisolated;
  _detRegSelect = barrelEndcap;
} // CutBarrelEndcapLepton


/******************************************************************************
 * CutBarrelEndcapLepton::~CutBarrelEndcapLepton()                                              *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutBarrelEndcapLepton::~CutBarrelEndcapLepton()
{
  
}//~CutBarrelEndcapLepton

/******************************************************************************
 * void CutBarrelEndcapLepton::BookHistogram()                                         *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutBarrelEndcapLepton::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // Histogram Before Cut
  std::ostringstream histNameBeforeStream;
  histNameBeforeStream << "LepEtaBefore";
  TString histNameBefore = histNameBeforeStream.str().c_str();

  std::ostringstream histTitleBeforeStream;
  histTitleBeforeStream << "Lepton Eta Before";
  TString histTitleBefore = histTitleBeforeStream.str().c_str();

  // Histogram After Cut
  std::ostringstream histNameAfterStream;
  histNameAfterStream << "LepEtaAfter";
  TString histNameAfter = histNameAfterStream.str().c_str();

  std::ostringstream histTitleAfterStream;
  histTitleAfterStream << "Lepton Eta After";
  TString histTitleAfter = histTitleAfterStream.str().c_str();

  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // Histogram before cut
  _hLepEtaBefore =  DeclareTH1F(histNameBefore.Data(), histTitleBefore.Data(), 100, 0.0, 5.);
  _hLepEtaBefore -> SetXAxisTitle("Lepton Eta");
  _hLepEtaBefore -> SetYAxisTitle("Events");

  // Histogram after cut
  _hLepEtaAfter=  DeclareTH1F(histNameAfter.Data(), histTitleAfter.Data(), 100, 0.0, 5.);
  _hLepEtaAfter-> SetXAxisTitle("Lepton Eta");
  _hLepEtaAfter-> SetYAxisTitle("Events");


  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************
  ostringstream cutFlowTitleStream;
  ostringstream cutFlowNameStream;
  TString cutFlowTitle;
  TString cutFlowName;

  // Min cut
  cutFlowTitleStream << "Detector Region Selection";
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream << "DetRegionCut";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  //First, get the config file
  EventContainer *EventContainerObj = GetEventContainer();

}//BookHistograms()

/******************************************************************************
 * Bool_t CutBarrelEndcapLepton::Apply()                                               *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the selected trigger                                                *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutBarrelEndcapLepton::Apply()
{

  EventContainer *evc = GetEventContainer();

  bool isBarrel = true;                     
  
  Particle lepton;
                                        
  if (!_unisolated){                        
    if (evc->tightMuons.size() > 0){        
      lepton = evc->tightMuons[0];          
      isBarrel = fabs(lepton.Eta()) < 1.2;  
    }                                       
    else {                                  
      lepton = evc->tightElectrons[0];      
      isBarrel = fabs(lepton.Eta()) < 1.479;
    }                                       
  }                                         
  else {                                    
    if (evc->unIsolatedMuons.size() > 0){   
      lepton = evc->unIsolatedMuons[0];     
      isBarrel = fabs(lepton.Eta()) < 1.2;  
    }                                       
    else {                                  
      lepton = evc->unIsolatedElectrons[0]; 
      isBarrel = fabs(lepton.Eta()) < 1.479;
    }                                       
  }                                         

  _hLepEtaBefore->Fill(fabs(lepton.Eta()));

  Bool_t passesDetRegSelection = (isBarrel == _detRegSelect);

  TString cutFlowName = "DetRegionCut";  

  if (passesDetRegSelection){
    GetCutFlowTable()->PassCut(cutFlowName);
    _hLepEtaAfter->Fill(fabs(lepton.Eta()));
  }
  else {
    GetCutFlowTable()->FailCut(cutFlowName);
  }

  return passesDetRegSelection;

} //Apply












