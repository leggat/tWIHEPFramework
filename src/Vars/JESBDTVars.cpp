/******************************************************************************
 * testVar.cpp                                                                *
 *
 * Testing out some variables being added to the skim tree.
 *
 * History
 *      10th Nov 2016 - Created by D. Leggat. Just a test, so probably not that important
 ******************************************************************************/

#include "SingleTopRootAnalysis/Vars/JESBDTVars.hpp"

#include <iostream>

//Test out a couple of variables, one int and one float I guess
JESBDTVars::JESBDTVars(bool makeHists){

  SetName("JESBDTVars");

  _floatVars["M_channel"] = 5.;

  _floatVecVars["M_channel_JESShifts"];
  _floatVecVars["M_nJet2040_JESShifts"];
  _floatVecVars["M_DeltaRBJetLepton_JESShifts"];
  _floatVecVars["M_DeltaRlightjets_JESShifts"];
  _floatVecVars["M_topMass2_lep_JESShifts"];
  _floatVecVars["M_Pt_Lepton_JESShifts"];
  _floatVecVars["M_Pt_AllJetsLeptonMET_JESShifts"];
  _floatVecVars["M_DeltaRLeptonJet1_JESShifts"];
  _floatVecVars["M_Mass_Jet1Jet2Jet3LeptonMET_JESShifts"];
  _floatVecVars["M_hadronicWmass_JESShifts"];
  _floatVecVars["lightJet1CSV_JESShifts"];
  _floatVecVars["M_DeltaRWlvJet2_JESShifts"];
  _floatVecVars["M_Mass_Jet1Jet3_JESShifts"];
  _floatVecVars["M_E_Jet2Jet3_JESShifts"];
  _floatVecVars["M_Pt_AllJets2040_JESShifts"];

  _floatVecVars["M_nJet3040e24_JESShifts"];
  _floatVecVars["M_Mass_AllJets_JESShifts"];
  _floatVecVars["M_Pt_BJet1_4000_JESShifts"];
  _floatVecVars["M_DeltaRJet1Jet2_JESShifts"];
  _floatVecVars["M_Mass_Jet1Jet3_JESShifts"];
  _floatVecVars["M_HT_JESShifts"];
  _floatVecVars["M_DeltaRBJethadronicW_JESShifts"];
  _floatVecVars["M_Mass_AllJetsMET_JESShifts"];
  _floatVecVars["M_DeltaPhiJet1Jet2_JESShifts"];
  _floatVecVars["M_Pt_Jet1LeptonMET_JESShifts"];
  _floatVecVars["M_Pt_Jet1_JESShifts"];

  SetRunAdditionalVariables(true);


  if (makeHists){
    _floatVars["M_nJet2040_nom"] = 10.;
    _floatVars["M_DeltaRBJetLepton_nom"] = 6.;
    _floatVars["M_DeltaRlightjets_nom"] = 6.;
    _floatVars["M_topMass2_lep_nom"] = 500.;
    _floatVars["M_Pt_Lepton_nom"] = 200.;
    _floatVars["M_Pt_AllJetsLeptonMET_nom"] = 200.;
    _floatVars["M_DeltaRLeptonJet1_nom"] = 6.;
    _floatVars["M_Mass_Jet1Jet2Jet3LeptonMET_nom"] = 1000.;
    _floatVars["M_hadronicWmass_nom"] = 200.;
    _floatVars["lightJet1CSV_nom"] = 0.6;
    
    _floatVars["M_DeltaRWlvJet2_nom"] = 6.;
    _floatVars["M_Mass_Jet1Jet3_nom"] = 500.;
    _floatVars["M_E_Jet2Jet3_nom"] = 500.;
    _floatVars["M_Pt_AllJets2040_nom"] = 80.;
    
    _floatVars["M_nJet3040e24_nom"] = 100.;
    _floatVars["M_Mass_AllJets_nom"] = 600.;
    _floatVars["M_Pt_BJet1_4000_nom"] = 300.;
    _floatVars["M_DeltaRJet1Jet2_nom"] = 6.;
    _floatVars["M_Mass_Jet1Jet3_nom"] = 500.;
    _floatVars["M_HT_nom"] = 600.;
    _floatVars["M_DeltaRBJethadronicW_nom"] = 6.;
    _floatVars["M_Mass_AllJetsMET_nom"] = 600.;
    _floatVars["M_DeltaPhiJet1Jet2_nom"] = 6.;
    _floatVars["M_Pt_Jet1LeptonMET_nom"] = 400.;
    _floatVars["M_Pt_Jet1_nom"] = 100.;

    SetDoHists(makeHists);
    

  }

  SetRunAdditionalVariables(true);


  
}

