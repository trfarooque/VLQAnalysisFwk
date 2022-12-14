import os
import string
import random
import re
import time, getpass
import socket
import sys
import datetime
from optparse import OptionParser
from ROOT import *
from numpy import sqrt

import glob

from VLQCouplingCalculator import VLQCouplingCalculator as couplingCalculator
from VLQCrossSectionCalculator import *

from array import array

####
# Setup using: lsetup "root 6.22.00-x86_64-centos7-gcc8-opt"
# not regular VLQAnalysisFramework setup chain
####

####
# Example command format:
# python DumpLimitPlot2D_NEWROOT.py -i <input directory (= TRexFitter Results folder)> -o <output directory> -s <signal> -m ALL -l <integrated luminosity (e.g. 139)> -d -t -n -u -c <output file suffix> -g
# See OPTIONS for further explanation of options
####

##_____________________________________________________________________________________________
##
def FitFunctionAndDefineIntersection( Theory, Med ):
    '''
    Function to determine the intersection between theory and limits by
    doing an exponential extrapolation between the different points for
    the expected/observe limit.
    '''
    diff_min = 1000
    for i in xrange(0,Theory.GetN()-1):
        x_ini_th = Double(-1)
        x_end_th = Double(-1)
        x_ini_ex = Double(-1)
        x_end_ex = Double(-1)
        
        y_ini_th = Double(-1)
        y_end_th = Double(-1)
        y_ini_ex = Double(-1)
        y_end_ex = Double(-1)
        
        Theory.GetPoint(i,x_ini_th, y_ini_th)
        Theory.GetPoint(i+1,x_end_th, y_end_th)
        
        Med.GetPoint(i,x_ini_ex, y_ini_ex)
        Med.GetPoint(i+1,x_end_ex, y_end_ex)
        
        Extra_Theory = TF1("Extra_Theory","expo",x_ini_th,x_end_th)
        Theory.Fit("Extra_Theory","RSNQ","",x_ini_th,x_end_th)
        
        Extra_Exp  = TF1("Extra_Exp","expo",x_ini_th,x_end_th)
        Med.Fit("Extra_Exp","RSQN","",x_ini_th,x_end_th)
        
        for x in range(0,int(x_end_th-x_ini_th)):
            
            xmod=x_ini_th+x
            value_th = Extra_Theory.Eval(xmod)
            value_ex = Extra_Exp.Eval(xmod)
            diff = abs(value_th-value_ex)
            if(diff<diff_min):
                diff_min = diff
                x_int = xmod
                y_int = value_ex

    return x_int
##.....................................................................................
##

def SumQuad(val1,val2):
    return sqrt(val1**2 + val2**2)

##.....................................................................................
##

gROOT.SetBatch(1)

##________________________________________________________
## OPTIONS
parser = OptionParser()
parser.add_option("-i","--inputDir",help="location of the log files ",dest="inDir",default="")
parser.add_option("-o","--outDir",help="output folder",dest="outDir",default="./test/")
parser.add_option("-s","--signal",help="signal sample",dest="signal",default="TTD")
parser.add_option("-m","--mode",help="single VLQ signal mode", dest="mode", default="WTHt")
parser.add_option("-e","--energy",help="energy",dest="energy",default="13")
parser.add_option("-a","--addText",help="additional text to plot",dest="addText",default="")
parser.add_option("-l","--lumi",help="luminosity",dest="lumi",default="3.2")
parser.add_option("-d","--data",help="consider data",dest="data",action="store_true",default=False)
parser.add_option("-x","--suffix",help="suffix of input directory of each mass point",dest="suffix",default="")
parser.add_option("-t","--theory",help="draw theory",dest="drawTheory",action="store_true",default=False)
parser.add_option("-n","--nonTheory",help="draw expected and observed limits",dest="drawNonTheory",action="store_true",default=False)
parser.add_option("-f","--forceranges",help="force ranges",dest="forceRanges",action="store_true",default=False)
parser.add_option("-c","--outsuffix",help="suffix in output file",dest="outSuffix",default="")
parser.add_option("-b","--labels",help="list of labels",dest="labels",default="")
parser.add_option("-r","--ratio",help="make ratio panel",dest="ratio",action="store_true",default=False)
parser.add_option("-g","--skiplargewidth",help="skip signals with large relative width",dest="skiplargewidth",action="store_true",default=False)
parser.add_option("-u","--xsecuncertainty",help="include theoretical cross-section uncertainty",dest="xsecuncertainty",action="store_true",default=False)
parser.add_option

(options, args) = parser.parse_args()
inDir=options.inDir
outDir=options.outDir
signal=options.signal
mode=options.mode
energy=options.energy
addText=options.addText.replace("_"," ")
lumi=options.lumi
data=options.data
suffix=options.suffix
drawTheory=options.drawTheory
drawNonTheory=options.drawNonTheory
forceRanges=options.forceRanges
outSuffix=options.outSuffix
labels=options.labels
ratio=options.ratio
skiplargewidth=options.skiplargewidth
xsecuncertainty=options.xsecuncertainty

# Build labels
labels = map(str, labels.strip('[]').split(','))

if outSuffix != "":
    outSuffix = '_'+outSuffix

os.system("mkdir -p "+outDir)

signal_label = ""
if(signal=="TTD") or (signal=="TD"):
    signal_label = "T doublet"
elif(signal=="TTS") or (signal=="TS"):
    signal_label = "T singlet"

###
# Getting the values of the masses and cross-sections
###
print signal
masses = []
gms = []
if(signal.upper()=="TS" or signal.upper()=="TD"):
    if(mode.upper()=="WTHT"):
        sigtype=["WTHt"]
        signal_label += " (T(#rightarrow Ht)qb)"
    elif(mode.upper()=="WTZT"):
        sigtype=["WTZt"]
        signal_label += " (T(#rightarrow Zt)qb)"            
    elif(mode.upper()=="ZTHT"):
        sigtype=["ZTHt"]
        signal_label += " (T(#rightarrow Ht)qt)"
    elif(mode.upper()=="ZTZT"):
        sigtype=["ZTZt"]
        signal_label += " (T(#rightarrow Zt)qt)"
    elif(mode.upper()=="ALL"):
        sigtype=["WTHt","WTZt","ZTHt","ZTZt"]
    else:
        print "<!> ERROR !! Unrecognized single VLQ mode! Valid options are WTHt, WTZt, ZTHt, ZTZt, or all."
        sys.exit(-1)

elif(signal.upper()=="WTHT" or signal.upper()=="WTZT" or signal.upper()=="ZTHT" or signal.upper()=="ZTZT"):
    sigtype="WTHt"
    xsec = 0.1
    if(signal.upper()=="WTHT"):
       sigtype="WTHt"
    if(signal.upper()=="ZTHT"):
       sigtype="ZTHt"
    if(signal.upper()=="WTZT"):
       sigtype="WTZt"
    if(signal.upper()=="ZTZT"):
       sigtype="ZTZt"


Masses = [m for m in range(1000,2400,100)]
Kappas = [0.20,0.30,0.40,0.50,0.60,0.70,0.80,0.90,1.00,1.10,1.20,1.30,1.40,1.50,1.60]
vlqMode = 'T'

# Key map: 0=W; 1=Z; 2=H
keyDictionary = {"WT": 0,"ZT": 1,"HT": 2}
contourgms = []

