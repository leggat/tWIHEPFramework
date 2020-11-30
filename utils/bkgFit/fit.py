from ROOT import gSystem

gSystem.Load('libRooFit')

from ROOT import *

gROOT.SetBatch()

import sys

def getHists(inputFile):
    #do things
    print "Entering getHists"

    inFile = TFile(inputFile,"READ")

    canvy = 0

    for prim in inFile.GetListOfKeys():
        canvy = prim.ReadObj()
#        canvy =  inFile.Get(prim.GetName())
 
    dataH,qcdH,wJetsH,mcH = 0,0,0,0

    stack = 0
    for i in canvy.GetListOfPrimitives():
        if "TH1" in i.ClassName(): dataH = i
        if i.ClassName() == "THStack":
            stack = i

    print canvy,stack
    for hist in stack.GetHists():
        if "wPlusJets" in hist.GetName(): wJetsH = hist
        elif "qcd" in hist.GetName(): qcdH = hist
        elif "data" in hist.GetName(): dataH = hist
        else:
            if mcH == 0: mcH = hist
            else: mcH.Add(hist)

    return dataH,qcdH,wJetsH,mcH

def doFit(data,qcd,wJets,mc,savePostfix,fixWJets = False):
    
    print "Entering fit routine"

    w = RooWorkspace("w",kTRUE)
    if w == 0:
        print "Uh-oh, didn't work for some reason!"
        return

    #The variable we will be using/
    fitVar = RooRealVar("mTW","mTW",data.GetXaxis().GetXmin(),data.GetXaxis().GetXmax())

    #Make the pdfs we will be fitting
    mcRooHist = RooDataHist("mcRooHist","mcRooHist",RooArgList(fitVar),mc)
    wJRooHist = RooDataHist("wJRooHist","wJRooHist",RooArgList(fitVar),wJets)
    qcdRooHist = RooDataHist("qcdRooHist","qcdRooHist",RooArgList(fitVar),qcd)
    dataRooHist = RooDataHist("dataRooHist","dataRooHist",RooArgList(fitVar),data)

    #Define the pdfs here
    mc_template_pdf = RooHistPdf("mc_template_pdf","mc_template_pdf",RooArgSet(fitVar), mcRooHist)
    wJ_template_pdf = RooHistPdf("wJ_template_pdf","wJ_template_pdf",RooArgSet(fitVar), wJRooHist)
    qcd_template_pdf = RooHistPdf("qcd_template_pdf","qcd_template_pdf",RooArgSet(fitVar), qcdRooHist)

    #Define the coefficients of the model
    wJCoeff = RooRealVar("wJCoeff","wJCoeff",wJets.Integral(),0.5*wJets.Integral(),1.5*wJets.Integral())
    if fixWJets: wJCoeff = RooRealVar("wJCoeff","wJCoeff",wJets.Integral(),wJets.Integral(),wJets.Integral())
    qcdCoeff = RooRealVar("qcdCoeff","qcdCoeff",qcd.Integral(),0.,5*qcd.Integral())
    mcCoeff = RooRealVar("mcCoeff","mcCoeff",mc.Integral(),mc.Integral(),mc.Integral())

    modelCoeff = RooRealVar("modelCoeff","modelCoeff",0.5,0.,1.)

    pdf_sum = RooAddPdf("pdf_sum","pdf_sum",RooArgList(mc_template_pdf,wJ_template_pdf,qcd_template_pdf),RooArgList(mcCoeff,wJCoeff,qcdCoeff))

    pdf_sum_reduced = RooAddPdf("pdf_sum","pdf_sum",RooArgList(wJ_template_pdf,qcd_template_pdf),RooArgList(modelCoeff))
    
    pdf_sum.fitTo(dataRooHist,RooFit.Extended(True))
    fitResult = pdf_sum_reduced.fitTo(dataRooHist,RooFit.Save(True),RooFit.Strategy(0),RooFit.Hesse(),RooFit.Minos(True),RooFit.Extended(True))

    #Now print off information about the mnodel
    print "-----------------------------------------------------------------------"
    print "                              Initial info"
    print "-----------------------------------------------------------------------" 
    print "Nevents of Data        = ", data.Integral()
    print "Nevents total MC	  = ", qcd.Integral() + wJets.Integral() + mc.Integral()
    print "Nevents of QCD         = ", qcd.Integral()
    print "Nevents of WJets       = ", wJets.Integral() 
    print "Nevents of Other       = ", mc.Integral()
    print 

    print "-----------------------------------------------------------------------" 
    print "                              Fit results"
    print "-----------------------------------------------------------------------"     
    print "wJets Coeff         = ", wJCoeff.getVal(),"+-",wJCoeff.getError()
    print "QCD Coeff           = ", qcdCoeff.getVal(),"+-",qcdCoeff.getError()
    print "MC Coeff            = ", mcCoeff.getVal(),"+-",mcCoeff.getError()
    print "Total MC after fit  = ", wJCoeff.getVal() + qcdCoeff.getVal() + mcCoeff.getVal()
    print "Total Data	       = ", data.Integral()
    print "Model Coeff         = ", modelCoeff.getVal(),"+-",modelCoeff.getError()

    print "-----------------------------------------------------------------------"
    print "                             Scale factors"
    print "-----------------------------------------------------------------------"
    print "wJet SF	       = ", wJCoeff.getVal()/wJets.Integral() 
    print "QCD SF	       = ", qcdCoeff.getVal()/qcd.Integral()
    print
    print "Done!"


    canvy = TCanvas("c1","plots",400,400,800,400)
    gStyle.SetPadTopMargin(0.13)
    gStyle.SetPadRightMargin(0.13)
    gStyle.SetPadLeftMargin(0.13)

    data.Draw("ep")
    
    leg = TLegend(0.7,0.6,0.87,0.85)
    #leggy.SetFillStyle(1001)
    leg.SetBorderSize(1)
    leg.SetFillColor(0)
    leg.SetLineColor(0)
    leg.SetShadowColor(0)
    leg.SetFillColor(kWhite)
