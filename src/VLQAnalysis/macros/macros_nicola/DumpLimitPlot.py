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

#final asimov stuff
#parser.add_option("-i","--inputDir",help="location of the log files ",dest="inDir",default="/afs/cern.ch/work/o/orlando/TRexFitter/rev/trunk/")

parser.add_option("-i","--inputDir",help="location of the log files ",dest="inDir",default="/afs/cern.ch/work/o/orlando/HBSM_fit_optimisation/")

parser.add_option("-o","--outDir",help="output folder",dest="outDir",default="./test/")
parser.add_option("-s","--signal",help="signal sample",dest="signal",default="TBH")
parser.add_option("-m","--model",help="2HDM model type",dest="model_type",default=1)
parser.add_option("-z","--higgs_boson",help="Use scalar or speudo-scalar",dest="do_scalar",default="H")
parser.add_option("-e","--energy",help="energy",dest="energy",default="13")
parser.add_option("-a","--addText",help="additional text to plot",dest="addText",default="")
parser.add_option("-l","--lumi",help="luminosity",dest="lumi",default="35")
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
model_type=options.model_type
do_scalar=options.do_scalar

print 'do scalar %s' %(do_scalar)

os.system("mkdir -p "+outDir)

###
# Getting the values of the masses and cross-sections
###
masses = []
if(energy=="13"):
    if(signal.upper()=="TTD" or signal.upper()=="TTS" or signal.upper()=="TTHTHT" or signal.upper()=="TBH" or signal.upper()=="TTH" or signal.upper()=="TTHBB" or signal.upper()=="BBH"):
        type = "TDoublet"
        table_name = "tbH"
        if signal.upper()=="TBH":
            type="tbH"
            table_name = "tbH"

            tier_factor=1.
            
            masses += [{'name':"batch_lowmass/batch_lowmass_Hplus_200"  ,'mass':200  ,'xsec':tier_factor}]
            #masses += [{'name':"batch_lowmass/batch_lowmass_Hplus_250"  ,'mass':250  ,'xsec':tier_factor}]
            masses += [{'name':"batch_lowmass/batch_lowmass_Hplus_300"  ,'mass':300  ,'xsec':tier_factor}]
            masses += [{'name':"batch_lowmass/batch_lowmass_Hplus_350"  ,'mass':350  ,'xsec':tier_factor}]
            masses += [{'name':"batch_lowmass/batch_lowmass_Hplus_400"  ,'mass':400  ,'xsec':tier_factor}]
            masses += [{'name':"batch_lowmass/batch_lowmass_Hplus_500"  ,'mass':500  ,'xsec':tier_factor}]
            #masses += [{'name':"batch_lowmass/batch_lowmass_Hplus_600"  ,'mass':600  ,'xsec':tier_factor}]
            masses += [{'name':"batch_lowmass/batch_lowmass_Hplus_700"  ,'mass':700  ,'xsec':tier_factor}]
            #masses += [{'name':"batch_hplushighmass/batch_hplushighmass_Hplus_800"  ,'mass':800  ,'xsec':tier_factor}]
            #masses += [{'name':"batch_hplushighmass/batch_hplushighmass_Hplus_900"  ,'mass':900  ,'xsec':tier_factor}]
            masses += [{'name':"batch_hplushighmass/batch_hplushighmass_Hplus_1000"  ,'mass':1000  ,'xsec':tier_factor}]
            masses += [{'name':"batch_hplushighmass/batch_hplushighmass_Hplus_1200"  ,'mass':1200  ,'xsec':tier_factor}]
            masses += [{'name':"batch_hplushighmass/batch_hplushighmass_Hplus_1400"  ,'mass':1400  ,'xsec':tier_factor}]
            masses += [{'name':"batch_hplushighmass/batch_hplushighmass_Hplus_1600"  ,'mass':1600  ,'xsec':tier_factor}]
            masses += [{'name':"batch_hplushighmass/batch_hplushighmass_Hplus_1800"  ,'mass':1800  ,'xsec':tier_factor}]
            masses += [{'name':"batch_hplushighmass/batch_hplushighmass_Hplus_2000"  ,'mass':2000  ,'xsec':tier_factor}]

        if signal.upper()=="TTH":
            type="ttH"
            if do_scalar == "A" :
                table_name = "A_ttH"
            if do_scalar == "H" :
                table_name = "H_ttH"

            tier_factor=1.

            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_tttt_400"  ,'mass':400  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_tttt_500"  ,'mass':500  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_tttt_600"  ,'mass':600  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_tttt_700"  ,'mass':700  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_tttt_800"  ,'mass':800  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_tttt_900"  ,'mass':900  ,'xsec':tier_factor}]
            #masses += [{'name':"batch_ttbb_tttt_tttt_1000" ,'mass':1000 ,'xsec':tier_factor}]

        if signal.upper()=="TTHBB":
            type="ttHbb"
            if do_scalar == "A" :
                table_name = "ttAbb"
            if do_scalar == "H" :
                table_name = "ttHbb"

            tier_factor=1.

            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_ttbb_200"  ,'mass':200  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_ttbb_300"  ,'mass':300  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_ttbb_400"  ,'mass':400  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_ttbb_500"  ,'mass':500  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_ttbb_600"  ,'mass':600  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_ttbb_700"  ,'mass':700  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_ttbb_800"  ,'mass':800  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_ttbb_900"  ,'mass':900  ,'xsec':tier_factor}]
            masses += [{'name':"batch_ttbb_tttt/batch_ttbb_tttt_ttbb_1000" ,'mass':1000 ,'xsec':tier_factor}]

        if signal.upper()=="BBH":
            type="bbH"
            if do_scalar == "A" :
                table_name = "A_bb"
            if do_scalar == "H" :
                table_name = "H_bb"

            tier_factor=1.
            
            masses += [{'name':"batch_lowmass/batch_lowmass_bbtt_400"          ,'mass':400  ,'xsec':tier_factor}]
            masses += [{'name':"batch_lowmass/batch_lowmass_bbtt_500"          ,'mass':500  ,'xsec':tier_factor}]
            masses += [{'name':"batch_lowmass/batch_lowmass_bbtt_600"          ,'mass':600  ,'xsec':tier_factor}]
            masses += [{'name':"batch_lowmass/batch_lowmass_bbtt_700"          ,'mass':700  ,'xsec':tier_factor}]
            masses += [{'name':"batch_bbHhighmass/batch_bbHhighmass_bbtt_800"  ,'mass':800  ,'xsec':tier_factor}]
            masses += [{'name':"batch_bbHhighmass/batch_bbHhighmass_bbtt_900"  ,'mass':900  ,'xsec':tier_factor}]
            masses += [{'name':"batch_bbHhighmass/batch_bbHhighmass_bbtt_1000" ,'mass':1000 ,'xsec':tier_factor}]

        if signal.upper()=="TTS":
            type="TSinglet"
        if signal.upper()=="TTHTHT":
            type="BR_0.00_0.00_1.00"
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
        tier_factor = 1.
        masses += [{'name':"UEDRPP_1000",'mass':1000,'xsec':0.3429*tier_factor}]
        masses += [{'name':"UEDRPP_1200",'mass':1200,'xsec':0.07559*tier_factor}]
        masses += [{'name':"UEDRPP_1400",'mass':1400,'xsec':0.01804*tier_factor}]
        masses += [{'name':"UEDRPP_1600",'mass':1600,'xsec':0.004464*tier_factor}]
        masses += [{'name':"UEDRPP_1800",'mass':1800,'xsec':0.001112*tier_factor}]
    elif(signal.upper()=="CI4TOPS"):
        masses += [{'name':"CI4tops",'mass':1800,'xsec':0.928}]
    elif(signal.upper()=="SM4TOPS"):
        masses += [{'name':"SM4tops",'mass':1800,'xsec':0.009201}]

