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
## OPTIONS
parser = OptionParser()
parser.add_option("-i","--inputDir",help="location of the log files ",dest="inDir",default="/nfs/at3/scratch2/mcasolino/TRexFitter/TtHFitter/")
parser.add_option("-o","--outDir",help="output folder",dest="outDir",default="./test/")
parser.add_option("-s","--signal",help="signal sample",dest="signal",default="TTD")
parser.add_option("-e","--energy",help="energy",dest="energy",default="13")
parser.add_option("-a","--addText",help="additional text to plot",dest="addText",default="")
parser.add_option("-l","--lumi",help="luminosity",dest="lumi",default="13.2")
parser.add_option("-d","--data",help="consider data",dest="data",action="store_true",default=False)
parser.add_option

(options, args) = parser.parse_args()

inDir=options.inDir
outDir=options.outDir
signal=options.signal
energy=options.energy
addText=options.addText.replace("_"," ")
lumi=options.lumi
data=options.data

os.system("mkdir -p "+outDir)

###
# Getting the values of the masses and cross-sections
###
masses = []
if(energy=="13"):
    if(signal.upper()=="TTD" or signal.upper()=="TTS"  or signal.upper()=="TTZTZT"  or signal.upper()=="TTHTHT"):
        type = "TDoublet"
        if signal.upper()=="TTS":
            type="TSinglet"
        if signal.upper()=="TTHTHT":
            type="BR_0.00_0.00_1.00"
        if signal.upper()=="TTZTZT":
            type="ZtZt"
        masses += [{'name':"VLQ_TT_600_"+type,'mass':600,'xsec':1.16}]
        masses += [{'name':"VLQ_TT_700_"+type,'mass':700,'xsec':0.455}]
        masses += [{'name':"VLQ_TT_750_"+type,'mass':750,'xsec':0.295}]
        masses += [{'name':"VLQ_TT_800_"+type,'mass':800,'xsec':0.195}]
        masses += [{'name':"VLQ_TT_850_"+type,'mass':850,'xsec':0.132}]
        masses += [{'name':"VLQ_TT_900_"+type,'mass':900,'xsec':0.0900}]
        masses += [{'name':"VLQ_TT_950_"+type,'mass':950,'xsec':0.0624}]
        masses += [{'name':"VLQ_TT_1000_"+type,'mass':1000,'xsec':0.0438}]
        masses += [{'name':"VLQ_TT_1050_"+type,'mass':1050,'xsec':0.0311}]
        masses += [{'name':"VLQ_TT_1100_"+type,'mass':1100,'xsec':0.0223}]
        masses += [{'name':"VLQ_TT_1150_"+type,'mass':1150,'xsec':0.0161}]
        masses += [{'name':"VLQ_TT_1200_"+type,'mass':1200,'xsec':0.0117}]
        masses += [{'name':"VLQ_TT_1300_"+type,'mass':1300,'xsec':0.00634}]
        masses += [{'name':"VLQ_TT_1400_"+type,'mass':1400,'xsec':0.00350}]
    if(signal.upper()=="UEDRPP"):
        masses += [{'name':"UEDRPP_1000",'mass':1000,'xsec':0.3429}]
        masses += [{'name':"UEDRPP_1200",'mass':1200,'xsec':0.07559}]
        masses += [{'name':"UEDRPP_1400",'mass':1400,'xsec':0.01804}]
        masses += [{'name':"UEDRPP_1600",'mass':1600,'xsec':0.004464}]
        masses += [{'name':"UEDRPP_1800",'mass':1800,'xsec':0.001112}]

###
# Effectively declaring the plots
###
tg_obs = TGraph(len(masses))
tg_exp = TGraph(len(masses))
tg_1s = TGraph(4)
tg_2s = TGraph(4)
tg_0s = TGraph(2)
#tg_1s.SetLineColor(kBlue)
#tg_2s.SetLineColor(kRed)
tg_1s.SetLineWidth(2)
tg_2s.SetLineWidth(2)
tg_1s.SetFillColor(kYellow)
tg_2s.SetFillColor(kRed)
tg_1s.SetFillStyle(3002)
tg_2s.SetFillStyle(3002)



###
# All significances
###
counter = -1
for mass in masses:
    counter += 1
    files = glob.glob(inDir + "/*"+mass['name']+"*/Significance/*.root")
    if len(files)!=1:
        print "<!> ERROR !!"
    else:
        rootfile = TFile(files[0],"read")
        histogram = rootfile.Get("hypo")
        tg_obs.SetPoint(counter,mass['mass'],histogram.GetBinContent(1))
        tg_exp.SetPoint(counter,mass['mass'],histogram.GetBinContent(2))
        print "-> " + mass['name'] + "   expected: " + `histogram.GetBinContent(2)` + "    observed: " + `histogram.GetBinContent(1)`
        rootfile.Close()

