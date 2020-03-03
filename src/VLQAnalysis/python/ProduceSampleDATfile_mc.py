#!/bin/python
import sys
import os
import string
import time, getpass
import socket
import datetime
import json
from ROOT import *
from VLQ_Samples_mc import *

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
jsonOutput = True
##........................................................

##________________________________________________________
## Defines some useful variables
platform = socket.gethostname()
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()
##........................................................

##________________________________________________________
## Defining the paths and the tarball
inputDir="/nfs/atlas-data07/tvdaalen/VLQ_FullRun2/Ttbar_AltGenSyst_output_Oct2019/MV2/Nominal_AFII/" 
# "nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.67-htztx-0-fJvt-DL1.PFlow/"
listFolder=here+"/Lists_Analysis_" + now
histogramName="cut_flow"
mccampaign=""
configName = "configFile_MV2_"+mccampaign+".dat"
doSysWeights = False

if(len(sys.argv))>1:
    for x in sys.argv[1:]:
        splitted=x.split("=")
        if(len(splitted)!=2):
            printError("<!> The argument \"" + x + "\" has no equal signs ... Please check")
            sys.exit(-1)
        argument = splitted[0].upper().replace("--","")
        value = splitted[1]
        if(argument=="INPUTDIR"):
            inputDir=value
        elif(argument=="LISTFOLDER"):
            listFolder=value
        elif(argument=="CAMPAIGN"):
            mccampaign=value
        elif(argument=="CONFIGNAME"):
            configName=value
        elif(argument=="XSECFILE"):
            xsecFile=value
        elif(argument=="DOSYSWEIGHTS"):
            if splitted[1].upper()=="TRUE":
                doSysWeights = True
            elif splitted[1].upper()=="FALSE":
                doSysWeights = False

##........................................................

##________________________________________________________
## Creating usefull repositories
os.system("mkdir -p " + listFolder) #list files folder
##........................................................

##________________________________________________________
## Getting all samples and their associated weight/object systematics
Samples = []
Samples += GetTtbarSamples(hfSplitted=False,ttbarSystSamples=False,useHTSlices=False,campaign=mccampaign)
Samples += GetOtherBackgroundSamples (campaign=mccampaign)
#Samples += GetSingleVLQSamples( includeSingleVLQ=True, campaign=mccampaign )
printGoodNews("--> All samples recovered")
##........................................................

##________________________________________________________
## Creating the config file
configFile = open(configName, "w")
if jsonOutput:
    configDict = {}
else:
    configFile.write("#\n#\n")
    configFile.write("# Path to files: " + inputDir + "\n")
    configFile.write("# Date: " + now + "\n")
    configFile.write("# Histogram used to normalize: "+ histogramName +"\n")
    configFile.write("#\n#\n")

##________________________________________________________
## Looking into the XSe DB file
with open(xsecFile,'r') as f:
#with open("/nfs/atlas-data07/tvdaalen/SingleVLQWorkArea/br_R21_master_Oct2019/VLQAnalysis/python/xsec_ttbar_dummy.json",'r') as f:
    weights = json.load(f)

##________________________________________________________
## Loop over samples
printGoodNews("--> Performing the loop over samples")
for sample in Samples:

    SName = sample['name'] # sample name
    SType = sample['sampleType'] # sample type

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

    weightDict = {}
    doSysWeights_temp = doSysWeights
    if SType not in ['ttbar','Singletop','Wjets','Zjets','Dibosons']:
        doSysWeights_temp = False

    for f in files:
        root_f = TFile(f,"read")

        if(root_f.IsZombie()):
            printError("<!> The file "+f+" is corrupted ... removes it from the list")
            continue

        h_nEvents = root_f.Get(histogramName).Clone()
        h_nEvents.SetDirectory(0)
        nEventsWeighted += h_nEvents.GetBinContent(2)
        d_SampleInfo = weights.get(SName.replace("."+mccampaign,""))
        if not d_SampleInfo:
            continue

        crossSection = d_SampleInfo.get('xsec')
        kFactor = d_SampleInfo.get('kFactor')
        filterEff = d_SampleInfo.get('genFiltEff')

        crossSection = float(crossSection*filterEff*kFactor)

        if doSysWeights_temp:
            try:
                h_sumOfWeights = root_f.Get('sumOfWeights_pmg').Clone()
            except ReferenceError:
                printError("<!> No 'sumOfWeights_pmg' histogram found in file "+f+" ... Aborting for this sample.")
                doSysWeights_temp = False
                continue
            if not h_sumOfWeights.Integral() > 0.:
                printError("<!> Unfilled 'sumOfWeights_pmg' histogram found in file "+f+" ... Aborting for this sample.")
                doSysWeights_temp = False
                continue

            weightList = list(h_sumOfWeights.GetXaxis().GetLabels())

            for weight in weightList:
                if str(weight).replace(' ','') == 'nominal':
                    continue

                sumOfWeightsAdjusted = h_sumOfWeights.GetBinContent(weightList.index(weight)+1)
                weightname = 'nWeightedEvents_'+str(weight).replace('_','__').replace(' = ','').replace(', ','__').replace(':','_').replace('=','').replace('.','').replace(' ','')

                if not weightDict.has_key(weightname):
                    weightDict[weightname] = sumOfWeightsAdjusted
                else:
                    weightDict[weightname] += sumOfWeightsAdjusted

    if jsonOutput:
        configDict[dsid] = {'crossSection': crossSection, 'nWeightedEvents': nEventsWeighted}

        if doSysWeights_temp:
            configDict[dsid].update(weightDict)

    else:
        configFile.write(dsid+" "+`nEventsWeighted`+" "+`crossSection`+"\n")

    if crossSection < 0:
        printError("<!> The sample "+ SName +" is not in the cross-section file. Please check ! The cross-section is set to -1.")
        continue

if jsonOutput:
    configFile.write(json.dumps(configDict,sort_keys=True,indent=4))

configFile.close()

##________________________________________________________
## Removing list folder
os.system("rm -rf " + listFolder) #list files folder
##........................................................
