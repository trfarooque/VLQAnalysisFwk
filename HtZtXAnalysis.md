# Steps to run the statistical analysis for the Ht/Zt+X single VLQ search

The following file will take you through the steps needed to run the full Ht/Zt+X analysis, 
starting from the production of histogram fit inputs, intermediate steps required to prepare 
the inputs in the expected structure, preparation of the configuration files and launching the 
background fits.

The signal interpretation is a work in progress and information on this will be added as we go along

## Preparing inputs


### Producing the histograms
VLQAnalysis will produce the required histograms for fitting and plotting, with ntuples produced by 
MBJ_Analysis as inputs. A few submit scripts have been added which will produce these histograms with 
the correct settings. Some points to note:
  * The ntuples containing theory systematic weights (colloquiually, PMG weights) were produced separately 
from the bulk ntuple production, and therefore the instrumental uncertainties and theory uncertainties 
have to be run separately
  * Since AFII and FullSim samples have the same DSID and the same SampleName as far as VLQAnalysis is concerned, 
the alternative samples required to compute theory uncertainties for ttbar and single top samples are stored 
in a separate directory, along with the nominal AFII sample for these processes. 
  * sample_info.dat files for each ntuple input directory is stored in the VLQAnalysis/data directory
  * The relevant ntuple input directories and their corresponding sample_info.dat files are as follows:
    * /nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst/nominal/ [Nominal samples; all instrumental systematics]
      * samples_info.tag-21.2.87-htztx-3-syst.mc*.dat
    * /nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst-VJETSSYST/ [V+jets nominal samples; all PMG weights]
      * samples_info.tag-21.2.87-htztx-3-syst-VJETSSYST.mc*.dat
    * /nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst-STOPSYST/ [single-top nominal samples; all PMG weights]
      * samples_info.tag-21.2.87-htztx-3-syst-STOPSYST.mc*.dat
    * /nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.87-htztx-3-syst/alt_samples_v2/
      * samples_info_ttst_alt.tag-21.2.87-htztx-3-syst.mc*.dat

To produce the fit inputs and not overwhelm the batch system in the process, the jobs have to be split into groups.
The job submission is broken down by process, and a set of submission scripts with the correct options are provided 
in the VLQAnalysis/python directory:
  * submit_FitInputs_DATA.sh: Run over data
  * submit_FitInputs_TTBAR.sh: Run over ttbar. Turn the option --reweightKinematics=TRUE/FALSE to produce histograms 
    with or without the data-driven kinematic reweighting applied
  * submit_FitInputs_SINGLETOP.sh: Run over singletop. Turn the option --reweightKinematics=TRUE/FALSE to produce histograms 
    with or without the data-driven kinematic reweighting applied.
    * NOTE: Singletop processes are split into s-channel, t-channel, and Wt subprocesses. This splitting is necessary 
      when single-top modelling uncertainties are considered since the Diagram Subtraction variation of Wt production is 
      one of the required uncertainties. However, for reasons of statistics, all the subprocesses are merged together in 
      a later step
    * NOTE: --doTheorySys is FALSE for this submission, since the PMG weights are not available in the nominal directory (see above)
  * submit_FitInputs_WJETS.sh: Run over W+jets. Turn the option --reweightKinematics=TRUE/FALSE to produce histograms 
    with or without the data-driven kinematic reweighting applied.
    * NOTE: --doTheorySys is FALSE for this submission, since the PMG weights are not available in the nominal directory (see above)
  * submit_FitInputs_ZJETS.sh: Run over Z+jets. Turn the option --reweightKinematics=TRUE/FALSE to produce histograms 
    with or without the data-driven kinematic reweighting applied.
    * NOTE: --doTheorySys is FALSE for this submission, since the PMG weights are not available in the nominal directory (see above)
  * submit_FitInputs_OTHERS.sh: Run over topEW+dibosons+ttH. The --reweightKinematics option is irrelevant for these processes.
  * submit_FitInputs_SIGNAL.sh: Run over the signal single-VLQ files

  * submit_FitInputs_VJETSSYST: Run over W+jets and Z+jets samples from the VJETSSYST input directory, with *only* theory systematics.
    Therefore, these samples are run with --useWeightSys=TRUE --useObjectSys=FALSE --doExpSys=FALSE --doTheorySys=TRUE
  * submit_FitInputs_STOPSYST: Run over singletop samples from the STOPSYST input directory, with *only* theory systematics.
    Therefore, these samples are run with --useWeightSys=TRUE --useObjectSys=FALSE --doExpSys=FALSE --doTheorySys=TRUE
  * submit_FitInputs_TTSTALT: Run over alternative ttbar and single-top samples from the alt_samples_v2 directory. These will be 
    compared to the nominal samples to derive theory uncertainties. 
    Therefore, these samples are run with --useWeightSys=FALSE --useObjectSys=FALSE --doExpSys=FALSE --doTheorySys=FALSE
    Also, since the nominal ttbar and Wt samples in this directory are AFII, --isAFII=TRUE in this run (this does not affect the 
    reweighting of alternative generators)   

