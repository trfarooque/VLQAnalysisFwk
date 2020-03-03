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

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "python/IFAETopFramework/" )
from BatchTools import *
from Samples import *
gROOT.SetBatch(1)

##____________________________________________________
## Options
parser = OptionParser()
parser.add_option("-i","--inputDir",dest="inputDir",help="repository for the splitted files are located",action="store",default="")
parser.add_option("-o","--outputDir",dest="outputDir",help="repository where to put the hadded files",action="store",default="")
parser.add_option("-s","--statOnly",dest="statOnly",help="repository where to put the hadded files",action="store_true",default=False)
parser.add_option("-e","--doEfficiency",dest="doEfficiency",help="do VLQ Coupling Reweighting for Efficiency plots",action="store_true",default=False)
parser.add_option("-B","--doBatch",dest="doBatch",help="launch this job on batch",action="store_true",default=False)
parser.add_option("-l","--doLepton",dest="doLepton",help="consider 1L regions",action="store_true",default=True)
parser.add_option("-z","--doZeroLepton",dest="doZeroLepton",help="consider 0L regions",action="store_true",default=True)
parser.add_option("-a","--doAllBR",dest="doAllBR",help="do all the BR by 0.1 steps",action="store_true",default=False)
parser.add_option("-q","--queue",dest="queue",help="batch queue",action="store",default="at3_short")
parser.add_option("-t","--tthfitter",dest="tthfitter",help="for TRexFitter",action="store_true",default=False)
parser.add_option("-n","--nMerge",type=int,dest="nMerge",help="Merging the operations of the script",default=1)
parser.add_option("-d","--debug",dest="debug",help="Debug mode (no job submission)",action="store_true",default=False)
(options, args) = parser.parse_args()

outputDir=options.outputDir
inputDir=options.inputDir
statOnly=options.statOnly
doEfficiency=options.doEfficiency
doBatch=options.doBatch
doAllBR=options.doAllBR
queue=options.queue
tthfitter=options.tthfitter
doLepton=options.doLepton
doZeroLepton=options.doZeroLepton
nMerge=options.nMerge
debug=options.debug
os.system("mkdir -p " + outputDir)
##........................................................

##________________________________________________________
## Getting all signal samples and their associated weight/object systematics
VLQMass = [600,700,750,800,850,900,950,1000,1050,1100,1150,1200,1300,1400]
if doAllBR:#just some mass points (not sensitive otherwise)
    VLQMass = [600,700,750,800,850,900,950,1000]
##........................................................

##________________________________________________________
## Defining the list of variables to look at
Variables = []
if not tthfitter:
    # number of objects
    Variables += ["jets_n"]
    Variables += ["bjets_n"]
    Variables += ["RCTTMass_jets_n"]
    Variables += ["RCjets_n"]
    Variables += ["mtbmin"]

    #kinematic variables
    Variables += ["meff"]
    Variables += ["meff_zoom"]
    Variables += ["met_zoom"]
    Variables += ["met"]
    Variables += ["hthad"]
    Variables += ["hthad_zoom"]
    Variables += ["mtw_zoom"]
    Variables += ["mtw"]

    #jet variables
    Variables += ["jet0_pt"]
    Variables += ["jet0_eta"]

    #mus
    Variables += ["mu0_pt"]
    Variables += ["mu0_eta"]

    #electrons
    Variables += ["el0_pt"]
    Variables += ["el0_eta"]
    Variables += ["lep0_pt"]
    Variables += ["lep0_eta"]
    Variables += ["RCTTMass_jet0_pt"]
    Variables += ["RCTTMass_jet0_eta"]
    Variables += ["RCTTMass_jet0_m"]

    #mbb
    Variables += ["mbb_mindR"]

else: #the ttHFitter inputs
    Variables += ["meff"]