if len(masses)==1:
    mass = masses[0]
    files = glob.glob(inDir + "/*"+mass['name']+"*/Limits/*.root")
    print "files= " %(files) 
    if len(files)==0 or len(files)>1:
        print "<!> ERROR !!"
    else:
        rootfile = TFile(files[0],"read")
        histogram = rootfile.Get("limit")
        print "Observed limit: %.3f pb (mu: %.3f)" %( histogram.GetBinContent(1)*mass['xsec'], histogram.GetBinContent(1))
        print "Expected +2s: %.3f pb (mu: %.3f)" %( histogram.GetBinContent(3)*mass['xsec'], histogram.GetBinContent(3))
        print "Expected +1s: %.3f pb (mu: %.3f)" %( histogram.GetBinContent(4)*mass['xsec'], histogram.GetBinContent(4))
        print "Expected limit: %.3f pb (mu: %.3f)" %( histogram.GetBinContent(2)*mass['xsec'], histogram.GetBinContent(2))
        print "Expected -1s: %.3f pb (mu: %.3f)" %( histogram.GetBinContent(5)*mass['xsec'], histogram.GetBinContent(5))
        print "Expected -2s: %.3f pb (mu: %.3f)" %( histogram.GetBinContent(6)*mass['xsec'], histogram.GetBinContent(6))
        rootfile.Close()
    sys.exit(-1)