After launching the jobs, check the batch intermittently to track completion. Once the queue is clear *it is very important that 
you explicitly check whether all files were produced*, using `IFAETopFramework/python/CheckOutputs.py`. This is run as:
`python IFAETopFramework/python/CheckOutputs.py input=[output_directory]/Scripts_[output_directory]/JobCheck.chk`
where output_directory is the output directory created by Submit_VLQAnalysis_new.py during the job submission. After seeing if any 
jobs failed and understanding the cause, one can add the option `relaunch=True`, which will automatically relaunch the jobs that 
failed (default is False).


### Merging outputs
At present, the configuration file templates for TRexFitter expect a certain structure for the fit inputs, and it is easiest if 
everyone maintains this structure. It is also useful to maintain links with the original histogram output directory, because one 
can quickly trace back the log files and code used to produce them in case debugging is required. Therefore:
  * Make a fresh directory, with any name, e.g. FitInputs
  * Soft link all of the histograms from the *nominal* run into FitInputs
  * Make the following subdirectories and soft link the appropriate files in there: 
    * FitInputs/TheorySys: Soft link all files from the VJETSYST run and STOPSYST run in here
    * FitInputs/TTSTALT: Soft link all files from the TTSTALT run in here

Once all files are correctly soft linked, they can be merged using the script provided in `VLQAnalysis/macros/macros_stat/`

Nominal samples:
`python ../macros/macros_stats/PrepareInputFilesTRexFitter.py inputDir=FitInputs/ useData=false singletopSyst=false useBkgd=true useSystematics=true splitCampaigns=true signal=NONE splitSingletop=true mergeSingletop=true outputDir=FitInputs/FilesTRexF/`

PMG weights samples:
`python ../macros/macros_stats/PrepareInputFilesTRexFitter.py inputDir=FitInputs/TheorySys/ useData=false singletopSyst=false useBkgd=true useSystematics=false splitCampaigns=true signal=NONE splitSingletop=true mergeSingletop=true outputDir=$VLQSCRATCH/FitInputs/FilesTRexF/TheorySys/`

Alternative generator samples:
`python ../macros/macros_stats/PrepareInputFilesTRexFitter.py inputDir=FitInputs/TtStAlt/ useData=false ttbarSyst=true singletopSyst=true useBkgd=false useSystematics=false splitCampaigns=true signal=NONE splitSingletop=true mergeSingletop=true outputDir=FitInputs/FilesTRexF/TtStAlt/ singletopNominalDir=FitInputs/FilesTRexF/`

### Adding parametrised uncertainties
The modelling (theory) uncertainties for single-top processes are parametrised as a function of meff in looser source regions and propagated to the signal 
regions, due to lack of statistics in the SRs. On the other hand, for V+jets samples, an envelope of the individual scale variations has to be created. 
Scripts are provided for both of these steps:

Single-top uncertainty propagation:
`python VLQAnalysis/python/SystRegionHistoScript.py --inputDir=FitInputs/FilesTRexF/TheorySys/ --inputDirAlt=FitInputs/FilesTRexF/TtStAlt/`

V+jets uncertainty envelope (has to be done sample by sample):
```
for sample in "Wjets" "Zjets"
do
    for	campaign in "mc16a" "mc16d" "mc16e"
    do
        python MakeVjetsSyst.py --inputDir=FitInputs/FilesTRexF/TheorySys/ --outputDir=FitInputs/FilesTRexF/TheorySys/NewVJets/ \
            --sample=${sample}.${campaign} --doSR=1 --doVR=1
    done
done
```

