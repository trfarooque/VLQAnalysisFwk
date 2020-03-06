import os
import string
import time, getpass
import socket
import sys
import datetime
from VLQ_Samples_mc import *

#sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "python/IFAETopFramework/" )
from BatchTools import *
from Job import *
from Samples import *

##______________________________________________________________________________
## Defines some useful variables
platform = socket.gethostname()
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
here = os.getcwd()
##..............................................................................

##______________________________________________________________________________
## OPTIONS
nFilesSplit = 20 #number of files to merge in a single run of the code (chaining them)
nMerge = 10 #number of merged running of the code in a single job
channels = ["./.",]
sleep = 1
param_campaign = ""
param_removeNull = False

#Options to control individual submission of different types of samples
param_runQCD = True
param_runData = True
param_runTOPQ1Data = True
param_runTOPQ4Data = True

param_runSignal = True
param_runTtbar = True
param_runOtherBkgd = True
param_runSingleTop = True
param_runWjets = True
param_runZjets = True
param_runTopEW = True
param_runDibosons = True
param_runTtSyst = False
param_runSTSyst = False
param_splitSTChannels = False

param_runSingleVLQ = True
param_runPairVLQ = False
param_run4tops = False
param_runUEDRPP = False

param_useSlices = True
param_useObjectSyst = False
param_useWeightSyst = False
param_onlyDumpSystHistos = False
param_produceTarBall = True
param_tarballPath = ""
param_dryRun = False
param_batch = "condor"
param_queue = "at3_short"
param_inputDir="/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.67-htztx-0-MV2/" #place where to find the input files
param_outputDir = "/nfs/at3/scratch2/"+os.getenv("USER")+"/VLQAnalysisRun2/VLQAnalysisOutputs_" #output repository
param_outputDirSuffix = "NOW"
userParams = []
if(len(sys.argv))>1:
    for x in sys.argv[1:]:
        splitted=x.split("=")
        if(len(splitted)!=2):
            printError("<!> The argument \"" + x + "\" has no equal signs ... Please check")
            sys.exit(-1)
        argument = splitted[0].upper().replace("--","")
        value = splitted[1]
        if(argument=="CAMPAIGN"):
            param_campaign = value
        elif(argument=="NFILESPLIT"):
            nFilesSplit = int(value)
        elif(argument=="NMERGE"):
            nMerge = int(value)
        elif(argument=="SLEEP"):
            sleep = int(value)
        elif(argument=="REMOVENULL"):
            param_removeNull = (value.upper()=="TRUE")
        elif(argument=="RUNDATA"):
            param_runData = (value.upper()=="TRUE")
        elif(argument=="RUNQCD"):
            param_runQCD = (value.upper()=="TRUE")
        elif(argument=="RUNTOPQ1DATA"):
            param_runTOPQ1Data = (value.upper()=="TRUE")
        elif(argument=="RUNTOPQ4DATA"):
            param_runTOPQ4Data = (value.upper()=="TRUE")
        elif(argument=="RUNSIGNALS"):
            param_runSignal = (value.upper()=="TRUE")
        elif(argument=="RUNTTBAR"):
            param_runTtbar = (value.upper()=="TRUE")
        elif(argument=="RUNOTHERBKGD"):
            param_runOtherBkgd = (value.upper()=="TRUE")
        elif(argument=="RUNSINGLETOP"):
            param_runSingleTop = (value.upper()=="TRUE")
        elif(argument=="SIMPLESINGLETOP"):
            param_simpleSingletop = (value.upper()=="TRUE")
        elif(argument=="RUNTCHAN"):
            param_runTchan = (value.upper()=="TRUE")
        elif(argument=="RUNWTPROD"):
            param_runWtprod = (value.upper()=="TRUE")
        elif(argument=="RUNSCHAN"):
            param_runSchan = (value.upper()=="TRUE")
        elif(argument=="RUNWJETS"):
            param_runWjets = (value.upper()=="TRUE")
        elif(argument=="RUNZJETS"):
            param_runZjets = (value.upper()=="TRUE")
        elif(argument=="RUNTOPEW"):
            param_runTopEW = (value.upper()=="TRUE")
        elif(argument=="RUNDIBOSONS"):
            param_runDibosons = (value.upper()=="TRUE")
        elif(argument=="RUNTTSYST"):
            param_runTtSyst = (value.upper()=="TRUE")
        elif(argument=="RUNSTSYST"):
            param_runSTSyst = (value.upper()=="TRUE")
        elif(argument=="SPLITSTCHANNELS"):
            param_splitSTChannels = (value.upper()=="TRUE")

        elif(argument=="RUNSINGLEVLQ"):
            param_runSingleVLQ = (value.upper()=="TRUE")
        elif(argument=="RUNPAIRVLQ"):
            param_runPairVLQ = (value.upper()=="TRUE")
        elif(argument=="RUN4TOPS"):
            param_run4tops = (value.upper()=="TRUE")
        elif(argument=="RUNUEDRPP"):
            param_runUEDRPP = (value.upper()=="TRUE")

        elif(argument=="USESLICES"):
            param_useSlices = (value.upper()=="TRUE")
        elif(argument=="USEOBJECTSYST"):
            param_useObjectSyst = (value.upper()=="TRUE")
        elif(argument=="USEWEIGHTSYST"):
            param_useWeightSyst = (value.upper()=="TRUE")
        elif(argument=="ONLYDUMPSYSTHISTOS"):
            param_onlyDumpSystHistos = (value.upper()=="TRUE")
        elif(argument=="PRODUCETARBALL"):
            param_produceTarBall = (value.upper()=="TRUE")
        elif(argument=="TARBALLPATH"):
            param_tarballPath = value
        elif(argument=="DRYRUN"):
            param_dryRun = (value.upper()=="TRUE")
        elif(argument=="BATCH"):
            param_batch = value
        elif(argument=="QUEUE"):
            param_queue = value
        elif(argument=="INPUTDIR"):
            param_inputDir = value
        elif(argument=="OUTPUTDIR"):
            param_outputDir = value
        elif(argument=="OUTPUTDIRSUFFIX"):
            param_outputDirSuffix = value
        else:
            userParams += [{'arg':argument,'value':value}]
