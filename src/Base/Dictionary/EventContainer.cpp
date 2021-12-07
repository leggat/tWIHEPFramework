/******************************************************************************
 * EventContainer.cpp                                                         *
 *                                                                            *
 * Read in variables from EventTree and put them into an Object               *
 *                                                                            *
 * Public Members of EventContainer    class                                  *
 *    EventContainer()                  -- Default Constructor                *
 *    ~EventContainer()                 -- Destructor                         *
 *    GetSourceName()                   -- Get name of source being run on    *
 *    GetSourceNumber()                 -- Get number of source being run on  *
 *    GetTargetTopMass()                -- Get Top Quark Mass                 *
 *    SetSourceName()                   -- Set name of source being run on    *
 *    SetSourceNumber()                 -- Set number of source being run on  *
 *    SetTargetTopMass()                -- Set Top Quark Mass                 *
 *    Initialize()                      -- Initialize class                   *
 *    MakeTopQuarks()                   -- Reconstruct Top in several ways    *
 *    GetNextEvent()                    -- Set up particles for next event    *
 *    SetEventCount()                   -- Set counter for specific event     *
 *    GetCollectionTree()               -- Get the collection tree            *
 *    GetEventTree()                    -- Get the event tree                 *
 *    GetTruthTree()                    -- Get the truth tree                 *
 *    GetTruthAllTree()                    -- Get the truth tree              *
 *    GetTriggerTree()                  -- Get the trigger tree               *
 *    GetFastSimTree()                  -- Get the fastsim tree               *
 *    GetDebugLevel()                   -- Get debug level                    *
 *    SetDebugLevel()                   -- Set debug level                    *
 *    SetDoTrigger()                    -- Set if trigger tree                *
 *    SetDoTruthAll()                   -- Set if TruthAll tree               *
 *    SetDoTruth()                      -- Set if Truth tree                  *
 *    DoTrigger()                       -- If trigger tree                    *
 *    DoTruthAll()                      -- If TruthAll tree                   *
 *    DoTruth()                         -- If Truth tree                      *
 *    GetEventWeight()                  -- Get Event Weight                   *
 *    GetOutputEventWeight()            -- Get Output EventWeight             *
 *    GetTreeEventWeight()              -- Get Tree Event Weight              *
 *    GetGlobalEventWeight()            -- Get Global EventWeight             *
 *    SetGlobalEventWeight()            -- Set global event weight            *
 *    SetOutputEventWeight()            -- Set output event weight            *
 *    GetConfig()                       -- Get Configuration                  *
 *    ApplyJESShift()                   -- scale jet energies                 * 
 *                                                                            *
 *    Int_t ReadEvent()                 -- Create TLorentz Vectors            *
 *    Bool_t CreateFinalState()         -- Create final state particles       *
 *    Int_t runNumber                   -- Run number from the root tree      *
 *    Int_t eventNumber                 -- Event number from the root tree    *
 *    vector<Electron> electrons        -- Electron Vector                    *
 *    vector<Muon>     muons            -- Muon Vector                        *
 *    vector<Muon>     isolatedmuons    -- Isolated Muon Vector               *
 *    vector<Muon>     unisolatedmuons  -- UnIsolated Muon Vector             *
 *    vector<Tau>      taus             -- Tau Vector                         *
 *    vector<Jet>      jets             -- Jet Vector                         *
 *    vector<Jet>      taggedJets       -- taggedJet Vector                   *
 *    vector<Jet>      untaggedJets     -- untaggedJet Vector                 *
 *    vector<Jet>       bLabeledJets    -- jet id'ed with MC B quark          *
 *    vector<Jet>       cLabeledJets    -- jet id'ed with MC C quark          *
 *    vector<Jet>      tauLabeledJets   -- jet id'ed with MC tau              *
 *    vector<Jet> lightQuarkLabeledJets -- jet not id'ed with MC B, C, or tau *
 *    vector<MCParticle> MCParticles    -- MC Particle Vector                 *
 *    vector<MCMuon>   MCmuons          -- MC Muon Vector                     *
 *    vector<MCElectron>   MCElectrons  -- MC Electron Vector                 *
 *    vector<MCTau>   MCTaus            -- MC Tau Vector                      *
 *    vector<MCJet>   MCJets            -- MC Jet Vector                      *
 *    vector<MCJet>   MCBJets           -- MC BJet Vector                     *
 *    vector<MCJet>   MCCJets           -- MC CJet Vector                     *
 *    vector<MCJet>   MCLightQuarkJets  -- MC LightJetJet Vector              *
 *    vector<MCTop>   MCTops            -- MC Top Vector                      *
 *    vector<MCW>     MCWs              -- MC W Vector                        * 
 *    vector<MCAllParticle> MCAllParticles -- MCAllParticle Vector            *
 *    vector<MCAllMuon> MCAllMuons      -- MCAllMuon Vector                   *
 *    vector<MCAllElectron> MCAllElectrons -- MCAll Electron Vector           *
 *    vector<MCAllJet> MCAllJets         -- MCAll Jet Vector                  *
 *    vector<MCAllTau> MCAllTaus         -- MCAll Tau Vector                  *
 *    vector<MCAllChargedPion> MCAllChargedPions -- MCAll Charged Pion Vector *
 *    vector<MCAllChargedK> MCAllChargedKs -- MCAll Charged K Vector          *
 *                                                                            *
 * Protected Members of EventContainer class                                  *
 *    Int_t debugLevel                  -- Debug level                        *
 *    TEnv _config                      -- Used to read configuration         *
 *                                                                            *
 * Private Members of EventContainer class                                    *
 *    Double_t _targetTopMass           -- Target top mass                    *
 *    CollectionTree *_collectionTree   -- Collection tree                    *
 *    EventTree *_eventTree             -- Event tree                         *
 *    TruthTree *_thruthTree            -- Truth tree                         *
 *    Int_t    _eventCount              -- Location in event chain            *
 *    TString  _sourceName              -- Name of source being read          *
 *    TString _sourceNumber             -- Number of source being read        *
 *    Double_t _globalEventWeight       -- Global Event Weight                *
 *    Double_t _treeEventWeight         -- Tree Event Weight                  *
 *    Double_t _outputEventWeight       -- Output Event Weight                *
 *                                                                            *
 *    Apply()                           -- Apply cuts and fill histograms     *
 *    GetCutName()                      -- Returns "CutJetN"                  *
 *                                                                            *
 * Private Data Members of CutJetN                                            *
 *    myTH1F* _hNJetsBefore              -- Hist of Num of jets before cuts   *
 *    myTH1F* _hNJetsAfter               -- Hist of Num of jets before cuts   *
 *    Int_t _nJetsMin;                   -- Minimum number of jets to require *
 *    Int_t _nJetsMax;                   -- Maximum number of jets allowed    *
 *                                                                            *
 * History                                                                    *
 *      16 June 2015 - Created by Huaqiao ZHANG(zhanghq@ihep.ac.cn) for CMS   *
 *****************************************************************************/

#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"
#include <iostream>
#include <string>
#include <fstream>

#include <TLeaf.h>

#include "SingleTopRootAnalysis/Base/Histograms/utils.hpp"

using namespace std;
//using namespace Analysis;
// Integrate classes into the Root system
ClassImp(EventContainer)

//---if (! DoFastSim()   && _eventTree      == NULL) return -1;--------------------------------------------------------------------------------------
// Get the next event.
// It reads in the next event from the current chain and 
// Creates final state particles.
// This method returns -1 when there are no more events.
// It returns the total event count if successful.
//
Int_t EventContainer::GetNextEvent(){

  //cout << "<EventContainer::GetNextEvent Start> " << endl;
  Int_t countOffSet=0;
  if (! (DoFastSim() || DonanoAOD())   && _eventTree      == NULL) return -1;
  if (DoTruth()       && _truthTree      == NULL) return -1;
  if (DoFastSim()     && _fastSimTree    == NULL) return -1;
  if (DonanoAOD()     && _nanoAODTree    == NULL) return -1;

  // now Set things up
  Int_t bytesReadTru;
  Int_t bytesReadEv;

  // ************************
  // FastSim
  // ************************
  if(DoFastSim()) {
    bytesReadEv = _fastSimTree->fChain->GetEntry(_eventCount);
  } 
  else if (DonanoAOD()){
    bytesReadEv = _nanoAODTree->fChain->GetEntry(_eventCount);
  }
  else {
    // normal running, get the next eventin the event tree
    bytesReadEv = _eventTree->fChain->GetEntry(_eventCount);
  } 

  if(0 == bytesReadEv) {
    if(DoFastSim()){
      cout << "<EventContainer> Read 0 bytes from fast sim tree." << endl;
      return -1;
    }else  {
      cout << "<EventContainer> Read 0 bytes from event tree." << endl;
      return -1;
    }
  } //if
  // ************************
  // Truth
  // ************************
  if(DoTruth()) {
    Int_t bytesReadTruth = _truthTree->fChain->GetEntry(_eventCount);
    if(0 == bytesReadTruth) {
    cout << "<EventContainer> Read 0 bytes from truth tree." << endl;
    return -1;
    } //if
  } //if
  
  // ************************
  // Skimming
  // ************************
  if(DoSkim()){
    Int_t bytesReadTri =0;
    if (DonanoAOD()){
      bytesReadTri = _nanoAODTree->fChain->GetEntry(_eventCount);
      if(0 == bytesReadTri) {
	cout << "<EventContainer> Read 0 bytes from LooseTopInputs tree." << endl;
	return -1;
      }
    }
    else {
    bytesReadTri = _eventTree->fChain->GetEntry(_eventCount);
    if(0 == bytesReadTri) {
      cout << "<EventContainer> Read 0 bytes from LooseTopInputs tree." << endl;
      return -1;
    }
    }
    if (_truthTree!=NULL){
      bytesReadTri = _truthTree->fChain->GetEntry(_eventCount);
      if(0 == bytesReadTri) {
	cout << "<EventContainer> Read 0 bytes from Truth tree." << endl;
	return -1;
      }
    }

  }

  // Create 4-vectors
  //cout << "<EventContainer::GetNextEvent::ReadEvent Start> " << endl;
  ReadEvent();         //The object selections and corrections are here
  //cout << "<EventContainer::GetNextEvent::CreateFinalState Start> " << endl;
  CreateFinalState();  // the event selections are here
  //cout << "<EventContainer::GetNextEvent::MakeTopQuarks Start> " << endl;
  MakeTopQuarks();
  //cout << "<EventContainer::GetNextEvent::MakeTopQuarks End> " << endl;
  // and finally increment the internal event counter
  _eventCount++;
  if(GetIsFirstEvent() == true) SetIsFirstEvent(false); // not the first event
  return(countOffSet + _eventCount-1);
} // end of GetNextEvent
  
