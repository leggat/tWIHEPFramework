/*************************************
 * Do documentation later!
 */

#ifndef VarBase_h
#define VarBase_h

#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"
#include "SingleTopRootAnalysis/Base/Histograms/myTH1F.hpp"
#include <map>
#include "TROOT.h"
#include "TBranch.h"

class VarBase : public TNamed
{
public:
  VarBase(bool addHists = false); //Not sure what to put in here yet - presumably I will need the skim tree to add branches to it?

  // set name
  virtual void SetName(const char *name);
  
  //Standard set event container method
  inline void SetEventContainer(EventContainer * obj){ _eventContainerObj = obj;};

  //Add in additional variables from the event container if we need to know how many things to include based on the event
  //Example of this is add in extra channel variables based on jets with JES shifts
  virtual bool AddAdditionalVariables(EventContainer * obj){return true;};

  //Book the branches
  void BookBranches(TTree * skimTree);

  //Book histogram
  inline myTH1F* BookTH1FHistogram(const std::string& name, const std::string& title, int nBins, float min, float max){
    myTH1F* h = new myTH1F(GetDirectory(), GetEventContainer(), name, title, nBins, min, max);
_histos.Add(h);
    return h;
  }

  //Getter and setter for hitogram directory
  inline void SetDirectory(TDirectory * dirName){_histogramDir = dirName;};
  inline TDirectory * GetDirectory(){return _histogramDir;};

  //Getter and setter for event container
  inline EventContainer* GetEventContainer() {return _eventContainerObj;};

  //Reset the branches
  void ResetBranches();
  
  //Do we want to make histograms for these variables?
  void SetDoHists(bool doHists){_makeHists = doHists;};
  bool DoHists(){return _makeHists;};
  inline bool runAdditionalVariables() const {return _runAdditionalVariables;};

  //Fill the variables. There are two methods for this - the first is called from the AdditionalVarsProcessor which calls the individual VarBase classes filling routines before filling the branches themselves.
  virtual void FillBranches(EventContainer * evtObj) = 0;

  
  //Touch the variables.
  void TouchBranches();

  //Output the value of the branches
  void OutputBranches();
  
private:
  EventContainer *_eventContainerObj;

protected:
  std::map<string,int> _intVars;
  std::map<string,float> _floatVars;
  //vectors of ints and float can also be saved
  std::map<string,std::vector<int> > _intVecVars;
  std::map<string,std::vector<float> > _floatVecVars;
  //The branches that will be added to the tree
  std::map<string,TBranch*> _branchVec;

  //Fill the histograms if we're doinng that
  void FillHistograms(double weight = -999.);

  bool _makeHists;

  std::map<string,myTH1F*> _histograms;
  TDirectory* _histogramDir;

  TObjArray _histos;

  //A flag to know whether to run the additional variable adding based on event container
  bool _runAdditionalVariables;
  void SetRunAdditionalVariables(bool run){_runAdditionalVariables = run;};

  ////////////////////////////////////////////////////////////////////////////////
  // Integrate classes into the Root system                                       
  // Must come at end of class definition                                         
  ClassDef (VarBase,1);
  ////////////////////// Do Not Put Anything Below this Line /////////////////////


}; // end class Var Base

#endif
