/******************************************************************************
 * Muon.cpp                                                                   *
 *                                                                            *
 * Store information about final state muons                                  *
 *                                                                            *
 * Derived from Particle class                                                *
 *                                                                            *
 * Public Member Functions of Muon class                                      *
 *    Muon()                             -- Default Constructor               *
 *    Muon()                             -- Parameterized Constructor         *
 *    Muon()                             -- Constructor with Particle         *
 *    ~Muon()                            -- Destructor                        *
 *    Clear()                            -- Set contents to default           *
 *    Fill                               -- Fill muon from event tree         *
 *    FillFastSim                        -- Fill muon from FastSim tree       *
 *    SetEtCone                          -- Set ET for EM layer 0             *
 *    GetEtCone                          -- Set ET for EM layer 0             *
 *    EtCone                             -- Set ET for EM layer 0             *
 *    operator+=                         -- Overload +=                       *
 *    operator+                          -- Overload +                        *
 *    operator=                          -- Overload = Particle               *
 *    operator=                          -- Overload = const Muon             *
 *    operator=                          -- Overload =                        *
 *                                                                            *
 * Private Data Members of Muon class                                         *
 *    Double_t _EtCone20                     -- CAL Energy in cone around muon*
 *    Double_t _EtCone30                     -- CAL Energy in cone around muon*
 *                                                                            *
 * History                                                                    *
 *      16 June 2015 - Created by Huaqiao ZHANG                               *
 *****************************************************************************/
#include "SingleTopRootAnalysis/Particles/Recon/Muon.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Integrate classes into the Root system
ClassImp(Muon)

/******************************************************************************
 * Muon::Muon()                                                               *
 *                                                                            *
 * Default Constructor                                                        *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
  Muon::Muon() : Particle::Particle(), 
  _passTightId (kFALSE), 
  _passMediumId(kFALSE), 
  _passLooseId (kFALSE),
  _isGlobal    (kFALSE),
  _isTracker   (kFALSE),
  _isSoft      (kFALSE),
  _isHighPt    (kFALSE),
  _isPf        (kFALSE),
  _isoCharged  (0.0),
  _isoSum      (0.0),
  _isoCharParPt		(0.0),
  _isoNeutralHadron	(0.0),
  _isoPhoton		(0.0),
  _isoPU		(0.0),
  _chi2			(0.0),
  _dxy			(0.0),
  _dz			(0.0),
  _validHits		(0.0),
  _validHitsInner	(0.0),
  _matchedStat		(0.0),
  _TLayers		(0.0),
  _relIsoR04		(0.0),
  _ndof			(0.0),
  _charge		(0.0),
  _isTightMu            (kFALSE),
  _isVetoMu             (kFALSE),
  _isUnIsolatedMu       (kFALSE),
  _isTriggerMatchedMu   (kFALSE),
  _isMetMu              (kFALSE)

{

} //Muon

/******************************************************************************
 * Muon::~Muon()                                                              *
 *                                                                            *
 * Destructor                                                                 *  
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
Muon::~Muon()
{
  //cout<<"In muon destructor"<<endl;
} //~Muon

/******************************************************************************
 * Muon::Muon(const Muon& other)                                              *
 *                                                                            *
 * Copy constructor Muon const                                                *
 *                                                                            *
 * Input:  Muon                                                               *
 * Output: None                                                               *
 ******************************************************************************/
Muon::Muon(const Muon& other): Particle(other),
  _passTightId(other.GetpassTightId()),
  _passMediumId(other.GetpassMediumId()),
  _passLooseId(other.GetpassLooseId()),
  _isGlobal(other.GetisGlobal()),			       
  _isTracker(other.GetisTracker()),
  _isSoft(other.GetisSoft()),
  _isHighPt(other.GetisHighPt()),
  _isPf(other.GetisPf()),
  _isoCharged(other.GetisoCharged()),
  _isoSum(other.GetisoSum()),
  _isoCharParPt(other.GetisoCharParPt()),
  _isoNeutralHadron(other.GetisoNeutralHadron()),
  _isoPhoton(other.GetisoPhoton()),
  _isoPU(other.GetisoPU()),
  _chi2(other.Getchi2()),
  _dxy(other.Getdxy()),
  _dz(other.Getdz()),
  _validHits(other.GetvalidHits()),
  _validHitsInner(other.GetvalidHitsInner()),
  _matchedStat(other.GetmatchedStat()),
  _TLayers(other.GetTLayers()),
  _relIsoR04(other.GetrelIsoR04()),
  _ndof(other.Getndof()),
  _charge(other.GetCharge()),
  _isTightMu(other.GetisTightMu()),
  _isVetoMu(other.GetisVetoMu()),
  _isUnIsolatedMu(other.GetisUnIsolatedMu()),
  _isTriggerMatchedMu(other.GetisTriggerMatchedMu()),
  _isMetMu(other.GetisMetMu())

			       
{
} //Muon()

