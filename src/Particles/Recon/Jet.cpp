/******************************************************************************
 * Jet.cpp                                                                    *
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
 *    Double_t _EtEm0                     -- Et for ME layer 0                *
 *                                                                            *
 * History                                                                    *
 *      16 Nov 2006 - Created by R. Schwienhorst                              *
 *      17 Nov 2006 - Modified by P. Ryan - cleanup and reorganized           *
 *      20 Mar 2007 - RS: Add filling from fastsim tree                       *
 *      29 Nov 2007 - RS: Add b-tagging choice when filling                   *
 *      11 Jan 2008 - RS: Fill only if object passes ID cuts                  *
 *      14 Jan 2008 - RS: For TRFIP2D, add systematics                        *
 *      08 May 2009 - J. Holzbauer: added quarkFlavorLabel tag                *
 *****************************************************************************/
#include "SingleTopRootAnalysis/Particles/Recon/Jet.hpp"
#include "SingleTopRootAnalysis/Particles/Recon/Electron.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <TRandom3.h>
#include<TString.h>


using namespace std;
//using namespace Analysis;

//
// give me a random number generator.
// put it here and make it static so that I don't create multiple copies
static TRandom3 jet_myran;
static TRandom3 jet_jer_myran;

// Integrate classes into the Root system
ClassImp(Jet)

/******************************************************************************
 * Jet::Jet()                                                                 *
 *                                                                            *
 * Default Constructor                                                        *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
  Jet::Jet() : Particle::Particle(),
  _numberOfConstituents(0), _chargedMultiplicity(0),  _bDiscriminator ( -999.0),  _btagCSVV2(-999.), _btagCMVA(-999.), _btagDeepB(-999.), _btagDeepC(-999.), _pileupId ( 0.0), _mass ( 0.0), _uncorrPt ( 0.0), _neutralHadEnergyFraction(0.0), _neutralEmEmEnergyFraction ( 0.0), _chargedHadronEnergyFraction (0.0), _chargedEmEnergyFraction(0.0), _muonEnergyFraction(0.0), _electronEnergy(0.0), _photonEnergy(0.0),_genPt(-999.0), _jerSF(1.0), _jesSF(1.0), _smearFactor(1.0), _jesUp(false), _jesDown(false), _jerUp(false), _jerDown(false), _hadronFlavour(-1), _tagged(0), _prefireVeto(0), _nominalPx(0.), _nominalPy(0.), _nominalPz(0.), _passesIDs(false), _jetID(0)
{
} //Jet()

/******************************************************************************
 * Jet::~Jet()                                                                *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
Jet::~Jet()
{
} //~Jet()

/******************************************************************************
 * Jet::Jet(const Jet& other)                                                 *
 *                                                                            *
 * Copy Constructor Jet const                                                 *
 *                                                                            *
 * Input:  Jet                                                                *
 * Output: None                                                               *
 ******************************************************************************/
Jet::Jet(const Jet& other): Particle(other),
_numberOfConstituents		(other.GetnumberOfConstituents()), 
_hadronFlavour		        (other.GethadronFlavour()), 
_tagged				(other.IsTagged()),
_prefireVeto			(other.IsPrefireVeto()),
_chargedMultiplicity		(other.GetchargedMultiplicity()),  
_bDiscriminator 		(other.GetbDiscriminator()), 
_btagCSVV2                      (other.GetbtagCSVV2()),
_btagCMVA                       (other.GetbtagCMVA()),
_btagDeepB                      (other.GetbtagDeepB()),
_btagDeepC                      (other.GetbtagDeepC()),
_pileupId 			(other.GetpileupId()), 
_mass 				(other.Getmass()), 
_uncorrPt 			(other.GetuncorrPt()), 
_neutralHadEnergyFraction	(other.GetneutralHadEnergyFraction()), 
_neutralEmEmEnergyFraction 	(other.GetneutralEmEmEnergyFraction()),
 _chargedHadronEnergyFraction 	(other.GetchargedHadronEnergyFraction()), 
_chargedEmEnergyFraction	(other.GetchargedEmEnergyFraction()), 
_muonEnergyFraction		(other.GetmuonEnergyFraction()), 
_electronEnergy			(other.GetelectronEnergy()), 
_photonEnergy			(other.GetphotonEnergy()),
_nominalPx                      (other.GetNominalPx()),
_nominalPy                      (other.GetNominalPy()),
_nominalPz                      (other.GetNominalPz()),
_genPt                          (other.GetgenPt()),
_jerSF                          (other.GetjerSF()),
_jesSF                          (other.GetjesSF()),
_smearFactor                    (other.GetsmearFactor()),
_passesIDs			(other.GetPassesIDs()),
_jesShifts 			(other.GetJesShifts()),
_jetID                          (other.GetjetID())
{
} //Jet()

/******************************************************************************
 * Jet::Jet(const Particle& other)                                            *
 *                                                                            *
 * Copy Constructor Particle const                                            *
 *                                                                            *
 * Input:  Particle                                                           *
 * Output: None                                                               *
 ******************************************************************************/
Jet::Jet(const Particle& other): Particle(other),
				 _numberOfConstituents(0), _hadronFlavour(-1), _chargedMultiplicity(0),  _bDiscriminator ( -999.0), _btagCSVV2(-999.), _btagCMVA(-999.), _btagDeepB(-999.), _btagDeepC(-999.), _pileupId ( 0.0), _mass ( 0.0), _uncorrPt ( 0.0), _neutralHadEnergyFraction(0.0), _neutralEmEmEnergyFraction ( 0.0), _chargedHadronEnergyFraction (0.0), _chargedEmEnergyFraction(0.0), _muonEnergyFraction(0.0), _electronEnergy(0.0), _photonEnergy(0.0), _genPt(-999.0), _jerSF(1.0), _jesSF(1.0), _smearFactor(1.0), _tagged(0), _prefireVeto(0), _nominalPx(0.), _nominalPy(0.), _nominalPz(0.), _passesIDs(false), _jetID(0)
{
 
} //Jet()


