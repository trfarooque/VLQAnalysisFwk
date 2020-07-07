#!/bin/bash

dirBase=/nfs/at3/scratch2/tvdaalen/TRExFitter_WorkArea/Note/FullFit/ST_tt+Wt_Tests/
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=prefit #fitpreplotspostplots

# inDirs=(" BlindedDataFit_allSyst Asimov_BONLY_Blind_Jun4 Asimov_BONLY_Jun4 ") #BlindedDataFit_allSyst
# inDirs=(" Asimov_BONLY_Template_prefit_Jun4 ") #BlindedDataFit_allSyst

# inDirs=(" TtbarDebug_Asimov_TtStOnly_prefit_HF+PMGcor ") #BlindedDataFit_allSyst

inDirs=(" tt+Wt_combined_RW_ModOnly_preselFineBinning_Jul2 tt+Wt_combined_RW_ModOnly_FullSim_REF_GENPS_preselFineBinning_Jul2 ") #tt+Wt_combined_untreated_NORW_templates_Jul2 ")
for dir in ${inDirs[@]}
do

    fitinputDir="${dirBase}/${dir}/configFile_"
    fitoutputDir="${dirBase}/${dir}/Results/"

    python LaunchTRExFitterOnBatch_newTRexF.py \
        tarball=${tarpath} \
        action=${fitactions} \
        inputDir=${fitinputDir} outputDir=${fitoutputDir}/ \
        queue=at3_short
done