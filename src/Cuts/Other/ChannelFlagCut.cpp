/******************************************************************************
 * ChannelFlagCut.cpp                                                       *
 *                                                                            *
 * Cuts on the requirement of a good primary vertex                           *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of ChannelFlagCut class                              *
 *    ChannelFlagCut()                     -- Parameterized Constructor         *
 *    ~ChannelFlagCut()                    -- Destructor                        *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "ChannelFlagCut"                *
 *                                                                            *
 * Private Data Members of ChannelFlagCut class                                 *
 *    myTH1F* _hMissingEtBefore;    -- Hist of PV before cut        *
 *    myTH1F* _hMissingEtAfter;     -- Hist of PV after cut         *
 *                                                                            *
 * History                                                                    *
 *      5th July 2016 - Created by Duncan Leggat                              *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Other/ChannelFlagCut.hpp"
#include <iostream>

using namespace std;

/******************************************************************************
 * ChannelFlagCut::ChannelFlagCut(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
ChannelFlagCut::ChannelFlagCut(EventContainer *EventContainerObj, Int_t channel)
{
  // Set Event Container
  SetEventContainer(EventContainerObj);
  _channel = channel;
} // ChannelFlagCut


/******************************************************************************
 * ChannelFlagCut::~ChannelFlagCut()                                              *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
ChannelFlagCut::~ChannelFlagCut()
{
  
}//~ChannelFlagCut

/******************************************************************************
 * void ChannelFlagCut::BookHistogram()                                         *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void ChannelFlagCut::BookHistogram(){
  
  // ***********************************************
  // Make Strings for histogram titles and labels
  // ***********************************************  

  // Histogram Before Cut
  std::ostringstream histNameBeforeStream;
  histNameBeforeStream << "ChannelFlagBefore";
  TString histNameBefore = histNameBeforeStream.str().c_str();

  std::ostringstream histTitleBeforeStream;
  histTitleBeforeStream << "ChannelFlag Before";
  TString histTitleBefore = histTitleBeforeStream.str().c_str();

  // Histogram After Cut
  std::ostringstream histNameAfterStream;
  histNameAfterStream << "ChannelFlagAfter";
  TString histNameAfter = histNameAfterStream.str().c_str();

  std::ostringstream histTitleAfterStream;
  histTitleAfterStream << "ChannelFlag After";
  TString histTitleAfter = histTitleAfterStream.str().c_str();

  // ***********************************************
  // Book Histograms
  // ***********************************************  

  // Histogram before cut
  _hChannelFlagBefore =  DeclareTH1F(histNameBefore.Data(), histTitleBefore.Data(), 5, 0.0, 5.);
  _hChannelFlagBefore -> SetXAxisTitle("Missing Et");
  _hChannelFlagBefore -> SetYAxisTitle("Events");

  // Histogram after cut
  _hChannelFlagAfter=  DeclareTH1F(histNameAfter.Data(), histTitleAfter.Data(), 5, 0.0, 5.);
  _hChannelFlagAfter-> SetXAxisTitle("Missing Et");
  _hChannelFlagAfter-> SetYAxisTitle("Events");


  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************
  ostringstream cutFlowTitleStream;
  ostringstream cutFlowNameStream;
  TString cutFlowTitle;
  TString cutFlowName;

  // Min cut
  cutFlowTitleStream << "Channel Selection";
  cutFlowTitle = cutFlowTitleStream.str().c_str();

  cutFlowNameStream << "ChannelCut";
  cutFlowName = cutFlowNameStream.str().c_str();

  GetCutFlowTable()->AddCutToFlow(cutFlowName.Data(), cutFlowTitle.Data());

  //First, get the config file
  EventContainer *EventContainerObj = GetEventContainer();

}//BookHistograms()

/******************************************************************************
 * Bool_t ChannelFlagCut::Apply()                                               *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the selected trigger                                                *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t ChannelFlagCut::Apply()
{

  EventContainer *EventContainerObj = GetEventContainer();

  EventTree * eventTree = EventContainerObj->GetEventTree();

  Bool_t passesMETCut = kFALSE;

  //  Float_t met = EventContainerObj->missingEt_xy;

  Int_t channel_tree = eventTree->channel;

  _hChannelFlagBefore->Fill(channel_tree);

  ostringstream cutFlowNameStream;
                                
  TString cutFlowName;            

  cutFlowNameStream << "ChannelCut";
  cutFlowName = cutFlowNameStream.str().c_str();

  //Temporary change whilst the flag is broken
  int nJets = EventContainerObj->jets.size();       
  int nbJets = EventContainerObj->taggedJets.size();

  Int_t channel = -1;

  if (nJets == 2 && nbJets == 1) channel = 2;
  else if (nJets == 3 && nbJets == 1) channel = 0;
  else if (nJets == 4 && nbJets == 1) channel = 3;
  else if (nJets == 3 && nbJets == 2) channel = 1;
  else if (nJets == 4 && nbJets == 2) channel = 4;
  // Adding in additional channels for 0 tags and also no tag information ofr plotting purpose
  else if (nJets == 2 && nbJets == 0) channel = 5;
  else if (nJets == 3 && nbJets == 0) channel = 6;
  else if (nJets == 4 && nbJets == 0) channel = 7;
  else if (nJets == 2) channel = 8;
  else if (nJets == 3) channel = 9;
  else if (nJets == 4) channel = 10;


  //  if (EventContainerObj->eventNumber == 951498 ) std::cout << "nJets: " << nJets << " nbJets: " << nbJets << " channel: " << channel << " tree_channel: "  << channel_tree << std::endl;
  //  if (channel == 1) std::cout << EventContainerObj->eventNumber << " weight: " << EventContainerObj->GetEventWeight() << " outputWeight: " <<EventContainerObj->GetOutputEventWeight() <<  std::endl;

  if (channel == _channel){
    passesMETCut = kTRUE;
  }
  else{
    if (_channel == 8 && (channel == 2 || channel == 5)) passesMETCut = kTRUE;
    if (_channel == 9 && (channel == 0 || channel == 1 || channel == 6)) passesMETCut = kTRUE;
    if (_channel == 10 && (channel == 3 || channel == 4 || channel == 7)) passesMETCut = kTRUE;
  }

  if (_channel == 11) passesMETCut = kTRUE; //Special flag to check everything
  
  if (passesMETCut){
    GetCutFlowTable()->PassCut(cutFlowName);
    _hChannelFlagAfter->Fill(channel);
  }
  else {
    GetCutFlowTable()->FailCut(cutFlowName);
  }

  
  

  return passesMETCut;

} //Apply