for M in range(1000,2310,10):
    for Kappa in [0.1*x for x in range(1,17)]:

        vlqSinglet = couplingCalculator(M, vlqMode)
        vlqDoublet = couplingCalculator(M, vlqMode)
        
        ## Set the VLQ Parameters: kappa, xi parameterization as in https://arxiv.org/pdf/1305.4172.pdf
        vlqSinglet.setKappaxi(Kappa, 0.5, 0.25) # kappa, xiW, xiZ. xiH = 1 - xiW - xiZ
        vlqDoublet.setKappaxi(Kappa, 0., 0.5) # kappa, xiW, xiZ. xiH = 1 - xiW - xiZ
        
        cSinglet = vlqSinglet.getcVals()
        cDoublet = vlqDoublet.getcVals()
        
        BRSinglet = vlqSinglet.getBRs()
        BRDoublet = vlqDoublet.getBRs()
        
        GammaSinglet = vlqSinglet.getGamma()
        GammaDoublet = vlqDoublet.getGamma()

        TotalXSecSinglet = 0
        TotalXSecDoublet = 0
        
        for sig in sigtype:

            prodIndex = keyDictionary[sig[:2:]]
            decayIndex = keyDictionary[sig[2::].upper()]
            
            XSecSinglet = XS_NWA(M, cSinglet[prodIndex], sig[:2:])*BRSinglet[decayIndex]/PNWA(proc=sig, mass=M, GM=GammaSinglet/M)
            XSecDoublet = XS_NWA(M, cDoublet[prodIndex], sig[:2:])*BRDoublet[decayIndex]/PNWA(proc=sig, mass=M, GM=GammaDoublet/M)

            XSecSinglet *= FtFactor(sig, M, GammaSinglet/M, useAverageXS = True)[0]
            XSecDoublet *= FtFactor(sig, M, GammaDoublet/M, useAverageXS = True)[0]

            TotalXSecSinglet += XSecSinglet
            TotalXSecDoublet += XSecDoublet

        if signal.upper()=="TS":
            contourgms += [{'mass_theory':M, "Kappa": Kappa, "Gamma": GammaSinglet, "xsecTheory": TotalXSecSinglet}]
        elif signal.upper()=="TD":
            contourgms += [{'mass_theory':M, "Kappa": Kappa, "Gamma": GammaDoublet, "xsecTheory": TotalXSecDoublet}]

for M in Masses:
    for Kappa in Kappas:

        if (signal.upper()=="TS" or signal.upper()=="TD"):
            if mode.upper()=="ALL":
                Mstring = str(M/100) if M%100==0 else str(M/100.)
                signalName = "sVLQ_"+signal.upper()+'M'+Mstring+'K'+str(int(100*Kappa))
                lmsuffix = '' #'low_mass'
                nmsuffix = '' #'nom_mass'
            else:
                if sigtype[0]=="WTHt":
                    signalName = sigtype[0]+str(M)+'K'+str(int(100*Kappa))
                else:
                    Mstring = str(M/100) if M%100==0 else str(M/100.)
                    signalName = sigtype[0]+'M'+Mstring+'K'+str(int(100*Kappa))
                lmsuffix = ''
                nmsuffix = ''
        else:
            if sigtype[0]=="WTHt":
                signalName = sigtype[0]+str(M)+'K'+str(int(100*Kappa))
            else:
                Mstring = str(M/100) if M%100==0 else str(M/100.)
                signalName = sigtype[0]+'M'+Mstring+'K'+str(int(100*Kappa))
                masses += [{'name': signalName, 'mass':M, 'mass_theory':M, 'xsec':xsec, 'xsecTheory': 0.1, "Kappa": Kappa}]

        if not (signal.upper()=="TS" or signal.upper()=="TD"):
            continue

        vlqSinglet = couplingCalculator(M, vlqMode)
        vlqDoublet = couplingCalculator(M, vlqMode)
        
        ## Set the VLQ Parameters: kappa, xi parameterization as in https://arxiv.org/pdf/1305.4172.pdf
        vlqSinglet.setKappaxi(Kappa, 0.5, 0.25) # kappa, xiW, xiZ. xiH = 1 - xiW - xiZ
        vlqDoublet.setKappaxi(Kappa, 0., 0.5) # kappa, xiW, xiZ. xiH = 1 - xiW - xiZ
        
        cSinglet = vlqSinglet.getcVals()
        cDoublet = vlqDoublet.getcVals()
        
        BRSinglet = vlqSinglet.getBRs()
        BRDoublet = vlqDoublet.getBRs()
        
        GammaSinglet = vlqSinglet.getGamma()
        GammaDoublet = vlqDoublet.getGamma()

        TotalXSecSinglet = 0
        TotalXSecDoublet = 0

        TotalXSecErrorSinglet = 0
        TotalXSecErrorDoublet = 0
    
        for sig in sigtype:

            prodIndex = keyDictionary[sig[:2:]]
            decayIndex = keyDictionary[sig[2::].upper()]
            
            print "sigtype : ",sig," prodIndex : ",prodIndex," decayIndex : ",decayIndex

            XSecSinglet = 1000*XS_NWA(M, cSinglet[prodIndex], sig[:2:])*BRSinglet[decayIndex]/PNWA(proc=sig, mass=M, GM=GammaSinglet/M)
            XSecDoublet = 1000*XS_NWA(M, cDoublet[prodIndex], sig[:2:])*BRDoublet[decayIndex]/PNWA(proc=sig, mass=M, GM=GammaDoublet/M)

            FtSinglet, FtErrorSinglet = FtFactor(sig, M, GammaSinglet/M, useAverageXS = True)
            FtDoublet, FtErrorDoublet = FtFactor(sig, M, GammaDoublet/M, useAverageXS = True)

            XSecSinglet *= FtSinglet
            XSecDoublet *= FtDoublet

            XSecErrorSinglet = XSecSinglet * FtErrorSinglet / FtSinglet
            XSecErrorDoublet = XSecDoublet * FtErrorDoublet / FtDoublet

            TotalXSecErrorSinglet += XSecErrorSinglet
            TotalXSecErrorDoublet += XSecErrorDoublet

            TotalXSecSinglet += XSecSinglet
            TotalXSecDoublet += XSecDoublet

            print "process : ",sig," M =",M,", kappa =",Kappa,", width/mass =",GammaSinglet/M
            print "Xsec Singlet = ", XSecSinglet, "fb"
            print "Xsec Doublet = ", XSecDoublet, "fb"

        XSecErrors = XSUncertainty(M)
        XSecErrorSingletUp = XSecErrors[0]*TotalXSecSinglet
        XSecErrorDoubletUp = XSecErrors[0]*TotalXSecDoublet
        XSecErrorSingletDown = XSecErrors[1]*TotalXSecSinglet
        XSecErrorDoubletDown = XSecErrors[1]*TotalXSecDoublet

        XSecErrorSingletUp = SumQuad(XSecErrorSingletUp, TotalXSecErrorSinglet)
        XSecErrorDoubletUp = SumQuad(XSecErrorDoubletUp, TotalXSecErrorDoublet)
        XSecErrorSingletDown = SumQuad(XSecErrorSingletDown, TotalXSecErrorSinglet)
        XSecErrorDoubletDown = SumQuad(XSecErrorDoubletDown, TotalXSecErrorDoublet)

        if skiplargewidth:
            if signal.upper()=="TS":
                if GammaSinglet/M > 0.75:
                    TotalXSecSinglet = 10000.
            elif signal.upper()=="TD":
                if GammaDoublet/M > 0.75:
                    TotalXSecDoublet = 10000.


        if signal.upper()=="TS":
            masses +=[{'name': signalName+nmsuffix, 'mass':M, 'mass_theory':M, 'xsec': 100., 'xsecTheory':TotalXSecSinglet, 'Kappa': Kappa, 'Gamma': GammaSinglet, 'errUp':XSecErrorSingletUp, 'errDown':XSecErrorSingletDown}]

        elif signal.upper()=="TD":
            masses +=[{'name': signalName+nmsuffix, 'mass':M, 'mass_theory':M, 'xsec': 100., 'xsecTheory':TotalXSecDoublet, 'Kappa': Kappa, 'Gamma': GammaDoublet, 'errUp':XSecErrorDoubletUp, 'errDown':XSecErrorDoubletDown}]

        # if mode.upper()=="ALL":
            #     masses +=[{'name': signalName+lmsuffix, 'mass':M-100., 'mass_theory':M, 'xsec': 0.1, 'xsecTheory':TotalXSecDoublet, "Kappa": Kappa}]
            # masses +=[{'name': signalName+nmsuffix, 'mass':M, 'mass_theory':M, 'xsec': 0.1, 'xsecTheory':TotalXSecDoublet, "Kappa": Kappa}]


