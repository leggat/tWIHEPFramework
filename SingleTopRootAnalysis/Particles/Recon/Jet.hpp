/******************************************************************************
 * Jet.hpp                                                                    *
 *                                                                            *
 * Store information about final state jets                                   *
 *                                                                            *
 * Derived from Particle class                                                *
 *                                                                            *
 * Public Member Functions of Jet class                                       *
 *    Jet()                              -- Default Constructor               *
 *    Jet()                              -- Parameterized Constructor         *
 *    ~Jet()                             -- Destructor                        *
 *    Clear()                            -- Set contents to default           *
 *    Fill                               -- Fill jet from event tree          *
 *    FillFastSim                        -- Fill jet from FastSim tree        *
 *    SetEtM0                            -- Set ET for EM layer 0             *
 *    GetEtM0                            -- Set ET for EM layer 0             *
 *    EtEM0                              -- Set ET for EM layer 0             *
 *    ApplyJESCorrection                 -- Shift jet energy by some amount   *
 *    operator+=                         -- Overload +=                       *
 *    operator+                          -- Overload +                        *
 *    operator=                          -- Overload = Particle               *
 *    operator=                          -- Overload = const Jet              *
 *    operator=                          -- Overload =                        *
 *                                                                            *
 * Private Data Members of Jet class                                          *
 *                                                                            *
 * History                                                                    *
 *      17 June 2015 - Created by Huaqiao ZHANG                               *
 *****************************************************************************/

#ifndef jet_h
#define jet_h

#include "SingleTopRootAnalysis/Particles/Truth/MCJet.hpp"
#include "SingleTopRootAnalysis/Particles/Truth/MCTau.hpp"
#include "SingleTopRootAnalysis/Particles/Recon/Particle.hpp"
//#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"
#include "SingleTopRootAnalysis/Particles/Recon/Electron.hpp"
#include "SingleTopRootAnalysis/Particles/Recon/Muon.hpp"
//  #include "SingleTopRootAnalysis/Trees/EventTree.hpp"
#include "SingleTopRootAnalysis/Trees/FastSimTree.hpp"
//#include "SingleTopRootAnalysis/Base/Dictionary/MultijetJESUncertaintyProvider.hpp"
//#include "SingleTopRootAnalysis/Base/Dictionary/JetEfficiencyEstimator.hpp"
//#include "SingleTopRootAnalysis/Base/Dictionary/JERProvider.hpp"
//#include "SingleTopRootAnalysis/Base/Dictionary/JetResolution.hpp"
//#include "SingleTopRootAnalysis/Base/Dictionary/JetResolutionObject.hpp"
//
#include <TString.h>
#include <TFormula.h>

//using namespace Analysis;

class Electron;
class Muon;

class Jet: public Particle
{
 public:
  // Default Constructor
  Jet();

  // Copy Constructor
  Jet(const Jet& other);

  // Construtor using particle
  Jet(const Particle& other);

  // Destructor
  ~Jet();

  // Set all contents to their defaults
  inline void Clear() { Particle::Clear(); _numberOfConstituents=0; _chargedMultiplicity=0;  _bDiscriminator = -999.0; _pileupId = 0.0; _mass = 0.0; _uncorrPt = 0.0; _neutralHadEnergyFraction=0.0; _neutralEmEmEnergyFraction = 0.0; _chargedHadronEnergyFraction =0.0; _chargedEmEnergyFraction=0.0; _muonEnergyFraction=0.0; _electronEnergy=0.0; _photonEnergy=0.0; _tagged = kFALSE; _hadronFlavour=-1;
}

  void SetCuts(TEnv* config);

  // Fill the jet. Overloaded functions for nanoAOD and BSM trees
  Bool_t Fill( std::vector<Muon>& selectedMuons, std::vector<Electron>& selectedElectrons, EventTree *evtr, Int_t iE, TLorentzVector * met, bool isMC, std::vector<std::vector<std::string> > * resolution, std::vector<std::vector<std::string> > * resSFs, TString * resFormula);
  Bool_t Fill( std::vector<Muon>& selectedMuons, std::vector<Electron>& selectedElectrons, nanoAODTree *evtr, Int_t iE, TLorentzVector * met, bool isMC,std::vector<std::vector<std::string> > * resolution, std::vector<std::vector<std::string> > * resSFs, TString * resFormula, std::vector<std::vector<std::string> > * jesUncs);
  //  Bool_t Fill( double myJESCorr, double myJERCorr, std::vector<Electron>& selectedElectrons, EventTree *evtr, Int_t iE);
  // Also fill from FastSim tree:
  Bool_t FillFastSim( std::vector<MCJet>& MCBJets, std::vector<MCJet>& MCCJets, std::vector<MCTau>& MCTaus,  std::vector<Electron>& electrons, FastSimTree *tr,Int_t iE,TEnv *config,const TString& tagName="default", Double_t btagCut = 999, Double_t mistagCut = 999, Double_t eshift = 0 );
 
