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

##_____________________________________________________________________________________________
##
def SetupTGraphObject(n_points, options, index=0, do_errors = False):
    tg = TGraph(n_points) if not do_errors else TGraphErrors(n_points)
    if "LINEWIDTH" in options: tg.SetLineWidth(options["LINEWIDTH"])
    if "LINESTYLE" in options: tg.SetLineStyle(options["LINESTYLE"])
    if "LINECOLOR" in options: tg.SetLineColorAlpha(options["LINECOLOR"][index][0], options["LINECOLOR"][index][1])
    if "FILLCOLOR" in options: tg.SetFillColorAlpha(options["FILLCOLOR"][index][0], options["FILLCOLOR"][index][1])
    if "FILLSTYLE" in options: tg.SetFillStyle(options["FILLSTYLE"])

    tg.GetXaxis().SetNdivisions(512)
    tg.GetHistogram().GetXaxis().SetLabelSize(tg.GetHistogram().GetXaxis().GetLabelSize()*1.2)
    tg.GetHistogram().GetYaxis().SetLabelSize(tg.GetHistogram().GetYaxis().GetLabelSize()*1.2)
    tg.GetHistogram().GetXaxis().SetTitleSize(tg.GetHistogram().GetXaxis().GetTitleSize()*1.4)
    tg.GetHistogram().GetYaxis().SetTitleSize(tg.GetHistogram().GetYaxis().GetTitleSize()*1.2)
    tg.GetHistogram().GetXaxis().SetTitleOffset(1.35)
    tg.GetHistogram().GetYaxis().SetTitleOffset(1.5)

    return tg