/******************************************************************************
 * Jet& Jet::operator+=(const Jet& other)                                     *
 *                                                                            *
 * Overload +=                                                                *
 *                                                                            *
 * Input:  Jet                                                                *
 * Output: Jet                                                                *
 ******************************************************************************/
Jet& Jet::operator+=(const Jet& other)
{
  
  Particle::operator+=(other);
  return *this;
} //+=

/******************************************************************************
 * Jet& Jet::operator+(const Jet& other)                                      *
 *                                                                            *
 * Overload +                                                                 *
 *                                                                            *
 * Input:  Jet                                                                *
 * Output: Jet                                                                *
 ******************************************************************************/
Jet Jet::operator+(const Jet& other)
{
  Particle particleTemp = Particle::operator+(other);
  Jet jetTemp = particleTemp;
  return jetTemp;

} //+

/******************************************************************************
 * Jet& Jet::operator=(const Jet& other)                                      *
 *                                                                            *
 * Overload = Particle                                                        *
 *                                                                            *
 * Input:  Particle                                                           *
 * Output: Jet                                                                *
 ******************************************************************************/
Jet& Jet::operator=(const Particle& other)
{
  
  Particle::operator=(other);
  SetnumberOfConstituents(0), 
  SethadronFlavour(-1),
  SetTagged(0),
  SetPrefireVeto(0),
  SetchargedMultiplicity(0),  
  SetbDiscriminator ( -999.0), 
  SetbtagCSVV2  (-999.),
  SetbtagCMVA  (-999.),
  SetbtagDeepB (-999.),
  SetbtagDeepC (-999.),    
  SetpileupId ( 0.0), 
  Setmass ( 0.0), 
  SetuncorrPt ( 0.0), 
  SetneutralHadEnergyFraction(0.0), 
  SetneutralEmEmEnergyFraction ( 0.0), 
  SetchargedHadronEnergyFraction (0.0), 
  SetchargedEmEnergyFraction(0.0), 
  SetmuonEnergyFraction(0.0), 
  SetelectronEnergy(0.0), 
  SetNominalPx(0.),
  SetNominalPy(0.), 
  SetNominalPz(0.), 
  SetNominalE(0.),
  SetgenPt(-999.0),
  SetjerSF(1.0),
  SetjesSF(1.0),
  SetsmearFactor(1.0),
  SetPassesIDs(false),
  SetphotonEnergy(0.0);
  SetjetID(0);

  return *this;
} //= Particle

/******************************************************************************
 * Jet& Jet::operator=(const Jet& other)                                      *
 *                                                                            *
 * Overload = const Jet                                                       *
 *                                                                            *
 * Input:  Jet                                                                *
 * Output: Jet                                                                *
 ******************************************************************************/
Jet& Jet::operator=(const Jet& other)
{
  
  Particle::operator=(other);
  SetnumberOfConstituents		(other.GetnumberOfConstituents());
  SethadronFlavour	         	(other.GethadronFlavour());
  SetTagged				(other.IsTagged());
  SetPrefireVeto			(other.IsPrefireVeto());
  SetchargedMultiplicity		(other.GetchargedMultiplicity()); 
  SetbDiscriminator 			(other.GetbDiscriminator());
  SetbtagCSVV2                          (other.GetbtagCSVV2());
  SetbtagCMVA                           (other.GetbtagCMVA());
  SetbtagDeepB                          (other.GetbtagDeepB());
  SetbtagDeepC                          (other.GetbtagDeepC());
  SetpileupId 				(other.GetpileupId());
  Setmass 				(other.Getmass());
  SetuncorrPt 				(other.GetuncorrPt());
  SetneutralHadEnergyFraction		(other.GetneutralHadEnergyFraction());
  SetneutralEmEmEnergyFraction 		(other.GetneutralEmEmEnergyFraction());
  SetchargedHadronEnergyFraction 	(other.GetchargedHadronEnergyFraction());
  SetchargedEmEnergyFraction		(other.GetchargedEmEnergyFraction());
  SetmuonEnergyFraction			(other.GetmuonEnergyFraction());
  SetelectronEnergy			(other.GetelectronEnergy());
  SetphotonEnergy			(other.GetphotonEnergy());
  SetNominalPx                      (other.GetNominalPx());
  SetNominalPy                      (other.GetNominalPy());
  SetNominalPz                      (other.GetNominalPz());
  SetNominalE                       (other.GetNominalE());
  SetgenPt                          (other.GetgenPt());
  SetjerSF                          (other.GetjerSF());
  SetjesSF                          (other.GetjesSF());
  SetsmearFactor                    (other.GetsmearFactor());
  SetJesShifts			    (other.GetJesShifts());
  SetPassesIDs                      (other.GetPassesIDs());
  SetjetID                          (other.GetjetID());
  return *this;
} //= const

/******************************************************************************
 * Jet& Jet::operator=(const Jet& other)                                      *
 *                                                                            *
 * Overload = non-const Jet                                                   *
 *                                                                            *
 * Input:  Jet                                                                *
 * Output: Jet                                                                *
 ******************************************************************************/
