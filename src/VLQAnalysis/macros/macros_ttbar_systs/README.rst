Producing the inputs
============

The inputs for the systematics determination are produced using the following command::

  python Submit_VLQAnalysis_new.py dryrun=false --useLeptonsSF=true --filterType=APPLYFILTER --doTRF=true --recomputeTRF=false --otherVariables=false --btagOP=77 --doBlind=false --applyTtbbCorrection=true --SCALETTBARHTSLICES=false --computeWeightSys=false --splitVLQDecays=true --dumpHistos=true --dumpTree=false --applyMetMtwCuts=true --invertMetMtwCuts=false --doTruthAnalysis=false --uselargeRjets=false --doOneLeptonAna=true --doZeroLeptonAna=true --doLowBRegions=false --DOLOWJREGIONS=false --DOSPLITEMU=false --DOFITREGIONS=true --useLeptonTrigger=true --useMETTrigger=true --jetPtCut=25 --RCJetPtCut=200 --RCNSUBJETSCUT=2 --applydeltaphicut=true --invertdeltaphicut=false --inputDir=/nfs/at3/scratch2/lvalery/VLQFiles/MBJ-2.4.28-4/ --sampleDat=samples_info_MBJ-2.4.28-4-ttbarSyst.dat producetarball=true --APPLYTTBARNNLOCORRECTION=true --APPLYVJETSSHERPA22RW=true --sleep=0 --OUTPUTDIRSUFFIX=InputsTtbarSysts_NOW --RWTTFRACTIONS=true --USEPUWEIGHT=false --onlyDumpSystHistograms=true --applyMetSigCut=true

Once checked that all outputs are there. You can merge them to be used by the
ttbar systematics tool::

  for ttbartype in light cc bb; do
    for syst in "" "PowHer" "radHi" "radLow" "aMCHer"; do
      hadd ttbar$ttbartype$syst.root <input_folder>/outVLQAnalysis_ttbar$ttbartype${syst}_*.root;
    done;
  done

where ``input_folder`` is the folder contianing the outputs of the previous command.

Treating the inputs
============

You can then execute the ``dottbarSystAsWeights.py`` script in the following way::

  python dottbarSystAsWeights.py -i <input_folder> -o <output_folder> -t <threshold> -m -w -a

where

  * ``<input_folder>`` is the place where the merged files are stored
  * ``<output_folder>`` is the place where you want the output root file to be stored
  * ``<threshold`` is the threshold for MC stat uncertainty allowed in each bin (otherwise, rebinning is done)
  * ``-m`` to merge regions following the definition on the top of the code
  * ``-w`` dump some warning text (if relative uncertainty is large)
  * ``-a`` run over all regions 