bool JESBDTVars::AddAdditionalVariables(EventContainer * obj){
  
  for (int i = 0; i < obj->jets[0].GetNumberOfJESCorrections()+2; i++){ //The plus 2 is for the met uncertainty

    float defaultValue = -999.;

    _floatVecVars["M_channel_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_nJet2040_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_DeltaRBJetLepton_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_DeltaRlightjets_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_topMass2_lep_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Pt_Lepton_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Pt_AllJetsLeptonMET_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_DeltaRLeptonJet1_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Mass_Jet1Jet2Jet3LeptonMET_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_hadronicWmass_JESShifts"].push_back(defaultValue);
    _floatVecVars["lightJet1CSV_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_DeltaRWlvJet2_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Mass_Jet1Jet3_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_E_Jet2Jet3_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Pt_AllJets2040_JESShifts"].push_back(defaultValue);

    _floatVecVars["M_nJet3040e24_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Mass_AllJets_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Pt_BJet1_4000_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_DeltaRJet1Jet2_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Mass_Jet1Jet3_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_HT_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_DeltaRBJethadronicW_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Mass_AllJetsMET_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_DeltaPhiJet1Jet2_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Pt_Jet1LeptonMET_JESShifts"].push_back(defaultValue);
    _floatVecVars["M_Pt_Jet1_JESShifts"].push_back(defaultValue);
    

  }                                                                  


  return true;
}

