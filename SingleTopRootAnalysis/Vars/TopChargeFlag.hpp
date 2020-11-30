/******************************************************************************
 * TopChargeFlag.hpp                                                                *
 *                                                                            *
 * Add various weight variables to the skim tree
 * 
 * History
 *      10 Nov 2016 - Created by D. Leggat
 ******************************************************************************/

#ifndef TopChargeFlag_h
#define TopChargeFlag_h

#include "SingleTopRootAnalysis/Base/Dictionary/VarBase.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp" 
#include "SingleTopRootAnalysis/Trees/EventTree.hpp" 

class TopChargeFlag: public VarBase {
  
public:
  
  TopChargeFlag();

  //Add in additional channel variables for each JES systematic
  bool AddAdditionalVariables(EventContainer * obj);
  
  //void BookBranches(TTree * skimTree);
  void FillBranches(EventContainer * evtObj);

private:

  Int_t numberOfEvents;
  Int_t numberOfAllEvents;

  Int_t eventsThatDeltaRWasRight;
  Int_t eventsThatMassWasRight;
  
};

#endif
