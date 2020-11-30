/******************************************************************************
 * testVar.hpp                                                                *
 *                                                                            *
 * Puts some testing variables into the skim tree
 * 
 * History
 *      10 Nov 2016 - Created by D. Leggat
 ******************************************************************************/

#ifndef Bootstrap_h
#define Bootstrap_h

#include "SingleTopRootAnalysis/Base/Dictionary/VarBase.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp" 

#include "TRandom.h"

class Bootstrap: public VarBase {
  
public:
  Bootstrap();
  ~Bootstrap();
  
  //void BookBranches(TTree * skimTree);
  void FillBranches(EventContainer * evtObj);

private:
  TRandom * random;
  
};

#endif
