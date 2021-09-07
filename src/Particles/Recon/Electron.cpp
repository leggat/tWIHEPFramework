/******************************************************************************
 * Electron.hpp                                                               *
 *                                                                            *
 * Store information about final state electrons                              *
 *                                                                            *
 * Derived from Particle class                                                *
 *                                                                            *
 * Public Member Functions of Electron class                                  *
 *    Electron()                         -- Default Constructor               *
 *    Electron()                         -- Parameterized Constructor         *
 *    Clear()                            -- Set contents to default           *
 *    Fill                               -- Fill electron from event tree     *
 *    FillFastSim                        -- Fill electron from FastSim tree   *
 *    operator+=                         -- Overload +=                       *
 *    operator+                          -- Overload +                        *
 *    operator=                          -- Overload = Particle               *
 *    operator=                          -- Overload = const Electron         *
 *    operator =                         -- Overload =                        *
 *                                                                            *
 * Private Data Members of Electron class                                     *
 *                                                                            *
 * History                                                                    *
 *      16 June 2015 - Created by Huaqiao ZHANG                               *
 *****************************************************************************/
#include "SingleTopRootAnalysis/Particles/Recon/Electron.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Integrate classes into the Root system
ClassImp(Electron)


/******************************************************************************
 * Electon::Electron()                                                        *
 *                                                                            *
 * Default Constructor                                                        *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
Electron::Electron() : Particle::Particle(),_passVetoId(0), _passLooseId(0), _passMediumId(0), _passTightId(0), _passHEEPId(0), _passConversionVeto(0), _expectedMissingInnerHits(0), _patElectron_d0(0.0),  _patElectron_dz(0.0),  _isoChargedHadrons(0.0),  _isoNeutralHadrons(0.0), _isoPhotons(0.0),  _isoPU(0.0), _charge(0.0), _scEta(0.0), _inCrack(0), _sigmaEtaEta(0.), _dEtaInSeed(0.), _dPhiIn(0.), _hOverE(0.), _ooEmooP(0.), _relIsoPFRhoEA(0.), _relIsoPFBeta(0.), _missingHits(0.)
{
} //Electron()

/******************************************************************************
 * Electon::~Electron()                                                       *
 *                                                                            *
 * Default Constructor                                                        *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
Electron::~Electron()
{

} //~Electron


/******************************************************************************
 * Electon::Electron(const Electron& other)                                   *
 *                                                                            *
 * Copy constructor Electron const                                            *
 *                                                                            *
 * Input:  Electron                                                           *
 * Output: None                                                               *
 ******************************************************************************/
Electron::Electron(const Electron& other): Particle(other),
_passVetoId(other.GetpassVetoId()), 
_passLooseId(other.GetpassLooseId()), 
_passMediumId(other.GetpassMediumId()), 
_passTightId(other.GetpassTightId()), 
_passHEEPId(other.GetpassHEEPId()), 
_passConversionVeto(other.GetpassConversionVeto()), 
_expectedMissingInnerHits(other.GetexpectedMissingInnerHits()), 
_patElectron_d0(other.GetpatElectron_d0()), 
_patElectron_dz(other.GetpatElectron_dz()), 
_isoChargedHadrons(other.GetisoChargedHadrons()), 
_isoNeutralHadrons(other.GetisoNeutralHadrons()), 
_isoPhotons(other.GetisoPhotons()), 
					   _isoPU(other.GetisoPU()),
					   _charge(other.GetCharge()),
					   _scEta(other.scEta()),
					   _inCrack(other.inCrack()),
					   _sigmaEtaEta(other.sigmaEtaEta()), 
					   _dEtaInSeed(other.dEtaInSeed()), 
					   _dPhiIn(other.dPhiIn()), 
					   _hOverE(other.hOverE()), 
					   _ooEmooP(other.ooEmooP()), 
					   _relIsoPFRhoEA(other.relIsoPFRhoEA()), 
  _relIsoPFBeta(other.relIsoPFBeta()),
					   _missingHits(other.missingHits())
{
} //Electron()

