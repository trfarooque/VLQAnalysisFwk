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
isolateBadFiles = True
##........................................................

##________________________________________________________
## Defines some useful variables
platform = socket.gethostname()
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()
##........................................................

##________________________________________________________
## Defining the paths and the tarball
inputDir="/nfs/at3/scratch2/tvdaalen/VLQ_FullRun2/Production_tag-21.2.87-htztx-3-syst/" 
# "nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.67-htztx-0-fJvt-DL1.PFlow/"
listFolder=here+"/Lists_Analysis_" + now
histogramName="cut_flow"
mccampaign=""
configName = "configFile_MV2_"+mccampaign+".dat"
doSysWeights = False
doVLQSignalSumWeights = False
badFileList = []

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
        elif(argument=="DOVLQSIGNALSUMWEIGHTS"):
            if splitted[1].upper()=="TRUE":
                doVLQSignalSumWeights = True
            # doVLQSignalSumWeights is False by default

##........................................................

##________________________________________________________
## Creating usefull repositories
os.system("mkdir -p " + listFolder) #list files folder
##........................................................

##________________________________________________________
## Getting all samples and their associated weight/object systematics
Samples = []
Samples += GetTtbarSamples(hfSplitted=False,ttbarSystSamples=True,useHTSlices=True,campaign=mccampaign)
Samples += GetOtherBackgroundSamples (campaign=mccampaign,includeSingletopSystSamples=False)
Samples += GetSingleVLQSamples( campaign=mccampaign )
#Samples += GetDijetSamples( campaign=mccampaign )
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
            printError("<!> The file "+f+" is corrupted ... removed it from the list")
            badFileList.append(f)
            continue

        try:
            h_nEvents = root_f.Get(histogramName).Clone()
            h_nEvents.SetDirectory(0)
        except ReferenceError:
            printError("<!> No "+histogramName+" histogram found in file "+f+" ... removed it from the list")
            badFileList.append(f)
            continue

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

        if doVLQSignalSumWeights:
            try:
                h_vlqSumOfWeights = root_f.Get('vlqRW_sumOfWeights').Clone()
            except ReferenceError:
                printError("<!> No 'vlqRW_sumOfWeights' histogram found in file "+f+" ... Aborting for this sample.")
                doVLQSignalSumWeights = False
                continue
            if not h_vlqSumOfWeights.Integral() > 0.:
                printError("<!> Unfilled 'vlqRW_sumOfWeights' histogram in file "+f+" ... Aborting for this sample.")
                doVLQSignalSumWeights = False
                continue

            couplings = [0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6]
            
            for i in range(40):
                #
                weight_name = "sumOfWeights_"
                if i == 33:
                    weight_name += "nom_mass_K100"
                    weightDict[weight_name] = 1.0
                elif i > 33:
                    weight_name += "nom_mass_K"+str(int(couplings[i%20]*100))
                    weightDict[weight_name] = h_vlqSumOfWeights.GetBinContent(i)
                else:
                    if i < 20:
                        weight_name += "low_mass_K"+str(int(couplings[i]*100))
                    else:
                        weight_name += "nom_mass_K"+str(int(couplings[i%20]*100))
                    weightDict[weight_name] = h_vlqSumOfWeights.GetBinContent(i+1)


    if jsonOutput:
        configDict[dsid] = {'crossSection': crossSection, 'nWeightedEvents': nEventsWeighted}

        if doSysWeights_temp:
            configDict[dsid].update(weightDict)
        if doVLQSignalSumWeights:
            configDict[dsid].update(weightDict)

    else:
        configFile.write(dsid+" "+`nEventsWeighted`+" "+`crossSection`+"\n")

    if crossSection < 0:
        printError("<!> The sample "+ SName +" is not in the cross-section file. Please check ! The cross-section is set to -1.")
        continue

if jsonOutput:
    configFile.write(json.dumps(configDict,sort_keys=True,indent=4))

configFile.close()

if isolateBadFiles and len(badFileList)>0:

    if inputDir[-1] == '/':
        isolationDir = inputDir[:-1] + '_isolatedFiles/'
    else:
        isolationDir = inputDir + '_isolatedFiles/'

    try:
        if os.system('mkdir -p %s'%isolationDir) != 0:
            raise Exception('OSError')
    except:
        printError("<!> Unable to create isolation directory. Please manually make sure the follow bad files are not included in VLQAnalysis inputs:")
        for badfile in badFileList:
            print badfile

    else:
        printGoodNews("--> Moving %s bad file(s) to %s"%(len(badFileList),isolationDir))
        for badfile in badFileList:
            os.system('mv %s %s'%(badfile,isolationDir))

##________________________________________________________
## Removing list folder
os.system("rm -rf " + listFolder) #list files folder
##........................................................
