#Compare the JES shifts with the nominal distribution to see if there's anything really weird happening

from ROOT import *

import sys,os,analysisComponents

comp = analysisComponents.AnalysisComponents()

if "--help" in sys.argv or "-h" in sys.argv:
    print "Compare the JES uncertainties for different distributions"
    print "usage <input file/directory> <output directory> [options]"
    print "--useChannel <channel> - run on a different channel that 2j1t (2)"
    print "--useNFiles <nFiles> - only run over N files"
    print "--useAllFiles - run over all files in the directory"
    print "--useFile <file> - use specific file" 
    print "--makeDiscComps - make BDT discriminant comparisons instead"
    print "--multiSamples - run over several samples"
    print "--adaptiveFiles - add in more files if there are low stats"
    print "--rebin <n> - apply a rebinning to our plots"
    print "--lessVars - for debugging, only run one variable"
    sys.exit(0)


xAxisLabels = {
"lightJet1CSV":"CSV_{light jet 1}",
"M_DeltaRBJetLepton":"#DeltaR_{b jet,lepton}",
"M_DeltaRLeptonJet1":"#DeltaR_{lepton,jet_{1}}",
"M_DeltaRlightjets":"#DeltaR_{light jets}",
"M_hadronicWmass":"m_{W_{hadronic}}",
"M_Mass_Jet1Jet2Jet3LeptonMET":"m_{j_{1,2,3},lepton,MET}",
"M_nJet2040":"N_{Jets:20<p_{T}<40}",
"M_Pt_AllJetsLeptonMET":"p_{T}_{j_{all},lepton,MET}",
"M_Pt_Lepton":"p_{T,lepton}",
"M_topMass2_lep":"m_{top_{leptonic}}",
"M_DeltaPhiJet1Jet2":"#Delta#Phi_{j_{1,2}}",
"M_DeltaRJet1Jet2":"#DeltaR_{j_{1,2}}",
"M_DeltaRWlvJet2":"#DeltaR_{W_{lv}j_{2}}",
"M_E_AllJets":"E_{j_{all}}",
"M_E_Jet1Jet2Jet3MET":"E_{j_{1,2,3}MET}",
"M_E_Jet2Jet3":"E_{j_{2,3}}",
"M_HT":"H_{T}",
"M_Mass_AllJets":"m_{j_{all}}",
"M_Mass_AllJetsMET":"m_{j_{all}MET}",
"M_Mass_Jet1Jet2":"m_{j_{1,2}}",
"M_Mass_Jet1Jet3":"m_{j_{1,3}}",
"M_Pt_AllJets2040":"p_{T,j_{20-40}}",
"M_Pt_Jet1LeptonMET":"p_{T,j_{1}lMET}",
"M_Pt_Jet1Jet2Lepton":"p_{T,j_{1,2}l}",
"M_Pt_LeptonJet1":"p_{T,l,j_{1}}",
"M_Pt_Jet1_2030":"p_{T,j_{1,20-30}}",
}

bdtInputs = True

maxEvents = -1

adaptiveFiles = False
if "--adaptiveFiles" in sys.argv:
    adaptiveFiles = True
    useMultipleFiles = True

rebin = 50
if "--rebin" in sys.argv:
    ind = sys.argv.index("--rebin")
    rebin = int(sys.argv[ind+1])

desiredChannels = [0,2,3]
#desiredChannels = [2,3]
outDirs = ["plots/comparePlots3j1t/","plots/comparePlots3j2t/","plots/comparePlots2j1t/","plots/comparePlots4j1t/"]
regionNames = ["_3j1t","_3j2t","_2j1t","_4j1t"]
if "--useChannel" in sys.argv:
    ind = sys.argv.index("--useChannel")
    desiredChannels = [int(sys.argv[ind+1])]

#outDir = sys.argv[2]
for outDir in outDirs: 
    if not os.path.exists(outDir): os.makedirs(outDir)