else:
    printError("<!> No arguments seen ... Aborting !")
    sys.exit(-1)

param_outputDirSuffix = param_outputDirSuffix.replace("NOW",now)
param_outputDir += param_outputDirSuffix
##..............................................................................


##______________________________________________________________________________
## Printing the options
print "param_campaign = ", param_campaign
print "nFilesSplit = ", nFilesSplit
print "nMerge = ", nMerge
print "param_removeNull = ", param_removeNull

print "param_runData = ", param_runData
print "param_runTOPQ1Data = ", param_runTOPQ1Data
print "param_runTOPQ4Data = ", param_runTOPQ4Data

print "param_runSignal = ", param_runSignal
print "param_runTtbar = ", param_runTtbar
print "param_runOtherBkgd = ", param_runOtherBkgd
print "param_runSingleTop = ", param_runSingleTop
print "param_runWjets = ", param_runWjets
print "param_runZjets = ", param_runZjets
print "param_runTopEW = ", param_runTopEW
print "param_runDibosons = ", param_runDibosons
print "param_runTtSyst = ", param_runTtSyst
print "param_runSTSyst = ", param_runSTSyst
print "param_splitSTChannels = ", param_splitSTChannels

print "param_runSingleVLQ = ", param_runSingleVLQ
print "param_runPairVLQ = ", param_runPairVLQ
print "param_run4tops = ", param_run4tops
print "param_runUEDRPP = ", param_runUEDRPP

print "param_useSlices = ", param_useSlices
print "param_useObjectSyst = ", param_useObjectSyst
print "param_useWeightSyst = ", param_useWeightSyst
print "param_onlyDumpSystHistos = ", param_onlyDumpSystHistos
print "param_produceTarBall = ", param_produceTarBall
print "param_tarballPath = ", param_tarballPath
print "param_dryRun = ", param_dryRun
print "param_inputDir = ", param_inputDir
print "param_outputDir = ", param_outputDir
for param in userParams:
    print param['arg'] + " = " + param['value']
##..............................................................................

##______________________________________________________________________________
## Defining the paths and the tarball
listFolder = param_outputDir + "/Lists_Analysis_" + param_outputDir.split("/")[len(param_outputDir.split("/"))-1] #name of the folder containing the file lists
scriptFolder = param_outputDir + "/Scripts_Analysis_" + param_outputDir.split("/")[len(param_outputDir.split("/"))-1] #name of the folder containing the scripts
##..............................................................................

##______________________________________________________________________________
## Creating usefull repositories
os.system("mkdir -p " + param_outputDir) #output files folder
os.system("mkdir -p " + listFolder) #list files folder
os.system("mkdir -p " + scriptFolder) #script files folder
##..............................................................................

##______________________________________________________________________________
## Creating tarball
tarballPath=""
if param_produceTarBall:
    tarballPath = param_outputDir + "/AnaCode_forBatch.tgz"
    if not (os.path.exists(tarballPath) or param_dryRun):
        prepareTarBall(here+"/../../",tarballPath)
        prepareTarBall(os.getenv("VLQAnalysisFramework_DIR")+"/../../",tarballPath)
else:
    tarballPath = param_tarballPath


##..............................................................................

##______________________________________________________________________________
## Getting all samples and their associated weight/object systematics
##
Samples = []
if param_runQCD :
   Samples += GetQCDSamples(  )
if param_runData :
    if param_runTOPQ1Data:
        Samples += GetDataSamples( data_type = "TOPQ1" )
    if param_runTOPQ4Data:
        Samples += GetDataSamples( data_type = "TOPQ4" )