/******************************************************************************
 * Electon::Electron(const Particle& other)                                   *
 *                                                                            *
 * Copy constructor Particle const                                            *
 *                                                                            *
 * Input:  Particle                                                           *
 * Output: None                                                               *
 ******************************************************************************/
Electron::Electron(const Particle& other) : Particle(other),_passVetoId(0), _passLooseId(0), _passMediumId(0), _passTightId(0), _passHEEPId(0), _passConversionVeto(0), _expectedMissingInnerHits(0), _patElectron_d0(0.0),  _patElectron_dz(0.0),  _isoChargedHadrons(0.0),  _isoNeutralHadrons(0.0), _isoPhotons(0.0),  _isoPU(0.0), _charge(0.0), _scEta(0.0), _inCrack(0), _sigmaEtaEta(0.), _dEtaInSeed(0.), _dPhiIn(0.), _hOverE(0.), _ooEmooP(0.), _relIsoPFRhoEA(0.), _relIsoPFBeta(0.),_missingHits(0.)
{ 
} //Electron()


/******************************************************************************
 * Electron& Electron::operator+=(const Electron& other)                      *
 *                                                                            *
 * Overload +=                                                                *
 *                                                                            *
 * Input:  Electron                                                           *
 * Output: Electron                                                           *
 ******************************************************************************/
Electron& Electron::operator+=(const Electron& other)
{
  
  Particle::operator+=(other);
  return *this;
} //Electron +=

/******************************************************************************
 * Electron& Electron::operator+(const Electron& other)                       *
 *                                                                            *
 * Overload +                                                                 *
 *                                                                            *
 * Input:  Electron                                                           *
 * Output: Electron                                                           *
 ******************************************************************************/
Electron Electron::operator+(const Electron& other)
{

  Particle particleTemp = Particle::operator+(other);
  Electron electronTemp = particleTemp;
  return electronTemp;

} //+

/******************************************************************************
 * Electron& Electron::operator=(const Electron& other)                       *
 *                                                                            *
 * Overload = Particle                                                        *
 *                                                                            *
 * Input:  Electron                                                           *
 * Output: Electron                                                           *
 ******************************************************************************/
Electron& Electron::operator=(const Particle& other)
{
  
  Particle::operator = (other);
  SetpassVetoId(0.0);
  SetpassLooseId(0.0);
  SetpassMediumId(0.0);
  SetpassTightId(0.0);
  SetpassHEEPId(0.0);
  SetpassConversionVeto(0.0);
  SetexpectedMissingInnerHits(0.0);
  SetpatElectron_d0(0.0);
  SetpatElectron_dz(0.0);
  SetisoChargedHadrons(0.0);
  SetisoNeutralHadrons(0.0);
  SetisoPhotons(0.0);
  SetisoPU(0.0);
  SetCharge(0.0);
  SetSCeta(0.0);
  SetInCrack(0);
  SetSigmaEtaEta(0.);
  SetDEtaInSeed(0.);
  SetDPhiIn(0.);
  SetHOverE(0.);
  SetOoEmooP(0.);
  SetRelIsoPFRhoEA(0.);
  SetRelIsoPFBeta(0.);
  SetMissingHits(0);

  return *this;
} // = Particle

/******************************************************************************
 * Electron& Electron::operator+(const Electron& other)                       *
 *                                                                            *
 * Overload = for const Electron                                              *
 *                                                                            *
 * Input:  Electron                                                           *
 * Output: Electron                                                           *
 ******************************************************************************/
