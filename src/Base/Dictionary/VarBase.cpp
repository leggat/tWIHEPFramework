/******************************************************************************
 * VarBase.cpp
 *
 *
 * Abstract class
 *
 * The definition of a class that will add variables to the skim tree
 * Works in a similar manner to BaseCut, i.e. doesn't really do anything
 * itself, but allows variables to be booked and saved later on.
 * 
 * Something about public member functions:
 *
 *
 * History
 *      10 Nov 2016 - Created by D Leggat
 *****************************************************************************/

#include "SingleTopRootAnalysis/Base/Dictionary/VarBase.hpp"

#include "TROOT.h"

// Integrate classes into the Root system
ClassImp(VarBase);


/******************************************************************************  
 * void VarBase::VarBase ()		                                      *  
 *                                                                            *  
 * Default constructor                                                        *  
 *                                                                            *  
 * Input:  None                                                               *  
 * Output: None                                                               *  
 ******************************************************************************/ 
VarBase::VarBase(bool addHists){
  //Set up whether we want to make histograms of the BDT variables
  _makeHists = addHists;
  _runAdditionalVariables = false;
//Nothing immediately happens in here.
}



/****************************************************************************** 
 * void VarBase::SetName (const char *name)                                   * 
 *                                                                            * 
 * Set Name of Object                                                         * 
 *                                                                            * 
 * Input:  name to set                                                        * 
 * Output: None                                                               * 
 ******************************************************************************/
void VarBase::SetName(const char *name){
  //Set name
  TNamed::SetName(name);
  
} // end SetName

/****************************************************************************** 
 * void VarBase::BookBranches (TTree * skimTree)                              * 
 *                                                                            * 
 * Book the branches into skim tree                                           * 
 *                                                                            * 
 * Input:  The skimTree is the TTree to associate the variables with          *
 * Output: None                                                               * 
 ******************************************************************************/
void VarBase::BookBranches(TTree * skimTree){

  for (auto intVar : _intVars){
    string tempString = intVar.first;
    if (skimTree != NULL) _branchVec[tempString.c_str()] = skimTree->Branch(tempString.c_str(),&(_intVars[tempString.c_str()]),(tempString+"/I").c_str());
    if (DoHists()) {
      //      std::cout << intVar.first << std::endl;
      _histograms[tempString] = BookTH1FHistogram(tempString,tempString,10,0.,intVar.second);
      //      std::cout << tempString << " " << _histograms[tempString]->GetName() << " " << GetEventContainer() << std::endl;
    }
  }

  for (auto floatVar: _floatVars){
    string tempString = floatVar.first;
    if (skimTree!= NULL) _branchVec[tempString.c_str()] = skimTree->Branch(tempString.c_str(),&(_floatVars[tempString.c_str()]),(tempString+"/F").c_str());
    if (DoHists()) {
      float startVar = 0.;
      if (floatVar.second < 0.) startVar = floatVar.second;
      _histograms[tempString] = BookTH1FHistogram(tempString,tempString,100,startVar,std::fabs(floatVar.second));
      //      std::cout << tempString << " " << _histograms[tempString]->GetName() << std::endl;
    }
  }

  for (auto intVecVar : _intVecVars){
    string tempString = intVecVar.first;
    if (skimTree != NULL) _branchVec[tempString.c_str()] = skimTree->Branch(tempString.c_str(),&(_intVecVars[tempString.c_str()]));
  }

  for (auto floatVecVar: _floatVecVars){
    string tempString = floatVecVar.first;
    if (skimTree!= NULL) _branchVec[tempString.c_str()] = skimTree->Branch(tempString.c_str(),&(_floatVecVars[tempString.c_str()]));
  }


}

/****************************************************************************** 
 * void VarBase::ResetBranches ()                                             * 
 *                                                                            * 
 * Reset the branches in skim tree                                            * 
 *                                                                            * 
 * Input:  name to set                                                        * 
 * Output: None                                                               * 
 ******************************************************************************/
void VarBase::ResetBranches(){
  for (auto intVar : _intVars){
    _intVars[intVar.first] = -999;
  }
  for (auto floatVar : _floatVars){
    _floatVars[floatVar.first] = -999.;
  }
  for (auto intVecVar : _intVecVars){
    for (int i = 0; i < _intVecVars[intVecVar.first].size(); i++){
      _intVecVars[intVecVar.first][i] = -999;
    }
  }
  for (auto floatVecVar : _floatVecVars){
    for (int i = 0; i < _floatVecVars[floatVecVar.first].size(); i++){
      _floatVecVars[floatVecVar.first][i] = -999;
    }
  }
}

/****************************************************************************** 
 * void VarBase::OutputBranches ()                                             * 
 *                                                                            * 
 * Output the value of the variables
 *                                                                            * 
 * Input:  None                                                               * 
 * Output: None                                                               * 
 ******************************************************************************/
void VarBase::OutputBranches(){
  for (auto floatVar : _floatVars){
    std::cout << floatVar.first << " " << floatVar.second << std::endl;
    if (DoHists()) std::cout << _histograms[floatVar.first] << std::endl;
  }

  for (auto intVar : _intVars){
    std::cout << intVar.first << " " << intVar.second << std::endl;
    if (DoHists()) std::cout << _histograms[intVar.first] << std::endl;
  }

  return;

  for (auto floatVec : _floatVecVars){
    std::cout << floatVec.first << std::endl;
    for (int i = 0; i < floatVec.second.size() ; i++) {
      std::cout << "Index: " << i << " Value: " << floatVec.second[i] << std::endl;
    }
  }

  for (auto intVec : _intVecVars){
    std::cout << intVec.first << std::endl;
    for (int i = 0; i < intVec.second.size() ; i++) {
      std::cout << "Index: " << i << " Value: " << intVec.second[i] << std::endl;
    }
  }

}

/****************************************************************************** 
 * void VarBase::TouchBranches ()                                             * 
 *                                                                            * 
 * Touch every filled variable because that helped for some reason
 *                                                                            * 
 * Input:  None                                                               * 
 * Output: None                                                               * 
 ******************************************************************************/
void VarBase::TouchBranches(){
  for (auto floatVar : _floatVars){
    float temp = floatVar.second;
  }
  for (auto intVar : _intVars){
    int temp = intVar.second;
  }
}


/****************************************************************************** 
 * void VarBase::FillHistograms ()                                             * 
 *                                                                            * 
 * Fill the histograms we're making here *
 *                                                                            * 
 * Input:  None                                                               * 
 * Output: None                                                               * 
 ******************************************************************************/
void VarBase::FillHistograms(double weight){
  double fillWeight = weight;
  if (weight == -999.) weight = 1.;
  for (auto intVar : _intVars){
    //    std::cout << intVar.first << " " << intVar.second << std::endl;
    _histograms[intVar.first]->Fill(intVar.second,weight);
  }
  
  for (auto floatVar : _floatVars){
    //std::cout << floatVar.first << " " << floatVar.second << std::endl;
    _histograms[floatVar.first]->Fill(floatVar.second,weight);
  }
}
