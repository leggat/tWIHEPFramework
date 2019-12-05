/******************************************************************************
 * CutTaggedJetN.hpp                                                          *
 *                                                                            *
 * Cuts on number of tagged jets in an event                                  *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of CutTaggedJetN class                             *
 *    CutTaggedJetN()                   -- Parameterized Constructor          *
 *    ~CutTaggedJetN()                  -- Destructor                         *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Apply cuts and fill histograms     *
 *    GetCutName()                      -- Returns "CutTaggedJetN"            *
 *                                                                            *
 * Private Data Members of CutTaggedJetN                                      *
 *    myTH1F* _hJetNumberBefore         -- Hist of Num of jets before cuts    *
 *    myTH1F* _hJetNumberAfter          -- Hist of Num of jets before cuts    *
 *    Int_t _JetNumberMin;              -- Minimum number of jets to require  *
 *    Int_t _JetNumberMax;              -- Maximum number of jets allowed     *
 *                                                                            *
 * History                                                                    *
 *      23 Feb 2007 - Created by P. Ryan                                      *
 *****************************************************************************/


#include "SingleTopRootAnalysis/Cuts/TaggedJet/CutTaggedJetN.hpp"
#include <iostream>
#include <sstream>

using namespace std;


/******************************************************************************
 * CutTaggedJetN::CutTaggedJetN(EventContainer *EventContainerObj)            *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutTaggedJetN::CutTaggedJetN(EventContainer *EventContainerObj, Int_t nbJetsDefault)
{
  SetEventContainer(EventContainerObj);
  _JetNumberMin = nbJetsDefault; // Overriding configuration file.
  _JetNumberMax = nbJetsDefault;
  
} // CutTaggedJetN


/******************************************************************************
 * CutTaggedJetN::~CutTaggedJetN()                                            *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutTaggedJetN::~CutTaggedJetN()
{
  
}//~CutTaggedJetN

/******************************************************************************
 * void CutTaggedJetN::BookHistogram()                                        *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutTaggedJetN::BookHistogram(){
  
  // Histogram of events before cut on number of jets
  _hJetNumberBefore =  DeclareTH1F("TaggedJetNumberBefore","Number of Tagged Jets before the cut",10, 0.0, 10.0);
  _hJetNumberBefore -> SetXAxisTitle("N_{Jet}^{Tagged}");
  _hJetNumberBefore -> SetYAxisTitle("Events");

  // Histogram of events after cut on number of jets
  _hJetNumberAfter =  DeclareTH1F("TaggedJetNumberAfter","Number of Tagged Jets after the cut",10, 0.0, 10.0);
  _hJetNumberAfter -> SetXAxisTitle("N_{Jet}^{Tagged}");
  _hJetNumberAfter -> SetYAxisTitle("Events");

  // Histogram that will be filled if JES shifts the jet topology of the event"
  _hJetNumberTopologyShift = DeclareTH1F("bJetTopologyChangeJESShift","Number of events in which the topolgy changes with b-jet JES shift",2,0,2.);
  _hJetNumberTopologyShift -> SetXAxisTitle("1 change");
  _hJetNumberTopologyShift -> SetYAxisTitle("Events");
  
  // Get configuration
  EventContainer *EventContainerObj = GetEventContainer();
  TEnv *config = EventContainerObj -> GetConfig();

  // Use configuration to set min and max number of jets to cut on if default value used
  if (_JetNumberMax < 0.){
    _JetNumberMin = config -> GetValue("Cut.Tagged.Jet.Number.Min", 999);
    _JetNumberMax = config -> GetValue("Cut.Tagged.Jet.Number.Max", 999);
  }
  
  //
  // also add these two cuts to the cut flow table
  ostringstream titleStr;
  titleStr <<"Tagged Jet : " << "N >=" << _JetNumberMin;
  TString tTitle(titleStr.str().c_str());
  GetCutFlowTable()->AddCutToFlow("Tagged.Jet.Number.Min",tTitle);
  titleStr.str("");
  titleStr <<"Tagged Jet : N <= "<<_JetNumberMax;
  tTitle=titleStr.str().c_str();
  GetCutFlowTable()->AddCutToFlow("Tagged.Jet.Number.Max",tTitle);
  // also add the overal pass rate
  titleStr.str("");
  titleStr <<"Tagged Jet : "<< _JetNumberMin<<" <= N <= "<<_JetNumberMax;
  tTitle=titleStr.str().c_str();
  GetCutFlowTable()->AddCutToFlow("Tagged.Jet.Number.All",tTitle);
  
}//BookHistograms()

/******************************************************************************
 * Bool_t CutTaggedJetN::Apply()                                              *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutTaggedJetN::Apply()
{
  //cout<<"Begin of CutTaggedJetN::Apply()"<<endl;

  // Get Event Tree
  EventContainer *evObj = GetEventContainer();

  Int_t JetNumber = evObj->taggedJets.size();

  Int_t minJets = JetNumber;
  Int_t maxJets = JetNumber;

  if (evObj->jets.size() > 0){
    for (int i = 0; i > evObj->jets[0].GetNumberOfJESCorrections(); i++){
      Int_t nBJets = 0;
      for (auto const jet : evObj->jesShiftedJets[i]){
	if (jet.IsTagged()) nBJets++;
      }
      if (nBJets < minJets) minJets = nBJets;
      if (nBJets > maxJets) maxJets = nBJets;
    }
  }

  if (minJets == JetNumber && maxJets == JetNumber) _hJetNumberTopologyShift->Fill(0.);
  else _hJetNumberTopologyShift->Fill(1.);

  // Fill the histogram before the cut
  _hJetNumberBefore -> Fill(JetNumber);

  // Flags 
  Bool_t JetNumberMinPass    = kTRUE;   // Event passes min cut
  Bool_t JetNumberMaxPass    = kTRUE;   // Event passes max cut

  // Cut on Min Pt of Jet
  // Negative cut value for Min means there is no Min cut
  if( (_JetNumberMin != 999) && (maxJets < _JetNumberMin) ){
     JetNumberMinPass = kFALSE;
     GetCutFlowTable()->FailCut("Tagged.Jet.Number.Min");
  }//if
  else{
    GetCutFlowTable() -> PassCut("Tagged.Jet.Number.Min");
  } //else

  // Cut on Min Pt of Jet
  // Negative cut value for Min means there is no Min cut
  if( (_JetNumberMax != 999) && (minJets > _JetNumberMax) ){
     JetNumberMaxPass = kFALSE;
     GetCutFlowTable()->FailCut("Tagged.Jet.Number.Max");
  } //if
  else{
    GetCutFlowTable() -> PassCut("Tagged.Jet.Number.Max");
  } //else

  // Fill Histogram after cuts
  if(JetNumberMinPass && JetNumberMaxPass){
    _hJetNumberAfter -> Fill(JetNumber);
    GetCutFlowTable() -> PassCut("Tagged.Jet.Number.All");
  } //if
  else GetCutFlowTable() -> FailCut("Tagged.Jet.Number.All");

  if(JetNumberMinPass && JetNumberMaxPass){
    //  cout<<"EventNumber : "<<evObj->eventNumber<<endl;
  }

  return(JetNumberMinPass && JetNumberMaxPass);

} //Apply












