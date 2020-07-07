#!/bin/bash

dirBase=/nfs/at3/scratch2/tvdaalen/TRExFitter_WorkArea/Note/FullFit/FixedYieldTables_Jun18Fits/ChangedGhostSampleTitles/
tarpath=/nfs/at3/scratch2/farooque/TRexFitterArea/TtHFitter-00-04-08/TRExFitter/TRExFitter-00-04-08.tgz
fitactions=fitpreplotspostplots

inDirs=(" Asimov_BONLY Asimov_BONLY_Blind Asimov_BONLY_mu0 BlindedDataFit BlindedDataFit_mu0 ")

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