/******************************************************************************
 * Muon::Muon(const Muon& other)                                              *
 *                                                                            *
 * Copy constructor Particle const                                            *
 *                                                                            *
 * Input:  Muon                                                               *
 * Output: None                                                               *
 ******************************************************************************/
Muon::Muon(const Particle& other): Particle(other),
				   _passTightId (kFALSE), 
				   _passMediumId(kFALSE), 
				   _passLooseId(kFALSE),
				   _isGlobal   (kFALSE),
				   _isTracker  (kFALSE),
				   _isSoft     (kFALSE),
				   _isHighPt   (kFALSE),
				   _isPf       (kFALSE),
				   _isoCharged (0.0),
				   _isoSum     (0.0),
				   _isoCharParPt	(0.0),
				   _isoNeutralHadron	(0.0),
				   _isoPhoton		(0.0),
				   _isoPU		(0.0),
				   _chi2		(0.0),
				   _dxy			(0.0),
				   _dz			(0.0),
				   _validHits		(0.0),
				   _validHitsInner	(0.0),
                                   _matchedStat		(0.0),
                                   _TLayers		(0.0),
				   _relIsoR04            (0.0),
				   _ndof (0.0),
  _charge(0.0),
  _isTightMu(kFALSE),                  
  _isVetoMu(kFALSE),                    
  _isUnIsolatedMu(kFALSE),        
  _isTriggerMatchedMu(kFALSE),
  _isMetMu(kFALSE)                     
                                                   

{
} //Muon

/******************************************************************************
 * Muon& Muon::operator+=(const Muon& other)                                  *
 *                                                                            *
 * Overload +=                                                                *
 *                                                                            *
 * Input:  Muon                                                               *
 * Output: Muon                                                               *
 ******************************************************************************/
Muon& Muon::operator+=(const Muon& other)
{
  
  Particle::operator+=(other);
  return *this;
}

/******************************************************************************
 * Muon& Muon::operator+(const Muon& other)                                   *
 *                                                                            *
 * Overload +                                                                 *
 *                                                                            *
 * Input:  Muon                                                               *
 * Output: Muon                                                               *
 ******************************************************************************/
Muon Muon::operator+(const Muon& other)
{

  Particle particleTemp = Particle::operator+(other);
  return other;
} //+

/******************************************************************************
 * Muon& Muon::operator=(const Muon& other)                                   *
 *                                                                            *
 * Overload = Particle                                                        *
 *                                                                            *
 * Input:  Particle                                                           *
 * Output: Muon                                                               *
 ******************************************************************************/
Muon& Muon::operator=(const Particle& other)
{
  Particle::operator=(other);
  SetpassTightId (kFALSE);
  SetpassMediumId(kFALSE);
  SetpassLooseId (kFALSE);
  SetisGlobal    (kFALSE);
  SetisTracker   (kFALSE);
  SetisSoft      (kFALSE);
  SetisHighPt    (kFALSE);
  SetisPf        (kFALSE);
  SetisoCharged  (0.0);
  SetisoSum      (0.0);
  SetisoCharParPt	(0.0);
  SetisoNeutralHadron	(0.0);
  SetisoPhoton		(0.0);
  SetisoPU		(0.0);
  Setchi2		(0.0);
  Setdxy		(0.0);
  Setdz			(0.0);
  SetvalidHits		(0.0);
  SetvalidHitsInner	(0.0);
  SetmatchedStat	(0.0);
  SetTLayers		(0.0);
  SetrelIsoR04		(0.0);
  Setndof		(0.0);
  SetCharge		(0.0);
  SetisTightMu(kFALSE);
  SetisVetoMu(kFALSE);
  SetisUnIsolatedMu(kFALSE);
  SetisTriggerMatchedMu(kFALSE);
  SetisMetMu(kFALSE);
  return *this;
} //= Particle

