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
from VLQ_Samples import *
from regions_dictionary import *

gROOT.SetBatch(1)

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
os.system("mkdir -p " + outputDir)
##........................................................

##________________________________________________________
## Getting all signal samples and their associated weight/object systematics
#VLQMass = [600,700,750,800,850,900,950,1000,1050,1100,1150,1200,1300,1400]
VLQMass = [1400]
if doAllBR:#just some mass points (not sensitive otherwise)
    VLQMass = [700,800,900,1000,1100,1200,1300]
##........................................................

##________________________________________________________
## Defining the list of variables to look at
Variables = []
if not tthfitter:

    Variables =  [  "Ht_recoVLQ0_dEta12",
                    "Ht_recoVLQ0_dPhi12",
                    "Ht_recoVLQ0_dR12",
                    "Ht_recoVLQ0_eta",
                    "Ht_recoVLQ0_fpT12",
                    "Ht_recoVLQ0_m",
                    "Ht_recoVLQ0_pt",
                    "Ht_recoVLQ1_dEta12",
                    "Ht_recoVLQ1_dPhi12",
                    "Ht_recoVLQ1_dR12",
                    "Ht_recoVLQ1_eta",
                    "Ht_recoVLQ1_fpT12",
                    "Ht_recoVLQ1_m",
                    "Ht_recoVLQ1_pt",
                    "Ht_recoVLQ_n",
                    "Ht_recoVLQs_dEta12",
                    "Ht_recoVLQs_dPhi12",
                    "Ht_recoVLQs_dR12",
                    "Ht_recoVLQs_eta",
                    "Ht_recoVLQs_fpT12",
                    "Ht_recoVLQs_m",
                    "Ht_recoVLQs_pt",
                    "J_J_invariant_mass",
                    "J_leadingb_invariant_mass",
                    "J_lepton_invariant_mass",
                    "LooseRCTTMass_jet0_bconsts_n",
                    "LooseRCTTMass_jet0_consts_n",
                    "LooseRCTTMass_jet0_dPhi_lep",
                    "LooseRCTTMass_jet0_dPhi_leptop",
                    "LooseRCTTMass_jet0_dPhi_met",
                    "LooseRCTTMass_jet0_dR_lep",
                    "LooseRCTTMass_jet0_dR_leptop",
                    "LooseRCTTMass_jet0_eta",
                    "LooseRCTTMass_jet0_m",
                    "LooseRCTTMass_jet0_pt",
                    "LooseRCTTMass_jets_bconsts_n",
                    "LooseRCTTMass_jets_consts_n",
                    "LooseRCTTMass_jets_dPhi_lep",
                    "LooseRCTTMass_jets_dPhi_leptop",
                    "LooseRCTTMass_jets_dPhi_met",
                    "LooseRCTTMass_jets_dR_lep",
                    "LooseRCTTMass_jets_dR_leptop",
                    "LooseRCTTMass_jets_eta",
                    "LooseRCTTMass_jets_m",
                    "LooseRCTTMass_jets_n",
                    "LooseRCTTMass_jets_pt",
                    "LooseRCTTMass_leptop_b_dRmin",
                    "LooseRCTTMass_leptop_dRmin",
                    "RCMHiggs_jet0_bconsts_n",
                    "RCMHiggs_jet0_dPhi_lep",
                    "RCMHiggs_jet0_dPhi_leptop",
                    "RCMHiggs_jet0_dPhi_met",
                    "RCMHiggs_jet0_dPhimin_hadtop",
                    "RCMHiggs_jet0_dR_lep",
                    "RCMHiggs_jet0_dR_leptop",
                    "RCMHiggs_jet0_dRmin_hadtop",
                    "RCMHiggs_jet0_eta",
                    "RCMHiggs_jet0_m",
                    "RCMHiggs_jet0_minv_hadtop",
                    "RCMHiggs_jet0_minv_leptop",
                    "RCMHiggs_jet0_pt",
                    "RCMHiggs_jets_bconsts_n",
                    "RCMHiggs_jets_dPhi_lep",
                    "RCMHiggs_jets_dPhi_leptop",
                    "RCMHiggs_jets_dPhi_met",
                    "RCMHiggs_jets_dPhimin_hadtop",
                    "RCMHiggs_jets_dR_lep",
                    "RCMHiggs_jets_dR_leptop",
                    "RCMHiggs_jets_dRmin_hadtop",
                    "RCMHiggs_jets_eta",
                    "RCMHiggs_jets_m",
                    "RCMHiggs_jets_minv_hadtop",
                    "RCMHiggs_jets_minv_leptop",
                    "RCMHiggs_jets_n",
                    "RCMHiggs_jets_pt",
                    "RCMHiggs_leptop_b_dRmin",
                    "RCMHiggs_leptop_dRmin",
                    "RCMTopHiggs_jet0_bconsts_n",
                    "RCMTopHiggs_jet0_dPhi_lep",
                    "RCMTopHiggs_jet0_dPhi_leptop",
                    "RCMTopHiggs_jet0_dPhi_met",
                    "RCMTopHiggs_jet0_dR_lep",
                    "RCMTopHiggs_jet0_dR_leptop",
                    "RCMTopHiggs_jet0_eta",
                    "RCMTopHiggs_jet0_m",
                    "RCMTopHiggs_jet0_pt",
                    "RCMTopHiggs_jets_bconsts_n",
                    "RCMTopHiggs_jets_dPhi_lep",
                    "RCMTopHiggs_jets_dPhi_leptop",
                    "RCMTopHiggs_jets_dPhi_met",
                    "RCMTopHiggs_jets_dR_lep",
                    "RCMTopHiggs_jets_dR_leptop",
                    "RCMTopHiggs_jets_eta",
                    "RCMTopHiggs_jets_m",
                    "RCMTopHiggs_jets_n",
                    "RCMTopHiggs_jets_pt",
                    "RCMTopHiggs_leptop_b_dRmin",
                    "RCMTopHiggs_leptop_dRmin",
                    "RCMTop_jet0_bconsts_n",
                    "RCMTop_jet0_dPhi_lep",
                    "RCMTop_jet0_dPhi_leptop",
                    "RCMTop_jet0_dPhi_met",
                    "RCMTop_jet0_dR_lep",
                    "RCMTop_jet0_dR_leptop",
                    "RCMTop_jet0_eta",
                    "RCMTop_jet0_m",
                    "RCMTop_jet0_pt",
                    "RCMTop_jets_bconsts_n",
                    "RCMTop_jets_dPhi_lep",
                    "RCMTop_jets_dPhi_leptop",
                    "RCMTop_jets_dPhi_met",
                    "RCMTop_jets_dR_lep",
                    "RCMTop_jets_dR_leptop",
                    "RCMTop_jets_eta",
                    "RCMTop_jets_m",
                    "RCMTop_jets_n",
                    "RCMTop_jets_pt",
                    "RCMTop_leptop_b_dRmin",
                    "RCMTop_leptop_dRmin",
                    "RCMV_jet0_bconsts_n",
                    "RCMV_jet0_dPhi_lep",
                    "RCMV_jet0_dPhi_leptop",
                    "RCMV_jet0_dPhi_met",
                    "RCMV_jet0_dPhimin_hadtop",
                    "RCMV_jet0_dR_lep",
                    "RCMV_jet0_dR_leptop",
                    "RCMV_jet0_dRmin_hadtop",
                    "RCMV_jet0_eta",
                    "RCMV_jet0_m",
                    "RCMV_jet0_minv_hadtop",
                    "RCMV_jet0_minv_leptop",
                    "RCMV_jet0_pt",
                    "RCMV_jets_bconsts_n",
                    "RCMV_jets_dPhi_lep",
                    "RCMV_jets_dPhi_leptop",
                    "RCMV_jets_dPhi_met",
                    "RCMV_jets_dPhimin_hadtop",
                    "RCMV_jets_dR_lep",
                    "RCMV_jets_dR_leptop",
                    "RCMV_jets_dRmin_hadtop",
                    "RCMV_jets_eta",
                    "RCMV_jets_m",
                    "RCMV_jets_minv_hadtop",
                    "RCMV_jets_minv_leptop",
                    "RCMV_jets_n",
                    "RCMV_jets_pt",
                    "RCMV_leptop_b_dRmin",
                    "RCMV_leptop_dRmin",
                    "RCTTMass_jet0_bconsts_n",
                    "RCTTMass_jet0_consts_n",
                    "RCTTMass_jet0_dPhi_lep",
                    "RCTTMass_jet0_dPhi_leptop",
                    "RCTTMass_jet0_dPhi_met",
                    "RCTTMass_jet0_dR_lep",
                    "RCTTMass_jet0_dR_leptop",
                    "RCTTMass_jet0_eta",
                    "RCTTMass_jet0_m",
                    "RCTTMass_jet0_pt",
                    "RCTTMass_jets_bconsts_n",
                    "RCTTMass_jets_consts_n",
                    "RCTTMass_jets_dPhi_lep",
                    "RCTTMass_jets_dPhi_leptop",
                    "RCTTMass_jets_dPhi_met",
                    "RCTTMass_jets_dR_lep",
                    "RCTTMass_jets_dR_leptop",
                    "RCTTMass_jets_eta",
                    "RCTTMass_jets_m",
                    "RCTTMass_jets_n",
                    "RCTTMass_jets_pt",
                    "RCTTMass_leptop_b_dRmin",
                    "RCTTMass_leptop_dRmin",
                    "RCjet0_eta",
                    "RCjet0_isRCTTMass",
                    "RCjet0_m",
                    "RCjet0_nbconsts",
                    "RCjet0_nconsts",
                    "RCjet0_pt",
                    "RCjets_eta",
                    "RCjets_isRCTTMass",
                    "RCjets_m",
                    "RCjets_n",
                    "RCjets_nbconsts",
                    "RCjets_nconsts",
                    "RCjets_pt",
                    "Wb_recoVLQ0_dEta12",
                    "Wb_recoVLQ0_dPhi12",
                    "Wb_recoVLQ0_dR12",
                    "Wb_recoVLQ0_eta",
                    "Wb_recoVLQ0_fpT12",
                    "Wb_recoVLQ0_m",
                    "Wb_recoVLQ0_pt",
                    "Wb_recoVLQ1_dEta12",
                    "Wb_recoVLQ1_dPhi12",
                    "Wb_recoVLQ1_dR12",
                    "Wb_recoVLQ1_eta",
                    "Wb_recoVLQ1_fpT12",
                    "Wb_recoVLQ1_m",
                    "Wb_recoVLQ1_pt",
                    "Wb_recoVLQ_n",
                    "Wb_recoVLQs_dEta12",
                    "Wb_recoVLQs_dPhi12",
                    "Wb_recoVLQs_dR12",
                    "Wb_recoVLQs_eta",
                    "Wb_recoVLQs_fpT12",
                    "Wb_recoVLQs_m",
                    "Wb_recoVLQs_pt",
                    "Zt_recoVLQ0_dEta12",
                    "Zt_recoVLQ0_dPhi12",
                    "Zt_recoVLQ0_dR12",
                    "Zt_recoVLQ0_eta",
                    "Zt_recoVLQ0_fpT12",
                    "Zt_recoVLQ0_m",
                    "Zt_recoVLQ0_pt",
                    "Zt_recoVLQ1_dEta12",
                    "Zt_recoVLQ1_dPhi12",
                    "Zt_recoVLQ1_dR12",
                    "Zt_recoVLQ1_eta",
                    "Zt_recoVLQ1_fpT12",
                    "Zt_recoVLQ1_m",
                    "Zt_recoVLQ1_pt",
                    "Zt_recoVLQ_n",
                    "Zt_recoVLQs_dEta12",
                    "Zt_recoVLQs_dPhi12",
                    "Zt_recoVLQs_dR12",
                    "Zt_recoVLQs_eta",
                    "Zt_recoVLQs_fpT12",
                    "Zt_recoVLQs_m",
                    "Zt_recoVLQs_pt",
                    "bW_hadtop0_dR_bW",
                    "bW_hadtop0_eta",
                    "bW_hadtop0_m",
                    "bW_hadtop0_pt",
                    "bW_hadtop1_dR_bW",
                    "bW_hadtop1_eta",
                    "bW_hadtop1_m",
                    "bW_hadtop1_pt",
                    "bW_hadtop2_dR_bW",
                    "bW_hadtop2_eta",
                    "bW_hadtop2_m",
                    "bW_hadtop2_pt",
                    "bW_hadtops_dR_bW",
                    "bW_hadtops_eta",
                    "bW_hadtops_m",
                    "bW_hadtops_pt",
                    "bjet0_btagw",
                    "bjet0_eta",
                    "bjet0_jvt",
                    "bjet0_pt",
                    "bjet1_btagw",
                    "bjet1_eta",
                    "bjet1_jvt",
                    "bjet1_pt",
                    "bjet2_btagw",
                    "bjet2_eta",
                    "bjet2_jvt",
                    "bjet2_pt",
                    "bjet3_btagw",
                    "bjet3_eta",
                    "bjet3_jvt",
                    "bjet3_pt",
                    "bjets_btagw",
                    "bjets_eta",
                    "bjets_jvt",
                    "bjets_n",
                    "bjets_pt",
                    "centrality",
                    "dPhi_jetmet",
                    "dPhi_jetmet5",
                    "dPhi_jetmet6",
                    "dPhi_jetmet7",
                    "dPhi_lepbjet",
                    "dPhi_lepjet",
                    "dPhi_lepmet",
                    "dR_bjetbjet",
                    "dR_ebjet",
                    "dR_ejet",
                    "dR_jetjet",
                    "dR_mubjet",
                    "dR_mujet",
                    "dRaverage_bjetbjet",
                    "dRaverage_jetjet",
                    "dRmin_TTL_bjets",
                    "dRmin_TTLooser_bjets",
                    "dRmin_TTT_bjets",
                    "fwdjet0_eta",
                    "fwdjet0_m",
                    "fwdjet0_phi",
                    "fwdjet0_pt",
                    "fwdjet1_eta",
                    "fwdjet1_m",
                    "fwdjet1_phi",
                    "fwdjet1_pt",
                    "fwdjet2_eta",
                    "fwdjet2_m",
                    "fwdjet2_phi",
                    "fwdjet2_pt",
                    "fwdjet3_eta",
                    "fwdjet3_m",
                    "fwdjet3_phi",
                    "fwdjet3_pt",
                    "fwdjet4_eta",
                    "fwdjet4_m",
                    "fwdjet4_phi",
                    "fwdjet4_pt",
                    "fwdjet5_eta",
                    "fwdjet5_m",
                    "fwdjet5_phi",
                    "fwdjet5_pt",
                    "fwdjets_eta",
                    "fwdjets_m",
                    "fwdjets_n",
                    "fwdjets_phi",
                    "fwdjets_pt",
                    "hthad",
                    "hthadRC",
                    "hthadRCM",
                    "hthadRCtag",
                    "jet0_btagw",
                    "jet0_eta",
                    "jet0_jvt",
                    "jet0_m",
                    "jet0_phi",
                    "jet0_pt",
                    "jet1_btagw",
                    "jet1_eta",
                    "jet1_jvt",
                    "jet1_m",
                    "jet1_phi",
                    "jet1_pt",
                    "jet2_btagw",
                    "jet2_eta",
                    "jet2_jvt",
                    "jet2_m",
                    "jet2_phi",
                    "jet2_pt",
                    "jet3_btagw",
                    "jet3_eta",
                    "jet3_jvt",
                    "jet3_m",
                    "jet3_phi",
                    "jet3_pt",
                    "jet4_btagw",
                    "jet4_eta",
                    "jet4_jvt",
                    "jet4_m",
                    "jet4_phi",
                    "jet4_pt",
                    "jet5_btagw",
                    "jet5_eta",
                    "jet5_jvt",
                    "jet5_m",
                    "jet5_phi",
                    "jet5_pt",
                    "jets40_n",
                    "jets_btagw",
                    "jets_eta",
                    "jets_jvt",
                    "jets_m",
                    "jets_n",
                    "jets_phi",
                    "jets_pt",
                    "lep0_d0sig",
                    "lep0_eta",
                    "lep0_phi",
                    "lep0_pt",
                    "lep0_pt_zoom",
                    "lep0_z0",
                    "lepW_eta",
                    "lepW_m",
                    "lepW_pt",
                    "lep_n",
                    "leps_d0sig",
                    "leps_eta",
                    "leps_phi",
                    "leps_pt",
                    "leps_pt_zoom",
                    "leps_z0",
                    "leptop_dPhi_bnu",
                    "leptop_dR_bW",
                    "leptop_dR_blep",
                    "leptop_eta",
                    "leptop_m",
                    "leptop_pt",
                    "ljet0_btagw",
                    "ljet0_eta",
                    "ljet0_jvt",
                    "ljet0_pt",
                    "ljet1_btagw",
                    "ljet1_eta",
                    "ljet1_jvt",
                    "ljet1_pt",
                    "ljet2_btagw",
                    "ljet2_eta",
                    "ljet2_jvt",
                    "ljet2_pt",
                    "ljet3_btagw",
                    "ljet3_eta",
                    "ljet3_jvt",
                    "ljet3_pt",
                    "ljet4_btagw",
                    "ljet4_eta",
                    "ljet4_jvt",
                    "ljet4_pt",
                    "ljets_btagw",
                    "ljets_eta",
                    "ljets_jvt",
                    "ljets_pt",
                    "mJsum",
                    "mbb_leading_bjets",
                    "mbb_mindR",
                    "mbb_softest_bjets",
                    "meff",
                    "met",
                    "met_phi",
                    "met_sig",
                    "mtbmin",
                    "mtw",
                    "mu",
                    "nu_dphi_MET",
                    "nu_eta",
                    "nu_fracMET",
                    "nu_m",
                    "nu_pt",
                    "ptw"]


    # number of objects
    #Variables += ["jets_n"]
    #Variables += ["bjets_n"]
    #Variables += ["RCMTop_jets_n"]
    #Variables += ["RCMV_jets_n"]
    #Variables += ["RCMHiggs_jets_n"]
    #Variables += ["RCjets_n"]
    #kinematic variables
    #Variables += ["meff"]
    ##Variables += ["mtbmin_vs_meff"]
    #Variables += ["mtbmin_zoom"]
    #Variables += ["mtbmin"]
    #Variables += ["met"]
    ##Variables += ["met_zoom"]
    #Variables += ["hthad"]
    #Variables += ["mtw_zoom"]
    #jet variables
    #Variables += ["jet0_pt"]
    #Variables += ["jet0_eta"]
    #Variables += ["jets_pt"]
    #Variables += ["jets_eta"]
    #Variables += ["RCjets_pt"]
    #Variables += ["RCjets_m"]
    #Variables += ["RCjet0_pt"]
    #Variables += ["RCjet0_m"]
    #mus
    #Variables += ["mu0_pt"]
    #Variables += ["mu0_eta"]
    #electrons
    #Variables += ["el0_pt"]
    #Variables += ["el0_eta"]
    #Variables += ["lep0_pt"]
    #Variables += ["lep0_pt_zoom"]
    #Variables += ["lep0_eta"]