# Palette 

number = 3
azurem9 = gROOT.GetColor(kAzure+2);
orangem4 = gROOT.GetColor(kYellow-9);
yellowm9 = gROOT.GetColor(kOrange-4);
Red = [yellowm9.GetRed(),orangem4.GetRed(),azurem9.GetRed()];
Green = [yellowm9.GetGreen(),orangem4.GetGreen(),azurem9.GetGreen()];
Blue = [yellowm9.GetBlue(),orangem4.GetBlue(),azurem9.GetBlue()];
length = [0,0.5,1]
l = array('d',[1.2,5.0])
TColor.CreateGradientColorTable(int(number),array('d',length),array('d',Red),array('d',Green),array('d',Blue),50);
gStyle.SetNumberContours(50);

###
# Effectively building the plots
###
tg_theory = TGraph2D()
tg_theory_GammaM = TGraph2D()
tg_theory_diff = TGraph2D()

tg_obs = TGraph2D()
tg_obs_plus1s, tg_obs_min1s = TGraph2D(), TGraph2D()
tg_exp = TGraph2D()
tg_exp_plus1s,tg_exp_min1s = TGraph2D(),TGraph2D()
tg_exp_plus2s,tg_exp_min2s = TGraph2D(),TGraph2D()

tg_obs_diff = TGraph2D()
tg_obs_plus1s_diff, tg_obs_min1s_diff = TGraph2D(), TGraph2D()
tg_exp_diff = TGraph2D()
tg_exp_plus1s_diff,tg_exp_min1s_diff = TGraph2D(),TGraph2D()
tg_exp_plus2s_diff,tg_exp_min2s_diff = TGraph2D(),TGraph2D()

tg_diff_list = [tg_obs_diff,tg_obs_plus1s_diff,tg_obs_min1s_diff,tg_exp_diff,tg_exp_plus1s_diff,tg_exp_min1s_diff,tg_exp_plus2s_diff,tg_exp_min2s_diff]

# THEORY PLOTS

for iMass in range(len(contourgms)):
    tg_theory.SetPoint(iMass,contourgms[iMass]['mass_theory'],contourgms[iMass]['Kappa'],contourgms[iMass]['xsecTheory'])
    tg_theory_GammaM.SetPoint(iMass,contourgms[iMass]['mass_theory'],contourgms[iMass]['Kappa'],contourgms[iMass]['Gamma']/contourgms[iMass]['mass_theory'])

"""
# HEAT MAP XS

can2 = TCanvas("Limit_XS_"+signal,"Limit_XS_"+signal,950,900)
can2.SetBottomMargin(0.13)
can2.SetLeftMargin(0.125)
can2.SetRightMargin(0.185)
can2.SetTopMargin(0.11)

gStyle.SetPalette(kGreyYellow) #kBird
can2.cd()

tg_theory.SetTitle("#splitline{#scale[0.9]{"+signal_label+" cross section}}{#scale[0.75]{#sqrt{s}=13 TeV}}")
tg_theory.GetHistogram().GetXaxis().SetLimits(1000,2100)
tg_theory.GetHistogram().GetYaxis().SetLimits(0.1,1.6)
tg_theory.GetHistogram().SetMinimum(0.00004)
tg_theory.GetHistogram().SetMaximum(1.0)
tg_theory.GetHistogram().GetXaxis().SetNdivisions(406)
tg_theory.GetHistogram().GetXaxis().SetTitle("m_{T} [GeV]")
tg_theory.GetHistogram().GetYaxis().SetTitle("#kappa")
tg_theory.GetHistogram().GetZaxis().SetTitle("#sigma [fb]")
tg_theory.GetHistogram().GetXaxis().SetLabelSize(tg_theory.GetHistogram().GetXaxis().GetLabelSize()*1.2)
tg_theory.GetHistogram().GetYaxis().SetLabelSize(tg_theory.GetHistogram().GetYaxis().GetLabelSize()*1.2)
tg_theory.GetHistogram().GetZaxis().SetLabelSize(tg_theory.GetHistogram().GetZaxis().GetLabelSize()*1.2)
tg_theory.GetHistogram().GetXaxis().SetTitleSize(tg_theory.GetHistogram().GetXaxis().GetTitleSize()*1.3)
tg_theory.GetHistogram().GetYaxis().SetTitleSize(tg_theory.GetHistogram().GetYaxis().GetTitleSize()*1.3)
tg_theory.GetHistogram().GetZaxis().SetTitleSize(tg_theory.GetHistogram().GetZaxis().GetTitleSize()*1.3)
tg_theory.GetHistogram().GetXaxis().SetTitleOffset(1.32)
tg_theory.GetHistogram().GetYaxis().SetTitleOffset(1.4)
tg_theory.GetHistogram().GetZaxis().SetTitleOffset(1.45)
tg_theory.Draw('colz')

# HIGH GAMMA/M BORDER

clone = tg_theory_GammaM.Clone("gmclon")
clone.GetHistogram().SetDirectory(0)
clone.GetHistogram().SetContour(1)
clone.GetHistogram().SetContourLevel(0,0.5)

clone.Draw('cont list same')
can2.Update()
conts = gROOT.GetListOfSpecials().FindObject("contours")
tg_border = conts.At(0).First().Clone()
tg_border.SetPoint(0, 2100, 0.595)
tg_border.SetPoint(tg_border.GetN(), 1000., 1.25)
tg_border.SetPoint(tg_border.GetN(), 1000., 1.6)
tg_border.SetPoint(tg_border.GetN(), 2100., 1.6)
tg_border.SetFillColor(18)
tg_border.Draw("f same")

borderlabel = TLatex(1140,1.15,"#Gamma_{T}/M_{T}=50%")
borderlabel.SetTextAngle(-35)
borderlabel.SetTextSize(0.028)
borderlabel.SetTextFont(42)
borderlabel.SetTextColor(kBlack)
borderlabel.Draw()

tl_siglabel = TLatex(0.2,1.15,signal_label)
tl_siglabel.SetNDC()
tl_siglabel.SetTextSize(0.042)
tl_siglabel.SetTextFont(42)
tl_siglabel.Draw()

gPad.Update()
gPad.SetLogz()
gPad.RedrawAxis()
can2.SetTickx()
can2.SetTicky()

can2.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_XS_THEORY.pdf")
can2.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_XS_THEORY.png")

exit()

# HEAT MAP GAMMA/M

can3 = TCanvas("Limit_GAMMAM_"+signal,"Limit_GAMMAM_"+signal,900,900)
can3.SetBottomMargin(0.13)
can3.SetLeftMargin(0.13)
can3.SetRightMargin(0.14)
can3.SetTopMargin(0.08)

gStyle.SetPalette(kCividis)
can3.cd()

tg_theory_GammaM.SetTitle(signal_label)
tg_theory_GammaM.GetHistogram().GetXaxis().SetLimits(1000,2000)
tg_theory_GammaM.GetHistogram().GetYaxis().SetLimits(0.1,1.6)
tg_theory_GammaM.GetHistogram().GetXaxis().SetNdivisions(406)
tg_theory_GammaM.GetHistogram().GetXaxis().SetTitle("m_{T} [GeV]")
tg_theory_GammaM.GetHistogram().GetYaxis().SetTitle("#kappa")
tg_theory_GammaM.GetHistogram().GetZaxis().SetTitle("#Gamma/M_{T}")
tg_theory_GammaM.GetHistogram().GetXaxis().SetLabelSize(tg_theory_GammaM.GetHistogram().GetXaxis().GetLabelSize()*1.2)
tg_theory_GammaM.GetHistogram().GetYaxis().SetLabelSize(tg_theory_GammaM.GetHistogram().GetYaxis().GetLabelSize()*1.2)
tg_theory_GammaM.GetHistogram().GetZaxis().SetLabelSize(tg_theory_GammaM.GetHistogram().GetZaxis().GetLabelSize()*1.2)
tg_theory_GammaM.GetHistogram().GetXaxis().SetTitleSize(tg_theory_GammaM.GetHistogram().GetXaxis().GetTitleSize()*1.3)
tg_theory_GammaM.GetHistogram().GetYaxis().SetTitleSize(tg_theory_GammaM.GetHistogram().GetYaxis().GetTitleSize()*1.3)
tg_theory_GammaM.GetHistogram().GetZaxis().SetTitleSize(tg_theory_GammaM.GetHistogram().GetZaxis().GetTitleSize()*1.3)
tg_theory_GammaM.GetHistogram().GetXaxis().SetTitleOffset(1.32)
tg_theory_GammaM.GetHistogram().GetYaxis().SetTitleOffset(1.4)
tg_theory_GammaM.GetHistogram().GetZaxis().SetTitleOffset(0.97)
tg_theory_GammaM.Draw('colz')

tl_siglabel = TLatex(0.2,1.15,signal_label)
tl_siglabel.SetNDC()
tl_siglabel.SetTextSize(0.042)
tl_siglabel.SetTextFont(42)
tl_siglabel.Draw()

gPad.Update()
# gPad.SetLogz()
gPad.RedrawAxis()
can3.SetTickx()
can3.SetTicky()

can3.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_GAMMAM_THEORY.pdf")
can3.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_GAMMAM_THEORY.png")

exit()

"""

