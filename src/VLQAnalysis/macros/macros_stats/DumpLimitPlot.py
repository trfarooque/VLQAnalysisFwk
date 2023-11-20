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

##_____________________________________________________________________________________________
##
def FitFunctionAndDefineIntersection( Theory, Med, isData ):
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
        Extra_Theory.SetLineColor(kBlack)
        Extra_Theory.SetLineStyle(2)

        Extra_Exp  = TF1("Extra_Exp","expo",x_ini_th,x_end_th)
        Med.Fit("Extra_Exp","RSQN","",x_ini_th,x_end_th)
        Extra_Exp.SetLineColor(kBlack)
        Extra_Exp.SetLineStyle(2)
        if not isData:
            Extra_Exp.Draw("same")

        for x in range(0,int(x_end_th-x_ini_th)):

            xmod=x_ini_th+x
            value_th = Extra_Theory.Eval(xmod)
            value_ex = Extra_Exp.Eval(xmod)
            diff = abs(value_th-value_ex)
            if(diff<diff_min):
                diff_min = diff
                x_int = xmod
                y_int = value_ex

    vertical = TGraph(2)
    vertical.SetPoint(0,x_int,0)
    vertical.SetPoint(1,x_int,y_int)
    vertical.SetLineStyle(2)
    if not isData:
        vertical.SetLineColor(kBlue)
    else:
        vertical.SetLineColor(kRed)
    vertical.SetLineWidth(2)

    return x_int,vertical
##.....................................................................................
##

gROOT.SetBatch(1)

##________________________________________________________
## OPTIONS
parser = OptionParser()
parser.add_option("-i","--inputDir",help="location of the log files ",dest="inDir",default="")
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
parser.add_option("-b","--labels",help="list of labels",dest="labels",default="")
parser.add_option("-r","--ratio",help="make ratio panel",dest="ratio",action="store_true",default=False)
parser.add_option

(options, args) = parser.parse_args()
inDir=options.inDir
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
labels=options.labels
ratio=options.ratio

# Build labels
labels = map(str, labels.strip('[]').split(','))

if outSuffix != "":
    outSuffix = '_'+outSuffix

os.system("mkdir -p "+outDir)

# Prepare for comparing multiple configurations in limit plots
doMulti = False
if inDir.startswith('[') and inDir.endswith(']'):
    inDir = map(str, inDir.strip('[]').split(','))
    doMulti = True

    if len(labels) != len(inDir):
        print "<!> ERROR !! Give labels of equal length when giving multiple input directories !!"
        sys.exit(-1)