###
# Effectively building the plots
###
tg_obs = TGraph(len(masses))
tg_exp = TGraph(len(masses))
tg_exp1s = TGraph(2*len(masses))
tg_exp2s = TGraph(2*len(masses))
tg_theory = TGraphErrors(len(masses))

#Theory
name_tb_01 ="theory/CrossSec_xsec_%s_type_%s_tanbeta_1.txt"%(table_name,model_type)   
name_tb_03 ="theory/CrossSec_xsec_%s_type_%s_tanbeta_3.txt"%(table_name,model_type)   
name_tb_05 ="theory/CrossSec_xsec_%s_type_%s_tanbeta_5.txt"%(table_name,model_type)  
name_tb_1  ="theory/CrossSec_xsec_%s_type_%s_tanbeta_10.txt"%(table_name,model_type)  
name_tb_5  ="theory/CrossSec_xsec_%s_type_%s_tanbeta_50.txt"%(table_name,model_type)  
name_tb_10 ="theory/CrossSec_xsec_%s_type_%s_tanbeta_100.txt"%(table_name,model_type)  
name_tb_50 ="theory/CrossSec_xsec_%s_type_%s_tanbeta_10.txt"%(table_name,model_type)  

#add one more point
if(signal=="TBH"): 
    name_tb_50 ="theory/CrossSec_xsec_%s_type_%s_tanbeta_500.txt"%(table_name,model_type)  

print "Opening theory file %s"%(name_tb_01 ) 
print "Opening theory file %s"%(name_tb_03 ) 
print "Opening theory file %s"%(name_tb_05 ) 
print "Opening theory file %s"%(name_tb_1  ) 
print "Opening theory file %s"%(name_tb_5  ) 
print "Opening theory file %s"%(name_tb_10 ) 
print "Opening theory file %s"%(name_tb_50 ) 

gr_tb_01  = TGraph(name_tb_01)
gr_tb_03  = TGraph(name_tb_03)
gr_tb_05  = TGraph(name_tb_05)
gr_tb_1   = TGraph(name_tb_1 )
gr_tb_5   = TGraph(name_tb_5)
gr_tb_10  = TGraph(name_tb_10)
gr_tb_50  = TGraph(name_tb_50)

#Theory plot
for iMass in range(len(masses)):
    tg_theory.SetPoint(iMass,masses[iMass]['mass'],masses[iMass]['xsec'])
    if 'err' in masses[iMass].keys():
        tg_theory.SetPointError(iMass,0,masses[iMass]['err'])
    else:
        tg_theory.SetPointError(iMass,0,0.)

