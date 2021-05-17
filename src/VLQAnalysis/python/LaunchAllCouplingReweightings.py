#!/bin/python
import os
import string
import random
import re
import time, getpass
import socket
import sys
import datetime
from glob import glob
from ROOT import *
from optparse import OptionParser

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "python/VLQAnalysis/" )
from VLQ_BR import *
from VLQ_Samples_mc import *

#from regions_dictionary_pVLQ import *
#from regions_dictionary_pVLQ_oldAna import *
from regions_dictionary_pVLQ_newAna_merged_regions import *

#from regions_dictionary import *

from BatchTools import *
from Job import *

gROOT.SetBatch(1)

##______________________________________________________________________________
## Defines some useful variables
platform = socket.gethostname()
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()
sleep=1
##..............................................................................


##____________________________________________________
## Options
parser = OptionParser()
parser.add_option("-i","--inputDir",dest="inputDir",help="repository for the splitted files are located",action="store",default="")
parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the hadded files",action="store",default="")
parser.add_option("-s","--statOnly",dest="statOnly",help="repository where to put the hadded files",action="store_true",default=False)
parser.add_option("-B","--doBatch",dest="doBatch",help="launch this job on batch",action="store_true",default=False)
parser.add_option("-l","--doLepton",dest="doLepton",help="consider 1L regions",action="store_true",default=False)
parser.add_option("-z","--doZeroLepton",dest="doZeroLepton",help="consider 0L regions",action="store_true",default=False)
parser.add_option("-a","--doAllBR",dest="doAllBR",help="do all the BR by 0.1 steps",action="store_true",default=False)
parser.add_option("-q","--queue",dest="queue",help="batch queue",action="store",default="at3_short")
parser.add_option("-t","--tthfitter",dest="tthfitter",help="for TRexFitter",action="store_true",default=False)
parser.add_option("-n","--nMerge",type=int,dest="nMerge",help="Merging the operations of the script",default=1)
parser.add_option("-d","--debug",dest="debug",help="Debug mode (no job submission)",action="store_true",default=False)
parser.add_option("-r","--allRegions",dest="allRegions",help="Use all regions",action="store_true",default=False)
parser.add_option("-m","--mcCampaign",dest="mcCampaign",help="MC campaign",action="store",default="mc16a")
(options, args) = parser.parse_args()

outputDir=options.outputDir
inputDir=options.inputDir
statOnly=options.statOnly
doBatch=options.doBatch
doAllBR=options.doAllBR
queue=options.queue
tthfitter=options.tthfitter
doLepton=options.doLepton
doZeroLepton=options.doZeroLepton
nMerge=options.nMerge
debug=options.debug
allRegions=options.allRegions
mcCampaign=options.mcCampaign
os.system("mkdir -p " + outputDir)
os.system("mkdir -p " + outputDir + "/Scripts")
##........................................................

##________________________________________________________
## Getting all signal samples and their associated weight/object systematics

VLQMass=[600,800,1000,1100,1200,1300,1400,1500,1600,1700,1800,2000]
VLQMass=[1200]
#VLQMass = [600,700,750,800,850,900,950,1000,1050,1100,1150,1200,1300,1400]

if doAllBR:#just some mass points (not sensitive otherwise)
    VLQMass=[600,800,1000,1100,1200,1300,1400,1500,1600,1700,1800,2000]
    #VLQMass = [700,800,900,1000,1100,1200,1300]
##........................................................

##________________________________________________________
## Defining the list of variables to look at
Variables = []
if not tthfitter:

    Variables +=  ["meff", "met", "jets_n", "trkbjets_n", "RCMHiggs_jets_n", "RCMTop_jets_n", "RCMV_jets_n"]

else: #the TRExFitter inputs
    Variables += ["meff"]    
##........................................................

##________________________________________________________
## Defining the list of regions to look at
Regions = []
if allRegions:
    Regions = [{'name':"all"}]
else:
    if( doLepton ):
        #
        # Control/signal regions
        #
        Regions += all_regions_1l
    if( doZeroLepton ):
        #
        # Validation regions
        #
        Regions += all_regions_0l
##........................................................

