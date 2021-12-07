/******************************************************************************
 * CutJsonFilter.cpp                                                       *
 *                                                                            *
 * Cuts on the requirement of a good primary vertex                           *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutJsonFilter class                              *
 *    CutJsonFilter()                     -- Parameterized Constructor         *
 *    ~CutJsonFilter()                    -- Destructor                        *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutJsonFilter"                *
 *                                                                            *
 * Private Data Members of CutJsonFilter class                                 *
 *    myTH1F* _hMissingEtBefore;    -- Hist of PV before cut        *
 *    myTH1F* _hMissingEtAfter;     -- Hist of PV after cut         *
 *                                                                            *
 * History                                                                    *
 *      5th July 2016 - Created by Duncan Leggat                              *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Cuts/Other/CutJsonFilter.hpp"
#include <iostream>

using namespace std;

/******************************************************************************
 * CutJsonFilter::CutJsonFilter(EventContainer *EventContainerObj, TString electronType)    *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutJsonFilter::CutJsonFilter(EventContainer *EventContainerObj)
{
  // Set Event Container
  SetEventContainer(EventContainerObj);
  TEnv *config = EventContainerObj -> GetConfig();
  TString jsonInFile = config -> GetValue("Include.jsonFile","");
  if (jsonInFile){
    std::cout << "Reading in a json filter " << jsonInFile << std::endl;
    std::ifstream cfgfile(jsonInFile);
    string readLine;
    getline(cfgfile,readLine);
    TString processString = readLine;
    TString segment;
    Ssiz_t from = 0;
    //    std::cout << processString << std::endl;
    while(processString.Tokenize(segment,from,", \"")){
      //      std::cout << "Processed..." << std::endl;
      TObjArray *mapItem = segment.Tokenize("\"");
      Int_t key = (((TObjString *)(mapItem->At(0)))->String()).Atoi();
      TString value = ((TObjString *)(mapItem->At(1)))->String();
      TObjArray *values = value.Tokenize("[], :}");
      std::vector<Int_t> valueVec;
      for (Int_t i = 0; i < values->GetEntries(); i++){
	valueVec.push_back( (((TObjString *)(values->At(i)))->String()).Atoi());
      }
      _goodLumis[key] = valueVec;
    }
    /*TObjArray *mapSplit = processString
    for (Int_t i = 0; i < mapSplit->GetEntries(); i++){
      
      std::cout << ((TObjString *)(mapSplit->At(i)))->String() << std::endl;
      }*/
  }
    
} // CutJsonFilter


/******************************************************************************
 * CutJsonFilter::~CutJsonFilter()                                              *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutJsonFilter::~CutJsonFilter()
{
  
}//~CutJsonFilter

/******************************************************************************
 * void CutJsonFilter::BookHistogram()                                         *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutJsonFilter::BookHistogram(){
  
  // ***********************************************
  // Book Histograms
  // ***********************************************  

  //No histograms


  // ***********************************************
  // Add these cuts to the cut flow table
  // ***********************************************

  GetCutFlowTable()->AddCutToFlow("Json.Filter","Pass JSON filter");

}//BookHistograms()

/******************************************************************************
 * Bool_t CutJsonFilter::Apply()                                                *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Cut on the selected trigger                                                *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutJsonFilter::Apply(){

  EventContainer *EventContainerObj = GetEventContainer();

  Bool_t passesJsonFilter = kTRUE;

  if (EventContainerObj->GetIsSimulation()) {
    GetCutFlowTable()->PassCut("Json.Filter");
    return passesJsonFilter; //Only do this for data
  }

  Int_t runNumber = EventContainerObj->runNumber;
  Int_t lumi = EventContainerObj->lumiBlock;

  passesJsonFilter = kFALSE;

  if (!(_goodLumis.find(runNumber) == _goodLumis.end())){
    for ( Int_t i ; i < _goodLumis[runNumber].size()/2.; i++){
      if (lumi > _goodLumis[runNumber][i*2] && lumi < _goodLumis[runNumber][i*2+1]){
	passesJsonFilter = kTRUE;
	break;
      }
    }
  }

  if (passesJsonFilter){
    GetCutFlowTable()->PassCut("Json.Filter");
  }
  else {
    GetCutFlowTable()->FailCut("Json.Filter");
  }
  
  return passesJsonFilter;

} //Apply