  inline void SetnumberOfConstituents(Int_t numberOfConstituents){_numberOfConstituents = numberOfConstituents;};
  inline Int_t GetnumberOfConstituents() const {return _numberOfConstituents;};
  inline Int_t numberOfConstituents() const {return _numberOfConstituents;};

  inline void SethadronFlavour(Int_t hadronFlavour){_hadronFlavour = hadronFlavour;};
  inline Int_t GethadronFlavour() const {return _hadronFlavour;};
  inline Int_t hadronFlavour() const {return _hadronFlavour;};

  inline void SetchargedMultiplicity(Int_t chargedMultiplicity){_chargedMultiplicity = chargedMultiplicity;};
  inline Int_t GetchargedMultiplicity() const {return _chargedMultiplicity;};
  inline Int_t chargedMultiplicity() const {return _chargedMultiplicity;};

  inline void SetbDiscriminator(Double_t bDiscriminator){_bDiscriminator = bDiscriminator;};
  inline Double_t GetbDiscriminator() const {return _bDiscriminator;};
  inline Double_t bDiscriminator() const {return _bDiscriminator;};

  inline void SetbtagCSVV2(Double_t btagCSVV2){_btagCSVV2 = btagCSVV2;};
  inline Double_t GetbtagCSVV2() const {return _btagCSVV2;};
  inline Double_t btagCSVV2() const {return _btagCSVV2;};

  inline void SetbtagCMVA(Double_t btagCMVA){_btagCMVA = btagCMVA;};
  inline Double_t GetbtagCMVA() const {return _btagCMVA;};
  inline Double_t btagCMVA() const {return _btagCMVA;};

  inline void SetbtagDeepB(Double_t btagDeepB){_btagDeepB = btagDeepB;};
  inline Double_t GetbtagDeepB() const {return _btagDeepB;};
  inline Double_t btagDeepB() const {return _btagDeepB;};

  inline void SetbtagDeepC(Double_t btagDeepC){_btagDeepC = btagDeepC;};
  inline Double_t GetbtagDeepC() const {return _btagDeepC;};
  inline Double_t btagDeepC() const {return _btagDeepC;};

  inline void SetpileupId(Double_t pileupId){_pileupId = pileupId;};
  inline Double_t GetpileupId() const {return _pileupId;};
  inline Double_t pileupId() const {return _pileupId;};

  inline void Setmass(Double_t mass){_mass = mass;};
  inline Double_t Getmass() const {return _mass;};
  inline Double_t mass() const {return _mass;};

  inline void SetneutralHadEnergyFraction(Double_t neutralHadEnergyFraction){_neutralHadEnergyFraction = neutralHadEnergyFraction;};
  inline Double_t GetneutralHadEnergyFraction() const {return _neutralHadEnergyFraction;};
  inline Double_t neutralHadEnergyFraction() const {return _neutralHadEnergyFraction;};

  inline void SetneutralEmEmEnergyFraction(Double_t neutralEmEmEnergyFraction){_neutralEmEmEnergyFraction = neutralEmEmEnergyFraction;};
  inline Double_t GetneutralEmEmEnergyFraction() const {return _neutralEmEmEnergyFraction;};
  inline Double_t neutralEmEmEnergyFraction() const {return _neutralEmEmEnergyFraction;};

  inline void SetchargedHadronEnergyFraction(Double_t chargedHadronEnergyFraction){_chargedHadronEnergyFraction = chargedHadronEnergyFraction;};
  inline Double_t GetchargedHadronEnergyFraction() const {return _chargedHadronEnergyFraction;};
  inline Double_t chargedHadronEnergyFraction() const {return _chargedHadronEnergyFraction;};