/******************************************************************************
 * Muon& Muon::operator=(const Muon& other)                                   *
 *                                                                            *
 * Overload = const Muon                                                      *
 *                                                                            *
 * Input:  Muon                                                               *
 * Output: Muon                                                               *
 ******************************************************************************/
Muon& Muon::operator=(const Muon& other)
{
  Particle::operator=(other);
  SetpassTightId(other.GetpassTightId());
  SetpassMediumId(other.GetpassMediumId());
  SetpassLooseId(other.GetpassLooseId());
  SetisGlobal(other.GetisGlobal());
  SetisTracker(other.GetisTracker());
  SetisSoft(other.GetisSoft());
  SetisHighPt(other.GetisHighPt());
  SetisPf(other.GetisPf());
  SetisoCharged(other.GetisoCharged());
  SetisoSum(other.GetisoSum());
  SetisoCharParPt(other.GetisoCharParPt());
  SetisoNeutralHadron(other.GetisoNeutralHadron());
  SetisoPhoton(other.GetisoPhoton());
  SetisoPU(other.GetisoPU());
  Setchi2(other.Getchi2());
  Setdxy(other.Getdxy());
  Setdz(other.Getdz());
  SetvalidHits(other.GetvalidHits());
  SetvalidHitsInner(other.GetvalidHitsInner());
  SetmatchedStat(other.GetmatchedStat());
  SetTLayers(other.GetTLayers());
  SetrelIsoR04(other.GetrelIsoR04());
  Setndof(other.Getndof());
  SetCharge(other.GetCharge());
  SetisTightMu(other.GetisTightMu());         
  SetisVetoMu(other.GetisVetoMu());          
  SetisUnIsolatedMu(other.GetisUnIsolatedMu());    
  SetisTriggerMatchedMu(other.GetisTriggerMatchedMu());
  SetisMetMu(other.GetisMetMu());           

  return *this;
} //= const muon

/******************************************************************************
 * Muon& Muon::operator=(const Muon& other)                                   *
 *                                                                            *
 * Overload = non-const Muon                                                  *
 *                                                                            *
 * Input:  Muon                                                               *
 * Output: Muon                                                               *
 ******************************************************************************/
Muon& Muon::operator=(Muon& other)
{
  Particle::operator=(other);
  SetpassTightId(other.GetpassTightId());
  SetpassMediumId(other.GetpassMediumId());
  SetpassLooseId(other.GetpassLooseId());
  SetisGlobal(other.GetisGlobal());
  SetisTracker(other.GetisTracker());
  SetisSoft(other.GetisSoft());
  SetisHighPt(other.GetisHighPt());
  SetisPf(other.GetisPf());
  SetisoCharged(other.GetisoCharged());
  SetisoSum(other.GetisoSum());
  SetisoCharParPt(other.GetisoCharParPt());
  SetisoNeutralHadron(other.GetisoNeutralHadron());
  SetisoPhoton(other.GetisoPhoton());
  SetisoPU(other.GetisoPU());
  Setchi2(other.Getchi2());
  Setdxy(other.Getdxy());
  Setdz(other.Getdz());
  SetvalidHits(other.GetvalidHits());
  SetvalidHitsInner(other.GetvalidHitsInner());
  SetmatchedStat(other.GetmatchedStat());
  SetTLayers(other.GetTLayers());
  SetrelIsoR04(other.GetrelIsoR04());
  Setndof(other.Getndof());
  SetCharge(other.GetCharge());
  SetisTightMu(other.GetisTightMu());                  
  SetisVetoMu(other.GetisVetoMu());                    
  SetisUnIsolatedMu(other.GetisUnIsolatedMu());        
  SetisTriggerMatchedMu(other.GetisTriggerMatchedMu());
  SetisMetMu(other.GetisMetMu());                      

  return *this;
} //= non-const muon

/******************************************************************************         
 * void Muon::SetCuts(TEnv* config, TString muonType)                         * 
 *                                                                            *         
 * Set up the cuts for muonType                                               *
 *                                                                            *         
 * Input:  TEnv* config, TString muonType                                     *
 * Output: None                                                               *
 ******************************************************************************/
