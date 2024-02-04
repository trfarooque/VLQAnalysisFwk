import os
import string
import time, getpass
import socket
import sys
import datetime
import argparse

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/")
from VLQ_Samples_mc import *

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "python/IFAETopFramework/" )
from BatchTools import *
from Job import *
from Samples import *

##______________________________________________________________________________
## Defines some useful variables
platform = socket.gethostname()
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
##..............................................................................

#### Some common lists of attributes ######
allowed_campaigns = ['mc16a', 'mc16d', 'mc16e']

bkg_procs = ['ttbar','singletop','Wjets','Zjets','diboson','topEW','dijet']
bkg_sys_procs = ['ttbar_alt','singletop_alt']
all_procs = ['data','pvlq','svlq'] + bkg_procs + bkg_sys_procs
allowed_procs = all_procs + ['bkg','bkg_sys']

##______________________________________________________________________________
## OPTIONS

parser = argparse.ArgumentParser()

parser.add_argument('--nFilesSplit', help='Number of files by which jobs are split', default=250)
parser.add_argument('--nMerge', help='Number of jobs merged in each job set', default=1)
parser.add_argument('--sleep', help='Sleep time between each job submission', default=2)
parser.add_argument('--removeNull', help='If set, remove samples that are known to have zero input events ', 
                    action='store', default=0)
parser.add_argument('--inputDir', help='Input directory containing ntuples', required=True)
parser.add_argument('--outputDir', help='Output directory where histogram/mini-tree files will be stored', 
                    default="/data/at3/scratch2/"+os.getenv("USER")+"/VLQAnalysisRun2/VLQAnalysisOutputs_")
parser.add_argument('--outputDirSuffix', help='Suffix to add to name of output directory', default='NOW')
parser.add_argument('--useProcSubdirs', help='Input files will be found in sub-directories of the \
input directory when this is set', action='store', type=int, default=1)
##Arguments that will also be passed to VLQAna
parser.add_argument('--useTtbarHtSlices', help='Use HT slices for ttbar sample when set', action='store', type=int, default=1)
parser.add_argument('--mode', help='Specify what type of job to run and set corresponding set of options', 
                    choices=['FITINPUTS','MVAINPUTTREES','DATAMC','MVAVALIDATION','FULLFITVALIDATION',
                             'DERIVEREWEIGHTING', 'DERIVEMULTIJET'], default='DATAMC')
parser.add_argument('--vlqOptString', help='extra options to send to VLQAna', default='')
parser.add_argument('--rewriteTarball', help='Code tarball will be rewritten when set', action='store', type=int, default=1)
parser.add_argument('--tarballPath', help='Path to code tarball', default='')


###Common arguments used by all/many parts of processing chain
parser.add_argument('--campaigns', help='MC campaigns to launch', nargs='*', 
                    choices=allowed_campaigns, default=allowed_campaigns)
parser.add_argument('--channel', help='Lepton channel', choices=['0lep','1lep'], required=True)
parser.add_argument('--processes', help='Space-separated list of sample processes', nargs='*', 
                    choices=allowed_procs, default=all_procs)
parser.add_argument('--useSyst', help='Run systematics when set', action='store', type=int, default=0)

parser.add_argument('--dryRun', help='Do not launch jobs when set ', action='store', type=int, default=0)
parser.add_argument('--batch', help='Type of batch system', default='condor')
parser.add_argument('--queue', help='Batch queue to which jobs are submittied', default='at3_short')



args = parser.parse_args()

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

##_________________________________________
##Sample data to be passed to vlqOptions - process dependent
sampleDatPattern="samples_info/pvlq/"

##______________________________________________________________________________
##Make list of options to be passed to VLQAna
vlqOptions = {}

##Default options
vlqOptions['applydeltaphicut'] = 'TRUE'
vlqOptions['applyMetMtwCuts'] = 'TRUE'
vlqOptions['useMETTrigger'] = 'TRUE'
vlqOptions['minMeffCut'] = '1000'
vlqOptions['doKinRWSmoothing'] = 'TRUE'
vlqOptions['lowMVACutOneLep'] = '0.17'
vlqOptions['highMVACutOneLep'] = '0.80'
vlqOptions['lowMVACutZeroLep'] = '0.90'
vlqOptions['highMVACutZeroLep'] = '0.95'