Electron& Electron::operator=(const Electron& other)
{
  
  Particle::operator=(other);
  SetpassVetoId(other.GetpassVetoId());
  SetpassLooseId(other.GetpassLooseId());
  SetpassMediumId(other.GetpassMediumId());
  SetpassTightId(other.GetpassTightId());
  SetpassHEEPId(other.GetpassHEEPId());
  SetpassConversionVeto(other.GetpassConversionVeto());
  SetexpectedMissingInnerHits(other.GetexpectedMissingInnerHits());
  SetpatElectron_d0(other.GetpatElectron_d0());
  SetpatElectron_dz(other.GetpatElectron_dz());
  SetisoChargedHadrons(other.GetisoChargedHadrons());
  SetisoNeutralHadrons(other.GetisoNeutralHadrons());
  SetisoPhotons(other.GetisoPhotons());
  SetisoPU(other.GetisoPU());
  SetCharge(other.GetCharge());
  SetSCeta(other.GetSCEta());
  SetInCrack(other.GetInCrack());
  SetSigmaEtaEta(other.sigmaEtaEta());
  SetDEtaInSeed(other.dEtaInSeed());
  SetDPhiIn(other.dPhiIn());
  SetHOverE(other.hOverE());
  SetOoEmooP(other.ooEmooP());
  SetRelIsoPFRhoEA(other.relIsoPFRhoEA());
  SetRelIsoPFBeta(other.relIsoPFBeta());
  SetMissingHits(other.missingHits());
  return *this;
} //= const Electron

/******************************************************************************
 * Electron& Electron::operator+(const Electron& other)                       *
 *                                                                            *
 * Overload = for non-const Electron                                          *
 *                                                                            *
 * Input:  Electron                                                           *
 * Output: Electron                                                           *
 ******************************************************************************/
Electron& Electron::operator=(Electron& other)
{
  
  Particle::operator=(other);
  SetpassVetoId(other.GetpassVetoId());
  SetpassLooseId(other.GetpassLooseId());
  SetpassMediumId(other.GetpassMediumId());
  SetpassTightId(other.GetpassTightId());
  SetpassHEEPId(other.GetpassHEEPId());
  SetpassConversionVeto(other.GetpassConversionVeto());
  SetexpectedMissingInnerHits(other.GetexpectedMissingInnerHits());
  SetpatElectron_d0(other.GetpatElectron_d0());
  SetpatElectron_dz(other.GetpatElectron_dz());
  SetisoChargedHadrons(other.GetisoChargedHadrons());
  SetisoNeutralHadrons(other.GetisoNeutralHadrons());
  SetisoPhotons(other.GetisoPhotons());
  SetisoPU(other.GetisoPU());
  SetCharge(other.GetCharge());
  SetSCeta(other.GetSCEta());
  SetInCrack(other.GetInCrack());
  SetSigmaEtaEta(other.sigmaEtaEta());
  SetDEtaInSeed(other.dEtaInSeed());
  SetDPhiIn(other.dPhiIn());
  SetHOverE(other.hOverE());
  SetOoEmooP(other.ooEmooP());
  SetRelIsoPFRhoEA(other.relIsoPFRhoEA());
  SetRelIsoPFBeta(other.relIsoPFBeta());
  SetMissingHits(other.missingHits());
  return *this;
} //= non-const Electron

/******************************************************************************
 * void Electron::SetCuts(TEnv* config, TString electron)                     *
 *                                                                            *
 * Set up the cuts to be made on the different types of electron              *
 *                                                                            *
 * Input:  config with cuts                                                   *
 *         string of what type of electron, Veto, Tight etc                   *
 * Output: kTRUE if this electron passes electron ID cuts                     *
 ******************************************************************************/
void Electron::SetCuts(TEnv* config, TString electronType)
{
  _maxEtaCuts[electronType] =    config -> GetValue("ObjectID.Electron." + electronType + ".MaxEta", 100.);
  _minPtCuts[electronType] =   	 config -> GetValue("ObjectID.Electron." + electronType + ".MinPt", 0.);
  _minEtaGapCuts[electronType] = config -> GetValue("ObjectID.Electron." + electronType + ".MinEtaGap", 100.);
  _maxEtaGapCuts[electronType] = config -> GetValue("ObjectID.Electron." + electronType + ".MaxEtaGap", -100.);
  _dZCutEndcap[electronType] = config -> GetValue("ObjectID.Electron." + electronType + ".dZEndcap", 100.);
  _dZCutBarrel[electronType] = config -> GetValue("ObjectID.Electron." + electronType + ".dZBarrel", 100.);
  _d0CutEndcap[electronType] = config -> GetValue("ObjectID.Electron." + electronType + ".d0Endcap", 100.);
  _d0CutBarrel[electronType] = config -> GetValue("ObjectID.Electron." + electronType + ".d0Barrel", 100.);

} // End SetCuts