# LIMIT PLOTS

counter=-1
goodmasses = []
for mass in masses:
    typename = mass['name']
    thismass = mass['mass']
    files = glob.glob(inDir + "/*"+mass['name']+suffix+"*/Limits/asymptotics/*.root")
    if len(files)==1:
        goodmasses.append(mass['mass'])
    else:
        files = glob.glob(inDir + "/*"+mass['name'].replace('M','').replace('K','00K')+suffix+"*/Limits/asymptotics/*.root")
        if len(files)==1:
            goodmasses.append(mass['mass'])

    if len(files)==0 or len(files)>1:
        print "<!> ERROR for mass " + `mass['mass']` + " !!"
        continue
    counter += 1
    goodmasses.append(thismass)
    rootfile = TFile(files[0],"read")
    limtree = rootfile.Get('stats')
    limtree.GetEntry(0)
    print  " Mass: ", thismass," Kappa: ", mass['Kappa'], " mu : ", limtree.obs_upperlimit, " xsec : ", mass['xsec']
    tg_obs.SetPoint(counter,thismass,mass['Kappa'],limtree.obs_upperlimit*mass['xsec'])
    tg_exp.SetPoint(counter,thismass,mass['Kappa'],limtree.exp_upperlimit*mass['xsec'])
    tg_exp_plus1s.SetPoint(counter,thismass,mass['Kappa'],limtree.exp_upperlimit_plus1*mass['xsec'])
    tg_exp_plus2s.SetPoint(counter,thismass,mass['Kappa'],limtree.exp_upperlimit_plus2*mass['xsec'])
    tg_exp_min1s.SetPoint(counter,thismass,mass['Kappa'],limtree.exp_upperlimit_minus1*mass['xsec'])
    tg_exp_min2s.SetPoint(counter,thismass,mass['Kappa'],limtree.exp_upperlimit_minus2*mass['xsec'])
    rootfile.Close()

goodmasses = sorted(list(set(goodmasses)))

for iMass in range(len(masses)):

    ycoord = masses[iMass]['Kappa']

    obs_interp = tg_obs.Interpolate(masses[iMass]['mass_theory'],ycoord)
    exp_interp = tg_exp.Interpolate(masses[iMass]['mass_theory'],ycoord)
    exp_plus1s_interp = tg_exp_plus1s.Interpolate(masses[iMass]['mass_theory'],ycoord)
    exp_plus2s_interp = tg_exp_plus2s.Interpolate(masses[iMass]['mass_theory'],ycoord)
    exp_min1s_interp = tg_exp_min1s.Interpolate(masses[iMass]['mass_theory'],ycoord)
    exp_min2s_interp = tg_exp_min2s.Interpolate(masses[iMass]['mass_theory'],ycoord)

    tg_obs_diff.SetPoint(iMass,masses[iMass]['mass_theory'],ycoord,obs_interp-masses[iMass]['xsecTheory'])
    tg_obs_plus1s_diff.SetPoint(iMass,masses[iMass]['mass_theory'],ycoord,obs_interp-(masses[iMass]['xsecTheory']+masses[iMass]['errUp']))
    tg_obs_min1s_diff.SetPoint(iMass,masses[iMass]['mass_theory'],ycoord,obs_interp-(masses[iMass]['xsecTheory']-masses[iMass]['errDown']))
    tg_exp_diff.SetPoint(iMass,masses[iMass]['mass_theory'],ycoord,exp_interp-masses[iMass]['xsecTheory'])
    tg_exp_plus1s_diff.SetPoint(iMass,masses[iMass]['mass_theory'],ycoord,exp_plus1s_interp-masses[iMass]['xsecTheory'])
    tg_exp_plus2s_diff.SetPoint(iMass,masses[iMass]['mass_theory'],ycoord,exp_plus2s_interp-masses[iMass]['xsecTheory'])
    tg_exp_min1s_diff.SetPoint(iMass,masses[iMass]['mass_theory'],ycoord,exp_min1s_interp-masses[iMass]['xsecTheory'])
    tg_exp_min2s_diff.SetPoint(iMass,masses[iMass]['mass_theory'],ycoord,exp_min2s_interp-masses[iMass]['xsecTheory'])

# PLOTTING

