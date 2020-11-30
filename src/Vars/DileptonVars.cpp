/******************************************************************************
 * testVar.cpp                                                                *
 *
 * Testing out some variables being added to the skim tree.
 *
 * History
 *      10th Nov 2016 - Created by D. Leggat. Just a test, so probably not that important
 ******************************************************************************/

#include "SingleTopRootAnalysis/Vars/DileptonVars.hpp"

#include <iostream>

//Test out a couple of variables, one int and one float I guess
DileptonVars::DileptonVars(bool makeHistos){

  SetName("DileptonVars");
  
  _floatVars["tWVars_nJet2030"] = 10;
  _floatVars["tWVars_nJet3040"] = 10;
  _floatVars["tWVars_nJet4000"] = 10;
  _floatVars["tWVars_nJet2040"] = 10;
  _floatVars["tWVars_nBJet2030"] = 10;
  _floatVars["tWVars_nBJet3040"] = 10;
  _floatVars["tWVars_nBJet4000"] = 10;
  _floatVars["tWVars_nBJet2040"] = 10;
  _floatVars["tWVars_nJet2030e24"] = 10;
  _floatVars["tWVars_nJet3040e24"] = 10;
  _floatVars["tWVars_nJet4000e24"] = 10;
  _floatVars["tWVars_nJet2040e24"] = 10;
  _floatVars["tWVars_nJet3500e24"] = 10;

  _floatVars["tWVars_Mass_AllJets"] = 1500.;
  _floatVars["tWVars_E_AllJets"] = 500.;
  _floatVars["tWVars_Pt_AllJets"] = 500.;
  _floatVars["tWVars_Eta_AllJets"] = -6.;
  _floatVars["tWVars_Phi_AllJets"] = -3.2;

  _floatVars["tWVars_Mass_AllJets2030"] = 2000;
  _floatVars["tWVars_E_AllJets2030"] = 100;
  _floatVars["tWVars_Pt_AllJets2030"] = 100;
  _floatVars["tWVars_Eta_AllJets2030"] = -6.;
  _floatVars["tWVars_Phi_AllJets2030"] = -3.2;
  _floatVars["tWVars_Mass_AllJets2040"] = 3000.;
  _floatVars["tWVars_E_AllJets2040"] = 100.;
  _floatVars["tWVars_Pt_AllJets2040"] = 100.;
  _floatVars["tWVars_Eta_AllJets2040"] = -6.;
  _floatVars["tWVars_Phi_AllJets2040"] = -3.2;
  _floatVars["tWVars_Mass_AllJets3040"] = 2000.;
  _floatVars["tWVars_E_AllJets3040"] = 100.;
  _floatVars["tWVars_Pt_AllJets3040"] = 100.;
  _floatVars["tWVars_Eta_AllJets3040"] = -6.;
  _floatVars["tWVars_Phi_AllJets3040"] = -3.2;
  _floatVars["tWVars_Mass_AllJets4000"] = 5000.;
  _floatVars["tWVars_E_AllJets4000"] = 300.;
  _floatVars["tWVars_Pt_AllJets4000"] = 300.;
  _floatVars["tWVars_Eta_AllJets4000"] = -6.;
  _floatVars["tWVars_Phi_AllJets4000"] = -3.2;

  _floatVars["tWVars_Mass_LeadJet"] = 100.;
  _floatVars["tWVars_E_LeadJet"] = 300.;
  _floatVars["tWVars_Pt_LeadJet"] = 300.;
  _floatVars["tWVars_Eta_LeadJet"] = -6.;
  _floatVars["tWVars_Phi_LeadJet"] = -3.2;

  _floatVars["tWVars_Pt_Jet_2030"] = 30.;
  _floatVars["tWVars_Eta_Jet_2030"] = -6.;
  _floatVars["tWVars_Phi_Jet_2030"] = -3.2;
  _floatVars["tWVars_Pt_Jet_2040"] = 40.;
  _floatVars["tWVars_Eta_Jet_2040"] = -6.;
  _floatVars["tWVars_Phi_Jet_2040"] = -3.2;
  _floatVars["tWVars_Pt_Jet_3040"] = 40.;
  _floatVars["tWVars_Eta_Jet_3040"] = -6.;
  _floatVars["tWVars_Phi_Jet_3040"] = -3.2;
  _floatVars["tWVars_Pt_Jet_4000"] = 300;
  _floatVars["tWVars_Eta_Jet_4000"] = -6.;
  _floatVars["tWVars_Phi_Jet_4000"] = -3.2;
  
  _floatVars["tWVars_Pt_BJet_2030"] = 30.;
  _floatVars["tWVars_Eta_BJet_2030"] = -6.;
  _floatVars["tWVars_Phi_BJet_2030"] = -3.2;
  _floatVars["tWVars_Pt_BJet_2040"] = 40.;
  _floatVars["tWVars_Eta_BJet_2040"] = -6.;
  _floatVars["tWVars_Phi_BJet_2040"] = -3.2;
  _floatVars["tWVars_Pt_BJet_3040"] = 40.;
  _floatVars["tWVars_Eta_BJet_3040"] = -6.;
  _floatVars["tWVars_Phi_BJet_3040"] = -3.2;
  _floatVars["tWVars_Pt_BJet_4000"] = 300.;
  _floatVars["tWVars_Eta_BJet_4000"] = -6.;
  _floatVars["tWVars_Phi_BJet_4000"] = -3.2;
  
  _floatVars["tWVars_Pt_Jet_2030e24"] = 30;
  _floatVars["tWVars_Eta_Jet_2030e24"] = -6.;
  _floatVars["tWVars_Phi_Jet_2030e24"] = -3.2;
  _floatVars["tWVars_Pt_Jet_2040e24"] = 40;
  _floatVars["tWVars_Eta_Jet_2040e24"] = -6.;
  _floatVars["tWVars_Phi_Jet_2040e24"] = -3.2;
  _floatVars["tWVars_Pt_Jet_3040e24"] = 40;
  _floatVars["tWVars_Eta_Jet_3040e24"] = -6.;
  _floatVars["tWVars_Phi_Jet_3040e24"] = -3.2;
  _floatVars["tWVars_Pt_Jet_4000e24"] = 300;
  _floatVars["tWVars_Eta_Jet_4000e24"] = -6.;
  _floatVars["tWVars_Phi_Jet_4000e24"] = -3.2;
  
  _floatVars["tWVars_Mass_Muon"] = 100.;
  _floatVars["tWVars_Mass_Electron"] = 100.;
  _floatVars["tWVars_Mass_Leptons"] = 100.;
  _floatVars["tWVars_E_Muon"] = 200.;
  _floatVars["tWVars_E_Electron"] = 200.;
  _floatVars["tWVars_E_Leptons"] = 200.;
  _floatVars["tWVars_Pt_Muon"] = 300.;
  _floatVars["tWVars_Pt_Electron"] = 300.;
  _floatVars["tWVars_Pt_Leptons"] = 300.;
  _floatVars["tWVars_Eta_Muon"] = -6.;
  _floatVars["tWVars_Eta_Electron"] = -6.;
  _floatVars["tWVars_Eta_Leptons"] = -6.;
  _floatVars["tWVars_Phi_Muon"] = -3.2;
  _floatVars["tWVars_Phi_Electron"] = -3.2;
  _floatVars["tWVars_Phi_Leptons"] = -3.2;
  _floatVars["tWVars_Pt_sys"] = 300;
  _floatVars["tWVars_Pt_sys_sig"] = 70;
  _floatVars["tWVars_Pt_sys_MuonJet"] = 250;
  _floatVars["tWVars_Pt_sys_ElectronJet"] = 250;
  _floatVars["tWVars_Pt_sys_LeptonsJet"] = 250;
  _floatVars["tWVars_eta_sys_MuonJet"] = -6.;
  _floatVars["tWVars_eta_sys_ElectronJet"] = -6.;
  _floatVars["tWVars_eta_sys_LeptonsJet"] = -6.;
  
  _floatVars["tWVars_Mass_AllJetsMuon"] = 1200;
  _floatVars["tWVars_Mass_AllJetsElectron"] = 1200;
  _floatVars["tWVars_Mass_AllJetsLeptons"] = 1200;
  _floatVars["tWVars_E_AllJetsMuon"] = 300;
  _floatVars["tWVars_E_AllJetsElectron"] = 300;
  _floatVars["tWVars_E_AllJetsLeptons"] = 300;
  _floatVars["tWVars_Pt_AllJetsMuon"] = 300;
  _floatVars["tWVars_Pt_AllJetsElectron"] = 300;
  _floatVars["tWVars_Pt_AllJetsLeptons"] = 300;
  _floatVars["tWVars_Eta_AllJetsMuon"] = -6.;
  _floatVars["tWVars_Eta_AllJetsElectron"] = -6.;
  _floatVars["tWVars_Eta_AllJetsLeptons"] = -6.;
  _floatVars["tWVars_Phi_AllJetsMuon"] = -3.2;
  _floatVars["tWVars_Phi_AllJetsElectron"] = -3.2;
  _floatVars["tWVars_Phi_AllJetsLeptons"] = -3.2;
  _floatVars["tWVars_Mass_AllJetsMET"] = 1800;
  _floatVars["tWVars_E_AllJetsMET"] = 300;
  _floatVars["tWVars_Pt_AllJetsMET"] = 300;
  _floatVars["tWVars_Eta_AllJetsMET"] = -6.;
  _floatVars["tWVars_Phi_AllJetsMET"] = -3.2;
  _floatVars["tWVars_Mass_AllJetsMuonMET"] = 2000;
  _floatVars["tWVars_Mass_AllJetsElectronMET"] = 2000;
  _floatVars["tWVars_Mass_AllJetsLeptonsMET"] = 2000;
  _floatVars["tWVars_E_AllJetsMuonMET"] = 300;
  _floatVars["tWVars_E_AllJetsElectronMET"] = 300;
  _floatVars["tWVars_E_AllJetsLeptonsMET"] = 300;
  _floatVars["tWVars_Pt_AllJetsMuonMET"] = 300;
  _floatVars["tWVars_Pt_AllJetsElectronMET"] = 300;
  _floatVars["tWVars_Pt_AllJetsLeptonsMET"] = 300;
  _floatVars["tWVars_Eta_AllJetsMuonMET"] = -6.;
  _floatVars["tWVars_Eta_AllJetsElectronMET"] = -6.;
  _floatVars["tWVars_Eta_AllJetsLeptonsMET"] = -6.;
  _floatVars["tWVars_Phi_AllJetsMuonMET"] = -3.2;
  _floatVars["tWVars_Phi_AllJetsElectronMET"] = -3.2;
  _floatVars["tWVars_Phi_AllJetsLeptonsMET"] = -3.2;
  
  _floatVars["tWVars_AllJetsMuon_Centrality"] = 1.;
  _floatVars["tWVars_AllJetsElectron_Centrality"] = 1.;
  _floatVars["tWVars_AllJetsLeptons_Centrality"] = 1.;
  _floatVars["tWVars_Shat"] = 1000.;
  
  _floatVars["tWVars_DeltaEtaMuonMET"] = 6.;
  _floatVars["tWVars_DeltaEtaElectronMET"] = 6.;
  _floatVars["tWVars_DeltaEtaLeptonsMET"] = 6.;
  _floatVars["tWVars_DeltaPhiMuonMET"] = 3.2;
  _floatVars["tWVars_DeltaPhiElectronMET"] = 3.2;
  _floatVars["tWVars_DeltaPhiLeptonsMET"] = 3.2;
  _floatVars["tWVars_DeltaRMuonMET"] = 6.;
  _floatVars["tWVars_DeltaRElectronMET"] = 6.;
  _floatVars["tWVars_DeltaRLeptonsMET"] = 6.;
  
  _floatVars["tWVars_DeltaEtaJetMET"] = 6.;
  _floatVars["tWVars_DeltaPhiJetMET"] = 3.2;
  _floatVars["tWVars_DeltaRJetMET"] = 6.;
  
  _floatVars["tWVars_Mass_MuonJet"] = 1000.;
  _floatVars["tWVars_Mass_ElectronJet"] = 1000.;
  _floatVars["tWVars_Mass_LeptonsJet"] = 1000.;
  _floatVars["tWVars_E_MuonJet"] = 300;
  _floatVars["tWVars_E_ElectronJet"] = 300;
  _floatVars["tWVars_E_LeptonsJet"] = 300;
  _floatVars["tWVars_Pt_MuonJet"] = 300;
  _floatVars["tWVars_Pt_ElectronJet"] = 300;
  _floatVars["tWVars_Pt_LeptonsJet"] = 300;
  _floatVars["tWVars_Eta_MuonJet"] = -6.;
  _floatVars["tWVars_Eta_ElectronJet"] = -6.;
  _floatVars["tWVars_Eta_LeptonsJet"] = -6.;
  _floatVars["tWVars_Phi_MuonJet"] = -3.2;
  _floatVars["tWVars_Phi_ElectronJet"] = -3.2;
  _floatVars["tWVars_Phi_LeptonsJet"] = -3.2;
  
  _floatVars["tWVars_Mass_JetMET"] = 1200.;
  _floatVars["tWVars_E_JetMET"] = 300;
  _floatVars["tWVars_Pt_JetMET"] = 300;
  _floatVars["tWVars_Eta_JetMET"] = -6.;
  _floatVars["tWVars_Phi_JetMET"] = -3.2;
  
  _floatVars["tWVars_Mass_JetMuonMET"] = 1300.;
  _floatVars["tWVars_Mass_JetElectronMET"] = 1300.;
  _floatVars["tWVars_Mass_JetLeptonsMET"] = 1300.;
  _floatVars["tWVars_E_JetMuonMET"] = 300;
  _floatVars["tWVars_E_JetElectronMET"] = 300;
  _floatVars["tWVars_E_JetLeptonsMET"] = 300;
  _floatVars["tWVars_Pt_JetMuonMET"] = 300;
  _floatVars["tWVars_Pt_JetElectronMET"] = 300;
  _floatVars["tWVars_Pt_JetLeptonsMET"] = 300;
  _floatVars["tWVars_Eta_JetMuonMET"] = -6.;
  _floatVars["tWVars_Eta_JetElectronMET"] = -6.;
  _floatVars["tWVars_Eta_JetLeptonsMET"] = -6.;
  _floatVars["tWVars_Phi_JetMuonMET"] = -3.2;
  _floatVars["tWVars_Phi_JetElectronMET"] = -3.2;
  _floatVars["tWVars_Phi_JetLeptonsMET"] = -3.2;
  
  _floatVars["tWVars_Pt_RHT"] = 15.;
  
  _floatVars["tWVars_DeltaEtaMuonJet"] = 6.;
  _floatVars["tWVars_DeltaEtaElectronJet"] = 6.;
  _floatVars["tWVars_DeltaEtaLeptonsJet"] = 6.;
  _floatVars["tWVars_DeltaPhiMuonJet"] = 3.4;
  _floatVars["tWVars_DeltaPhiElectronJet"] = 3.4;
  _floatVars["tWVars_DeltaPhiLeptonsJet"] = 3.4;
  _floatVars["tWVars_DeltaRMuonJet"] = 6.;
  _floatVars["tWVars_DeltaRElectronJet"] = 6.;
  _floatVars["tWVars_DeltaRLeptonsJet"] = 6.;
  
  _floatVars["largestCSV"] = 1.;

  _floatVars["firstJetCSV"] = 1.;
  _floatVars["secondJetCSV"] = 1.;
  _floatVars["thirdJetCSV"] = 1.;
  
  _floatVars["taggedJetCSV"] = 1.;
  _floatVars["lightJetCSV"] = 1.;
  _floatVars["lightJet2CSV"] = 1.;
  
  _floatVars["tWVars_DeltaRBJetMuon"] = 6.;
  _floatVars["tWVars_DeltaRBJetElectron"] = 6.;
  _floatVars["tWVars_DeltaRBJetLeptons"] = 6.;
  
  _floatVars["tWVars_HT"] = 800.;
  _floatVars["tWVars_HT_JetLeptons"] = 800.;
  _floatVars["tWVars_HTLeptonsOverHT"] = 1.;
  _floatVars["tWVars_ptSysOverHT"] = 1.;


  SetDoHists(makeHistos);

}