tg_theory.SetLineColor(kWhite)
tg_theory.SetFillColor(kWhite)

tg_theory.GetXaxis().SetLimits(masses[0]['mass'],masses[len(masses)-1]['mass'])
tg_theory.GetHistogram().SetMaximum(tg_theory.GetHistogram().GetMaximum()*10.)
tg_theory.SetLineWidth(2)

#All limits
counter = -1
for mass in masses:
    counter += 1
    files = glob.glob(inDir + "/*"+mass['name']+"*/Limits/*.root")
    print "Opening inDir %s mass %s" %(inDir,mass['name'])
    if len(files)==0 or len(files)>1:
        print "<!> ERROR !!"
    else:
        rootfile = TFile(files[0],"read")
        histogram = rootfile.Get("limit")
        tg_obs.SetPoint(counter,mass['mass'],histogram.GetBinContent(1)*mass['xsec'])
        tg_exp.SetPoint(counter,mass['mass'],histogram.GetBinContent(2)*mass['xsec'])
        tg_exp1s.SetPoint(counter,mass['mass'],histogram.GetBinContent(4)*mass['xsec'])
        tg_exp2s.SetPoint(counter,mass['mass'],histogram.GetBinContent(3)*mass['xsec'])
        tg_exp1s.SetPoint(2*len(masses)-counter-1,mass['mass'],histogram.GetBinContent(5)*mass['xsec'])
        tg_exp2s.SetPoint(2*len(masses)-counter-1,mass['mass'],histogram.GetBinContent(6)*mass['xsec'])
        rootfile.Close()

###
# Creating the canvas
###
can = TCanvas("1DLimit_"+signal,"1DLimit_"+signal,800,650)

leg = TLegend(0.48,0.81,0.85,0.95)
leg.SetFillColor(0)               
leg.SetLineColor(0)               

leg_2 = TLegend(0.48,0.72,0.95,0.77)
leg_2.SetFillColor(0)               
leg_2.SetLineColor(0)               

#Theory
if(signal.upper()=="TTD" or signal.upper()=="TTS" or signal.upper()=="TTHTHT"):
    tg_theory.GetHistogram().SetMinimum(0.003)
if(signal.upper()=="TBH"):
    tg_theory.GetHistogram().SetMinimum(0.001)
    tg_theory.GetHistogram().SetMaximum(1000)
if(signal.upper()=="TTH"):
    tg_theory.GetHistogram().SetMinimum(0.001)
    tg_theory.GetHistogram().SetMaximum(1000)
if(signal.upper()=="TTHBB"):
    tg_theory.GetHistogram().SetMinimum(0.001)
    tg_theory.GetHistogram().SetMaximum(1000)
if(signal.upper()=="BBH"):
    tg_theory.GetHistogram().SetMinimum(0.001)
    tg_theory.GetHistogram().SetMaximum(1000)
if(signal.upper().find("UEDRPP")>-1):
    tg_theory.GetHistogram().SetMinimum(tg_exp2s.GetHistogram().GetMinimum()/1.5)
tg_theory.Draw("al3")
#tg_theory.Draw("a")
tg_theory.SetTitle("")
tg_theory.GetHistogram().GetXaxis().SetLabelSize(tg_theory.GetHistogram().GetXaxis().GetLabelSize()*1.2)
tg_theory.GetHistogram().GetYaxis().SetLabelSize(tg_theory.GetHistogram().GetYaxis().GetLabelSize()*1.2)
tg_theory.GetHistogram().GetXaxis().SetTitleSize(tg_theory.GetHistogram().GetXaxis().GetTitleSize()*1.2)
tg_theory.GetHistogram().GetYaxis().SetTitleSize(tg_theory.GetHistogram().GetYaxis().GetTitleSize()*1.2)
tg_theory.GetHistogram().GetXaxis().SetTitleOffset(1.4)
tg_theory.GetHistogram().GetYaxis().SetTitleOffset(1.4)

