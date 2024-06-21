## Work in progress

# VLQAnalysis Wrapper

This is the CMake wrapper around the VLQAnalysis framework. This is meant to make it significantly easier to check out 
all required packages to make the code work.

## Code setup

### Checking Out
Checkout the package and all nested submodules with
`git clone --recursive ssh://git@gitlab.cern.ch:7999/htx/VLQAnalysisFramework.git`

### Setting up a release and compiling for the first time

```
mkdir build
cd build
setupATLAS && asetup AnalysisBase,21.2.87
cmake ../src
make
source */setup.sh
cd ../
```

### Setting up the code from a new terminal
```
cd build
setupATLAS && asetup --restore
source */setup.sh
```

## Code structure 

The VLQAnalysis package is equipped to run over ntuples created by MBJ_Analysis, which is a SUSYTools-based package. 
MBJ_Analysis will already have applied calibrations and CP-recommended selections to physics objects, in addition to 
storing event-level nominal and systematics weights, performing a minimal skimming to events, etc. 
VLQAnalysis will analyse these ntuples and apply further kinematic selections to objects, perform the final event selection 
and categorise events into different selection regions. The final output of VLQAnalysis are most commonly histograms, 
but one can also choose to produce mini-ntuples instead or in addition. The package also provides macros to merge and 
further process output files to prepare them as inputs to the TRexFitter fitting package.

Class structure:
  - VLQ_Analysis_Data2015: Main steering class 
  - VLQ_Options: Class containing all command-line options. [derived from IFAETopFramework::OptionsBase]
  - VLQ_NtupleData: Class holding branch information of input tree 
  - VLQ_NtupleReader: Class in which rules for input tree reading are defined
  - VLQ_OutputData: Class holding all variables that will be used across analysis classes and those to be written to output histogram/tree
  - VLQ_Selector: Class defining selection regions 
  - VLQ_WeightManager: Class to manage nominal and systematic event-level weight computation
  - VLQ_TRFManager: Interface class to BTaggingTruthTaggingTool
  - VLQ_AnalysisTools: Class in which object-level selections are applied. Also handles steering of code to calculate event-level observables
  - VLQ_VariableComputer: Class which calculates event-level observables
  - VLQ_ResonanceMaker: Class that reconstructs resonances such as leptonic W, top or VLQ
  - VLQ_KinReweighter: Class that applies kinematic reweightings to fix data/MC disagreements
  - VLQ_TtbarSystematicsManager: Class that adds parametrised ttbar modelling uncertainties as weight variations
  - VLQ_PropagateSingleTopSystematics: Class that propagates single-top modelling uncertainties as weight variations


