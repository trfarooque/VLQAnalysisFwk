import os
import string
import random
import re
import time, getpass
import socket
import sys
import datetime
from array import array
from optparse import OptionParser
from ROOT import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/")
from BatchTools import *

gROOT.SetBatch(1)

##________________________________________________________
##
def GetMuSignal(fit_txt):
    mu_signal, minus1, plus1 = None, None, None
    vals = []
    with open(fit_txt,'r') as f:
        for line in f:
            line = line.rstrip()
            if line.startswith('mu_signal'):
                vals = line.split(' ')
                break
    mu_signal = float(vals[2])
    plus1 = float(vals[3].replace('+',''))
    minus1 = float(vals[4].replace('-',''))
    return mu_signal, minus1, plus1

##________________________________________________________
## OPTIONS
parser = OptionParser()
parser.add_option("-i","--inputDir",help="location of the input ",dest="inDir",default="")
parser.add_option("-m","--secondDir",help="location of the second input ",dest="secDir",default="")
parser.add_option("-o","--outDir",help="output folder",dest="outDir",default="./test/")
parser.add_option("-s","--signal",help="signal sample",dest="signal",default="TTD")
parser.add_option("-e","--energy",help="energy",dest="energy",default="13")
parser.add_option("-a","--addText",help="additional text to plot",dest="addText",default="")
parser.add_option("-l","--lumi",help="luminosity",dest="lumi",default="3.2")
parser.add_option("-d","--data",help="consider data",dest="data",action="store_true",default=False)
parser.add_option("-x","--suffix",help="suffix of input directory of each mass point",dest="suffix",default="")
parser.add_option("-t","--theory",help="draw theory",dest="drawTheory",action="store_true",default=False)
parser.add_option("-f","--forceranges",help="force ranges",dest="forceRanges",action="store_true",default=False)
parser.add_option("-c","--outsuffix",help="suffix in output file",dest="outSuffix",default="")
parser.add_option

(options, args) = parser.parse_args()
inDir=options.inDir
secDir=options.secDir
outDir=options.outDir
signal=options.signal
energy=options.energy
addText=options.addText.replace("_"," ")
lumi=options.lumi
data=options.data
suffix=options.suffix
drawTheory=options.drawTheory
forceRanges=options.forceRanges
outSuffix=options.outSuffix
if outSuffix != "":
    outSuffix = '_'+outSuffix

os.system("mkdir -p "+outDir)