Jet& Jet::operator=(Jet& other)
{
  Particle::operator=(other);
  SetnumberOfConstituents		(other.GetnumberOfConstituents());
  SethadronFlavour	         	(other.GethadronFlavour());
  SetTagged			        (other.IsTagged());
  SetPrefireVeto		        (other.IsPrefireVeto());
  SetchargedMultiplicity		(other.GetchargedMultiplicity()); 
  SetbDiscriminator 			(other.GetbDiscriminator());
  SetbtagCSVV2                          (other.GetbtagCSVV2());
  SetbtagCMVA                           (other.GetbtagCMVA());
  SetbtagDeepB                          (other.GetbtagDeepB());
  SetbtagDeepC                          (other.GetbtagDeepC());
  SetpileupId 				(other.GetpileupId());
  Setmass 				(other.Getmass());
  SetuncorrPt 				(other.GetuncorrPt());
  SetneutralHadEnergyFraction		(other.GetneutralHadEnergyFraction());
  SetneutralEmEmEnergyFraction 		(other.GetneutralEmEmEnergyFraction());
  SetchargedHadronEnergyFraction 	(other.GetchargedHadronEnergyFraction());
  SetchargedEmEnergyFraction		(other.GetchargedEmEnergyFraction());
  SetmuonEnergyFraction			(other.GetmuonEnergyFraction());
  SetelectronEnergy			(other.GetelectronEnergy());
  SetphotonEnergy			(other.GetphotonEnergy());
  SetNominalPx                      (other.GetNominalPx());
  SetNominalPy                      (other.GetNominalPy());
  SetNominalPz                      (other.GetNominalPz());
  SetNominalE                       (other.GetNominalE());
  SetgenPt                          (other.GetgenPt());
  SetjerSF                          (other.GetjerSF());
  SetjesSF                          (other.GetjesSF());
  SetsmearFactor                    (other.GetsmearFactor());
  SetJesShifts			    (other.GetJesShifts());
  SetPassesIDs                      (other.GetPassesIDs());
  SetjetID                          (other.GetjetID());
  return *this;
} //= non-const

/******************************************************************************         
 * void Jet::SetCuts(TEnv* config)                                            *
 *                                                                            *         
 * Set the cuts to be used on the jet objects                                 *
 *                                                                            *         
 * Input:  TEnv* config                                                       * 
 *         - Contains the cuts to be applied to each jet                      *
 * Output: None                                                               *
 ******************************************************************************/

void Jet::SetCuts(TEnv * config)
{
  _maxEtaCut = 		config -> GetValue("ObjectID.Jet.MaxEta",100.);
  _minPtCut = 		config -> GetValue("ObjectID.Jet.MinPt",0.);
  _jetIDValue =         config -> GetValue("ObjectID.Jet.JetIDValue",6.);
  _bMaxEtaCut = 	config -> GetValue("ObjectID.BJet.MaxEta",100.);
  _bMinPtCut = 		config -> GetValue("ObjectID.BJet.MinPt",0.);
  _bTagCut = 		config -> GetValue("ObjectID.BJet.BTagCut",0.0);
  _closestLeptonCut = 	config -> GetValue("ObjectID.Jet.LepCleanR",0.0);
  _jesUp = 		config -> GetValue("Systs.doJESUp",0);
  _jesDown = 		config -> GetValue("Systs.doJESDown",0);
  _jerUp = 		config -> GetValue("Systs.doJERUp",0);
  _jerDown = 		config -> GetValue("Systs.doJERDown",0);
  
}

/****************************************************************************** 
 * void Jet::Fill(nanoAODTree *evtr, Int_t iE,const TString& tagName)           * 
 *                                                                            * 
 * Fill jet vector from tree                                                  * 
 *                                                                            * 
 * Input:  Event Tree pointer                                                 * 
 *         - which jet                                                        * 
 * Output: True if this jet passes jet ID cuts                                * 
 ******************************************************************************/


Bool_t Jet::Fill(std::vector<Muon>& selectedMuons, std::vector<Electron>& selectedElectrons, nanoAODTree *evtr, Int_t iE, TLorentzVector * met, bool isMC, std::vector<std::vector<std::string> > * resolution, std::vector<std::vector<std::string> > * resSFs, TString * resFormula, std::vector<std::vector<std::string> > * jesUncs)
{
  //Set up the TLorentzVector of this particle
  Double_t jetPt,jetEta,jetPhi,jetMass;
  jetPt    = evtr -> Jet_pt    [iE];
  jetEta   = evtr -> Jet_eta   [iE];
  jetPhi   = evtr -> Jet_phi   [iE];
  jetMass  = evtr -> Jet_mass  [iE];

  SetPtEtaPhiM(jetPt,jetEta,jetPhi,jetMass);

  //Now set up other information in the class that are available in nanoAOD
  SetnumberOfConstituents		(evtr -> Jet_nConstituents         	[iE]);
  SetbDiscriminator 			(evtr -> Jet_btagCSVV2           	[iE]); // This is the default variable used in b-jet cuts. Other available taggers: btagCMVA,btagCSVV2,btagDeepB,btagDeepC
  SetbtagCSVV2  			(evtr -> Jet_btagCSVV2           	[iE]);
  SetbtagDeepB   			(evtr -> Jet_btagDeepB           	[iE]);
  SetpileupId 				(evtr -> Jet_puId          		[iE]);
  Setmass 				(evtr -> Jet_mass     			[iE]);
  SetneutralHadEnergyFraction		(evtr -> Jet_neHEF              	[iE]);
  SetneutralEmEmEnergyFraction 		(evtr -> Jet_neEmEF                     [iE]);
  SetchargedHadronEnergyFraction 	(evtr -> Jet_chHEF                      [iE]);
  SetchargedEmEnergyFraction		(evtr -> Jet_chEmEF                     [iE]);
  SetjetID                              (evtr -> Jet_jetId                      [iE]);

  //Bits that were filled in the TNT/BOOM trees but are not immediately obvious to me in the nanoAOD trees
  /*SetmuonEnergyFraction			(evtr -> Jet_muonEnergyFraction     	-> operator[](iE));
  SetelectronEnergy			(evtr -> Jet_electronEnergy     	-> operator[](iE));
  SetphotonEnergy			(evtr -> Jet_photonEnergy     		-> operator[](iE));
  SetchargedMultiplicity		(evtr -> Jet_chargedMultiplicity     	-> operator[](iE)); 
  SetuncorrPt 				(evtr -> Jet_Uncorr_pt     		-> operator[](iE));
  SetbtagCMVA   			(evtr -> Jet_btagCMVA           	[iE]);
  SetbtagDeepC   			(evtr -> Jet_btagDeepCvL           	[iE]);
*/
  _jesShifts.clear();

  if (isMC){
    SethadronFlavour                    (evtr -> Jet_hadronFlavour              [iE]);
    //This information does not seem immediately obvious either.
    //SetgenPt                            (evtr -> Jet_genpt                      -> operator[](iE));
    SetgenPt                            (evtr -> GenJet_pt[evtr->Jet_genJetIdx  [iE]]); 
    
    //Get jer sf
    SetjerSF(GetJerFromFile(Eta(),resSFs,0));

    //jet smearing - should this include MET variation?
    double smearFactor = GetSmearFactor(Pt(),genPt(),Eta(),evtr->fixedGridRhoFastjetAll,GetjerSF(),resolution,resFormula);
    SetsmearFactor(smearFactor);

    //Get jes shifts. Doing it here because maybe the pt shift changes some things
    _jesShifts = GetNanoJESShifts(evtr->fixedGridRhoFastjetAll,resSFs,resolution,resFormula,jesUncs);
        
    double ptRescale = smearFactor;
    if (ptRescale == 0) ptRescale = 1.;

    //Propogate the smearing to the missing et
    met->SetPx(met->Px() + (1-ptRescale)*Px());
    met->SetPy(met->Py() + (1-ptRescale)*Py());


    SetPx(Px()*ptRescale);
    SetPy(Py()*ptRescale);
    SetPz(Pz()*ptRescale);
    SetE(E()*ptRescale);

  }

  SetNominalPx(Px());
  SetNominalPy(Py());
  SetNominalPz(Pz());
  SetNominalE(E());  

  //The current implementation of JES uncertainties relies on information directly stored in the BSM trees. TODO: Update to nanoAOD
  
  //Apply the cuts. The return value is true if it passes, false if not.
  return ApplyCuts(selectedMuons,selectedElectrons,kTRUE); 

  return kFALSE;
}


