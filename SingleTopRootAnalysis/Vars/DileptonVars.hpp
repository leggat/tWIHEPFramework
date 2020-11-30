/******************************************************************************
 * DileptonVars.hpp                                                                *
 *                                                                            *
 * Add the variables we will use in the BDT to the skim tree                  *
 * 
 * History
 *      25 Nov 2016 - Created by D. Leggat
 ******************************************************************************/

#ifndef DileptonVars_h
#define DileptonVars_h

#include "SingleTopRootAnalysis/Base/Dictionary/VarBase.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp" 
#include "TLorentzVector.h"

class DileptonVars: public VarBase {
  
public:
  DileptonVars(bool makeHistos = false);
  
  //void BookBranches(TTree * skimTree);
  void FillBranches(EventContainer * evtObj);
  
private:
  std::vector<TLorentzVector> Jet;
  std::vector<TLorentzVector> Jet2030;
  std::vector<TLorentzVector> Jet2040;
  std::vector<TLorentzVector> Jet3040;
  std::vector<TLorentzVector> Jet4000;

  std::vector<TLorentzVector> BJet;
  std::vector<TLorentzVector> BJet2030;
  std::vector<TLorentzVector> BJet2040;
  std::vector<TLorentzVector> BJet3040;
  std::vector<TLorentzVector> BJet4000;

  std::vector<TLorentzVector>     Jete24;
  std::vector<TLorentzVector> Jet2030e24;
  std::vector<TLorentzVector> Jet2040e24;
  std::vector<TLorentzVector> Jet3040e24;
  std::vector<TLorentzVector> Jet4000e24;
  std::vector<TLorentzVector> Jet3500e24;

  TLorentzVector totalJets;
  TLorentzVector sumJets2030;
  TLorentzVector sumJets2040;
  TLorentzVector sumJets3040;
  TLorentzVector sumJets4000;

};

#endif