else: #the TRExFitter inputs
    Variables += ["meff"]
##........................................................

##________________________________________________________
## Defining the list of regions to look at
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

##________________________________________________________
## Now writes scripts ... and submits jobs !
merging = 0
scriptName = ""
for sample in VLQMass:
    for br in BRs:

        printGoodNews("=> VLQ_TT_"+`sample`+"_"+br.split(",")[0])
        openOption = "w"
        if merging==0:
            scriptName = outputDir + "/script_reweighting_" + `sample` + "_br_" + br
        else:
            openOption = "a"

        script = open(scriptName,openOption)

        if openOption=="w":
            script.write("export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase \n")
            script.write("source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh \n")
            script.write("source " + os.getenv("VLQAnalysisFramework_DIR") + "/../rcSetup.sh \n")
            script.write("cd $TMPDIR \n")
            script.write("\n")
            script.write("cp " + os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/VLQCouplingReweighter.py VLQCouplingReweighter.py \n")
            script.write("cp " + os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/VLQ_BR.py VLQ_BR.py \n")
            script.write("cp " + os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/VLQ_Samples.py VLQ_Samples.py \n")

        script.write("mkdir -p inputs/ \n")
        script.write("cp "+inputDir+"/out*VLQ_TT_%.0f_30*.root inputs/ \n" %(sample))

        com = "python VLQCouplingReweighter.py -i inputs/ -o ./"
        if statOnly:
            com += " -s "
        com += " -m %.0f " %(sample)
        com += " -c " + br
        com += " -r "
        for reg in Regions:
            com += (reg['name']).replace("HTX_","")
            if reg != Regions[len(Regions)-1]:
                com += ","
        com += " -v "
        for var in Variables:
            com += var
            if var != Variables[len(Variables)-1]:
                com += ","
        script.write(com + " >& logfile_reweighting_" + `sample` + "_br_" + br +" \n")
        script.write("mv *.root "+outputDir+"/ \n")
        script.write("rm -rf inputs/ \n ")
        script.write("\n")
        script.write("\n")

        if openOption=="a" and merging==(nMerge-1):
            script.write("mv logfile_* "+outputDir+"\n")
            script.write("rm -rf $TMPDIR \n")
        else:
            script.write("mv logfile_* "+outputDir+"\n")
            script.write("rm -rf $TMPDIR \n")

        script.close()

        if merging==(nMerge-1):
            submission = "qsub -q " + queue + " " + scriptName + " -o " + outputDir + " -e " + outputDir
            if not debug:
                os.system(submission)
            else:
                print " ==> Should execute command: " + submission
            merging = 0
        else:
            merging += 1
##........................................................
