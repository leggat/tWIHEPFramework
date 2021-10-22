/******************************************************************************
 * GenPart.hpp                                                               *
 *                                                                            *
 * Store information about final state gparts                              *
 *                                                                            *
 * Derived from Particle class                                                *
 *                                                                            *
 * Public Member Functions of GenPart class                                  *
 *    GenPart()                         -- Default Constructor               *
 *    GenPart()                         -- Parameterized Constructor         *
 *    Clear()                            -- Set contents to default           *
 *    Fill                               -- Fill GenPart from event tree     *
 *    FillFastSim                        -- Fill GenPart from FastSim tree   *
 *    operator+=                         -- Overload +=                       *
 *    operator+                          -- Overload +                        *
 *    operator=                          -- Overload = Particle               *
 *    operator=                          -- Overload = const GenPart         *
 *    operator =                         -- Overload =                        *
 *                                                                            *
 * Private Data Members of GenPart class                                     *
 *                                                                            *
 * History                                                                    *
 *      16 June 2015 - Created by Huaqiao ZHANG                               *
 *****************************************************************************/
#include "SingleTopRootAnalysis/Particles/Truth/GenPart.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Integrate classes into the Root system
ClassImp(GenPart)


/******************************************************************************
 * GenPart::GenPart()                                                        *
 *                                                                            *
 * Default Constructor                                                        *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
GenPart::GenPart() : Particle::Particle(), _status(0.0),_pdgId(0.0),_idxmother(0.0)
{
  //cout<<"Begin of GenPart Hello"<<endl;
} //GenPart()

/******************************************************************************
 * GenPart::~GenPart()                                                       *
 *                                                                            *
 * Default Constructor                                                        *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
GenPart::~GenPart()
{

} //~GenPart


/******************************************************************************
 * GenPart::GenPart(const GenPart& other)                                   *
 *                                                                            *
 * Copy constructor GenPart const                                            *
 *                                                                            *
 * Input:  GenPart                                                           *
 * Output: None                                                               *
 ******************************************************************************/
GenPart::GenPart(const GenPart& other): Particle(other),_status(other.GetStatus()),_pdgId(other.GetpdgId()),_idxmother(other.GetIdxmother())
{
} //GenPart()

/******************************************************************************
 * GenPart::GenPart(const Particle& other)                                   *
 *                                                                            *
 * Copy constructor Particle const                                            *
 *                                                                            *
 * Input:  Particle                                                           *
 * Output: None                                                               *
 ******************************************************************************/
GenPart::GenPart(const Particle& other) : Particle(other),_status(0.0),_pdgId(0.0),_idxmother(0.0)
{ 
} //GenPart()


/******************************************************************************
 * GenPart& GenPart::operator+=(const GenPart& other)                      *
 *                                                                            *
 * Overload +=                                                                *
 *                                                                            *
 * Input:  GenPart                                                           *
 * Output: GenPart                                                           *
 ******************************************************************************/
GenPart& GenPart::operator+=(const GenPart& other)
{
  
  Particle::operator+=(other);
  return *this;
} //GenPart +=

/******************************************************************************
 * GenPart& GenPart::operator+(const GenPart& other)                       *
 *                                                                            *
 * Overload +                                                                 *
 *                                                                            *
 * Input:  GenPart                                                           *
 * Output: GenPart                                                           *
 ******************************************************************************/
GenPart GenPart::operator+(const GenPart& other)
{

  Particle particleTemp = Particle::operator+(other);
  GenPart GenPartTemp = particleTemp;
  return GenPartTemp;

} //+

/******************************************************************************
 * GenPart& GenPart::operator=(const GenPart& other)                       *
 *                                                                            *
 * Overload = Particle                                                        *
 *                                                                            *
 * Input:  GenPart                                                           *
 * Output: GenPart                                                           *
 ******************************************************************************/
GenPart& GenPart::operator=(const Particle& other)
{
  
  Particle::operator = (other);
  SetStatus(0.0);
  SetpdgId(0.0);
  SetIdxmother(0.0);

  return *this;
} // = Particle

/******************************************************************************
 * GenPart& GenPart::operator+(const GenPart& other)                       *
 *                                                                            *
 * Overload = for const GenPart                                              *
 *                                                                            *
 * Input:  GenPart                                                           *
 * Output: GenPart                                                           *
 ******************************************************************************/
GenPart& GenPart::operator=(const GenPart& other)
{
  
  Particle::operator=(other);
  SetStatus(other.GetStatus());
  SetpdgId(other.GetpdgId());
  SetIdxmother(other.GetIdxmother());
  return *this;
} //= const GenPart

/******************************************************************************
 * GenPart& GenPart::operator+(const GenPart& other)                       *
 *                                                                            *
 * Overload = for non-const GenPart                                          *
 *                                                                            *
 * Input:  GenPart                                                           *
 * Output: GenPart                                                           *
 ******************************************************************************/
GenPart& GenPart::operator=(GenPart& other)
{
  
  Particle::operator=(other);
  SetStatus(other.GetStatus());
  SetpdgId(other.GetpdgId());
  SetIdxmother(other.GetIdxmother());
  return *this;
} //= non-const GenPart


/******************************************************************************
 * void GenPart::Fill(nanoAODTree *evtr,int iE)                                *
 *                                                                            *
 * Fill GenPart vector from a nanoAODTree                                             *
 *                                                                            *
 * Input:  nanoAOD Tree                                                         *
 * Output: kTRUE if this GenPart passes GenPart ID cuts                     *
 ******************************************************************************/
Bool_t GenPart::Fill(nanoAODTree *evtr, Int_t iE, Bool_t isSimulation)
{
  //cout<<"Begin of GenPart::Fill"<<endl;
  // ******************************  Set up GenPart candidate ********************************
  Double_t gpPt        = evtr -> GenPart_pt      [iE];                         
  Double_t gpEta       = evtr -> GenPart_eta     [iE];                         
  Double_t gpPhi       = evtr -> GenPart_phi     [iE];                         
  Double_t gpM         = evtr -> GenPart_mass    [iE];                         

  SetPtEtaPhiM(gpPt, gpEta, gpPhi, gpM);
 

  //Cut based ID. Can be changed to MVA if required.
  SetStatus         ( evtr -> GenPart_status   [iE] );
  SetpdgId         ( evtr -> GenPart_pdgId   [iE] );
  SetIdxmother         ( evtr ->GenPart_genPartIdxMother   [iE] );
  
  return kTRUE;
  //cout<<"End of GenPart::Fill"<<endl;

} // Fill(nanoAODTree);