if ratio and not doMulti:
    print "<!> ERROR !! Cannot do ratio panel if not given multiple input directories !!"
    sys.exit(-1)

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
            type="HtHt"
        if signal.upper()=="TTZTZT":
            type="ZtZt"
            
        # xsec and total uncertainty values taken from table 17 in https://cds.cern.ch/record/1662536/files/ATL-COM-PHYS-2014-112.pdf?

        #masses += [{'name':"VLQ_TT_600_"+type,'mass':600,'xsec':1.16,'err':0.10}]
        #masses += [{'name':"VLQ_TT_700_"+type,'mass':700,'xsec':0.455,'err':0.043}]
        #masses += [{'name':"VLQ_TT_750_"+type,'mass':750,'xsec':0.295,'err':0.029}]
        #masses += [{'name':"VLQ_TT_800_"+type,'mass':800,'xsec':0.195,'err':0.020}]
        #masses += [{'name':"VLQ_TT_850_"+type,'mass':850,'xsec':0.132,'err':0.014}]
        #masses += [{'name':"VLQ_TT_900_"+type,'mass':900,'xsec':0.0900,'err':0.0096}]
        #masses += [{'name':"VLQ_TT_950_"+type,'mass':950,'xsec':0.0624,'err':0.0068}]
        masses += [{'name':"VLQ_TT_1000_"+type,'mass':1000,'xsec':0.0438,'err':0.0048}]
        #masses += [{'name':"VLQ_TT_1050_"+type,'mass':1050,'xsec':0.0311,'err':0.0035}]
        masses += [{'name':"VLQ_TT_1100_"+type,'mass':1100,'xsec':0.0223,'err':0.0025}]
        #masses += [{'name':"VLQ_TT_1150_"+type,'mass':1150,'xsec':0.0161,'err':0.0018}]
        masses += [{'name':"VLQ_TT_1200_"+type,'mass':1200,'xsec':0.0117,'err':0.0013}]
        masses += [{'name':"VLQ_TT_1300_"+type,'mass':1300,'xsec':0.00634,'err':0.00075}]
        masses += [{'name':"VLQ_TT_1400_"+type,'mass':1400,'xsec':0.00350,'err':0.00043}]
        masses += [{'name':"VLQ_TT_1500_"+type,'mass':1500,'xsec':0.00197,'err':0.00026}]
        masses += [{'name':"VLQ_TT_1600_"+type,'mass':1600,'xsec':0.00112,'err':0.00015}]
        masses += [{'name':"VLQ_TT_1700_"+type,'mass':1700,'xsec':0.00065,'err':0.00009}]
        masses += [{'name':"VLQ_TT_1800_"+type,'mass':1800,'xsec':0.00038,'err':0.00006}]
        masses += [{'name':"VLQ_TT_2000_"+type,'mass':2000,'xsec':0.000132,'err':0.000024}]

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

        for m in xrange(1000,2200,200):
            masses += [{'name': type+str(m)+"K50", 'mass':m, 'xsec':0.1 }]

        # if(signal.upper()=="WTHT" or signal.upper()=="WTZT"):
        #     masses += [{'name': "sVLQ_"+type+"11K03", 'mass':1100, 'xsec':0.1 }]
        # else:
        #     masses += [{'name': "sVLQ_"+type+"11K05", 'mass':1100, 'xsec':0.1 }]

        # masses += [{'name': "sVLQ_"+type+"16K05", 'mass':1600, 'xsec':0.1 }]

        # if(not(signal.upper()=="ZTZT")):
        #     masses += [{'name': "sVLQ_"+type+"20K05", 'mass':2000, 'xsec':0.1 }]

if len(masses)==1:
    mass = masses[0]
    files = glob.glob(inDir + "/*"+mass['name']+suffix+"*/Limits/Asymptotics/*.root")
    if len(files)==0 or len(files)>1:
        print "<!> ERROR !!"
    else:
        limtree = rootfile.Get('stats')
        limtree.GetEntry(0)
        print "Observed limit: %.3f pb (mu: %.3f)" %( limtree.obs_upperlimit*mass['xsec'], limtree.obs_upperlimit)
        print "Expected +2s: %.3f pb (mu: %.3f)" %( limtree.exp_upperlimit_plus2*mass['xsec'], limtree.exp_upperlimit_plus2)
        print "Expected +1s: %.3f pb (mu: %.3f)" %( limtree.exp_upperlimit_plus1*mass['xsec'], limtree.exp_upperlimit_plus1)
        print "Expected limit: %.3f pb (mu: %.3f)" %( limtree.exp_upperlimit*mass['xsec'], limtree.exp_upperlimit)
        print "Expected -1s: %.3f pb (mu: %.3f)" %( limtree.exp_upperlimit_minus1*mass['xsec'], limtree.exp_upperlimit_minus1)
        print "Expected -2s: %.3f pb (mu: %.3f)" %( limtree.exp_upperlimit_minus2*mass['xsec'], limtree.exp_upperlimit_minus2)
        rootfile.Close()
    sys.exit(-1)

###
# Effectively building the plots
###
tg_theory = TGraphErrors(len(masses))

# if comparing mutliple configurations
if doMulti:
    tg_obs = [TGraph(len(masses)) for i in range(len(inDir))]
    tg_exp = [TGraph(len(masses)) for i in range(len(inDir))]
    tg_exp1s = [TGraph(2*len(masses)) for i in range(len(inDir))]
    tg_exp2s = [TGraph(2*len(masses)) for i in range(len(inDir))]

    if ratio:
        tg_ratio = [TGraph(len(masses)) for i in range(len(inDir))]
