#!/bin/python
import os
import time, getpass
import sys
import datetime
import argparse
from ROOT import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/")
from VLQ_Samples_mc import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/" )
from BatchTools import *
from Samples import *



##------------------------------------------------------
## Defines some useful variables
##------------------------------------------------------
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()


#### Some common lists of attributes ######
allowed_campaigns = ['mc16a', 'mc16d', 'mc16e']

bkg_procs = ['ttbar','singletop','Wjets','Zjets','diboson','topEW','dijet']
bkg_sys_procs = ['ttbar_alt','singletop_alt']
all_procs = ['data','pvlq','svlq'] + bkg_procs + bkg_sys_procs
allowed_procs = all_procs + ['bkg','bkg_sys']


parser = argparse.ArgumentParser()

##------------------------------------------------------
## Checking the arguments
##------------------------------------------------------
parser.add_argument('--inputDir', help='Input directory containing ntuples', required=True)
parser.add_argument('--outputDir', help='Output directory where histogram/mini-tree files will be stored', required=True)

parser.add_argument('--campaigns', help='MC campaigns to launch', nargs='*',
                    choices=allowed_campaigns, default=allowed_campaigns)
#parser.add_argument('--channel', help='Lepton channel', choices=['0lep','1lep'], required=True)
parser.add_argument('--processes', help='Space-separated list of sample processes', nargs='*',
                    choices=allowed_procs, default=all_procs)
parser.add_argument('--useSyst', help='Run systematics when set', action='store', type=int, default=0)
parser.add_argument('--splitSingleTop', help='Find single top samples split by channel', action='store', type=int, default=1)
parser.add_argument('--mergeSingleTop', help='Merge all single top channels', action='store', type=int, default=1)
parser.add_argument('--mergeRareBkg', help='Merge rare background processes', action='store', type=int, default=0)
parser.add_argument('--doAllBRPVLQ', help='Run all branching ratios for pvlq samples', action='store', type=int, default=0)

parser.add_argument('--dryRun', help='Do not launch jobs when set', action='store', type=int, default=0)
parser.add_argument('--verbose', help='Print debug messages', action='store', type=int, default=0)

#parser.add_argument('--batch', help='Type of batch system', default='condor')
#parser.add_argument('--queue', help='Batch queue to which jobs are submittied', default='at3_short')

args = parser.parse_args()

inputDir = args.inputDir
outputDir = args.outputDir
campaigns = args.campaigns
#channel = args.channel
useSystematics = args.useSyst
splitSingleTop = args.splitSingleTop
mergeSingleTop = args.mergeSingleTop
mergeRareBkg = args.mergeRareBkg
doAllBRPVLQ = args.doAllBRPVLQ
dryRun = args.dryRun
verbose = args.verbose

##_________________________________________
##List of processes to run
procs_to_run = []
for proc in args.processes:
    print 'Adding process : ',proc
    if proc=='bkg':
        procs_to_run += bkg_procs
    elif proc=='bkg_sys':
        procs_to_run += bkg_sys_procs
    else:
        procs_to_run += [proc]

print "#Processes: ",len(procs_to_run)

#if(inputDir==""):
#    printError("<!> Please provide an input config file to use !")
#    sys.exit()

##-------------------------------------------------------------
## Function which will find and merge files for a given process
##-------------------------------------------------------------
def FindAndMergeFiles(proc_name):

    sample_inputDir = inputDir + '/ttstalt/' if(proc_name=='ttbar_alt' or proc_name=='singletop_alt') \
                      else inputDir + '/nominal/'
    sample_outputDir = outputDir + '/ttstalt/' if(proc_name=='ttbar_alt' or proc_name=='singletop_alt') \
                       else outputDir + '/nominal/'
    sample_campaigns = [''] if(proc_name=='data') else campaigns
    sample_systList = ['nominal'] if(proc_name=='data' or proc_name=='ttbar_alt' or proc_name=='singletop_alt') \
                      else systList

    Samples = retrieveSamples(proc_name, '', useSyst=False, postRWPVLQ=True, allBRPVLQ=doAllBRPVLQ)

    usedProc = []
    for sample in Samples:

        SName = sample['name'] #sample name/DSID.campaign
        SType = sample['sampleType'] #name of process

        ### Multiple DSIDs will share the same SType (process name).
        ### Each process name is only merged once
        if SType in usedProc:
            continue
        else:
            usedProc += [SType]

        for campaign in sample_campaigns:

            for syst in sample_systList:

                globPattern = sample_inputDir+"/outVLQAna_"+SType+"_*."+campaign+"_"+syst+"_*.root" \
                             if not(campaign=="") else \
                                sample_inputDir+"/outVLQAna_"+SType+"_*_"+syst+"_*.root"
                #find the list of files that need to be merged
                if verbose:
                    print 'Listing files : ',globPattern
                listfiles = glob.glob(globPattern)

                name_temp_outfile = sample_outputDir+"/"+SType
                if not(campaign==""):
                    name_temp_outfile += "."+campaign
                if syst.upper().find("NOMINAL")==-1:
                    name_temp_outfile += "_"+syst
                name_temp_outfile += ".root"

                #command to run
                comm = ''
                if len(listfiles)==0:
                    printWarning('No files retrieved')
                elif(len(listfiles)==1):
                    comm = 'cp '+listfiles[0]+' '+name_temp_outfile
                else:
                    comm = 'hadd -v 1 '+name_temp_outfile
                    for infile in listfiles:
                        comm += ' '+infile
                #print comm
                #Commands += [comm]
                if dryRun:
                    print comm
                else:
                    os.system(comm)
                if verbose:
                    print "syst : ",syst," <<====="

            printGoodNews("campaign : "+campaign+" <<=====")
        if verbose:
            print "SType : ",SType," <<====="

    printGoodNews("proc : "+proc_name+" <<=====")

    return