void DileptonVars::FillBranches(EventContainer * evtObj){

  //Initialise vectors;
  Jet.clear();
  Jet2030.clear();
  Jet2040.clear();
  Jet3040.clear();
  Jet4000.clear();
  BJet.clear();
  BJet2030.clear();
  BJet2040.clear();
  BJet3040.clear();
  BJet4000.clear();
  Jete24.clear();
  Jet2030e24.clear();
  Jet2040e24.clear();
  Jet3040e24.clear();
  Jet4000e24.clear();
  Jet3500e24.clear();

  totalJets.SetPtEtaPhiE(0,0,0,0);
  sumJets2030.SetPtEtaPhiE(0,0,0,0);
  sumJets2040.SetPtEtaPhiE(0,0,0,0);
  sumJets3040.SetPtEtaPhiE(0,0,0,0);
  sumJets4000.SetPtEtaPhiE(0,0,0,0);

  //First thing we do is split up the jets into the vectors defined by pt
  TLorentzVector tempjet(0,0,0,0);
  EventTree* eventTree = evtObj->GetEventTree();
  for (auto jet : evtObj->alljets){
    tempjet.SetPtEtaPhiE(jet.Pt(),jet.Eta(),jet.Phi(),jet.E());
    //  for (int i = 0; i < eventTree-> Jet_pt->size(); i++){
    //tempjet.SetPtEtaPhiE(eventTree-> Jet_pt->at(i),eventTree->Jet_eta->at(i),eventTree->Jet_phi->at(i),eventTree->Jet_energy->at(i));
    if (tempjet.Pt() > 20 && tempjet.Pt() < 30) Jet2030.push_back(tempjet);
    if (tempjet.Pt() > 20 && tempjet.Pt() < 40) Jet2040.push_back(tempjet);
    if (tempjet.Pt() > 30 && tempjet.Pt() < 40) Jet3040.push_back(tempjet);
    if (tempjet.Pt() > 40) Jet4000.push_back(tempjet);
  }

  for (auto jet : evtObj->jets){
    tempjet.SetPtEtaPhiE(jet.Pt(),jet.Eta(),jet.Phi(),jet.E());
    Jet.push_back(tempjet);
    totalJets += tempjet;
    Jete24.push_back(tempjet);
    if (jet.Pt() > 20 && jet.Pt() < 30) Jet2030e24.push_back(tempjet);
    if (jet.Pt() > 20 && jet.Pt() < 40) Jet2040e24.push_back(tempjet);
    if (jet.Pt() > 30 && jet.Pt() < 40) Jet3040e24.push_back(tempjet);
    if (jet.Pt() > 40) Jet4000e24.push_back(tempjet);
  }
  
  for (auto jet : evtObj->taggedJets){
    tempjet.SetPtEtaPhiE(jet.Pt(),jet.Eta(),jet.Phi(),jet.E());
    BJet.push_back(tempjet);
    if (jet.Pt() > 20 && jet.Pt() < 30) BJet2030.push_back(tempjet);
    if (jet.Pt() > 20 && jet.Pt() < 40) BJet2040.push_back(tempjet);
    if (jet.Pt() > 30 && jet.Pt() < 40) BJet3040.push_back(tempjet);
    if (jet.Pt() > 40) BJet4000.push_back(tempjet);
  }

  //Now set up the lepton and met variables
  TLorentzVector Electron(00,0,0,0);
  TLorentzVector Muon(0,0,0,0);
  TLorentzVector Leptons(0,0,0,0);
  TLorentzVector Miss(00,0,0,0);
  
  Miss.SetPtEtaPhiE(evtObj->missingEt_xy,0,evtObj->missingPhi_xy,evtObj->missingEt_xy);
  Electron.SetPtEtaPhiE(evtObj->electronsToUsePtr->at(0).Pt(),evtObj->electronsToUsePtr->at(0).Eta(),evtObj->electronsToUsePtr->at(0).Phi(),evtObj->electronsToUsePtr->at(0).E());
  Muon.SetPtEtaPhiE(evtObj->muonsToUsePtr->at(0).Pt(),evtObj->muonsToUsePtr->at(0).Eta(),evtObj->muonsToUsePtr->at(0).Phi(),evtObj->muonsToUsePtr->at(0).E());
  Leptons = Electron + Muon;

  _floatVars["tWVars_Mass_Muon"]       = (Muon).M();
  _floatVars["tWVars_Mass_Electron"]       = (Electron).M();
  _floatVars["tWVars_Mass_Leptons"]       = (Leptons).M();

  _floatVars["tWVars_E_Muon"]          = (Muon).E();
  _floatVars["tWVars_E_Electron"]          = (Electron).E();
  _floatVars["tWVars_E_Leptons"]          = (Leptons).E();

  _floatVars["tWVars_Pt_Muon"]         = (Muon).Pt();
  _floatVars["tWVars_Pt_Electron"]         = (Electron).Pt();
  _floatVars["tWVars_Pt_Leptons"]         = (Leptons).Pt();

  _floatVars["tWVars_Eta_Muon"]        = (Muon).Eta();
  _floatVars["tWVars_Eta_Electron"]        = (Electron).Eta();
  _floatVars["tWVars_Eta_Leptons"]        = (Leptons).Eta();

  _floatVars["tWVars_Phi_Muon"]        = (Muon).Phi();
  _floatVars["tWVars_Phi_Electron"]        = (Electron).Phi();
  _floatVars["tWVars_Phi_Leptons"]        = (Leptons).Phi();  

  _floatVars["tWVars_DeltaEtaMuonMET"] =  fabs(Muon.Eta()-Miss.Eta());
  _floatVars["tWVars_DeltaEtaElectronMET"] =  fabs(Electron.Eta()-Miss.Eta());
  _floatVars["tWVars_DeltaEtaLeptonsMET"] =  fabs(Leptons.Eta()-Miss.Eta());

  _floatVars["tWVars_DeltaPhiMuonMET"] =  fabs(Miss.DeltaPhi(Muon));
  _floatVars["tWVars_DeltaPhiElectronMET"] =  fabs(Miss.DeltaPhi(Electron));
  _floatVars["tWVars_DeltaPhiLeptonsMET"] =  fabs(Miss.DeltaPhi(Leptons));

  _floatVars["tWVars_DeltaRMuonMET"] =  fabs(Miss.DeltaR(Muon));
  _floatVars["tWVars_DeltaRElectronMET"] =  fabs(Miss.DeltaR(Electron));
  _floatVars["tWVars_DeltaRLeptonsMET"] =  fabs(Miss.DeltaR(Leptons));


  _floatVars["tWVars_nJet2030"]   = Jet2030.size();
  for(int i=0;i<_floatVars["tWVars_nJet2030"];i++)
    {sumJets2030 += Jet2030.at(i);}


  _floatVars["tWVars_nJet2040"]   = (float)Jet2040.size();
  for(int i=0;i<Jet2040.size();i++)
    {sumJets2040 += Jet2040.at(i);}


  _floatVars["tWVars_nJet3040"]   = Jet3040.size();
  for(int i=0;i<_floatVars["tWVars_nJet3040"];i++)
    {sumJets3040 += Jet3040.at(i);}

  _floatVars["tWVars_nJet4000"]   = Jet4000.size();
  for(int i=0;i<_floatVars["tWVars_nJet4000"];i++)
    {sumJets4000 += Jet4000.at(i);}

  _floatVars["tWVars_nBJet2030"]   = BJet2030.size();
  _floatVars["tWVars_nBJet2040"]   = BJet2040.size();
  _floatVars["tWVars_nBJet3040"]   = BJet3040.size();
  _floatVars["tWVars_nBJet4000"]   = BJet4000.size();

  _floatVars["tWVars_nJet2030e24"]   = Jet2030e24.size();
  _floatVars["tWVars_nJet2040e24"]   = Jet2040e24.size();
  _floatVars["tWVars_nJet3040e24"]   = Jet3040e24.size();
  _floatVars["tWVars_nJet4000e24"]   = Jet4000e24.size();

  if(Jet2040.size()>=1){
    _floatVars["tWVars_Pt_Jet_2040"]     = (Jet2040.at(0)).Pt();
    _floatVars["tWVars_Eta_Jet_2040"]     = (Jet2040.at(0)).Eta();
    _floatVars["tWVars_Phi_Jet_2040"]     = (Jet2040.at(0)).Phi();
    _floatVars["tWVars_Mass_AllJets2040"]   = (sumJets2040).M();
    _floatVars["tWVars_E_AllJets2040"]   = (sumJets2040).E();
    _floatVars["tWVars_Pt_AllJets2040"]   = (sumJets2040).Pt();
    _floatVars["tWVars_Eta_AllJets2040"]   = (sumJets2040).Eta();
    _floatVars["tWVars_Phi_AllJets2040"]   = (sumJets2040).Phi();
  }

  if(_floatVars["tWVars_nJet2030"]>=1){
    _floatVars["tWVars_Pt_Jet_2030"]     = (Jet2030.at(0)).Pt();
    _floatVars["tWVars_Eta_Jet_2030"]     = (Jet2030.at(0)).Eta();
    _floatVars["tWVars_Phi_Jet_2030"]     = (Jet2030.at(0)).Phi();
    _floatVars["tWVars_Mass_AllJets2030"]   = (sumJets2030).M();
    _floatVars["tWVars_E_AllJets2030"]   = (sumJets2030).E();
    _floatVars["tWVars_Pt_AllJets2030"]   = (sumJets2030).Pt();
    _floatVars["tWVars_Eta_AllJets2030"]   = (sumJets2030).Eta();
    _floatVars["tWVars_Phi_AllJets2030"]   = (sumJets2030).Phi();
  }



  if(_floatVars["tWVars_nJet3040"]>=1){
    _floatVars["tWVars_Pt_Jet_3040"]     = (Jet3040.at(0)).Pt();
    _floatVars["tWVars_Eta_Jet_3040"]     = (Jet3040.at(0)).Eta();
    _floatVars["tWVars_Phi_Jet_3040"]     = (Jet3040.at(0)).Phi();
    _floatVars["tWVars_Mass_AllJets3040"]   = (sumJets3040).M();
    _floatVars["tWVars_E_AllJets3040"]   = (sumJets3040).E();
    _floatVars["tWVars_Pt_AllJets3040"]   = (sumJets3040).Pt();
    _floatVars["tWVars_Eta_AllJets3040"]   = (sumJets3040).Eta();
    _floatVars["tWVars_Phi_AllJets3040"]   = (sumJets3040).Phi();
  }


  if(_floatVars["tWVars_nJet4000"]>=1){
    _floatVars["tWVars_Pt_Jet_4000"]     = (Jet4000.at(0)).Pt();
    _floatVars["tWVars_Eta_Jet_4000"]     = (Jet4000.at(0)).Eta();
    _floatVars["tWVars_Phi_Jet_4000"]     = (Jet4000.at(0)).Phi();
    _floatVars["tWVars_Mass_AllJets4000"]   = (sumJets4000).M();
    _floatVars["tWVars_E_AllJets4000"]   = (sumJets4000).E();
    _floatVars["tWVars_Pt_AllJets4000"]   = (sumJets4000).Pt();
    _floatVars["tWVars_Eta_AllJets4000"]   = (sumJets4000).Eta();
    _floatVars["tWVars_Phi_AllJets4000"]   = (sumJets4000).Phi();
  }


  if(_floatVars["tWVars_nBJet2040"]>=1){
    _floatVars["tWVars_Pt_BJet_2040"]     = (BJet2040.at(0)).Pt();
    _floatVars["tWVars_Eta_BJet_2040"]     = (BJet2040.at(0)).Eta();
    _floatVars["tWVars_Phi_BJet_2040"]     = (BJet2040.at(0)).Phi();
  }
  if(_floatVars["tWVars_nBJet2030"]>=1){
    _floatVars["tWVars_Pt_BJet_2030"]     = (BJet2030.at(0)).Pt();
    _floatVars["tWVars_Eta_BJet_2030"]     = (BJet2030.at(0)).Eta();
    _floatVars["tWVars_Phi_BJet_2030"]     = (BJet2030.at(0)).Phi();
  }
  if(_floatVars["tWVars_nBJet3040"]>=1){
    _floatVars["tWVars_Pt_BJet_3040"]     = (BJet3040.at(0)).Pt();
    _floatVars["tWVars_Eta_BJet_3040"]     = (BJet3040.at(0)).Eta();
    _floatVars["tWVars_Phi_BJet_3040"]     = (BJet3040.at(0)).Phi();
  }
  if(_floatVars["tWVars_nBJet4000"]>=1){
    _floatVars["tWVars_Pt_BJet_4000"]     = (BJet4000.at(0)).Pt();
    _floatVars["tWVars_Eta_BJet_4000"]     = (BJet4000.at(0)).Eta();
    _floatVars["tWVars_Phi_BJet_4000"]     = (BJet4000.at(0)).Phi();
  }

  if(_floatVars["tWVars_nJet2040e24"]>=1){
    _floatVars["tWVars_Pt_Jet_2040e24"]     = (Jet2040e24.at(0)).Pt();
    _floatVars["tWVars_Eta_Jet_2040e24"]     = (Jet2040e24.at(0)).Eta();
    _floatVars["tWVars_Phi_Jet_2040e24"]     = (Jet2040e24.at(0)).Phi();
  }

  if(_floatVars["tWVars_nJet2030e24"]>=1){
    _floatVars["tWVars_Pt_Jet_2030e24"]     = (Jet2030e24.at(0)).Pt();
    _floatVars["tWVars_Eta_Jet_2030e24"]     = (Jet2030e24.at(0)).Eta();
    _floatVars["tWVars_Phi_Jet_2030e24"]     = (Jet2030e24.at(0)).Phi();
  }

  if(_floatVars["tWVars_nJet3040e24"]>=1){
    _floatVars["tWVars_Pt_Jet_3040e24"]     = (Jet3040e24.at(0)).Pt();
    _floatVars["tWVars_Eta_Jet_3040e24"]     = (Jet3040e24.at(0)).Eta();
    _floatVars["tWVars_Phi_Jet_3040e24"]     = (Jet3040e24.at(0)).Phi();
  }

  if(_floatVars["tWVars_nJet4000e24"]>=1){
    _floatVars["tWVars_Pt_Jet_4000e24"]     = (Jet4000e24.at(0)).Pt();
    _floatVars["tWVars_Eta_Jet_4000e24"]     = (Jet4000e24.at(0)).Eta();
    _floatVars["tWVars_Phi_Jet_4000e24"]     = (Jet4000e24.at(0)).Phi();
  }

  if(Jet.size()>=1){
    _floatVars["tWVars_Mass_AllJets"]   = (totalJets).M();
    _floatVars["tWVars_E_AllJets"]   = (totalJets).E();
    _floatVars["tWVars_Pt_AllJets"]   = (totalJets).Pt();
    _floatVars["tWVars_Eta_AllJets"]   = (totalJets).Eta();
    _floatVars["tWVars_Phi_AllJets"]   = (totalJets).Phi();
    
    _floatVars["tWVars_Mass_AllJetsMuon"]  = (totalJets+Muon).M();
    _floatVars["tWVars_Mass_AllJetsElectron"]  = (totalJets+Electron).M();
    _floatVars["tWVars_Mass_AllJetsLeptons"]  = (totalJets+Leptons).M();

    _floatVars["tWVars_E_AllJetsMuon"]  = (totalJets+Muon).E();
    _floatVars["tWVars_E_AllJetsElectron"]  = (totalJets+Electron).E();
    _floatVars["tWVars_E_AllJetsLeptons"]  = (totalJets+Leptons).E();

    _floatVars["tWVars_Pt_AllJetsMuon"]  = (totalJets+Muon).Pt();
    _floatVars["tWVars_Pt_AllJetsElectron"]  = (totalJets+Electron).Pt();
    _floatVars["tWVars_Pt_AllJetsLeptons"]  = (totalJets+Leptons).Pt();

    _floatVars["tWVars_Eta_AllJetsMuon"]  = (totalJets+Muon).Eta();
    _floatVars["tWVars_Eta_AllJetsElectron"]  = (totalJets+Electron).Eta();
    _floatVars["tWVars_Phi_AllJetsLeptons"]  = (totalJets+Leptons).Phi();
    
    _floatVars["tWVars_Mass_AllJetsMET"]     = (totalJets+Miss).M();
    _floatVars["tWVars_E_AllJetsMET"]     = (totalJets+Miss).E();
    _floatVars["tWVars_Pt_AllJetsMET"]     = (totalJets+Miss).Pt();
    _floatVars["tWVars_Eta_AllJetsMET"]     = (totalJets+Miss).Eta();
    _floatVars["tWVars_Phi_AllJetsMET"]     = (totalJets+Miss).Phi();
    
    _floatVars["tWVars_Mass_AllJetsMuonMET"]     = (totalJets + Muon + Miss).M();
    _floatVars["tWVars_Mass_AllJetsElectronMET"]     = (totalJets + Electron + Miss).M();
    _floatVars["tWVars_Mass_AllJetsLeptonsMET"]     = (totalJets + Leptons + Miss).M();

    _floatVars["tWVars_E_AllJetsMuonMET"]     = (totalJets + Muon + Miss).E();
    _floatVars["tWVars_E_AllJetsElectronMET"]     = (totalJets + Electron + Miss).E();
    _floatVars["tWVars_E_AllJetsLeptonsMET"]     = (totalJets + Leptons + Miss).E();

    _floatVars["tWVars_Pt_AllJetsMuonMET"]     = (totalJets + Muon + Miss).Pt();
    _floatVars["tWVars_Pt_AllJetsElectronMET"]     = (totalJets + Electron + Miss).Pt();
    _floatVars["tWVars_Pt_AllJetsLeptonsMET"]     = (totalJets + Leptons + Miss).Pt();

    _floatVars["tWVars_Eta_AllJetsMuonMET"]     = (totalJets + Muon + Miss).Eta();
    _floatVars["tWVars_Eta_AllJetsElectronMET"]     = (totalJets + Electron + Miss).Eta();
    _floatVars["tWVars_Eta_AllJetsLeptonsMET"]     = (totalJets + Leptons + Miss).Eta();

    _floatVars["tWVars_Phi_AllJetsMuonMET"]     = (totalJets + Muon + Miss).Phi();
    _floatVars["tWVars_Phi_AllJetsElectronMET"]     = (totalJets + Electron + Miss).Phi();
    _floatVars["tWVars_Phi_AllJetsLeptonsMET"]     = (totalJets + Leptons + Miss).Phi();

   
    _floatVars["tWVars_Mass_LeadJet"]     = (Jet.at(0)).M();
    _floatVars["tWVars_E_LeadJet"]     = (Jet.at(0)).E();
    _floatVars["tWVars_Pt_LeadJet"]     = (Jet.at(0)).Pt();
    _floatVars["tWVars_Eta_LeadJet"]     = (Jet.at(0)).Eta();
    _floatVars["tWVars_Phi_LeadJet"]     = (Jet.at(0)).Phi();

    _floatVars["tWVars_AllJetsMuon_Centrality"] = (totalJets.Pt() + Muon.Pt())/ (totalJets.P() + Muon.P());
    _floatVars["tWVars_AllJetsElectron_Centrality"] = (totalJets.Pt() + Electron.Pt())/ (totalJets.P() + Electron.P());
    _floatVars["tWVars_AllJetsLeptons_Centrality"] = (totalJets.Pt() + Leptons.Pt())/ (totalJets.P() + Leptons.P());

    _floatVars["tWVars_Shat"] = (totalJets+Leptons).M();
    double px_sys        = (Leptons + totalJets  + Miss).Px();
    double py_sys        = (Leptons + totalJets  + Miss).Px();
    _floatVars["tWVars_Pt_sys"]              =  sqrt(pow(px_sys,2)+pow(py_sys,2));
    // the pT_sys_err is related to calobase measurement and track measurment, now only consider the Calo item since they are dominate
    double pT_sys_err = sqrt((Leptons + totalJets + Miss).Pt()+(Leptons + totalJets + Miss).Et());
    //System_Pt_significance => get missing parts in the transverse plate
    _floatVars["tWVars_Pt_sys_sig"]            = _floatVars["tWVars_Pt_sys"]/pT_sys_err;

    _floatVars["tWVars_Pt_sys_MuonJet"]     = (Muon + Jet.at(0)).Pt();
    _floatVars["tWVars_Pt_sys_ElectronJet"]     = (Electron + Jet.at(0)).Pt();
    _floatVars["tWVars_Pt_sys_LeptonsJet"]     = (Leptons + Jet.at(0)).Pt();

    _floatVars["tWVars_eta_sys_MuonJet"]    = (Muon + Jet.at(0)).Eta();
    _floatVars["tWVars_eta_sys_ElectronJet"]    = (Electron + Jet.at(0)).Eta();
    _floatVars["tWVars_eta_sys_LeptonsJet"]    = (Leptons + Jet.at(0)).Eta();

    
    



    _floatVars["tWVars_Mass_MuonJet"] = (Jet.at(0)+Muon).M();
    _floatVars["tWVars_Mass_ElectronJet"] = (Jet.at(0)+Electron).M();
    _floatVars["tWVars_Mass_LeptonsJet"] = (Jet.at(0)+Leptons).M();

    _floatVars["tWVars_E_MuonJet"] = (Jet.at(0)+Muon).E();
    _floatVars["tWVars_E_ElectronJet"] = (Jet.at(0)+Electron).E();
    _floatVars["tWVars_E_LeptonsJet"] = (Jet.at(0)+Leptons).E();

    _floatVars["tWVars_Pt_MuonJet"] = (Jet.at(0)+Muon).Pt();
    _floatVars["tWVars_Pt_ElectronJet"] = (Jet.at(0)+Electron).Pt();
    _floatVars["tWVars_Pt_LeptonsJet"] = (Jet.at(0)+Leptons).Pt();

    _floatVars["tWVars_Eta_MuonJet"] = (Jet.at(0)+Muon).Eta();
    _floatVars["tWVars_Eta_ElectronJet"] = (Jet.at(0)+Electron).Eta();
    _floatVars["tWVars_Eta_LeptonsJet"] = (Jet.at(0)+Leptons).Eta();

    _floatVars["tWVars_Phi_MuonJet"] = (Jet.at(0)+Muon).Phi();
    _floatVars["tWVars_Phi_ElectronJet"] = (Jet.at(0)+Electron).Phi();
    _floatVars["tWVars_Phi_LeptonsJet"] = (Jet.at(0)+Leptons).Phi();

    
    _floatVars["tWVars_Mass_JetMET"]     = (Jet.at(0)+Miss).M();
    _floatVars["tWVars_E_JetMET"]     = (Jet.at(0)+Miss).E();
    _floatVars["tWVars_Pt_JetMET"]     = (Jet.at(0)+Miss).Pt();
    _floatVars["tWVars_Eta_JetMET"]     = (Jet.at(0)+Miss).Eta();
    _floatVars["tWVars_Phi_JetMET"]     = (Jet.at(0)+Miss).Phi();
    


    _floatVars["tWVars_Mass_JetMuonMET"]     = (Jet.at(0) + Muon +  Miss).M();
    _floatVars["tWVars_Mass_JetElectronMET"]     = (Jet.at(0) + Electron +  Miss).M();
    _floatVars["tWVars_Mass_JetLeptonsMET"]     = (Jet.at(0) + Leptons +  Miss).M();

    _floatVars["tWVars_E_JetMuonMET"]     = (Jet.at(0) + Muon +  Miss).E();
    _floatVars["tWVars_E_JetElectronMET"]     = (Jet.at(0) + Electron +  Miss).E();
    _floatVars["tWVars_E_JetLeptonsMET"]     = (Jet.at(0) + Leptons +  Miss).E();

    _floatVars["tWVars_Pt_JetMuonMET"]     = (Jet.at(0) + Muon +  Miss).Pt();
    _floatVars["tWVars_Pt_JetElectronMET"]     = (Jet.at(0) + Electron +  Miss).Pt();
    _floatVars["tWVars_Pt_JetLeptonsMET"]     = (Jet.at(0) + Leptons +  Miss).Pt();

    _floatVars["tWVars_Eta_JetMuonMET"]     = (Jet.at(0) + Muon +  Miss).Eta();
    _floatVars["tWVars_Eta_JetElectronMET"]     = (Jet.at(0) + Electron +  Miss).Eta();
    _floatVars["tWVars_Eta_JetLeptonsMET"]     = (Jet.at(0) + Leptons +  Miss).Eta();

    _floatVars["tWVars_Phi_JetMuonMET"]     = (Jet.at(0) + Muon +  Miss).Phi();
    _floatVars["tWVars_Phi_JetElectronMET"]     = (Jet.at(0) + Electron +  Miss).Phi();
    _floatVars["tWVars_Phi_JetLeptonsMET"]     = (Jet.at(0) + Leptons +  Miss).Phi();

    _floatVars["tWVars_Pt_RHT"] = _floatVars["tWVars_Pt_JetLeptonMET"]/_floatVars["tWVars_Pt_AllJetsLeptonMET"];
    _floatVars["tWVars_DeltaEtaJetMET"] =   fabs(Jet.at(0).Eta()-Miss.Eta());
    _floatVars["tWVars_DeltaPhiJetMET"] =   fabs(Jet.at(0).DeltaPhi(Miss));
    _floatVars["tWVars_DeltaRJetMET"] =   fabs(Jet.at(0).DeltaR(Miss));
    

    _floatVars["tWVars_DeltaEtaMuonJet"] =  fabs(Muon.Eta()-Jet.at(0).Eta());
    _floatVars["tWVars_DeltaEtaElectronJet"] =  fabs(Electron.Eta()-Jet.at(0).Eta());
    _floatVars["tWVars_DeltaEtaLeptonsJet"] =  fabs(Leptons.Eta()-Jet.at(0).Eta());

    _floatVars["tWVars_DeltaPhiMuonJet"] =  fabs(Jet.at(0).DeltaPhi(Muon));
    _floatVars["tWVars_DeltaPhiElectronJet"] =  fabs(Jet.at(0).DeltaPhi(Electron));
    _floatVars["tWVars_DeltaPhiLeptonsJet"] =  fabs(Jet.at(0).DeltaPhi(Leptons));

    _floatVars["tWVars_DeltaRMuonJet"] =  fabs(Jet.at(0).DeltaR(Muon));
    _floatVars["tWVars_DeltaRElectronJet"] =  fabs(Jet.at(0).DeltaR(Electron));
    _floatVars["tWVars_DeltaRLeptonsJet"] =  fabs(Jet.at(0).DeltaR(Leptons));

  }

  TLorentzVector W(0,0,0,0), Top(0,0,0,0);
  int maxInd = 2;
  if (evtObj->unTaggedJets.size() < maxInd) maxInd = evtObj->unTaggedJets.size();
  for (int jetIn = 0; jetIn < maxInd; jetIn++){
  	W = W + evtObj->unTaggedJets[jetIn];
  }

  _floatVars["tWVars_hadronicWmass"] = W.M();
  _floatVars["tWVars_hadronicWPt"] = W.Pt();
  _floatVars["tWVars_hadronicWEta"] = W.Eta();
  _floatVars["tWVars_hadronicWPhi"] = W.Phi();
  
  _floatVars["taggedJetCSV"] = -1;
  _floatVars["lightJetCSV"] = -1;
  _floatVars["lightJet2CSV"] = -1;
  if (evtObj->taggedJets.size() > 0) _floatVars["taggedJetCSV"] = evtObj->taggedJets[0].GetbDiscriminator();
  if (evtObj->unTaggedJets.size() > 0) _floatVars["lightJetCSV"] = evtObj->unTaggedJets[0].GetbDiscriminator();
  if (evtObj->unTaggedJets.size() > 1) _floatVars["lightJet2CSV"] = evtObj->unTaggedJets[1].GetbDiscriminator();

  _floatVars["firstJetCSV"] = evtObj->jets[0].GetbDiscriminator();
  _floatVars["secondJetCSV"] = -1.;
  _floatVars["thirdJetCSV"] = -1.;
  if (evtObj->jets.size() > 1) _floatVars["secondJetCSV"] = evtObj->jets[1].GetbDiscriminator();
  if (evtObj->jets.size() > 2) _floatVars["thirdJetCSV"] = evtObj->jets[2].GetbDiscriminator();
    
  int selectedBJetIndex = -1;
  _floatVars["largestCSV"] = -2.;
  selectedBJetIndex = 0;

  _floatVars["largestCSV"] = evtObj->jets[selectedBJetIndex].GetbDiscriminator();
  _floatVars["tWVars_DeltaRBJetMuon"] = fabs(Jet.at(selectedBJetIndex).DeltaR(Muon));
  _floatVars["tWVars_DeltaRBJetElectron"] = fabs(Jet.at(selectedBJetIndex).DeltaR(Electron));
  _floatVars["tWVars_DeltaRBJetLeptons"] = fabs(Jet.at(selectedBJetIndex).DeltaR(Leptons));

  //Make the HT variable
  float HT = Electron.Pt() + Muon.Pt() + Jet.at(0).Pt() + Miss.Pt();
  double px_sys        = (Leptons + totalJets  + Miss).Px();
  double py_sys        = (Leptons + totalJets  + Miss).Px();

  _floatVars["tWVars_HT"] = HT;

  _floatVars["tWVars_HT_JetLeptons"] =  Electron.Pt() + Muon.Pt() + Jet.at(0).Pt();

  if (sqrt(pow(px_sys,2)+pow(py_sys,2)) > 0.){
    _floatVars["tWVars_ptSysOverHT"] = HT/sqrt(pow(px_sys,2)+pow(py_sys,2));
  }
  else
    _floatVars["tWVars_ptSysOverHT"] = 0.;

  if (HT > 0.){
    _floatVars["tWVars_HTLeptonsOverHT"] = (Electron.Pt() + Muon.Pt())/HT;
  }
  else _floatVars["tWVars_HTLeptonsOverHT"] = 0.;

  if (DoHists()) FillHistograms(evtObj->GetEventWeight());

}