/******************************************************************************
 * void Electron::Fill(nanoAODTree *evtr,int iE)                                *
 *                                                                            *
 * Fill electron vector from a nanoAODTree                                             *
 *                                                                            *
 * Input:  nanoAOD Tree                                                         *
 * Output: kTRUE if this electron passes electron ID cuts                     *
 ******************************************************************************/
Bool_t Electron::Fill(nanoAODTree *evtr, Int_t iE, TString electronType, Bool_t isSimulation)
{

  // ******************************  Set up electron candidate ********************************
  Double_t elPt        = evtr -> Electron_pt      [iE];                         
  Double_t elEta       = evtr -> Electron_eta     [iE];                         
  Double_t elPhi       = evtr -> Electron_phi     [iE];                         
  Double_t elM         = evtr -> Electron_mass    [iE];                         

  SetPtEtaPhiM(elPt, elEta, elPhi, elM);

  SetCharge             ( evtr -> Electron_charge [iE] );

  //Cut based ID. Can be changed to MVA if required.
  Int_t idBit = evtr -> Electron_cutBased [iE];

  SetpassVetoId         ( idBit > 0 );
  SetpassLooseId        ( idBit > 1 );
  SetpassMediumId       ( idBit > 2 );
  SetpassTightId        ( idBit > 3 );

  
  SetpassHEEPId         ( evtr -> Electron_cutBased_HEEP   [iE] );
  SetpatElectron_d0     ( evtr -> Electron_dxy             [iE] );
  SetpatElectron_dz     ( evtr -> Electron_dz              [iE] );

  return ApplyCuts(electronType);


} // Fill(nanoAODTree);


/******************************************************************************
 * void Electron::Fill(EventTree *evtr,int iE)                                *
 *                                                                            *
 * Fill electron vector from tree                                             *
 *                                                                            *
 * Input:  Event Tree                                                         *
 * Output: kTRUE if this electron passes electron ID cuts                     *
 ******************************************************************************/
