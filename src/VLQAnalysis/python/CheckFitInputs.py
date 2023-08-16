import os, sys
import subprocess
import argparse

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/IFAETopFramework/" ) 
import CheckOutputs

sys.path.append( os.getenv("VLQAnalysisFramework_DIR") + "/python/VLQAnalysis/" ) 
import LaunchAllCouplingReweightings

baseDir="/data/at3/scratch2/"+os.getenv("USER")+"/VLQAnalysisRun2/"

parser = argparse.ArgumentParser()

parser.add_argument("--inDirBase", 
                    help="Basic pattern of the input directories **relative to baseDir**, \
                    to which channel, process and mc_campaign names are added \
                    to make the full directory path", 
                    required=True)
parser.add_argument("--date_string", 
                    help="Optional date string in the input directory pattern, \
                    to restrict searches to directories produced on a particular date")
parser.add_argument("--channel",
                    help="0lep/1lep", required=True)
parser.add_argument("--processes",
                    help="comma-separated list of processes",
                    default="data,pvlq,ttbar,singletop,Wjets,Zjets,diboson,topEW,dijet,ttbar_alt,singletop_alt")
parser.add_argument("--mc_campaigns",
                    help="comma-separated list of mc campaigns", default="mc16a,mc16d,mc16e")
parser.add_argument("--do_check",
                    help="check if all input files exist", action='store_true')
parser.add_argument("--relaunch",
                    help="relaunch failed jobs", action='store_true')
parser.add_argument("--reweight_signals",
                    help="launch BR reweighting on signals", action='store_true')
parser.add_argument("--link_files",
                    help="make soft links of files to the output directory", action='store_true')
parser.add_argument("--input_suffix",
                    help="link files whose names contain the given suffix", default='')
parser.add_argument("--outDir", 
                    help="Relative path (from baseDir) to output directories")
parser.add_argument("--do_syst",
                    help="include systematics", action='store_true')


args = parser.parse_args()
#print(args)

if(args.link_files):
    subprocess.Popen("mkdir -p {}/nominal/".format(args.outDir), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    subprocess.Popen("mkdir -p {}/ttstalt/".format(args.outDir), shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

process_list = (args.processes).split(',')
campaign_list = (args.mc_campaigns).split(',')

for process in process_list:
    for campaign in campaign_list:

        print (process + '; ' + campaign)
        dirPattern = "{}_{}_{}_{}".format(args.inDirBase,process,args.channel,campaign)
        if not(args.date_string is None or args.date_string == ""): dirPattern += "_"+args.date_string
        foundDirs = ""
        try:
            #foundDirs = subprocess.check_output("ls {} |grep {}".format(baseDir,dirPattern), shell=True, check=True)
            foundDirs = subprocess.check_output("ls {} |grep {}".format(baseDir,dirPattern), shell=True)
        except:
            print("Directory search for {} produces error. Exiting".format(dirPattern))
            exit(1)

        dirList = foundDirs.split()
        relaunch = "TRUE" if(args.relaunch) else "FALSE"
        if (len(dirList) > 1):
            print("WARNING: More than one directory matching pattern {} has been found. \
            Please check. ****** No jobs will be relaunched ****** ".format(dirPattern))
            relaunch = "FALSE"
        for subDir in dirList:
            inDir = "{}/{}/".format(baseDir,subDir)
            if(args.do_check):
                scriptFile="{}/Scripts_Analysis_{}/JobCheck.chk".format(inDir,subDir)
                CheckOutputs.main(['input={}'.format(scriptFile), 'relaunch={}'.format(relaunch)])

            if(process=='pvlq' and args.reweight_signals):

                #rw_opts = ['--inputDir='+inDir, '--outputDir='+args.outDir+'/nominal/', 
                #           '--fileSuffix='+args.input_suffix,
                #           '--mcCampaign='+campaign, '--doBatch', 'queue=at3', '--doSR', '--doPR']

                #,
                rw_opts = ['--inputDir',inDir, '--outputDir',args.outDir+'/nominal/', '--fileSuffix',args.input_suffix,
                           '--mcCampaign',campaign, '--doBatch', '--queue','at3', '--doSR', '--doPR' ]

                if(not args.do_syst): 
                    rw_opts.append('--statOnly')
                if(args.channel=='1lep'):
                    rw_opts.append('--doLepton')
                if(args.channel=='0lep'):
                    rw_opts.append('--doZeroLepton')

                LaunchAllCouplingReweightings.main(rw_opts)

            #Now link to the outputDirectory
            if(args.link_files):
                outDir = args.outDir
                outDir = outDir + "/ttstalt/" if(process=="ttbar_alt" or process=="singletop_alt" ) \
                         else outDir + "/nominal/"
                #link_inDir = inDir + '/reweighted/' if(process=='pvlq') else inDir
                subprocess.Popen("ln -s {}/outVLQAna_*{}.root {}/.".format(inDir,args.input_suffix,outDir), 
                                 shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            #if(args.do_merge):
            #    PrepareInputFilesTRexFitter.main('inputDir={}'.format(outDir), 'useData=')

#python ../macros/macros_stats/PrepareInputFilesTRexFitter.py inputDir=$VLQSCRATCH/VLQAnalysisOutputs_Wjets_Sh2.2.11_meff600GeV_validn_WITHSYST_svlq_mc16a_2023_03_02_1804/ useData=false singletopSyst=false useBkgd=true useSystematics=false splitCampaigns=true signal=NONE splitSingletop=true mergeSingletop=false mergeRareBkg=false outputDir=$VLQSCRATCH/VLQAnalysisOutputs_Wjets_Sh2.2.11_meff600GeV_validn_WITHSYST_svlq_mc16a_2023_03_02_1804/FilesTRexF/
