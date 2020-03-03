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
parser.add_option("-i","--inputDir",help="location of the outputs",dest="inDir",default="/nfs/at3/scratch2/mcasolino/TRexFitter/TtHFitter/")
parser.add_option("-o","--outFile",help="output file name",dest="outFile",default="results_2d.txt")
parser.add_option("-r","--relaunch",help="relaunch missing jobs",dest="relaunch",action="store_true",default=False)
parser.add_option("-s","--scripts",help="path to find the scripts for the resubmission",dest="scripts",default="")
parser.add_option

(options, args) = parser.parse_args()
inDir   = options.inDir
outFile = options.outFile
relaunchMissingJobs = options.relaunch
scripts = options.scripts

###
# Getting the values of the masses and cross-sections
###
masses = []
masses += [{'name':"VLQ_TT_600_",'mass':600,'xsec':1.16,'err':0.10}]
masses += [{'name':"VLQ_TT_700_",'mass':700,'xsec':0.455}]
masses += [{'name':"VLQ_TT_750_",'mass':750,'xsec':0.295}]
masses += [{'name':"VLQ_TT_800_",'mass':800,'xsec':0.195}]
masses += [{'name':"VLQ_TT_850_",'mass':850,'xsec':0.132}]
masses += [{'name':"VLQ_TT_900_",'mass':900,'xsec':0.0900}]
masses += [{'name':"VLQ_TT_950_",'mass':950,'xsec':0.0624}]
masses += [{'name':"VLQ_TT_1000_",'mass':1000,'xsec':0.0438}]
masses += [{'name':"VLQ_TT_1050_",'mass':1050,'xsec':0.0311,'err':0.0035}]
masses += [{'name':"VLQ_TT_1100_",'mass':1100,'xsec':0.0223,'err':0.0025}]
masses += [{'name':"VLQ_TT_1150_",'mass':1150,'xsec':0.0161,'err':0.0018}]
masses += [{'name':"VLQ_TT_1200_",'mass':1200,'xsec':0.0117,'err':0.0013}]
masses += [{'name':"VLQ_TT_1300_",'mass':1300,'xsec':0.00634,'err':0.00075}]

###
# Branching ratios
###
BRs = []
step_width = 0.05
steps_n = int((1 + step_width)/step_width)

if(outFile.find("/")>-1):
    folder = ""
    splitted = outFile.split("/")
    for part in range(len(splitted)-1):
        folder += splitted[part] + "/"
    os.system("mkdir -p " + folder)
out_limit_results = open(outFile,"w")

for iHt in range(0,steps_n):
    for iWb in range(0,steps_n):
        for iZt in range(0,steps_n):
            coupling_Ht = iHt * step_width
            coupling_Zt = iZt * step_width
            coupling_Wb = iWb * step_width
            if abs(coupling_Ht+coupling_Wb+coupling_Zt-1.)<0.001:
                BRs += [{'name':"BR_%.2f_%.2f_%.2f" %(coupling_Wb,coupling_Zt,coupling_Ht),'Wb':coupling_Wb,'Zt':coupling_Zt,'Ht':coupling_Ht,'legend': "Wb: %.0f%%, Zt: %.0f%%, Ht: %.0f%%"%(coupling_Wb*100.,coupling_Zt*100.,coupling_Ht*100.)}]

for br in BRs:
    for mass in masses:
        files = glob.glob(inDir + "/*"+mass['name']+"*"+br['name']+"*/Limits/*.root")
        if len(files)>1 or (len(files)==0 and not relaunchMissingJobs):
            print "<!> ERROR !! : " + inDir + "/*"+mass['name']+"*"+br['name']+"*/Limits/*.root ("+`len(files)`+")"
        elif len(files)==0 and relaunchMissingJobs :
            print "-> Want to relaunch for configuration :" +mass['name']+"*"+br['name']
            list_scripts = glob.glob(scripts+"/*"+mass['name']+"*"+br['name']+"*.txt")
            if(len(list_scripts)==1):
                print "     -> Found script to submit: " + list_scripts[0]
                os.system("qsub -q at3 "+list_scripts[0])
        else:
            rootfile = TFile(files[0],"read")
            histogram = rootfile.Get("limit")
            out_limit_results.write(mass['name']+br['name']+"\n")
            out_limit_results.write("Observed: "+`histogram.GetBinContent(1)`+"\n")
            out_limit_results.write("Expected: "+`histogram.GetBinContent(2)`+"\n")
            rootfile.Close()
out_limit_results.close()