can = TCanvas("Limit_"+signal,"Limit_"+signal,900,900)
can.SetBottomMargin(0.15)
can.SetLeftMargin(0.15)
can.SetRightMargin(0.05)
can.SetTopMargin(0.05)

tg_obs_diff.SetLineColor(kBlack)
tg_obs_diff.SetLineStyle(1)

tg_exp_diff.SetLineColor(kBlack)
tg_exp_diff.SetLineStyle(2)

for tg_diff in tg_diff_list:
    tg_diff.SetLineWidth(3)
    tg_diff.GetHistogram().SetContour(2)
    tg_diff.GetHistogram().SetContourLevel(1,0.0)

# retrieve contours
tg_exp_plus2s_diff.Draw('contz list')
can.Update()
conts = gROOT.GetListOfSpecials().FindObject("contours")
tg_line_exp_plus2s = conts.At(0).First().Clone()

tg_exp_min2s_diff.Draw('cont list')
can.Update()
conts = gROOT.GetListOfSpecials().FindObject("contours")
tg_line_exp_min2s = conts.At(0).First().Clone()

tg_exp_plus1s_diff.Draw('cont list')
can.Update()
conts = gROOT.GetListOfSpecials().FindObject("contours")
tg_line_exp_plus1s = conts.At(0).First().Clone()

tg_exp_min1s_diff.Draw('cont list')
can.Update()
conts = gROOT.GetListOfSpecials().FindObject("contours")
tg_line_exp_min1s = conts.At(0).First().Clone()

tg_obs_diff.Draw('cont list')
can.Update()
conts = gROOT.GetListOfSpecials().FindObject("contours")
tg_line_obs = conts.At(0).First().Clone()

tg_obs_plus1s_diff.Draw('cont list')
can.Update()
conts = gROOT.GetListOfSpecials().FindObject("contours")
tg_line_obs_plus1s = conts.At(0).First().Clone()

tg_obs_min1s_diff.Draw('cont list')
can.Update()
conts = gROOT.GetListOfSpecials().FindObject("contours")
tg_line_obs_min1s = conts.At(0).First().Clone()

tg_exp_diff.Draw('cont list')
can.Update()
conts = gROOT.GetListOfSpecials().FindObject("contours")
tg_line_exp = conts.At(0).First().Clone()

tg_fill_obs_1s = TGraph()
tg_fill_exp_2s = TGraph()
tg_fill_exp_1s = TGraph()

# 2 SIGMA BAND
for i in range(tg_line_exp_plus2s.GetN()):
    tg_fill_exp_2s.SetPoint(i,tg_line_exp_plus2s.GetPointX(i),tg_line_exp_plus2s.GetPointY(i))
# corner point
if signal.upper()=="TS":
    tg_fill_exp_2s.SetPoint(tg_line_exp_plus2s.GetN(),tg_line_exp_min2s.GetPointX(tg_line_exp_min2s.GetN()-1),tg_line_exp_plus2s.GetPointY(tg_line_exp_plus2s.GetN()-1))
for i in range(tg_line_exp_min2s.GetN()):
    if signal.upper()=="TS":
        tg_fill_exp_2s.SetPoint(tg_line_exp_plus2s.GetN()+tg_line_exp_min2s.GetN()-i,tg_line_exp_min2s.GetPointX(i),tg_line_exp_min2s.GetPointY(i))
    else:
        tg_fill_exp_2s.SetPoint(tg_line_exp_plus2s.GetN()+tg_line_exp_min2s.GetN()-i-1,tg_line_exp_min2s.GetPointX(i),tg_line_exp_min2s.GetPointY(i))
# final closing
tg_fill_exp_2s.SetPoint(tg_fill_exp_2s.GetN(),tg_fill_exp_2s.GetPointX(tg_fill_exp_2s.GetN()-1),tg_fill_exp_2s.GetPointY(0))

# 1 SIGMA BAND
for i in range(tg_line_exp_plus1s.GetN()):
    tg_fill_exp_1s.SetPoint(i,tg_line_exp_plus1s.GetPointX(i),tg_line_exp_plus1s.GetPointY(i))
# corner point
if signal.upper()=="TS":
    tg_fill_exp_1s.SetPoint(tg_line_exp_plus1s.GetN(),tg_line_exp_min1s.GetPointX(tg_line_exp_min1s.GetN()-1),tg_line_exp_plus1s.GetPointY(tg_line_exp_plus1s.GetN()-1))
for i in range(tg_line_exp_min1s.GetN()):
    if signal.upper()=="TS":
        tg_fill_exp_1s.SetPoint(tg_line_exp_plus1s.GetN()+tg_line_exp_min1s.GetN()-i,tg_line_exp_min1s.GetPointX(i),tg_line_exp_min1s.GetPointY(i))
    else:
        tg_fill_exp_1s.SetPoint(tg_line_exp_plus1s.GetN()+tg_line_exp_min1s.GetN()-i-1,tg_line_exp_min1s.GetPointX(i),tg_line_exp_min1s.GetPointY(i))
# final closing
tg_fill_exp_1s.SetPoint(tg_fill_exp_1s.GetN(),tg_fill_exp_1s.GetPointX(tg_fill_exp_1s.GetN()-1),tg_fill_exp_1s.GetPointY(0))

# 1 SIGMA BAND OBSERVED
for i in range(tg_line_obs_plus1s.GetN()):
    tg_fill_obs_1s.SetPoint(i,tg_line_obs_plus1s.GetPointX(i),tg_line_obs_plus1s.GetPointY(i))
# corner point
if signal.upper()=="TS":
    tg_fill_obs_1s.SetPoint(tg_line_obs_plus1s.GetN(),tg_line_obs_min1s.GetPointX(tg_line_obs_min1s.GetN()-1),tg_line_obs_plus1s.GetPointY(tg_line_obs_plus1s.GetN()-1))
for i in range(tg_line_obs_min1s.GetN()):
    if signal.upper()=="TS":
        tg_fill_obs_1s.SetPoint(tg_line_obs_plus1s.GetN()+tg_line_obs_min1s.GetN()-i,tg_line_obs_min1s.GetPointX(i),tg_line_obs_min1s.GetPointY(i))
    else:
        tg_fill_obs_1s.SetPoint(tg_line_obs_plus1s.GetN()+tg_line_obs_min1s.GetN()-i-1,tg_line_obs_min1s.GetPointX(i),tg_line_obs_min1s.GetPointY(i))
# final closing
tg_fill_obs_1s.SetPoint(tg_fill_obs_1s.GetN(),tg_fill_obs_1s.GetPointX(tg_fill_obs_1s.GetN()-1),tg_fill_obs_1s.GetPointY(0))

# final drawing
can.Update()

tg_line_obs_plus1s.SetLineColor(kRed+1)
tg_line_obs_min1s.SetLineColor(kRed+1)
tg_line_obs_plus1s.SetLineWidth(1)
tg_line_obs_min1s.SetLineWidth(1)

tg_fill_exp_2s.SetFillColor(kYellow)
tg_fill_exp_1s.SetFillColor(kGreen)
tg_fill_exp_2s.SetLineColor(0)
tg_fill_exp_1s.SetLineColor(0)
tg_fill_obs_1s.SetFillColor(kRed+1)
tg_fill_obs_1s.SetFillStyle(3145)
tg_fill_obs_1s.SetLineColor(kRed+1)