void JESBDTVars::FillBranches(EventContainer * evtObj){
  
  int nCorrs = evtObj->jets[0].GetNumberOfJESCorrections();

  int nJets = evtObj->jets.size();
  int nbJets = evtObj->taggedJets.size();

  _floatVars["M_channel"] = -1;
  if (nJets == 2 && nbJets == 1) _floatVars["M_channel"] = 2.;
  else if (nJets == 3 && nbJets == 1) _floatVars["M_channel"] = 0.;
  else if (nJets == 4 && nbJets == 1) _floatVars["M_channel"] = 3.;
  else if (nJets == 3 && nbJets == 2) _floatVars["M_channel"] = 1.;
  else if (nJets == 4 && nbJets == 2) _floatVars["M_channel"] = 4.;

  //Try hard resetting all variables here and see if that changes outputs.
  for (auto jetvar: _floatVecVars){
    for (int i = 0; i < _floatVecVars[jetvar.first].size(); i++){
      _floatVecVars[jetvar.first][i] = -999.;
    }
  }

  //Reset each JES channel flag here
  for (int i = 0; i < nCorrs + 2; i++){
    _floatVecVars["M_channel_JESShifts"][i] = -1;
  }
  //Was doing this inside the loop before, but why?
  TLorentzVector Lepton(00,0,0,0);
  TLorentzVector Miss(00,0,0,0);
    
  Miss.SetPtEtaPhiE(evtObj->missingEt_xy,0,evtObj->missingPhi_xy,evtObj->missingEt_xy);

  if (evtObj->electronsToUsePtr->size() > 0){ // if this number is >0 we're in the electron channel. Otherwise use muons
    Lepton.SetPtEtaPhiE(evtObj->electronsToUsePtr->at(0).Pt(),evtObj->electronsToUsePtr->at(0).Eta(),evtObj->electronsToUsePtr->at(0).Phi(),evtObj->electronsToUsePtr->at(0).E());
  }
  else{
    Lepton.SetPtEtaPhiE(evtObj->muonsToUsePtr->at(0).Pt(),evtObj->muonsToUsePtr->at(0).Eta(),evtObj->muonsToUsePtr->at(0).Phi(),evtObj->muonsToUsePtr->at(0).E());
  }
  Jet2040.clear();
  Jet3040e24.clear();
  BJet4000.clear();
  UntaggedJet.clear();

  //  std::cout << _floatVars["M_channel"] << " " << evtObj->jets.size() << " channel nominal"  << std::endl;


  if ((evtObj->jets.size() > 1 && evtObj->taggedJets.size() > 0) && (evtObj->jets.size() != evtObj->taggedJets.size() )){
    
    for (auto jet : evtObj->alljets){
      if (jet.Pt() > 20 && jet.Pt() < 40) Jet2040.push_back(jet);
    }
    
    for (auto jet : evtObj->jets){
      if(!jet.IsTagged()) UntaggedJet.push_back(jet);         
      if (jet.IsTagged() && jet.Pt() > 40) BJet4000.push_back(jet);
      if (jet.Pt() > 30 && jet.Pt() < 40) Jet3040e24.push_back(jet);
    }                                          
    
    
    //Putting this is the loop before was dumb. Fixed now.
    Double_t metShiftUp = evtObj->GetMETShiftUp();
    Double_t metShiftDown = evtObj->GetMETShiftDown();
    TLorentzVector tempMet = Miss;
    tempMet.SetPx(Miss.Px()*metShiftUp);
    tempMet.SetPy(Miss.Py()*metShiftUp);
    fillVectorBranches(Lepton,tempMet,evtObj->taggedJets,evtObj->jets,Jet2040,UntaggedJet,nCorrs);
    tempMet.SetPx(Miss.Px()*metShiftDown);
    tempMet.SetPy(Miss.Py()*metShiftDown);
    fillVectorBranches(Lepton,tempMet,evtObj->taggedJets,evtObj->jets,Jet2040,UntaggedJet,nCorrs+1);
    
  }


  //Evaluate each distribution once per JES shift
  for (int i = 0; i < nCorrs; i++){
    //First let's clear the things we already have
    
    selectedJet.clear();
    Jet2040.clear();
    BJet.clear();
    UntaggedJet.clear();

    //And make a dummy variable here
    Jet tempJet;
    TLorentzVector tempjet(0,0,0,0);

    //Number of loose jets
    for (auto jet : evtObj->alljets){
      tempJet = jet;
      TLorentzVector tempmet(0,0,0,0);
      tempJet.ShiftPtWithJESCorr(i,&tempmet);
      if (tempJet.Pt() > 20 && tempJet.Pt() < 40) Jet2040.push_back(tempJet);
    }
    
    _floatVecVars["M_nJet2040_JESShifts"][i] = (float)Jet2040.size();

    for (auto jet : evtObj->jesShiftedJets[i]){
      selectedJet.push_back(jet);
      if (jet.IsTagged()) BJet.push_back(jet);
      else UntaggedJet.push_back(jet);
    }
    if (evtObj->jesShiftedJets[i].size() < 2 || BJet.size() < 1) continue;
    if (evtObj->jesShiftedJets[i].size() == BJet.size()) continue;

    //Now set up the lepton and met variables
    Miss = evtObj->metVecsJESShifted[i];

  
    fillVectorBranches(Lepton,Miss,BJet,selectedJet,Jet2040,UntaggedJet,i);
    if (i == 0){ //If we're doing the nominal case, we'll also fill the vector with distributions for shifter 
      
    }

  }

  /* This bit is to check what's going on when the channel flag vary wildly between the nominal and the jes variation - leaving it in so it isn't a bitch to solve if I have to worry about this again.
  for (int i = 0; i < _floatVecVars["M_channel_JESShifts"].size()-2; i++){
    if (_floatVecVars["M_channel_JESShifts"][i] == _floatVars["M_channel"]){
      std::cout <<"yep it's the same";}
    if (_floatVecVars["M_channel_JESShifts"][i] != _floatVars["M_channel"]){
      int bTaggedJets = 0;
      for (auto jet : evtObj->jesShiftedJets[i]){
	if (jet.IsTagged()) bTaggedJets++;
      }                                          

      std::cout << "nominal: " << _floatVars["M_channel"] << " " << i << " "  << _floatVecVars["M_channel_JESShifts"][i] << " " << evtObj->jets.size() << " " << evtObj->taggedJets.size() << " " << evtObj->jesShiftedJets[i].size() <<  " " << bTaggedJets << std::endl;
    }
    }*/
  
  if(DoHists()){  
    _floatVars["M_nJet2040_nom"] =   _floatVecVars["M_nJet2040_JESShifts"][0];
    _floatVars["M_DeltaRBJetLepton_nom"] =   _floatVecVars["M_DeltaRBJetLepton_JESShifts"][0];
    _floatVars["M_DeltaRlightjets_nom"] =   _floatVecVars["M_DeltaRlightjets_JESShifts"][0];
    _floatVars["M_topMass2_lep_nom"] =   _floatVecVars["M_topMass2_lep_JESShifts"][0];
    _floatVars["M_Pt_Lepton_nom"] =   _floatVecVars["M_Pt_Lepton_JESShifts"][0];
    _floatVars["M_Pt_AllJetsLeptonMET_nom"] =   _floatVecVars["M_Pt_AllJetsLeptonMET_JESShifts"][0];
    _floatVars["M_DeltaRLeptonJet1_nom"] =   _floatVecVars["M_DeltaRLeptonJet1_JESShifts"][0];
    _floatVars["M_Mass_Jet1Jet2Jet3LeptonMET_nom"] =   _floatVecVars["M_Mass_Jet1Jet2Jet3LeptonMET_JESShifts"][0];
    _floatVars["M_hadronicWmass_nom"] =   _floatVecVars["M_hadronicWmass_JESShifts"][0];
    _floatVars["lightJet1CSV_nom"] =   _floatVecVars["lightJet1CSV_JESShifts"][0];

    _floatVars["M_DeltaRWlvJet2_nom"] =     _floatVecVars["M_DeltaRWlvJet2_JESShifts"][0];
    _floatVars["M_Mass_Jet1Jet3_nom"] =     _floatVecVars["M_Mass_Jet1Jet3_JESShifts"][0];
    _floatVars["M_E_Jet2Jet3_nom"] =     _floatVecVars["M_E_Jet2Jet3_JESShifts"][0];
    _floatVars["M_Pt_AllJets2040_nom"] =     _floatVecVars["M_Pt_AllJets2040_JESShifts"][0];
    _floatVars["M_Pt_Jet1_nom"] = _floatVecVars["M_Pt_Jet1_JESShifts"][0];
    
    FillHistograms(evtObj->GetEventWeight());
  }
}

