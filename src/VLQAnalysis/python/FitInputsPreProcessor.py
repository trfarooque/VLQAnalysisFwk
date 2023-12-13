import os, sys
import subprocess
import argparse
import datetime

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/" ) 
import CheckOutputs

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" ) 
import LaunchAllCouplingReweightings
import PrepareInputFilesTRexFitter
import MakeSingletopSyst
import MakeVjetsSyst

##### Actions to perform on the histogram files #######
'''
- CheckOutputs.py --> check if all jobs succeeded and relaunch if any have failed
- LaunchAllCouplingReweightings --> reweight the VLQ signals
- Extrapolate single top systematics
- Extrapolate QCD background (TO BE IMPLEMENTED)
- Link all files
- Merge all files
'''


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


##------------------------------------------------------
## Checking the arguments
##------------------------------------------------------

parser = argparse.ArgumentParser()

### Steps to run #######
parser.add_argument("--checkOutputs",
                    help="check if all input files exist", type=int, action='store', default=1)
parser.add_argument("--relaunchFailedJobs",
                    help="relaunch failed jobs", type=int, action='store', default=1)
parser.add_argument("--linkFiles",
                    help="make soft links of files to the output directory", 
                    action='store',type=int, default=0)
parser.add_argument("--mergeFiles",
                    help="make merged files for TRexFitter/IFAEPlotter processing in the output directory", 
                    action='store', type=int, default=0)

parser.add_argument("--reweightVLQ",
                    help="launch BR reweighting on VLQ signals", 
                    action='store', type=int, default=0)
parser.add_argument('--doAllBRPVLQ', help='Run all branching ratios for pvlq samples', 
                    action='store', type=int, default=0)
parser.add_argument("--extrapolateSingletopSyst",
                    help="extrapolate single top uncertainties from loose regions",
                    action='store',type=int, default=0)
parser.add_argument("--extrapolateQCD",
                    help="extrapolate single top uncertainties from loose regions",
                    action='store',type=int, default=0)
parser.add_argument("--makeEnvelopeVjetsSyst",
                    help="make envelope of weight variations for V+jets samples",
                    action='store',type=int, default=0)


parser.add_argument("--inputSuffix",
                    help="only use files whose names contain the given suffix", default='')
parser.add_argument('--inputDir', help='Input directory containing histogram files', required=True)
parser.add_argument('--outputDir', 
                    help='Output directory where TRexFitter/IFAEPlotter inputs will be stored', 
                    required=True)

###Common arguments used by all/many parts of processing chain
parser.add_argument('--campaigns', help='MC campaigns to launch', nargs='*',
                    choices=allowed_campaigns, default=allowed_campaigns)
parser.add_argument('--channel', help='Lepton channel', choices=['0lep','1lep'], required=True)
parser.add_argument('--processes', help='Space-separated list of sample processes', nargs='*',
                    choices=allowed_procs, default=all_procs)
parser.add_argument('--useSyst', help='Run systematics when set', 
                    action='store', type=int, default=0)
parser.add_argument("--otherVariables",help="Do variables other than meff and MVAScore",
                    action="store", type=int, default=0)
parser.add_argument('--dryRun', help='Do not launch jobs when set ', 
                    action='store', type=int, default=0)
parser.add_argument('--batch', help='Type of batch system', default='condor')
parser.add_argument('--queue', help='Batch queue to which jobs are submittied', default='at3_short')



args = parser.parse_args()
#print(args)

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

##______________________________________________________________________________
## Print all options
print "############################################################"
print " Command-line options : "
for argument in vars(args):
    print argument," : ",getattr(args,argument)
print "############################################################"

print '===== **** BEGIN PRE-PROCESSING CHAIN **** ====='
#........................................................................
print '=====>> Check Outputs ====='
if args.checkOutputs:

    relaunch = "TRUE" if(args.relaunchFailedJobs) else "FALSE"
    nRelaunched = 0
    for proc_name in procs_to_run: 
        print "proc: ",proc_name
        sample_inputDir = args.inputDir + '/'+proc_name+'/'

        scriptFile="{}/Scripts_Analysis/JobCheck.chk".format(sample_inputDir)
        nRelaunched += CheckOutputs.main(['input={}'.format(scriptFile), 'relaunch={}'.format(relaunch)])

    if(nRelaunched>0):
        print nRelaunched,' jobs were relaunched in total'
        print ' ****** WARNING ----> Re-run chain after relaunched jobs have succeeded ******'
        sys.exit()

print '===== Check Outputs <<====='
    