/******************************************************************************
 * EventContainer::EventContainer()                                           *
 *                                                                            *
 * Default Constructor                                                        *
 * Initializes many private data members                                      *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
EventContainer::EventContainer(): 
  //_collectionTree(NULL), 
  _eventTree(NULL), _nanoAODTree(NULL), _truthTree(NULL), 
  _eventCount(0),   _targetTopMass(175.),
  _debugLevel(0),   _donanoAOD(false),  _doFastSim(false),_doSkim(false),_doManyMETs(false),
  _sourceName("NONE"),
  _globalEventWeight(1.), _treeEventWeight(1.), _outputEventWeight(1.),_EventPileupWeight(-1),_genEventWeight(1.),_generalReweight(1.),
  _EventPileupMinBiasUpWeight(-1),_EventPileupMinBiasDownWeight(-1),
  _config("Configuration"), _JESconfig("JESConfiguration"),_jesError(0.), _jesShift(0), _bTagAlgo("default"), _bTagCut(999), _misTagCut(999), jeteoverlap(kFALSE),closeindex(999),ejordr(999), bestjetdr(999), _isFirstEvent(true), isSimulation(kTRUE), _badJetEvent(kFALSE),  _celloutShift(0),_softjetShift(0),_pileupShift(0),_larShift(0),_metShift(0), _JESconfigread(false),_jesUShift(0),_jesPtShift(0),_jesEtaShift(0),_useUnisolatedLeptons(kFALSE),_trigID(0)
{
 
} //EventContainer()


/******************************************************************************
 * EventContainer::~EventContainer()                                          *
 *                                                                            *
 * Destructor                                                                 *
 * Sets pointers to NULL                                                      *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
EventContainer::~EventContainer()
{

  // Event Tree
  if(_eventTree) delete _eventTree;
  _eventTree = NULL;

  // Truth Tree
  if(_truthTree) delete _truthTree;
  _truthTree = NULL;

  // Fast Sim Tree
  // This causes a seg fault - don't know why
  //if(_fastSimTree) delete _fastSimTree;
  //_fastSimTree = NULL;

} //~EventContainer()

/******************************************************************************
 * void EventContainer::Initialize()                                          *
 *                                                                            *
 * Initialize class                                                           *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
//void EventContainer::Initialize(CollectionTree* collectionTree,EventTree* eventTree, TruthTree* truthTree)
//
void EventContainer::Initialize( EventTree* eventTree, nanoAODTree* nanoAODTree, TruthTree* truthTree)
{

  _eventTree       = eventTree;
  _truthTree       = truthTree;
  _nanoAODTree     = nanoAODTree;
//  // now Set up the internal variables:
  SetIsFirstEvent(true);
  actualIntPerXing = 0;
  averageIntPerXing = 0 ;
//  Pvtxall_n = -999;
//  isSimulation = kTRUE;

  _eventCount = 0;
  electrons.clear();
  tightElectrons.clear();
  vetoElectrons.clear();
  metElectrons.clear();
  ptetaElectrons.clear();
  isolatedElectrons.clear();
  unIsolatedElectrons.clear();
  muons.clear();
  tightMuons.clear();
  vetoMuons.clear();
  metMuons.clear();
  triggerMatchMuons.clear();
  ptetaMuons.clear();
  isolatedMuons.clear();
  unIsolatedMuons.clear();
  taus.clear();
  jets.clear();
  alljets.clear();
  taggedJets.clear();
  unTaggedJets.clear();
  bLabeledJets.clear();
  cLabeledJets.clear();
  tauLabeledJets.clear();
  lightQuarkLabeledJets.clear();
  neutrinos.clear();
  triggerObjects.clear();

  genparts.clear();
  triggerBits.clear();
  _triggerNames.clear();
//  MCParticles.clear();
//  MCMuons.clear();
//  MCElectrons.clear();
//  MCTaus.clear();
//  MCJets.clear();
//  MCBJets.clear();
//  MCCJets.clear();
//  MCLightQuarkJets.clear();
//  MCTops.clear();
//  MCWs.clear();
//  MCNeutrinos.clear();
//  MCBQuarks.clear();
//  MCCQuarks.clear();
//
//  JER tool  
//  myJER = new JERProvider("AntiKt4TopoJES", "Truth", "config/JER/JERProviderPlots.root");
//  myJER->init();//for JER if not flat shift

//  electron energy scale and resolution
//  int elecEnergyScale = _config.GetValue("ObjectID.ElecEnergyScale.shift",0);
//  int elecEnergySmear = _config.GetValue("ObjectID.ElecEnergySmear.shift",0);
//  string mcWithConstantTerm = _config.GetValue("ObjectID.ElecEnergySmear.mcWithConstantTerm","True");
//  useElecEnergyScale = elecEnergyScale;
//  useElecEnergySmear = elecEnergySmear;

//  if(mcWithConstantTerm=="True") {
//    useMCWithConstantTerm = true;
//  } else {
//    useMCWithConstantTerm = false;
//  }
//
//  //taggername = "SV0";
//  taggername = "Jet1FitterCOMBNN";
//  CalibROOT = new CalibrationDataInterfaceROOT(taggername, "config/btag/BTagCalibration.env", ""); 
//  CalibVar.jetAuthor = "AntiKt4Topo";
//  uncertainty = Total;
//
  
  // Check for any systematic uncertainties we may be calculating
  _metShift = _config.GetValue("Systs.metShift",0);
  _channelName = _config.GetValue("ChannelName","");
  TString jerFile =GetConfig()->GetValue("Include.jerFile","config/weights/Spring16_25nsV6_MC_PtResolution_AK4PFchs.txt");
  std::ifstream file(jerFile);
  std::string lineStr;
  bool firstLine = true;
  while (std::getline(file,lineStr)){
    std::stringstream ss(lineStr);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);
    if (firstLine){
      _resFormula = vstrings[5];
      firstLine = false;
      continue;
    }
    _resolution.push_back(vstrings);
  }
  TString sfFile =GetConfig()->GetValue("Include.jerSFFile","config/weights/Spring16_25nsV6_MC_SF_AK4PFchs.txt");
  std::ifstream sfile(sfFile);                                                                                            
  firstLine = true;                                                                                                  
  while (std::getline(sfile,lineStr)){                                                                                     
    std::stringstream ss(lineStr);                                                                                        
    std::istream_iterator<std::string> begin(ss);                                                                         
    std::istream_iterator<std::string> end;                                                                               
    std::vector<std::string> vstrings(begin, end);                                                                        
    if (firstLine){                                                                                                       
      firstLine = false;                                                                                                  
      continue;                                                                                                           
    }                                                                                                                     
    _resSFs.push_back(vstrings);                                                                                      
  }                                                                                                                       

  yearName = GetConfig()->GetValue("Info.RunYear","2018");

  //Set up the default values for the proxy muon and electron collections used in jet cleaning. Can be adjusted elsewhere.
  SetUseUnisolatedLeptons(kFALSE,0);

  //Get a list of the trigger names in the tree if doing nanoAOD
  if (DonanoAOD()) GetTriggerNames(nanoAODTree);

  return;
} //Initialize()

/******************************************************************************
 * void EventContainer::GetTriggerNames(nanoAODTree* tree)                    *
 *                                                                            *
 * Read the list of HLT names from the nanoAOD tree                           *
 *                                                                            *
 * Input:  nanoAODTree                                                        *
 * Output: None                                                               *
 ******************************************************************************/
void EventContainer::GetTriggerNames(nanoAODTree * inputTree){
  Int_t numberOfBranches = inputTree->fChain->GetListOfBranches()->GetEntries();
  for (int i=0; i < numberOfBranches; i++){
    TString branchName = inputTree->fChain->GetListOfBranches()->At(i)->GetName();
    if (branchName.BeginsWith("HLT_")){
	_triggerNames.push_back(branchName);
	//	TBranch* tmpBranch = (TBranch*) inputTree->fChain->GetListOfBranches()->At(i);
	//triggerBranches[branchName] = tmpBranch;
	triggerBits[branchName] = kFALSE;
	//	inputTree->fChain->SetBranchAddress(branchName, &triggerBits[branchName], &triggerBranches[branchName]);
	inputTree->fChain->SetBranchAddress(branchName, &triggerBits[branchName]);
      }
  }
}


