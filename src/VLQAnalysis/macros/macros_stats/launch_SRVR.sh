#!/bin/bash

dirBase=/nfs/at3/scratch2/tvdaalen/TRExFitter_WorkArea/Note/SRVR_Split/
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=fitpreplotspostplotslimits

# inDirs=(" BlindedDataFit_allSyst Asimov_BONLY_Blind_Jun4 Asimov_BONLY_Jun4 ") #BlindedDataFit_allSyst
# inDirs=(" Asimov_BONLY_Template_prefit_Jun4 ") #BlindedDataFit_allSyst

# inDirs=(" TtbarDebug_Asimov_TtStOnly_prefit_HF+PMGcor ") #BlindedDataFit_allSyst
# inDirs=(" Asimov_BONLY_HF+PMGcor_Jun18 Asimov_BONLY_HF+PMGcor_Blind_Jun18 BlindedDataFit_HF+PMGcor_Jun18 ")

inDirs=(" StatOnly_SR_Jun25 ")

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
