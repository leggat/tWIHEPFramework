#Put the JES comparison plots into a useful latex file
import sys,os,analysisComponentsNoRoot,subprocess


if __name__ == "__main__":
    
    if "--help" is sys.argv:
        print "--postfix <postfix> - add a postfix to the comparison file"
        print "--reducedSet - run on less regions and jes shifts"
        print "--singTest - produce for single test"
        print "--lessVars - only do the del r light jets var"
        sys.exit(0)
    
    postfix = ""
    if "--postfix" in sys.argv:
        ind = sys.argv.index("--postfix")
        postfix = sys.argv[ind+1]

    reducedSet = False
    if "--reducedSet" in sys.argv: reducedSet = True
       
    singTest = False
    if "--singTest" in sys.argv: singTest = True
 
    jetShifts = ["jetShift0"]
    jetShifts = []

    for i in range(27):
        jetShifts.append("jetShift{0}".format(i))

    if reducedSet:
        jetShifts = ["jetShift0","jetShift1"]
        jetShifts += ["jetShift15","jetShift6","jetShift21","jetShift25"]
    if singTest: jetShifts = ["jetShift0","jetShift24","jetShift25"]

    varsToCheck = ["M_DeltaRlightjets", 
                   "M_hadronicWmass",                  
                   "M_DeltaRBJetLepton",               
                   "M_DeltaRWlvJet2",                  
                   "M_Mass_Jet1Jet3",                  
                   "M_Pt_Lepton",                      
                   "M_E_Jet2Jet3",                     
                   "M_Pt_AllJetsLeptonMET"]            

    if "--lessVars" in sys.argv: varsToCheck = ["M_DeltaRlightjets"]

    samples = ["tW_top_nfh",
               "ttbar",
               "wPlus2Jets"]

    samples = ["tW",
               "ttbar",
               "wPlusJets",
               "singleTop",
               "VV"]

    if singTest: samples = ["singCheck"]

    pwd = os.getcwd()

    regions = ["3j1t","2j1t","4j1t"]
    if reducedSet:
        regions = ["3j1t"]

    comps = analysisComponentsNoRoot.AnalysisComponents()

    if not os.path.exists("plots/latexDir/"): os.mkdir("plots/latexDir/")
    latexFile = open("plots/latexDir/latexFileJesComp{0}.tex".format(postfix),"w")
    latexFile.write("\\documentclass{article}\n\\usepackage{graphicx}\n\\usepackage{titlesec}\n\\titleformat*{\\section}{\\small\\bfseries}\n\\titleformat*{\\subsection}{\\small\\bfseries}\n\\titleformat*{\\subsubsection}{\\small\\bfseries}\n\n\\usepackage{geometry}\n\\geometry{\na4paper,\nmargin=5pt,\nleft = 30pt\n}\n\n\\begin{document}\n\n")
    for variable in varsToCheck:
        latexFile.write("\\section{"+"-".join(variable.split("_"))+"}\n")
        for jetShift in jetShifts:
            latexFile.write("\\subsection{"+"-".join(comps.jetShiftDict[jetShift].split("_"))+"}\n")
            for sample in samples:
                latexFile.write("\\subsubsection{"+"-".join(sample.split("_"))+"}\n")
                for region in regions:
                    latexFile.write("\\includegraphics[width=0.3\\textwidth]{"+pwd+"/plots/comparePlots"+region+"/"+variable+jetShift+sample+".png}\n")
                latexFile.write("\\\\\\vspace{-25pt}\n")
        latexFile.write("\\pagebreak\n")
                                
    latexFile.write("\\end{document}")
    sys.exit(0)
    os.chdir("plots/latexDir")
    subprocess.call("pdflatex latexFileJesComp.tex",shell=True)
    os.chdir("../../")