vlqOptions['doLooseSystRegions'] = 'FALSE'
vlqOptions['doExtendedPreselection'] = 'FALSE'

vlqOptions['doExpSys'] = 'TRUE'
vlqOptions['doTheorySys'] = 'TRUE'
vlqOptions['doKinRWSyst'] = 'TRUE'

vlqOptions['doExpSys'] = 'TRUE'

##Channel-dependent options
if args.channel=='1lep':
    sampleDatPattern += "1L/samples_info.tag-21.2.213-htztx-syst__SYSTSUFFIX__1L"
    vlqOptions['doOneLeptonAna'] = 'TRUE'
    vlqOptions['doZeroLeptonAna'] = 'FALSE'
    if args.mode=='DERIVEREWEIGHTING':
        vlqOptions['doTwoLeptonAna'] = 'TRUE'
    vlqOptions['useLeptonsSF'] = 'TRUE'
    vlqOptions['useLeptonTrigger'] = 'TRUE'
    vlqOptions['useMETTriggerOneLep'] = 'TRUE'
    vlqOptions['mvaWeightFile'] = 'TMVA/weightsCV_1L_30vars_allbkgd/TMVAClassificationCV_MLP.weights.xml'

elif args.channel=='0lep':
    sampleDatPattern += "0L/samples_info.tag-21.2.213-htztx-syst__SYSTSUFFIX__0L"
    vlqOptions['doOneLeptonAna'] = 'FALSE'
    vlqOptions['doZeroLeptonAna'] = 'TRUE'
    vlqOptions['useLeptonsSF'] = 'FALSE'
    vlqOptions['useLeptonTrigger'] = 'FALSE'
    vlqOptions['useMETTriggerOneLep'] = 'FALSE'
    vlqOptions['doSplitMET'] = 'TRUE'
    vlqOptions['mvaWeightFile'] = 'TMVA/weightsCV_0L_22vars_allbkgd/TMVAClassificationCV_MLP.weights.xml'

if args.mode=='DERIVEREWEIGHTING':
    vlqOptions['deriveReweighting'] = 'TRUE'
else:
    vlqOptions['deriveReweighting'] = 'FALSE'

if args.mode=='DERIVEMULTIJET':
    vlqOptions['invertMetMtwCuts'] = 'TRUE'
    vlqOptions['minDeltaPhiCut'] = '0.'
    vlqOptions['maxDeltaPhiCut'] = '0.1'
else:
    vlqOptions['invertMetMtwCuts'] = 'FALSE'
    vlqOptions['minDeltaPhiCut'] = '0.4'
    vlqOptions['maxDeltaPhiCut'] = '-1'

if args.mode=='MVAINPUTTREES':
    vlqOptions['makeMVAInputTree'] = 'TRUE'
else:
    vlqOptions['dumpHistos'] = 'TRUE'
    vlqOptions['applyMVA'] = 'TRUE' ## Also means training regions will be added

if args.mode=='DATAMC' or args.mode=='MVAVALIDATION':
    vlqOptions['makeMVAInputHists'] = 'TRUE'
else:
    vlqOptions['makeMVAInputHists'] = 'FALSE'


if args.mode=='FITINPUTS' or args.mode=='FULLFITVALIDATION':
    vlqOptions['doFitRegions'] = 'TRUE'
else:
    vlqOptions['doFitRegions'] = 'FALSE'

if args.mode=='FITINPUTS' or args.mode=='FULLFITVALIDATION' or args.mode=='MVAVALIDATION':
    vlqOptions['onlyDumpSystHistograms'] = 'TRUE'