###
# Getting the values of the masses and cross-sections
###
print signal
masses = []
if(energy=="13"):
    if(signal.upper()=="TTD" or signal.upper()=="TTS" or signal.upper()=="TTHTHT" or signal.upper()=="TTZTZT"):
        print signal
        type = "TDoublet"
        if signal.upper()=="TTS":
            type="TSinglet"
        if signal.upper()=="TTHTHT":
            type="BR_0.00_0.00_1.00"
        if signal.upper()=="TTZTZT":
            type="ZtZt"
        masses += [{'name':"VLQ_TT_600_"+type,'mass':600,'xsec':1.16,'err':0.10}]
        masses += [{'name':"VLQ_TT_700_"+type,'mass':700,'xsec':0.455,'err':0.043}]
        masses += [{'name':"VLQ_TT_750_"+type,'mass':750,'xsec':0.295,'err':0.029}]
        masses += [{'name':"VLQ_TT_800_"+type,'mass':800,'xsec':0.195,'err':0.020}]
        masses += [{'name':"VLQ_TT_850_"+type,'mass':850,'xsec':0.132,'err':0.014}]
        masses += [{'name':"VLQ_TT_900_"+type,'mass':900,'xsec':0.0900,'err':0.0096}]
        masses += [{'name':"VLQ_TT_950_"+type,'mass':950,'xsec':0.0624,'err':0.0068}]
        masses += [{'name':"VLQ_TT_1000_"+type,'mass':1000,'xsec':0.0438,'err':0.0048}]
        masses += [{'name':"VLQ_TT_1050_"+type,'mass':1050,'xsec':0.0311,'err':0.0035}]
        masses += [{'name':"VLQ_TT_1100_"+type,'mass':1100,'xsec':0.0223,'err':0.0025}]
        masses += [{'name':"VLQ_TT_1150_"+type,'mass':1150,'xsec':0.0161,'err':0.0018}]
        masses += [{'name':"VLQ_TT_1200_"+type,'mass':1200,'xsec':0.0117,'err':0.0013}]
        masses += [{'name':"VLQ_TT_1300_"+type,'mass':1300,'xsec':0.00634,'err':0.00075}]
        masses += [{'name':"VLQ_TT_1400_"+type,'mass':1400,'xsec':0.00350,'err':0.00043}]

    elif(signal.upper().find("UEDRPP")>-1):
        masses += [{'name':"UEDRPP_1000",'mass':1000,'xsec':0.3429}]
        masses += [{'name':"UEDRPP_1200",'mass':1200,'xsec':0.07559}]
        masses += [{'name':"UEDRPP_1400",'mass':1400,'xsec':0.01804}]
        masses += [{'name':"UEDRPP_1600",'mass':1600,'xsec':0.004464}]
        masses += [{'name':"UEDRPP_1800",'mass':1800,'xsec':0.001112}]

    elif(signal.upper()=="CI4TOPS"):
        masses += [{'name':"CI4tops",'mass':1800,'xsec':0.928}]

    elif(signal.upper()=="SM4TOPS"):
        masses += [{'name':"SM4tops",'mass':1800,'xsec':0.009201}]

    elif(signal.upper()=="WTHT" or signal.upper()=="WTZT" or signal.upper()=="ZTHT" or signal.upper()=="ZTZT"):
        type="WTHt"
        if(signal.upper()=="WTHT"):
           type="WTHt"
        if(signal.upper()=="ZTHT"):
           type="ZTHt"
        if(signal.upper()=="WTZT"):
           type="WTZt"
        if(signal.upper()=="ZTZT"):
           type="ZTZt"

        if(signal.upper()=="WTHT" or signal.upper()=="WTZT"):
            masses += [{'name': "sVLQ_"+type+"11K03", 'mass':1100, 'xsec':0.1 }]
        else:
            masses += [{'name': "sVLQ_"+type+"11K05", 'mass':1100, 'xsec':0.1 }]

        masses += [{'name': "sVLQ_"+type+"16K05", 'mass':1600, 'xsec':0.1 }]

        if(not(signal.upper()=="ZTZT")):
            masses += [{'name': "sVLQ_"+type+"20K05", 'mass':2000, 'xsec':0.1 }]

###
# Effectively building the plots
###
tg_mu = TGraphAsymmErrors(len(masses))
tg_mu2 = TGraphAsymmErrors(len(masses))

#All mus
counter = -1
for mass in masses:
    counter += 1
    files = glob.glob(inDir + "/*"+mass['name']+suffix+"*/Fits/*"+mass['name']+"*.txt")

    if len(files)==0 or len(files)>1:
        print "<!> ERROR for mass " + `mass['mass']` + " !!"
    else:
        mu_signal, minus1, plus1 = GetMuSignal(files[0])
        print  " Mass: ", mass['mass'], " mu : ", mu_signal, "+"+str(plus1), minus1

        if secDir != "":
            tg_mu.SetPoint(counter,mass['mass']-4,mu_signal)
        else:
            tg_mu.SetPoint(counter,mass['mass'],mu_signal)

        tg_mu.SetPointError(counter,0.,0.,minus1,plus1)


#All mus for 2nd dir
if secDir != "":
    counter = -1
    for mass in masses:
        counter += 1
        files = glob.glob(secDir + "/*"+mass['name']+suffix+"*/Fits/*"+mass['name']+"*.txt")
    
        if len(files)==0 or len(files)>1:
            print "<!> ERROR for mass " + `mass['mass']` + " !!"
        else:
            mu_signal, minus1, plus1 = GetMuSignal(files[0])
            print  " Mass: ", mass['mass'], " mu : ", mu_signal, "+"+str(plus1), minus1
            tg_mu2.SetPoint(counter,mass['mass']+4,mu_signal)
            tg_mu2.SetPointError(counter,0.,0.,minus1,plus1)