if doEfficiency:#used for top-tagging efficiency studies
    Variables=[]
    Variables+=["den_tag_eff_TTLoose_fjpt"]
    Variables+=["den_tag_eff_TTLoose_meff"]
    Variables+=["den_tag_eff_TTLoose_tpt"]
    Variables+=["den_tag_eff_TTMassCut_fjpt"]
    Variables+=["den_tag_eff_TTMassCut_meff"]
    Variables+=["den_tag_eff_TTMassCut_tpt"]
    Variables+=["num_tag_eff_TTLoose_fjpt"]
    Variables+=["num_tag_eff_TTLoose_meff"]
    Variables+=["num_tag_eff_TTLoose_tpt"]
    Variables+=["num_tag_eff_TTMassCut_fjpt"]
    Variables+=["num_tag_eff_TTMassCut_meff"]
    Variables+=["num_tag_eff_TTMassCut_tpt"]
    Variables+=["den_tag_eff_TTRCJ_fjpt"]
    Variables+=["den_tag_eff_TTRCJ_meff"]
    Variables+=["den_tag_eff_TTRCJ_tpt"]
    Variables+=["num_tag_eff_TTRCJ_fjpt"]
    Variables+=["num_tag_eff_TTRCJ_meff"]
    Variables+=["num_tag_eff_TTRCJ_tpt"]
    Variables+=["den_tag_mistag_TTLoose_fjpt"]
    Variables+=["den_tag_mistag_TTLoose_meff"]
    Variables+=["den_tag_mistag_TTLoose_tpt"]
    Variables+=["den_tag_mistag_TTMassCut_fjpt"]
    Variables+=["den_tag_mistag_TTMassCut_meff"]
    Variables+=["den_tag_mistag_TTMassCut_tpt"]
    Variables+=["num_tag_mistag_TTLoose_fjpt"]
    Variables+=["num_tag_mistag_TTLoose_meff"]
    Variables+=["num_tag_mistag_TTLoose_tpt"]
    Variables+=["num_tag_mistag_TTMassCut_fjpt"]
    Variables+=["num_tag_mistag_TTMassCut_meff"]
    Variables+=["num_tag_mistag_TTMassCut_tpt"]
    Variables+=["den_tag_mistag_TTRCJ_fjpt"]
    Variables+=["den_tag_mistag_TTRCJ_meff"]
    Variables+=["den_tag_mistag_TTRCJ_tpt"]
    Variables+=["num_tag_mistag_TTRCJ_fjpt"]
    Variables+=["num_tag_mistag_TTRCJ_meff"]
    Variables+=["num_tag_mistag_TTRCJ_tpt"]
    Variables+=["den_tag_eff_e_TTLoose_fjpt"]
    Variables+=["den_tag_eff_e_TTLoose_meff"]
    Variables+=["den_tag_eff_e_TTLoose_tpt"]
    Variables+=["den_tag_eff_e_TTMassCut_fjpt"]
    Variables+=["den_tag_eff_e_TTMassCut_meff"]
    Variables+=["den_tag_eff_e_TTMassCut_tpt"]
    Variables+=["num_tag_eff_e_TTLoose_fjpt"]
    Variables+=["num_tag_eff_e_TTLoose_meff"]
    Variables+=["num_tag_eff_e_TTLoose_tpt"]
    Variables+=["num_tag_eff_e_TTMassCut_fjpt"]
    Variables+=["num_tag_eff_e_TTMassCut_meff"]
    Variables+=["num_tag_eff_e_TTMassCut_tpt"]
    Variables+=["den_tag_eff_e_TTRCJ_fjpt"]
    Variables+=["den_tag_eff_e_TTRCJ_meff"]
    Variables+=["den_tag_eff_e_TTRCJ_tpt"]
    Variables+=["num_tag_eff_e_TTRCJ_fjpt"]
    Variables+=["num_tag_eff_e_TTRCJ_meff"]
    Variables+=["num_tag_eff_e_TTRCJ_tpt"]
    Variables+=["den_tag_mistag_e_TTLoose_fjpt"]
    Variables+=["den_tag_mistag_e_TTLoose_meff"]
    Variables+=["den_tag_mistag_e_TTLoose_tpt"]
    Variables+=["den_tag_mistag_e_TTMassCut_fjpt"]
    Variables+=["den_tag_mistag_e_TTMassCut_meff"]
    Variables+=["den_tag_mistag_e_TTMassCut_tpt"]
    Variables+=["num_tag_mistag_e_TTLoose_fjpt"]
    Variables+=["num_tag_mistag_e_TTLoose_meff"]
    Variables+=["num_tag_mistag_e_TTLoose_tpt"]
    Variables+=["num_tag_mistag_e_TTMassCut_fjpt"]
    Variables+=["num_tag_mistag_e_TTMassCut_meff"]
    Variables+=["num_tag_mistag_e_TTMassCut_tpt"]
    Variables+=["den_tag_mistag_e_TTRCJ_fjpt"]
    Variables+=["den_tag_mistag_e_TTRCJ_meff"]
    Variables+=["den_tag_mistag_e_TTRCJ_tpt"]
    Variables+=["num_tag_mistag_e_TTRCJ_fjpt"]
    Variables+=["num_tag_mistag_e_TTRCJ_meff"]
    Variables+=["num_tag_mistag_e_TTRCJ_tpt"]
    Variables+=["den_tag_eff_mu_TTLoose_fjpt"]
    Variables+=["den_tag_eff_mu_TTLoose_meff"]
    Variables+=["den_tag_eff_mu_TTLoose_tpt"]
    Variables+=["den_tag_eff_mu_TTMassCut_fjpt"]
    Variables+=["den_tag_eff_mu_TTMassCut_meff"]
    Variables+=["den_tag_eff_mu_TTMassCut_tpt"]
    Variables+=["num_tag_eff_mu_TTLoose_fjpt"]
    Variables+=["num_tag_eff_mu_TTLoose_meff"]
    Variables+=["num_tag_eff_mu_TTLoose_tpt"]
    Variables+=["num_tag_eff_mu_TTMassCut_fjpt"]
    Variables+=["num_tag_eff_mu_TTMassCut_meff"]
    Variables+=["num_tag_eff_mu_TTMassCut_tpt"]
    Variables+=["den_tag_eff_mu_TTRCJ_fjpt"]
    Variables+=["den_tag_eff_mu_TTRCJ_meff"]
    Variables+=["den_tag_eff_mu_TTRCJ_tpt"]
    Variables+=["num_tag_eff_mu_TTRCJ_fjpt"]
    Variables+=["num_tag_eff_mu_TTRCJ_meff"]
    Variables+=["num_tag_eff_mu_TTRCJ_tpt"]
    Variables+=["den_tag_mistag_mu_TTLoose_fjpt"]
    Variables+=["den_tag_mistag_mu_TTLoose_meff"]
    Variables+=["den_tag_mistag_mu_TTLoose_tpt"]
    Variables+=["den_tag_mistag_mu_TTMassCut_fjpt"]
    Variables+=["den_tag_mistag_mu_TTMassCut_meff"]
    Variables+=["den_tag_mistag_mu_TTMassCut_tpt"]
    Variables+=["num_tag_mistag_mu_TTLoose_fjpt"]
    Variables+=["num_tag_mistag_mu_TTLoose_meff"]
    Variables+=["num_tag_mistag_mu_TTLoose_tpt"]
    Variables+=["num_tag_mistag_mu_TTMassCut_fjpt"]
    Variables+=["num_tag_mistag_mu_TTMassCut_meff"]
    Variables+=["num_tag_mistag_mu_TTMassCut_tpt"]
    Variables+=["den_tag_mistag_mu_TTRCJ_fjpt"]
    Variables+=["den_tag_mistag_mu_TTRCJ_meff"]
    Variables+=["den_tag_mistag_mu_TTRCJ_tpt"]
    Variables+=["num_tag_mistag_mu_TTRCJ_fjpt"]
    Variables+=["num_tag_mistag_mu_TTRCJ_meff"]
    Variables+=["num_tag_mistag_mu_TTRCJ_tpt"]

    Variables+=["den_tag_mistag_0top_TTLoose_fjpt"]
    Variables+=["den_tag_mistag_0top_TTLoose_meff"]
    Variables+=["den_tag_mistag_0top_TTLoose_tpt"]
    Variables+=["den_tag_mistag_0top_TTMassCut_fjpt"]
    Variables+=["den_tag_mistag_0top_TTMassCut_meff"]
    Variables+=["den_tag_mistag_0top_TTMassCut_tpt"]
    Variables+=["num_tag_mistag_0top_TTLoose_fjpt"]
    Variables+=["num_tag_mistag_0top_TTLoose_meff"]
    Variables+=["num_tag_mistag_0top_TTLoose_tpt"]
    Variables+=["num_tag_mistag_0top_TTMassCut_fjpt"]
    Variables+=["num_tag_mistag_0top_TTMassCut_meff"]
    Variables+=["num_tag_mistag_0top_TTMassCut_tpt"]
    Variables+=["den_tag_mistag_0top_TTRCJ_fjpt"]
    Variables+=["den_tag_mistag_0top_TTRCJ_meff"]
    Variables+=["den_tag_mistag_0top_TTRCJ_tpt"]
    Variables+=["num_tag_mistag_0top_TTRCJ_fjpt"]
    Variables+=["num_tag_mistag_0top_TTRCJ_meff"]
    Variables+=["num_tag_mistag_0top_TTRCJ_tpt"]
    Variables+=["den_tag_mistag_0top_e_TTLoose_fjpt"]
    Variables+=["den_tag_mistag_0top_e_TTLoose_meff"]
    Variables+=["den_tag_mistag_0top_e_TTLoose_tpt"]
    Variables+=["den_tag_mistag_0top_e_TTMassCut_fjpt"]
    Variables+=["den_tag_mistag_0top_e_TTMassCut_meff"]
    Variables+=["den_tag_mistag_0top_e_TTMassCut_tpt"]
    Variables+=["num_tag_mistag_0top_e_TTLoose_fjpt"]
    Variables+=["num_tag_mistag_0top_e_TTLoose_meff"]
    Variables+=["num_tag_mistag_0top_e_TTLoose_tpt"]
    Variables+=["num_tag_mistag_0top_e_TTMassCut_fjpt"]
    Variables+=["num_tag_mistag_0top_e_TTMassCut_meff"]
    Variables+=["num_tag_mistag_0top_e_TTMassCut_tpt"]
    Variables+=["den_tag_mistag_0top_e_TTRCJ_fjpt"]
    Variables+=["den_tag_mistag_0top_e_TTRCJ_meff"]
    Variables+=["den_tag_mistag_0top_e_TTRCJ_tpt"]
    Variables+=["num_tag_mistag_0top_e_TTRCJ_fjpt"]
    Variables+=["num_tag_mistag_0top_e_TTRCJ_meff"]
    Variables+=["num_tag_mistag_0top_e_TTRCJ_tpt"]
    Variables+=["den_tag_mistag_0top_mu_TTLoose_fjpt"]
    Variables+=["den_tag_mistag_0top_mu_TTLoose_meff"]
    Variables+=["den_tag_mistag_0top_mu_TTLoose_tpt"]
    Variables+=["den_tag_mistag_0top_mu_TTMassCut_fjpt"]
    Variables+=["den_tag_mistag_0top_mu_TTMassCut_meff"]
    Variables+=["den_tag_mistag_0top_mu_TTMassCut_tpt"]
    Variables+=["num_tag_mistag_0top_mu_TTLoose_fjpt"]
    Variables+=["num_tag_mistag_0top_mu_TTLoose_meff"]
    Variables+=["num_tag_mistag_0top_mu_TTLoose_tpt"]
    Variables+=["num_tag_mistag_0top_mu_TTMassCut_fjpt"]
    Variables+=["num_tag_mistag_0top_mu_TTMassCut_meff"]
    Variables+=["num_tag_mistag_0top_mu_TTMassCut_tpt"]
    Variables+=["den_tag_mistag_0top_mu_TTRCJ_fjpt"]
    Variables+=["den_tag_mistag_0top_mu_TTRCJ_meff"]
    Variables+=["den_tag_mistag_0top_mu_TTRCJ_tpt"]
    Variables+=["num_tag_mistag_0top_mu_TTRCJ_fjpt"]
    Variables+=["num_tag_mistag_0top_mu_TTRCJ_meff"]
    Variables+=["num_tag_mistag_0top_mu_TTRCJ_tpt"]
    Variables+=["den_tag_mistag_1top_TTLoose_fjpt"]
    Variables+=["den_tag_mistag_1top_TTLoose_meff"]
    Variables+=["den_tag_mistag_1top_TTLoose_tpt"]
    Variables+=["den_tag_mistag_1top_TTMassCut_fjpt"]
    Variables+=["den_tag_mistag_1top_TTMassCut_meff"]
    Variables+=["den_tag_mistag_1top_TTMassCut_tpt"]
    Variables+=["num_tag_mistag_1top_TTLoose_fjpt"]
    Variables+=["num_tag_mistag_1top_TTLoose_meff"]
    Variables+=["num_tag_mistag_1top_TTLoose_tpt"]
    Variables+=["num_tag_mistag_1top_TTMassCut_fjpt"]
    Variables+=["num_tag_mistag_1top_TTMassCut_meff"]
    Variables+=["num_tag_mistag_1top_TTMassCut_tpt"]
    Variables+=["den_tag_mistag_1top_TTRCJ_fjpt"]
    Variables+=["den_tag_mistag_1top_TTRCJ_meff"]
    Variables+=["den_tag_mistag_1top_TTRCJ_tpt"]
    Variables+=["num_tag_mistag_1top_TTRCJ_fjpt"]
    Variables+=["num_tag_mistag_1top_TTRCJ_meff"]
    Variables+=["num_tag_mistag_1top_TTRCJ_tpt"]
    Variables+=["den_tag_mistag_1top_e_TTLoose_fjpt"]
    Variables+=["den_tag_mistag_1top_e_TTLoose_meff"]
    Variables+=["den_tag_mistag_1top_e_TTLoose_tpt"]
    Variables+=["den_tag_mistag_1top_e_TTMassCut_fjpt"]
    Variables+=["den_tag_mistag_1top_e_TTMassCut_meff"]
    Variables+=["den_tag_mistag_1top_e_TTMassCut_tpt"]
    Variables+=["num_tag_mistag_1top_e_TTLoose_fjpt"]
    Variables+=["num_tag_mistag_1top_e_TTLoose_meff"]
    Variables+=["num_tag_mistag_1top_e_TTLoose_tpt"]
    Variables+=["num_tag_mistag_1top_e_TTMassCut_fjpt"]
    Variables+=["num_tag_mistag_1top_e_TTMassCut_meff"]
    Variables+=["num_tag_mistag_1top_e_TTMassCut_tpt"]
    Variables+=["den_tag_mistag_1top_e_TTRCJ_fjpt"]
    Variables+=["den_tag_mistag_1top_e_TTRCJ_meff"]
    Variables+=["den_tag_mistag_1top_e_TTRCJ_tpt"]
    Variables+=["num_tag_mistag_1top_e_TTRCJ_fjpt"]
    Variables+=["num_tag_mistag_1top_e_TTRCJ_meff"]
    Variables+=["num_tag_mistag_1top_e_TTRCJ_tpt"]
    Variables+=["den_tag_mistag_1top_mu_TTLoose_fjpt"]
    Variables+=["den_tag_mistag_1top_mu_TTLoose_meff"]
    Variables+=["den_tag_mistag_1top_mu_TTLoose_tpt"]
    Variables+=["den_tag_mistag_1top_mu_TTMassCut_fjpt"]
    Variables+=["den_tag_mistag_1top_mu_TTMassCut_meff"]
    Variables+=["den_tag_mistag_1top_mu_TTMassCut_tpt"]
    Variables+=["num_tag_mistag_1top_mu_TTLoose_fjpt"]
    Variables+=["num_tag_mistag_1top_mu_TTLoose_meff"]
    Variables+=["num_tag_mistag_1top_mu_TTLoose_tpt"]
    Variables+=["num_tag_mistag_1top_mu_TTMassCut_fjpt"]
    Variables+=["num_tag_mistag_1top_mu_TTMassCut_meff"]
    Variables+=["num_tag_mistag_1top_mu_TTMassCut_tpt"]
    Variables+=["den_tag_mistag_1top_mu_TTRCJ_fjpt"]
    Variables+=["den_tag_mistag_1top_mu_TTRCJ_meff"]
    Variables+=["den_tag_mistag_1top_mu_TTRCJ_tpt"]
    Variables+=["num_tag_mistag_1top_mu_TTRCJ_fjpt"]
    Variables+=["num_tag_mistag_1top_mu_TTRCJ_meff"]
    Variables+=["num_tag_mistag_1top_mu_TTRCJ_tpt"]

    Variables+=["truthclass_mistag_TTLoose"]
    Variables+=["truthclass_mistag_0top_TTLoose"]
    Variables+=["truthclass_mistag_1top_TTLoose"]
    Variables+=["truthclass_mistag_TTMassCut"]
    Variables+=["truthclass_mistag_0top_TTMassCut"]
    Variables+=["truthclass_mistag_1top_TTMassCut"]
    Variables+=["truthclass_mistag_TTRCJ"]
    Variables+=["truthclass_mistag_0top_TTRCJ"]
    Variables+=["truthclass_mistag_1top_TTRCJ"]
    Variables+=["truthclass_mistag_mu_TTLoose"]
    Variables+=["truthclass_mistag_0top_mu_TTLoose"]
    Variables+=["truthclass_mistag_1top_mu_TTLoose"]
    Variables+=["truthclass_mistag_mu_TTMassCut"]
    Variables+=["truthclass_mistag_0top_mu_TTMassCut"]
    Variables+=["truthclass_mistag_1top_mu_TTMassCut"]
    Variables+=["truthclass_mistag_mu_TTRCJ"]
    Variables+=["truthclass_mistag_0top_mu_TTRCJ"]
    Variables+=["truthclass_mistag_1top_mu_TTRCJ"]
    Variables+=["truthclass_mistag_e_TTLoose"]
    Variables+=["truthclass_mistag_0top_e_TTLoose"]
    Variables+=["truthclass_mistag_1top_e_TTLoose"]
    Variables+=["truthclass_mistag_e_TTMassCut"]
    Variables+=["truthclass_mistag_0top_e_TTMassCut"]
    Variables+=["truthclass_mistag_1top_e_TTMassCut"]
    Variables+=["truthclass_mistag_e_TTRCJ"]
    Variables+=["truthclass_mistag_0top_e_TTRCJ"]
    Variables+=["truthclass_mistag_1top_e_TTRCJ"]