#0.6,0.75,0.8,0.85)
    
    frame = fitVar.frame()
#    fitVar.plotOn(frame)
    dataRooHist.plotOn(frame)
#    pdf_sum.plotOn(frame)


#    pdf_sum.plotOn(frame)
#    qcd_template_pdf.plotOn(frame,RooFit.FillColor(kGreen),RooFit.LineWidth(2))
#    pdf_sum.plotOn(frame,RooFit.Components("qcd_template_pdf"),RooFit.FillColor(kGreen),RooFit.LineWidth(2))
#    pdf_sum.plotOn(frame,RooFit.Components("wJ_template_pdf"),RooFit.VisualizeError(fitResult),RooFit.FillColor(kRed),RooFit.LineWidth(2))
#    pdf_sum.plotOn(frame,RooFit.Components("mc_template_pdf"),RooFit.VisualizeError(fitResult),RooFit.FillColor(kBlue),RooFit.LineWidth(2))
#    pdf_sum.plotOn(frame,RooFit.LineStyle(kDashed),RooFit.VisualizeError(fitResult),RooFit.FillColor(kBlack))
#    pdf_sum_reduced.plotOn(frame,(wJ_template_pdf),(fitResult),FillColor =kRed, LineWidth = 2 )
#    pdf_sum_reduced.plotOn(frame,LineStyle(kDashed),(fitResult),(kBlue), LineWidth(2) )


    lepton = "ele"

    leg.AddEntry("","{1} {0}".format(savePostfix,lepton),"")
    

    pdf_sum.plotOn(frame,RooFit.LineColor(kYellow))
    totalHist = TH1F("total","total",10,0,10)
    totalHist.SetLineColor(kYellow)
    totalHist.SetLineWidth(2)
    leg.AddEntry(totalHist,"Total Fit","l")

    pdf_sum.plotOn(frame,RooFit.Components("qcd_template_pdf"),RooFit.LineColor(kGreen))
    qcdHist = TH1F("qcd","qcd",10,0,10)
    qcdHist.SetLineColor(kGreen)
    qcdHist.SetLineWidth(2)
    leg.AddEntry(qcdHist,"QCD","l")
    pdf_sum.plotOn(frame,RooFit.Components("wJ_template_pdf"),RooFit.LineColor(kBlue))
    wH = TH1F("w","w",10,0,10)
    wH.SetLineColor(kBlue)
    wH.SetLineWidth(2)
    leg.AddEntry(wH,"W+jets","l")

    pdf_sum.plotOn(frame,RooFit.Components("mc_template_pdf"),RooFit.LineColor(kRed))
    mH = TH1F("w","w",10,0,10)
    mH.SetLineColor(kRed)
    mH.SetLineWidth(2)
    leg.AddEntry(mH,"Other","l")

    frame.SetTitle("")
    frame.SetName("") 
#    fitVar.plotOn(frame)
#    frame.Draw("hsame")
    frame.Draw()
    leg.Draw()
    
    canvy.SetTitle("")
    canvy.SetName("")

    canvy.SaveAs("fit{0}.png".format(savePostfix))
    canvy.SaveAs("fit{0}.pdf".format(savePostfix))

def main():
    print "Entering main"
    data, qcd, wJets, mc = getHists(sys.argv[1])

    savePostfix = ""
    if len(sys.argv) > 2 and "--" not in sys.argv[2]: savePostfix = sys.argv[2]
    fixWJets = False
    if "--fixWJets" in sys.argv: fixWJets = True

#    mc.Scale(-1.)
#    data.Add(mc)

    doFit(data,qcd,wJets,mc,savePostfix,fixWJets)


if __name__ == "__main__":
    main()
