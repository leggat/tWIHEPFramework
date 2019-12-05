/******************************************************************************
 * testVar.cpp                                                                *
 *
 * Testing out some variables being added to the skim tree.
 *
 * History
 *      10th Nov 2016 - Created by D. Leggat. Just a test, so probably not that important
 ******************************************************************************/

#include "SingleTopRootAnalysis/Vars/ChannelFlag.hpp"

#include <iostream>

//Test out a couple of variables, one int and one float I guess
ChannelFlag::ChannelFlag(){

  _intVars["channel"] = 0;

  SetRunAdditionalVariables(true);
  
}

bool ChannelFlag::AddAdditionalVariables(EventContainer * obj){
  
  if (obj->jets.size() < 1) {
    std::cout << "This is a really obscure corner case that I really hope doesn't happen, because then I'll have to deal with this properly.." << std::endl;
    return false;
  }

  _intVecVars["channel_JESShifts"];

  for (int i = 0; i < obj->jets[0].GetNumberOfJESCorrections(); i++){
    _intVecVars["channel_JESShifts"].push_back(0);
  }

  return true;
}

void ChannelFlag::FillBranches(EventContainer * evtObj){

  //Get the number of jets and tagged jets to determine value of channel.
  int nJets = evtObj->jets.size();
  int nbJets = evtObj->taggedJets.size();

  _intVars["channel"] = -1;

  if (nJets == 2 && nbJets == 1) _intVars["channel"] = ChannelFlag::j2t1;
  else if (nJets == 3 && nbJets == 1) _intVars["channel"] = ChannelFlag::j3t1;
  else if (nJets == 4 && nbJets == 1) _intVars["channel"] = ChannelFlag::j4t1;
  else if (nJets == 3 && nbJets == 2) _intVars["channel"] = ChannelFlag::j3t2;
  else if (nJets == 4 && nbJets == 2) _intVars["channel"] = ChannelFlag::j4t2;

  if (evtObj->jets.size() <1) return;
  for (int i = 0; i < evtObj->jets[0].GetNumberOfJESCorrections(); i++){
    nJets = evtObj->jesShiftedJets[i].size();
    nbJets = 0;
    for (auto const jet : evtObj->jesShiftedJets[i]){
      if (jet.IsTagged()) nbJets++;
    }
    int tempChannel = -1;
    
    if (nJets == 2 && nbJets == 1) tempChannel = ChannelFlag::j2t1;
    else if (nJets == 3 && nbJets == 1) tempChannel = ChannelFlag::j3t1;
    else if (nJets == 4 && nbJets == 1) tempChannel = ChannelFlag::j4t1;
    else if (nJets == 3 && nbJets == 2) tempChannel = ChannelFlag::j3t2;
    else if (nJets == 4 && nbJets == 2) tempChannel = ChannelFlag::j4t2;

    _intVecVars["channel_JESShifts"][i] = tempChannel;

  }

}