void JESBDTVars::fillVectorBranches(TLorentzVector Lepton, TLorentzVector Miss, std::vector<Jet> BJet,   std::vector<Jet> selectedJet, std::vector<Jet> Jet2040, std::vector<Jet> UntaggedJet, int i) {

  //  std::cout << i << " "  << Miss.Pt() << " " << Miss.Px() << " " << Miss.Py() << std::endl;


  _floatVecVars["M_channel_JESShifts"][i] = -1;                                    
                                       
  int nJets = selectedJet.size();
  int nbJets = BJet.size();
                          
  if (nJets == 2 && nbJets == 1) _floatVecVars["M_channel_JESShifts"][i] = 2.;     
  else if (nJets == 3 && nbJets == 1) _floatVecVars["M_channel_JESShifts"][i] = 0.;
  else if (nJets == 4 && nbJets == 1) _floatVecVars["M_channel_JESShifts"][i] = 3.;
  else if (nJets == 3 && nbJets == 2) _floatVecVars["M_channel_JESShifts"][i] = 1.;
  else if (nJets == 4 && nbJets == 2) _floatVecVars["M_channel_JESShifts"][i] = 4.;
  else if (nJets == 2 && nbJets == 0) _floatVecVars["M_channel_JESShifts"][i] = 5.;
  else if (nJets == 3 && nbJets == 0) _floatVecVars["M_channel_JESShifts"][i] = 6.;
  else if (nJets == 4 && nbJets == 0) _floatVecVars["M_channel_JESShifts"][i] = 7.;
  
  if (_floatVecVars["M_channel_JESShifts"][i] == -1) return;

  /*  if (_floatVars["M_channel"] != _floatVecVars["M_channel_JESShifts"][i]){
    std::cout << "nominal: " << _floatVars["M_channel"] << " " << i << " " << _floatVecVars["M_channel_JESShifts"][i] << std::endl;
    for (auto je : selectedJet){
      //      std::cout << "pt: " << je.Pt() << " eta: " << je.Eta() << std::endl;
    }
    }*/

  _floatVecVars["M_nJet2040_JESShifts"][i] = (float)Jet2040.size();

  TLorentzVector Wlv(0,0,0,0);
  Wlv = Lepton+Miss;



  TLorentzVector W(0,0,0,0), Top(0,0,0,0), sumJets2040(0,0,0,0);
    
  int maxInd = 2;
  if (UntaggedJet.size() < maxInd) maxInd = UntaggedJet.size();
  for (int jetIn = 0; jetIn < maxInd; jetIn++){
    W = W + UntaggedJet[jetIn];
  }
  /*    for (auto jet : UntaggedJet){
	W = W + jet;
	}*/

  TLorentzVector totalJets(0,0,0,0);
  for (auto jet : selectedJet){
    totalJets += jet;
  }

  for (auto jet : Jet2040) {sumJets2040 += jet;}
  if (Jet2040.size() >= 1) _floatVecVars["M_Pt_AllJets2040_JESShifts"][i] = sumJets2040.Pt();

  int highestCSVTaggedJet = 0;
  for (unsigned int bJetInd = 1; bJetInd < BJet.size(); bJetInd++){
    if (BJet.at(bJetInd).GetbDiscriminator() > BJet.at(highestCSVTaggedJet).GetbDiscriminator()) highestCSVTaggedJet = bJetInd;
  }

  //That should be all of the things we need to make the BDT variables, so let's make the variables now.
  _floatVecVars["M_DeltaRBJetLepton_JESShifts"][i] = fabs(BJet[highestCSVTaggedJet].DeltaR(Lepton));
  _floatVecVars["M_DeltaRlightjets_JESShifts"][i] = 0.;
  if (UntaggedJet.size() > 1) _floatVecVars["M_DeltaRlightjets_JESShifts"][i] = UntaggedJet.at(0).DeltaR(UntaggedJet.at(1));
  if (BJet[highestCSVTaggedJet].DeltaR(Lepton) > BJet[highestCSVTaggedJet].DeltaR(W)){
    _floatVecVars["M_topMass2_lep_JESShifts"][i] = -1;
  }
  else{
    Top = W + BJet[highestCSVTaggedJet];
    _floatVecVars["M_topMass2_lep_JESShifts"][i] = Top.M();
  }
  _floatVecVars["M_Pt_Lepton_JESShifts"][i] = Lepton.Pt();
  
  _floatVecVars["M_Pt_Jet1_JESShifts"][i] = selectedJet[0].Pt();

  _floatVecVars["M_Pt_AllJetsLeptonMET_JESShifts"][i] = (Lepton + Miss + totalJets).Pt();

  _floatVecVars["M_DeltaRLeptonJet1_JESShifts"][i] = fabs(selectedJet.at(0).DeltaR(Lepton));

  if (selectedJet.size() > 1) _floatVecVars["M_DeltaRWlvJet2_JESShifts"][i] = fabs(selectedJet.at(1).DeltaR(Wlv)); 
 
 
  //  _floatVecVars["M_Mass_Jet1Jet2Jet3LeptonMET_JESShifts"][i] = 0.;
  
  //  _floatVecVars["M_Mass_Jet1Jet3_JESShifts"][i] =  0.;

  if (selectedJet.size() > 2){
    TLorentzVector jets123(0,0,0,0);
    for (int jetInd = 0; jetInd < 3; jetInd++){
      jets123 += selectedJet[jetInd];
    }
    _floatVecVars["M_Mass_Jet1Jet2Jet3LeptonMET_JESShifts"][i] = (jets123 + Lepton + Miss).M();
    _floatVecVars["M_Mass_Jet1Jet3_JESShifts"][i] = (selectedJet.at(0)+selectedJet.at(2)).M(); 
    _floatVecVars["M_E_Jet2Jet3_JESShifts"][i] = (selectedJet.at(1)+selectedJet.at(2)).E();        
    _floatVecVars["M_Mass_Jet1Jet3_JESShifts"][i] = (selectedJet.at(0)+selectedJet.at(2)).M();        
    _floatVecVars["M_HT_JESShifts"][i] = (selectedJet.at(0).Pt() + selectedJet.at(1).Pt() + selectedJet.at(2).Pt() );


  }
  _floatVecVars["M_hadronicWmass_JESShifts"][i] = W.M();

  _floatVecVars["lightJet1CSV_JESShifts"][i] = UntaggedJet[0].GetbDiscriminator();

  _floatVecVars["M_nJet3040e24_JESShifts"][i] = Jet3040e24.size();
  _floatVecVars["M_Mass_AllJets_JESShifts"][i] = (totalJets).M();
  if (BJet4000.size() > 0) _floatVecVars["M_Pt_BJet1_4000_JESShifts"][i] = (BJet4000.at(0).Pt());
  if (selectedJet.size() > 1){
    _floatVecVars["M_DeltaRJet1Jet2_JESShifts"][i] = fabs(selectedJet.at(0).DeltaR(selectedJet.at(1)));
_floatVecVars["M_DeltaPhiJet1Jet2_JESShifts"][i] = fabs(selectedJet.at(0).DeltaPhi(selectedJet.at(1)));
  }
  if (BJet.size() > 0) _floatVecVars["M_DeltaRBJethadronicW_JESShifts"][i] = fabs(BJet[highestCSVTaggedJet].DeltaR(W));
  _floatVecVars["M_Mass_AllJetsMET_JESShifts"][i] = (totalJets+Miss).Pt();
  _floatVecVars["M_Pt_Jet1LeptonMET_JESShifts"][i] = (Miss+Lepton+selectedJet.at(0)).Pt();

}