##........................................................

##________________________________________________________
## Defining the list of regions to look at
Regions = []
#1lepton regions
if( doLepton ):
    #
    # Control/signal regions
    #
    #0HOT
    Regions += ["c1l0RCTTMass6j2b"]
    Regions += ["c1l0RCTTMass6j3b"]
    Regions += ["c1l0RCTTMass6j4b"]
    Regions += ["c1l0RCTTMass6j3bLowMbb"]
    Regions += ["c1l0RCTTMass6j3bHighMbb"]
    Regions += ["c1l0RCTTMass6j4bLowMbb"]
    Regions += ["c1l0RCTTMass6j4bHighMbb"]
    #1HOT
    Regions += ["c1l1RCTTMass6j2b"]
    Regions += ["c1l1RCTTMass6j3b"]
    Regions += ["c1l1RCTTMass6j4b"]
    Regions += ["c1l1RCTTMass6j3bLowMbb"]
    Regions += ["c1l1RCTTMass6j3bHighMbb"]
    Regions += ["c1l1RCTTMass6j4bLowMbb"]
    Regions += ["c1l1RCTTMass6j4bHighMbb"]
    #2HOT
    Regions += ["c1l2RCTTMass6j2b"]
    Regions += ["c1l2RCTTMass6j3b"]
    Regions += ["c1l2RCTTMass6j4b"]
    #
    # Validation regions
    #
    #0HOT
    Regions += ["c1l0RCTTMass5j2b"]
    Regions += ["c1l0RCTTMass5j3b"]
    Regions += ["c1l0RCTTMass5j4b"]
    #1HOT
    Regions += ["c1l1RCTTMass5j2b"]
    Regions += ["c1l1RCTTMass5j3b"]
    Regions += ["c1l1RCTTMass5j4b"]
    #2HOT
    Regions += ["c1l2RCTTMass5j2b"]
    Regions += ["c1l2RCTTMass5j3b"]
    Regions += ["c1l2RCTTMass5j4b"]