useMultipleFiles = False
if "--useAllFiles" in sys.argv:
    useMultipleFiles = True

maxFile = -1
if "--useNFiles" in sys.argv:
    ind = sys.argv.index("--useNFiles")
    maxFile = int(sys.argv[ind+1])
    print "Only using {0} files".format(maxFile)
    useMultipleFiles = True

if "--makeDiscComps" in sys.argv:
    bdtInputs = False

inFileNameFromUse =  ""
useSpecificFile = False
if "--useFile" in sys.argv:
    ind = sys.argv.index("--useFile")
    inFileNameFromUse = sys.argv[ind+1]
    useSpecificFile = True
    maxFile = 1
varsToCheck = ["M_DeltaRlightjets",
"M_hadronicWmass",
"M_DeltaRBJetLepton",
"M_DeltaRWlvJet2",
"M_Mass_Jet1Jet3",
"M_nJet2040",
"M_Pt_AllJets2040",
"M_Pt_Lepton",
"M_E_Jet2Jet3",
"M_Pt_AllJetsLeptonMET"]
if "--lessVars" in sys.argv: varsToCheck = ["M_DeltaRlightjets"]


gROOT.SetBatch()

latex = TLatex()
latex.SetNDC()
latex.SetTextAlign(31)

gStyle.SetTitleYOffset(0.8)
gStyle.SetOptTitle(0)

gStyle.SetPalette(1)
gStyle.SetCanvasBorderMode(0)
gStyle.SetCanvasColor(kWhite)
gStyle.SetCanvasDefH(600)
gStyle.SetCanvasDefW(600)
gStyle.SetLabelFont(18,"")

cmsTextFont = 61
extraTextFont = 52

ratioMax = 1.05
ratioMin = 0.95

latex2 = TLatex();
latex2.SetNDC();
latex2.SetTextSize(0.04);
latex2.SetTextAlign(31);

cmsText = "CMS"
extraText = "Preliminary"

#def getSRPlots(

def makeSystHist(nominalHist,upHist,downHist,canvasName,region="_3j1t"):
#    canvasName = upHist.GetName().split("Up")[0]                      
    nominalHist.Rebin(nominalHist.GetXaxis().GetNbins()/rebin)
    upHist.Rebin(upHist.GetXaxis().GetNbins()/rebin)
    downHist.Rebin(downHist.GetXaxis().GetNbins()/rebin)
    canvy = TCanvas(canvasName+region,canvasName+region,1000,800)
    canvy.cd()
    canvy.SetBottomMargin(0.3)
    nominalHist.SetLineColor(kBlack)
    histMax = 0.
    if upHist.GetMaximum() > histMax: histMax = upHist.GetMaximum()
    if downHist.GetMaximum() > histMax: histMax = downHist.GetMaximum()
    nominalHist.SetMaximum(histMax*1.2)
    nominalHist.Draw("hist")
    upHist.SetLineColor(kRed)
    upHist.Draw("hist same")
    downHist.SetLineColor(kBlue)
    downHist.Draw("hist same")

    latex.SetTextSize(0.04)
    latex.SetTextFont(cmsTextFont)
    latex.DrawLatex(0.23, 0.95, cmsText )

    latex.SetTextFont(extraTextFont)
    latex.SetTextSize(0.04*0.76)
    latex.DrawLatex(0.35, 0.95 , extraText )

    latex2.DrawLatex(0.95, 0.95, canvasName+region);

    ratioCanvy = TPad("mva_ratio","mva_ratio",0.0,0.0,1.0,1.0)
    ratioCanvy.SetTopMargin(0.7)
    ratioCanvy.SetFillColor(0)
    ratioCanvy.SetFillStyle(0)
    ratioCanvy.SetGridy(1)
    ratioCanvy.Draw()
    ratioCanvy.cd(0)
    SetOwnership(ratioCanvy,False)

    upHistRatio = upHist.Clone()
    upHistRatio.Divide(nominalHist)