##________________________________________________________
## BR definitions
BRs = []
BRs += ["TSinglet"]
BRs += ["TDoublet"]
BRs += ["HtHt,0.00,0.00,1.00"]
BRs += ["ZtZt,0.00,1.00,0.00"]
if doAllBR:
    #
    # For the BR scan
    #
    step_width = 0.05 #step in BR plane
    steps_n = int((1 + step_width)/step_width)
    th2 = TH2F("h2_BR_plane","h2_Zh_BR_plane",steps_n,0,1 + step_width,steps_n,0,1 + step_width)
    for iHt in range(0,steps_n):
        for iWb in range(0,steps_n):
            for iZt in range(0,steps_n):
                coupling_Ht = iHt * step_width
                coupling_Zt = iZt * step_width
                coupling_Wb = iWb * step_width
                if abs(coupling_Ht+coupling_Wb+coupling_Zt-1.)<0.001:
                    BRs += ["BR_%.2f_%.2f_%.2f,%.2f,%.2f,%.2f" %(coupling_Wb,coupling_Zt,coupling_Ht,coupling_Wb,coupling_Zt,coupling_Ht)]
                    th2.Fill(coupling_Wb,coupling_Ht,coupling_Zt+0.001)

    th2.SetMarkerStyle(20)

    c = TCanvas("c_Zh_BR","c_Zh_BR",600,600)
    th2.SetTitle("")
    th2.GetXaxis().SetTitle("BR(T#rightarrowWb)")
    th2.GetYaxis().SetTitle("BR(T#rightarrowHt)")
    gStyle.SetOptStat(0)
    gStyle.SetPaintTextFormat("1.2f");
    th2.Draw("textcol")
    c.Print("c_Zh_BR.png")
##........................................................

## Creating tarball                                                                                                                                                           
tarballPath = outputDir + "AnaCode_forBatch.tgz"
if not (os.path.exists(tarballPath) or debug):
    prepareTarBall(here+"/../../",tarballPath)
    prepareTarBall(os.getenv("VLQAnalysisFramework_DIR")+"/../../",tarballPath)

##________________________________________________________                                                                                                                                                                        
## Now writes scripts ... and submits jobs !   
JOSet = JobSet(platform)
JOSet.setBatch("condor")
JOSet.setScriptDir(outputDir+"/Scripts")
JOSet.setLogDir(outputDir)
JOSet.setTarBall(tarballPath)
JOSet.setJobRecoveryFile(outputDir+"/Scripts/JobCheck.chk")
JOSet.setQueue(queue)

for sample in VLQMass:
    for br in BRs:
        printGoodNews("=> VLQ_TT_"+`sample`+"_"+br.split(",")[0])
        
        jO = Job(platform)
        jO.setExecutable("python VLQCouplingReweighter.py")
        jO.setDebug(False)
        jO.setOutDir(outputDir)

        jOName = `sample`+"_br_"+br
        jO.setName(jOName)
        
        jO.addOption("inputDir",   inputDir)
        jO.addOption("outputDir", "./")

        if(statOnly):
            jO.addOption("statOnly", "True")
        
        jO.addOption("mass", "%.0f " %(sample))
        jO.addOption("vlqCoupling", br)
        jO.addOption("mcCampaign", mcCampaign)
        jO.addOption("outputFile", "outVLQAna_VLQ_TT_"+str(sample)+"_"+br.split(",")[0]+"."+mcCampaign+"__nominal___0.root")
    
        regOpt = ""

        for reg in Regions:
            regOpt += (reg['name']).replace("HTX_","")
            if reg != Regions[len(Regions)-1]:
                regOpt += ","
            
        jO.addOption("regions", regOpt)

        varOpt = ""
        
        for var in Variables:
            varOpt += var
            if var != Variables[len(Variables)-1]:
                varOpt += ","

        jO.addOption("variables", varOpt)

        JOSet.addJob(jO)

        if(JOSet.size()==nMerge):
            
            JOSet.writeScript()

            if(not debug):
                JOSet.submitSet()

            JOSet.clear()

if(JOSet.size()>0):
    JOSet.writeScript()

    if(not debug):
        JOSet.submitSet()
    JOSet.clear()

os.system("sleep "+`sleep`)
##........................................................