  inline void SetchargedEmEnergyFraction(Double_t chargedEmEnergyFraction){_chargedEmEnergyFraction = chargedEmEnergyFraction;};
  inline Double_t GetchargedEmEnergyFraction() const {return _chargedEmEnergyFraction;};
  inline Double_t chargedEmEnergyFraction() const {return _chargedEmEnergyFraction;};

  inline void SetmuonEnergyFraction(Double_t muonEnergyFraction){_muonEnergyFraction = muonEnergyFraction;};
  inline Double_t GetmuonEnergyFraction() const {return _muonEnergyFraction;};
  inline Double_t muonEnergyFraction() const {return _muonEnergyFraction;};

  inline void SetelectronEnergy(Double_t electronEnergy){_electronEnergy = electronEnergy;};
  inline Double_t GetelectronEnergy() const {return _electronEnergy;};
  inline Double_t electronEnergy() const {return _electronEnergy;};

  inline void SetphotonEnergy(Double_t photonEnergy){_photonEnergy = photonEnergy;};
  inline Double_t GetphotonEnergy() const {return _photonEnergy;};
  inline Double_t photonEnergy() const {return _photonEnergy;};

  inline void SetuncorrPt(Double_t uncorrPt){_uncorrPt = uncorrPt;};
  inline Double_t GetuncorrPt() const {return _uncorrPt;};
  inline Double_t uncorrPt() const {return _uncorrPt;};

  inline void SetTagged(Int_t isTagged){_tagged = isTagged;};
  inline Int_t IsTagged() const {return _tagged;};
  inline Int_t tagged() const {return _tagged;};

  inline void SetPrefireVeto(Int_t prefireVeto){_prefireVeto = prefireVeto;};
  inline Int_t IsPrefireVeto() const {return _prefireVeto;};
  inline Int_t prefireVeto() const {return _prefireVeto;};

  inline void SetClosestLep(Double_t closestLep){_closestLep = closestLep;};
  inline Double_t GetClosestLep() const {return _closestLep;};
  inline Double_t closestLep() const {return _closestLep;};

  inline Int_t GetNumberOfJESCorrections() const {return _jesShifts.size();};

  inline void SetNominalPx(Double_t nomPx){_nominalPx = nomPx;};
  inline Double_t GetNominalPx() const {return _nominalPx;};
  inline Double_t nominalPx() const {return _nominalPx;};

  inline void SetNominalPy(Double_t nomPy){_nominalPy = nomPy;};
  inline Double_t GetNominalPy() const {return _nominalPy;};
  inline Double_t nominalPy() const {return _nominalPy;};

  inline void SetNominalPz(Double_t nomPz){_nominalPz = nomPz;};
  inline Double_t GetNominalPz() const {return _nominalPz;};
  inline Double_t nominalPz() const {return _nominalPz;};

  inline void SetNominalE(Double_t nomE){_nominalE = nomE;};
  inline Double_t GetNominalE() const {return _nominalE;};
  inline Double_t nominalE() const {return _nominalE;};

  inline void SetgenPt(Double_t genPt){_genPt = genPt;};
  inline Double_t GetgenPt() const {return _genPt;};
  inline Double_t genPt() const {return _genPt;};

  inline void SetjetID(Double_t jetID){_jetID = jetID;};
  inline Double_t GetjetID() const {return _jetID;};
  inline Double_t jetID() const {return _jetID;};

  inline void SetjerSF(Double_t jerSF){_jerSF = jerSF;};
  inline Double_t GetjerSF() const {return _jerSF;};
  inline Double_t jerSF() const {return _jerSF;};

  inline void SetjesSF(Double_t jesSF){_jesSF = jesSF;};
  inline Double_t GetjesSF() const {return _jesSF;};
  inline Double_t jesSF() const {return _jesSF;};

  inline void SetsmearFactor(Double_t smearFactor){_smearFactor = smearFactor;};
  inline Double_t GetsmearFactor() const {return _smearFactor;};
  inline Double_t smearFactor() const {return _smearFactor;};

  Bool_t ShiftPtWithJESCorr(Int_t jesShiftInd, TLorentzVector * met);