###
# Creating signal label
###
signal_label = ""
if(signal=="TTD"):
    signal_label = "SU(2) doublet"
elif(signal=="TTS"):
    signal_label = "SU(2) singlet"
elif(signal=="TTHtHt"):
    signal_label = "BR(T#rightarrowHt) = 1"
elif(signal.upper()=="TTZTZT"):
    signal_label = "BR(T#rightarrowZt) = 1"
elif(signal=="UEDRPP11"):
    signal_label = "Tier (1,1)"
elif (signal.upper()=="WTHT"):
    signal_label = "pp #rightarrow T(#rightarrowHt)qb"
elif (signal.upper()=="ZTHT"):
    signal_label = "pp #rightarrow T(#rightarrowHt)qt"
elif (signal.upper()=="WTZT"):
    signal_label = "pp #rightarrow T(#rightarrowZt)qb"
elif (signal.upper()=="ZTZT"):
    signal_label = "pp #rightarrow T(#rightarrowZt)qt"
else:
    signal_label = signal

###
# Creating the canvas
###
can = TCanvas("1DLimit_"+signal,"1DLimit_"+signal,1000,800)
leg = TLegend(0.55,0.7,0.95,0.92)
leg.SetFillColor(0)
leg.SetLineColor(0)

###
# Making axis limits
###
ylim_min = -1
ylim_max = 3.5

###
#Limits
###

tg_mu.GetXaxis().SetLimits(1000.,2100.)
tg_mu.SetMinimum(ylim_min)
tg_mu.SetMaximum(ylim_max)
tg_mu.GetXaxis().SetNdivisions(406)
tg_mu.SetTitle("")
tg_mu.GetXaxis().SetTitle("m_{T} [GeV]")
tg_mu.GetYaxis().SetTitle("#mu")
tg_mu.GetHistogram().GetXaxis().SetLabelSize(tg_mu.GetHistogram().GetXaxis().GetLabelSize()*1.2)
tg_mu.GetHistogram().GetYaxis().SetLabelSize(tg_mu.GetHistogram().GetYaxis().GetLabelSize()*1.2)
tg_mu.GetHistogram().GetXaxis().SetTitleSize(tg_mu.GetHistogram().GetXaxis().GetTitleSize()*1.4)
tg_mu.GetHistogram().GetYaxis().SetTitleSize(tg_mu.GetHistogram().GetYaxis().GetTitleSize()*1.4)
tg_mu.GetHistogram().GetXaxis().SetTitleOffset(1.35)
tg_mu.GetHistogram().GetYaxis().SetTitleOffset(1.3)
# tg_mu.SetMarkerColor(kBlue+1)
# tg_mu.SetLineColor(kBlue+1)
tg_mu.SetMarkerColor(kBlue+1)
tg_mu.SetLineColor(kBlue+1)
tg_mu.SetLineWidth(2)
tg_mu.SetMarkerStyle(23)
tg_mu.SetMarkerSize(2)
tg_mu.Draw("a2")
tg_mu.Draw("p")

###
#Drawing some dotted lines
###

if secDir != "":
    tg_mu2.SetMarkerColor(kRed)
    tg_mu2.SetLineColor(kRed)
    tg_mu2.SetLineWidth(2)
    tg_mu2.SetMarkerStyle(22)
    tg_mu2.SetMarkerSize(2)
    tg_mu2.Draw("2")
    tg_mu2.Draw("p")


tline_0 = TLine(1000.,0.,2100.,0.)
tline_1 = TLine(1000.,1.,2100.,1.)
tline_0.SetLineStyle(4)
tline_1.SetLineStyle(2)
tline_0.Draw('same')
tline_1.Draw('same')

##
# Legend

leg.SetTextSize(0.032)
leg.Draw()