if(signal.upper()=="TTD" or signal.upper()=="TTS" or signal.upper()=="TTHTHT"):
    tg_theory.GetXaxis().SetTitle("m_{T} [GeV]")
    tg_theory.GetYaxis().SetTitle("#sigma(pp #rightarrow T#bar{T}) [pb]")

if(signal.upper()=="TBH"):
        tg_theory.GetXaxis().SetTitle("m_{H^{+}} [GeV]")
        tg_theory.GetYaxis().SetTitle("#sigma(pp #rightarrow t#bar{b}H^{+}) #times BR(H^{+} #rightarrow t#bar{b}) [pb]")
if(signal.upper()=="TTH"):
    if do_scalar == "H" :
        tg_theory.GetXaxis().SetTitle("m_{H} [GeV]")
        tg_theory.GetYaxis().SetTitle("#sigma(pp #rightarrow t#bar{t}H) #times BR(H #rightarrow t#bar{t}) [pb]")
    if do_scalar == "A" :
        tg_theory.GetXaxis().SetTitle("m_{A} [GeV]")
        tg_theory.GetYaxis().SetTitle("#sigma(pp #rightarrow t#bar{t}A) #times BR(A #rightarrow t#bar{t}) [pb]")
if(signal.upper()=="TTHBB"):
    if do_scalar == "H" :
        tg_theory.GetXaxis().SetTitle("m_{H} [GeV]")
        tg_theory.GetYaxis().SetTitle("#sigma(pp #rightarrow t#bar{t}H) #times BR(H #rightarrow b#bar{b}) [pb]")
    if do_scalar == "A" :
        tg_theory.GetXaxis().SetTitle("m_{A} [GeV]")
        tg_theory.GetYaxis().SetTitle("#sigma(pp #rightarrow t#bar{t}A) #times BR(A #rightarrow b#bar{b}) [pb]")
if(signal.upper()=="BBH"):
    if do_scalar == "H" :
        tg_theory.GetXaxis().SetTitle("m_{H} [GeV]")
        tg_theory.GetYaxis().SetTitle("#sigma(pp #rightarrow b#bar{b}H) #times BR(H #rightarrow t#bar{t}) [pb]")
    if do_scalar == "A" :
        tg_theory.GetXaxis().SetTitle("m_{A} [GeV]")
        tg_theory.GetYaxis().SetTitle("#sigma(pp #rightarrow b#bar{b}A) #times BR(A #rightarrow t#bar{t}) [pb]")

if(signal.upper().find("UEDRPP")>-1):
    tg_theory.GetXaxis().SetTitle("m_{KK} [GeV]")
    tg_theory.GetYaxis().SetTitle("#sigma #times BR [pb]")

#Limits
tg_exp2s.SetLineColor(kYellow)
tg_exp2s.SetFillColor(kYellow)
tg_exp2s.SetMarkerSize(1)
tg_exp2s.Draw("f")

tg_exp1s.SetLineColor(kGreen)
tg_exp1s.SetFillColor(kGreen)
tg_exp1s.Draw("f")

#tg_theory.Draw("l3")
#tg_theory.Draw("lX")

tg_exp.SetLineColor(kBlack)
tg_exp.SetLineWidth(3)
tg_exp.SetLineStyle(2)
#tg_exp.SetMarkerStyle(kFullCircle)
tg_exp.Draw("l")

gr_tb_01.SetLineColor(kRed)   
gr_tb_03.SetLineColor(kBlue)   
gr_tb_05.SetLineColor(kMagenta)   
gr_tb_1 .SetLineColor(kBlue)   
gr_tb_5 .SetLineColor(kRed)   
gr_tb_10.SetLineColor(kMagenta)   
gr_tb_50.SetLineColor(kRed)   

gr_tb_01.SetLineWidth(2)     
gr_tb_03.SetLineWidth(2)      
gr_tb_05.SetLineWidth(2)       
gr_tb_1 .SetLineWidth(2)      
gr_tb_5 .SetLineWidth(2)      
gr_tb_10.SetLineWidth(2)      
gr_tb_50.SetLineWidth(2)     