  // Overloaded Operators
  // +=
  Jet& operator+=(const Jet& other);
  // +
  Jet operator+(const Jet& other);
  // = Jet const
  Jet& operator=(const Particle& other);
  // = Particle const
  Jet& operator=(const Jet& other);
  // = Jet non-const
  Jet& operator=(Jet& other);
    

 private:
  Int_t _numberOfConstituents;   
  Int_t _chargedMultiplicity;   
  Int_t _hadronFlavour;
  Double_t _bDiscriminator;   
  Double_t _btagCSVV2;
  Double_t _btagCMVA;
  Double_t _btagDeepB;
  Double_t _btagDeepC;
  Double_t _pileupId;   
  Double_t _mass;   
  Double_t _neutralHadEnergyFraction;   
  Double_t _neutralEmEmEnergyFraction;   
  Double_t _chargedHadronEnergyFraction;   
  Double_t _chargedEmEnergyFraction;   
  Double_t _muonEnergyFraction;   
  Double_t _electronEnergy;   
  Double_t _photonEnergy;   
  Double_t _uncorrPt;  
  Double_t _genPt;  
  Double_t _jerSF;
  Double_t _jesSF;
  Double_t _smearFactor;
  Int_t _tagged;
  Int_t _prefireVeto;
  Double_t _closestLep;
  Int_t _jetID;

  //Internal method for running the cuts, so that they don't have to be written several times in the cpp
  Bool_t ApplyCuts(std::vector<Muon>& selectedMuons, std::vector<Electron>& selectedElectrons, Bool_t useJetIDFromTree);

  //This is a variable that will be filled once during the normal filling but used by the JES shift to check
  Bool_t _passesIDs;
  inline void SetPassesIDs(Bool_t passesIDs){_passesIDs = passesIDs;};
  inline Bool_t GetPassesIDs() const {return _passesIDs;};
  inline Bool_t passesIDs() const {return _passesIDs;};

  float GetStochasticFactor(float pt, float eta, float rho, std::vector<std::vector<std::string> > * resolution, TString * resFormula);
  float GetJerFromFile(float eta, std::vector<std::vector<std::string> > * resSFs, int central);
  float GetJesFromFile(float pt, float eta, std::vector<std::vector<std::string> > * resSFs);
  float GetSmearFactor(float pt, float genPt, float eta, float rho, float jer_sf, std::vector<std::vector<std::string> > * resolution, TString * resFormula);

  // Cuts applied to the jet objects
  Double_t _maxEtaCut;
  Double_t _minPtCut;
  Double_t _bMaxEtaCut;
  Double_t _bMinPtCut;
  Double_t _bTagCut;
  Double_t _closestLeptonCut;
  Int_t    _jetIDValue;

  // Are we running systematic variations?
  Int_t _jesUp;
  Int_t _jesDown;
  Int_t _jerUp;
  Int_t _jerDown;

  // New approach to running JES shifts. We will save a list of the shifted Pts
  std::vector<Double_t> _jesShifts;
  
  //In order to calculate the resolution for the stoachastic smearing
  //  JME::JetResolution resolution;

  //Setters/getters
  void SetJesShifts(std::vector<Double_t> jesShifts){_jesShifts = jesShifts;};
  std::vector<Double_t> GetJesShifts() const {return _jesShifts;};
  std::vector<Double_t> jesShifts() const {return _jesShifts;};

  //Nominal p_{x,y,z} for 
  Double_t _nominalPx;
  Double_t _nominalPy;
  Double_t _nominalPz;
  Double_t _nominalE; //Iguess I need to change E as well?

  // Apply the jet correction systematics
  void SystematicPtShift(EventTree * evtr, Int_t iE, TLorentzVector * met);

  //Read out the JES shifts from the event tree
  std::vector<Double_t> GetJESShifts(EventTree * evtr, Int_t iE, float jetEta, std::vector<std::vector<std::string> > * resSFs, std::vector<std::vector<std::string> > * resolution, TString * resFormula);
  std::vector<Double_t> GetNanoJESShifts(float rho, std::vector<std::vector<std::string> > * resSFs, std::vector<std::vector<std::string> > * resolution, TString * resFormula, std::vector<std::vector<std::string> > * jesUnc);
 
  ////////////////////////////////////////////////////////////////////////////////
  // Integrate classes into the Root system
  // Must come at end of class definition
  ClassDef(Jet,0)
  ////////////////////////////////////////////////////////////////////////////////
};


#endif
