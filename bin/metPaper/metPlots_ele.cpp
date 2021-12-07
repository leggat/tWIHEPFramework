/******************************************************************************
 * Wt.cpp                                                                     *
 *                                                                            *
 * Top Level file in analysis package                                         *
 * Drives the package                                                         *
 *                                                                            *
 * This file is the main file which will loop over all of the events, make    *
 * cuts and generate histograms.                                              *
 *                                                                            *
 * For general information on the analysis package as a whole, see            *
 * https://hep.pa.msu.edu/twiki/bin/view/AtlasSingleTop/HowToUseTheAnalysisFramework *
 *                                                                            *
 * Modifications:                                                             *
 *   23 Nov 2010 - H. ZHANG, modify first version from SandB analysis         *
 *****************************************************************************/
#define Glodfsdbal_cxx
#include "SingleTopRootAnalysis/Base/Dictionary/AnalysisMain.hpp"
#include <iostream>
#include <string>
#include "TSystem.h"

// Include histogramming classes
#include "SingleTopRootAnalysis/Cuts/Weights/EventWeight_nanoAOD.hpp"
#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingMuon.hpp"
#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingElectron.hpp"
#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingMET.hpp"
#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingMETPaper.hpp"
#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingMtW.hpp"
#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingJetAngular.hpp"
#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingJet.hpp"
#include "SingleTopRootAnalysis/Histogramming/Recon/HistogrammingNPvtx.hpp"
// Include cuts classes
#include "SingleTopRootAnalysis/Cuts/Jet/CutJetN.hpp"
#include "SingleTopRootAnalysis/Cuts/TaggedJet/CutTaggedJetN.hpp"
#include "SingleTopRootAnalysis/Cuts/Muon/CutMuonN.hpp"
#include "SingleTopRootAnalysis/Cuts/Muon/CutDiMuon.hpp"
#include "SingleTopRootAnalysis/Cuts/Muon/CutMuonTrigger.hpp"
#include "SingleTopRootAnalysis/Cuts/Muon/MatchTriggerMuon.hpp"
//#include "SingleTopRootAnalysis/Cuts/Muon/CutMuonTighterPt.hpp"
#include "SingleTopRootAnalysis/Cuts/Electron/CutElectronN.hpp"
#include "SingleTopRootAnalysis/Cuts/Electron/CutDiElectron.hpp"
#include "SingleTopRootAnalysis/Cuts/Electron/CutElectronTrigger.hpp"
#include "SingleTopRootAnalysis/Cuts/Electron/MatchTriggerElectron.hpp"
#include "SingleTopRootAnalysis/Cuts/Other/CutPrimaryVertex.hpp"
#include "SingleTopRootAnalysis/Cuts/Other/ChannelFlagCut.hpp"
#include "SingleTopRootAnalysis/Cuts/Other/CutMETFilters.hpp"
#include "SingleTopRootAnalysis/Cuts/Other/CutJsonFilter.hpp"
//
//#include "SingleTopRootAnalysis/Cuts/Other/CutZveto.hpp"
#include "SingleTopRootAnalysis/Cuts/Other/CutTriggerSelection.hpp"
//#include "SingleTopRootAnalysis/Cuts/Other/CutHFOR.hpp"
//#include "SingleTopRootAnalysis/Cuts/Other/CutBadPileupEvent.hpp"

//Include the additional variables that we would want in the skim tree
#include "SingleTopRootAnalysis/Vars/TestVar.hpp"
#include "SingleTopRootAnalysis/Vars/BDTVars.hpp"
#include "SingleTopRootAnalysis/Vars/JESBDTVars.hpp"
#include "SingleTopRootAnalysis/Vars/WeightVars.hpp"
#include "SingleTopRootAnalysis/Vars/ChannelFlag.hpp"
#include "SingleTopRootAnalysis/Vars/TopChargeFlag.hpp"
#include "SingleTopRootAnalysis/Vars/Bootstrap.hpp"

using std::cout;
using std::endl;

/******************************************************************************
 * int main(int argc, char **argv)                                            *
 *                                                                            *
 * Main Routine                                                               *
 *                                                                            *
 * Input:  Command line arguements                                            *
 * Output: Returns 0 of successful                                            *
 ******************************************************************************/
