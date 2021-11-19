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
 *    Double_t _NumTracks                -- CAL Energy in cone around TriggerObj     *
 *                                                                            *
 *****************************************************************************/

#ifndef triggerObj_h
#define triggerObj_h

#include "SingleTopRootAnalysis/Particles/Recon/Particle.hpp"
//#include "SingleTopRootAnalysis/Trees/EventTree.hpp"
#include "SingleTopRootAnalysis/Trees/FastSimTree.hpp"

class TriggerObj: public Particle
{
public:
  // Default Constructor
  TriggerObj();

  // Copy constructor  - TriggerObj
  TriggerObj(const TriggerObj& other);

  // Copy Constructor - Particle
  TriggerObj(const Particle& other);

  // Destructor
  ~TriggerObj();

  // Set all contents to their defaults
  inline void Clear() { Particle::Clear(); _id = 0; };

  // Fill the trigger object from an EventTree
  Bool_t Fill(EventTree *evtr, Int_t iE, TEnv* config);
  // Fill the trigger object from an nanoAODTree
  Bool_t Fill(nanoAODTree *evtr, Int_t iE);
  // also fill from a fastsim tree
  Bool_t FillFastSim(FastSimTree *tr, Int_t iE, TEnv* config);

  //Public getter/setters
  void Setid(const Int_t& id){_id = id;};
  Int_t Getid() const {return _id;};
  Int_t id() const {return _id;};

  // Overloaded operators:
  // +=
  TriggerObj& operator+=(const TriggerObj& other);
  // +
  TriggerObj operator+(const TriggerObj& other);
  // = const Particle
  TriggerObj& operator=(const Particle& other);
  // = const TriggerObj
  TriggerObj& operator=(const TriggerObj& other);
  // = non-const TriggerObj
  TriggerObj& operator=(TriggerObj& other);

private:
  Int_t _id;   // trigger id number 11 = Electron (PixelMatched e/gamma), 22 = Photon (PixelMatch-vetoed e/gamma), 13 = Muon, 15 = Tau, 1 = Jet, 6 = FatJet, 2 = MET, 3 = HT, 4 = MHT

  ////////////////////////////////////////////////////////////////////////////////
  // Integrate classes into the Root system
  // Must come at end of class definition
  ClassDef(TriggerObj,0)
  ////////////////////////////////////////////////////////////////////////////////    

};


#endif