Bool_t Electron::Fill(EventTree *evtr, Int_t iE, TString electronType, Bool_t isSimulation)
{

  // **************************************************************
  // Check electron type
  // **************************************************************
  if( (electronType != "All") && (electronType != "Veto") && (electronType != "Tight") && (electronType != "Isolated") && (electronType != "UnIsolated") && (electronType != "PtEtaCut") ){
    std::cout << "ERROR: <Electron::Fill()> " << "Passed variable electronType of value " << electronType << " is not valid.  "
	      << "Must be All, Veto, Tight, Isolated, UnIsolated, PtEtaCut" << std::endl;
  } 

  // ******************************  Set up electron candidate ********************************
  Double_t elPt        = evtr -> patElectron_pt     -> operator[](iE);
  Double_t elEta       = evtr -> patElectron_eta    -> operator[](iE);
  Double_t elPhi       = evtr -> patElectron_phi    -> operator[](iE);
  Double_t elE         = evtr -> patElectron_energy -> operator[](iE);

 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  SetPtEtaPhiE(elPt, elEta, elPhi, elE);

  SetCharge             ( evtr -> patElectron_charge                   -> operator[](iE) );
  if (NULL != evtr -> patElectron_isPassVeto){
    SetpassVetoId         ( evtr -> patElectron_isPassVeto               -> operator[](iE) );
    SetpassLooseId        ( evtr -> patElectron_isPassLoose              -> operator[](iE) );
    SetpassMediumId       ( evtr -> patElectron_isPassMedium             -> operator[](iE) );
    SetpassHEEPId         ( evtr -> patElectron_isPassHEEPId             -> operator[](iE) );
  
  }
  else{
    SetpassVetoId(true);
    SetpassLooseId(true);
    SetpassMediumId(true);
    SetpassTightId(true);
    SetpassHEEPId(true);
  }
  if (NULL != evtr -> patElectron_isPassTight) {     SetpassTightId        ( evtr -> patElectron_isPassTight              -> operator[](iE) );}
  else SetpassTightId(true);


  SetpassConversionVeto ( evtr -> patElectron_passConversionVeto       -> operator[](iE) );
  SetisoChargedHadrons  ( evtr -> patElectron_isoChargedHadrons        -> operator[](iE) );
  SetisoNeutralHadrons  ( evtr -> patElectron_isoNeutralHadrons        -> operator[](iE) );
  SetisoPhotons         ( evtr -> patElectron_isoPhotons               -> operator[](iE) );
  SetisoPU              ( evtr -> patElectron_isoPU                    -> operator[](iE) );
  SetCharge		( evtr -> patElectron_charge		       -> operator[](iE) );
  SetSCeta		( evtr -> patElectron_SCeta		       -> operator[](iE) );
  SetInCrack		( evtr -> patElectron_inCrack		       -> operator[](iE) );
  SetpatElectron_d0	( evtr -> patElectron_d0		       -> operator[](iE) );
  SetpatElectron_dz	( evtr -> patElectron_gsfTrack_dz_pv	       -> operator[](iE) );
  SetSigmaEtaEta	( evtr -> patElectron_full5x5_sigmaIetaIeta    -> operator[](iE) );
  if (NULL != evtr -> patElectron_dEtaInSeed)  SetDEtaInSeed		( evtr -> patElectron_dEtaInSeed               -> operator[](iE) ); //This is the actual definition, but it on;y exists in re-processed samples.
  else SetDEtaInSeed	( evtr -> patElectron_dEtaIn                   -> operator[](iE) ); //Fallback option if the dataset hasn't been reprocessed yet.
  SetDPhiIn		( evtr -> patElectron_dPhiIn		       -> operator[](iE) );
  SetHOverE		( evtr -> patElectron_hOverE		       -> operator[](iE) );
  SetOoEmooP		( evtr -> patElectron_ooEmooP		       -> operator[](iE) );
  SetRelIsoPFRhoEA	( evtr -> patElectron_relIsoRhoEA	       -> operator[](iE) );
  SetRelIsoPFBeta	( evtr -> patElectron_relIsoDeltaBeta	       -> operator[](iE) );
  SetMissingHits	( evtr -> patElectron_expectedMissingInnerHits -> operator[](iE) );
  
  //Redefine reliso with corrected EA
  float rho = evtr -> EVENT_rhopog;                                                                                 
  float effArea = get_effarea(scEta());                                                                             
  SetRelIsoPFRhoEA((isoChargedHadrons() + (std::max( 0.0, isoNeutralHadrons() + isoPhotons() - rho*effArea)))/elPt);

  
  // Apply the ID requirements individually so that we can invert the isolation for QCD electrons
  // For BSM framework electrons only. Sets the TightID property
  Bool_t passIDnoIso = kTRUE;
  Bool_t passIso     = kTRUE;
  Bool_t passInvIso  = kTRUE;
  
  if (TMath::Abs(scEta()) <= 1.479){ //Electron in barrel
    if (sigmaEtaEta() >= 0.00998 ||
	TMath::Abs(dEtaInSeed()) >= 0.00308 ||
	TMath::Abs(dPhiIn()) >= 0.0816 ||
	hOverE() >= 0.0414 ||
	TMath::Abs(ooEmooP()) >= 0.0129 ||
	missingHits() > 1 ||
	!passConversionVeto())
      passIDnoIso = kFALSE;
    if (relIsoPFRhoEA() > 0.0588){
      passIso = kFALSE;
    }
    if (relIsoPFRhoEA() < 0.06 || relIsoPFRhoEA() > 0.5){
      passInvIso = kFALSE;
    }
  }
  else { // Electron in endcap
    if (sigmaEtaEta() >= 0.0292 ||
	TMath::Abs(dEtaInSeed()) >= 0.00605 ||
	TMath::Abs(dPhiIn()) >= 0.0394 ||
	hOverE() >= 0.0641 ||
	TMath::Abs(ooEmooP()) >= 0.0129 ||
	missingHits() > 1 ||
	!passConversionVeto())
      passIDnoIso = kFALSE;
    if (relIsoPFRhoEA() > 0.0571) {
      passIso = kFALSE;
    }
    if (relIsoPFRhoEA() < 0.06 || relIsoPFRhoEA() > 0.5){
      passInvIso = kFALSE;
    }
  }
  
  SetpassTightId ( passIso && passIDnoIso );
  SetpassUnIsolatedId( passInvIso && passIDnoIso );

  return ApplyCuts(electronType);

}//Fill(EventTree)