/******************************************************************************         
 * void Jet::Fill(EventTree *evtr, Int_t iE,const TString& tagName)           *         
 *                                                                            *         
 * Fill jet vector from tree                                                  *         
 *                                                                            *         
 * Input:  Event Tree pointer                                                 *         
 *         - which jet                                                        *
 * Output: True if this jet passes jet ID cuts                                *         
 ******************************************************************************/

Bool_t Jet::Fill(std::vector<Muon>& selectedMuons, std::vector<Electron>& selectedElectrons, EventTree *evtr, Int_t iE, TLorentzVector * met, bool isMC, std::vector<std::vector<std::string> > * resolution, std::vector<std::vector<std::string> > * resSFs, TString * resFormula)
{

  Double_t jetPt, jetEta,jetPhi,jetE, jetCharge, jetM;
  jetPt     = evtr -> Jet_pt     -> operator[](iE);
  jetEta    = evtr -> Jet_eta    -> operator[](iE);
  jetPhi    = evtr -> Jet_phi    -> operator[](iE);
  jetE      = evtr -> Jet_energy -> operator[](iE);

  if(jetE > 0){
    SetPtEtaPhiE(jetPt,jetEta,jetPhi,jetE);
  }else{
    SetPtEtaPhiE(1,1,1,3);
  }

  SetnumberOfConstituents		(evtr -> Jet_numberOfConstituents     	-> operator[](iE));
  SetchargedMultiplicity		(evtr -> Jet_chargedMultiplicity     	-> operator[](iE)); 
  SetbDiscriminator 			(evtr -> Jet_pfCombinedInclusiveSecondaryVertexV2BJetTags     	-> operator[](iE));
  SetpileupId 				(evtr -> Jet_pileupId     		-> operator[](iE));
  Setmass 				(evtr -> Jet_mass     			-> operator[](iE));
  SetuncorrPt 				(evtr -> Jet_Uncorr_pt     		-> operator[](iE));
  SetneutralHadEnergyFraction		(evtr -> Jet_neutralHadEnergyFraction	-> operator[](iE));
  SetneutralEmEmEnergyFraction 		(evtr -> Jet_neutralEmEnergyFraction    -> operator[](iE));
  SetchargedHadronEnergyFraction 	(evtr -> Jet_chargedHadronEnergyFraction-> operator[](iE));
  SetchargedEmEnergyFraction		(evtr -> Jet_chargedEmEnergyFraction    -> operator[](iE));
  SetmuonEnergyFraction			(evtr -> Jet_muonEnergyFraction     	-> operator[](iE));
  SetelectronEnergy			(evtr -> Jet_electronEnergy     	-> operator[](iE));
  SetphotonEnergy			(evtr -> Jet_photonEnergy     		-> operator[](iE));


  if (isMC){
    SethadronFlavour                    (evtr -> Jet_hadronFlavour              -> operator[](iE));
    SetgenPt                            (evtr -> Jet_genpt                      -> operator[](iE));
    SetjerSF                            (evtr -> Jet_JerSF                      -> operator[](iE));
    SetjesSF                            (evtr -> Jet_JesSF                      -> operator[](iE));

    //    if (jerSF() == 1.) SetjerSF(GetJerFromFile(Eta(),resSFs,0));
    SetjerSF(GetJerFromFile(Eta(),resSFs,0)); //Let's do rthis all from the file...
    if (jerSF() <= 0.) SetjerSF(1.);
    if (jesSF() <= 0.) SetjesSF(1.);
  }

  _jesShifts.clear();
  _jesShifts = GetJESShifts(evtr,iE,Eta(),resSFs,resolution,resFormula);

  double smearFactor = 1.;

  if (isMC){ //Here is where we smear the jet if it's MC
    //If there is a gen jet use the genpt
    smearFactor = GetSmearFactor(Pt(),genPt(),Eta(),evtr->EVENT_rhopog,GetjerSF(),resolution,resFormula);
    //    std::cout << Pt() << " " << genPt() << " " << Eta() << " " << GetjerSF() << " "<< smearFactor << std::endl;
    //    double jer_sf = evtr->Jet_JerSF->operator[](iE);
    if (smearFactor == 0) smearFactor = 1.;
    SetPx(Px()*smearFactor);
    SetPy(Py()*smearFactor);
    SetPz(Pz()*smearFactor);
    SetE(E()*smearFactor);


  }

  SetsmearFactor(smearFactor);

  SetNominalPx(Px());
  SetNominalPy(Py());
  SetNominalPz(Pz());
  SetNominalE(E());
  
  //Apply the cuts and then return that value to the EventContainer
  return ApplyCuts(selectedMuons,selectedElectrons,kFALSE);

} //End of BSM framework fill routine