To run a full analysis, the following steps must be taken:
  1. Produce a `sample_info.dat` file with the cross section and sum of weights of all DSIDs to be processed
     
     a. Make a json file containing the cross section information with `MakeXSecFile.py`
     Only for this part, you would need a different setup than for the rest of the code:
     ```  
     cd build
     setupATLAS && lsetup pyami 
     voms-proxy-init -voms atlas
     acmSetup AthAnalysis,21.2,latest
     asetup --restore
     source */setup.sh     
     ```

     Then, run	this from `src/VLQAnalysis/python`
     ```
     MakeXSecFile.py --listProc <list of processes> --xsecFile <name of output file> --procDir <directory containing process lists>
     ```  
     Typically, the ```procDir``` would be the ```MBJ_Analysis/samples/htx/``` directory. The code will look for dataset IDs 
     from the mc16a.<proc>.list files for each process in the comma-separated list in ```--listProc```. The output of this code 
     is the json file with name specified in ```--xsecFile``` 
    
     **NOTE** : Since the cross section information will be the same for all MC campaigns, this step can be run with only the mc16a files.

     b. Run `ProduceSampleDAT_mc.py` to find sum of weights from ntuple files, and read xsec information from the json file
     ```
     ProduceSampleDAT_mc.py --inputDir <directory containing MBJ ntuples> --campaign <MC campaign of files> --configName <name of output>
     --xsecFile <cross section json file>
     ```
	NOTE: This file can be made separately for each campaign, or all campaigns can be combined by omitting the ``--campaign`` option

  2. Locally test VLQAnalysis. An example line of code to execute is:
       ```
       VLQAna --textFileList=false --outputFile=test_tag21.2.67.410470.sl.root \
       --inputFile=/nfs/at3/scratch2/farooque/MBJOutputs/tag-21.2.67-0-fJvt/DL1/user.farooque.410470.mc16a.ttbar.DAOD_TOPQ1.e6337_s3126_r9364_p3832.21.2.67-htztx-0-fJvt-DL1_output_tree.root/user.farooque.18837558._000001.output_tree.root \
       --sampleName=ttbar --sampleID=410470.mc16a \
       --sampleDat=../data/samples_info_tag-21.2.67-htztx-0-MV2.mc16a.dat \
       --useLeptonsSF=true --filterType=NOFILTER \
       --doTRF=false --recomputeTRF=false \
       --recomputeBtagSF=false \
       --TRFCDI=xAODBTaggingEfficiency/13TeV/2017-21-13TeV-MC16-CDI-2019-07-30_v1.root \
       --otherVariables=false --btagOP=FixedCutBEff_77 --doBlind=false \
       --applyTtbbCorrection=true --SCALETTBARHTSLICES=false --splitVLQDecays=false \
       --dumpHistos=true --dumpOverlapTree=false --dumpTree=false --applyMetMtwCuts=true --invertMetMtwCuts=false \
       --doTruthAnalysis=false --uselargeRjets=false \
       --doOneLeptonAna=true --doTwoLeptonAna=false --doZeroLeptonAna=false \
       --doExclusiveJetRegions=true --doLowBRegions=false --DOLOWJREGIONS=false --DOSPLITEMU=true --DOSPLITMTB=false \
       --DOFITREGIONS=false --DOVALIDNREGIONS=false --doPreselection=true --doExtendedPreselection=false \
       --doSingleVLQRegions=true --doPairVLQRegions=true \
       --doOldBoost=false\
       --useLeptonTrigger=true --useMETTriggerOneLep=false --useMETTrigger=true \
       --jetPtCut=25 --fwdJetPtCut=25 --RCJetPtCut=200 --RCNSUBJETSCUT=2 --applydeltaphicut=true --invertdeltaphicut=false \
       --applyTtbbCorrection=false --APPLYTTBARNNLOCORRECTION=true --APPLYVJETSSHERPA22RW=true --computeWeightSyst=false \
       --ONLYDUMPSYSTHISTOGRAMS=false --inputTree=nominal --doRecoVLQ=single \
       --leptopOpt=VETO_RCMATCH --btagCollection=TRACK --trkJetPtCut=20.

  3. Launch jobs on the batch:

     The script `SubmitVLQAnalysis_new.py` creates lists of input files to submit to the batch system, as specified by the option `NFILESSPLIT`. 
     The option `NMERGE` can also be used to merge jobs in the same submission script. This is useful, for example, when one wants to run several 
     object systematics.
     All options for VLQAnalysis can be passed to `SubmitVLQAnalysis_new.py` through the command line. The samples to be submitted can be controlled 
     through additional options such as `RUNDATA`, `RUNTTBAR`, `RUNOTHERS`, etc. (See the file for details).
     The script currently supports submission on HTCondor batch systems or PBS-type batch systems, including lxbatch.

  4. Check whether all jobs succeeded on batch system and relaunch as needed with `CheckOutputs.py`

  5. Merge output files by process. These merged files can then be used as inputs to a **likelihood fit** (usually with **TRExFitter**) 
     or for **plots** (usually with **IFAEPlotter**)

The VLQAnalysis class provides utility macros to prepare configuration files and input files for TRexFitter. Follow the steps below 
to prepare and launch a fit:
  1. Prepare configuration files for the fit
  2. Use `LaunchTRexFitteronBatch.py` to launch the fit
  3. Inspect the output
  4. Make Brazilian limit plots with `DumpLimitPlots.py`