/****************************************************************************** 
 * void Electron::ApplyCuts(TString electronType)                             * 
 *                                                                            * 
 * Apply the relevant cuts based on electron type selecting                   * 
 *                                                                            * 
 * Input:  TString electronType                                               * 
 * Output: kTRUE if this electron passes electron ID cuts                     * 
 ******************************************************************************/

Bool_t Electron::ApplyCuts(TString electronType){


  // **************************************************************
  // Pt and Eta Cuts
  // **************************************************************
  // If event passes or fails requirements
  Bool_t passMinPt   = kTRUE;
  Bool_t passMaxEta  = kTRUE;
  Bool_t passTight   = kTRUE;
m Bool_t passIDnoIso = kTRUE;
  Bool_t passd0dZ    = kTRUE;
  Bool_t passIso     = kTRUE;
  Bool_t passInvIso  = kTRUE;


  // Test requirements
  if(Pt() <= _minPtCuts[electronType])               passMinPt  = kFALSE;
  if(TMath::Abs(Eta()) >= _maxEtaCuts[electronType]) passMaxEta = kFALSE;
  if(!passTightId())				     passTight  = kFALSE;
  
  
  // Checking that the d0 and dz cuts pass - are these still necessary in nanoAOD?
  if(TMath::Abs(Eta()) < 1.4442) passd0dZ = ((GetpatElectron_d0() < _d0CutBarrel[electronType]) && (GetpatElectron_dz() < _dZCutBarrel[electronType])); //barrel
  else passd0dZ = ((GetpatElectron_d0() < _d0CutEndcap[electronType]) && (GetpatElectron_dz() < _dZCutEndcap[electronType])); //endcap



  // **************************************************************
  // Gap Electrons
  // **************************************************************
  // If event passes or fails requirements
  Bool_t passNoGapElectron  = kTRUE;
  
  // Test requirements and set variable
  if( (TMath::Abs(scEta()) >= _minEtaGapCuts[electronType]) && (TMath::Abs(scEta()) <= _maxEtaGapCuts[electronType]) ) passNoGapElectron = kFALSE;

  
  // **************************************************************
  // Return value according to electron type passed to Fill
  // **************************************************************

  if(     "Tight"      == electronType) return( passMinPt && passMaxEta && passTight && passNoGapElectron && passd0dZ);
  else if("Veto"       == electronType) return( passMinPt && passMaxEta);//no tight or isolation req.
  else if(     "UnIsolated" == electronType) return( passMinPt && passMaxEta && passUnIsolatedId && passNoGapElectron && passd0dZ);

  return kTRUE;
  
} //ApplyCuts

/******************************************************************************
 * void Electron::FillFastSim(FastSimTree *evtr,int iE)                       *
 *                                                                            *
 * Fill electron vector from tree                                             *
 *                                                                            *
 * Input:  FastSim Tree                                                       *
 * Output: kTRUE if this electron passes electron ID cuts                     *
 ******************************************************************************/
Bool_t Electron::FillFastSim(FastSimTree *tr, Int_t iE,TEnv* config,TString electronType)
{
} //FillFastSim


double Electron::get_effarea(double eta){
  double effarea = -1;                              
  if(abs(eta) < 1.0)        effarea = 0.1703;       
  else if(abs(eta) < 1.479) effarea = 0.1715;       
  else if(abs(eta) < 2.0)   effarea = 0.1213;       
  else if(abs(eta) < 2.2)   effarea = 0.1230;       
  else if(abs(eta) < 2.3)   effarea = 0.1635;       
  else if(abs(eta) < 2.4)   effarea = 0.1937;       
  else                      effarea = 0.2393;       
  return effarea;                                   
}                                                   
