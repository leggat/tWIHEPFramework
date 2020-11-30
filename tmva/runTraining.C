#include "/publicfs/cms/user/duncanleg/tW13TeV/framework/tmva/mvaTool.C"

void runTraining(TString inDir, int nTrees=400, int channel = 0)
{
  bool isEle = kFALSE;
  if (inDir.Contains("Ele"))
    isEle = kTRUE;
  mvaTool t = mvaTool(channel,false,isEle);

  t.setNTrees(nTrees);
  
  t.doTraining(inDir);

}
