#!/bin/bash
bin/nanoTest/nanoTest.x -config config/overall/nanoTestOverall.config -inlist config/files/nanoAOD/nanoAOD_testFile.list -hfile hists/nanoAODTest_histFile.root -skimfile skims/nanoAODTest_skimFile.root -mc -BkgdTreeName DiElectronPreTagTree  -UseTotalEvtFromFile -MCatNLO -mc -SelectTrigger Muon -lepSFs  -bTagReshape -PileUpWgt 