##........................................................
start =time.time()
print "START"


##------------------------------------------------------
## Creating the output repositories
##------------------------------------------------------
os.system("mkdir -p "+outputDir)
os.system("mkdir -p "+outputDir+'/nominal/')
os.system("mkdir -p "+outputDir+'/ttstalt/')



##------------------------------------------------------
## Getting all samples and their associated weight/object systematics
##------------------------------------------------------

#Get list of object systematics
ObjectSystematics = []
if useSystematics:
    ObjectSystematics += CommonObjectSystematics
else:
    ObjectSystematics = [getSystematics(name="nominal",nameUp="",oneSided=True)]

##Unpack systematics list:
systList = []
for Systematic in ObjectSystematics:
    if Systematic['oneSided']:
        systList += [Systematic['name']]
    else:
        systList += [Systematic['nameUp']]
        systList += [Systematic['nameDown']]

print "#Systematics : ",len(systList)

for s_proc in procs_to_run:
    FindAndMergeFiles(s_proc)

printGoodNews("--> All samples merged")

##------------------------------------------------------
## Merge single top files if needed
##------------------------------------------------------
if mergeSingleTop:
    printGoodNews("--> Now merging single-top channels")

    if True:#'singletop' in procs_to_run:
        sampleDir = outputDir + '/nominal/'

        for campaign in campaigns:

            for syst in systList:

                file_suffix = "."+campaign
                if syst.upper().find("NOMINAL")==-1:
                    file_suffix += "_"+syst
                file_suffix += ".root"

                comm = "hadd  -v 1 "+sampleDir+"/Singletop"+file_suffix \
                       +" "+sampleDir+"/Singletopschan"+file_suffix \
                       +" "+sampleDir+"/Singletoptchan"+file_suffix \
                       +" "+sampleDir+"/SingletopWtprod"+file_suffix
                if dryRun:
                    print comm
                else:
                    os.system(comm)

    if 'singletop_alt' in procs_to_run:
        sampleDir = outputDir + '/ttstalt/'
        sampleDirNominal = outputDir + '/nominal/'

        for campaign in campaigns:

            for syst in ["","PowHer","aMCPy", "DiagSub"]:

                #SingletopschanPowHer.mc16a.root
                file_suffix = syst+"."+campaign+".root"
                file_suffix_nominal = "."+campaign+".root"
                if syst=="DiagSub":
                    comm = "hadd  -v 1 "+sampleDir+"/Singletop"+file_suffix \
                           +" "+sampleDir+"/Singletopschan"+file_suffix \
                           +" "+sampleDir+"/Singletoptchan"+file_suffix \
                           +" "+sampleDir+"/SingletopWtprod"+file_suffix
                else:
                    comm = "hadd -v 1 "+sampleDir+"/Singletop"+file_suffix \
                           +" "+sampleDirNominal+"/Singletopschan"+file_suffix_nominal \
                           +" "+sampleDirNominal+"/Singletoptchan"+file_suffix_nominal \
                           +" "+sampleDir+"/SingletopWtprod"+file_suffix


##------------------------------------------------------
## Merge rare backgrounds (tZ, SM4tops, VH)
##------------------------------------------------------
if mergeRareBkg:
    printGoodNews("--> Now merging rare backgrounds")

    sampleDir = outputDir + '/nominal/'

    for campaign in sample_campaigns:

        for syst in systList:

            file_suffix = "."+campaign
            if syst.upper().find("NOMINAL")==-1:
                file_suffix += "_"+syst
            file_suffix += ".root"

            comm = "hadd -v 1 "+sampleDir+"/rareBkg"+file_suffix \
                   +" "+sampleDir+"/tZ"+file_suffix \
                   +" "+sampleDir+"/VH"+file_suffix \
                   +" "+sampleDir+"/SM4tops"+file_suffix

            if dryRun:
                print comm
            else:
                os.system(comm)

elapsed=time.time()-start
elapsed=elapsed/60
print elapsed, " minuti trascorsi"
print "END"