else:
    vlqOptions['onlyDumpSystHistograms'] = 'FALSE'

    if args.mode=='FITINPUTS':
        vlqOptions['otherVariables'] = 'FALSE'
        vlqOptions['doPreselectionRegions'] = 'FALSE'
        vlqOptions['doPreselSys'] = 'FALSE'
    else:
        vlqOptions['otherVariables'] = 'TRUE'
        vlqOptions['doPreselectionRegions'] = 'TRUE'
        vlqOptions['doPreselSys'] = 'TRUE'

if args.useTtbarHtSlices:
    vlqOptions['scaleTtbarHtSlices'] = 'TRUE'
    vlqOptions['filterType'] = 'APPLYFILTER'
    
# Capitalise all keys in vlqOptions for clean overrides:
for k in vlqOptions.keys():
    vlqOptions[k.upper()]=vlqOptions.pop(k)

for opt in args.vlqOptString.split():
    splitted = opt.split('=')
    if(len(splitted)!=2):
        printError("<!> The argument \"" + opt + "\" has no equal signs ... Please check")
        sys.exit(-1)
    argument = splitted[0].upper().replace('--','')
    value = splitted[1]

    vlqOptions[argument] = value ### Also overwrites pre-set options with new value if provided in vlqOptString

##______________________________________________________________________________
## Print all options
print "############################################################"
print " Command-line options : "
for argument in vars(args):
    if( argument != 'vlqOptString' ):
        print argument," : ",getattr(args,argument)
for arg,value in vlqOptions.items():
    print arg + " = " + value

print "############################################################"

##..............................................................................

##______________________________________________________________________________
## Set all paths

outputDir  = args.outputDir

if(args.outputDirSuffix != ""): 
    outputDir += args.outputDirSuffix
outputDir = outputDir.replace("NOW", now)
print 'outputDir : ',outputDir
os.system("mkdir -p " + outputDir) #output files folder

tarballPath = args.tarballPath
if(tarballPath == ''):
    tarballPath = outputDir + "/AnaCode_forBatch.tgz"

##..............................................................................
##..............................................................................

##______________________________________________________________________________
## Creating tarball
##The code directory should be tarred if and only if this is not a dry run, and either 
##the provided path does not exist or we explicitly want it to be overwritten

if(not(args.dryRun) and (args.rewriteTarball or not(os.path.exists(tarballPath)))):
    print 'producing tarball'
    prepareTarBall(os.getenv("VLQAnalysisFramework_DIR")+"/../../",tarballPath)
    print 'produced tarball'

##..............................................................................
###### FUNCTION DEFINITIONS BEGIN
##..............................................................................

