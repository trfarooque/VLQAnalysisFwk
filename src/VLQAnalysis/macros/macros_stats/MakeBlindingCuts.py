#!/bin/python3

import argparse
import sys,os
import importlib
import numpy as np
from ROOT import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" )

parser = argparse.ArgumentParser()

parser.add_argument('--baseDir', 
                    help='Directory containing fit input files for all signals', 
                    required=True)
#parser.add_argument('--outFileName',
#                    help='Name of output file',
#                    default='BinningList.txt')
parser.add_argument('--threshold',
                    help='S/B threshold to use for blinding',
                    type=float, default=0.05)
parser.add_argument('--signals',
                    help='comma-separated list of signals',
                    required=True)
parser.add_argument('--regDict', 
                    help='Name of region dictionary module', 
                    default='regions_dictionary_pVLQ_newAna_MVA_regions')
parser.add_argument('--doOneLep',
                    help='Use 1lep channel regions',
                    type=int, required=True)
parser.add_argument('--doZeroLep',
                    help='Use 0lep channel regions',
                    type=int, required=True)

args = parser.parse_args()

baseDir=args.baseDir
#outFileName=args.outFileName
threshold=args.threshold 
regDict=args.regDict
doOneLep=args.doOneLep
doZeroLep=args.doZeroLep

signalList = (args.signals).split(',')

bkgList = ["Dibosons","Dijet","SM4tops","Singletop","SingletopWtprod","Singletopschan","Singletoptchan","Wjets","Zjets","topEW","ttH","ttbarbb","ttbarcc","ttbarlight"]
#binnings = [0,1000]
binnings = [e for e in range(1000,2000,100)]
binnings += [e for e in range(2000,3000,200)]
binnings += [e for e in range(3000,7000,1000)]

#binnings = [e for e in range(1000,7100,100)]
##------------------------------------------------------
## Getting all regions
##------------------------------------------------------
Regions = []
regModule = importlib.import_module(regDict)

if doOneLep:
    Regions += regModule.fit_regions_1l
if doZeroLep:
    Regions += regModule.fit_regions_0l


#outputFile = open(outFileName, 'w')

#Map of the signal and background histograms
bkgd_hists = {}
sig_hists = {}

for signal in signalList:
    sig_hists[signal] = {}

mcScales = {"mc16a":36207.66, "mc16d":44307.4, "mc16e":58450.1}

############# Read all histograms ##################################
for bkg in bkgList:

    for campaign in ["mc16a","mc16d","mc16e"]:

        bkgFile = TFile.Open(baseDir+bkg+'.'+campaign+'.root','READ')
        for reg in Regions:
            bkgHist = bkgFile.Get(reg['name'].replace('HTX_','')+'_meff')
            bkgHist.Scale(mcScales[campaign])
            if reg['name'] in bkgd_hists:
                bkgd_hists[reg['name']].Add(bkgHist)
            else:
                bkgd_hists[reg['name']] = bkgHist.Clone()
                bkgd_hists[reg['name']].SetDirectory(0)
        bkgFile.Close()


for signal in signalList:

    for campaign in ["mc16a","mc16d","mc16e"]:

        sigFile = TFile.Open(baseDir+signal+'.'+campaign+'.root','READ')
        for reg in Regions:
            sigHist = sigFile.Get(reg['name'].replace('HTX_','')+'_meff')
            sigHist.Scale(mcScales[campaign])
            if reg['name'] in sig_hists[signal]:
                sig_hists[signal][reg['name']].Add(sigHist)
            else:
                sig_hists[signal][reg['name']] = sigHist.Clone()
                sig_hists[signal][reg['name']].SetDirectory(0)
        sigFile.Close()
                
####################################################################

############# Calculate and print blinding thresholds ########################
for reg in Regions:

    print '\n\n'
    print "#############################"
    print reg['name']
    ###### Set initial binnings ###########
    h_bkg = bkgd_hists[reg['name']]
    bkgd_hists[reg['name']] = h_bkg.Rebin(len(binnings)-1, h_bkg.GetName()+'_rebinned',
                                          np.array(binnings,dtype=float))
    for signal in signalList:
        h_sig = sig_hists[signal][reg['name']]
        sig_hists[signal][reg['name']] = h_sig.Rebin(len(binnings)-1, h_sig.GetName()+'_rebinned',
                                                     np.array(binnings,dtype=float))

    #######################################
    blind_edge = -1.
    blind_edge_cumul = -1.

    ##### if overall S/B exceeds threshold, then full region should be blinded ####
    #SbyB_tot = np.array( [sig_hists[s][reg['name']].Integral() / bkgd_hists[reg['name']].Integral() \
    #                      for s in signalList] )
    #if np.any(SbyB_tot > threshold):
    #    print('reg: ',reg['name'])
    #    print('blind_edge: ',0.)
    #    print('blind_edge_cumul: ',0.)
    #    continue


    ### Loop through bins and compute blinding cut ###
    #
    for ibin,low_edge in reversed(list(enumerate(binnings, start=1))):
        if(bkgd_hists[reg['name']].GetBinContent(ibin) > 0):
            SbyB = np.array([sig_hists[s][reg['name']].GetBinContent(ibin)\
                             /bkgd_hists[reg['name']].GetBinContent(ibin) for s in signalList])
            #print ibin,', ',low_edge,' : ',SbyB
            #for s in np.where(SbyB>threshold)[0]:
            #    print signalList[s],' : ',SbyB[s] 

            if np.any(SbyB > threshold):
                blind_edge = low_edge

        if(bkgd_hists[reg['name']].Integral(ibin,-1) > 0):
            SbyB_cumul = np.array([sig_hists[s][reg['name']].Integral(ibin,-1)\
                                   /bkgd_hists[reg['name']].Integral(ibin,-1) for s in signalList])
            if np.any(SbyB_cumul > threshold):
                blind_edge_cumul = low_edge

    print('reg: ',reg['name'])
    print('blind_edge: ',blind_edge)
    print('blind_edge_cumul: ',blind_edge_cumul)


    #######################################


########################################################
