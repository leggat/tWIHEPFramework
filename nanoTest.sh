#!/bin/bash
bin/nanoTest/nanoTest.x -config config/overall/nanoTestOverall.config -inlist config/files/nanoAOD/summer20_testFile.list -hfile hists/nanoAODTest_histFile.root -skimfile skims/nanoAODTest_skimFile.root -mc -UseTotalEvtFromFile
