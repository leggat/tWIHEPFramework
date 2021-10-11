/******************************************************************************
 * GenPart.hpp                                                               *
 *                                                                            *
 * Store information about final state GenParts                              *
 *                                                                            *
 * Derived from Particle class                                                *
 *                                                                            *
 * Public Member Functions of GenPart class                                  *
 *    GenPart()                         -- Default Constructor               *
 *    GenPart()                         -- Parameterized Constructor         *
 *    Clear()                            -- Set contents to default           *
 *    Fill                               -- Fill GenPart from event tree     *
 *    FillFastSim                        -- Fill GenPart from FastSim tree   *
 *    SetEoverP                          -- Set E over P for GenPart         *
 *    GetEoverP                          -- GetEoverP                         *
 *    operator+=                         -- Overload +=                       *
 *    operator+                          -- Overload +                        *
 *    operator=                          -- Overload = Particle               *
 *    operator=                          -- Overload = const GenPart         *
 *    operator =                         -- Overload =                        *
 *                                                                            *
 * Private Data Members of GenPart class                                     *
 *                                                                            *
 * History                                                                    *
 *      16 June 2015 - Created by Huaqiao ZHANG for CMS                       *
 *****************************************************************************/

#ifndef GenPart_h
#define GenPart_h

//#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"
#include "SingleTopRootAnalysis/Particles/Recon/Jet.hpp"
#include "SingleTopRootAnalysis/Particles/Recon/Particle.hpp"
//#include "SingleTopRootAnalysis/Trees/EventTree.hpp"
#include "SingleTopRootAnalysis/Trees/FastSimTree.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/Top_MET.hpp"

class GenPart: public Particle
{
 public:

  // Default constructor
  GenPart();

  // Copy constructor for GenParts
  GenPart(const GenPart& other);

  // Copy constructor for Particles
  GenPart(const Particle& other);

  // Destructor
  ~GenPart();

  // Overloaded operators:
  // +=
  GenPart& operator+=(const GenPart& other);
  // +
  GenPart operator+(const GenPart& other);
  // = Particle
  GenPart& operator=(const Particle& other);
  // = GenPart const
  GenPart& operator=(const GenPart& other);
  // = GenPart non-const
  GenPart& operator=(GenPart& other);

  // Set all contents to their defaults
  inline void Clear() { Particle::Clear();
  }

  // Fill the GenPart from a nanoAODTree
  Bool_t Fill(nanoAODTree *evtr, Int_t iE, Bool_t isSimulation=false);

  //inline void SetthetaID(Double_t thetaID){_thetaID = thetaID;};
  //inline Double_t GetthetaID() const {return _thetaID;};
  //inline Double_t thetaID() const {return _thetaID;};


  inline void SetStatus(Double_t status){_status = status;};
  inline Double_t GetStatus() const {return _status;};
  inline Double_t status() const {return _status;};

  inline void SetIdxmother(Double_t idxmother){_idxmother = idxmother;};
  inline Double_t GetIdxmother() const {return _idxmother;};
  inline Double_t idxmother() const {return _idxmother;};


  inline void SetpdgId(Double_t pdgId){_pdgId = pdgId;};
  inline Double_t GetpdgId() const {return _pdgId;};
  inline Double_t pdgId() const {return _pdgId;};


 private:

  Double_t _status;
  Double_t _pdgId;
  Double_t _idxmother;

  ////////////////////////////////////////////////////////////////////////////////
  // Integrate classes into the Root system
  // Must come at end of class definition
  ClassDef(GenPart,0)
  ////////////////////////////////////////////////////////////////////////////////
};


#endif