##_____________________________________________________________________________________________
##
def BuildMultiGraph(Masses, InputDir, InputDirSuffix, DoTheory, DoData, DoMulti, DoRatio, DoLogY, SignalLabel, DoAtlasLabel=True):

    mg = TMultiGraph()
    leg = SetupLegend(0.54,0.7,0.9,0.9)
    tl_atlas,tl_int,tl_energy = SetupTextLabel()

    #color and transparency
    color_list_exp   = [[kBlack, 1], [kBlue+1, 1], [kPink-9, 1], [kOrange+4, 1]]
    color_list_exp1s = [[kGreen  if not DoMulti else kBlack, 1 if not DoMulti else 0.2], [kBlue+1, 0.2], [kRed, 0.2], [kOrange+4, 0.2]]
    color_list_exp2s = [[kYellow if not DoMulti else kBlack, 1 if not DoMulti else 0.5], [kBlue+1, 0.5], [kRed, 0.5], [kOrange+4, 0.5]]

    tg_exp_options_dictionary    = {"LINEWIDTH":3, "LINESTYLE":2, "LINECOLOR":color_list_exp}
    tg_theory_options_dictionary = {"LINEWIDTH":2, "LINESTYLE":1, "LINECOLOR":[[kRed,1]], "FILLCOLOR":[[kRed-9,0.35]]}
    tg_obs_options_dictionary    = {"LINEWIDTH":3, "LINESTYLE":1, "LINECOLOR":color_list_exp}
    tg_exp1s_options_dictionary  = {"LINEWIDTH":3, "LINESTYLE":2, "LINECOLOR":color_list_exp1s, "FILLCOLOR":color_list_exp1s}
    tg_exp2s_options_dictionary  = {"LINEWIDTH":3, "LINESTYLE":2, "LINECOLOR":color_list_exp2s, "FILLCOLOR":color_list_exp2s}


    if DoMulti:
        tg_exp_options_dictionary["FILLCOLOR"]   = color_list_exp
        tg_exp_options_dictionary["FILLSTYLE"]   = 3005
        tg_exp1s_options_dictionary["FILLSTYLE"] = 3005
        tg_exp2s_options_dictionary["FILLSTYLE"] = 3354

    # Expected limits and uncertainties
    tg_exp   = [SetupTGraphObject(len(Masses),   tg_exp_options_dictionary,   index=i) for i in range(len(InputDir))]
    tg_exp1s = [SetupTGraphObject(2*len(Masses), tg_exp1s_options_dictionary, index=i) for i in range(len(InputDir))]
    tg_exp2s = [SetupTGraphObject(2*len(Masses), tg_exp2s_options_dictionary, index=i) for i in range(len(InputDir))]

    # Observed limits
    if DoData: tg_obs = [SetupTGraphObject(len(Masses), tg_obs_options_dictionary, index=i) for i in range(len(InputDir))]
        
    # Theory limits
    if DoTheory: tg_theory = SetupTGraphObject(len(Masses), tg_theory_options_dictionary, index=0, do_errors=True)

    # Expected Limit Ratios
    #if DoMulti and DoRatio:
        #mg_ratio  = TMultiGraph()
        #tg_ratio  = [TGraph(len(Masses)) for i in range(len(InputDir))]
        #"Ratio to %s limit"%labels[n]                                                                                                                              
        #SetGraphSettings(tg_ratio[i], XMin, XMax, 0.40, 1.2, SignalLabel, LineColor=cols[i], LineWidth=3, LineStyle=2)
        #mg_ratio.Add(tg_ratio[i], "l")

    # Filling the limit plots
    for iMass in range(len(Masses)): # Loop through all the masses and get the cross-sections and limits
        if DoTheory:
            tg_theory.SetPoint(iMass,Masses[iMass]['mass'],Masses[iMass]['xsec'])
            tg_theory.SetPointError(iMass, 0, 0. if 'err' not in Masses[iMass].keys() else Masses[iMass]['err'])
        
        for n, indir in enumerate(InputDir): # Assume same masses and signals available in all folders if doMulti
            files = glob.glob(indir+"/*"+Masses[iMass]['name']+InputDirSuffix+"*/Limits/asymptotics/*.root")
                
            if len(files)==0 or len(files)>1:
                print "<!> ERROR for mass " + `Masses[iMass]['mass']` + " !!"
            else:
                rootfile = TFile(files[0],"read")
                limtree = rootfile.Get('stats')
                limtree.GetEntry(0)
                print  " Mass: ", Masses[iMass]['mass'], " mu : ", limtree.obs_upperlimit, " xsec : ", Masses[iMass]['xsec']

                if DoData:
                    tg_obs[n].SetPoint(iMass, Masses[iMass]['mass'], limtree.obs_upperlimit*Masses[iMass]['xsec'])

                tg_exp[n].SetPoint(iMass, Masses[iMass]['mass'], limtree.exp_upperlimit*Masses[iMass]['xsec'])
                tg_exp1s[n].SetPoint(iMass, Masses[iMass]['mass'], limtree.exp_upperlimit_plus1*Masses[iMass]['xsec'])
                tg_exp2s[n].SetPoint(iMass, Masses[iMass]['mass'], limtree.exp_upperlimit_plus2*Masses[iMass]['xsec'])                    
                tg_exp1s[n].SetPoint(2*len(Masses)-iMass-1, Masses[iMass]['mass'],limtree.exp_upperlimit_minus1*Masses[iMass]['xsec'])
                tg_exp2s[n].SetPoint(2*len(Masses)-iMass-1, Masses[iMass]['mass'],limtree.exp_upperlimit_minus2*Masses[iMass]['xsec'])
                rootfile.Close()

                #if DoRatio:
                #    tg_ratio[n].SetPoint(iMass, Masses[iMass]['mass'], tg_exp[n].Eval(Masses[iMass]['mass'])/tg_exp[0].Eval(Masses[iMass]['mass']))


    # Adding objects to multigraph and creating legend entries
    for i in range(len(InputDir)):

        leg.AddEntry(tg_exp[i],"95% CL expected limit","l")
        leg.AddEntry(tg_exp1s[i],"95% CL expected limit #pm1#sigma","f")
        leg.AddEntry(tg_exp2s[i],"95% CL expected limit #pm2#sigma","f")
        mg.Add(tg_exp2s[i], "f")
        mg.Add(tg_exp1s[i], "f")
        mg.Add(tg_exp[i],   "l")

    if DoTheory:
        mg.Add(tg_theory, "l3")
        leg.AddEntry(tg_theory,"Theory (LO prediction)","l")

        intersx=FitFunctionAndDefineIntersection(tg_theory,tg_exp[0],isData=False )
        print "Expected limit: " + `intersx[0]`
        #mg.Add(intersx[1], "lp")
        if DoData:
            intersxData=FitFunctionAndDefineIntersection(tg_theory,tg_obs[0],isData=True )
            print "Observed limit: " + `intersxData[0]`
            #mg.Add(intersxData[1], "lp")

    if DoData:
        mg.Add(tg_obs[0], "l")
        leg.AddEntry(tg_obs[0], "95% CL observed limit","l")

    # Setup canvas to draw plots
    CanvasObjects = SetupCanvas(SignalLabel, DoRatio)
    can = CanvasObjects[0]
    can.cd()

    if DoRatio:
        pad1, pad2 = CanvasObjects[1], CanvasObjects[2]
        pad1.Draw()
        pad2.Draw()
        pad1.cd()

    mg.GetXaxis().SetTitle("m_{T} [GeV]")
    mg.GetYaxis().SetTitle("#sigma("+SignalLabel+") [pb]")
    mg.Draw("a")

    leg.Draw()
    
    if DoAtlasLabel:
        tl_atlas.Draw()
        tl_int.Draw()
    tl_energy.Draw()

    can.SetLogy()
    can.Draw()
    mg.GetXaxis().SetLimits(Masses[0]['mass'], Masses[-1]['mass'])
    mg.GetYaxis().SetRangeUser(0.0002, 17)
    gPad.RedrawAxis()
    can.SetTickx()
    can.SetTicky()
    can.SaveAs(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_log.eps")
    can.SaveAs(outDir + "/" + signal.upper()+"_"+lumi.replace(".","")+outSuffix+"_log.png")
    

##_____________________________________________________________________________________________
##
def SetupCanvas(signal, ratio):
    canvas = TCanvas("1DLimit_"+signal, "1DLimit_"+signal, 1000, 1150 if ratio else 800)
    if ratio:
        pad1 = TPad("pad1","",0,0.4,1,1)
        pad1.SetBottomMargin(0.15)
        pad1.SetLeftMargin(0.15)
        pad1.SetRightMargin(0.03)
        pad1.SetTopMargin(0.05)

        pad2 = TPad("pad2","",0,0.00,1,0.4)
        pad2.SetBottomMargin(0.15)
        pad2.SetLeftMargin(0.15)
        pad2.SetRightMargin(0.03)
        pad2.SetTopMargin(0.03)
    else:
        canvas.SetBottomMargin(0.15)
        canvas.SetLeftMargin(0.15)
        canvas.SetRightMargin(0.05)
        canvas.SetTopMargin(0.05)
        canvas.SetTicks(1,1)

    return  [canvas] if not ratio else [canvas, pad1, pad2]

##_____________________________________________________________________________________________
##
def SetupLegend(xmin,ymin,xmax,ymax):
    legend = TLegend(xmin,ymin,xmax,ymax)
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetFillStyle(0)
    legend.SetTextSize(0.028)
    return legend

##_____________________________________________________________________________________________
##
def SetupTextLabel(additional_text=""):
    tl_atlas = TLatex(0.19,0.88,"ATLAS")
    tl_atlas.SetNDC()
    tl_atlas.SetTextFont(72)
    tl_atlas.SetTextSize(tl_atlas.GetTextSize()*0.85)

    tl_int = TLatex(0.31,0.88,"Internal")
    tl_int.SetNDC()
    tl_int.SetTextFont(42)
    tl_int.SetTextSize(tl_int.GetTextSize()*0.85)

    tl_energy = TLatex(0.19,0.82,"#sqrt{s} = "+energy+" TeV, "+lumi+" fb^{-1}")
    tl_energy.SetNDC()
    tl_energy.SetTextFont(42)
    tl_energy.SetTextSize(tl_energy.GetTextSize()*0.85)

    if(additional_text!=""):
        tl_addtext = TLatex(0.19,0.18,additional_text)
        tl_addtext.SetNDC()
        tl_addtext.SetTextFont(42)
        tl_addtext.SetTextSize(tl_addtext.GetTextSize()*0.85)

    return tl_atlas,tl_int,tl_energy


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

if outSuffix != "":
    outSuffix = '_'+outSuffix

os.system("mkdir -p "+outDir)


inDir = map(str, inDir.strip('[]').split(','))
labels = map(str, labels.strip('[]').split(','))
doMulti = len(inDir) > 1

print doMulti
print ratio
print inDir

if(len(labels) != len(inDir)):
    print "< ERROR! > Number of labels and input directories does not match. Please check."
    sys.exit(-1)

if ratio and not doMulti:
    print "< ERROR! > Cannot do ratio panel if multiple input directoriies are not provided. Please check."
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

if len(masses)==1:
    mass = masses[0]
    files = glob.glob(inDir + "/*"+mass['name']+suffix+"*/Limits/asymptotics/*.root")
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
# Creating signal label
###
signal_label = ""
if(signal=="TTD"):
    signal_label = "SU(2) doublet"
elif(signal=="TTS"):
    signal_label = "SU(2) singlet"
elif(signal=="TTHTHT"):
    signal_label = "BR(T#rightarrowHt) = 1"
elif(signal.upper()=="TTZTZT"):
    signal_label = "BR(T#rightarrowZt) = 1"
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
# Effectively building the plots
###
BuildMultiGraph(masses, inDir, suffix, drawTheory, data, doMulti, ratio, True, signal_label, DoAtlasLabel=False)