/******************************************************************************
 * void EventContainer::SetupObjectDefinitions(TEnv config)                   *
 *                                                                            *
 * Sets up the different object definitions for each object                   *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void EventContainer::SetupObjectDefinitions(){
  newMuon.SetCuts(GetConfig(),"Tight");
  newMuon.SetCuts(GetConfig(),"All");
  newMuon.SetCuts(GetConfig(),"UnIsolated");
  newMuon.SetCuts(GetConfig(),"Veto");
  newMuon.SetCuts(GetConfig(),"MetMu");
  newMuon.SetCuts(GetConfig(),"TriggerMatch");

  newElectron.SetCuts(GetConfig(),"Tight");
  newElectron.SetCuts(GetConfig(),"All");
  newElectron.SetCuts(GetConfig(),"Veto");
  newElectron.SetCuts(GetConfig(),"MetEle");
  newElectron.SetCuts(GetConfig(),"UnIsolated");

  newJet.SetCuts(GetConfig());

}

/******************************************************************************
 * void EventContainer::SetUseUnisolatedLeptons(                              *
 *           Bool_t useUnisolatedLeptons, int whichtrig)                      *
 *                                                                            *
 * Used to switch between QCD estimation and non. This is important for       *
 * the jet cleaning algorithm						      *
 * This is only important for the tW analysis. I am going to leave it here    *
 * as an example of how to go about it, but I will call a default version in  *
 * the constructor to set up the variables and forget about this for now.     *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/

void EventContainer::SetUseUnisolatedLeptons(const Bool_t& useUnisolatedLeptons, int whichtrig){
  _useUnisolatedLeptons = useUnisolatedLeptons;
  _trigID = whichtrig;
  electronsToUsePtr = &tightElectrons;
  muonsToUsePtr = &tightMuons;
  if (_trigID == 0 && _useUnisolatedLeptons){
    electronsToUsePtr = &unIsolatedElectrons;
  }
  else if (_trigID == 1 && useUnisolatedLeptons){
    muonsToUsePtr = &unIsolatedMuons;
  }

  //For the synch excercise we want it to always be tight leptons, so I'm gonna add here the ability to just make it all tight.
  //  if (GetChannelName() == "ee" || GetChannelName() == "emu" || GetChannelName() == "mumu"){
  // }
} //SetUseUnisolatedLeptons

/******************************************************************************
 * void EventContainer::InitializeFastSim()                                   *
 *                                                                            *
 * Initialize class                                                           *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
//void EventContainer::InitializeFastSim(FastSimTree* fsTree, TruthTree* truthTree)
//{
//  return;
//} //Initialize()


/******************************************************************************
 * Int_t EventContainer::ReadEvent()                                          *
 *                                                                            *
 * Fill objects with event info                                               *
 *                                                                            *
 * Input:  None                                                               *
 * Output: Return 0                                                           *
 ******************************************************************************/