if(signal=="TTH" or ( signal=="BBH" and model_type=="1") ):
    gr_tb_01.Draw("l")           
    gr_tb_03.Draw("l")            
    gr_tb_05.Draw("l")             

if(signal=="TTHBB" and model_type=="1"):
    gr_tb_01.Draw("l")           
    gr_tb_05.Draw("l")             
    gr_tb_1.Draw("l")             

if( signal=="BBH" and model_type=="2" or (signal=="TTHBB" and model_type=="2") ):
    gr_tb_1.Draw("l")           
    gr_tb_5.Draw("l")            
    gr_tb_10.Draw("l")             

if(signal=="TBH"): 
    gr_tb_01 .Draw("l")             
    gr_tb_05 .Draw("l")            
    gr_tb_1  .Draw("l")           

if data:
    tg_obs.SetLineColor(kBlack)
    tg_obs.SetLineWidth(3)
    tg_obs.SetLineStyle(1)
    tg_obs.SetMarkerStyle(kFullCircle)
    tg_obs.Draw("lp")

#Legend
#if signal.find("UEDRPP")==-1: leg.AddEntry(tg_theory,"Theory (NNLO prediction #pm1#sigma)","lf")
#else: leg.AddEntry(tg_theory,"Theory","l")

leg_2.SetNColumns(3)

if data:
    leg.AddEntry(tg_obs,"95% CL observed limit","lp")
leg.AddEntry(tg_exp,"95% CL expected limit","l")
leg.AddEntry(tg_exp1s,"95% CL expected limit #pm1#sigma","f")
leg.AddEntry(tg_exp2s,"95% CL expected limit #pm2#sigma","f")

neme_leg_gr_tb_01 ="tan#beta=0.1"
neme_leg_gr_tb_03 ="tan#beta=0.3"
neme_leg_gr_tb_05 ="tan#beta=0.5"
neme_leg_gr_tb_1  ="tan#beta=1"
neme_leg_gr_tb_5  ="tan#beta=5"
neme_leg_gr_tb_10 ="tan#beta=10"
neme_leg_gr_tb_50 ="tan#beta=50"
 
if(signal=="TTH" or ( signal=="BBH" and model_type=="1")): 
    leg_2.AddEntry(gr_tb_01 , neme_leg_gr_tb_01 ,"l")            
    leg_2.AddEntry(gr_tb_03 , neme_leg_gr_tb_03 ,"l")             
    leg_2.AddEntry(gr_tb_05 , neme_leg_gr_tb_05 ,"l")                

if(signal=="TTHBB" and model_type=="1"): 
    leg_2.AddEntry(gr_tb_01 , neme_leg_gr_tb_01 ,"l")            
    #leg_2.AddEntry(gr_tb_03 , neme_leg_gr_tb_03 ,"l")             
    leg_2.AddEntry(gr_tb_05 , neme_leg_gr_tb_05 ,"l")                
    leg_2.AddEntry(gr_tb_1 , neme_leg_gr_tb_1 ,"l")                

if(signal=="BBH" and model_type=="2" or (signal=="TTHBB" and model_type=="2") ): 
    leg_2.AddEntry(gr_tb_1 , neme_leg_gr_tb_1  ,"l")            
    leg_2.AddEntry(gr_tb_5 , neme_leg_gr_tb_5  ,"l")             
    leg_2.AddEntry(gr_tb_10, neme_leg_gr_tb_10 ,"l")                

if(signal=="TBH"): 
    leg_2.AddEntry(gr_tb_01 , neme_leg_gr_tb_01 ,"l")                
    leg_2.AddEntry(gr_tb_05 , neme_leg_gr_tb_05 ,"l")                
    leg_2.AddEntry(gr_tb_1  , neme_leg_gr_tb_1  ,"l")             