tg_fill_exp_2s.SetTitle("")
tg_fill_exp_2s.GetXaxis().SetLimits(1000,2300)#goodmasses[0],goodmasses[-1])
tg_fill_exp_2s.GetXaxis().SetNdivisions(406)
tg_fill_exp_2s.GetXaxis().SetTitle("m_{T} [GeV]")

tg_fill_exp_2s.GetYaxis().SetTitle("#kappa")
tg_fill_exp_2s.SetMinimum(0.125)
tg_fill_exp_2s.SetMaximum(1.6)

tg_fill_exp_2s.GetXaxis().SetLabelSize(tg_fill_exp_2s.GetHistogram().GetXaxis().GetLabelSize()*1.2)
tg_fill_exp_2s.GetYaxis().SetLabelSize(tg_fill_exp_2s.GetHistogram().GetYaxis().GetLabelSize()*1.2)
tg_fill_exp_2s.GetXaxis().SetTitleSize(tg_fill_exp_2s.GetHistogram().GetXaxis().GetTitleSize()*1.3)
tg_fill_exp_2s.GetYaxis().SetTitleSize(tg_fill_exp_2s.GetHistogram().GetYaxis().GetTitleSize()*1.3)
tg_fill_exp_2s.GetXaxis().SetTitleOffset(1.32)
tg_fill_exp_2s.GetYaxis().SetTitleOffset(1.4)

tg_fill_exp_2s.Draw('af')
tg_fill_exp_1s.Draw('f same')

if xsecuncertainty:
    tg_fill_obs_1s.Draw('f same')
    tg_line_obs_plus1s.Draw('cont3 same')
    tg_line_obs_min1s.Draw('cont3 same')

tg_exp_diff.Draw('cont3 same')
tg_obs_diff.Draw('cont3 same')

# CONTOURS

if (signal.upper()=="TS" or signal.upper()=="TD") and (mode.upper()=="ALL"):
    clones = []
    contour_labels = []
    if signal.upper()=="TS":
        contour_gm = [0.05,0.1,0.2,0.5]
    elif signal.upper()=="TD":
        contour_gm = [0.1,0.2,0.5] #0.3
    
    for i,gm in enumerate(contour_gm):
        even = ((-1)**i < 0)
        clones += [tg_theory_GammaM.Clone("gmclon"+`i`)]
        clones[i].GetHistogram().SetDirectory(0)
        clones[i].GetHistogram().SetContour(1)
        clones[i].GetHistogram().SetContourLevel(0,gm)
        clones[i].SetLineWidth(2)
        clones[i].SetLineColor(kGray+2)
        clones[i].SetLineStyle(7)

        if gm==0.5:
            clones[i].Draw('cont list same')
            can.Update()
            conts = gROOT.GetListOfSpecials().FindObject("contours")
            tg_border = conts.At(0).First().Clone()
            tg_border.SetPoint(tg_border.GetN(), 1000., 1.6)
            tg_border.SetPoint(tg_border.GetN(), 2300., 1.6)
            tg_border.SetFillColor(kWhite)
            tg_border.Draw("f same")

            clones[i].Draw('samecont3')

        else:
            clones[i].Draw("samecont3")

    
        if signal.upper()=="TS":
            if gm==0.05:
                contour_labels += [TLatex(1410,0.22,"#Gamma_{T}/M_{T}="+"%.0f"%(100*gm)+"%")]
                contour_labels[i].SetTextAngle(-10)
            elif gm==0.1:
                contour_labels += [TLatex(1790,0.26,"#Gamma_{T}/M_{T}="+"%.0f"%(100*gm)+"%")]
                contour_labels[i].SetTextAngle(-8)
            elif gm==0.2:
                contour_labels += [TLatex(1050,0.69,"#Gamma_{T}/M_{T}="+"%.0f"%(100*gm)+"%")]
                contour_labels[i].SetTextAngle(-26)
            elif gm==0.5:
                contour_labels += [TLatex(1230,0.95,"#Gamma_{T}/M_{T}="+"%.0f"%(100*gm)+"%")]
                contour_labels[i].SetTextAngle(-31)
    
        elif signal.upper()=="TD":
            if gm==0.1:
                contour_labels += [TLatex(1730,0.27,"#Gamma_{T}/M_{T}="+"%.0f"%(100*gm)+"%")]
                contour_labels[i].SetTextAngle(-9)
            elif gm==0.2:
                contour_labels += [TLatex(1850,0.37,"#Gamma_{T}/M_{T}="+"%.0f"%(100*gm)+"%")]
                contour_labels[i].SetTextAngle(-9)
            # elif gm==0.3:
            #     contour_labels += [TLatex(1090,0.82,"#Gamma_{T}/M_{T}="+"%.0f"%(100*gm)+"%")]
            #     contour_labels[i].SetTextAngle(-30)
            elif gm==0.5:
                contour_labels += [TLatex(1100,1.07,"#Gamma_{T}/M_{T}="+"%.0f"%(100*gm)+"%")]
                contour_labels[i].SetTextAngle(-36)
                
        contour_labels[i].SetTextSize(0.028)
        contour_labels[i].SetTextFont(42)
        contour_labels[i].SetTextColor(kGray+2)
        contour_labels[i].Draw()

if signal.upper()=="TS":
    leg = TLegend(0.42,0.64,0.72,0.795)
else:
    leg = TLegend(0.42,0.64,0.72,0.795)
leg.SetFillColor(0)
leg.SetLineColor(0)

if xsecuncertainty:
    tg_fill_obs_1s.SetLineWidth(3)
    tg_fill_obs_1s.SetLineColor(kBlack)
    leg.AddEntry(tg_fill_obs_1s,"95% CL observed limit #pm1#sigma_{th}","lf")
else:
    leg.AddEntry(tg_obs_diff,"95% CL observed limit","l")
leg.AddEntry(tg_exp_diff,"95% CL expected limit","l")
leg.AddEntry(tg_fill_exp_1s,"95% CL expected limit #pm1#sigma","f")
leg.AddEntry(tg_fill_exp_2s,"95% CL expected limit #pm2#sigma","f")

leg.SetTextSize(0.0365)
leg.Draw()

# FORMATTING

atl_x = 0.42
atl_y = 0.88

tl_atlas = TLatex(atl_x,atl_y,"ATLAS")
tl_atlas.SetNDC()
tl_atlas.SetTextFont(72)
tl_atlas.SetTextSize(tl_atlas.GetTextSize()*0.85)
tl_atlas.Draw()
tl_int = TLatex(atl_x+0.15,atl_y,"Internal")
tl_int.SetNDC()
tl_int.SetTextFont(42)
tl_int.SetTextSize(tl_int.GetTextSize()*0.85)
tl_int.Draw()
tl_energy = TLatex(atl_x,atl_y-0.06,"#sqrt{s} = "+energy+" TeV, "+lumi+" fb^{-1}")
tl_energy.SetNDC()
tl_energy.SetTextFont(42)
tl_energy.SetTextSize(tl_energy.GetTextSize()*0.85)
tl_energy.Draw()

tl_siglabel = TLatex(0.72,0.59,signal_label)

tl_siglabel.SetNDC()
tl_siglabel.SetTextSize(0.0365)
tl_siglabel.SetTextFont(42)
tl_siglabel.Draw()

gPad.Update()
gPad.RedrawAxis()
can.SetTickx()
can.SetTicky()

can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_CONT.pdf")
can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_CONT.png")
can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_CONT.eps")

