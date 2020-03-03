Prepare configuration files
=====================================

The macro ``PrepareConfigFilesFromTemplate.py`` dumps configuration files based on the templates in ``templates/`` folder.

Example call to this macro::

  python PrepareConfigFilesFromTemplate.py inputTemplate=/nfs/pic.es/user/l/lvalery/AnalysisCode/VLQAnalysis/macros/macros_stats/templates/TEMPLATE_configFile__SIGNAL_.txt
  outputFolder=/nfs/pic.es/user/l/lvalery/scratch2/VLQFitOutputs/test/
  inputDir=/nfs/atlas-data07/lvalery/VLQFitOutputs/2017_01_12_ttbar/InputFiles/
  addition="" FITBLIND=TRUE signal=ONE LIMITBLIND=TRUE FITTYPE=BONLY
  LUMIVALUE=36470.16
  systConfig=/nfs/pic.es/user/l/lvalery/AnalysisCode/VLQAnalysis/macros/macros_stats/templates/TEMPLATE_Systematics.txt
  USEDATA=FALSE DOONELEP=TRUE DOVR=FALSE useBlindingCuts=TRUE

The arguments are defined as follows:

  * ``inputTemplate``: the template file to be used for building the total config files
  * ``outputFolder``: place where the resulting config files are stored
  * ``inputDir``: place where the TRexFitter input rootfiles are stored
  * ``addition``: any additional information to be kept in the name of the folder
  * ``fitBlind``: is the fit blinded ? [TRUE/FALSE]
  * ``signal``: ONE (dump one config file per signal) ==> use should use that
  * ``limitBlind``: is the limit blinded ?   [TRUE/FALSE]
  * ``fitType``: `BONLY` or `SPLUSB`
  * ``lumiValue``: the value of the luminosity (defined in pb-1)
  * ``systConfig``: full path to the template for systematic uncertainties
  * ``useData``: adds the Data sample in the config file
  * ``doOneLep``: includes the 1-lepton regions
  * ``doZeroLep``: includes the 0-lepton regions
  * ``doVR``: includes the validation regions (of the channels chosen above)
  * ``useBlindingCuts``: adapts the meff binning with blinding cuts instead of the full range

Launching the TRExFitter on the batch
=====================================

The macro ``LaunchTRExFitterOnBatch.py`` allows to launch the TRExFitter jobs on thebatch (for now, only at3 batch though)::

  python LaunchTRExFitterOnBatch.py tarball=$VLQAnalysisFramework_DIR/../VLQAnalysis/macros/macros_stats/TRExFitter.tgz outputDir=/nfs/atlas-data07/lvalery/VLQFitOutputs/2016_08_23_Unblinding_FinalFiles/Limits/Results/ action=fitpreplotspostplotslimits inputDir=/nfs/pic.es/user/l/lvalery/scratch2/VLQFitOutputs/2016_08_23_Unblinding_FinalFiles/Limits/ConfigFiles/configFile queue=at3

Doing the limit plot
=====================================

Once the jobs are finished, you can build the limit plots based on your outputs::

  python DumpLimitPlot.py -i /nfs/atlas-data07/lvalery/VLQFitOutputs/2016_08_23_Unblinding_FinalFiles/Limits/Results/ -o LimitPlots/ -s TTD -d -l 13.2

Dumping the results of the 2D scan
=====================================

Assuming you launched all the jobs for the 2D scan, you can reover all the results in one signle text file, used as input for the plotting macro::

  python Dump2DResults.py -i ~/scratch2/VLQFitOutputs/2016_08_23_Unblinding_FinalFiles/Results_2DBR_Combi/ -l 13.2 -d -o Results2D/ResultsComb.txt
