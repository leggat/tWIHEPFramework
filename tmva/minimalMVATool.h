#ifndef mvaTool_h
#define mvaTool_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2F.h>
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TString.h>

#include <TLorentzVector.h>

#include <iostream>

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <tuple>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TThread.h"

#include "TMVAGui.C"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"




class minimalMVATool {
 public :
  minimalMVATool();  //  ~minimalMVATool();

  //run the bdt training
  void doTraining(TString signalFile, TString bkgFile);
  
  //Apply the BDT weights to other samples
  void doReading(TString inFile, TString outFile, TString sampleName);

  //  print the variable list
  void printVarList();

  //Set up BDT input variables
  void initialiseVarList();

  // Set up bdt variables
  void setNTrees(Int_t nTrees){_nTrees = nTrees;};
  void setDepth(Int_t depth){_depth = depth;};
  void setNCuts(Int_t nCuts){_nCuts = nCuts;};

 private:


  //Used to loop over MC samples
  void processMCSample(TString inFile,TString outFile,TString outDir, std::vector<float> proxyvars, TString sampleName);
  void loopInSample(TTree* theTree, TString sampleName, std::vector<std::vector<float > > proxyvars, bool isData, bool doMVA = true);

  //Make and fill histograms of BDT inputs and discriminant
  void createHists(TString sampleName);
  void fillHists(TString sampleName, std::vector<float> treevars, double mvaValue, double theweight);
  void saveHists(TFile * outFile);

  // These will be the histograms we made earlier
  std::map<TString,std::vector<TH1F*> > theHistoMap;
  //The BDT output
  std::map<TString,std::vector<TH1F*> > bdtHistoMap;

  //The BDT input variables 
  std::vector<TString> varList;
  //A proxy so that we can read systematics
  std::map<TString,float > _varsInBDTs;

  TMVA::Reader * reader;

  float theweight;
  float mvaValue;
  float mvawJetsValue;
  
  int _nTrees;
  int _depth;
  int _nCuts;

  //Names of various parts of the input
  TString _treeName; 
  TString _bdtName;

};

#endif