if param_runTtbar:
    Samples += GetTtbarSamples ( useWeightSyst = param_useWeightSyst, useObjectSyst = param_useObjectSyst, 
                                 ttbarSystSamples = param_runTtSyst, hfSplitted = True, 
                                 useHTSlices = param_useSlices, campaign = param_campaign )

if param_runOtherBkgd:
    Samples += GetOtherBackgroundSamples ( useWeightSyst = param_useWeightSyst, useObjectSyst = param_useObjectSyst, campaign=param_campaign
                                           , includeSingleTop = param_runSingleTop, simple = param_simpleSingletop
                                           , TChan = param_runTchan, Wtprod = param_runWtprod, Schan = param_runSchan
                                           , includeWjets = param_runWjets, includeZjets = param_runZjets
                                           , includeTopEW = param_runTopEW, includeDibosons = param_runDibosons
                                           , includeSingletopSystSamples = param_runSTSyst
                                           , splitSTChannels = param_splitSTChannels, removeNull = param_removeNull)

if param_runSignal:
    Samples += GetSignalSamples( useWeightSyst = param_useWeightSyst, useObjectSyst = param_useObjectSyst, campaign = param_campaign
                                 , includeSingleVLQ = param_runSingleVLQ, includePairVLQ = param_runPairVLQ
                                 , include4tops = param_run4tops)

printGoodNews("--> All samples recovered")
##..............................................................................

##______________________________________________________________________________
## Loop over channels
printGoodNews("--> Performing the loop over samples and jobs")
nJobs = 0
for channel in channels:

    ##__________________________________________________________________________
    ## Loop over samples
    for sample in Samples:

        SName = sample['name'] # sample name
        SType = sample['sampleType'] # sample type (first argument of the getSamplesUncertainties())

        excluded = []
        joblist = getSampleJobs(sample,InputDir=param_inputDir+"/"+channel+"/",NFiles=nFilesSplit,UseList=False,ListFolder=listFolder,exclusions=excluded,useDiffFilesForObjSyst=False)
        if(not joblist):
            continue

        #Setting caracteristics for the JobSet object
        JOSet = JobSet(platform)
        JOSet.setBatch(param_batch)
        JOSet.setScriptDir(scriptFolder)
        JOSet.setLogDir(param_outputDir)
        JOSet.setTarBall(tarballPath)#tarball sent to batch (contains all executables)
        JOSet.setJobRecoveryFile(scriptFolder+"/JobCheck.chk")
        JOSet.setQueue(param_queue)

        setCom=""
        setCom+="rcSetup Base,2.4.43 \n "
        JOSet.setSetUpCommand(setCom)
        ##______________________________________________________________________
        ## Loop over jobs for this sample (multiple files or systematics)
        for iJob in range(len(joblist)):

            ## Declare the Job object (one job = one code running once)
            jO = Job(platform)

            ## Name of the executable you want to run
            jO.setExecutable("VLQAna")
            jO.setDebug(False)

            name  = SType
            name += "_" + SName
            name += "_"+joblist[iJob]['objSyst']+"_"+`iJob` #name of the job

            jO.setName(name)

            # Settings of the jobs (inputs, outputs, ...)
            OFileName = "out"+jO.execName+"_"+name+".root"
            jO.addOption("outputFile",OFileName) #name of the output file
            jO.addOption("inputFile",joblist[iJob]['filelist']) #name of the input file (already got from ls)
            jO.addOption("textFileList","false")
            jO.addOption("sampleName",sample['sampleType'])

            # Weight systematics
            if param_useWeightSyst and joblist[iJob]['objSyst'].upper()=="NOMINAL" and SType.upper().find("DATA")==-1:
                jO.addOption("computeWeightSys","true")
            else:
                jO.addOption("computeWeightSys","false")

            jO.setOutDir(param_outputDir)
            jO.addOption("sampleID",SName)

            # Input tree name (might change for systematics)
            jO.addOption("inputTree",joblist[iJob]['objectTree'])

            # isData flag
            if(sample['sampleType'].upper().find("DATA")>-1):
                jO.addOption("isData","true")
            else:
                jO.addOption("isData","false")

            #Adding further options set by the user
            for param in userParams:
                jO.addOption(param['arg'],param['value'])

            ## SKIPPING ALREADY PROCESSED FILES
            if(os.path.exists(param_outputDir+"/"+OFileName)):
                printWarning("=> Already processed: skipping")
                continue

            JOSet.addJob(jO)

            if ( JOSet.size()==nMerge or joblist[iJob]['objSyst'].upper()=="NOMINAL" ):
                JOSet.writeScript()
                if not param_dryRun:
                    JOSet.submitSet()
                JOSet.clear()

        if(JOSet.size()>0):
            JOSet.writeScript()
            if not param_dryRun:
                JOSet.submitSet()
            JOSet.clear()
        os.system("sleep "+`sleep`)
##..............................................................................