void Muon::SetCuts(TEnv* config, TString muonType)
{
  _minPtCuts[muonType] = config -> GetValue("ObjectID.Muon."+muonType+".MinPt", 100.0);
  _maxEtaCuts[muonType] = config -> GetValue("ObjectID.Muon."+muonType+".MaxEta", 0.0);
  _maxRelIsoCuts[muonType] = config -> GetValue("ObjectID.Muon."+muonType+".MaxRelIso", 100.0);
  _maxDxyCuts[muonType] = config -> GetValue("ObjectID.Muon."+muonType+".MaxDxy", 100.0);
  _maxDzCuts[muonType] = config -> GetValue("ObjectID.Muon."+muonType+".MaxDz", 100.0);

}

/******************************************************************************         
 * void Muon::Fill(nanoAODTree *evtr, Int_t iE)                                 *         
 *                                                                            *         
 * Fill Muon vector from nanoAODTree                                                 *         
 *                                                                            *         
 * Input:  nanoAODTree tree                                                         *         
 * Output: kTRUE if the muon passes object ID cuts                            *         
 ******************************************************************************/
Bool_t Muon::Fill(nanoAODTree *evtr,int iE, Bool_t isSimulation)
{

  Double_t muPt     = evtr -> Muon_pt              [iE];
  Double_t muEta    = evtr -> Muon_eta             [iE];
  Double_t muPhi    = evtr -> Muon_phi             [iE];
  Double_t muM      = evtr -> Muon_mass            [iE];
  Double_t muCharge = evtr -> Muon_charge          [iE];
  SetrelIsoR04      ( evtr -> Muon_pfRelIso04_all  [iE] );
  SetpassTightId    ( evtr -> Muon_tightId         [iE] );
  SetpassMediumId   ( evtr -> Muon_mediumId        [iE] );
  SetisGlobal       ( evtr -> Muon_isGlobal        [iE] );
  SetisTracker      ( evtr -> Muon_isTracker       [iE] );
  Setdxy            ( evtr -> Muon_dxy             [iE] );
  Setdz             ( evtr -> Muon_dz              [iE] );
  SetisPf           ( evtr -> Muon_isPFcand        [iE] );

  SetPtEtaPhiM(muPt, muEta, muPhi, muM); 

  SetisTightMu      ( ApplyCuts( "Tight"        ) );
  SetisVetoMu       ( ApplyCuts( "Veto"         ) );
  SetisUnIsolatedMu ( ApplyCuts( "UnIsolated"   ) );
  SetisTriggerMatchedMu      ( kFALSE ); // This is done elsewhere
  SetisMetMu        ( ApplyCuts( "MetMu"        ) );

  return kTRUE;

}//Fill(nanoAODTree)

/******************************************************************************         
 * void Muon::Fill(EventTree *evtr, Int_t iE)                                 *         
 *                                                                            *         
 * Fill Muon vector from tree                                                 *         
 *                                                                            *         
 * Input:  Event Tree                                                         *         
 * Output: kTRUE if the muon passes object ID cuts                            *         
 ******************************************************************************/