## Make configuration files
The default regions_dictionary that should be used is `VLQAnalysis/python/regions_dictionary_sVLQ.py`. Should you need to make a 
regions dictionary for testing purposes or to change the configuration of regions in the fit, this can be done through 
`VLQAnalysis/python/MakeRegionsDictionary.py`. 

The configuration files for TRexFitter should be made with ```VLQAnalysis/macros/macros_stats/PrepareConfigFilesFromTemplate.py```
Three default configurations are given below:

Asimov B-only fit:
```
MACRODIR=VLQAnalysis/macros/macros_stats
TEMPLATEDIR=${MACRODIR}/templates/
python ${MACRODIR}/PrepareConfigFilesFromTemplate.py \
inputTemplate=${TEMPLATEDIR}/TEMPLATE_configFile_sVLQ__SIGNAL_.txt \
outputFolder=$SCRATCHDIR/VLQFITINPUTS/VLQ2018/sVLQ/tag21.2.87-3-syst/FitSysBlind_incSTSys/ \
systConfig=${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_Exp.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Objects.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_ttMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_ttbar.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_singletopMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_singletop.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_Vjets.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_BackNorm.txt \
signal=ONE signalType=SINGLE doOneLep=True doZeroLep=false doSR=true doVR=true doPresel=false useData=false \
regions=regions_dictionary_sVLQ \
inputDir=FitInputs/FilesTRexF/ \
signalScaling="BENCHMARK" \
useBlindingCuts=FALSE
```

Asimov B-only blinded fit:
```
MACRODIR=VLQAnalysis/macros/macros_stats
TEMPLATEDIR=${MACRODIR}/templates/
python ${MACRODIR}/PrepareConfigFilesFromTemplate.py \
inputTemplate=${TEMPLATEDIR}/TEMPLATE_configFile_sVLQ__SIGNAL_.txt \
outputFolder=$SCRATCHDIR/VLQFITINPUTS/VLQ2018/sVLQ/tag21.2.87-3-syst/FitSysBlind_incSTSys/ \
systConfig=${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_Exp.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Objects.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_ttMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_ttbar.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_singletopMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_singletop.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_Vjets.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_BackNorm.txt \
signal=ONE signalType=SINGLE doOneLep=True doZeroLep=false doSR=true doVR=true doPresel=false useData=false \
regions=regions_dictionary_sVLQ \
inputDir=FitInputsCR/FilesTRexF/ \
signalScaling="BENCHMARK" \
useBlindingCuts=TRUE
```

Data blinded fit:
```
MACRODIR=VLQAnalysis/macros/macros_stats
TEMPLATEDIR=${MACRODIR}/templates/
python ${MACRODIR}/PrepareConfigFilesFromTemplate.py \
inputTemplate=${TEMPLATEDIR}/TEMPLATE_configFile_sVLQ__SIGNAL_.txt \
outputFolder=$SCRATCHDIR/VLQFITINPUTS/VLQ2018/sVLQ/tag21.2.87-3-syst/FitSysBlind_incSTSys/ \
systConfig=${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_Exp.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Objects.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_ttMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_ttbar.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_singletopMod.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_singletop.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_Weights_pmg_Vjets.txt,${TEMPLATEDIR}/TEMPLATE_Systematics_BackNorm.txt \
signal=ONE signalType=SINGLE doOneLep=True doZeroLep=false doSR=true doVR=true doPresel=false useData=true \
regions=regions_dictionary_sVLQ \
inputDir=FitInputsCR/FilesTRexF/ \
signalScaling="BENCHMARK" \
useBlindingCuts=TRUE FitBlind=FALSE
```

## Launch the fits
The current version of TRexFitter to use is TRExFitter-00-04-08. A tarball with this version is available on at3:
```/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz```

`VLQAnalysis/macros/macros_stats/LaunchTRexFitterOnBatch_newTRexF.py` will help you launch TRexFitter jobs on the batch system.
Here is an example set of commands:
```
dirBase=Full_Path_To_Directory_With_Config_Files
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=fitpreplotspostplots
fitinputDir="${dirBase}/configFile_"
fitoutputDir="${dirBase}/Results/"

python ../../../src/VLQAnalysis/macros/macros_stats/LaunchTRExFitterOnBatch_newTRexF.py \
    tarball=${tarpath} \
    action=${fitactions} \
    inputDir=${fitinputDir} outputDir=${fitoutputDir}/ \
    queue=at3_short
```