else:
    tg_obs = TGraph(len(masses))
    tg_exp = TGraph(len(masses))
    tg_exp1s = TGraph(2*len(masses))
    tg_exp2s = TGraph(2*len(masses))

#Theory plot
for iMass in range(len(masses)):
    tg_theory.SetPoint(iMass,masses[iMass]['mass'],masses[iMass]['xsec'])
#    if 'err' in masses[iMass].keys():
#        tg_theory.SetPointError(iMass,0,masses[iMass]['err'])
#    else:
#        tg_theory.SetPointError(iMass,0,0.)


#All limits

# Multiple configurations
counter = -1
if doMulti:
    for mass in masses: # Assume same masses and signals available in all folders if doMulti
        counter += 1
        for n,indir in enumerate(inDir):
            files = glob.glob(indir + "/*"+mass['name']+suffix+"*/Limits/Asymptotics/*.root")
            print(indir + "/*"+mass['name']+suffix+"*/Limits/Asymptotics/*.root")
            if len(files)==0 or len(files)>1:
                print "<!> ERROR for mass " + `mass['mass']` + " !!"
            else:
                rootfile = TFile(files[0],"read")
                limtree = rootfile.Get('stats')
                limtree.GetEntry(0)
                #print  " Mass: ", mass['mass'], " mu : ", limtree.exp_upperlimit, " xsec : ", mass['xsec']
                tg_obs[n].SetPoint(counter,mass['mass'],limtree.obs_upperlimit*mass['xsec'])
                tg_exp[n].SetPoint(counter,mass['mass'],limtree.exp_upperlimit*mass['xsec'])
                tg_exp1s[n].SetPoint(counter,mass['mass'],limtree.exp_upperlimit_plus1*mass['xsec'])
                tg_exp2s[n].SetPoint(counter,mass['mass'],limtree.exp_upperlimit_plus2*mass['xsec'])
                tg_exp1s[n].SetPoint(2*len(masses)-counter-1,mass['mass'],limtree.exp_upperlimit_minus1*mass['xsec'])
                tg_exp2s[n].SetPoint(2*len(masses)-counter-1,mass['mass'],limtree.exp_upperlimit_minus2*mass['xsec'])
                rootfile.Close()

                # Build ratio plot
                if ratio:
                    tg_ratio[n].SetPoint(counter,mass['mass'],tg_exp[n].Eval(mass['mass'])/tg_exp[0].Eval(mass['mass']))

else:
    for mass in masses:
        counter += 1
        files = glob.glob(inDir + "/*"+mass['name']+suffix+"*/Limits/Asymptotics/*.root")
        print(inDir + "/*"+mass['name']+suffix+"*/Limits/Asymptotics/*.root")
        if len(files)==0 or len(files)>1:
            print "<!> ERROR for mass " + `mass['mass']` + " !!"
        else:
            rootfile = TFile(files[0],"read")
            limtree = rootfile.Get('stats')
            limtree.GetEntry(0)
            #print  " Mass: ", mass['mass'], " mu : ", limtree.exp_upperlimit, " xsec : ", mass['xsec']
            tg_obs.SetPoint(counter,mass['mass'],limtree.obs_upperlimit*mass['xsec'])
            tg_exp.SetPoint(counter,mass['mass'],limtree.exp_upperlimit*mass['xsec'])
            tg_exp1s.SetPoint(counter,mass['mass'],limtree.exp_upperlimit_plus1*mass['xsec'])
            tg_exp2s.SetPoint(counter,mass['mass'],limtree.exp_upperlimit_plus2*mass['xsec'])
            tg_exp1s.SetPoint(2*len(masses)-counter-1,mass['mass'],limtree.exp_upperlimit_minus1*mass['xsec'])
            tg_exp2s.SetPoint(2*len(masses)-counter-1,mass['mass'],limtree.exp_upperlimit_minus2*mass['xsec'])
            rootfile.Close()

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
if ratio:
    can = TCanvas("1DLimit_"+signal,"1DLimit_"+signal,1000,1150)
    pad1 = TPad("pad1","",0,0.4,1,1)
    pad1.SetBottomMargin(0.15)
    pad1.SetLeftMargin(0.15)
    pad1.SetRightMargin(0.03)
    pad1.SetTopMargin(0.05)
    pad1.Draw()

    pad2 = TPad("pad2","",0,0.00,1,0.4)
    pad2.SetBottomMargin(0.15)
    pad2.SetLeftMargin(0.15)
    pad2.SetRightMargin(0.03)
    pad2.SetTopMargin(0.03)
    pad2.Draw()

    pad1.cd()
