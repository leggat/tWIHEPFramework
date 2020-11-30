/******************************************************************************
 * testVar.cpp                                                                *
 *
 * Testing out some variables being added to the skim tree.
 *
 * History
 *      10th Nov 2016 - Created by D. Leggat. Just a test, so probably not that important
 ******************************************************************************/

#include "SingleTopRootAnalysis/Vars/TopChargeFlag.hpp"

#include <iostream>

//Test out a couple of variables, one int and one float I guess
TopChargeFlag::TopChargeFlag(){

  _intVars["leptonicTop"] = 0;
  _intVars["hadronicTop"] = 0;
  _intVars["topCharge"] = 0;

  _intVars["leptonicTop_deltaR"] = 0;
  _intVars["hadronicTop_deltaR"] = 0;
  _intVars["topCharge_deltaR"] = 0;

  numberOfEvents = 0;
  numberOfAllEvents = 0;
  eventsThatDeltaRWasRight = 0;
  eventsThatMassWasRight = 0;
  
  
}

bool TopChargeFlag::AddAdditionalVariables(EventContainer * obj){
  
  if (obj->jets.size() < 1) {
    std::cout << "This is a really obscure corner case that I really hope doesn't happen, because then I'll have to deal with this properly.." << std::endl;
    return false;
  }

  _intVecVars["channel_JESShifts"];

  for (int i = 0; i < obj->jets[0].GetNumberOfJESCorrections(); i++){
    _intVecVars["channel_JESShifts"].push_back(0);
  }

  return true;
}

void TopChargeFlag::FillBranches(EventContainer * evtObj){

  //Get the event tree and find the top quark
  EventTree * eventTree = evtObj->GetEventTree();
  Int_t numberOfTops = 0;
  Int_t leadingTopIndex;
  float leadingPt = 0;
  for (int i = 0; i < eventTree->Gen_pdg_id->size();i++){
    if (abs(eventTree->Gen_pdg_id->at(i)) == 6){
      numberOfTops++;
      if (eventTree->Gen_pt->at(i) > leadingPt){
	leadingPt = eventTree->Gen_pt->at(i);
	leadingTopIndex = i;
      }
    }
  }

  TLorentzVector genTop(0,0,0,0);
  Int_t topCharge = eventTree->Gen_charge->at(leadingTopIndex);
  genTop.SetPtEtaPhiE(eventTree->Gen_pt->at(leadingTopIndex),eventTree->Gen_eta->at(leadingTopIndex),eventTree->Gen_phi->at(leadingTopIndex), eventTree->Gen_energy->at(leadingTopIndex));
  
  //  std::cout << numberOfTops << " " << leadingPt << " " << leadingTopIndex << " " << topCharge << std::endl;

  //Get the number of jets and tagged jets to determine value of channel.
  int nJets = evtObj->jets.size();
  int nbJets = evtObj->taggedJets.size();

  //Let's assume that we want to only do this in the signal region for now.
  if (! (nJets == 3 && nbJets == 1)){
    _intVars["leptonicTop"] = 0;
    _intVars["hadronicTop"] = 0;
    _floatVars["topCharge"] = 0;
    _intVars["leptonicTop_deltaR"] = 0;
    _intVars["hadronicTop_deltaR"] = 0;
    _floatVars["topCharge_deltaR"] = 0;
    return;
  }

  auto jetVec = evtObj->jets;
  
  TLorentzVector leptonicW(0,0,0,0);
  TLorentzVector hadronicW(0,0,0,0);
  TLorentzVector Miss(0,0,0,0);
  TLorentzVector Lepton(0,0,0,0);
  TLorentzVector bJet(0,0,0,0);

  Double_t leptonCharge = 0;

  Miss.SetPtEtaPhiE(evtObj->missingEt_xy,0,evtObj->missingPhi_xy,evtObj->missingEt_xy);
  if (evtObj->electronsToUsePtr->size() > 0){ // if this number is >0 we're in the electron channel. Otherwise use muons
    Lepton.SetPtEtaPhiE(evtObj->electronsToUsePtr->at(0).Pt(),evtObj->electronsToUsePtr->at(0).Eta(),evtObj->electronsToUsePtr->at(0).Phi(),evtObj->electronsToUsePtr->at(0).E());
    leptonCharge = evtObj->electronsToUsePtr->at(0).charge();
  }
  else{
    Lepton.SetPtEtaPhiE(evtObj->muonsToUsePtr->at(0).Pt(),evtObj->muonsToUsePtr->at(0).Eta(),evtObj->muonsToUsePtr->at(0).Phi(),evtObj->muonsToUsePtr->at(0).E());
    leptonCharge = evtObj->muonsToUsePtr->at(0).charge();
  }
  leptonicW = Lepton+Miss;

  for (auto jet : jetVec){
    if (jet.IsTagged()) bJet = jet;
    else hadronicW += jet;
  }

  //First we'll do the mass based check
  float leptonicTopMass = (leptonicW + bJet).M();
  float hadronicTopMass = (hadronicW + bJet).M();

  float topMassNom = 172.5;
 
  if (fabs(leptonicTopMass - topMassNom) < fabs(hadronicTopMass - topMassNom)) { //Leptonic top
    _intVars["leptonicTop"] = 1;
    _floatVars["topCharge"] = leptonCharge;
  }
  else { //hadronic lepton
    _intVars["hadronicTop"] = 1;
    _floatVars["topCharge"] = -leptonCharge;
  }

  //Now we'll do the delta R check
  if (bJet.DeltaR(Lepton) > bJet.DeltaR(hadronicW)){ //hadronic top
    _intVars["hadronicTop_deltaR"] = 1;
    _floatVars["topCharge_deltaR"] = -leptonCharge;
  }
  else {
    _intVars["leptonicTop_deltaR"] = 1;
    _floatVars["topCharge_deltaR"] = leptonCharge;
  }
  
  //Some debugging couts here
  std::cout << "recoMass selection: l:"  << _intVars["leptonicTop"] << " h: " << _intVars["hadronicTop"] << " charge " << _floatVars["topCharge"] << std::endl;
  std::cout << "deltaR selection:   l:"  << _intVars["leptonicTop_deltaR"] << " h: " << _intVars["hadronicTop_deltaR"] << " charge " << _floatVars["topCharge_deltaR"];
  numberOfAllEvents++;
  if (_floatVars["topCharge_deltaR"] != _floatVars["topCharge"]) {
    numberOfEvents++;
    std::cout << " these don't match! This has happened " << numberOfEvents << " times out of " << numberOfAllEvents;
    
  }

  if (genTop.DeltaR(hadronicW+bJet) < genTop.DeltaR(leptonicW+bJet)){ //Then we should assume it's actually a hadronicW
    if (_intVars["hadronicTop_deltaR"] == 1) eventsThatDeltaRWasRight++;
    if (_intVars["hadronicTop"] == 1) eventsThatMassWasRight++;
  }
  else {
    if (_intVars["leptonicTop_deltaR"] == 1) eventsThatDeltaRWasRight++;
    if (_intVars["leptonicTop"] == 1) eventsThatMassWasRight++;         
  }
  std::cout << "\nNumber of events DeltaR was right: " << eventsThatDeltaRWasRight << " Number of events mass was right: " << eventsThatMassWasRight << " (out of " << numberOfAllEvents << " events)" << std::endl;
  std::cout << std::endl;

}