Bool_t Muon::Fill(EventTree *evtr,int iE,TString muonType, Bool_t isSimulation)
{
  // **************************************************************
  // Check muon type
  // **************************************************************
  if( (muonType != "All") && (muonType != "Veto") && (muonType != "Tight") && (muonType != "Isolated") && (muonType != "UnIsolated") && (muonType != "PtEtaCut") ){
    std::cout << "ERROR: <Muon::Fill()> " << "Passed variable muonType of value " << muonType << " is not valid.  "
	      << "Must be All, Veto, Tight, Isolated, UnIsolated, PtEtaCut" << std::endl;
  } //if
  
  // **************************************************************
  // Fill muon
  // **************************************************************
  Double_t muPt     = evtr -> Muon_pt       -> operator[](iE);
  Double_t muEta    = evtr -> Muon_eta      -> operator[](iE);
  Double_t muPhi    = evtr -> Muon_phi      -> operator[](iE);
  Double_t muE      = evtr -> Muon_energy   -> operator[](iE);
  Double_t muCharge = evtr -> Muon_charge   -> operator[](iE);
  SetPtEtaPhiE(muPt, muEta, muPhi, muE);

  SetpassTightId	(evtr -> Muon_tight   		-> operator[](iE));
  SetpassLooseId	(evtr -> Muon_loose   		-> operator[](iE));
  SetisSoft     	(evtr -> Muon_soft    		-> operator[](iE));
  SetisHighPt   	(evtr -> Muon_isHighPt		-> operator[](iE));
  SetisPf       	(evtr -> Muon_pf      		-> operator[](iE));
  SetisoCharged 	(evtr -> Muon_isoR04Charged 	-> operator[](iE));
  SetisoSum     	(evtr -> Muon_isoSum  		-> operator[](iE));
  SetisoCharParPt	(evtr -> Muon_isoR04CharParPt  	-> operator[](iE));
  SetisoNeutralHadron	(evtr -> Muon_isoR04NeutralHadron  -> operator[](iE));
  SetisoPhoton		(evtr -> Muon_isoR04Photon   	-> operator[](iE));
  SetisoPU		(evtr -> Muon_isoR04PU   		-> operator[](iE));
  Setchi2		(evtr -> Muon_chi2   		-> operator[](iE));
  Setdxy		(evtr -> Muon_dxy_pv   		-> operator[](iE));
  Setdz			(evtr -> Muon_dz_pv   		-> operator[](iE));
  SetvalidHits		(evtr -> Muon_validHits   	-> operator[](iE));
  SetvalidHitsInner	(evtr -> Muon_validHitsInner   	-> operator[](iE));
  SetmatchedStat	(evtr -> Muon_matchedStat   	-> operator[](iE));
  SetTLayers		(evtr -> Muon_TLayers   	-> operator[](iE));
  SetrelIsoR04		(evtr -> Muon_relIsoDeltaBetaR04-> operator[](iE));
  Setndof		(evtr -> Muon_ndof		-> operator[](iE));
  SetCharge		(evtr -> Muon_charge		-> operator[](iE));

  return ApplyCuts(muonType);
 
} //Fill(EventTree)


/******************************************************************************  
 * Bool_t Muon::ApplyCuts(TString muonType)                                   *  
 *                                                                            *  
 * Apply the relevant cuts based on muon type selecting                       *  
 *                                                                            *  
 * Input:  TString muonType                                                   *  
 * Output: kTRUE if this muon passes muon ID cuts                             *  
 ******************************************************************************/
Bool_t Muon::ApplyCuts(TString muonType)
{
  // **************************************************************
  // Run 2 relative isolation cuts
  // **************************************************************
  Bool_t passRelIso = kTRUE;

  if (relIsoR04() > _maxRelIsoCuts[muonType]) {passRelIso = kFALSE; }
  
  // **************************************************************
  // Pt and Eta Cuts
  // **************************************************************
  // If event passes or fails requirements
  Bool_t passMinPt  = kTRUE;
  Bool_t passMaxEta = kTRUE;

   // Test Requirements
  if(Pt() <= _minPtCuts[muonType])               passMinPt  = kFALSE;
  if(TMath::Abs(Eta()) >= _maxEtaCuts[muonType]) passMaxEta = kFALSE;

  // **************************************************************
  // Impact parameter cuts
  // **************************************************************
  Bool_t passImpactParam = kTRUE;

  if (dxy() > _maxDxyCuts[muonType] || dz() > _maxDzCuts[muonType]) passImpactParam = kFALSE;

  // **************************************************************
  // Moer complicated ID selection
  // **************************************************************
  Bool_t passMetId = kTRUE;
  
  if (! (passMediumId() && (isGlobal() || isTracker()))) passMetId = kFALSE;


  //Set up the ID requirements for the event container
  if (muonType == "MetMu")      return (passMinPt && passMaxEta  && passMetId && passRelIso && passImpactParam);
  if (muonType == "Tight")      return (passMinPt && passMaxEta  && passTightId() && passRelIso);
  if (muonType == "Veto")       return (passMinPt && passMaxEta);//no isolation req. or inner det or jet overlap.
  if (muonType == "UnIsolated") return (passMinPt && passMaxEta  && passTightId() && ! passRelIso); //The same as tight muons, but with an inverted isolation requirement

  //This is True for the "All" collection of muons
  return kTRUE;

} //ApplyCuts()
 
/******************************************************************************         
 * void Muon::FillFastSim(FastSimTree *tr, Int_t iE)                          *         
 *                                                                            *         
 * Fill Muon vector from tree                                                 *         
 *                                                                            *         
 * Input:  FastSim Tree                                                       *         
 * Output: kTRUE if the muon passes object ID cuts                            *         
 ******************************************************************************/
Bool_t Muon::FillFastSim(TEnv *config,FastSimTree *tr,int iE,TString muonType)
{

} //FillFastSim()