else:
    can = TCanvas("1DLimit_"+signal,"1DLimit_"+signal,1000,800)
    can.SetBottomMargin(0.15)
    can.SetLeftMargin(0.15)
    can.SetRightMargin(0.05)
    can.SetTopMargin(0.05)

leg = TLegend(0.59,0.7,0.95,0.9)
leg.SetFillColor(0)
leg.SetLineColor(0)

###
# Making axis limits
###
if forceRanges:
    ylim_min = 0.
    ylim_max = 0.27
    #ylim_min_log = 0.001
    #ylim_max_log = 3.
    ylim_min_log = 0.0002
    ylim_max_log = 17.

else:
    if drawTheory:
        if doMulti:
            ymin = tg_theory.GetHistogram().GetMinimum() if tg_theory.GetHistogram().GetMinimum() < min([t.GetHistogram().GetMinimum() for t in tg_exp2s]) else min([t.GetHistogram().GetMinimum() for t in tg_exp2s])
            ymax = tg_theory.GetHistogram().GetMaximum() if tg_theory.GetHistogram().GetMaximum() > max([t.GetHistogram().GetMaximum() for t in tg_exp2s]) else max([t.GetHistogram().GetMaximum() for t in tg_exp2s])
        else:
            ymin = tg_theory.GetHistogram().GetMinimum() if tg_theory.GetHistogram().GetMinimum() < tg_exp2s.GetHistogram().GetMinimum() else tg_exp2s.GetHistogram().GetMinimum()
            ymax = tg_theory.GetHistogram().GetMaximum() if tg_theory.GetHistogram().GetMaximum() > tg_exp2s.GetHistogram().GetMaximum() else tg_exp2s.GetHistogram().GetMaximum()
    else:
        if doMulti:
            ymin = min([t.GetHistogram().GetMinimum() for t in tg_exp2s])
            ymax = max([t.GetHistogram().GetMaximum() for t in tg_exp2s])
        else:
            ymin = tg_exp2s.GetHistogram().GetMinimum()
            ymax = tg_exp2s.GetHistogram().GetMaximum()            
    
    ylim_min = ymin - 0.2*(ymax-ymin) if (ymin - 0.2*(ymax-ymin)) > 0. else 0.
    ylim_max = ymin + 1.5*(ymax-ymin)
    ylim_min_log = ymin/1.1
    ylim_max_log = ymax*25

###
#Theory
###
if drawTheory:
    tg_theory.SetLineColor(kRed)
    tg_theory.SetFillColor(kRed-9)

    tg_theory.GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
    tg_theory.SetMinimum(ylim_min)
    tg_theory.SetMaximum(ylim_max)
    tg_theory.GetXaxis().SetNdivisions(406)
    tg_theory.SetLineWidth(2)

    tg_theory.Draw("al3")
    tg_theory.SetTitle("")
    tg_theory.GetHistogram().GetXaxis().SetLabelSize(tg_theory.GetHistogram().GetXaxis().GetLabelSize()*1.2)
    tg_theory.GetHistogram().GetYaxis().SetLabelSize(tg_theory.GetHistogram().GetYaxis().GetLabelSize()*1.2)
    tg_theory.GetHistogram().GetXaxis().SetTitleSize(tg_theory.GetHistogram().GetXaxis().GetTitleSize()*1.4)
    tg_theory.GetHistogram().GetYaxis().SetTitleSize(tg_theory.GetHistogram().GetYaxis().GetTitleSize()*1.2)
    tg_theory.GetHistogram().GetXaxis().SetTitleOffset(1.35)
    tg_theory.GetHistogram().GetYaxis().SetTitleOffset(1.5)