###
# Creating the canvas
###
can = TCanvas("1DSig_"+signal,"1Sig_"+signal,800,800)
leg = TLegend(0.45,0.70,0.82,0.85)
leg.SetFillColor(0)
leg.SetLineColor(0)

#Theory
if signal.upper()=="TTD":
    tg_exp.GetHistogram().SetMinimum(-1)
    tg_exp.GetHistogram().SetMaximum(20)
if signal.upper()=="TTS":
    tg_exp.GetHistogram().SetMinimum(-1)
    tg_exp.GetHistogram().SetMaximum(15)
if signal.upper()=="TTHTHT":
    tg_exp.GetHistogram().SetMinimum(-1)
    tg_exp.GetHistogram().SetMaximum(23)
if signal.upper()=="TTZTZT":
    tg_exp.GetHistogram().SetMinimum(-1)
    tg_exp.GetHistogram().SetMaximum(18)

if(signal.upper()=="UEDRPP"):
    tg_exp.GetHistogram().SetMinimum(-5)
    tg_exp.GetHistogram().SetMaximum(27)
tg_exp.SetTitle("")
tg_exp.GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
'''
#1sigma
tg_1s.SetPoint(0,masses[0]['mass'],1)
tg_1s.SetPoint(1,masses[len(masses)-1]['mass'],1)
tg_1s.SetPoint(2,masses[len(masses)-1]['mass'],-1)
tg_1s.SetPoint(3,masses[0]['mass'],-1)

#2sigma
tg_2s.SetPoint(0,masses[0]['mass'],2)
tg_2s.SetPoint(1,masses[len(masses)-1]['mass'],2)
tg_2s.SetPoint(2,masses[len(masses)-1]['mass'],-2)
tg_2s.SetPoint(3,masses[0]['mass'],-2)
'''
#0sigma
tg_0s.SetPoint(0,masses[0]['mass'],0)
tg_0s.SetPoint(1,masses[len(masses)-1]['mass'],0)

if(signal.upper()=="TTD" or signal.upper()=="TTS"):
    tg_exp.GetXaxis().SetTitle("m_{T} [GeV]")
if(signal.upper()=="UEDRPP"):
    tg_exp.GetXaxis().SetTitle("m_{KK} [GeV]")

tg_exp.GetYaxis().SetTitle("Significance")

tg_exp.SetLineColor(kBlack)
tg_exp.SetLineWidth(2)
tg_exp.SetLineStyle(2)
tg_exp.Draw("alp")

tg_2s.Draw("lf")
tg_1s.Draw("lf")
tg_exp.Draw("lp")
tg_0s.SetLineWidth(1)
tg_0s.SetLineStyle(2)
tg_0s.Draw("l")

if data:
    tg_obs.SetLineColor(kBlack)
    tg_obs.SetLineWidth(3)
    tg_obs.SetLineStyle(1)
    tg_obs.Draw("lp")

#Legend
if data:
    leg.AddEntry(tg_obs,"Observed significance","l")
leg.AddEntry(tg_exp,"Expected significance","l")
leg.SetTextSize(0.04)
leg.Draw()

tl_atlas = TLatex(0.15,0.8,"ATLAS")
tl_atlas.SetNDC()
tl_atlas.SetTextFont(72)
tl_atlas.Draw()
tl_int = TLatex(0.3,0.8,"Internal")
tl_int.SetNDC()
tl_int.SetTextFont(42)
tl_int.Draw()
tl_energy = TLatex(0.15,0.73,"#sqrt{s} = "+energy+" TeV, "+lumi+" fb^{-1}")
tl_energy.SetNDC()
tl_energy.SetTextFont(42)
tl_energy.Draw()
if(addText!=""):
    tl_addtext = TLatex(0.6,0.66,addText)
    tl_addtext.SetNDC()
    tl_addtext.SetTextFont(42)
    tl_addtext.Draw()

gPad.RedrawAxis()
can.SetTickx()
can.SetTicky()

can.Print(outDir + "/" + signal.upper()+"_"+energy+"_"+lumi.replace(".","")+".eps")
can.Print(outDir + "/" + signal.upper()+"_"+energy+"_"+lumi.replace(".","")+".png")

can.Print(outDir + "/" + signal.upper()+"_"+energy+"_"+lumi.replace(".","")+".root")