#........................................................................
print '=====>> Link Files ====='
if(args.linkFiles):
    subprocess.Popen("mkdir -p {}/nominal/".format(args.outputDir), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    subprocess.Popen("mkdir -p {}/ttstalt/".format(args.outputDir), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    for proc_name in procs_to_run: 
        print proc_name
        sample_inputDir = args.inputDir + '/'+proc_name+'/'
        sample_outputDir = args.outputDir + '/ttstalt/' if(proc_name=='ttbar_alt' or proc_name=='singletop_alt') \
                      else args.outputDir + '/nominal/'

        sample_campaigns = [''] if(proc_name=='data') else args.campaigns

        for campaign in sample_campaigns:
            subprocess.Popen("ln -s {}/outVLQAna_*{}*{}.root {}/.".\
                             format(sample_inputDir,campaign,args.inputSuffix,sample_outputDir), 
                             shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

print '===== Link Files <<====='

#........................................................................
print '=====>> Merge Files ====='
if(args.mergeFiles):

    mergeOpts = ['--inputDir', args.outputDir, '--outputDir', args.outputDir+'/MergedFiles/', 
                 '--useSyst', str(args.useSyst), 
                 '--mergeSingleTop', '1', '--dryRun', str(args.dryRun)]
    mergeOpts.append('--campaigns')
    for cmpgn in args.campaigns:
        mergeOpts.append(cmpgn)
    mergeOpts.append('--processes')
    for proc in args.processes:
        mergeOpts.append(proc)

    PrepareInputFilesTRexFitter.main(mergeOpts)

print '===== Merge Files <<====='

#........................................................................
print '=====>> Reweight VLQ ====='
if args.reweightVLQ:

    #'--fileSuffix',args.input_suffix,
    for campaign in args.campaigns:
        rw_opts = ['--inputDir',args.outputDir+'/MergedFiles/nominal/', 
                   '--outputDir',args.outputDir+'/MergedFiles/nominal/', 
                   '--mcCampaign',campaign, '--postMerging', '1', '--doBatch', 
                   '--queue','at3', '--doSR', '--doPR', '--nMerge','10' ]

        if(not args.otherVariables):
            rw_opts.append('--tthfitter')
        if(not args.useSyst): 
            rw_opts.append('--statOnly')
        if(args.channel=='1lep'):
            rw_opts.append('--doLepton')
        if(args.channel=='0lep'):
            rw_opts.append('--doZeroLepton')
        if(args.dryRun):
            rw_opts.append('--debug')
        LaunchAllCouplingReweightings.main(rw_opts)

print '===== Reweight VLQ <<====='

#........................................................................
print '=====>> Extrapolate single-top syst ====='
if( (args.extrapolateSingletopSyst>0) and 
    ( (args.useSyst and ('singletop' in procs_to_run)) 
      or ('singletop_alt' in procs_to_run) ) ):

    for campaign in args.campaigns:

        if(args.useSyst):
            #extrapolate weight uncertainties
            wsyst_opts = ['--inputDir',args.outputDir+'/MergedFiles/nominal/', 
                          '--outputDir',args.outputDir+'/MergedFiles/Extrapolated/nominal/', 
                          '--campaign',campaign, '--doNominal','1', '--doSR','1', '--doPresel','1',
                          '--doWeightSys','1', '--doAltSys','0'] 
            if(args.otherVariables):
                wsyst_opts.append('--otherVariables')
            if(args.channel=='1lep'):
                wsyst_opts.append('--doLepton')
            if(args.channel=='0lep'):
                wsyst_opts.append('--doZeroLepton')
            MakeSingletopSyst.main(wsyst_opts)

        if 'singletop_alt' in procs_to_run:
            #extrapolate alt samples
            asyst_opts = ['--inputDir',args.outputDir+'/MergedFiles/ttstalt/', 
                          '--outputDir',args.outputDir+'/MergedFiles/Extrapolated/ttstalt/', 
                          '--campaign',campaign, '--doNominal','1', '--doSR','1', '--doPresel','1',
                          '--doWeightSys','0', '--doAltSys','1'] 
            if(args.otherVariables):
                asyst_opts.append('--otherVariables')
            if(args.channel=='1lep'):
                asyst_opts.append('--doLepton')
            if(args.channel=='0lep'):
                asyst_opts.append('--doZeroLepton')

            MakeSingletopSyst.main(asyst_opts)

print '===== Extrapolate single-top syst <<====='

#........................................................................
print '=====>> Make V+jets syst envelope ====='
if args.useSyst and args.makeEnvelopeVjetsSyst:

    for campaign in args.campaigns:
        #extrapolate weight uncertainties
        vjets_opts = ['--inputDir',args.outputDir+'/MergedFiles/nominal/', 
                      '--outputDir',args.outputDir+'/MergedFiles/Extrapolated/nominal/', 
                      '--doSR','1', '--doPresel','0' ]
        if(args.otherVariables):
            vjets_opts.append('--otherVariables')
        if(args.channel=='1lep'):
            vjets_opts.append('--doLepton')
        if(args.channel=='0lep'):
            vjets_opts.append('--doZeroLepton')

        vjets_opts.append('--sample')
        if('Wjets' in procs_to_run):
            vjets_opts.append('Wjets.'+campaign)
            MakeVjetsSyst.main(vjets_opts)
            vjets_opts.pop()
        if('Wjets' in procs_to_run):
            vjets_opts.append('Zjets.'+campaign)
            MakeVjetsSyst.main(vjets_opts)

print '===== Make V+jets syst envelope <<====='
#........................................................................
if args.extrapolateQCD:
    pass