Bool_t Jet::ApplyCuts(std::vector<Muon>& selectedMuons, std::vector<Electron>& selectedElectrons, Bool_t useJetIDFromTree){

  /////////////////////////////////////////////////////////////////////////////
  // Jet Pt and Eta Cuts
  /////////////////////////////////////////////////////////////////////////////

  Bool_t passPt = Pt() > _minPtCut;
  Bool_t passEta = TMath::Abs(Eta()) < _maxEtaCut;

  /////////////////////////////////////////////////////////////////////////
  // Jet ID
  /////////////////////////////////////////////////////////////////////////
  
  Bool_t passesJetID = kFALSE;

  if (useJetIDFromTree){ //This is different for nanoAOD vs BSM
    passesJetID = (jetID() == _jetIDValue); // Check https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookNanoAOD#Jets for what value this should be. Also this should probably be saved in the config...
  }
  else {
    Bool_t neutralID = (TMath::Abs(Eta()) > 3. || (neutralHadEnergyFraction() < 0.99 &&  neutralEmEmEnergyFraction() < 0.99 && numberOfConstituents() > 1));
    Bool_t chargedID = (TMath::Abs(Eta()) > 2.4 || (chargedHadronEnergyFraction() > 0. && chargedMultiplicity() > 0. && chargedEmEnergyFraction() < 0.99));
    Bool_t neutralHighEtaID = (TMath::Abs(Eta()) < 3. || (neutralEmEmEnergyFraction() < 0.9 && (numberOfConstituents() - chargedMultiplicity()) > 10));
    
    passesJetID = neutralID && chargedID && neutralHighEtaID;
  }
  /////////////////////////////////////////////////////////////////////////
  // Jet Cleaning
  /////////////////////////////////////////////////////////////////////////

  Bool_t passesCleaning = kTRUE;
 
  Double_t closestLepton = 999.;

  for (auto const & ele : selectedElectrons){
    if (ele.DeltaR(*this) < closestLepton) closestLepton = ele.DeltaR(*this);
  }
  for (auto const & mu : selectedMuons){
    if (mu.DeltaR(*this) < closestLepton) closestLepton = mu.DeltaR(*this);
  }
  if (closestLepton < _closestLeptonCut) passesCleaning = kFALSE;

  SetClosestLep(closestLepton);

  /////////////////////////////////////////////////////////////////////////
  // Set prefire veto tag
  /////////////////////////////////////////////////////////////////////////

  SetPrefireVeto(0);

  if (Pt() > 100. && TMath::Abs(Eta()) > 2.25 && TMath::Abs(Eta()) < 3.0) {
    //    std::cout << "vetoing " << Pt() << " " << Eta() << std::endl;
    SetPrefireVeto(1);
  }

  /////////////////////////////////////////////////////////////////////////
  // B-tag related cuts
  /////////////////////////////////////////////////////////////////////////

  Bool_t passbPt = Pt() > _bMinPtCut;
  Bool_t passbEta = TMath::Abs(Eta()) < _bMaxEtaCut;
  Bool_t passTagCut = bDiscriminator() > _bTagCut;

  if (passbPt && passbEta && passTagCut) SetTagged(1);
  else SetTagged(0);

  SetPassesIDs(passesJetID && passesCleaning);

  if (passPt && passEta && passesJetID && passesCleaning) return kTRUE;
  
  return kFALSE;

} //ApplyCuts()

/******************************************************************************         
 * void Jet::FillFastSim(FastSimTree *tr, Int_t iE,const TString& tagName )   *         
 *                                                                            *         
 * Fill jet vector from fastsim tree                                          *         
 *                                                                            *         
 * Input:  - FastSim tree pointer                                             *         
 *         - which jet                                                        *
 *         - tagName, either default or IP2D                                  *
 * Output: True if this jet passes jet ID cuts                                *         
 ******************************************************************************/
