/******************************************************************************
 * JESBDTVars.hpp                                                                *
 *                                                                            *
 * Puts some testing variables into the skim tree
 * 
 * History
 *      03 May 2018 - Created by D. Leggat
 ******************************************************************************/

#ifndef JESBDTVars_h
#define JESBDTVars_h

#include "SingleTopRootAnalysis/Base/Dictionary/VarBase.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp" 
#include "SingleTopRootAnalysis/Particles/Recon/Jet.hpp"

class JESBDTVars: public VarBase {
  
public:
  JESBDTVars(bool makeHistos = false);

  bool AddAdditionalVariables(EventContainer * obj);
  
  //void BookBranches(TTree * skimTree);
  void FillBranches(EventContainer * evtObj);

private:
    std::vector<Jet> selectedJet;
  std::vector<TLorentzVector> Jet2030;
  std::vector<Jet> Jet2040;
  std::vector<TLorentzVector> Jet3040;
  std::vector<TLorentzVector> Jet4000;

  std::vector<Jet> BJet;
  std::vector<TLorentzVector> BJet2030;
  std::vector<TLorentzVector> BJet2040;
  std::vector<TLorentzVector> BJet3040;
  std::vector<TLorentzVector> BJet4000;

  std::vector<Jet> UntaggedJet;

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

  void fillVectorBranches(TLorentzVector Lepton, TLorentzVector Miss, std::vector<Jet> BJet,   std::vector<Jet> selectedJet, std::vector<Jet> Jet2040, std::vector<Jet> UntaggedJet, int i); 
  
};

#endif
