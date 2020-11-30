/******************************************************************************
 * Bootstrap.cpp                                                                *
 *
 * Testing out some variables being added to the skim tree.
 *
 * History
 *      10th Nov 2016 - Created by D. Leggat. Just a test, so probably not that important
 ******************************************************************************/

#include "SingleTopRootAnalysis/Vars/Bootstrap.hpp"

#include <iostream>

//Test out a couple of variables, one int and one float I guess
Bootstrap::Bootstrap(){

  SetName("BootstrapVars");

  random = new TRandom();

  _floatVars["bootStrap_1"] = 400.;
  _floatVars["bootStrap_2"] = 400.;
  _floatVars["bootStrap_3"] = 400.;
  _floatVars["bootStrap_4"] = 400.;
  _floatVars["bootStrap_5"] = 400.;
  _floatVars["bootStrap_6"] = 400.;
  _floatVars["bootStrap_7"] = 400.;
  _floatVars["bootStrap_8"] = 400.;
  _floatVars["bootStrap_9"] = 400.;
  _floatVars["bootStrap_10"] = 400.;

  SetDoHists(true);

}

Bootstrap::~Bootstrap(){
  delete random;
}

void Bootstrap::FillBranches(EventContainer * evtObj){

  _floatVars["bootStrap_1"] = random->PoissonD(100);
  _floatVars["bootStrap_2"] = random->PoissonD(100);
  _floatVars["bootStrap_3"] = random->PoissonD(100);
  _floatVars["bootStrap_4"] = random->PoissonD(100);
  _floatVars["bootStrap_5"] = random->PoissonD(100);
  _floatVars["bootStrap_6"] = random->PoissonD(100);
  _floatVars["bootStrap_7"] = random->PoissonD(100);
  _floatVars["bootStrap_8"] = random->PoissonD(100);
  _floatVars["bootStrap_9"] = random->PoissonD(100);
  _floatVars["bootStrap_10"] = random->PoissonD(100);


  //  std::cout << _floatVars["bootStrap_1"] << " " << _floatVars["bootStrap_2"] << " " << _floatVars["bootStrap_3"] << " " << _floatVars["bootStrap_10"] << " " << _floatVars["bootStrap_6"] <<  std::endl;

  if (DoHists()) FillHistograms(1.);
}