Bool_t Jet::FillFastSim( std::vector<MCJet>& MCBJets, std::vector<MCJet>& MCCJets, std::vector<MCTau>& MCTaus, std::vector<Electron>& electrons, FastSimTree *tr,Int_t iE,TEnv* config,const TString& tagName, Double_t btagCut, Double_t mistagCut, Double_t eshift)
{  
  Double_t jetPt, jetEta,jetPhi,jetE, jetCharge;
  jetPt     = tr -> PJet_p_T    -> operator[](iE);
  jetEta    = tr -> PJet_eta    -> operator[](iE);
  jetPhi    = tr -> PJet_phi    -> operator[](iE);
  jetE      = tr -> PJet_E      -> operator[](iE);
  jetCharge = tr -> PJet_charge -> operator[](iE); 
  
  SetPtEtaPhiE(jetPt,jetEta,jetPhi,jetE);
  SetCharge(jetCharge);

  if(eshift != 0){
    
    Double_t diffx,diffy,diffz,diffe;
    
    //  cout<<"OLD "<<Px()<<"  "<<Py()<<"  "<<Pz()<<"  "<<E()<<endl;
    //  cout<<"OLD "<<Pt()<<"  "<<Eta()<<"  "<<Phi()<<endl;
    
    diffx = eshift * Px();
    diffy = eshift * Py();
    diffz = eshift * Pz();
    diffe = eshift * E();
    SetPxPyPzE(diffx + Px(),
	       diffy + Py(),
	       diffz + Pz(),
	       diffe + E());
    
    //cout<<diffx<<"  "<<diffy<<"  "<<diffz<<"  "<<diffe<<"  "<<eshift<<endl;
    //cout<<"NEW "<<Px()<<"  "<<Py()<<"  "<<Pz()<<"  "<<E()<<endl;
    //cout<<"NEW "<<Pt()<<"  "<<Eta()<<"  "<<Phi()<<endl;
    
    // if met pointers have been provided, update those also
    //cout<<"OLDMET "<<metx<<"  "<<mety<<endl;
    
    // cout<<"NEWMET "<<*metx<<"  "<<*mety<<endl;
   
  }
  //##########################################################################
  //b-tagging.  Insert info when available, based on reco tree.
  //##########################################################################



  //##########################################################################
  //PdgId()
  //
  //identifies jet with a MC quark.  5=bquark, 4=cquark, 15=tau(mc), 0=lightquark or gluon, etc
  //
  //This section identifies the MC particle (b, c, tau, or other (light quark)
  //that the jet is closest to and labels it as that, by pdgid, where light quark
  //is 0.  Jets are labeled starting with b, then c, then tau, and then, if none
  //of these match, as light quark.  This follows proceedure in b-tagging csc note.
  //Jet must be within deltaR of 0.3 of particle to be matched, and mc particle
  //must have Pt > 5 GeV, also per the csc note.
  //
  //Currently modified to use the Jet_Truth_Label variable from tree
  //Currently no charge information carried in this label
  //
  //NOTE: This PDGId() method returns the flavor of the MC particle associated
  //with the jet (wrt position).  It is NOT nessesarily the jet's flavor, but
  //a reasonable assumption BASED ON MC AND RECO INFORMATION
  //##########################################################################

  Double_t DeltaRCurrent = 0;
  Double_t EtaCurrent  = 0;
  Double_t PhiCurrent  = 0;
  Double_t PtCurrent   = 0;
  //SetPdgId(999);//not set yet
  
  //SetPdgId(tr -> Jet_Truth_Label -> operator[](iE));//not set yet
  
  
  //////////////////////////////////////////////////
  // Overlap with Electron
  //////////////////////////////////////////////////
  // Get Minimum separation between the electron and jet from the config file
  Double_t electronDeltaRMin = config -> GetValue("ObjectID.Jet.ElectronDeltaRMin", 0.0);
  Double_t electronDeltaR = 0.0;
  std::vector<Electron>::iterator electronIterator;
  for(electronIterator = electrons.begin(); electronIterator != electrons.end(); electronIterator++){
    electronDeltaR = electronIterator->DeltaR(*this);
    // If jet is too close to any electron set overlap to 1 and break
    if(electronDeltaR < electronDeltaRMin ){
      //SetOverlapWithElectron(1);
      break;
    } //if
  } //for
  
  //
  // now check jet object ID cuts
  double maxEta=config->GetValue("ObjectID.Jet.MaxEta",100.);
  double minPt=config->GetValue("ObjectID.Jet.MinPt",0.);
  
  
  //if(jetPt<minPt) return kFALSE;
  //if(TMath::Abs(jetEta)>maxEta) return kFALSE;

  // Set to false if jet fails pT requirement, eta reqiurement, is overlapped with electron
  //if( jetPt <= minPt || TMath::Abs(jetEta) >= maxEta  || GetOverlapWithElectron() ) return kFALSE;
  //else return kTRUE;
  return kTRUE;
  
} //FillFastSim()

/******************************************************************************         
 * void Jet::SystematicPtShift(EventTree * evtr)                         *
 *                                                                            *         
 * Apply systematic shifts in jet pt                                          *
 *                                                                            *         
 * Input:  - the event tree (to access systematic SFs and MET info)      *
 * Output: -                                                                  *
 ******************************************************************************/
void Jet::SystematicPtShift(EventTree * evtr, Int_t iE, TLorentzVector * met){


  //  std::cout << "syst correct" << std::endl;
  float ptSF = 1.0;
  if (_jesUp){
    ptSF = evtr->Jet_JesSFup->operator[](iE)/evtr->Jet_JesSF->operator[](iE);
  }
  if (_jesDown){
    ptSF = evtr->Jet_JesSFdown->operator[](iE)/evtr->Jet_JesSF->operator[](iE);
  }
  if (_jerUp){
    ptSF = evtr->Jet_JerSFup->operator[](iE)/evtr->Jet_JerSF->operator[](iE);
  }
  if (_jerDown){
    ptSF = evtr->Jet_JerSFdown->operator[](iE)/evtr->Jet_JerSF->operator[](iE);
  }
  //  float ptBefore = Pt();
  //Remove jet from MET
  met->SetPx(met->Px() + Px());
  met->SetPy(met->Py() + Py());
  //Apply the correction
  //  std::cout << Px() << " " << Py() << " " << Pt() << std::endl;
  SetPx(Px()*ptSF);
  SetPy(Py()*ptSF);
  SetPz(Pz()*ptSF);
  //std::cout << Px() << " " << Py() << " " << Pt() << std::endl << std::endl;
  
  //Propagate to MET
  met->SetPx(met->Px() - Px());
  met->SetPy(met->Py() - Py());

  //  float ptAfter = Pt();
  //  if (ptBefore < 30 && ptAfter > 30) {
  //  std::cout << "Now selected: Jet #" << iE << "Jet Pt: " << ptBefore << " JER SF: " << evtr->Jet_JerSF->operator[](iE) << " up: " << evtr->Jet_JerSFup->operator[](iE) << " down: " << evtr->Jet_JerSFdown->operator[](iE) << " JES SF: " << evtr->Jet_JesSF->operator[](iE) << " up: " << evtr->Jet_JesSFup->operator[](iE) << " down: " << evtr->Jet_JesSFdown->operator[](iE);
  //  std::cout << " Pt SF: " << ptSF << " Jet pt after: " << Pt() << std::endl;
  //}
  //if (ptBefore > 30 && ptAfter < 30) {
  //  std::cout << "No longer selected: Jet #" << iE << "Jet Pt: " << ptBefore << " JER SF: " << evtr->Jet_JerSF->operator[](iE) << " up: " << evtr->Jet_JerSFup->operator[](iE) << " down: " << evtr->Jet_JerSFdown->operator[](iE) << " JES SF: " << evtr->Jet_JesSF->operator[](iE) << " up: " << evtr->Jet_JesSFup->operator[](iE) << " down: " << evtr->Jet_JesSFdown->operator[](iE);
  //  std::cout << " Pt SF: " << ptSF << " Jet pt after: " << Pt() << std::endl;
  //}

}