#    upHistRatio.SetMaximum(ratioMax)
#    upHistRatio.SetMinimum(ratioMin)
    upHistRatio.Draw("hist same")
    downHistRatio = downHist.Clone()
    downHistRatio.Divide(nominalHist)
    maximum = upHistRatio.GetMaximum()
    if downHistRatio.GetMaximum() > maximum: maximum = downHistRatio.GetMaximum()
    diff = maximum-1
    upHistRatio.SetMaximum(1+(diff * 1.1))
    upHistRatio.SetMinimum(1-(diff*1.1))
    #    print maximum,2-(maximum*1.1),upHistRatio.GetMaximum(),upHistRatio.GetMinimum()
    for key in xAxisLabels.keys():
        if key in canvasName:     upHistRatio.GetXaxis().SetTitle(xAxisLabels[key])
    downHistRatio.Draw("hist same")

    canvy.SaveAs(outDir+canvasName+".png")

def getMVADiscs(inFileName):
    
    print "Getting the MVA discriminants"

    inFile = TFile(inFileName,"READ")

    nominalVars = {}
    jesVars = {}    
    
    nominalVars["bdt"] = inFile.Get("MVA_ttbar_bin20_tW_top_nfh")
    nominalVars["bdt"].SetDirectory(0)
#    print nominalVars["bdt"].GetXaxis().GetNbins()
    jesVars["bdt"] = []
    for i in range(54):
        jesVars["bdt"].append(inFile.Get("MVA_ttbar_bin20_tW_top_nfh_JetShifts_"+str(i)))
        jesVars["bdt"][i].SetDirectory(0)
#        print jesVars["bdt"][i].GetXaxis().GetNbins()
    print nominalVars,jesVars
    return nominalVars,jesVars

def getHists(inFileNameList,desiredChannel=0):

    inFile = 0
    inTree = 0

    

    if useMultipleFiles:
        inTree = TChain("TNT/BOOM")
        for inFileName in inFileNameList:
            rootFiles = [f for f in os.listdir(inFileName) if ".root" in f]
            
            nFilesToUse = len(rootFiles) if (maxFile ==-1 or len(rootFiles) < maxFile) else maxFile
            print "Using {0} files".format(nFilesToUse)
            nFile = 0 
            for rootFile in rootFiles:
                print inFileName+rootFile
                inTree.Add(inFileName+rootFile)
                nFile += 1
                if maxFile > -1 and nFile > maxFile: break
                if adaptiveFiles:
                    inTree.Draw("M_channel","M_channel=={0}".format(desiredChannel))
                    if gPad.GetPrimitive("htemp"): break
#    elif useSpecificFile:
    else:
        inFile = TFile(inFileNameList,"READ")
        
        inTree = inFile.Get("TNT/BOOM")

    if inTree == 0:
        print "But how?!"
        sys.exit(1)
    nominalVars = {}
    jesVars = {}

    for var in varsToCheck:
    
        print var
        jesVars[var] = []
    
        inTree.Draw(var,"M_channel=={0}".format(desiredChannel))
    
        if not gPad.GetPrimitive("htemp"): 
            print "skipping?"
            continue

        nominalVars[var] = gPad.GetPrimitive("htemp").Clone(var)
        nominalVars[var].SetDirectory(0)
        nBins = nominalVars[var].GetXaxis().GetNbins()
        lowEdge = nominalVars[var].GetXaxis().GetBinLowEdge(1)
        upEdge = nominalVars[var].GetXaxis().GetBinUpEdge(nBins)
    #    for prim in nominalVars[var].GetListOfPrimitives():
    #        print prim.GetName(), prim.GetClassName()
        for i in range(54):
            inTree.Draw("{0}_JESShifts[{1}]>>{0}{1}({2},{3},{4})".format(var,i,nBins,lowEdge,upEdge),"M_channel_JESShifts[{0}]=={1}".format(i,desiredChannel))
#            jesVars[var].append(nominalVars[var].Clone(var+str(i)))
            jesVars[var].append(gPad.GetPrimitive("{0}{1}".format(var,i)))
