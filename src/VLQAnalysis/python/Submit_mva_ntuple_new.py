# Script copied from Submit_VLQAnalysis_new.py slightly changes to allow running the MVA ntuple making 
# This is a very dirty script and a first implementation that I don't have time to fix properly in the immediate future
# The workflow is as follows
# 1) Go under VLQ/python and copy all you find under https://gitlab.cern.ch/orlando/HBSMutilsMVA/tree/master/Root
# 2) Change the options param_configFile, param_inputDir according to your needs
# 3) Run it as python Submit_mva_ntuple_new.py --OUTPUTDIRSUFFIX=test_bdt 
# 4) Pray 

import os
import string
import time, getpass
import socket
import sys
import datetime
from VLQ_Samples import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "python/IFAETopFramework/" )
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
channels = ["./.",]
sleep = 1
param_produceTarBall = True
param_dryRun = False
param_queue = "at3"
param_inputDir="/nfs/at3/scratch2/orlando/VLQAnalysisRun2/VLQ_hbsm_2017-04-10_dt_v0_merged_test/" #place where to find the input files
param_outputDir = "/nfs/at3/scratch2/"+os.getenv("USER")+"/VLQAnalysisRun2/BDT_" #output repository
param_configFile = "config/run_config.conf"
param_codeDir = "VLQAnalysis/python"

print "Using input directory %s",param_inputDir
print "Using output directory %s",param_outputDir

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
        if(argument=="SLEEP"):
            sleep = int(value)
        elif(argument=="PRODUCETARBALL"):
            param_produceTarBall = (value.upper()=="TRUE")
        elif(argument=="DRYRUN"):
            param_dryRun = (value.upper()=="TRUE")
        elif(argument=="QUEUE"):
            param_queue = value
        elif(argument=="INPUTDIR"):
            param_inputDir = value
        elif(argument=="OUTPUTDIR"):
            param_outputDir = value
        elif(argument=="OUTPUTDIRSUFFIX"):
            param_outputDirSuffix = value
        elif(argument=="CONFIGFILE"):
            param_configFile = value
        elif(argument=="CODEDIR"):
            param_codeDir = value 
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
print "param_produceTarBall = ", param_produceTarBall
print "param_dryRun = ", param_dryRun
print "param_inputDir = ", param_inputDir
print "param_outputDir = ", param_outputDir
print "param_configFile = ", param_configFile
print "param_codeDir = ", param_codeDir

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
# Makes assumptions about where you run it
tarballPath=""
if param_produceTarBall:
    tarballPath = param_outputDir + "/MVACode_forBatch.tgz"
    if not os.path.exists(tarballPath):
        prepareTarBall(here+"/../../",tarballPath)
##..............................................................................

##_______________________________________________________________________________________________________________________________
## Using the nice IFAE code features to prepare the script and run all the files listed in the input directory: how cool is that? 
printGoodNews("--> Performing the loop over samples and jobs")
nJobs = 0

for filename in os.listdir(param_inputDir):
    if filename.endswith(".root") :
        
        JOSet = JobSet(platform)
        JOSet.setScriptDir(scriptFolder)
        JOSet.setLogDir(param_outputDir)
        JOSet.setTarBall(tarballPath)#tarball sent to batch (contains all executables)
        JOSet.setJobRecoveryFile(scriptFolder+"/JobCheck.chk")
        JOSet.setQueue(param_queue)

        jO = Job(platform)
        name  = "script_"+filename
        jO.setName(name)        
        jO.setOutDir(param_outputDir)

        command_to_run='cd %s\n root -l -b -q TMVAHBSM.C\(\\"%s\\",\\"%s\\"\)'%(param_codeDir,param_configFile,param_inputDir+filename)
        print command_to_run
        jO.setExecutable(command_to_run)
        
        jO.setDebug(False)
            
        JOSet.addJob(jO)
            
        JOSet.writeScript()
        JOSet.submitSet()
                
        os.system("sleep "+`sleep`)

JOSet.clear()
##..............................................................................