std::vector<Double_t> Jet::GetJESShifts(EventTree * evtr, Int_t iE, float jetEta, std::vector<std::vector<std::string> > * resSFs, std::vector<std::vector<std::string> > * resolution, TString * resFormula){
  std::vector<Double_t> jesShifts;
  Double_t nominalJES = evtr -> Jet_JesSF->operator[](iE);
  //Now get all of the correction factors
  //  std::cout << "preStatCheck" << std::endl;
  if (NULL == evtr->Jet_JesSF_AbsoluteStat_up) return jesShifts;
  jesShifts.push_back(evtr->Jet_JesSF_AbsoluteStat_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_AbsoluteStat_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_AbsoluteScale_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_AbsoluteScale_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_AbsoluteMPFBias_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_AbsoluteMPFBias_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_Fragmentation_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_Fragmentation_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_SinglePionECAL_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_SinglePionECAL_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_SinglePionHCAL_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_SinglePionHCAL_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_FlavorQCD_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_FlavorQCD_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_TimePtEta_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_TimePtEta_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeJEREC1_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeJEREC1_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeJEREC2_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeJEREC2_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeJERHF_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeJERHF_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativePtBB_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativePtBB_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativePtEC1_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativePtEC1_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativePtEC2_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativePtEC2_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativePtHF_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativePtHF_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeBal_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeBal_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeFSR_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeFSR_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeStatEC_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeStatEC_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeStatHF_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_RelativeStatHF_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtRef_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtRef_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtBB_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtBB_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtEC1_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtEC1_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtEC2_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtEC2_down->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtHF_up->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JesSF_PileUpPtHF_down->operator[](iE)/nominalJES);
  //New version of the JEr systematics that does it properly
  //  std::cout << nominalJES << " " << evtr->Jet_JesSF_AbsoluteStat_up->operator[](iE) << " " << evtr->Jet_JesSF_AbsoluteStat_down->operator[](iE) << std::endl;
  nominalJES = GetSmearFactor(Pt(),genPt(),Eta(),evtr->EVENT_rhopog,GetjerSF(),resolution,resFormula);
  if (nominalJES <= 0.) nominalJES = 1.;
  float downJERStat = GetSmearFactor(Pt(),genPt(),Eta(),evtr->EVENT_rhopog,GetJerFromFile(jetEta,resSFs,3),resolution,resFormula);
  float upJERStat = GetSmearFactor(Pt(),genPt(),Eta(),evtr->EVENT_rhopog,GetJerFromFile(jetEta,resSFs,4),resolution,resFormula);
  float downJERSyst = GetSmearFactor(Pt(),genPt(),Eta(),evtr->EVENT_rhopog,GetJerFromFile(jetEta,resSFs,5),resolution,resFormula);
  float upJERSyst = GetSmearFactor(Pt(),genPt(),Eta(),evtr->EVENT_rhopog,GetJerFromFile(jetEta,resSFs,6),resolution,resFormula);
  if (!(genPt() > 0.)){
    //std::cout << "no gen jet" << std::endl;
    //if its a randomised jet change it so that its not.
    nominalJES = GetJerFromFile(jetEta,resSFs,0); 
    downJERStat = GetJerFromFile(jetEta,resSFs,3);
    upJERStat = GetJerFromFile(jetEta,resSFs,4); 
    downJERSyst = GetJerFromFile(jetEta,resSFs,5); 
    upJERSyst = GetJerFromFile(jetEta,resSFs,6); 
  }
  /*  nominalJES = 1.;
  downJERStat = 1;
  upJERStat = 1;
  downJERSyst = 1;
  upJERSyst = 1;
  */
  //  if ( nominalJES > upJERStat || nominalJES < downJERStat || nominalJES > upJERSyst || nominalJES < downJERSyst){
  //  std::cout << "Weird thing here" << Pt() << " " << Eta() << " " << genPt() << " " << nominalJES << " " << downJERStat << " " << upJERStat << " " << downJERSyst << " " << upJERSyst  << std::endl;

  //}
  jesShifts.push_back(upJERStat/nominalJES);
  jesShifts.push_back(downJERStat/nominalJES);
  jesShifts.push_back(upJERSyst/nominalJES);
  jesShifts.push_back(downJERSyst/nominalJES);  
  /*  nominalJES = evtr->Jet_JerSF->operator[](iE);
  float upJER = evtr->Jet_JerSFup->operator[](iE);
  float downJER = evtr->Jet_JerSFdown->operator[](iE);
  if (nominalJES == 1){
    nominalJES = GetJerFromFile(jetEta,resSFs,0);
    downJER = GetJerFromFile(jetEta,resSFs,1);
    upJER = GetJerFromFile(jetEta,resSFs,2);
  }
  jesShifts.push_back(upJER/nominalJES);
  jesShifts.push_back(downJER/nominalJES);
  std::cout << " pre syst check" << std::endl;
  if (NULL == evtr->Jet_JerSFup_syst) return jesShifts;
  jesShifts.push_back(evtr->Jet_JerSFup_syst->operator[](iE)/nominalJES);
  jesShifts.push_back(evtr->Jet_JerSFdown_syst->operator[](iE)/nominalJES); */ //We're changing this so that 
  return jesShifts;

}

