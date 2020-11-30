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




class mvaTool {
 public :
 mvaTool() : mvaTool(0,true) {}
 mvaTool(Int_t channel) :  mvaTool(channel,true) {}
 mvaTool(Bool_t useIterFit ) : mvaTool(0,useIterFit) {}
 mvaTool(Bool_t useIterFit, Bool_t isEle) : mvaTool(0,useIterFit,isEle) {}
 mvaTool(Int_t channel, Bool_t useIterFit) : mvaTool(channel,useIterFit,kFALSE) {}
  mvaTool(Int_t channel, Bool_t useIterFit, Bool_t isEle);
  //  ~mvaTool();

  void doBothTraining(TString inDir);
  void doTraining(TString inDir, bool isttbar);
  void doReading(TString sampleName, TString inDir = "tW",TString outDir = "mvaOutput/", bool isData = false, int minFile = -1, int maxFile = -1);
  void doReadingNoMVA(TString sampleName, TString inDir = "tW",TString outDir = "mvaOutput/", bool isData = false);

  void setChannel(Int_t channel){_channel = channel;};
  //  void doReading();
  void printVarList();
  void initialiseVarList();
  void setNTrees(Int_t nTrees){_nTrees = nTrees;};
  void setDepth(Int_t depth){_depth = depth;};
  void setNCuts(Int_t nCuts){_nCuts = nCuts;};
  void setSplitBarrelEndcap(Bool_t split){_splitBarrelEndcap = split;};


 private:


  Int_t _channel; //The channel we want to read
  std::vector<TString> regionNames;

  //Used to loop over things
  void processMCSample(TString sampleName,TString inDir,TString outDir, std::vector<std::vector<float > > proxyvars, bool isData, bool doMVA = true, int minFile = -1, int maxFile = -1);
  void loopInSample(TTree* theTree, TString sampleName, std::vector<std::vector<float > > proxyvars, bool isData, bool doMVA = true);
  void createHists(TString sampleName);
  void fillHists(TString sampleName, std::vector<float> treevars, double mvaValue, double mvawJets, double theweight, float met, float mtw, float jet1Pt, int theChannel);
  void saveHists(std::vector<TFile *> outFile);
  void setbTagVars(TChain* theTree); 

  void makeStatVariationHists(TString sampleName,std::vector<TFile*> outputFile);


  std::tuple<float,float> calculatebTagSyst(float,std::vector<float>);
  std::tuple<float,float> calculatebMistagSyst(float,std::vector<float>);

  std::map<TString,std::vector<std::vector<TH1F*> > > theHistoMap;
  std::map<TString,std::vector<std::vector<TH1F*> > > bdtHistoMap;
  std::map<TString,std::vector<TH2F*> > the2DHistoMap;

  std::vector<std::vector<TString > > varList;
  std::map<TString,float > _varsInBDTs;
  std::map<TString,std::vector<float > * > _varsInBDT_JESShifts;

  std::vector<TString > samplelist;
  std::vector<TString > systlist;
  TString baseName;
  std::vector<float> bTagSysts;

  Bool_t _useIterFit;

  Bool_t _isEle;

  std::vector<TMVA::Reader *> reader;
  //TMVA::Reader * reader;

  float theweight;
  float mvaValue;
  float mvawJetsValue;
  
  int _nTrees;
  int _depth;
  int _nCuts;


  //How we decide whether to split between barrel and endcap or not. Default will be true hardcoded. 
  Bool_t _splitBarrelEndcap;

  std::vector<TString> _bdtName;

};

#endif