if ( doZeroLepton ):
    #
    # Signal/Control regions
    #
    #0HOT
    Regions += ["c0l0RCTTMass7j2b"]
    Regions += ["c0l0RCTTMass7j3b"]
    Regions += ["c0l0RCTTMass7j4b"]
    Regions += ["c0l0RCTTMass7j3bLowMtbmin"]
    Regions += ["c0l0RCTTMass7j4bLowMtbmin"]
    Regions += ["c0l0RCTTMass7j3bHighMtbmin"]
    Regions += ["c0l0RCTTMass7j4bHighMtbmin"]
    #1HOT
    Regions += ["c0l1RCTTMass7j2b"]
    Regions += ["c0l1RCTTMass7j3b"]
    Regions += ["c0l1RCTTMass7j4b"]
    Regions += ["c0l1RCTTMass7j3bLowMtbmin"]
    Regions += ["c0l1RCTTMass7j4bLowMtbmin"]
    Regions += ["c0l1RCTTMass7j3bHighMtbmin"]
    Regions += ["c0l1RCTTMass7j4bHighMtbmin"]
    #2HOT
    Regions += ["c0l2RCTTMass7j2b"]
    Regions += ["c0l2RCTTMass7j3b"]
    Regions += ["c0l2RCTTMass7j4b"]
    Regions += ["c0l2RCTTMass7j3bLowMtbmin"]
    Regions += ["c0l2RCTTMass7j3bHighMtbmin"]

    #
    # Vaidation regions
    #
    # 0HOT
    Regions += ["c0l0RCTTMass6j2b"]
    Regions += ["c0l0RCTTMass6j3b"]
    Regions += ["c0l0RCTTMass6j4b"]
    # 1HOT
    Regions += ["c0l1RCTTMass6j2b"]
    Regions += ["c0l1RCTTMass6j3b"]
    Regions += ["c0l1RCTTMass6j4b"]
    # 2HOT
    Regions += ["c0l2RCTTMass6j2b"]
    Regions += ["c0l2RCTTMass6j3b"]
    Regions += ["c0l2RCTTMass6j4b"]

if doEfficiency:
    Regions = []
    Regions += ["4j2b"]
    Regions += ["5j2b"]
    Regions += ["5j3b"]
    Regions += ["5j4b"]
    Regions += ["6j2b"]
    Regions += ["6j3b"]
    Regions += ["6j4b"]
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
        script.write("cp "+inputDir+"/out*VLQ_TT_%.0f_*.root inputs/ \n" %(sample))

        com = "python VLQCouplingReweighter.py -i inputs/ -o ./"
        if statOnly:
            com += " -s "
        if doEfficiency:
            com += " -e "
        com += " -m %.0f " %(sample)
        com += " -c " + br
        com += " -r "
        for reg in Regions:
            com += reg
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
