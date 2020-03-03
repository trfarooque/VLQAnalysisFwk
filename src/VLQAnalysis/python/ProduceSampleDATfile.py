#!/bin/python
import sys
import os
import string
import time, getpass
import socket
import datetime
import json
from ROOT import *
from VLQ_Samples_Tal import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "python/IFAETopFramework/" )
from BatchTools import *
from Job import *
from Samples import *

##________________________________________________________
## OPTIONS
debug=True
nFilesSplit = 1000
nMerge=1
wgtSysSplit=False
##........................................................

##________________________________________________________
## Defines some useful variables
platform = socket.gethostname()
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()
##........................................................

##________________________________________________________
## Defining the paths and the tarball
inputDir="/nfs/at3/scratch2/tvdaalen/VLQ_FullRun2/Production_tag_Feb2019_correctedsamples/"
listFolder=here+"/Lists_Analysis_" + now
histogramName="cut_flow"
##........................................................

##________________________________________________________
## Creating usefull repositories
os.system("mkdir -p " + listFolder) #list files folder
##........................................................

##________________________________________________________
## Getting all samples and their associated weight/object systematics
Samples = []
Samples += GetTtbarSamples(hfSplitted=False,ttbarSystSamples=True,useHTSlices=True)
#Samples += GetNewTTbarGenerators( useWeightSyst=False, useObjectSyst=False )
Samples += GetOtherSamples (includeSignals=True,includeSingletopSystSamples=True)
printGoodNews("--> All samples recovered")
##........................................................

##________________________________________________________
## Creating the config file
configFile = open("configFile_"+now+".dat","w")
configFile.write("#\n#\n")
configFile.write("# Path to files: " + inputDir + "\n")
configFile.write("# Date: " + now + "\n")
configFile.write("# Histogram used to normalize: "+ histogramName +"\n")
configFile.write("#\n#\n")

##________________________________________________________
## Looking into the XSe DB file
with open("../data/weights_mc16a.json",'r') as f:
    weights = json.load(f)

##________________________________________________________
## Loop over samples
printGoodNews("--> Performing the loop over samples")
for sample in Samples:

    SName = sample['name'] # sample name

    iDir = inputDir
    excluded = []

    joblist = getSampleJobs(sample,InputDir=iDir+"/",NFiles=nFilesSplit,UseList=False,ListFolder=listFolder,exclusions=excluded,useDiffFilesForObjSyst=False)

    if( not joblist or len(joblist)!=1):
        printError("<!> The job for sample "+SName+" isn't standard: please check")
        continue

    files = joblist[0]['filelist'].split(",")
    nEventsWeighted = 0
    crossSection = -1
    dsid = SName
    for f in files:
        root_f = TFile(f,"read")

        if(root_f.IsZombie()):
            printError("<!> The file "+f+" is corrupted ... removes it from the list")
            continue

        h_nEvents = root_f.Get(histogramName).Clone()
        h_nEvents.SetDirectory(0)
        nEventsWeighted += h_nEvents.GetBinContent(2)

        d_SampleInfo = weights.get(SName.replace(".",""))
        if not d_SampleInfo:
            continue

        crossSection = d_SampleInfo.get('cross section')
        kFactor = d_SampleInfo.get('k-factor')
        filterEff = d_SampleInfo.get('filter efficiency')

        crossSection = float(crossSection*filterEff*kFactor)

    configFile.write(dsid+" "+`nEventsWeighted`+" "+`crossSection`+"\n")
    if crossSection < 0:
        printError("<!> The sample "+ SName +" is not in the cross-section file. Please check ! The cross-section is set to -1.")
        continue

configFile.close()

##________________________________________________________
## Removing list folder
os.system("rm -rf " + listFolder) #list files folder
##........................................................
