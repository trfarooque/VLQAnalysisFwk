#!/bin/python
import sys
import os
import string
import time, getpass
import socket
import datetime
import json
import numpy as np
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
cut_thr=0.5
pmg_cut_thr = 0.005 # Currently applying a 0.1% quantile threshold to all pmg weights
##........................................................

##________________________________________________________
## Defines some useful variables
platform = socket.gethostname()
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()
##........................................................

##________________________________________________________
## Defining the paths and the tarball
inputDir="/data/at3/scratch2/tvdaalen/VLQ_FullRun2/Production_tag-21.2.87-htztx-3-syst/" 
# "nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.67-htztx-0-fJvt-DL1.PFlow/"
listFolder=here+"/Lists_Analysis_" + now
histogramName="cut_flow"
mccampaign=""
configName = ""
doSysWeights = False
prunePMGWeights = False
doVLQSignalSumWeights = False
newVLQSamples = False
pruneVLQWeights = False
useDefaultVLQCrossSection = False
badFileList = []

param_runTtbar = False
param_runOtherBkgd = False
param_runSingleTop = False
param_runWjets = False
param_runZjets = False
param_runTopEW = False
param_runDibosons = False
param_runDijet = False
param_runTtSyst = False
param_runSTSyst = False
param_runSingleVLQ = False
param_runPairVLQ = False

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
        elif(argument=="PRUNEPMGWEIGHTS"):
            if splitted[1].upper() == "TRUE":
                prunePMGWeights = True
                # prunePMGWeights is False by default
        elif(argument=="DOVLQSIGNALSUMWEIGHTS"):
            if splitted[1].upper()=="TRUE":
                doVLQSignalSumWeights = True
                # doVLQSignalSumWeights is False by default
        elif(argument=="NEWVLQSAMPLES"):
            if splitted[1].upper()=="TRUE":
                newVLQSamples = True
                # newVLQSamples is False by default
        elif(argument=="PRUNEVLQWEIGHTS"):
            if splitted[1].upper()=="TRUE":
                pruneVLQWeights = True
                # newVLQSamples is False by default
        # This option changes the cross section for every sample in the list provided.
        # Use caution, this should only be used when generating a .dat file for VLQ signal samples ONLY
        elif(argument=="USEDEFAULTVLQXS"):
            if splitted[1].upper()=="TRUE":
                useDefaultVLQCrossSection = True
                # useDefaultVLQCrossSection is False by default
        elif(argument=="RUNTTBAR"):
            param_runTtbar = (value.upper()=="TRUE")
        elif(argument=="RUNSINGLETOP"):
            param_runSingleTop = (value.upper()=="TRUE")
            param_runOtherBkgd = (value.upper()=="TRUE")
        elif(argument=="RUNWJETS"):
            param_runWjets = (value.upper()=="TRUE")
            param_runOtherBkgd = (value.upper()=="TRUE")
        elif(argument=="RUNZJETS"):
            param_runZjets = (value.upper()=="TRUE")
            param_runOtherBkgd = (value.upper()=="TRUE")
        elif(argument=="RUNTOPEW"):
            param_runTopEW = (value.upper()=="TRUE")
            param_runOtherBkgd = (value.upper()=="TRUE")
        elif(argument=="RUNDIBOSONS"):
            param_runDibosons = (value.upper()=="TRUE")
            param_runOtherBkgd = (value.upper()=="TRUE")
        elif(argument=="RUNDIJET"):
            param_runDijet = (value.upper()=="TRUE")
            param_runOtherBkgd = (value.upper()=="TRUE")
        elif(argument=="RUNTTSYST"):
            param_runTtbar = (value.upper()=="TRUE")
            param_runTtSyst = (value.upper()=="TRUE")
        elif(argument=="RUNSTSYST"):
            param_runSingleTop = (value.upper()=="TRUE")
            param_runSTSyst = (value.upper()=="TRUE")
            param_runOtherBkgd = (value.upper()=="TRUE")
        elif(argument=="RUNSINGLEVLQ"):
            param_runSingleVLQ = (value.upper()=="TRUE")
        elif(argument=="RUNPAIRVLQ"):
            param_runPairVLQ = (value.upper()=="TRUE")
        elif(argument=="RUNALLBKGD"):
            param_runTtbar = True
            param_runOtherBkgd = True
            param_runSingleTop = True
            param_runWjets = True
            param_runZjets = True
            param_runTopEW = True
            param_runDibosons = True
            param_runDijet = True



if not configName:
    configName = "sample_info_"+mccampaign+".dat"

##........................................................
#print "useDefaultVLQCrossSection : ", useDefaultVLQCrossSection
#print "doVLQSignalSumWeights : ", doVLQSignalSumWeights

##________________________________________________________
## Creating useful repositories
os.system("mkdir -p " + listFolder) #list files folder
##........................................................

##________________________________________________________
## Getting all samples and their associated weight/object systematics
Samples = []
if param_runTtbar:
    Samples += GetTtbarSamples(hfSplitted=False,ttbarSystSamples=param_runTtSyst,useHTSlices=True,campaign=mccampaign)