#
# OBSERVED AND EXCLUDED XS LIMITS
#

# PLOTTING

can2 = TCanvas("Limit_XS_"+signal,"Limit_XS_"+signal,1000,950)
can2.SetBottomMargin(0.13)
can2.SetLeftMargin(0.115)
can2.SetRightMargin(0.220)
can2.SetTopMargin(0.165)
can2.cd()

# OBSERVED LIMITS

tg_line_obs.SetLineColor(kBlack)
tg_line_obs.SetLineWidth(3)
tg_line_obs.SetLineStyle(1)

tg_obs.SetTitle("")
tg_obs.GetHistogram().GetXaxis().SetLimits(1000,2300)
tg_obs.GetHistogram().GetYaxis().SetLimits(0.2,1.6)
tg_obs.GetHistogram().SetMinimum(9.99)
tg_obs.GetHistogram().SetMaximum(200.01)
tg_obs.GetHistogram().GetXaxis().SetNdivisions(406)
tg_obs.GetHistogram().GetZaxis().SetMoreLogLabels()
tg_obs.GetHistogram().GetXaxis().SetTitle("m_{T} [GeV]")
tg_obs.GetHistogram().GetYaxis().SetTitle("#kappa")
tg_obs.GetHistogram().GetZaxis().SetTitle("95% CL #sigma limit [fb]")
tg_obs.GetHistogram().GetXaxis().SetLabelSize(tg_obs.GetHistogram().GetXaxis().GetLabelSize()*1.2)
tg_obs.GetHistogram().GetYaxis().SetLabelSize(tg_obs.GetHistogram().GetYaxis().GetLabelSize()*1.2)
tg_obs.GetHistogram().GetZaxis().SetLabelSize(tg_obs.GetHistogram().GetZaxis().GetLabelSize()*1.15)
tg_obs.GetHistogram().GetXaxis().SetTitleSize(tg_obs.GetHistogram().GetXaxis().GetTitleSize()*1.3)
tg_obs.GetHistogram().GetYaxis().SetTitleSize(tg_obs.GetHistogram().GetYaxis().GetTitleSize()*1.3)
tg_obs.GetHistogram().GetZaxis().SetTitleSize(tg_obs.GetHistogram().GetZaxis().GetTitleSize()*1.3)
tg_obs.GetHistogram().GetXaxis().SetTitleOffset(1.32)
tg_obs.GetHistogram().GetYaxis().SetTitleOffset(1.3)
tg_obs.GetHistogram().GetZaxis().SetTitleOffset(2.0)

tg_obs.Draw('colz')

# CONTOURS

if (signal.upper()=="TS" or signal.upper()=="TD") and (mode.upper()=="ALL"):
    clones = []
    contour_labels = []
    contour_xs = [20,30,50,80,120,180]
    for i,xs in enumerate(contour_xs):
        even = ((-1)**i < 0)
        clones += [tg_obs.Clone("obsclon"+`i`)]
        clones[i].GetHistogram().SetDirectory(0)
        clones[i].GetHistogram().SetContour(1)
        clones[i].GetHistogram().SetContourLevel(0,xs)
        clones[i].SetLineWidth(2)
        clones[i].SetLineColor(kViolet-6)
        clones[i].SetLineStyle(1)
        if (even): clones[i].SetLineStyle(7)
        clones[i].Draw("samecont3")
    
        if signal.upper()=="TS":
            if xs==20:
                contour_labels += [TLatex(1970,0.28,str(xs))]
            elif xs==30:
                contour_labels += [TLatex(1810,0.46,str(xs))]
            elif xs==50:
                contour_labels += [TLatex(1560,0.7,str(xs))]
            elif xs==80:
                contour_labels += [TLatex(1310,0.81,str(xs))]
            elif xs==120:
                contour_labels += [TLatex(1070,0.92,str(xs))]
            elif xs==180:
                continue
                # contour_labels += [TLatex(1065,0.95,str(xs))]
    
        elif signal.upper()=="TD":
            if xs==20:
                contour_labels += [TLatex(1850,0.28,str(xs))]
            elif xs==30:
                contour_labels += [TLatex(1480,0.35,str(xs))]
            elif xs==50:
                contour_labels += [TLatex(1335,0.44,str(xs))]
            elif xs==80:
                contour_labels += [TLatex(1075,0.74,str(xs))]
            elif xs==120:
                contour_labels += [TLatex(1050,0.95,str(xs))]
            elif xs==180:
                continue
                # contour_labels += [TLatex(1110,1.0,str(xs))]
        contour_labels[i].SetTextSize(0.03)
        contour_labels[i].SetTextFont(42)
        contour_labels[i].SetTextColor(kViolet-6)
        contour_labels[i].Draw()
        
if xsecuncertainty:
    tg_fill_obs_1s.Draw('f same')
    tg_line_obs_plus1s.Draw('cont3 same')
    tg_line_obs_min1s.Draw('cont3 same')

tg_line_obs.Draw('l same')

# GAMMA/M BORDER

if (signal.upper()=="TS" or signal.upper()=="TD") and (mode.upper()=="ALL"):

    clone = tg_theory_GammaM.Clone("gmclon"+`i`)
    clone.GetHistogram().SetDirectory(0)
    clone.GetHistogram().SetContour(1)
    clone.GetHistogram().SetContourLevel(0,0.5)

    clone.Draw('cont list same')
    can2.Update()
    conts = gROOT.GetListOfSpecials().FindObject("contours")
    tg_border = conts.At(0).First().Clone()
    tg_border.SetPoint(0, 2300, 0.539)
    tg_border.SetPoint(tg_border.GetN(), 1000., 1.25)
    tg_border.SetPoint(tg_border.GetN(), 1000., 1.6)
    tg_border.SetPoint(tg_border.GetN(), 2300., 1.6)
    tg_border.SetFillColor(18)
    tg_border.Draw("f same")

    borderlabel = TLatex(1140,1.15,"#Gamma_{T}/M_{T}=50%")
    borderlabel.SetTextAngle(-40)
    borderlabel.SetTextSize(0.028)
    borderlabel.SetTextFont(42)
    borderlabel.SetTextColor(kBlack)
    borderlabel.Draw()

# FORMATTING

if xsecuncertainty:
    leg = TLegend(0.45,0.865,.79,.92)
else:
    leg = TLegend(0.55,0.865,.79,.92)
leg.SetFillStyle(0)
leg.SetFillColor(0)
leg.SetLineColor(0)
if xsecuncertainty:
    tg_fill_obs_1s.SetLineWidth(3)
    tg_fill_obs_1s.SetLineColor(kBlack)
    leg.AddEntry(tg_fill_obs_1s,"95% CL observed limit #pm1#sigma_{th}","lf")
else:
    leg.AddEntry(tg_line_obs,"95% CL observed limit","l")
leg.SetTextSize(0.037)
leg.Draw()

atl_x = 0.05
atl_y = 0.94

tl_atlas = TLatex(atl_x,atl_y,"ATLAS")
tl_atlas.SetNDC()
tl_atlas.SetTextFont(72)
tl_atlas.SetTextSize(tl_atlas.GetTextSize()*0.85)
tl_atlas.Draw()
tl_int = TLatex(atl_x+0.15,atl_y,"Internal")
tl_int.SetNDC()
tl_int.SetTextFont(42)
tl_int.SetTextSize(tl_int.GetTextSize()*0.85)
tl_int.Draw()
tl_energy = TLatex(atl_x,atl_y-0.06,"#sqrt{s} = "+energy+" TeV, "+lumi+" fb^{-1}")
tl_energy.SetNDC()
tl_energy.SetTextFont(42)
tl_energy.SetTextSize(tl_energy.GetTextSize()*0.85)
tl_energy.Draw()

