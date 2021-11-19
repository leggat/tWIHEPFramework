/******************************************************************************
 * TriggerObj.hpp                                                                    *
 *                                                                            *
 * Store information about final state TriggerObjs                                   *
 *                                                                            *
 * Derived from Particle class                                                *
 *                                                                            *
 * Public Member Functions of TriggerObj class                                       *
 *    TriggerObj()                              -- Default Constructor               *
 *    TriggerObj()                              -- Parameterized Constructor         *
 *    TriggerObj()                              -- Constructor with Particle         *
 *    ~TriggerObj()                             -- Destructor                        *
 *    Clear()                            -- Set contents to default           *
 *    Fill                               -- Fill trigger object from event tree          *
 *    FillFastSim                        -- Fill trigger object from FastSim tree        *
 *    SetSetNumTracks                    -- Set Num Tracks                    *
 *    GetSetNumTracks                    -- Set Num Tracks                    *
 *    NumTracks                          -- Set Num Tracks                    *
 *    operator+=                         -- Overload +=                       *
 *    operator+                          -- Overload +                        *
 *    operator=                          -- Overload = Particle               *
 *    operator=                          -- Overload = const TriggerObj              *
 *    operator=                          -- Overload =                        *
 *                                                                            *
 * Private Data Members of TriggerObj class                                          *
 *    Double_t _id                -- CAL Energy in cone around TriggerObj     *
 *                                                                            *
 * History                                                                    *
 *      14 Nov 2006 - Created by R. Schwienhorst                              *
 *      20 Nov 2006 - Modified by P. Ryan - cleanup and reorganized           *
 *      20 Mar 2007 - RS: Add filling from fastsim tree                       *
 *      11 Jan 2008 - RS: Fill only if object passes ID cuts                  *
 *****************************************************************************/
#include "SingleTopRootAnalysis/Particles/Recon/TriggerObj.hpp"
#include <iostream>
#include <string>
#include <fstream>


using namespace std;

// Integrate classes into the Root system
ClassImp(TriggerObj)
/******************************************************************************
 * TriggerObj::TriggerObj()                                                                 *
 *                                                                            *
 * Default Constructor                                                        *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
TriggerObj::TriggerObj() : Particle::Particle(),_id(0.0)
{

} //TriggerObj()

/******************************************************************************
 * TriggerObj::~TriggerObj()                                                              *
 *                                                                            *
 * Destructor                                                                 *  
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
TriggerObj::~TriggerObj()
{

} //~TriggerObj()


/******************************************************************************
 * TriggerObj::TriggerObj(const TriggerObj& other)                                              *
 *                                                                            *
 * Copy constructor TriggerObj const                                                *
 *                                                                            *
 * Input:  TriggerObj                                                               *
 * Output: None                                                               *
 ******************************************************************************/
TriggerObj::TriggerObj(const TriggerObj& other): Particle(other),_id(other.Getid())
{

} //TriggerObj()

/******************************************************************************
 * TriggerObj::TriggerObj(const TriggerObj& other)                                                 *
 *                                                                            *
 * Copy constructor Particle const                                            *
 *                                                                            *
 * Input:  TriggerObj                                                                *
 * Output: None                                                               *
 ******************************************************************************/
TriggerObj::TriggerObj(const Particle& other): Particle(other),_id(0)
{

} //TriggerObj() particle

/******************************************************************************
 * TriggerObj& TriggerObj::operator+=(const TriggerObj& other)                                     *
 *                                                                            *
 * Overload +=                                                                *
 *                                                                            *
 * Input:  TriggerObj                                                                *
 * Output: TriggerObj                                                                *
 ******************************************************************************/
TriggerObj& TriggerObj::operator+=(const TriggerObj& other)
{
  Particle::operator+=(other);
  // Also add NumTracks
  
  return *this;
} //+=

/******************************************************************************
 * TriggerObj& TriggerObj::operator+(const TriggerObj& other)                                      *
 *                                                                            *
 * Overload +                                                                 *
 *                                                                            *
 * Input:  TriggerObj                                                                *
 * Output: TriggerObj                                                                *
 ******************************************************************************/
TriggerObj TriggerObj::operator+(const TriggerObj& other)
{

  Particle particleTemp = Particle::operator+(other);
  TriggerObj tauTemp = particleTemp;
  
  // I don't know how to add the NumTracks for two different taus.
  // For now, add them
  
  // Return the temp particle
  return tauTemp;
} //+

/******************************************************************************
 * TriggerObj& TriggerObj::operator=(const Particle& other)                                 *
 *                                                                            *
 * Overload = Particle                                                        *
 *                                                                            *
 * Input:  Particle                                                           *
 * Output: TriggerObj                                                                *
 ******************************************************************************/
TriggerObj& TriggerObj::operator=(const Particle& other)
{
  Particle::operator=(other);
  Setid(0);
  return *this;
} // = Particle

/******************************************************************************
 * TriggerObj& TriggerObj::operator=(const TriggerObj& other)                                      *
 *                                                                            *
 * Overload = const TriggerObj                                                       *
 *                                                                            *
 * Input:  TriggerObj                                                                *
 * Output: TriggerObj                                                                *
 ******************************************************************************/
TriggerObj& TriggerObj::operator=(const TriggerObj& other)
{
  
  Particle::operator=(other);
  Setid(other.Getid());
  return *this;
} //= const tau

/******************************************************************************
 * TriggerObj& TriggerObj::operator=(const TriggerObj& other)                                      *
 *                                                                            *
 * Overload = non-const TriggerObj                                                   *
 *                                                                            *
 * Input:  TriggerObj                                                                *
 * Output: TriggerObj                                                                *
 ******************************************************************************/
TriggerObj& TriggerObj::operator=(TriggerObj& other)
{
  
  Particle::operator=(other);
  Setid(other.Getid());
  return *this;
} //= tau non-const

/******************************************************************************       
 * void TriggerObj::Fill(nanoAODTree *evtr, Int_t iE)                         *
 *                                                                            *       
 * Fill TriggerObj vector from tree                                           *
 *                                                                            *       
 * Input:  nanoAODTree                                                        *       
 * Output: kTRUE if object passes ID cuts                                     *       
 ******************************************************************************/      
Bool_t TriggerObj::Fill(nanoAODTree* evtr, int iE)
{
  Double_t trigPt    = evtr -> TrigObj_pt     [iE];
  Double_t trigEta   = evtr -> TrigObj_eta    [iE];
  Double_t trigPhi   = evtr -> TrigObj_phi    [iE];

  SetPtEtaPhiE(trigPt,trigEta,trigPhi,trigPt);
  
  //Set ID
  Setid              ( evtr -> TrigObj_id     [iE] );

}

/******************************************************************************         
 * void TriggerObj::Fill(EventTree *evtr, Int_t iE)                                  *         
 *                                                                            *         
 * Fill TriggerObj vector from tree                                                  *         
 *                                                                            *         
 * Input:  Event Tree                                                         *         
 * Output: kTRUE if object passes ID cuts                                     *         
 ******************************************************************************/
Bool_t TriggerObj::Fill(EventTree *evtr,int iE,TEnv* config)
{

}

/******************************************************************************         
 * void TriggerObj::FillFastSim(FastSimTree *evtr, Int_t iE)                         *         
 *                                                                            *         
 * Fill TriggerObj vector from tree                                                  *         
 *                                                                            *         
 * Input:  FastSim Tree                                                       *         
 * Output: kTRUE if object passes ID cuts                                     *         
 ******************************************************************************/
Bool_t TriggerObj::FillFastSim(FastSimTree *tr,int iE,TEnv* config)
{

}


