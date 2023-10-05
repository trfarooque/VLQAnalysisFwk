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
#from optparse import OptionParser
#from argparse import ArgumentParser
import argparse

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )
from VLQ_BR import *
from VLQ_Samples_mc import *

from BatchTools import *
from Job import *

gROOT.SetBatch(1)


def main(args):

    ##______________________________________________________________________________
    ## Defines some useful variables
    platform = socket.gethostname()
    now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
    here = os.getcwd()
    sleep=1
    ##..............................................................................


    ##____________________________________________________
    ## Options
    parser = argparse.ArgumentParser()
    parser.add_argument("-i","--inputDir",dest="inputDir",
                      help="repository for the splitted files are located",action="store",default="")
    parser.add_argument("-o","--outputDir",dest="outputDir",
                      help="repository where to put the hadded files",action="store",default="")
    parser.add_argument("-s","--statOnly",dest="statOnly",
                      help="repository where to put the hadded files",action="store_true",default=False)
    parser.add_argument("-B","--doBatch",dest="doBatch",
                      help="launch this job on batch",action="store_true",default=False)
    parser.add_argument("-l","--doLepton",dest="doLepton",
                      help="consider 1L regions",action="store_true",default=False)
    parser.add_argument("-z","--doZeroLepton",dest="doZeroLepton",
                      help="consider 0L regions",action="store_true",default=False)
    parser.add_argument("-a","--doAllBR",dest="doAllBR",
                      help="do all the BR by 0.1 steps",action="store_true",default=False)
    parser.add_argument("-q","--queue",dest="queue",
                      help="batch queue",action="store",default="at3_short")
    parser.add_argument("-t","--tthfitter",dest="tthfitter",
                      help="for TRexFitter",action="store_true",default=False)
    parser.add_argument("-n","--nMerge",type=int,dest="nMerge",
                      help="Merging the operations of the script",default=1)
    parser.add_argument("-d","--debug",dest="debug",
                      help="Debug mode (no job submission)",action="store_true",default=False)
    parser.add_argument("-r","--allRegions",dest="allRegions",
                      help="Use all regions",action="store_true",default=False)
    parser.add_argument("-m","--mcCampaign",dest="mcCampaign",
                      help="MC campaign",action="store",default="mc16a")
    parser.add_argument("-S","--doSR", dest="doSR", 
                      help="Use fit regions", action="store_true", default=False)
    parser.add_argument("-V","--doVR", dest="doVR", 
                      help="Use validation regions", action="store_true", default=False)
    parser.add_argument("-P","--doPR", dest="doPR", 
                      help="Use preselection regions", action="store_true", default=False)
    parser.add_argument("-M","--moduleKeys", dest="moduleKeys", 
                      help="Comma separated list of keys of region dictionary modules", action="store", default="MVA")
    parser.add_argument("--fileSuffix",dest="fileSuffix",
                      help="Suffix to add at the end of the input and output files",action="store",default="")
    parser.add_argument("--postMerging",dest="postMerging",help="Uses post-merging naming convention for input files when this set",action="store",default=0)

    print(args)
    args = parser.parse_args(args)

    outputDir=args.outputDir
    inputDir=args.inputDir
    statOnly=args.statOnly
    doBatch=args.doBatch
    doAllBR=args.doAllBR
    queue=args.queue
    tthfitter=args.tthfitter ###TODO: add option mvainputs
    doLepton=args.doLepton
    doZeroLepton=args.doZeroLepton
    nMerge=args.nMerge
    debug=args.debug
    allRegions=args.allRegions
    mcCampaign=args.mcCampaign
    doSR=args.doSR
    doVR=args.doVR
    doPR=args.doPR
    moduleKeys = args.moduleKeys.split(",")
    fileSuffix = args.fileSuffix
    postMerging = args.postMerging

    os.system("mkdir -p " + outputDir)
    os.system("mkdir -p " + outputDir + "/Scripts")
    ##........................................................

    ##________________________________________________________
    ## Getting list of region modules
    module_list = []

    if "MVA" in moduleKeys:
        import regions_dictionary_pVLQ_newAna_MVA_regions as pVLQ_newAna_MVA_regions
        module_list += [pVLQ_newAna_MVA_regions]
    if "BOT" in moduleKeys:
        import regions_dictionary_pVLQ_newAna_boosted_object_cut_regions as pVLQ_newAna_BOT_regions
        module_list += [pVLQ_newAna_BOT_regions]
    if "OLD" in moduleKeys:
        import regions_dictionary_pVLQ as pVLQ_oldAna_BOT_regions
        module_list += [pVLQ_oldAna_BOT_regions]
    ##.........................................................

    ##________________________________________________________
    ## Getting all signal samples and their associated weight/object systematics

    VLQMass=[600]#,800,1000,1100,1200,1300,1400,1500,1600,1700,1800,2000]

    if doAllBR:#just some mass points (not sensitive otherwise)
        VLQMass=[600,800,1000,1100,1200,1300,1400,1500,1600,1700,1800,2000]
    ##........................................................

    ##________________________________________________________
    ## Defining the list of variables to look at


    Variables = []
    if not tthfitter:
        if(doZeroLepton):
            Variables +=  ['MVAScore', 'meff', 'mtbmin', 'met', 
                           'jets_n', 'bjets_n', 'RCMTT_jets_n', 
                           'RCMHiggs_jets_n', 'RCMTop_jets_n', 'RCMV_jets_n',
                           'mvlq0_RCTTM_drmax', 'mvlq0_RCTTM_drmax', 'mvlq1_rcj_drmax', 'mvlq1_rcj_drmax', 
                           'RCjet0_pt', 'RCjet1_pt', 'RCjet2_pt', 'RCMTop0_pt', 'RCMHiggs0_pt', 'RCMV0_pt',
                           'RCMV0_nconsts', 'RCMV0_nbconsts', 'RCMHiggs0_nconsts', 'RCMHiggs0_nbconsts',
                           'dRmin_RCjets', 'dRmin_RCMTT', 'dPhiavg_RCjets', 'dEtaavg_RCjets',  
                           'dEtamin_RCjets', 'dEtamin_RCMTT', 
                           'leadingdR_RCjets', 'leadingdPhi_RCjets', 'dPhiavg_RCMTTMET']

        if(doLepton):
            Variables +=  ['MVAScore', 'meff', 'mtbmin', 
                           'met', 'ptw', 'residualMET', 
                           'RCMHiggs_jets_n', 'RCMTop_jets_n', 'RCMV_jets_n', 
                           'RCjets_n', 'jets_n', 'bjets_n',
                           'RCjet0_pt', 'leptop_pt', 'dPhimin_RCMTT',
                           'leadingdEta_RCMTT', 'leadingdEta_RCjets', 'leadingdPhi_RCjets', 
                           'mvlq0_RCTTM_drmax', 'mvlq0_RCTTM_drmax', 'mvlq1_rcj_drmax', 'mvlq1_rcj_drmax', 
                           'RCMV0_m', 'RCMV0_pt', 'RCMV0_eta', 
                           'RCMHiggs0_m', 'RCMHiggs0_pt', 'RCMHiggs0_eta', 
                           'RCMTop0_m', 'RCMTop0_pt', 'RCMTop0_eta']
        
    else: #the TRExFitter inputs
        Variables += ["meff", "MVAScore"]
    ##........................................................

    ##________________________________________________________
    ## Defining the list of regions to look at
    Regions = []

    if allRegions:
        Regions = [{'name':"all"}]
    else:
        if( doLepton ):
            for module in module_list:
                if(doSR):
                    Regions += module.fit_regions_1l
                if(doVR):
                    Regions += module.validation_regions_1l
                if(doPR):
                    Regions += module.preselection_regions_1l
        if( doZeroLepton ):
            for module in module_list:
                if(doSR):
                    Regions += module.fit_regions_0l
                if(doVR):
                    Regions += module.validation_regions_0l
                if(doPR):
                    Regions += module.preselection_regions_0l

    ###........................................................

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
                        BRs += ["BR_%.2f_%.2f_%.2f,%.2f,%.2f,%.2f" 
                                %(coupling_Wb,coupling_Zt,coupling_Ht,coupling_Wb,coupling_Zt,coupling_Ht)]
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
        #prepareTarBall(here+"/../../",tarballPath)
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
            jO.addOption("fileSuffix", fileSuffix)
            jO.addOption("postMerging", str(postMerging))

            ###THIS IS DUMMY, NOT USED BY VLQCOUPLINGREWEIGHTER
            #outFile = "outVLQAna_VLQ_TT_"+str(sample)+"_"+br.split(",")[0]+"."+mcCampaign+"__nominal___0"
            #outFile += "_"+fileSuffix+".root" if fileSuffix else ".root"
            #jO.addOption("outputFile", outFile)
    
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

if __name__ == '__main__':
    main(sys.argv[1:])