can.SetBottomMargin(0.15)
can.SetLeftMargin(0.15)
can.SetRightMargin(0.05)
can.SetTopMargin(0.05)
tl_atlas = TLatex(0.19,0.88,"ATLAS")
if signal.find("UEDRPP")>-1: tl_atlas = TLatex(0.19,0.27,"ATLAS")
tl_atlas.SetNDC()
tl_atlas.SetTextFont(72)
tl_atlas.SetTextSize(tl_atlas.GetTextSize()*0.85)
tl_atlas.Draw()
tl_int = TLatex(0.31,0.88,"Internal")
if signal.find("UEDRPP")>-1: tl_int = TLatex(0.35,0.27,"Preliminary")
tl_int.SetNDC()
tl_int.SetTextFont(42)
tl_int.SetTextSize(tl_int.GetTextSize()*0.85)
tl_int.Draw()
tl_energy = TLatex(0.19,0.82,"#sqrt{s} = "+energy+" TeV, "+lumi+" fb^{-1}")
if signal.find("UEDRPP")>-1: tl_energy = TLatex(0.19,0.2,"#sqrt{s} = "+energy+" TeV, "+lumi+" fb^{-1}")
tl_energy.SetNDC()
tl_energy.SetTextFont(42)
tl_energy.SetTextSize(tl_energy.GetTextSize()*0.85)
tl_energy.Draw()
if(addText!=""):
    tl_addtext = TLatex(0.19,0.18,addText)
    tl_addtext.SetNDC()
    tl_addtext.SetTextFont(42)
    tl_addtext.SetTextSize(tl_addtext.GetTextSize()*0.85)
    tl_addtext.Draw()

if signal_label!="":

    if secDir != "":
        leg.AddEntry(TObject(), "Fit to S+B Asimov dataset", "")
    
        tg_dummy = TGraph()
        tg_dummy.SetMarkerSize(2)
        tg_dummy.SetMarkerStyle(33)
        tg_dummy.SetMarkerColor(kGray+2)
        tg_dummy.SetLineColor(kGray+2)
        tg_dummy.SetLineWidth(2)
        leg.AddEntry(tg_dummy,"Best fit #mu","ep")

        mu1 = leg.AddEntry(TObject(), signal_label+" (#mu=1)", "")
        mu1.SetTextColor(kRed)
        mu0 = leg.AddEntry(TObject(), signal_label+" (#mu=0)", "")
        mu0.SetTextColor(kBlue+1)

    else:
        # leg.AddEntry(TObject(), "Fit to blinded data", "")
        # leg.AddEntry(tg_mu,"Best fit #mu","ep")
        # leg.AddEntry(TObject(), signal_label+" (#mu=0)", "")

        leg.AddEntry(TObject(), "Fit to S+B Asimov dataset", "")
        leg.AddEntry(tg_mu,"Best fit #mu","ep")
        leg.AddEntry(TObject(), signal_label+" (#mu=1)", "")

gPad.RedrawAxis()
can.SetTickx()
can.SetTicky()

can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+".eps")
can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+".png")

# can.SetLogy()
# 
# if drawTheory:
    # tg_theory.GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
    # tg_theory.SetMinimum(ylim_min_log)
    # tg_theory.SetMaximum(ylim_max_log)
    # tg_theory.Draw("al3")
    # tg_exp2s.Draw("af")
    # tg_exp1s.Draw("f")
    # tg_exp.Draw("l")
    # leg.Draw()
    # tl_atlas.Draw()
    # tl_int.Draw()
    # tl_energy.Draw()
    # if(addText!=""):
        # tl_addtext.Draw()
    # tg_theory.GetXaxis().Draw()
    # tg_theory.GetYaxis().Draw()
# else:
    # tg_exp2s.GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
    # tg_exp2s.SetMinimum(ylim_min_log)
    # tg_exp2s.SetMaximum(ylim_max_log)
    # tg_exp2s.Draw("af")
    # tg_exp1s.Draw("f")
    # tg_exp.Draw("l")
    # leg.Draw()
    # tl_atlas.Draw()
    # tl_int.Draw()
    # tl_energy.Draw()
    # if(addText!=""):
        # tl_addtext.Draw()
    # tg_exp2s.GetXaxis().Draw()
    # tg_exp2s.GetYaxis().Draw()
# 
# gPad.RedrawAxis()
# can.SetTickx()
# can.SetTicky()
# 
# can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_log.eps")
# can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_log.png")