Int_t EventContainer::ReadEvent()
{

  // Set the event weight if there is any
  if(DoFastSim()) {
    // Will have to be updated when we have a fast sim tree
    _treeEventWeight = _fastSimTree -> eventWeight;
    runNumber   = _fastSimTree      -> runNumber;
    eventNumber = _fastSimTree      -> eventNumber;
    larError = 5;
    actualIntPerXing = 0;
    averageIntPerXing = 0 ;
    distns = -999;
    distbunch = -999;
    bcid = -999;
    safejetevent= -999;
    safejeteventup= -999;
    safejeteventdown= -999;  
  } else if (DonanoAOD()) {
    _treeEventWeight = 1.0;                        
    runNumber          = _nanoAODTree -> run;  
    eventNumber        = _nanoAODTree -> event;
    _genEventWeight    = _nanoAODTree -> genWeight;
    lumiBlock          = _nanoAODTree -> luminosityBlock;
    actualIntPerXing   = 1;//_eventTree ->         
    averageIntPerXing  = 1;//_eventTree ->         
    bcid               = 1;//_eventTree ->     

    //Adding HTXS variables
    HTXS_Higgs_pt        = _nanoAODTree -> HTXS_Higgs_pt;
    HTXS_Higgs_y        = _nanoAODTree -> HTXS_Higgs_y;
    HTXS_stage_0        = _nanoAODTree -> HTXS_stage_0;
    //HTXS

    distns = -999;                                 
    distbunch = -999;                              
    safejetevent= -999;                            
    safejeteventup= -999;                         
    safejeteventdown= -999;                        
  }  else {
    _treeEventWeight = 1.0;
    runNumber          = _eventTree -> EVENT_run;
    eventNumber        = _eventTree -> EVENT_event;
    actualIntPerXing   = 1;//_eventTree -> 
    averageIntPerXing  = 1;//_eventTree -> 
    bcid               = 1;//_eventTree -> 
    distns = -999;
    distbunch = -999;
    safejetevent= -999;
    safejeteventup= -999;
    safejeteventdown= -999;
  }
  //  isSimulation = kTRUE;
  _badJetEvent = kFALSE;

  // Reset all of the particle vectors
  electrons.clear();
  tightElectrons.clear();
  metElectrons.clear();
  vetoElectrons.clear();
  ptetaElectrons.clear();
  isolatedElectrons.clear();
  unIsolatedElectrons.clear();
  genparts.clear();

  muons.clear();
  tightMuons.clear();
  vetoMuons.clear();
  metMuons.clear();
  triggerMatchMuons.clear();
  ptetaMuons.clear();
  isolatedMuons.clear();
  unIsolatedMuons.clear();

  taus.clear();

  jets.clear();
  alljets.clear();
  taggedJets.clear();
  unTaggedJets.clear();
  bLabeledJets.clear();
  cLabeledJets.clear();
  tauLabeledJets.clear();
  lightQuarkLabeledJets.clear();
  neutrinos.clear();
  triggerObjects.clear();

  //Some extra resets
  missingEtVecs.clear();
  pv_chi2 = -999;
  pv_ndof = -999;
  pv_npvs = -999;
  pv_npvsGood = -999;
  pv_score = -999;
  pv_x = -999;
  pv_y = -999;
  pv_z = -999;
  trueInteractions = -999;
  npuVertices = -999;


  //Reset trigger bits
  //  for (auto triggerName : _triggerNames) triggerBits[triggerName] = kFALSE;

  ////////////////////////////////////////////////////
  // Fill objects
  ////////////////////////////////////////////////////

  bool useObj;  // decide whether to transfer this into our list of objects 

  //No cuts on MC particles, yet
  if(DoTruth()){
  } //if DoTruth
  // ***************************************************
  // Then from either the reconstruction tree or fast sim
  if(DoFastSim()) {
  } // end of filling from fastsim tree
  // ***************************************************
  // Reconstructed
  if(DonanoAOD()) { //Fill from the nanoAODTree

    ///////////////////////////////////////////
    // Primary vertex information
    //////////////////////////////////////////
    
    pv_chi2 = _nanoAODTree->PV_chi2;
    pv_ndof = _nanoAODTree->PV_ndof;
    pv_npvs = _nanoAODTree->PV_npvs;
    pv_npvsGood = _nanoAODTree->PV_npvsGood;
    pv_score = _nanoAODTree->PV_score;
    pv_x = _nanoAODTree->PV_x;
    pv_y = _nanoAODTree->PV_y;
    pv_z = _nanoAODTree->PV_z;
    if (GetIsSimulation()){
      trueInteractions = _nanoAODTree->Pileup_nTrueInt;
      npuVertices = _nanoAODTree->Pileup_nPU;
    }
    ///////////////////////////////////////////
    // Fill trigger info                           
    ///////////////////////////////////////////

    //This is done automatically

    ///////////////////////////////////////////
    // Fill MET info                           
    ///////////////////////////////////////////

    met_pt = _nanoAODTree->MET_pt;
    met_phi = _nanoAODTree->MET_phi;
    
    //Get the phi-corrected MET
    
    std::pair<double,double> met_xyCorrectedPair = METXYCorr_Met_MetPhi(met_pt,met_phi,runNumber,yearName,GetIsSimulation(),pv_npvsGood,true,false);

    missingEtVec.SetPtEtaPhiM(met_pt,0.,met_phi,0.);
    missingEtVecs["met_uncorr"] = missingEtVec;

    missingEtVec.SetPtEtaPhiM(met_xyCorrectedPair.first,0.,met_xyCorrectedPair.second,0.);
    missingEtVecs["met"] = missingEtVec;

    sumETs["met"] = _nanoAODTree->MET_sumEt;
    met_significance = _nanoAODTree->MET_significance;

    missingEt = met_xyCorrectedPair.first;
    missingEx = missingEtVec.Px(); 
    missingEy = missingEtVec.Py(); 
    missingPhi = met_xyCorrectedPair.second;

    TLorentzVector tempMet;
    //Fill the various mets
    //Calo met
    if (DoManyMETs()){
      tempMet.SetPtEtaPhiM(_nanoAODTree->CaloMET_pt,0.,_nanoAODTree->CaloMET_phi,0.);
      missingEtVecs["calo"] = tempMet;
      sumETs["calo"] = _nanoAODTree->CaloMET_sumEt;
      
      //chs met
      tempMet.SetPtEtaPhiM(_nanoAODTree->ChsMET_pt,0.,_nanoAODTree->ChsMET_phi,0.);
      missingEtVecs["chs"] = tempMet;
      sumETs["chs"] = _nanoAODTree->ChsMET_sumEt;
      
      //puppi met
      tempMet.SetPtEtaPhiM(_nanoAODTree->PuppiMET_pt,0.,_nanoAODTree->PuppiMET_phi,0.);
      missingEtVecs["puppi_uncorr"] = tempMet;
      std::pair<double,double> puppiMET_phiCorrectedPair =  METXYCorr_Met_MetPhi(_nanoAODTree->PuppiMET_pt,_nanoAODTree->PuppiMET_phi,runNumber,yearName,GetIsSimulation(),pv_npvsGood,true,true);
      tempMet.SetPtEtaPhiM(puppiMET_phiCorrectedPair.first,0.,puppiMET_phiCorrectedPair.second,0.);
      missingEtVecs["puppi"] = tempMet;
      sumETs["puppi"] = _nanoAODTree->PuppiMET_sumEt;
      
      //raw met
      tempMet.SetPtEtaPhiM(_nanoAODTree->RawMET_pt,0.,_nanoAODTree->RawMET_phi,0.);
      missingEtVecs["raw"] = tempMet;
      sumETs["raw"] = _nanoAODTree->RawMET_sumEt;
      
      //raw puppi met
      tempMet.SetPtEtaPhiM(_nanoAODTree->RawPuppiMET_pt,0.,_nanoAODTree->RawPuppiMET_phi,0.);
      missingEtVecs["rawPuppi"] = tempMet;
      sumETs["rawPuppi"] = _nanoAODTree->RawPuppiMET_sumEt;
      
    }
    //met filters
    passesAllMETFilters = _nanoAODTree->Flag_METFilters; // This weas is apparenyl buggy, so apply them separately:
    passesMETFilters = (_nanoAODTree->Flag_goodVertices &&
		       _nanoAODTree->Flag_globalSuperTightHalo2016Filter &&
		       _nanoAODTree->Flag_HBHENoiseFilter &&
		       _nanoAODTree->Flag_HBHENoiseIsoFilter &&
		       _nanoAODTree->Flag_EcalDeadCellTriggerPrimitiveFilter &&
		       _nanoAODTree->Flag_BadPFMuonFilter &&
		       _nanoAODTree->Flag_BadPFMuonDzFilter &&
		       _nanoAODTree->Flag_eeBadScFilter &&
		       _nanoAODTree->Flag_ecalBadCalibFilter);
		       
    
    //For compatability reasons right now
    missingEtVec_xy.SetPtEtaPhiM(met_pt,0.,met_phi,0.);

    ///////////////////////////////////////////
    // Trigger objects                               
    ///////////////////////////////////////////
    for (Int_t io = 0; io < _nanoAODTree->nTrigObj; io++) {
      newTriggerObj.Clear();
      useObj = newTriggerObj.Fill(_nanoAODTree,io);
      if (useObj) triggerObjects.push_back(newTriggerObj);
    }


    /////////Gen///////
    if (isSimulation){
      for (Int_t io = 0; io < _nanoAODTree->nGenPart; io++) {
	//cout<<"Begin of gen fill event"<<endl;
	newGenPart.Clear();
	useObj=newGenPart.Fill(_nanoAODTree, io,isSimulation);
	genparts.push_back(newGenPart);
	//cout<<"End of gen fill event"<<endl;
      }
    }
      ////
      
    ///////////////////////////////////////////                          
    // Electrons                                                         
    ///////////////////////////////////////////                          
                                                                         
    // All electrons                                                     
    for(Int_t io = 0; io < _nanoAODTree->nElectron; io++) {   
      newElectron.Clear();                                               
      useObj=newElectron.Fill(_nanoAODTree, io,isSimulation);        
      if(useObj) {                                                       
        electrons.push_back(newElectron);                                
      }                                                                  
      if (newElectron.isTightEle()) {
        tightElectrons.push_back(newElectron);                           
      }                 
      if (newElectron.isMetEle()) {
	metElectrons.push_back(newElectron);
      }
      if(newElectron.isVetoEle()) {                                                       
        vetoElectrons.push_back(newElectron);                            
      }                                                                  
    } //for                                    

    ///////////////////////////////////////////                          
    // Muons                                                             
    ///////////////////////////////////////////                          
    // All muon                                                          
    for(Int_t io = 0;io < _nanoAODTree -> nMuon; io++) {         
      newMuon.Clear();                                                   
      useObj = newMuon.Fill(_nanoAODTree, io, isSimulation);         
      if(useObj) {                                                       
        muons.push_back(newMuon);                                        
      } // if useObj                                                     
      if(newMuon.isTightMu()) {                                                       
        tightMuons.push_back(newMuon);                                   
      } // if useObj      
      if(newMuon.isMetMu()) {
	metMuons.push_back(newMuon);
      }
      if(newMuon.isTriggerMatchedMu()) {
	triggerMatchMuons.push_back(newMuon);
      }
      if(newMuon.isVetoMu()) {                                                       
        vetoMuons.push_back(newMuon);                                    
      } // if useObj                                                     
                                                                         
    } //for muon loop                                                    

    /////////////////////////////////////////// 
    // Jets                                
    /////////////////////////////////////////// 

    
    for(Int_t io = 0;io < _nanoAODTree -> nJet; io++) {                                                                                      
      newJet.Clear();                                                                                                                                
                                                                                                                                                     
      //Fill the jet object                                                                               
      //The MET vector is not actually adjusted in the nanoAOD version of jets YET, because no smearing etc is done. But we pass it anyway so that in the future it will.
      //Here muonsToUsePtr and electronsToUsePtr are proxies for the tight electrons and muons for jet cleaning purposes. These can be adjusted elsewhere if we want to clean with, say, loose or unisolation leptons.
      useObj = newJet.Fill(*muonsToUsePtr, *electronsToUsePtr, _nanoAODTree, io, &missingEtVec, isSimulation);

      //Record every jet in this
      alljets.push_back(newJet);                                                                                                        
                                                                                                                                        
      if(useObj) {              
	//Only jets that pass event selections
        jets.push_back(newJet);                                                                                                         
                                              
	//Put tagged selected jets into the tagged list, and non-tagged selected jets into the unTagged list
        if(newJet.IsTagged()) taggedJets.push_back(newJet);                                                                             
        else unTaggedJets.push_back(newJet);                                                                                            
                                                                                                                                        
      } // if useObj                                                                 
      //If this is the first 
      if (io == 0) {                                                                   
	metVecsJESShifted.clear();                                                   
	jesShiftedJets.clear();                                                        
	for (int jesSyst = 0; jesSyst < newJet.GetNumberOfJESCorrections(); jesSyst++){
	  metVecsJESShifted.push_back(missingEtVec_xy);                                
	  std::vector<Jet> tempVec;                                                    
	  jesShiftedJets.push_back(tempVec);                                           
	}                                                                              
      }                                                                                


      //Now for each jet shift it by all of the JES corrections and append it to the shifted jet collections if it passes selections now
      //Currently for nanoAOD the GetNumberOfJESCorrections is zero, so this will do nothing, but leaving it here for when it works again.
      for (int jesSyst = 0; jesSyst < newJet.GetNumberOfJESCorrections(); jesSyst++){                                                   
        if (newJet.ShiftPtWithJESCorr(jesSyst,&(metVecsJESShifted[jesSyst]))) jesShiftedJets[jesSyst].push_back(newJet);                                                                                                                                                        
      }                                                                                                                                 
      
    }  //for jet loop
   
}
 else { //Fill using the classic BSM trees
    //    isSimulation = _eventTree->isSimulation; // Why is this always set to true?!?
    
    ///////////////////////////////////////////  
    // Number of vertices
    ///////////////////////////////////////////  
    
    int ncount = 0;
    //Fill pvtx information
    nPvtx = _eventTree->nBestVtx;
    trueInteractions = _eventTree->trueInteractions;
    npuVertices = _eventTree->npuVertices;
    
    
    ///////////////////////////////////////////
    // Fill MET info 
    ///////////////////////////////////////////
    missingEt = _eventTree->Met_type1PF_pt;
    missingEx = _eventTree->Met_type1PF_px;
    missingPhi = _eventTree->Met_type1PF_phi;
    missingEy = _eventTree->Met_type1PF_py;

    missingEt_xy = _eventTree->Met_type1PFxy_pt;
    missingEx_xy = _eventTree->Met_type1PFxy_px;
    missingPhi_xy = _eventTree->Met_type1PFxy_phi;
    missingEy_xy = _eventTree->Met_type1PFxy_py;

    passesMETFilters = _eventTree->Flag_METFilters;

    missingEtUpSF = _eventTree->Met_type1PF_pt/_eventTree->Met_type1PF_shiftedPtUp;
    missingEtDownSF = _eventTree->Met_type1PF_pt/_eventTree->Met_type1PF_shiftedPtDown;

    // Systematic variations on met to be re-calculated here.
    if (_metShift != 0){
      float oldEt = missingEt;
      missingEt = (_metShift == 1) ? _eventTree->Met_type1PF_shiftedPtUp : _eventTree->Met_type1PF_shiftedPtDown;

      float ratioSF = missingEt/oldEt;
      missingEx *= ratioSF;
      missingEy *= ratioSF;

      missingEt_xy *= ratioSF;
      missingEx_xy *= ratioSF;
      missingEy_xy *= ratioSF;
    }

    missingEtVec.SetPtEtaPhiE(missingEt,0.,missingPhi,missingEt);
    missingEtVec_xy.SetPtEtaPhiE(missingEt_xy,0.,missingPhi_xy,missingEt_xy);


    ///////////////////////////////////////////
    // Electrons
    ///////////////////////////////////////////

    // All electrons
    for(Int_t io = 0; io < _eventTree->patElectron_pt->size(); io++) {
      newElectron.Clear();
      useObj=newElectron.Fill(_eventTree, io,"All",isSimulation);
      if(useObj) { 
	electrons.push_back(newElectron);
      }  
    
      newElectron.Clear();
      useObj=newElectron.Fill(_eventTree,  io,"Tight",isSimulation);
      if(useObj) {
        tightElectrons.push_back(newElectron);
      }

      newElectron.Clear();
      useObj=newElectron.Fill(_eventTree,  io,"Veto",isSimulation);
      if(useObj) {
        vetoElectrons.push_back(newElectron);
      }

      newElectron.Clear();
      useObj=newElectron.Fill(_eventTree,  io,"UnIsolated",isSimulation);
      if(useObj) {
        unIsolatedElectrons.push_back(newElectron);
      }
    } //for
    ///////////////////////////////////////////
    // Muons
    ///////////////////////////////////////////  
    // All muon
    for(Int_t io = 0;io < _eventTree -> Muon_pt->size(); io++) {
      newMuon.Clear();
      useObj = newMuon.Fill(_eventTree, io,"All", isSimulation);
      if(useObj) {
	muons.push_back(newMuon);
      } // if useObj

      newMuon.Clear();
      useObj = newMuon.Fill(_eventTree, io,"Tight", isSimulation);
      if(useObj) {
        tightMuons.push_back(newMuon);
      } // if useObj

      newMuon.Clear();
      useObj = newMuon.Fill(_eventTree, io,"Veto", isSimulation);
      if(useObj) {
        vetoMuons.push_back(newMuon);
      } // if useObj

      newMuon.Clear();
      useObj = newMuon.Fill(_eventTree, io,"UnIsolated", isSimulation);
      if(useObj) {
	unIsolatedMuons.push_back(newMuon);
      } // if useObj

    } //for muon loop

    ///////////////////////////////////////////
    // Jets
    ///////////////////////////////////////////

    for(Int_t io = 0;io < _eventTree -> Jet_pt->size(); io++) {
      newJet.Clear();

      //Fill the jet object
      useObj = newJet.Fill(*muonsToUsePtr, *electronsToUsePtr, _eventTree, io, &missingEtVec_xy, isSimulation, &_resolution, &_resSFs, &_resFormula);

      
      missingEt = TMath::Sqrt(pow(missingEx,2) + pow(missingEy,2));//so MET gets JES adjustment toogEx=top_met.MET_ExMiss();
      /////////////////////////////////////
      //On the first jet in the event fill up the JES corrected met and jet lists
      if (io == 0) {
	metVecsJESShifted.clear();
	jesShiftedJets.clear();
	for (int jesSyst = 0; jesSyst < newJet.GetNumberOfJESCorrections(); jesSyst++){
	  metVecsJESShifted.push_back(missingEtVec_xy);
	  std::vector<Jet> tempVec;
	  jesShiftedJets.push_back(tempVec);
	}
      }
 
      alljets.push_back(newJet);
     
      if(useObj) {
	jets.push_back(newJet);
 
	if(newJet.IsTagged()) taggedJets.push_back(newJet);
	else unTaggedJets.push_back(newJet);
   
      } // if useObj
      //Now for each jet shift it by all of the JES corrections and append it to the shifted jet collections if it passes selections now
      for (int jesSyst = 0; jesSyst < newJet.GetNumberOfJESCorrections(); jesSyst++){
	if (newJet.ShiftPtWithJESCorr(jesSyst,&(metVecsJESShifted[jesSyst]))) jesShiftedJets[jesSyst].push_back(newJet);
	
      }
	  

    } //jets
  } // end of else: filling from reco tree
  
  //
  // Create a neutrino, including nu_pzn calculation
  Neutrino newNeutrino;
  newNeutrino.Fill(tightMuons, tightElectrons,missingEx,missingEy);
  neutrinos.push_back(newNeutrino);
  
  return 0;
  
} //ReadEvent

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
 * Bool_t EventContainer::CreateFinalState()                                  *
 *                                                                            *
 * Create final state particles                                               *
 * Require at least 2 jets and 1 isolated lepton.  Make cuts if necessary.    *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTRUE if requirements fulfilled                                    *
 ******************************************************************************/
 Bool_t EventContainer::CreateFinalState()
{

  ///////////////////////////////////////
  // Prepare final state particles
  //////////////////////////////////////
  // There should only be one lepton.  If this is the case use that lepton
  if(      1 == tightMuons.size() && 0 == tightElectrons.size()){
    finalLepton = tightMuons[0];
    finalLeptonPDGID = 13;
  }
  else if( 0 == tightMuons.size() && 1 == tightElectrons.size()){
    finalLepton = tightElectrons[0];
    finalLeptonPDGID = 11;
  }
  // If it is the case where there is more than 1 lepton 
  // If more than 1 muon and no electron use leading muon. If more than 1 electron and no muons use leading electron
  else if( tightMuons.size()     > 0 && 0 == tightElectrons.size()){
    finalLepton = tightMuons[0];
    finalLeptonPDGID = 13;
  }
  else if( tightElectrons.size() > 0 && 0 == tightMuons.size()){
     finalLepton = tightElectrons[0];
     finalLeptonPDGID = 11;
  }
  // If there is 1 or more muon and 1 or more electron use lepton with highest pT
  else if( tightMuons.size() > 0 && tightElectrons.size() > 0 ){
    if( tightMuons[0].Pt() > tightElectrons[0].Pt() ){
      finalLepton = tightMuons[0];
      finalLeptonPDGID = 13;
    }
    else{
      finalLepton = tightElectrons[0];
      finalLeptonPDGID = 11;
    }
  } //if
  // If there are no leptons set finalLepton to 0
  else if( 0 == tightMuons.size() && 0 == tightElectrons.size()){
    finalLepton = TLorentzVector(0.0,0.0,0.0,0.0);
    finalLeptonPDGID = 0;
    //    std::cout << "WARNING: <EventContainer::CreateFinalState>" << " O Muons and 0 Electrons in Final State." << std::endl;
  } //else
  // Default (just in case) is to set finalLepton to 0
  else{
    finalLepton = TLorentzVector(0.0,0.0,0.0,0.0);
    finalLeptonPDGID = 0;
  }

  ///////////////////////////////////////////////////////////
  // Reconstruct Neutrino (Pretend that momentum is unknown)
  //NEED TO USE MET, MEX, MEY information to get top eta, etc.
  ///////////////////////////////////////////////////////////
  // Initialize neutrino with negative px and negative py from lepton
  Double_t finalNeutrino_Px = -finalLepton.Px();
  Double_t finalNeutrino_Py = -finalLepton.Py();

  // Truth neutrino pz
  Double_t truthNeutrino_Pz;
  if(MCNeutrinos.size() > 0) truthNeutrino_Pz = MCNeutrinos[0].Pz();
  else truthNeutrino_Pz = 0.0;
			
  // Loop over jets and add negative of jet px and py
  finalJetVector = jets;
  for(Int_t i = 0; i < finalJetVector.size(); i++){
    finalNeutrino_Px += -finalJetVector[i].Px();
    finalNeutrino_Py += -finalJetVector[i].Py();
  } //for

  // Calculate Neutrino pz   
  // 6th parameter: Large negative scale factor means that no scale factor is used
  // 7th parameter: Truth neutrino - not needed for pz calculation (does nothing in Neutrino_Pz())
  Double_t scaleFactor = -999999.0;
  // Double_t finalNeutrino_Pz = Neutrino_Pz(finalNeutrino_Px, finalNeutrino_Py, finalLepton.Px(), finalLepton.Py(), finalLepton.Pz(), finalLepton.E(), scaleFactor, truthNeutrino_Pz);
 Double_t finalNeutrino_Pz = Neutrino_Pz(missingEx, missingEy, finalLepton.Px(), finalLepton.Py(), finalLepton.Pz(), finalLepton.E(), scaleFactor, truthNeutrino_Pz);
  // Calculate final Neutrino Energy
  //Double_t finalNeutrino_E = sqrt(finalNeutrino_Px * finalNeutrino_Px + finalNeutrino_Py * finalNeutrino_Py + finalNeutrino_Pz * finalNeutrino_Pz);
 Double_t finalNeutrino_E = sqrt((missingEx*missingEx) + (missingEy*missingEy) + (finalNeutrino_Pz * finalNeutrino_Pz));
  // Set 4 vector and Id
  // finalNeutrino.SetPxPyPzE(finalNeutrino_Px, finalNeutrino_Py, finalNeutrino_Pz, finalNeutrino_E);
 finalNeutrino.SetPxPyPzE(missingEx,missingEy, finalNeutrino_Pz, finalNeutrino_E);
  // finalNeutrino.SetPdgId(-12); // Not implemented for reconstructed particles

  //  nu_pz=Neutrino_Pz(missingEx, missingEy,leadlep_px,leadlep_py,leadlep_pz,leadlep_e,scf,1.0);
  //  nu_e=sqrt(missingEx*missingEx + missingEy*missingEy + nu_pz*nu_pz);
  //  SetPxPyPzE(missingEx,missingEy,nu_pz,nu_e);
  
  ///////////////////////////////////////////////////////////
  // Reconstruct W 
  ///////////////////////////////////////////////////////////
  finalW = finalNeutrino;
  finalW += finalLepton;
  
  ///////////////////////////////////////////////////////////
  // Reconstruct CM system (add lepton, neutrino, and jets)
  ///////////////////////////////////////////////////////////
  finalCM = finalLepton + finalNeutrino;
  for(Int_t i = 0; i < finalJetVector.size(); i++){
    finalCM = finalCM + finalJetVector[i];
  } //for

  return kTRUE;

} //CreateFinalState() 