###
# Limits
###
cols = [kBlack,kBlue+1,kRed,kOrange+4] # max compare 4 configurations
fills = [3002,3004,3005,3007]
if doMulti:
    for n in range(len(inDir)):
        tg_exp2s[n].SetLineColorAlpha(cols[n],0.5)
        tg_exp2s[n].SetFillColorAlpha(cols[n],0.5)
        tg_exp2s[n].SetFillStyle(3354)
        if not drawTheory:
            tg_exp2s[n].GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
            tg_exp2s[n].SetMinimum(ylim_min)
            tg_exp2s[n].SetMaximum(ylim_max)
            #tg_exp2s[n].GetXaxis().SetNdivisions(406)
            tg_exp2s[n].GetXaxis().SetNdivisions(512)
            tg_exp2s[n].SetTitle("")
            tg_exp2s[n].GetXaxis().SetTitle("m_{T} [GeV]")
            tg_exp2s[n].GetYaxis().SetTitle("#sigma(%s) [pb]"%signal_label)
            tg_exp2s[n].GetHistogram().GetXaxis().SetLabelSize(tg_exp2s[n].GetHistogram().GetXaxis().GetLabelSize()*1.2)
            tg_exp2s[n].GetHistogram().GetYaxis().SetLabelSize(tg_exp2s[n].GetHistogram().GetYaxis().GetLabelSize()*1.2)
            tg_exp2s[n].GetHistogram().GetXaxis().SetTitleSize(tg_exp2s[n].GetHistogram().GetXaxis().GetTitleSize()*1.4)
            tg_exp2s[n].GetHistogram().GetYaxis().SetTitleSize(tg_exp2s[n].GetHistogram().GetYaxis().GetTitleSize()*1.2)
            tg_exp2s[n].GetHistogram().GetXaxis().SetTitleOffset(1.35)
            tg_exp2s[n].GetHistogram().GetYaxis().SetTitleOffset(1.5)
            if n==0:
                tg_exp2s[n].Draw("afl")
            else:
                tg_exp2s[n].Draw("fl")
        else:
            tg_exp2s[n].Draw("f")


        tg_exp1s[n].SetLineColorAlpha(cols[n],0.2)
        tg_exp1s[n].SetFillColorAlpha(cols[n],0.2)
        tg_exp1s[n].SetFillStyle(3005)
        tg_exp1s[n].Draw("fl")
    
        tg_exp[n].SetLineColor(cols[n])
        tg_exp[n].SetFillColor(cols[n])
        tg_exp[n].SetFillStyle(3005)
        tg_exp[n].SetLineWidth(3)
        tg_exp[n].SetLineStyle(2)
        tg_exp[n].Draw("l")
        tg_theory.Draw("l3 same")
########################
else:
    tg_exp2s.SetLineColor(kYellow)
    tg_exp2s.SetFillColor(kYellow)
    if not drawTheory:
        tg_exp2s.GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
        tg_exp2s.SetMinimum(ylim_min)
        tg_exp2s.SetMaximum(ylim_max)
        #tg_exp2s.GetXaxis().SetNdivisions(406)
        tg_exp2s.GetXaxis().SetNdivisions(512)
        tg_exp2s.SetTitle("")
        tg_exp2s.GetXaxis().SetTitle("m_{T} [GeV]")
        tg_exp2s.GetYaxis().SetTitle("#sigma(%s) [pb]"%signal_label)
        tg_exp2s.GetHistogram().GetXaxis().SetLabelSize(tg_exp2s.GetHistogram().GetXaxis().GetLabelSize()*1.2)
        tg_exp2s.GetHistogram().GetYaxis().SetLabelSize(tg_exp2s.GetHistogram().GetYaxis().GetLabelSize()*1.2)
        tg_exp2s.GetHistogram().GetXaxis().SetTitleSize(tg_exp2s.GetHistogram().GetXaxis().GetTitleSize()*1.4)
        tg_exp2s.GetHistogram().GetYaxis().SetTitleSize(tg_exp2s.GetHistogram().GetYaxis().GetTitleSize()*1.2)
        tg_exp2s.GetHistogram().GetXaxis().SetTitleOffset(1.35)
        tg_exp2s.GetHistogram().GetYaxis().SetTitleOffset(1.5)
        tg_exp2s.Draw("af")
    else:
        tg_exp2s.Draw("f")

    tg_exp1s.SetLineColor(kGreen)
    tg_exp1s.SetFillColor(kGreen)
    tg_exp1s.Draw("f")

    tg_exp.SetLineColor(kBlack)
    tg_exp.SetLineWidth(3)
    tg_exp.SetLineStyle(2)
    tg_exp.Draw("l")