#            jesVars[var][i].Reset()
            jesVars[var][i].SetDirectory(0)
 
    return nominalVars,jesVars
    print "Processing file with {0} events".format(inTree.GetEntries())
    
    maxEventsToRun = inTree.GetEntries()
    if maxEvents < maxEventsToRun and maxEvents > 0: maxEventsToRun = maxEvents

    totalEventShifts = 0

    for i in range(maxEventsToRun):
        inTree.GetEntry(i)
        runEvent = False
        if inTree.M_channel == desiredChannel: runEvent = True
        for j in range(54):
            if eval("inTree.M_channel_JESShifts[{0}]".format(j)) == desiredChannel: runEvent = True
        if not runEvent: continue
        if i%500 == 0: 
            sys.stdout.write("{0}\r".format(i))
            sys.stdout.flush()
        totalDiffs = 0
        up = 0
        down = 0
        for j in range(54):
            if inTree.M_channel == desiredChannel and not eval("inTree.M_channel_JESShifts[{0}]".format(j)) == desiredChannel: 
                up+=1
#                print "channel is there but jet shift isn't",eval("inTree.M_channel_JESShifts[{0}]".format(j))
            if not inTree.M_channel == desiredChannel and  eval("inTree.M_channel_JESShifts[{0}]".format(j)) == desiredChannel: 
                down+=1
#                print "Jet shift added when original channel wasn't",inTree.channel
            if not eval("inTree.M_channel_JESShifts[{0}]".format(j)) == desiredChannel: continue
            for var in varsToCheck:
#            if "hadronicWmass" in var:
#            if "E_Jet2Jet3" in var:
                
#                 print "\n",i, "{0:3.1f}".format(eval("inTree."+var))
                                
                
                jesVars[var][j].Fill(eval("inTree."+var+"_JESShifts[{0}]".format(j)))
#                if "hadronicWmass" in var:
#                if "E_Jet2Jet3" in var and i > 43 and i < 48:
#                    print "{1} {0:3.1f}".format(eval("inTree."+var+"_JESShifts[{0}]".format(i)),i),
        if up+down > 0:
            totalEventShifts += 1
#            print "{0} different contribution in this events, {1} out and {2} in".format(up+down,up,down)
            if up + down == 54:
                print inTree.M_channel,inTree.M_nJet3040 + inTree.M_nJet4000, inTree.M_nBJet3040 + inTree.M_nBJet4000
    print "{0} total events changed".format(totalEventShifts)
    print nominalVars,jesVars
    return nominalVars,jesVars
def printInfo(varsToCheck,nominalVars,jesVars):
    for var in varsToCheck:
        print var
        print nominalVars[var].Integral(), nominalVars[var].GetMean()
        for j in range(1,nominalVars[var].GetXaxis().GetNbins()+1):
            print nominalVars[var].GetBinContent(j)
            
        for i in range(54):
            print i,jesVars[var][i].Integral(),jesVars[var][i].GetMean()
            if not i == 5: continue
            for j in range(1,nominalVars[var].GetXaxis().GetNbins()+1):
                print "          ", jesVars[var][i].GetBinContent(j)

def makeAllSystsAndLatex(varsToCheck,outDir,nominalVars,jesVars,makeHists = False,savePost="",regionPostfix = "_3j1t",doSysts=range(27)):    
    print doSysts
    if nominalVars == 0 or jesVars == 0: makeHists = False
    #We want to compare the nominal and shifted plots here.
    latexFile = open(outDir+"latexFile{0}.tex".format(savePost),"w")
    latexFile.write("\\documentclass{beamer}\n\\usetheme{Warsaw}\n\n\\usepackage{graphicx}\n\\useoutertheme{infolines}\n\\setbeamertemplate{headline}{}\n\n\\begin{document}\n\n")
    for var in varsToCheck:
        
        for i in range(27):
            if not i in doSysts: continue
            #if i % 6 == 0: latexFile.write("\\frame{{\n\\frametitle{{{0}}}\n".format(var.split("_")[-1]))
            latexFile.write("\\frame{{\n\\frametitle{{{0}}}\n".format(var.split("_")[-1]))
            if makeHists: makeSystHist(nominalVars[var],jesVars[var][i*2],jesVars[var][i*2+1],var+"jetShift"+str(i)+savePost,regionPostfix)
            latexFile.write("\\includegraphics[width=0.3\\textwidth]{"+var+"jetShift"+str(i)+savePost+".png}\n")