if param_runOtherBkgd:
    Samples += GetOtherBackgroundSamples (campaign=mccampaign,
                                          includeSingleTop=param_runSingleTop, 
                                          includeWjets=param_runWjets, 
                                          includeZjets=param_runZjets, 
                                          includeTopEW=param_runTopEW,
                                          includeDibosons=param_runDibosons, 
                                          includeDijet=param_runDijet,
                                          includeSingletopSystSamples=param_runSTSyst, 
                                          splitSTChannels=False,
                                          includeTchan=True, 
                                          includeWtprod=True, 
                                          includeSchan=True,
                                          removeNull=False, sVLQAna=False)

if param_runSingleVLQ or param_runPairVLQ:
    Samples += GetSignalSamples( campaign = mccampaign,
                                 includeSingleVLQ = param_runSingleVLQ, 
                                 includePairVLQ = param_runPairVLQ)

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
#with open("/data/atlas-data07/tvdaalen/SingleVLQWorkArea/br_R21_master_Oct2019/VLQAnalysis/python/xsec_ttbar_dummy.json",'r') as f:
    weights = json.load(f)

##________________________________________________________
## Loop over samples
printGoodNews("--> Performing the loop over samples")
usedSNames = []
for sample in Samples:

    SName = sample['name'] # sample name
    SType = sample['sampleType'] # sample type

    #print 'SName : '+SName+' SType : '+SType
    #Use each SName (a.k.a DSID) only once
    if (SName in usedSNames):
        #print SName+' already in usedList'
        continue
    usedSNames.append(SName)
    iDir = inputDir
    excluded = []

    joblist = getSampleJobs(sample,InputDir=iDir+"/",NFiles=nFilesSplit,UseList=False,ListFolder=listFolder,exclusions=excluded,useDiffFilesForObjSyst=False)

    if( not joblist or len(joblist)!=1):
        printError("<!> The job for sample "+SName+" isn't standard: please check")
        continue

    files = joblist[0]['filelist'].split(",")
    nEvents = 0
    nEventsWeighted = 0
    crossSection = -1
    dsid = SName
    defaultVLQCrossSection = 0.1 # default VLQ XS set to 0.1pb

    ####### Only needed for weight pruning ######
    if(pruneVLQWeights):
        weights = {}
        weights_trimmed = {}
        Nev = 0
        for ii in range(1,41):
            weights[ii] = []
        
    ############################################

    #### Only needed for PMG weight pruning ####
    pmg_weight_names = []
    pmg_weights = {}
    pmg_weight_sum = {}
    pmg_weight_sum_trimmed = {}

    if(prunePMGWeights):
        root_f_tmp = TFile(files[0], "read")
        for branch in root_f_tmp.Get("nominal").GetListOfBranches():
            branch_name = branch.GetName()
            if("weight_pmg" in branch_name and "nominal" not in branch_name):
                pmg_weight_names.append(branch_name)
                pmg_weights[branch_name] = []
                pmg_weight_sum[branch_name] = 0
        root_f_tmp.Close()
    ############################################



    weightDict = {}
    doSysWeights_temp = doSysWeights
    prunePMGWeights_temp = prunePMGWeights
    if SType not in ['ttbar','Singletop','Wjets','Zjets','Dibosons']:
        doSysWeights_temp = False
        if(SType != 'ttbar'):
            prunePMGWeights_temp = False

    file_counter = 0
    files = [f for f in files if f.find("migrated") == -1]

    for f in files:
        if(file_counter%50==0):
            print("Processed %d files out of %d for sample %s."%(file_counter, len(files), SName))
        file_counter += 1

        root_f = TFile(f,"read")

        if(root_f.IsZombie()):
            printError("<!> The file "+f+" is corrupted ... removed it from the list")
            badFileList.append(f)
            continue
        if not pruneVLQWeights:
            try:
                h_nEvents = root_f.Get(histogramName).Clone()
                h_nEvents.SetDirectory(0)
            except ReferenceError:
                printError("<!> No "+histogramName+" histogram found in file "+f+" ... removed it from the list")
                badFileList.append(f)
                continue

            nEvents += h_nEvents.GetBinContent(1)
            nEventsWeighted += h_nEvents.GetBinContent(2)
            d_SampleInfo = weights.get(SName.replace("."+mccampaign,""))
            if d_SampleInfo:
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
                    
            if prunePMGWeights_temp:
                try:
                    nominalTree = root_f.Get("nominal")
                except ReferenceError:
                    printError("<!> No 'nominal' tree found in file "+f+" ... Aborting for this sample.")
                    continue

                N = nominalTree.GetEntries()
                for entry in range(N):
                    nominalTree.GetEntry(entry)
                        
                    for pmg_weight_name in pmg_weight_names:
                        current_weight = getattr(nominalTree, pmg_weight_name)
                        pmg_weights[pmg_weight_name].append(current_weight)
                        pmg_weight_sum[pmg_weight_name] += current_weight

                
        #####################################
        if doVLQSignalSumWeights:

            if not pruneVLQWeights:

                #### Get the weight histogram ###
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
                #################################
                if newVLQSamples:
                    nweights = h_vlqSumOfWeights.GetNbinsX()
                    for  i in range(1,nweights):
                        weight_name = "sumOfWeights_" + h_vlqSumOfWeights.GetXaxis().GetBinLabel(i)  
                        weight_name = weight_name.replace('K0','K')
                        sumWeights = h_vlqSumOfWeights.GetBinContent(i+1)

                        if not weightDict.has_key(weight_name):
                            weightDict[weight_name] = sumWeights
                        else:
                            weightDict[weight_name] += sumWeights
                else:
                    couplings = [0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6]
            
                    for i in range(40):
                #
                        weight_name = "sumOfWeights_"
                        sumWeights = 0
                        if i == 33:
                            weight_name += "nom_mass_K100"
                            #sumWeights = nEvents
                            sumWeights = h_nEvents.GetBinContent(1)
                        else:
                            if i > 33:
                                weight_name += "nom_mass_K"+str(int(couplings[i%20]*100))
                                sumWeights = h_vlqSumOfWeights.GetBinContent(i) #Skipped nom_mass_K100
                            else:
                                sumWeights = h_vlqSumOfWeights.GetBinContent(i+1)
                                if i < 20:
                                    weight_name += "low_mass_K"+str(int(couplings[i]*100))
                                else:
                                    weight_name += "nom_mass_K"+str(int(couplings[i%20]*100))
                    
                        if not weightDict.has_key(weight_name):
                            weightDict[weight_name] = sumWeights
                        else:
                            weightDict[weight_name] += sumWeights
                ###############################
            else:
                try:
                    eventInfoTree = root_f.Get("eventInfo")
                except ReferenceError:
                    printError("<!> No 'eventInfo' tree found in file "+f+" ... Aborting for this sample.")
                    doVLQSignalSumWeights = False
                    continue

                N = eventInfoTree.GetEntries()
                for entry in range(N):
                    eventInfoTree.GetEntry(entry)
                    for ii in range(1,41):
                        weights[ii].append(eventInfoTree.weights[ii])
                    Nev += 1
                    #if Nev % 10000 == 0: #and debug:
                    #    print("{} events done".format(Nev))

                for key in weights.keys():
                    weights[key] = np.array(weights[key])
                    #print key," : ",weights[key]


                couplings = [0,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6]
                for i in range(1,41):
                #
                    weight_name = "sumOfWeights_"
                    threshold_name = "threshold_"
                    mkname = ''
                    if i < 21:
                        mkname = "low_mass_K"+str(int(couplings[i]*100))
                    else:
                        mkname += "nom_mass_K"+str(int(couplings[i%20]*100))

                    weight_name += mkname
                    threshold_name += mkname
                    #print weight_name, " : ", i
                    #print weights[i].sum()
                    weights_trimmed[i] = np.copy(weights[i])
                    thr = np.percentile(weights_trimmed[i], 100.- cut_thr)
                    indices = weights_trimmed[i] > thr
                    weights_trimmed[i][indices] = 0.
                    #print thr," : ",weights_trimmed[i].sum()

                    weightDict[weight_name] = weights_trimmed[i].sum()
                    weightDict[threshold_name] = thr