########################

if data:
    tg_obs.SetLineColor(kBlack)
    tg_obs.SetLineWidth(3)
    tg_obs.SetLineStyle(1)
    tg_obs.Draw("l")

if drawTheory:
    if signal.find("UEDRPP")==-1:
       leg.AddEntry(tg_theory,"Theory (NNLO prediction #pm1#sigma)","lf")
    else:
       leg.AddEntry(tg_theory,"Theory (LO prediction)","l")

if data:
    leg.AddEntry(tg_obs,"95% CL observed limit","l")

if doMulti:

    # set up dummy graph for legend
    tg_dummy_exp,tg_dummy_exp1s,tg_dummy_exp2s = TGraph(),TGraph(),TGraph()
    tg_dummy_exp.SetLineColor(kGray+2)
    tg_dummy_exp.SetLineWidth(3)
    tg_dummy_exp.SetLineStyle(2)
    tg_dummy_exp2s.SetFillColorAlpha(kGray+2,0.2)
    tg_dummy_exp2s.SetFillStyle(3354)
    tg_dummy_exp1s.SetFillColorAlpha(kGray+2,0.5)
    tg_dummy_exp1s.SetFillStyle(3005)

    leg.AddEntry(tg_dummy_exp,"95% CL expected limit","l")
    leg.AddEntry(tg_dummy_exp1s,"95% CL expected limit #pm1#sigma","f")
    leg.AddEntry(tg_dummy_exp2s,"95% CL expected limit #pm2#sigma","f")

    for n in range(len(inDir)):
        leg.AddEntry(tg_exp[n],labels[n],"fl")

else:
    leg.AddEntry(tg_exp,"95% CL expected limit","l")
    leg.AddEntry(tg_exp1s,"95% CL expected limit #pm1#sigma","f")
    leg.AddEntry(tg_exp2s,"95% CL expected limit #pm2#sigma","f")

leg.SetTextSize(0.028)
leg.Draw()

#Intersections
#intersx=FitFunctionAndDefineIntersection(tg_theory,tg_exp,isData=False )
#print "Expected limit: " + `intersx[0]`
#intersx[1].Draw("lp")
#if(data):
#    intersxData=FitFunctionAndDefineIntersection(tg_theory,tg_obs,isData=True )
#    print "Observed limit: " + `intersxData[0]`


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
    leg.AddEntry(TObject(), signal_label, "")


