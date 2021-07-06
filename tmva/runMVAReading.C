#include "/publicfs/cms/user/duncanleg/tW13TeV/framework/tmva/mvaTool.C"

void runMVAReading(TString sampleName, TString inDir, TString outDir, int minFile = -1, int maxFile = -1, bool isData = false, float emptyVal = -999. ){

  //  gROOT->LoadMacro("/publicfs/cms/user/duncanleg/tW13TeV/tmva/mvaTool.C");
  
  std::cout << sampleName;
  if (! (minFile <0 && maxFile < 0)) std::cout << " " << minFile << "-" << maxFile;

  bool isEle = kFALSE;
  if (inDir.Contains("Ele"))
    isEle = kTRUE;
  mvaTool t = mvaTool(false,isEle);
  
  t.setEmptyVarValue(emptyVal);

  t.doReading(sampleName,inDir,outDir,isData,minFile,maxFile);
  //t.doReading("tW_top_nfh","tW/","output/");

}