// Only declare main if not in CINT (not interactive?)
#ifndef __CINT__
int main(int argc, char **argv) 
#endif
{
  cout << "<driver> Starting parton_analysis" << endl;
  gROOT->ProcessLine("#include <vector>");

  /////////////////////////////////////////////////////////////////////////////////
  // Declartions and Instantiations
  /////////////////////////////////////////////////////////////////////////////////
  // Instantiate the analysis class
  AnalysisMain mystudy;

  TChain *chainReco       = new TChain("Events");
  TChain *chainTruth      = new TChain("TruthTree");
  TChain *chainTrigger    = new TChain("TriggerTree");
  TChain *chainInfo       = new TChain("InfoTree");
  TChain *chainDecision   = new TChain("DecisionTree");
  TChain *chainFastSim    = new TChain("FastSimTree");

 
 // Check command line for Fast Running
  // In Fast Running only a few cuts are made and histograms filled
  string mcStr="";
  Bool_t doMC = kFALSE;
  Bool_t doPileup = kFALSE;
  Bool_t dobWeight = kFALSE;
  Bool_t useInvertedIsolation = kFALSE;
  Bool_t useLeptonSFs = kFALSE;
  Bool_t usebTagReweight = kFALSE;
  Bool_t useIterFitbTag = kTRUE;
  TString leptonTypeToSelect = "Tight"; //This variable is altered to unisolated when running QCD estimation.
  string evtListFileName="";
  Bool_t verbose = kFALSE;
  // A couple of jet selection overrides to limit the number of config faces.  
  // -1 means use the value from the config file
  Int_t nJets = -1;
  Int_t nbJets = -1;

  //  cout << "<Driver> Reading arguments" << end;
  for (int i = 1; i < argc; ++i) {
    cout<<"Command line parameter "<<i<<" is "<<argv[i]<<endl;
    if (!strcmp(argv[i], "-inlist")) {
	string ListName = (string)(argv[i+1]);
	int b = ListName.find("SingleTop.");
	//	evtListFileName="WtDiElectron_"+ListName.substr(b)+".lst.root";
	//cout << "evtListFileName : " << evtListFileName << endl;
    }
    if (!strcmp(argv[i], "-verbose")){
      verbose = kTRUE;
      cout << "Driver: Running in verbose mode" << std::endl;
    }
    if (!strcmp(argv[i], "-lepSFs")){
      useLeptonSFs = kTRUE;
      cout << "Driver: Using lepton ID/iso SFs" << endl;
    }
    if (!strcmp(argv[i], "-bTagReshape")){
      usebTagReweight = kTRUE;
      cout << "Driver: Enable b-tag discriminant reshaping" << endl;
    }
    if (!strcmp(argv[i], "-noIterFitbTag")){
      useIterFitbTag = kFALSE;
      cout << "Driver: Not using iterative fit b-tag reshaping" << endl;
    }
    if (!strcmp(argv[i], "-MC") ||!strcmp(argv[i], "-mc") ) {
      doMC = kTRUE;
      cout << "Driver: This is a MC sample" << endl;
    }//if MC
    if (!strcmp(argv[i], "-PileUpWgt")) {
      mcStr=mcStr+"PileUpWgt";
      doPileup = kTRUE;
      cout << "Driver: Use PileUpWgt " << endl;
    }//if PileUpWgt
    if (!strcmp(argv[i], "-BWgt")) {
      mcStr=mcStr+"BWgt";
      dobWeight = kTRUE;
      cout << "Driver: Use b weight " << endl;
    }//if PileUpWgt
    if (!strcmp(argv[i], "-UseTotalEvtFromFile")) {
      mcStr=mcStr+"UseTotalEvtFromFile";
      cout << "Driver: UseTotalEvtFromFile " << endl;
    }//if UseTotalEvtFromFile
    if(!strcmp(argv[i],"-InvertIsolation")) {
      useInvertedIsolation = kTRUE;
      leptonTypeToSelect = "UnIsolated";
      cout << "Driver: useInvertedIsolation " << endl;
    }
    if (!strcmp(argv[i],"-nJets")){
      nJets = atoi(argv[i+1]);
    }// if nJets
    if (!strcmp(argv[i],"-nbJets")){
      nbJets = atoi(argv[i+1]);
    }// if nbJets
  } // for

  //This commands makes the eventcontainer be set up with nanoAOD! Probably this should be moved to the parsecmdline section.
  mystudy.SetDonanoAOD();

  //This command makes sure that a numnber of bits of code use the met muons
  mystudy.SetUseMetMuons();

  //This command fills a bunch of additional mets and plots
  mystudy.SetDoManyMETs();

  //////////////////////////////////////////////////////////////////////////////////
  // Parse the Command Line (the rest of it)
  //////////////////////////////////////////////////////////////////////////////////
  cout<<"USING THE TOPINPUTS TREE"<<endl;
  Int_t parseReturnValue = mystudy.ParseCmdLine(argc,argv,chainReco,chainTruth,chainTrigger,chainInfo,chainDecision, chainFastSim);
  cout<<"ParseCmdLine done"<<endl;
  if(parseReturnValue != 0) return parseReturnValue;

  /////////////////////////////////////////////////////////////////////////////////
  // Define a Particle class to be the same as the AnalysisMain class
  /////////////////////////////////////////////////////////////////////////////////
  EventContainer *particlesObj = &mystudy;
  particlesObj->SetIsSimulation(doMC);

  /////////////////////////////////////////////////////////////////////////////////
  // Add Cuts and Histograms applicable to Fast and Full Analyses
  /////////////////////////////////////////////////////////////////////////////////
  // ******** Cuts and Histograms applied to all studies ********

  mystudy.AddCut(new CutJsonFilter(particlesObj));

  mystudy.AddAction(new MatchTriggerElectron(particlesObj,"MetEle")); // This just sets up one muon as the trigger matched

  mystudy.AddAction(new EventWeight_nanoAOD(particlesObj,mystudy.GetTotalMCatNLOEvents(), mcStr, doPileup, dobWeight, useLeptonSFs, usebTagReweight, useIterFitbTag, verbose, "MetMu","MetEle"));

  mystudy.AddHists(new HistogrammingNPvtx(particlesObj));

  mystudy.AddCut(new CutTriggerSelection(particlesObj));

  mystudy.AddCut(new CutMETFilters(particlesObj));

  mystudy.AddCut(new CutElectronN(particlesObj, "MetEle"));     //require that lepton to be isolated, central, high pt

  mystudy.AddCut(new CutElectronTrigger(particlesObj, "MetEle"));     //require that one muon is above the offline pt selection of the trigger

  mystudy.AddCut(new CutDiElectron(particlesObj, "MetEle"));    //make dimuon selections
 
  //  mystudy.AddCut(new CutElectronN(particlesObj, "Tight")); //require that lepton to be isolated, central, high pt

  mystudy.AddHists(new HistogrammingNPvtx(particlesObj));

  mystudy.AddHists(new HistogrammingElectron(particlesObj,"MetEle"));  // make the muon plots, hopefully.

  //  mystudy.AddCut(new CutJetN(particlesObj,nJets));

  mystudy.AddHists(new HistogrammingJet(particlesObj));

  mystudy.AddHists(new HistogrammingMETPaper(particlesObj,"Electron","MetEle"));
  
  //Here you can add any additional variables that you require
  mystudy.AddVars(new WeightVars(useIterFitbTag));


  //Now we set up the output
  TFile *_skimBDTFile;
  TString NNname = mystudy.GetHistogramFileName() + "skimBDT.root" ;
  _skimBDTFile = new TFile(NNname,"RECREATE"); 
  //     mystudy.AddCut(new HistogrammingWtDiLepTopology(particlesObj, _skimBDTFile));
  //mystudy.AddCut(new HistogrammingWtDiLepTopology(particlesObj, _skimBDTFile));  // used to select a Zee candidate sample
  
  cout << "<driver> Starting Loop over events in chain" << endl << endl;
  // Loop over events in chain
  //Here is where the events are actually looped and the cuts and histograms above are made
  mystudy.Loop();
  cout << "<driver> Finished Loop over events in chain" << endl;

  // Write output to files
  cout << "<driver> " << "Writing histograms to file: " << mystudy.GetHistogramFileName() << endl;
  
  _skimBDTFile->Write(); 
  _skimBDTFile->Close(); 

  mystudy.Finish();
  
  // Delete chains and set pointers to NULL
  cout << "<driver> Deleting chains" << endl;

  if(chainReco)       delete chainReco;
  if(chainTruth)      delete chainTruth;
  if(chainFastSim)    delete chainFastSim;

  //if(chainCollection) delete chainCollection;

  chainReco       = NULL;
  chainTruth      = NULL;
  chainFastSim    = NULL;
  
  cout << "<driver> Sucessful Completion." << endl;
  return 0;
}