##______________________________________________________________________________
## Submit jobs for specific sample and campaign
##
def SubmitSampleJob(sampleName, mc_campaign):

    Samples = retrieveSamples(sampleName, mc_campaign, dataChannel=args.channel, 
                              useSyst=args.useSyst,
                              useTtbarHtSlices=args.useTtbarHtSlices)

    sample_inputDir = args.inputDir
    sample_outputDir = outputDir

    if(args.useProcSubdirs):
        sample_inputDir += "/"+sampleName+"/"
        sample_outputDir += "/"+sampleName+"/"

    sample_listDir = sample_outputDir + "/Lists_Analysis/"
    sample_scriptsDir = sample_outputDir + "/Scripts_Analysis/"

    os.system("mkdir -p " + sample_listDir) #list files directory
    os.system("mkdir -p " + sample_scriptsDir) #script files directory

    for sample in Samples:
        SName = sample['name'] # sample name
        SType = sample['sampleType'] # sample type (first argument of the getSamplesUncertainties())

        excluded = []
        print "GetSampleJobs; SName : ",SName,"; SType : ",SType
        joblist = getSampleJobs(sample,
                                InputDir=sample_inputDir, NFiles=int(args.nFilesSplit),UseList=False,
                                ListFolder=sample_listDir,exclusions=[],
                                useDiffFilesForObjSyst=False)
        if(not joblist):
            continue

        #Setting caracteristics for the JobSet object
        JOSet = JobSet(platform)
        JOSet.setBatch(args.batch)
        JOSet.setScriptDir(sample_scriptsDir)
        JOSet.setLogDir(sample_outputDir)
        JOSet.setTarBall(tarballPath)#tarball sent to batch (contains all executables)
        JOSet.setJobRecoveryFile(sample_scriptsDir+"/JobCheck.chk")
        JOSet.setQueue(args.queue)

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
            if args.useSyst and joblist[iJob]['objSyst'].upper()=="NOMINAL" \
               and sampleName!='data' and sampleName!='ttbar_alt' and sampleName!='singletop_alt':
                jO.addOption("computeWeightSys","true")
            else:
                jO.addOption("computeWeightSys","false")

            jO.setOutDir(sample_outputDir)
            jO.addOption("sampleID",SName)

            # Input tree name (might change for systematics)
            if joblist[iJob]['objSyst']=="JET_JMR__1up":
                ### JMR uncertainty is calculated from nominal tree ####
                jO.addOption("inputTree","nominal")
                jO.addOption("doJMRSys","TRUE")
            else:
                jO.addOption("inputTree",joblist[iJob]['objectTree'])

            # isData flag
            if(sample['sampleType'].upper().find("DATA")>-1):
                jO.addOption("isData","true")
            else:
                jO.addOption("isData","false")

            # sample info
            if(sampleName == 'singletop_alt'):
                sampleDatPatternTemp = sampleDatPattern.replace("__SYSTSUFFIX__","-STOPALT-")
                if(("410646"in SName) or ("410647"in SName) or ("410644" in SName) or ("410645" in SName) or ("410658" in SName) or ("410659" in SName)):
                    jO.addOption("isAFII", "true")
                elif(("410654" in SName) or ("410655" in SName)):
                    jO.addOption("isDiagSub", "true")
            elif(sampleName == 'ttbar_alt'):
                sampleDatPatternTemp = sampleDatPattern.replace("__SYSTSUFFIX__","-TTBARALT-")
                if(("407342" in SName) or ("407343" in SName) or ("407344" in SName) or ("410470" in SName)):
                    jO.addOption("isAFII", "true")

            else:
                sampleDatPatternTemp = sampleDatPattern.replace("__SYSTSUFFIX__","-")

            if(args.channel == '1lep'):
                jO.addOption("sampleDat", sampleDatPatternTemp+"."+mc_campaign+".dat")
            else:
                jO.addOption("sampleDat", sampleDatPatternTemp+"."+sampleName+"."+mc_campaign+".dat")

            for arg,value in vlqOptions.items():
                jO.addOption(arg, value)
                if(arg.upper() == 'REWEIGHTKINEMATICS'):
                    if(sampleName == 'ttbar' or sampleName == 'singletop' or 
                       sampleName == 'ttbar_alt' or sampleName == 'singletop_alt'):
                        jO.addOption('KINRWLIST','JETSN,MEFFRED')
                    elif(sampleName == 'Wjets' or sampleName == 'Zjets'):
                        jO.addOption('KINRWLIST','JETSN')
                    
            ## SKIPPING ALREADY PROCESSED FILES
            if(os.path.exists(sample_outputDir+"/"+OFileName)):
                printWarning("=> Already processed: skipping")
                continue

            JOSet.addJob(jO)

            if ( JOSet.size()==int(args.nMerge) or joblist[iJob]['objSyst'].upper()=="NOMINAL" ):
                JOSet.writeScript()
                if not args.dryRun:
                    JOSet.submitSet()
                JOSet.clear()

        if(JOSet.size()>0):
            JOSet.writeScript()
            if not args.dryRun:
                JOSet.submitSet()
            JOSet.clear()

        os.system("sleep "+`args.sleep`)

    return 

##..............................................................................
###### FUNCTION DEFINITIONS END
##..............................................................................


##__________________________________________________________________________
## Loop over samples

printGoodNews("--> Performing the loop over samples and campaigns")
nJobs = 0

#### Loop over campaigns for MC samples
for proc_name in procs_to_run: 
    print 'Process : ',proc_name
    if proc_name == 'data':
        SubmitSampleJob(proc_name, '')
    else:
        for mc_campaign in args.campaigns:
            SubmitSampleJob(proc_name, mc_campaign)

##..............................................................................

##..............................................................................