#            if i % 6 == 2: latexFile.write("\\\\\n")
            latexFile.write("\\\\\n")
            #if i % 6 == 5: latexFile.write("\n}\n")
            latexFile.write("\n}\n")
#        latexFile.write("\n}\n")
    latexFile.write("\\end{document}")
    latexFile.close()

def combinePlots(nominalVars,jesVars,tempVars,tempJes,direc):
    
#    sample = comp.histoGramPerSample[direc]
    sample = direc
    if sample in nominalVars.keys():
        for var in varsToCheck:
            nominalVars[sample][var].Add(tempVars[var])
            for i in range(54):
                jesVars[sample][var][i].Add(tempJes[var][i])
    else:
        nominalVars[sample] = {}
        jesVars[sample] = {}
        for var in varsToCheck:
            nominalVars[sample][var]=tempVars[var]
            jesVars[sample][var]={}
            for i in range(54):                             
                jesVars[sample][var][i]=tempJes[var][i]

    return nominalVars,jesVars
        

if __name__ == "__main__":

    inFile = sys.argv[1]

    nominalVars,jesVars = {},{}
    
    if "--multiSamples" in sys.argv:
        for channel in desiredChannels:
            nominalVars,jesVars = {},{}
#            directories = [f for f in os.listdir(inFile)]
#            for direc in directories:
#                if direc == "wPlusJetsMCatNLO" or direc == "wPlusJetsMadgraph" or direc == "tW_mcanlo" or "qcd" in direc : continue
#                print direc
            for combinedSample in comp.histoGramOrder:                                                       
#            for combinedSample in ["tW"]:
                directories = [f for f in os.listdir(inFile) if f in comp.sample and comp.histoGramPerSample[f] == combinedSample]
                for i in range(len(directories)): directories[i] = "{0}/{1}/skims/".format(inFile,directories[i])
                print directories
                tempVars,jesTemp = getHists(directories,channel)
#                tempVars,jesTemp = getHists("{0}/{1}/skims/".format(inFile,direc),channel)
                nominalVars,jesVars = combinePlots(nominalVars,jesVars,tempVars,jesTemp,combinedSample)
            outDir = outDirs[channel]
            for sample in nominalVars.keys():
                makeAllSystsAndLatex(varsToCheck,outDir,nominalVars[sample],jesVars[sample],True,sample,regionNames[channel])
        sys.exit(0)
         

    if useSpecificFile:
        for channel in desiredChannels:
            print "Doing channel", channel
            nominalVars,jesVars = getHists(inFileNameFromUse,channel)
            outDir = outDirs[channel]
            makeAllSystsAndLatex(varsToCheck,outDir,nominalVars,jesVars,True,"singCheck",regionNames[channel],doSysts = [0,24,25])
        sys,exit(0)
#            for var in varsToCheck:

#                
#                for i in [0,24,25]:
 #                   makeSystHist(nominalVars[var],jesVars[var][i*2],jesVars[var][i*2+1],var+"jetShift"+str(i)+"test",regionNames[channel])

    if not bdtInputs: 
        varsToCheck = ["bdt"]
        nominalVars,jesVars = getMVADiscs(inFile)
    else:
        nominalVars,jesVars = getHists(inFile)

    print nominalVars,jesVars
    
    makeAllSystsAndLatex(varsToCheck,outDir,nominalVars,jesVars,True)