/******************************************************************************
 * void EventContainer::MakeTopQuarks()                                       *
 *                                                                            *
 * Make top quarks from W and several jets                                    *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void EventContainer::MakeTopQuarks()
{
  //  std::cout<<"MAKING TOP QUARKS"<<endl;

  ///////////////////////////////////////////////////////////
  // Set everything related to Top Quark jets to 0 
  ///////////////////////////////////////////////////////////
  finalTop_leadingJet.SetPxPyPzE( 0.0, 0.0, 0.0, 0.0);
  finalTop_bestJet.SetPxPyPzE(    0.0, 0.0, 0.0, 0.0);
  finalTop_bestJet12.SetPxPyPzE(  0.0, 0.0, 0.0, 0.0);
  finalTop_bgJet.SetPxPyPzE(      0.0, 0.0, 0.0, 0.0);

  bgJet.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);

  bestJet12Ptr      = NULL;
  nonBestJet12Ptr   = NULL;
  bestJetPtr        = NULL;
  bgJetbJetPtr      = NULL;
  nonBgJetPtr       = NULL;
  bgJetGluonPtr     = NULL;
  taggedJetPtr      = NULL;
  untaggedPtJetPtr  = NULL;
  untaggedEtaJetPtr = NULL;
  forwardJetPtr     = NULL;
  leadingJetPtr     = NULL;

  ///////////////////////////////////////////////////////////
  // Set Jet pointers
  ///////////////////////////////////////////////////////////
  // Set tagger Jet ptr to leading b-tagged jet 
  if(taggedJets.size() > 0) taggedJetPtr = &(taggedJets[0]);
  // Set untaggedPtJetPtr to the leading un-tagged jet ordered in pT (is this correct?)
  if(unTaggedJets.size() > 0) untaggedPtJetPtr = &(unTaggedJets[0]);


  ///////////////////////////////////////////////////////////
  // Create Several Top Quarks
  // Make top quarks from the W and several possible jets. 
  ///////////////////////////////////////////////////////////
  iBestJet = 0;  // index of best jet
  
  // The leading jet from the Top is the W plus the leading jet
  if( finalJetVector.size() > 0){
    finalTop_leadingJet = finalW + finalJetVector[0];
  } //if
  else{
    //std::cout<<"No Jets in this event, particle reconstruction unclear"<<endl;
  } //else

  // Determine Best Jet (one which reconstructs closest to Target Top Mass )
  // findBestJet returns 0 if there are no jets in the event.  But in this case, finalJetVector is empty.  So check number of jets first.
  iBestJet = findBestJet(finalJetVector.size(), finalJetVector, finalW, _targetTopMass);
  if(finalJetVector.size() > 0){
    bestJetPtr = &(finalJetVector[iBestJet]);
    finalTop_bestJet = finalW + *bestJetPtr;
  }
  
  // Determine Best Jet from only the 2 leading jets
  Int_t nJets12 = finalJetVector.size() > 2 ? 2: finalJetVector.size();
  iBestJet = findBestJet(nJets12, finalJetVector, finalW, _targetTopMass);
  if(finalJetVector.size() > 0){
     bestJet12Ptr = &(finalJetVector[iBestJet]);
     finalTop_bestJet12 = finalW + *(bestJet12Ptr);
   }
 
  if(finalJetVector.size() >= 2) nonBestJet12Ptr = &(finalJetVector[1-iBestJet]);  // What if iBestJet > 1 ?
  else nonBestJet12Ptr = NULL;

  // Leading Jet
  if(finalJetVector.size() > 0) leadingJetPtr = &(finalJetVector[0]);


  //   //cout<<"add in a possible gluon"<<endl;
//   // add in the gluon if it is close to this jet
//   // and then determine the best jet

  std::vector<Jet> tmpJetVector;
  // Add gluon if it is close to the jet and then determine the best jet
  // If 2 or less jets use the bestJet12
  if((finalJetVector.size() <= 2)&&(finalJetVector.size() > 0)) {
    finalTop_bgJet = finalTop_bestJet12;
    bgJet          = *bestJet12Ptr;
    bgJetbJetPtr   = bestJet12Ptr;
    nonBgJetPtr    = nonBestJet12Ptr;
  } //if
  
  // For 3 or more jets, pair each of the leading 2 jets with the gluon and then find the best jet
  // Gluon is finalJetVector[2] (how do we know this)
  else if (finalJetVector.size() > 2){
    tmpJetVector = finalJetVector;
  
    // Not sure if this is correct
    // Old version used a 4-element array even if 4th jet did not exist
    // This version only fills 4th element if 4th jet exists
    for(Int_t i = 0; i < 2 ;i++) {
      tmpJetVector[i]   =  finalJetVector[i];
      if( tmpJetVector.size() > i+2 ) tmpJetVector[i+2] =  finalJetVector[i] + finalJetVector[2];
    } // for over jets 1,2

    // Find best jet out of 4
    // Replaced 4 by tmpJetVector.size()
    
    iBestJet       = findBestJet(tmpJetVector.size(), tmpJetVector, finalW, _targetTopMass);
    finalTop_bgJet = tmpJetVector[iBestJet] + finalW;
    bgJet          = tmpJetVector[iBestJet];
    
    // Determine if gluon needs to be included

    if(iBestJet >= 2) {
      bgJetbJetPtr  = &(finalJetVector[iBestJet-2]);
      nonBgJetPtr   = &(finalJetVector[1-(iBestJet-2)]);
      bgJetGluonPtr = &(finalJetVector[2]);
    }  //if
    else {
      bgJetbJetPtr  = &(finalJetVector[iBestJet]);
      nonBgJetPtr   = &(finalJetVector[1-(iBestJet)]);
      bgJetGluonPtr = 0;
    }
    
  } // else: try to include the gluon


  // Final top from tagged jet and W
  if(taggedJetPtr != NULL) {
    finalTop_taggedJet = *taggedJetPtr + finalW;
    
    // and the W using a top mass constraint:
    // the neutrino from the top mass constraint
    // also use a constraint on the top mass rather than the smaller of the two
    // solutions.

    // cout<<"True Top mass is "<<tlt.M()<<" e= "<<tlt.E()<<", px= "<<tlt.Px()<<", py= "<<tlt.Py()<<", pz= "<<tlt.Pz()<<endl;

    // Truth neutrino pz
    Double_t truthNeutrino_Pz;
    if(MCNeutrinos.size() > 0) truthNeutrino_Pz = MCNeutrinos[0].Pz();
    else truthNeutrino_Pz = 0.0;

    Double_t finalNeutrino_Pz = Neutrino_Pz_Top(finalNeutrino.Px(), finalNeutrino.Py(),
						finalLepton.Px(), finalLepton.Py(), finalLepton.Pz(), finalLepton.E(),
						taggedJetPtr -> Px(), taggedJetPtr -> Py(), taggedJetPtr -> Pz(), taggedJetPtr -> E(),
						truthNeutrino_Pz );
    
    
     Double_t finalNeutrino_E = sqrt( pow(finalNeutrino.Px(),2) + pow(finalNeutrino.Py(),2) + finalNeutrino_Pz*finalNeutrino_Pz);
     finalNeutrinoTopC.SetPxPyPzE(finalNeutrino.Px(), finalNeutrino.Py(), finalNeutrino_Pz, finalNeutrino_E);
     //     finalNeutrinoTopC.SetPdgId(-12);  // Does not work at the moment
    
     // The final lepton from top constraint on neutrino
     finalWTopC = finalNeutrinoTopC + finalLepton;
     finalTop_taggedJetTopC = *taggedJetPtr + finalWTopC;
  } //if

   else{
     finalTop_taggedJet.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
     finalTop_taggedJetTopC.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);

   } //else

  return;
} //MakeTopQuark

//A shortcut to getting a particular muon collection so that it doesn't have to be done in each cut routine.
//Will likely add similar options to jets, electrons, photons, etc.
std::vector<Muon> EventContainer::GetMuonCollection(TString muonType){

  std::vector<Muon> vec;
  if(      ! muonType.CompareTo("Veto") )         return vetoMuons;
  else if( ! muonType.CompareTo("Tight") )        return tightMuons;
  else if( ! muonType.CompareTo("PtEtaCut") )     return ptetaMuons;
  else if( ! muonType.CompareTo("UnIsolated") )   return unIsolatedMuons;
  else if( ! muonType.CompareTo("Isolated") )     return isolatedMuons;
  else if( ! muonType.CompareTo("MetMu") )        return metMuons;
  else if( ! muonType.CompareTo("TriggerMatch") ) return triggerMatchMuons;
  else if( ! muonType.CompareTo("All") )          return muons;
  else{
    std::cout << "ERROR in getting muon collection of type: " << muonType.Data() << std::endl;

  } //else                                                                                                                      
  return vec;
}

std::vector<Electron> EventContainer::GetElectronCollection(TString electronType){
  std::vector<Electron> vec;
  if(      ! electronType.CompareTo("Veto") )         return vetoElectrons;
  else if( ! electronType.CompareTo("Tight") )        return tightElectrons;
  else if( ! electronType.CompareTo("PtEtaCut") )     return ptetaElectrons;
  else if( ! electronType.CompareTo("UnIsolated") )   return unIsolatedElectrons;
  else if( ! electronType.CompareTo("Isolated") )     return isolatedElectrons;
  else if( ! electronType.CompareTo("MetEle") )       return metElectrons;
  else if( ! electronType.CompareTo("All") )          return electrons;
  else{
    std::cout << "ERROR in getting electron collection of type: " << electronType.Data() << std::endl;;;

  }
  return vec;
}

//A way to check that your choice of particle and collection is valid
Bool_t EventContainer::IsValidCollection(TString particleName, TString collectionName){

  Bool_t returnValue = kFALSE;

  if ( !particleName.CompareTo("Muon") ){
    if( ! ( collectionName.CompareTo("All")   && collectionName.CompareTo("UnIsolated") &&
	    collectionName.CompareTo("Tight") && collectionName.CompareTo("Veto") &&
	    collectionName.CompareTo("Trigger") && collectionName.CompareTo("MetMu") &&
	    collectionName.CompareTo("Isolated") && collectionName.CompareTo("PtEtaCut") ) ){
      returnValue = kTRUE;
    }
    else {
      std::cout << "Muon collection "<< collectionName << " not valid!" << std::endl;
      exit(8);
    }
  }
  else if (!particleName.CompareTo("Electron") ){
    if ( ! ( collectionName.CompareTo("All")        && collectionName.CompareTo("Veto")     &&
	     collectionName.CompareTo("Tight")      && collectionName.CompareTo("Isolated") &&
	     collectionName.CompareTo("UnIsolated") && collectionName.CompareTo("PtEtaCut") &&
	     collectionName.CompareTo("MetEle") ) ){
      returnValue = kTRUE;
    }
    else {
      std::cout << "Electron collection "<< collectionName << " not valid!" << std::endl;
      exit(8);
    }
  }

  return returnValue;

}

//This is a bit hacked together for now, will try and sort out soon.
void EventContainer::SetObjectIsTrigger(TString particleName, TString collectionName, Int_t index){
  if (particleName == "Muon"){
    if (collectionName == "MetMu") {
      metMuons[index].SetisTriggerMatchedMu();
    }
  }
  else if (particleName == "Electron"){
    if (collectionName == "MetEle") {
      metElectrons[index].SetisTriggerMatchedEle();
    }
  }
  //std::cout << "after "<< metMuons.size() << std::endl;
}

/****************************************************************************** 
 *  std::pair<double,double> EventContainer::METXYCorr_Met_MetPhi()           *
 *                                                                            * 
 * Get the xy phi correction to MET                                           * 
 * Method  is provided by JetMET                                              * 
 *                                                                            * 
 * Input:  None                                                               * 
 * Output: None                                                               * 
 ******************************************************************************/
