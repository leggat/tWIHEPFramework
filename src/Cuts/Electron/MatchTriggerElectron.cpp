/******************************************************************************
 * MatchTriggerElectron.hpp                                                               *
 *                                                                            *
 * Cuts on electron Number (can cut on All, UnIsolated, Tight, or Veto Electrons)     *
 * Must pass All, UnIsolated, Tight, or Veto to constructor                   *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of MatchTriggerElectron class                                  *
 *    MatchTriggerElectron()                     -- Parameterized Constructor             *
 *    ~MatchTriggerElectron()                    -- Destructor                            *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "MatchTriggerElectron"                    *
 *                                                                            *
 * Private Data Members of MatchTriggerElectron class                                     *
 *    myTH1F* _hElectronNumberBefore;    -- Hist ele Number before cut             *
 *    myTH1F* _hElectronNumberAfter;     -- Hist ele Number of jets after cut      *
 *                                                                            *
 *    Int_t _ElectronNumberMin;          -- Minimum Electron Number                   *
 *    Int_t _ElectronNumberMax;          -- Maximum Electron Number                   *
 *                                                                            *
 * History                                                                    *
 *      15 Jan 2007 - Created by P. Ryan                                      *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Electron/MatchTriggerElectron.hpp"
#include<iostream>

using namespace std;

/******************************************************************************
 * MatchTriggerElectron::MatchTriggerElectron(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
MatchTriggerElectron::MatchTriggerElectron(EventContainer *EventContainerObj, TString electronTypePassed)
{
  // Check electronType parameter
  if (!EventContainerObj->IsValidCollection("Electron",electronTypePassed)){
    std::cout << "ERROR " << "<MatchTriggerElectron::MatchTriggerElectron()> " 
	      << "Incorrect electron type " << electronTypePassed << " passed. Exiting..." << std::endl;
    exit(8);
  } //if
  electronType = electronTypePassed;

  // Set Event Container
  SetEventContainer(EventContainerObj);
} // MatchTriggerElectron


/******************************************************************************
 * MatchTriggerElectron::~MatchTriggerElectron()                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
MatchTriggerElectron::~MatchTriggerElectron()
{
  
}//~MatchTriggerElectron

/******************************************************************************
 * void MatchTriggerElectron::BookHistogram()                                          *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void MatchTriggerElectron::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // Histogram Before Cut
  std::ostringstream histNameBeforeStream;
  histNameBeforeStream << electronType << "TrigObjDeltaR";
  TString histNameBefore = histNameBeforeStream.str().c_str();

  std::ostringstream histTitleBeforeStream;
  histTitleBeforeStream << electronType << " electron trig obj delta R ";
  TString histTitleBefore = histTitleBeforeStream.str().c_str();

  // Histogram After Cut
  std::ostringstream histNameAfterStream;
  histNameAfterStream << electronType << "TrigObjDelta";
  TString histNameAfter = histNameAfterStream.str().c_str();

  std::ostringstream histTitleAfterStream;
  histTitleAfterStream << electronType << " electron trig obj delta Pt ";
  TString histTitleAfter = histTitleAfterStream.str().c_str();

  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // Number of trigger objects with electron ID
  _hElectronTrigObjN =  DeclareTH1F("EleTrigObjs", "Number of electron trigger objects" , 10, 0.0, 10.0);
  _hElectronTrigObjN -> SetXAxisTitle("N_{#ele trigger}");
  _hElectronTrigObjN -> SetYAxisTitle("Events");

  // Delta R between electron and trigger electron
  _hElectronTrigDeltaR =  DeclareTH1F(histNameBefore.Data(), histTitleBefore.Data(), 100, 0.0, 7.);
  _hElectronTrigDeltaR -> SetXAxisTitle("#Delta(R)_{#ele,trig}");
  _hElectronTrigDeltaR -> SetYAxisTitle("Events");

  // Delta Pt between electron and trigger electron
  _hElectronTrigDeltaPt =  DeclareTH1F(histNameAfter.Data(), histTitleAfter.Data(), 100, 0.0, 100.);
  _hElectronTrigDeltaPt -> SetXAxisTitle("#Delta(Pt)_{#ele,trig}");
  _hElectronTrigDeltaPt -> SetYAxisTitle("Events");
  
}//BookHistograms()

/******************************************************************************
 * Bool_t MatchTriggerElectron::Apply()                                                *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the number of electrons and the p_T of these electrons              *
 * Apply separate p_T cuts to the first and second (don't worry about others) *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t MatchTriggerElectron::Apply()
{

  // Get Event Container                                   
  EventContainer *EventContainerObj = GetEventContainer();

  // Get electron collection
  std::vector<Electron> electrons = EventContainerObj->GetElectronCollection(electronType);

  // GetTriggerObjs
  std::vector<TriggerObj> trigObjs = EventContainerObj->triggerObjects;

  //Find a electron tirgger objects
  Float_t largestPt = 0.;
  Int_t nElectronTrigObjs = 0;
  Int_t trigElectronIndex = -1;

  Int_t indexCounter = 0;

  for (auto trigObj: trigObjs){
    indexCounter++;
    if (! (trigObj.Getid() == 11)) continue;
    nElectronTrigObjs++;
    if (trigObj.Pt() > largestPt){
      largestPt = trigObj.Pt();
      trigElectronIndex = indexCounter - 1;
    }
  }

  //There are no trigger objects. That's okay, move on?
  if (trigElectronIndex < 0) return kTRUE;

  _hElectronTrigObjN->FillWithoutWeight(nElectronTrigObjs);

  TriggerObj trigEle = trigObjs[trigElectronIndex];

  

  Float_t smallestDeltaR = 999.;

  Int_t electronIndex = 0;
  Int_t chosenElectron = -99;

  for (auto electron: electrons){
    electronIndex++;
    if (electron.DeltaR(trigEle) > smallestDeltaR) continue;
      smallestDeltaR = electron.DeltaR(trigEle);
    chosenElectron = electronIndex-1;
  }

  

  if (chosenElectron > -1){
    EventContainerObj->SetObjectIsTrigger("Electron",electronType,chosenElectron);
    _hElectronTrigDeltaR->FillWithoutWeight(electrons[chosenElectron].DeltaR(trigEle));
    _hElectronTrigDeltaPt->FillWithoutWeight(fabs(electrons[chosenElectron].Pt() - trigEle.Pt()));
  }

  return kTRUE;
 
} //Apply