std::vector<Double_t> Jet::GetNanoJESShifts(float rho, std::vector<std::vector<std::string> > * resSFs, std::vector<std::vector<std::string> > * resolution, TString * resFormula, std::vector<std::vector<std::string> > * jesUncs){

  //Define the vector
  std::vector<Double_t> jesShifts;

  //Calculate JER uncertainty
  float nominalSmear = GetSmearFactor(Pt(),genPt(),Eta(),rho,GetJerFromFile(Eta(),resSFs,0),resolution,resFormula);
  float jerDown = GetSmearFactor(Pt(),genPt(),Eta(),rho,GetJerFromFile(Eta(),resSFs,1),resolution,resFormula);
  float jerUp = GetSmearFactor(Pt(),genPt(),Eta(),rho,GetJerFromFile(Eta(),resSFs,2),resolution,resFormula);
  //This doesn't really work with the stoachastic term, so for that one we'll use the uncertainty from eta only
  if (!(genPt() > 0.)){ 
    nominalSmear =  GetJerFromFile(Eta(),resSFs,0);
    jerDown =  GetJerFromFile(Eta(),resSFs,1);
    jerUp =  GetJerFromFile(Eta(),resSFs,2);
  }
  jesShifts.push_back(jerDown/nominalSmear);
  jesShifts.push_back(jerUp/nominalSmear);

  //Now do jes uncertainty. For now we are only doing the one 'Total' uncertainty. Will split it up if I ever have to...
  float jesUncert = GetJesFromFile(Pt(),Eta(),jesUncs);
  jesShifts.push_back(1-jesUncert);
  jesShifts.push_back(1+jesUncert);

  //return these values
  return jesShifts;

}

float Jet::GetJesFromFile(float pt,float eta, std::vector<std::vector<std::string> > * jesUnc){
  float uncert = 0.;
  for (auto res: *jesUnc){
    if (eta < std::stof(res[0]) || eta > std::stof(res[1])) continue;
    for (int i = 6; i < res.size(); i+=3){
      if (pt < std::stof(res[i-3]) || pt > std::stof(res[i])) continue;
      uncert = std::stof(res[i-2]);
    }
  }
  return uncert;
}

Bool_t Jet::ShiftPtWithJESCorr(Int_t jesShiftInd, TLorentzVector * met){

  //  met->SetPx(met->Px() + Px());                                               
  // met->SetPy(met->Py() + Py());                                     

  //  if (jesShiftInd > 23){
  //  std::cout << jesShiftInd << " " << _jesShifts[jesShiftInd] << std::endl;
  //}


  Double_t ptSF = _jesShifts[jesShiftInd];

  //Apply the correction                                                      
  SetPx(nominalPx()*ptSF);                                                           
  SetPy(nominalPy()*ptSF);                                                           
  SetPz(nominalPz()*ptSF);                      
  SetE(nominalE()*ptSF);

  //std::cout << Px() << " " << Py() << " " << Pt() << std::endl << std::endl;

  //  std::cout << met->Px() << " " << met->Py() << " " << ptSF << " ";

  //Propagate to MET                                      
  met->SetPx(met->Px() + (1-ptSF)*nominalPx());                                               
  met->SetPy(met->Py() + (1-ptSF)*nominalPy());                                               

  Bool_t passPt = Pt() > _minPtCut;               
  Bool_t passEta = TMath::Abs(Eta()) < _maxEtaCut;

  if (passPt && passEta && passesIDs()) return kTRUE;
  else return kFALSE;

}

float Jet::GetStochasticFactor(float pt, float eta, float rho, std::vector<std::vector<std::string> > * resolution, TString * resFormula){

  TFormula resForm("",*resFormula);

  for (auto res : *resolution){
    //ski;p if not in the right bin
    if (eta < std::stof(res[0]) || eta > std::stof(res[1]) || rho < std::stof(res[2]) || rho > std::stof(res[3])) continue;
    resForm.SetParameter(0,std::stof(res[7]));
    resForm.SetParameter(1,std::stof(res[8]));
    resForm.SetParameter(2,std::stof(res[9]));
    resForm.SetParameter(3,std::stof(res[10]));
  }
  return resForm.Eval(pt);

}

float Jet::GetJerFromFile(float eta, std::vector<std::vector<std::string> > * resSFs, int central){
  for (auto res: *resSFs){
    if (eta < std::stof(res[0]) || eta > std::stof(res[1])) continue;
    return std::stof(res[central+3]);
  }
  return 1.;
}

float Jet::GetSmearFactor(float pt, float genPt, float eta, float rho, float jer_sf, std::vector<std::vector<std::string> > * resolution, TString * resFormula){

  //If we don't have a resFormula there was an error in reading the file, just return 1.
  if (*resFormula  == "") return 1.;

  float smearFactor = 1.;
  float relpterr = GetStochasticFactor(pt,eta,rho,resolution,resFormula);
  if (genPt > 0. && (abs(pt-genPt)<3*relpterr*pt)) {
    double dPt = pt - genPt;
    smearFactor = 1 + (jer_sf - 1.) * dPt / pt;
    //std::cout << "non stoch " << pt << " " << genPt << " " << relpterr << " " << jer_sf << " " << smearFactor << std::endl;
  }
  else { //stochastic smearing method when we don't have a gen jet
    //jer_sf = GetJerFromFile(Eta(),resSFs,0);
    //    float relpterr = GetStochasticFactor(pt,eta,rho,resolution,resFormula);
    smearFactor = 1 + jet_jer_myran.Gaus(0,relpterr)*std::sqrt(std::max(0.,(jer_sf*jer_sf)-1.));
    //    std::cout << "Stochastic! "<<pt << " " << smearFactor << std::endl;
    if (smearFactor <= 0.) smearFactor = 1.;
    //    double sigma = relpterr * std::sqrt(jer_sf*jer_sf-1);
    //smearFactor = (std::max(0.,1.+jet_jer_myran.Gaus(0,sigma)));
    
    //    std::cout << "jstoch term: " << pt << " " << relpterr << " " << jer_sf << " " << smearFactor << std::endl;

    //      std::cout << relpterr <<" "<<sigma<< " " << jet_jer_myran.Gaus(0,sigma)<<" " << jer_sf*jer_sf-1 << std::endl;
  }
  return smearFactor;
}