leg.SetTextSize(0.035)
leg.Draw()
leg_2.SetTextSize(0.035)
leg_2.Draw()

if model_type=="1":
    tl_model = TLatex(0.48,0.77,"type-I 2HDM, cos(#beta-#alpha)=0")
if model_type=="2":
    tl_model = TLatex(0.48,0.77,"type-II 2HDM, cos(#beta-#alpha)=0")

if(signal=="TTH"):
    tl_model = TLatex(0.48,0.77,"Type-I or Type-II 2HDM, cos(#beta-#alpha)=0")

tl_model.SetNDC()                                   
tl_model.SetTextFont(42)                            
tl_model.SetTextSize(0.035)
tl_model.Draw()                                     

#Intersections
#intersx=FitFunctionAndDefineIntersection(tg_theory,tg_exp,isData=False )
#print "Expected limit: " + `intersx[0]`
#if(data):
#    intersxData=FitFunctionAndDefineIntersection(tg_theory,tg_obs,isData=True )
#    print "Observed limit: " + `intersxData[0]`

can.SetBottomMargin(0.15)
can.SetLeftMargin(0.15)
can.SetRightMargin(0.05)
#can.SetTopMargin(0.25)
can.SetTopMargin(0.02)
tl_atlas = TLatex(0.18,0.927,"ATLAS")
if signal.find("UEDRPP")>-1: tl_atlas = TLatex(0.5,0.57,"ATLAS")
tl_atlas.SetNDC()
tl_atlas.SetTextFont(72)
tl_atlas.SetTextSize(tl_atlas.GetTextSize()*0.85)
tl_atlas.Draw()
tl_int = TLatex(0.32,0.925,"Internal")
if signal.find("UEDRPP")>-1: tl_int = TLatex(0.68,0.57,"Internal")
tl_int.SetNDC()
tl_int.SetTextFont(42)
tl_int.SetTextSize(tl_int.GetTextSize()*0.85)
tl_int.Draw()
tl_energy = TLatex(0.17,0.889,"#sqrt{s} = "+energy+" TeV, "+lumi+" fb^{-1}")
if signal.find("UEDRPP")>-1: tl_energy = TLatex(0.5,0.50,"#sqrt{s} = "+energy+" TeV, "+lumi+" fb^{-1}")
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
signal_legend = ""
if(signal=="TTD"):
    signal_legend = "SU(2) doublet"
elif(signal=="TTS"):
    signal_legend = "SU(2) singlet"
elif(signal=="TTHtHt"):
    signal_legend = "BR(T#rightarrowHt)=1"
elif(signal=="UEDRPP11"):
    signal_legend = "Tier (1,1)"
#elif(signal=="TBH")
#    signal_legend = "BR(T#rightarrowHt)=1"
if signal_legend!="":
    tl_sigleg = TLatex(0.6,0.6,signal_legend)
    if signal.find("UEDRPP")>-1: tl_sigleg = TLatex(0.3,0.2,signal_legend)
    tl_sigleg.SetNDC()
    tl_sigleg.SetTextFont(42)
    tl_sigleg.SetTextSize(tl_sigleg.GetTextSize()*0.85)
    tl_sigleg.Draw()

gPad.RedrawAxis()
can.SetTickx()
can.SetTicky()
can.SetLogy()

can.Print(outDir + "/" + signal.upper()+"_"+energy+"_"+lumi.replace(".","")+"_"+table_name+"_model_"+model_type+".eps")
can.Print(outDir + "/" + signal.upper()+"_"+energy+"_"+lumi.replace(".","")+"_"+table_name+"_model_"+model_type+".png")
#can.Print(outDir + "/" + signal.upper()+"_"+energy+"_"+lumi.replace(".","")+"_"+table_name+"_model_.eps")
#can.Print(outDir + "/" + signal.upper()+"_"+energy+"_"+lumi.replace(".","")+"_"+table_name+"_model_.png")
#can.Print(outDir + "/test.png")