#####################################
 
    if prunePMGWeights_temp:

        for pmg_weight_name in pmg_weight_names:
            pmg_weight_sum_trimmed[pmg_weight_name] = np.copy(pmg_weights[pmg_weight_name])

            pmg_weight_thr = np.percentile(pmg_weight_sum_trimmed[pmg_weight_name], 100.- pmg_cut_thr)
            pruning_mask = pmg_weight_sum_trimmed[pmg_weight_name] > pmg_weight_thr
            pmg_weight_sum_trimmed[pmg_weight_name][pruning_mask] = 0.
            
            weightDict["sumOfWeights_"+pmg_weight_name] = nEventsWeighted*pmg_weight_sum_trimmed[pmg_weight_name].sum()/pmg_weight_sum[pmg_weight_name]
            weightDict["threshold_"+pmg_weight_name] = pmg_weight_thr 


    # This flag changes the cross section for all samples included in the list of samples.
    if useDefaultVLQCrossSection:
        crossSection = defaultVLQCrossSection
        
    if jsonOutput:
        configDict[dsid] = {'crossSection': crossSection, 'nWeightedEvents': nEventsWeighted}

        if doSysWeights_temp or doVLQSignalSumWeights:
            configDict[dsid].update(weightDict)

    else:
        configFile.write(f"{dsid} {nEventsWeighted} {crossSection}\n")

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
            print( badfile)

    else:
        printGoodNews("--> Moving %s bad file(s) to %s"%(len(badFileList),isolationDir))
        for badfile in badFileList:
            os.system('mv %s %s'%(badfile,isolationDir))

##________________________________________________________
## Removing list folder
os.system("rm -rf " + listFolder) #list files folder
##........................................................