if signal.upper()=="TS":
    tl_siglabel = TLatex(0.795,0.93,signal_label)
else:
    tl_siglabel = TLatex(0.785,0.93,signal_label)    
tl_siglabel.SetNDC()
tl_siglabel.SetTextSize(0.037)
tl_siglabel.SetTextFont(42)
tl_siglabel.Draw()

gPad.Update()
gPad.SetLogz()
gPad.RedrawAxis()
can2.SetTickx()
can2.SetTicky()

can2.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_XSLIM_OBS.pdf")
can2.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_XSLIM_OBS.eps")
can2.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_XSLIM_OBS.png")

# EXPECTED LIMITS

tg_line_exp.SetLineColor(kBlack)
tg_line_exp.SetLineWidth(3)
tg_line_exp.SetLineStyle(2)

tg_exp.SetTitle("")
tg_exp.GetHistogram().GetXaxis().SetLimits(1000,2300)
tg_exp.GetHistogram().GetYaxis().SetLimits(0.2,1.6)
tg_exp.GetHistogram().SetMinimum(9.99)
tg_exp.GetHistogram().SetMaximum(200.01)
tg_exp.GetHistogram().GetXaxis().SetNdivisions(406)
tg_exp.GetHistogram().GetZaxis().SetMoreLogLabels()
tg_exp.GetHistogram().GetXaxis().SetTitle("m_{T} [GeV]")
tg_exp.GetHistogram().GetYaxis().SetTitle("#kappa")
tg_exp.GetHistogram().GetZaxis().SetTitle("95% CL #sigma limit [fb]")

tg_exp.GetHistogram().GetXaxis().SetLabelSize(tg_exp.GetHistogram().GetXaxis().GetLabelSize()*1.2)
tg_exp.GetHistogram().GetYaxis().SetLabelSize(tg_exp.GetHistogram().GetYaxis().GetLabelSize()*1.2)
tg_exp.GetHistogram().GetZaxis().SetLabelSize(tg_exp.GetHistogram().GetZaxis().GetLabelSize()*1.15)
tg_exp.GetHistogram().GetXaxis().SetTitleSize(tg_exp.GetHistogram().GetXaxis().GetTitleSize()*1.3)
tg_exp.GetHistogram().GetYaxis().SetTitleSize(tg_exp.GetHistogram().GetYaxis().GetTitleSize()*1.3)
tg_exp.GetHistogram().GetZaxis().SetTitleSize(tg_exp.GetHistogram().GetZaxis().GetTitleSize()*1.3)
tg_exp.GetHistogram().GetXaxis().SetTitleOffset(1.32)
tg_exp.GetHistogram().GetYaxis().SetTitleOffset(1.3)
tg_exp.GetHistogram().GetZaxis().SetTitleOffset(2.0)

tg_exp.Draw('colz')

# CONTOURS

if (signal.upper()=="TS" or signal.upper()=="TD") and (mode.upper()=="ALL"):
    clones = []
    contour_labels = []
    contour_xs = [20,30,50,80,120,180]
    for i,xs in enumerate(contour_xs):
        even = ((-1)**i < 0)
        clones += [tg_exp.Clone("expclon"+`i`)]
        clones[i].GetHistogram().SetDirectory(0)
        clones[i].GetHistogram().SetContour(1)
        clones[i].GetHistogram().SetContourLevel(0,xs)
        clones[i].SetLineWidth(2)
        clones[i].SetLineColor(kViolet-6)
        clones[i].SetLineStyle(1)
        if (even): clones[i].SetLineStyle(7)
        clones[i].Draw("samecont3")
    
        if signal.upper()=="TS":
            if xs==20:
                contour_labels += [TLatex(2150,0.32,str(xs))]
            elif xs==30:
                contour_labels += [TLatex(1940,0.53,str(xs))]
            elif xs==50:
                contour_labels += [TLatex(1560,0.66,str(xs))]
            elif xs==80:
                contour_labels += [TLatex(1330,0.82,str(xs))]
            elif xs==120:
                contour_labels += [TLatex(1180,0.9,str(xs))]
            elif xs==180:
                continue
                # contour_labels += [TLatex(1065,0.95,str(xs))]
    
        elif signal.upper()=="TD":
            if xs==20:
                contour_labels += [TLatex(1870,0.26,str(xs))]
            elif xs==30:
                contour_labels += [TLatex(1565,0.29,str(xs))]
            elif xs==50:
                contour_labels += [TLatex(1300,0.32,str(xs))]
            elif xs==80:
                contour_labels += [TLatex(1105,0.35,str(xs))]
            elif xs==120:
                continue
                # contour_labels += [TLatex(1325,0.965,str(xs))]
            elif xs==180:
                continue
                # contour_labels += [TLatex(1110,1.0,str(xs))]
        contour_labels[i].SetTextSize(0.03)
        contour_labels[i].SetTextFont(42)
        contour_labels[i].SetTextColor(kViolet-6)
        contour_labels[i].Draw()

tg_line_exp.Draw('l same')


# GAMMA/M BORDER

if (signal.upper()=="TS" or signal.upper()=="TD") and (mode.upper()=="ALL"):

    clone = tg_theory_GammaM.Clone("gmclon")
    clone.GetHistogram().SetDirectory(0)
    clone.GetHistogram().SetContour(1)
    clone.GetHistogram().SetContourLevel(0,0.5)

    clone.Draw('cont list same')
    can2.Update()
    conts = gROOT.GetListOfSpecials().FindObject("contours")
    tg_border = conts.At(0).First().Clone()
    tg_border.SetPoint(0, 2300, 0.539)
    tg_border.SetPoint(tg_border.GetN(), 1000., 1.25)
    tg_border.SetPoint(tg_border.GetN(), 1000., 1.6)
    tg_border.SetPoint(tg_border.GetN(), 2300., 1.6)
    tg_border.SetFillColor(18)
    tg_border.Draw("f same")

    borderlabel = TLatex(1140,1.15,"#Gamma_{T}/M_{T}=50%")
    borderlabel.SetTextAngle(-40)
    borderlabel.SetTextSize(0.028)
    borderlabel.SetTextFont(42)
    borderlabel.SetTextColor(kBlack)
    borderlabel.Draw()
    
leg = TLegend(0.55,0.865,.79,.92)
leg.SetFillStyle(0)
leg.SetFillColor(0)
leg.SetLineColor(0)
leg.AddEntry(tg_line_exp,"95% CL expected limit","l")
leg.SetTextSize(0.037)
leg.Draw()

tl_atlas.Draw()
tl_int.Draw()
tl_energy.Draw()

tl_siglabel = TLatex(0.795,0.93,signal_label)
tl_siglabel.SetNDC()
tl_siglabel.SetTextSize(0.037)
tl_siglabel.SetTextFont(42)
tl_siglabel.Draw()

gPad.Update()
gPad.SetLogz()
gPad.RedrawAxis()
can2.SetTickx()
can2.SetTicky()

can2.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_XSLIM_EXP.pdf")
can2.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_XSLIM_EXP.eps")
can2.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_XSLIM_EXP.png")