std::pair<double,double> EventContainer::METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb, TString year, bool isMC, int npv, bool isUL, bool ispuppi){
  std::pair<double,double>  TheXYCorr_Met_MetPhi(uncormet,uncormet_phi);
  
  if(npv>100) npv=100;
  int runera =-1;
  bool usemetv2 =false;
  if(isMC && year == "2016" && !isUL) runera = y2016MC;
  else if(isMC && year == "2017" && !isUL) {runera = y2017MC; usemetv2 =true;}
  else if(isMC && year == "2018" && !isUL) runera = y2018MC;
  else if(isMC && year == "2016APV" && isUL) runera = yUL2016MCAPV;
  else if(isMC && year == "2016nonAPV" && isUL) runera = yUL2016MCnonAPV;
  else if(isMC && year == "2017" && isUL) runera = yUL2017MC;
  else if(isMC && year == "2018" && isUL) runera = yUL2018MC;
  
  
  else if(!isMC && runnb >=272007 && runnb <=275376 && !isUL) runera = y2016B;
  else if(!isMC && runnb >=275657 && runnb <=276283 && !isUL) runera = y2016C;
  else if(!isMC && runnb >=276315 && runnb <=276811 && !isUL) runera = y2016D;
  else if(!isMC && runnb >=276831 && runnb <=277420 && !isUL) runera = y2016E;
  else if(!isMC && runnb >=277772 && runnb <=278808 && !isUL) runera = y2016F;
  else if(!isMC && runnb >=278820 && runnb <=280385 && !isUL) runera = y2016G;
  else if(!isMC && runnb >=280919 && runnb <=284044 && !isUL) runera = y2016H;
  
  else if(!isMC && runnb >=297020 && runnb <=299329 && !isUL){ runera = y2017B; usemetv2 =true;}
  else if(!isMC && runnb >=299337 && runnb <=302029 && !isUL){ runera = y2017C; usemetv2 =true;}
  else if(!isMC && runnb >=302030 && runnb <=303434 && !isUL){ runera = y2017D; usemetv2 =true;}
  else if(!isMC && runnb >=303435 && runnb <=304826 && !isUL){ runera = y2017E; usemetv2 =true;}
  else if(!isMC && runnb >=304911 && runnb <=306462 && !isUL){ runera = y2017F; usemetv2 =true;}
  
  else if(!isMC && runnb >=315252 && runnb <=316995 && !isUL) runera = y2018A;
  else if(!isMC && runnb >=316998 && runnb <=319312 && !isUL) runera = y2018B;
  else if(!isMC && runnb >=319313 && runnb <=320393 && !isUL) runera = y2018C;
  else if(!isMC && runnb >=320394 && runnb <=325273 && !isUL) runera = y2018D;

  else if(!isMC && runnb >=315252 && runnb <=316995 && isUL) runera = yUL2018A;
  else if(!isMC && runnb >=316998 && runnb <=319312 && isUL) runera = yUL2018B;
  else if(!isMC && runnb >=319313 && runnb <=320393 && isUL) runera = yUL2018C;
  else if(!isMC && runnb >=320394 && runnb <=325273 && isUL) runera = yUL2018D;

  else if(!isMC && runnb >=297020 && runnb <=299329 && isUL){ runera = yUL2017B; usemetv2 =false;}
  else if(!isMC && runnb >=299337 && runnb <=302029 && isUL){ runera = yUL2017C; usemetv2 =false;}
  else if(!isMC && runnb >=302030 && runnb <=303434 && isUL){ runera = yUL2017D; usemetv2 =false;}
  else if(!isMC && runnb >=303435 && runnb <=304826 && isUL){ runera = yUL2017E; usemetv2 =false;}
  else if(!isMC && runnb >=304911 && runnb <=306462 && isUL){ runera = yUL2017F; usemetv2 =false;}

  else if(!isMC && runnb >=272007 && runnb <=275376 && isUL) runera = yUL2016B;
  else if(!isMC && runnb >=275657 && runnb <=276283 && isUL) runera = yUL2016C;
  else if(!isMC && runnb >=276315 && runnb <=276811 && isUL) runera = yUL2016D;
  else if(!isMC && runnb >=276831 && runnb <=277420 && isUL) runera = yUL2016E;
  else if(!isMC && ((runnb >=277772 && runnb <=278768) || runnb==278770) && isUL) runera = yUL2016F;
  else if(!isMC && ((runnb >=278801 && runnb <=278808) || runnb==278769) && isUL) runera = yUL2016Flate;
  else if(!isMC && runnb >=278820 && runnb <=280385 && isUL) runera = yUL2016G;
  else if(!isMC && runnb >=280919 && runnb <=284044 && isUL) runera = yUL2016H;


  else {
    //Couldn't find data/MC era => no correction applied
    return TheXYCorr_Met_MetPhi;
  }
  
  double METxcorr(0.),METycorr(0.);

  if(!usemetv2){//Current recommendation for 2016 and 2018
    if(!ispuppi){
      if(runera==y2016B) METxcorr = -(-0.0478335*npv -0.108032);
      if(runera==y2016B) METycorr = -(0.125148*npv +0.355672);
      if(runera==y2016C) METxcorr = -(-0.0916985*npv +0.393247);
      if(runera==y2016C) METycorr = -(0.151445*npv +0.114491);
      if(runera==y2016D) METxcorr = -(-0.0581169*npv +0.567316);
      if(runera==y2016D) METycorr = -(0.147549*npv +0.403088);
      if(runera==y2016E) METxcorr = -(-0.065622*npv +0.536856);
      if(runera==y2016E) METycorr = -(0.188532*npv +0.495346);
      if(runera==y2016F) METxcorr = -(-0.0313322*npv +0.39866);
      if(runera==y2016F) METycorr = -(0.16081*npv +0.960177);
      if(runera==y2016G) METxcorr = -(0.040803*npv -0.290384);
      if(runera==y2016G) METycorr = -(0.0961935*npv +0.666096);
      if(runera==y2016H) METxcorr = -(0.0330868*npv -0.209534);
      if(runera==y2016H) METycorr = -(0.141513*npv +0.816732);
      if(runera==y2017B) METxcorr = -(-0.259456*npv +1.95372);
      if(runera==y2017B) METycorr = -(0.353928*npv -2.46685);
      if(runera==y2017C) METxcorr = -(-0.232763*npv +1.08318);
      if(runera==y2017C) METycorr = -(0.257719*npv -1.1745);
      if(runera==y2017D) METxcorr = -(-0.238067*npv +1.80541);
      if(runera==y2017D) METycorr = -(0.235989*npv -1.44354);
      if(runera==y2017E) METxcorr = -(-0.212352*npv +1.851);
      if(runera==y2017E) METycorr = -(0.157759*npv -0.478139);
      if(runera==y2017F) METxcorr = -(-0.232733*npv +2.24134);
      if(runera==y2017F) METycorr = -(0.213341*npv +0.684588);
      if(runera==y2018A) METxcorr = -(0.362865*npv -1.94505);
      if(runera==y2018A) METycorr = -(0.0709085*npv -0.307365);
      if(runera==y2018B) METxcorr = -(0.492083*npv -2.93552);
      if(runera==y2018B) METycorr = -(0.17874*npv -0.786844);
      if(runera==y2018C) METxcorr = -(0.521349*npv -1.44544);
      if(runera==y2018C) METycorr = -(0.118956*npv -1.96434);
      if(runera==y2018D) METxcorr = -(0.531151*npv -1.37568);
      if(runera==y2018D) METycorr = -(0.0884639*npv -1.57089);
      if(runera==y2016MC) METxcorr = -(-0.195191*npv -0.170948);
      if(runera==y2016MC) METycorr = -(-0.0311891*npv +0.787627);
      if(runera==y2017MC) METxcorr = -(-0.217714*npv +0.493361);
      if(runera==y2017MC) METycorr = -(0.177058*npv -0.336648);
      if(runera==y2018MC) METxcorr = -(0.296713*npv -0.141506);
      if(runera==y2018MC) METycorr = -(0.115685*npv +0.0128193);
    
      //UL2017
      if(runera==yUL2017B) METxcorr = -(-0.211161*npv +0.419333);
      if(runera==yUL2017B) METycorr = -(0.251789*npv +-1.28089);
      if(runera==yUL2017C) METxcorr = -(-0.185184*npv +-0.164009);
      if(runera==yUL2017C) METycorr = -(0.200941*npv +-0.56853);
      if(runera==yUL2017D) METxcorr = -(-0.201606*npv +0.426502);
      if(runera==yUL2017D) METycorr = -(0.188208*npv +-0.58313);
      if(runera==yUL2017E) METxcorr = -(-0.162472*npv +0.176329);
      if(runera==yUL2017E) METycorr = -(0.138076*npv +-0.250239);
      if(runera==yUL2017F) METxcorr = -(-0.210639*npv +0.72934);
      if(runera==yUL2017F) METycorr = -(0.198626*npv +1.028);
      if(runera==yUL2017MC) METxcorr = -(-0.300155*npv +1.90608);
      if(runera==yUL2017MC) METycorr = -(0.300213*npv +-2.02232);

      //UL2018
      if(runera==yUL2018A) METxcorr = -(0.263733*npv +-1.91115);
      if(runera==yUL2018A) METycorr = -(0.0431304*npv +-0.112043);
      if(runera==yUL2018B) METxcorr = -(0.400466*npv +-3.05914);
      if(runera==yUL2018B) METycorr = -(0.146125*npv +-0.533233);
      if(runera==yUL2018C) METxcorr = -(0.430911*npv +-1.42865);
      if(runera==yUL2018C) METycorr = -(0.0620083*npv +-1.46021);
      if(runera==yUL2018D) METxcorr = -(0.457327*npv +-1.56856);
      if(runera==yUL2018D) METycorr = -(0.0684071*npv +-0.928372);
      if(runera==yUL2018MC) METxcorr = -(0.183518*npv +0.546754);
      if(runera==yUL2018MC) METycorr = -(0.192263*npv +-0.42121);

      //UL2016
      if(runera==yUL2016B) METxcorr = -(-0.0214894*npv +-0.188255);
      if(runera==yUL2016B) METycorr = -(0.0876624*npv +0.812885);
      if(runera==yUL2016C) METxcorr = -(-0.032209*npv +0.067288);
      if(runera==yUL2016C) METycorr = -(0.113917*npv +0.743906);
      if(runera==yUL2016D) METxcorr = -(-0.0293663*npv +0.21106);
      if(runera==yUL2016D) METycorr = -(0.11331*npv +0.815787);
      if(runera==yUL2016E) METxcorr = -(-0.0132046*npv +0.20073);
      if(runera==yUL2016E) METycorr = -(0.134809*npv +0.679068);
      if(runera==yUL2016F) METxcorr = -(-0.0543566*npv +0.816597);
      if(runera==yUL2016F) METycorr = -(0.114225*npv +1.17266);
      if(runera==yUL2016Flate) METxcorr = -(0.134616*npv +-0.89965);
      if(runera==yUL2016Flate) METycorr = -(0.0397736*npv +1.0385);
      if(runera==yUL2016G) METxcorr = -(0.121809*npv +-0.584893);
      if(runera==yUL2016G) METycorr = -(0.0558974*npv +0.891234);
      if(runera==yUL2016H) METxcorr = -(0.0868828*npv +-0.703489);
      if(runera==yUL2016H) METycorr = -(0.0888774*npv +0.902632);
      if(runera==yUL2016MCnonAPV) METxcorr = -(-0.153497*npv +-0.231751);
      if(runera==yUL2016MCnonAPV) METycorr = -(0.00731978*npv +0.243323);
      if(runera==yUL2016MCAPV) METxcorr = -(-0.188743*npv +0.136539);
      if(runera==yUL2016MCAPV) METycorr = -(0.0127927*npv +0.117747);



    }
    
    //UL2017Puppi
    if(ispuppi){
      if(runera==yUL2017B) METxcorr = -(-0.00382117*npv +-0.666228);
      if(runera==yUL2017B) METycorr = -(0.0109034*npv +0.172188);
      if(runera==yUL2017C) METxcorr = -(-0.00110699*npv +-0.747643);
      if(runera==yUL2017C) METycorr = -(-0.0012184*npv +0.303817);
      if(runera==yUL2017D) METxcorr = -(-0.00141442*npv +-0.721382);
      if(runera==yUL2017D) METycorr = -(-0.0011873*npv +0.21646);
      if(runera==yUL2017E) METxcorr = -(0.00593859*npv +-0.851999);
      if(runera==yUL2017E) METycorr = -(-0.00754254*npv +0.245956);
      if(runera==yUL2017F) METxcorr = -(0.00765682*npv +-0.945001);
      if(runera==yUL2017F) METycorr = -(-0.0154974*npv +0.804176);
      if(runera==yUL2017MC) METxcorr = -(-0.0102265*npv +-0.446416);
      if(runera==yUL2017MC) METycorr = -(0.0198663*npv +0.243182);

      //UL2018Puppi
      if(runera==yUL2018A) METxcorr = -(-0.0073377*npv +0.0250294);
      if(runera==yUL2018A) METycorr = -(-0.000406059*npv +0.0417346);
      if(runera==yUL2018B) METxcorr = -(0.00434261*npv +0.00892927);
      if(runera==yUL2018B) METycorr = -(0.00234695*npv +0.20381);
      if(runera==yUL2018C) METxcorr = -(0.00198311*npv +0.37026);
      if(runera==yUL2018C) METycorr = -(-0.016127*npv +0.402029);
      if(runera==yUL2018D) METxcorr = -(0.00220647*npv +0.378141);
      if(runera==yUL2018D) METycorr = -(-0.0160244*npv +0.471053);
      if(runera==yUL2018MC) METxcorr = -(-0.0214557*npv +0.969428);
      if(runera==yUL2018MC) METycorr = -(0.0167134*npv +0.199296);

      //UL2016Puppi
      if(runera==yUL2016B) METxcorr = -(-0.00109025*npv +-0.338093);
      if(runera==yUL2016B) METycorr = -(-0.00356058*npv +0.128407);
      if(runera==yUL2016C) METxcorr = -(-0.00271913*npv +-0.342268);
      if(runera==yUL2016C) METycorr = -(0.00187386*npv +0.104);
      if(runera==yUL2016D) METxcorr = -(-0.00254194*npv +-0.305264);
      if(runera==yUL2016D) METycorr = -(-0.00177408*npv +0.164639);
      if(runera==yUL2016E) METxcorr = -(-0.00358835*npv +-0.225435);
      if(runera==yUL2016E) METycorr = -(-0.000444268*npv +0.180479);
      if(runera==yUL2016F) METxcorr = -(0.0056759*npv +-0.454101);
      if(runera==yUL2016F) METycorr = -(-0.00962707*npv +0.35731);
      if(runera==yUL2016Flate) METxcorr = -(0.0234421*npv +-0.371298);
      if(runera==yUL2016Flate) METycorr = -(-0.00997438*npv +0.0809178);
      if(runera==yUL2016G) METxcorr = -(0.0182134*npv +-0.335786);
      if(runera==yUL2016G) METycorr = -(-0.0063338*npv +0.093349);
      if(runera==yUL2016H) METxcorr = -(0.015702*npv +-0.340832);
      if(runera==yUL2016H) METycorr = -(-0.00544957*npv +0.199093);
      if(runera==yUL2016MCnonAPV) METxcorr = -(-0.0058341*npv +-0.395049);
      if(runera==yUL2016MCnonAPV) METycorr = -(0.00971595*npv +-0.101288);
      if(runera==yUL2016MCAPV) METxcorr = -(-0.0060447*npv +-0.4183);
      if(runera==yUL2016MCAPV) METycorr = -(0.008331*npv +-0.0990046);


    }


  }
  else {//these are the corrections for v2 MET recipe (currently recommended for 2017)
    if(runera==y2016B) METxcorr = -(-0.0374977*npv +0.00488262);
    if(runera==y2016B) METycorr = -(0.107373*npv +-0.00732239);
    if(runera==y2016C) METxcorr = -(-0.0832562*npv +0.550742);
    if(runera==y2016C) METycorr = -(0.142469*npv +-0.153718);
    if(runera==y2016D) METxcorr = -(-0.0400931*npv +0.753734);
    if(runera==y2016D) METycorr = -(0.127154*npv +0.0175228);
    if(runera==y2016E) METxcorr = -(-0.0409231*npv +0.755128);
    if(runera==y2016E) METycorr = -(0.168407*npv +0.126755);
    if(runera==y2016F) METxcorr = -(-0.0161259*npv +0.516919);
    if(runera==y2016F) METycorr = -(0.141176*npv +0.544062);
    if(runera==y2016G) METxcorr = -(0.0583851*npv +-0.0987447);
    if(runera==y2016G) METycorr = -(0.0641427*npv +0.319112);
    if(runera==y2016H) METxcorr = -(0.0706267*npv +-0.13118);
    if(runera==y2016H) METycorr = -(0.127481*npv +0.370786);
    if(runera==y2017B) METxcorr = -(-0.19563*npv +1.51859);
    if(runera==y2017B) METycorr = -(0.306987*npv +-1.84713);
    if(runera==y2017C) METxcorr = -(-0.161661*npv +0.589933);
    if(runera==y2017C) METycorr = -(0.233569*npv +-0.995546);
    if(runera==y2017D) METxcorr = -(-0.180911*npv +1.23553);
    if(runera==y2017D) METycorr = -(0.240155*npv +-1.27449);
    if(runera==y2017E) METxcorr = -(-0.149494*npv +0.901305);
    if(runera==y2017E) METycorr = -(0.178212*npv +-0.535537);
    if(runera==y2017F) METxcorr = -(-0.165154*npv +1.02018);
    if(runera==y2017F) METycorr = -(0.253794*npv +0.75776);
    if(runera==y2018A) METxcorr = -(0.362642*npv +-1.55094);
    if(runera==y2018A) METycorr = -(0.0737842*npv +-0.677209);
    if(runera==y2018B) METxcorr = -(0.485614*npv +-2.45706);
    if(runera==y2018B) METycorr = -(0.181619*npv +-1.00636);
    if(runera==y2018C) METxcorr = -(0.503638*npv +-1.01281);
    if(runera==y2018C) METycorr = -(0.147811*npv +-1.48941);
    if(runera==y2018D) METxcorr = -(0.520265*npv +-1.20322);
    if(runera==y2018D) METycorr = -(0.143919*npv +-0.979328);
    if(runera==y2016MC) METxcorr = -(-0.159469*npv +-0.407022);
    if(runera==y2016MC) METycorr = -(-0.0405812*npv +0.570415);
    if(runera==y2017MC) METxcorr = -(-0.182569*npv +0.276542);
    if(runera==y2017MC) METycorr = -(0.155652*npv +-0.417633);
    if(runera==y2018MC) METxcorr = -(0.299448*npv +-0.13866);
    if(runera==y2018MC) METycorr = -(0.118785*npv +0.0889588);




    
  }

  double CorrectedMET_x = uncormet *cos( uncormet_phi)+METxcorr;
  double CorrectedMET_y = uncormet *sin( uncormet_phi)+METycorr;

  double CorrectedMET = sqrt(CorrectedMET_x*CorrectedMET_x+CorrectedMET_y*CorrectedMET_y);
  double CorrectedMETPhi;
  if(CorrectedMET_x==0 && CorrectedMET_y>0) CorrectedMETPhi = TMath::Pi();
  else if(CorrectedMET_x==0 && CorrectedMET_y<0 )CorrectedMETPhi = -TMath::Pi();
  else if(CorrectedMET_x >0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x);
  else if(CorrectedMET_x <0&& CorrectedMET_y>0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) + TMath::Pi();
  else if(CorrectedMET_x <0&& CorrectedMET_y<0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) - TMath::Pi();
  else CorrectedMETPhi =0;

  TheXYCorr_Met_MetPhi.first= CorrectedMET;
  TheXYCorr_Met_MetPhi.second= CorrectedMETPhi;
  return TheXYCorr_Met_MetPhi;
}