# Draw ratio panel
if ratio:
    can.cd()
    pad2.cd()

    #leg_r = TLegend(0.7,0.72,0.96,0.95)
    leg_r = TLegend(0.7, 0.42, 0.96, 0.65)
    leg_r.SetFillColor(0)
    leg_r.SetLineColor(0)
    leg_r.SetTextSize(0.045)

    for n in range(len(inDir)):
        tg_ratio[n].SetLineColor(cols[n])
        tg_ratio[n].SetLineWidth(3)
        tg_ratio[n].SetLineStyle(2)
        
        if n==0:            
            tg_ratio[n].GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
            #tg_ratio[n].SetMinimum(0.80)
            tg_ratio[n].SetMinimum(0.40)
            tg_ratio[n].SetMaximum(1.2)
            tg_ratio[n].GetXaxis().SetNdivisions(406)
            tg_ratio[n].SetTitle("")
            tg_ratio[n].GetXaxis().SetTitle("m_{T} [GeV]")
            tg_ratio[n].GetYaxis().SetTitle("Ratio to %s limit"%labels[n])
            tg_ratio[n].GetHistogram().GetXaxis().SetLabelSize(tg_ratio[n].GetHistogram().GetXaxis().GetLabelSize()*1.6)
            tg_ratio[n].GetHistogram().GetYaxis().SetLabelSize(tg_ratio[n].GetHistogram().GetYaxis().GetLabelSize()*1.6)
            tg_ratio[n].GetHistogram().GetXaxis().SetTitleSize(tg_ratio[n].GetHistogram().GetXaxis().GetTitleSize()*1.8)
            tg_ratio[n].GetHistogram().GetYaxis().SetTitleSize(tg_ratio[n].GetHistogram().GetYaxis().GetTitleSize()*1.6)
            tg_ratio[n].GetHistogram().GetXaxis().SetTitleOffset(1.15)
            tg_ratio[n].GetHistogram().GetYaxis().SetTitleOffset(1.2)
            tg_ratio[n].Draw("al")
        else:
            tg_ratio[n].Draw("l")

        leg_r.AddEntry(tg_ratio[n],labels[n],"l")
    leg_r.Draw()


gPad.RedrawAxis()
can.SetTickx()
can.SetTicky()

can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+".eps")
can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+".png")

# No log plot if comparison
if doMulti:
    sys.exit(-1)

can.SetLogy()

########################
if doMulti:
    if drawTheory:
        tg_theory.GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
        tg_theory.SetMinimum(ylim_min_log)
        tg_theory.SetMaximum(ylim_max_log)
        tg_theory.Draw("al3")
        for n in range(len(inDir)):
            tg_exp2s[n].Draw("af")
            tg_exp1s[n].Draw("f")
            tg_exp[n].Draw("l")
        leg.Draw()
        tl_atlas.Draw()
        tl_int.Draw()
        tl_energy.Draw()
        if(addText!=""):
            tl_addtext.Draw()
        tg_theory.GetXaxis().Draw()
        tg_theory.GetYaxis().Draw()
    else:
        for n in range(len(inDir)):
            tg_exp2s[n].GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
            tg_exp2s[n].SetMinimum(ylim_min_log)
            tg_exp2s[n].SetMaximum(ylim_max_log)
            tg_exp2s[n].Draw("af")
            tg_exp1s[n].Draw("f")
            tg_exp[n].Draw("l")
        leg.Draw()
        tl_atlas.Draw()
        tl_int.Draw()
        tl_energy.Draw()
        if(addText!=""):
            tl_addtext.Draw()
        tg_exp2s[-1].GetXaxis().Draw()
        tg_exp2s[-1].GetYaxis().Draw()

########################

else:
    if drawTheory:
        tg_theory.GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
        tg_theory.SetMinimum(ylim_min_log)
        tg_theory.SetMaximum(ylim_max_log)
        tg_exp2s.Draw("af")
        tg_exp1s.Draw("f")
        tg_exp.Draw("l")
        tg_theory.Draw("l3 same")
        leg.Draw()
        tl_atlas.Draw()
        tl_int.Draw()
        tl_energy.Draw()
        if(addText!=""):
            tl_addtext.Draw()
        tg_theory.GetXaxis().Draw()
        tg_theory.GetYaxis().Draw()
    else:
        tg_exp2s.GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
        tg_exp2s.SetMinimum(ylim_min_log)
        tg_exp2s.SetMaximum(ylim_max_log)
        tg_exp2s.Draw("af")
        tg_exp1s.Draw("f")
        tg_exp.Draw("l")
        leg.Draw()
        tl_atlas.Draw()
        tl_int.Draw()
        tl_energy.Draw()
        if(addText!=""):
            tl_addtext.Draw()
        tg_exp2s.GetXaxis().Draw()
        tg_exp2s.GetYaxis().Draw()

########################

gPad.RedrawAxis()
can.SetTickx()
can.SetTicky()

can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_log.eps")
can.Print(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_log.png")

